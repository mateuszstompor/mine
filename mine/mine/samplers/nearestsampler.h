//
//  nearestsampler.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "bitmap.h"

class NearestSampler {
public:
    simd::float4 sample(float u, float v, const Bitmap & texture) {
        u = simd::clamp(u, 0.0f, 1.0f);
        v = simd::clamp(v, 0.0f, 1.0f);
        
        float normalizedU = simd::clamp(u, 0.0f, 1.0f);
        float normalizedV = simd::clamp(v, 0.0f, 1.0f);
        
        uint16_t x = (texture.width - 1) * normalizedU;
        uint16_t y = (texture.height - 1) * normalizedV;
        
        return texture.colorAt(x, y);
    }
};
