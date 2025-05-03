//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../../texture/bitmap.h"
#include "../../assertion/range.h"

namespace mine {
    class LinearSampler: public Sampler {
    public:
        simd::float4 sample(float u, float v, mine::Bitmap const & texture) override {
            assertInClosedRange(u, 0.0f, 1.0f);
            assertInClosedRange(v, 0.0f, 1.0f);
            float x = u * (texture.width - 1);
            float y = v * (texture.height - 1);

            float fX = std::floor(x);
            float cX = std::ceil(x);
            float fY = std::floor(y);
            float cY = std::ceil(y);
            
            simd::float4 p1 = texture.colorAt(fX, fY);
            simd::float4 p2 = texture.colorAt(cX, fY);
            
            simd::float4 p3 = texture.colorAt(fX, cY);
            simd::float4 p4 = texture.colorAt(cX, cY);
            
            float xFrac = x - fX;
            float yFrac = y - fY;
            
            simd::float4 f1 = simd::lerp(p1, p2, simd::float4(xFrac));
            simd::float4 f2 = simd::lerp(p3, p4, simd::float4(xFrac));
            simd::float4 f3 = simd::lerp(f1, f2, simd::float4(yFrac));

            return f3;
        }
    };
}
