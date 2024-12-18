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
    std::array<simd::float2, 3> uvs;
    simd::float3 normal;
    simd::float3 tangent;
    simd::float3 bitangent;

    Triangle(const std::array<simd::float3, 3>& vertices,
             const std::array<simd::float2, 3>& uvsInput) : uvs(uvsInput) {
        v0 = vertices[0];
        v1 = vertices[1];
        v2 = vertices[2];

        auto v0v1 = v1 - v0;
        auto v0v2 = v2 - v0;
        auto crossed = simd::cross(v0v1, v0v2);

        auto edge1 = vertices[1] - vertices[0];
        auto edge2 = vertices[2] - vertices[0];
        auto deltaUV1 = uvs[1] - uvs[0];
        auto deltaUV2 = uvs[2] - uvs[0];

        float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        simd::float3 tangent1;
        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        simd::float3 bitangent1;
        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        tangent = simd::normalize(tangent1);
        bitangent1 = simd::normalize(bitangent1);

        normal = simd::normalize(crossed);
        auto Bprim = simd::cross(normal, tangent);
        if (simd::dot(Bprim, bitangent1) < 0) {
            bitangent = -bitangent1;
        } else {
            bitangent = bitangent1;
        }
    }
};
