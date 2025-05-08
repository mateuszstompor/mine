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
    
    inline simd::float4x4 scale(simd::float3 const & s) {
        simd::float4x4 matrix = matrix_identity_float4x4;
        matrix.columns[0].x = s.x;
        matrix.columns[1].y = s.y;
        matrix.columns[2].z = s.z;
        return matrix;
    }

    inline simd::float3x3 float4x4_to_float3x3(const simd::float4x4& mat4x4) {
        return simd::float3x3(
            simd::float3(mat4x4.columns[0].xyz),
            simd::float3(mat4x4.columns[1].xyz),
            simd::float3(mat4x4.columns[2].xyz)
        );
    }
}
