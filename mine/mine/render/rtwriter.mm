//
//  rtwriter.cpp
//
//  Created on 06/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <chrono>
#include <algorithm>
#include <spdlog/spdlog.h>

#include "rtwriter.h"

mine::RTWriter::RTWriter(Config const & config)
: cgbitmap(config.width,config.height, 4)
, config{config} {
    queue = [[NSOperationQueue alloc] init];
    queue.maxConcurrentOperationCount = config.threads;
}

mine::RTWriter::~RTWriter() {
    [queue release];
}

void mine::RTWriter::captureRegion(Region<uint16_t> const & region,
                                   Scene const & scene,
                                   uint16_t iteration) {
    for (uint16_t x = region.x.lowerBound; x <= region.x.higherBound; x++) {
        for (uint16_t y = region.y.lowerBound; y <= region.y.higherBound; y++) {
            Metadata meta(x, y);
            Ray ray = scene.camera.ray(x,
                                       y,
                                       cgbitmap.bitmap.width,
                                       cgbitmap.bitmap.height);
            simd_float4 color = rt.trace(ray,
                                         scene,
                                         config,
                                         config.depth,
                                         meta);
            uint16_t flippedY = cgbitmap.bitmap.height - y - 1;
            simd_float4 currentColor = cgbitmap.bitmap.colorAt(x, flippedY);
            simd_float4 newColor = (currentColor * iteration + color) / float(iteration + 1);
            cgbitmap.bitmap.setNormalizedRGBA(x, cgbitmap.bitmap.height - y - 1, newColor);
        }
    }
}

void mine::RTWriter::capture(Scene const & scene) {
    std::vector<Region<uint16_t>> regions = randomizedRegions();
    
    for (uint16_t iteration = 0; iteration < config.raysPerPixel; ++iteration) {
        std::chrono::time_point start = std::chrono::high_resolution_clock::now();
        for (Region<uint16_t> const & region : regions) {
            [queue addOperationWithBlock:^{
                captureRegion(region, scene, iteration);
            }];
        }
        [queue waitUntilAllOperationsAreFinished];
        std::chrono::time_point end = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::milliseconds>(end - start);
        spdlog::info("Time taken for an iteration {0} ms", duration.count());
    }
}

void mine::RTWriter::capturePixel(Scene const & scene,
                                  simd::float2 const & coordinate) {
    assert(coordinate.x >= 0 && coordinate.x <= 1 && "Must be in [0, 1]");
    assert(coordinate.y >= 0 && coordinate.y <= 1 && "Must be in [0, 1]");
    uint16_t x = coordinate.x * cgbitmap.bitmap.width;
    uint16_t y = coordinate.y * cgbitmap.bitmap.height;
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
    result.reserve((cgbitmap.bitmap.width / config.regionSide) *
                   (cgbitmap.bitmap.height / config.regionSide));
    for (uint16_t x = 0; x < cgbitmap.bitmap.width; x += config.regionSide) {
        for (uint16_t y = 0; y < cgbitmap.bitmap.height; y += config.regionSide) {
            uint16_t xMax = static_cast<uint16_t>(std::min(static_cast<uint16_t>(x + config.regionSide - 1),
                                                           static_cast<uint16_t>(cgbitmap.bitmap.width - 1)));
            ClosedRange<uint16_t> xRange{x, xMax};
            uint16_t yMax = static_cast<uint16_t>(std::min(static_cast<uint16_t>(y + config.regionSide - 1),
                                                           static_cast<uint16_t>(cgbitmap.bitmap.height - 1)));
            ClosedRange<uint16_t> yRange{y, yMax};
            result.push_back(Region<uint16_t>{xRange, yRange});
        }
    }
    
    return result;
}
