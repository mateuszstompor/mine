//
//  linearsampler.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../texture/bitmap.h"

class LinearSampler {
public:
    simd::float4 sample(double u, double v, const mine::Bitmap& texture) {
        float x = u * (texture.width - 1);
        float y = v * (texture.height - 1);

        float fX = std::floor(x);
        float cX = std::ceil(x);
        float fY = std::floor(y);
        float cY = std::ceil(y);
        
        auto p1 = texture.colorAt(fX, fY);
        auto p2 = texture.colorAt(cX, fY);
        
        auto p3 = texture.colorAt(fX, cY);
        auto p4 = texture.colorAt(cX, cY);
        
        float xFrac = x - fX;
        float yFrac = y - fY;
        
        simd::float4 f1 = simd::lerp(p1, p2, simd::float4(xFrac));
        simd::float4 f2 = simd::lerp(p3, p4, simd::float4(xFrac));
        simd::float4 f3 = simd::lerp(f1, f2, simd::float4(yFrac));

        return f3;
    }
};
