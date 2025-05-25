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
    
    inline simd::float4x4 scale(simd::float3 const & scale) {
        simd::float4x4 matrix = matrix_identity_float4x4;
        matrix.columns[0].x = scale.x;
        matrix.columns[1].y = scale.y;
        matrix.columns[2].z = scale.z;
        return matrix;
    }

    inline simd::float3x3 float3x3(simd::float4x4 const & input) {
        return simd::float3x3(
            simd::float3(input.columns[0].xyz),
            simd::float3(input.columns[1].xyz),
            simd::float3(input.columns[2].xyz)
        );
    }
}
