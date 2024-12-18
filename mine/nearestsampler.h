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
        assert(u < 1);
        assert(v < 1);
        uint16_t x = texture.width * u;
        uint16_t y = texture.height * v;
        
        return texture.colorAt(x, y);
    }
};
