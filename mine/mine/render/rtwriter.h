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

#include "../config.h"
#import "../texture/cgbitmap.h"
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


namespace mine {
    struct RTWriter {
        RTWriter(Config const & config)
        : cgbitmap(config.width,config.height, 4)
        , config{config} {
            queue = [[NSOperationQueue alloc] init];
            queue.maxConcurrentOperationCount = config.threads;
        }
        ~RTWriter() {
            [queue release];
        }
            
        void capture(Scene const & scene) {
            std::vector<Region<uint16_t>> regions = randomizedRegions();
            
            for (int i = 0; i < config.raysPerPixel; ++i) {
                for (Region<uint16_t> const & region : regions) {
                    [queue addOperationWithBlock:^{
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
        std::vector<Region<uint16_t>> randomizedRegions() {
            std::vector<Region<uint16_t>> regions = divideIntoRegions();
            std::random_device rd;
            std::default_random_engine rng(rd());
            std::shuffle(regions.begin(), regions.end(), rng);
            return regions;
        }
        std::vector<Region<uint16_t>> divideIntoRegions() {
            std::vector<Region<uint16_t>> result{};
            
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
        
        RayTracer rt;
        Config config;
        NSOperationQueue * queue;
    };
}
