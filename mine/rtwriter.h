//
//  rtwriter.h
//
//  Created on 20/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#import "cgbitmap.h"
#import "raytracer.h"

#include <cstdint>

struct RTWriter {
    RTWriter(uint16_t width, uint16_t height) : cgbitmap(1000, 1000, 4) {
        
    }
    
    void capture(Scene const & scene) {
        int raysPerPixel = 5;
        for (int i = 0; i < raysPerPixel; ++i) {
            uint16_t width = cgbitmap.bitmap.width;
            uint16_t height = cgbitmap.bitmap.height;
            for (uint16_t y = 0; y < height; y++) {
                for (uint16_t x = 0; x < width; x++) {
                    
                    Ray ray = scene.camera.ray(x, y, width, height);
                    simd_float4 color = rt.trace(ray, scene, 10);
                    
                    
                    simd_float4 currentColor = cgbitmap.bitmap.colorAt(x, height - y - 1);
                    simd_float4 newColor = (currentColor * i + color) / float(i + 1);
                    cgbitmap.bitmap.setNormalizedRGBA(x, height - y - 1, newColor);
                }
            }
        }
    }
    
    RayTracer rt;
    CGBitmap cgbitmap;
};
