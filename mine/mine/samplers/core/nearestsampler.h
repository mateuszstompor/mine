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
        simd::float4 sample(simd::float2 const & uv, mine::Bitmap const * texture) override {
            assertInClosedRange(uv.x, 0.0f, 1.0f);
            assertInClosedRange(uv.y, 0.0f, 1.0f);
            
            uint16_t x = (texture->width - 1) * uv.x;
            uint16_t y = (texture->height - 1) * uv.y;
            
            return texture->get(x, y);
        }
    };
}
