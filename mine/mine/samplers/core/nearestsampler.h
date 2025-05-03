//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "sampler.h"
#include "../../assertion/range.h"
#include "../../texture/bitmap.h"

namespace mine {
    class NearestSampler: public Sampler {
    public:
        simd::float4 sample(float u, float v, const mine::Bitmap & texture) override {
            assertInClosedRange(u, 0.0f, 1.0f);
            assertInClosedRange(v, 0.0f, 1.0f);
            
            uint16_t x = (texture.width - 1) * u;
            uint16_t y = (texture.height - 1) * v;
            
            return texture.colorAt(x, y);
        }
    };
}
