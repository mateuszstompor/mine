//
//  triangle.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <array>
#include <cmath>
#include <simd/simd.h>

// Assumption
//      v2
// v0       v1

struct Triangle {
    // Vertices
    simd::float3 v0;
    simd::float3 v1;
    simd::float3 v2;
    simd::float3 normal;
    simd::float3 tangent;
    simd::float3 bitangent;
    simd::float2 uv0;
    simd::float2 uv1;
    simd::float2 uv2;

    Triangle(const std::array<simd::float3, 3>& vertices,
             const std::array<simd::float2, 3>& uvsInput);
    
    
};
