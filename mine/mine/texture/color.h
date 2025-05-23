//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

namespace mine {
    inline simd::float4 defaultNormalMapColor() {
        simd::float3 rgb = (simd_make_float3(0.0f, 0.0f, 1.0f) + 1.0f) / 2.0f;
        simd::float4 rgba = simd_make_float4(rgb, 1.0f);
        return rgba;
    }
}
