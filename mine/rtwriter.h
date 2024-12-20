//
//  rtwriter.h
//
//  Created on 20/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <Foundation/Foundation.h>

#include <vector>
#include <cstdint>
#include <algorithm>
#include <random>

#import "cgbitmap.h"
#import "raytracer.h"

template <typename T>
struct ClosedRange {
    T lowerBound;
    T higherBound;
};

template <typename T>
struct Region {
    ClosedRange<T> x;
    ClosedRange<T> y;
};


struct RTWriter {
    RTWriter(uint16_t width, uint16_t height, uint16_t regionSideSize = 100)
    : cgbitmap(1000, 1000, 4)
    , regionSideSize{regionSideSize} {
        queue = [[NSOperationQueue alloc] init];
        queue.maxConcurrentOperationCount = 8;
    }
    ~RTWriter() {
        [queue release];
    }
        
    void capture(Scene const & scene) {
        std::vector<Region<uint16_t>> regions = divideIntoRegions();
        std::random_device rd;
        std::default_random_engine rng(rd());
        std::shuffle(regions.begin(), regions.end(), rng);
        
        int raysPerPixel = 100;
        for (int i = 0; i < raysPerPixel; ++i) {
            for (Region<uint16_t> const & region : regions) {
                [queue addOperationWithBlock:^{
                    for (uint16_t x = region.x.lowerBound; x <= region.x.higherBound; x++) {
                        for (uint16_t y = region.y.lowerBound; y <= region.y.higherBound; y++) {
                            
                            Ray ray = scene.camera.ray(x,
                                                       y,
                                                       cgbitmap.bitmap.width,
                                                       cgbitmap.bitmap.height);
                            simd_float4 color = rt.trace(ray, scene, 2);
                            
                            simd_float4 currentColor = cgbitmap.bitmap.colorAt(x, cgbitmap.bitmap.height - y - 1);
                            simd_float4 newColor = (currentColor * i + color) / float(i + 1);
                            cgbitmap.bitmap.setNormalizedRGBA(x, cgbitmap.bitmap.height - y - 1, newColor);
                        }
                    }
                }];
            }
            [queue waitUntilAllOperationsAreFinished];
        }
    }
    CGBitmap cgbitmap;
    
private:
    std::vector<Region<uint16_t>> divideIntoRegions() {
        std::vector<Region<uint16_t>> result{};
        
        for (uint16_t x = 0; x < cgbitmap.bitmap.width; x += regionSideSize) {
            for (uint16_t y = 0; y < cgbitmap.bitmap.height; y += regionSideSize) {
                uint16_t xMax = static_cast<uint16_t>(std::min(static_cast<uint16_t>(x + regionSideSize - 1),
                                                               static_cast<uint16_t>(cgbitmap.bitmap.width - 1)));
                ClosedRange<uint16_t> xRange{x, xMax};
                uint16_t yMax = static_cast<uint16_t>(std::min(static_cast<uint16_t>(y + regionSideSize - 1),
                                                               static_cast<uint16_t>(cgbitmap.bitmap.height - 1)));
                ClosedRange<uint16_t> yRange{y, yMax};
                result.push_back(Region<uint16_t>{xRange, yRange});
            }
        }
        
        return result;
    }
    
    RayTracer rt;
    uint16_t regionSideSize;
    NSOperationQueue * queue;
};
