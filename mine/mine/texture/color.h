//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

namespace mine {
    struct Color {
        static simd::float4 defaultNormalMap() {
            simd::float3 rgb = (simd::make_float3(0.0f, 0.0f, 1.0f) + 1.0f) / 2.0f;
            return simd::make_float4(rgb, 1.0f);
        }
    };
}
