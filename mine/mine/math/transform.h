//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

namespace mine {
    inline simd::float4x4 translation(simd::float3 const & translation) {
        simd::float4x4 matrix = matrix_identity_float4x4;
        matrix.columns[3].xyz = translation;
        return matrix;
    }
}
