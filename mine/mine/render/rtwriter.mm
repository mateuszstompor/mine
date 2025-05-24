//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <chrono>
#include <algorithm>
#include <spdlog/spdlog.h>
#include <OpenImageDenoise/oidn.hpp>

#include "../assertion/finite.h"
#include "../scene/look/bitmaploader.h"
#include "rtwriter.h"

mine::RTWriter::RTWriter(Config const & config)
: accumulator(config.width, config.height)
, normals(config.width, config.height)
, albedo(config.width, config.height)
, config{config} {
    queue = [[NSOperationQueue alloc] init];
    queue.maxConcurrentOperationCount = config.threads;
}

mine::RTWriter::~RTWriter() {
    [queue release];
}

void mine::RTWriter::captureRegion(Region<uint16_t> const & region,
                                   Scene & scene,
                                   uint32_t iteration) {
    for (uint16_t x = region.x.lowerBound; x <= region.x.higherBound; x++) {
        for (uint16_t y = region.y.lowerBound; y <= region.y.higherBound; y++) {
            Metadata meta(x, y);
            Ray ray = scene.camera.ray(x,
                                       y,
                                       accumulator.width,
                                       accumulator.height);
            simd_float3 color = rt.trace(ray,
                                         scene,
                                         config,
                                         config.depth,
                                         meta);
            assertFinite(color);
            simd::float3 clampedColor = simd::clamp(color,
                                                    simd_make_float3(0, 0, 0),
                                                    simd_make_float3(1, 1, 1));
            simd::float4 clampedRGBA = simd_make_float4(clampedColor, 1.0f);
            simd_float4 currentColor = accumulator.get(x, y);
            simd_float4 newColor = (currentColor * iteration + clampedRGBA) / float(iteration + 1);
            accumulator.set(x, y, newColor);
        }
    }
}

void mine::RTWriter::captureRegionNormals(Region<uint16_t> const & region,
                                          Scene & scene,
                                          uint32_t iteration) {
    for (uint16_t x = region.x.lowerBound; x <= region.x.higherBound; x++) {
        for (uint16_t y = region.y.lowerBound; y <= region.y.higherBound; y++) {
            Metadata meta(x, y);
            Ray ray = scene.camera.ray(x,
                                       y,
                                       accumulator.width,
                                       accumulator.height);
            simd_float3 color = rt.traceNormal(ray,
                                         scene,
                                         config,
                                         config.depth,
                                         meta);
            assertFinite(color);
            simd::float3 clampedColor = simd::clamp(color,
                                                    simd_make_float3(0, 0, 0),
                                                    simd_make_float3(1, 1, 1));
            simd::float4 clampedRGBA = simd_make_float4(clampedColor, 1.0f);
            simd_float4 currentColor = normals.get(x, y);
            simd_float4 newColor = (currentColor * iteration + clampedRGBA) / float(iteration + 1);
            normals.set(x, y, newColor);
        }
    }
}

void mine::RTWriter::captureRegionAlbedo(Region<uint16_t> const & region,
                                          Scene & scene,
                                          uint32_t iteration) {
    for (uint16_t x = region.x.lowerBound; x <= region.x.higherBound; x++) {
        for (uint16_t y = region.y.lowerBound; y <= region.y.higherBound; y++) {
            Metadata meta(x, y);
            Ray ray = scene.camera.ray(x,
                                       y,
                                       accumulator.width,
                                       accumulator.height);
            simd_float3 color = rt.traceAlbedo(ray,
                                         scene,
                                         config,
                                         config.depth,
                                         meta);
            assertFinite(color);
            simd::float3 clampedColor = simd::clamp(color,
                                                    simd_make_float3(0, 0, 0),
                                                    simd_make_float3(1, 1, 1));
            simd::float4 clampedRGBA = simd_make_float4(clampedColor, 1.0f);
            simd_float4 currentColor = albedo.get(x, y);
            simd_float4 newColor = (currentColor * iteration + clampedRGBA) / float(iteration + 1);
            albedo.set(x, y, newColor);
        }
    }
}

void mine::RTWriter::capture(Scene & scene) {
    std::vector<Region<uint16_t>> regions = randomizedRegions();
    
    for (uint32_t iteration = 0; iteration < config.raysPerPixel; ++iteration) {
        std::chrono::time_point start = std::chrono::high_resolution_clock::now();
        for (Region<uint16_t> const & region : regions) {
            [queue addOperationWithBlock:^{
                captureRegion(region, scene, iteration);
            }];
        }
        [queue waitUntilAllOperationsAreFinished];
        std::chrono::time_point end = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::milliseconds>(end - start);
        spdlog::info("Time taken for an iteration {0} ms, iteration: {1}", duration.count(), iteration);
        
        BitmapConverter converter;
        RGBAUint8Bitmap saveableImage = converter.convert(accumulator);
        BitmapLoader::dumpScreenshot(saveableImage, iteration);
    }
    
    {
        spdlog::info("Capturing normals...");
        for (Region<uint16_t> const & region : regions) {
            [queue addOperationWithBlock:^{
                captureRegionNormals(region, scene, 0);
            }];
        }
        [queue waitUntilAllOperationsAreFinished];
    }
    
    {
        spdlog::info("Capturing albedo...");
        for (Region<uint16_t> const & region : regions) {
            [queue addOperationWithBlock:^{
                captureRegionAlbedo(region, scene, 0);
            }];
        }
        [queue waitUntilAllOperationsAreFinished];
    }
    
    spdlog::info("Denoising...");
    BitmapConverter converter;
    
    RGBFloat32Bitmap accumulatorRGB = converter.dropAlpha(accumulator);
    RGBFloat32Bitmap normalsRGB = converter.dropAlpha(normals);
    RGBFloat32Bitmap albedoRGB = converter.dropAlpha(albedo);
    
    oidn::DeviceRef device = oidn::newDevice(oidn::DeviceType::CPU);
    device.commit();


    oidn::BufferRef colorBuf  = device.newBuffer(accumulator.width * accumulator.height * 3 * sizeof(float));
    oidn::BufferRef normalsBuf  = device.newBuffer(normals.width * normals.height * 3 * sizeof(float));
    oidn::BufferRef albedoBuf  = device.newBuffer(albedo.width * albedo.height * 3 * sizeof(float));
    
    
    
    float * colorPtr = (float*)colorBuf.getData();
    std::memcpy(colorPtr, accumulatorRGB.data.data(), accumulator.width * accumulator.height * 3 * sizeof(float));
    
    float * normalPtr = (float*)normalsBuf.getData();
    std::memcpy(normalPtr, normalsRGB.data.data(), normalsRGB.width * normalsRGB.height * 3 * sizeof(float));
    
    float * albedoPtr = (float*)albedoBuf.getData();
    std::memcpy(albedoPtr, albedoRGB.data.data(), albedoRGB.width * albedoRGB.height * 3 * sizeof(float));
    
    
    oidn::FilterRef filter = device.newFilter("RT"); // generic ray tracing filter
    filter.setImage("color",  colorBuf,  oidn::Format::Float3, accumulator.width, accumulator.height); // beauty
    filter.setImage("output", colorBuf,  oidn::Format::Float3, accumulator.width, accumulator.height); // denoised beauty
    filter.setImage("normal", normalsBuf,  oidn::Format::Float3, accumulator.width, accumulator.height); // denoised beauty
    filter.setImage("albedo", albedoBuf,  oidn::Format::Float3, accumulator.width, accumulator.height); // denoised beauty

    filter.set("hdr", true); // beauty image is HDR
    filter.commit();


    filter.execute();

    const char* errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None) {
        std::cout << "Error: " << errorMessage << std::endl;
    }
    
    colorPtr = (float*)colorBuf.getData();
    std::memcpy(accumulatorRGB.data.data(), colorPtr, accumulator.width * accumulator.height * 3 * sizeof(float));
    RGBAFloat32Bitmap finalImage = converter.extendAlpha(accumulatorRGB);
    accumulator = finalImage;
    RGBAUint8Bitmap saveableImage = converter.convert(finalImage);
    RGBAUint8Bitmap saveableNormals = converter.convert(normals);
    RGBAUint8Bitmap saveableAlbedo = converter.convert(albedo);
    
    BitmapLoader::dumpScreenshot(saveableImage, config.raysPerPixel);
    BitmapLoader::dumpScreenshot(saveableNormals, config.raysPerPixel + 1);
    BitmapLoader::dumpScreenshot(saveableAlbedo, config.raysPerPixel + 2);
}

void mine::RTWriter::capturePixel(Scene & scene,
                                  simd::float2 const & coordinate) {
    assert(coordinate.x >= 0 && coordinate.x <= 1 && "Must be in [0, 1]");
    assert(coordinate.y >= 0 && coordinate.y <= 1 && "Must be in [0, 1]");
    uint16_t x = coordinate.x * accumulator.width;
    uint16_t y = coordinate.y * accumulator.height;
    Region<uint16_t> region {ClosedRange<uint16_t>{x, x},
                             ClosedRange<uint16_t>{y, y}};
    [queue addOperationWithBlock:^{
        captureRegion(region, scene, 1);
    }];
    [queue waitUntilAllOperationsAreFinished];
}

std::vector<mine::Region<uint16_t>> mine::RTWriter::randomizedRegions() {
    std::vector<Region<uint16_t>> regions = divideIntoRegions();
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(regions.begin(), regions.end(), rng);
    return regions;
}

std::vector<mine::Region<uint16_t>> mine::RTWriter::divideIntoRegions() {
    std::vector<Region<uint16_t>> result{};
    result.reserve((accumulator.width / config.regionSide) *
                   (accumulator.height / config.regionSide));
    for (uint16_t x = 0; x < accumulator.width; x += config.regionSide) {
        for (uint16_t y = 0; y < accumulator.height; y += config.regionSide) {
            uint16_t xMax = static_cast<uint16_t>(std::min(static_cast<uint16_t>(x + config.regionSide - 1),
                                                           static_cast<uint16_t>(accumulator.width - 1)));
            ClosedRange<uint16_t> xRange{x, xMax};
            uint16_t yMax = static_cast<uint16_t>(std::min(static_cast<uint16_t>(y + config.regionSide - 1),
                                                           static_cast<uint16_t>(accumulator.height - 1)));
            ClosedRange<uint16_t> yRange{y, yMax};
            result.push_back(Region<uint16_t>{xRange, yRange});
        }
    }
    
    return result;
}

mine::CGBitmap mine::RTWriter::getBitmap() {
    BitmapConverter converter;
    return CGBitmap(converter.convert(accumulator));
}
