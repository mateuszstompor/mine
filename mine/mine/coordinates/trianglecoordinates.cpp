//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "trianglecoordinates.h"

float mine::TriangleCoordinates::area(const simd::float3& v0, const simd::float3& v1,
                                      const simd::float3& v2) const {
    return simd::length(simd::cross(v1 - v0, v2 - v0)) * 0.5;
}

simd::float3 mine::TriangleCoordinates::barycentricCoordinates(const simd::float3& point,
                                                               const Triangle& triangle) const {
    const simd::float3& a = triangle.v0;
    const simd::float3& b = triangle.v1;
    const simd::float3& c = triangle.v2;

    simd::float3 v0 = b - a;
    simd::float3 v1 = c - a;
    simd::float3 v2 = point - a;

    float d00 = simd::dot(v0, v0);
    float d01 = simd::dot(v0, v1);
    float d11 = simd::dot(v1, v1);
    float d20 = simd::dot(v2, v0);
    float d21 = simd::dot(v2, v1);

    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    return simd::make_float3(u, v, w);
}

simd::float3 mine::TriangleCoordinates::barycentricToCartesian(const simd::float3& x,
                                                               const simd::float3& v0,
                                                               const simd::float3& v1,
                                                               const simd::float3& v2) const {
    return v0 * x.x + v1 * x.y + v2 * x.z;
}

simd::float2 mine::TriangleCoordinates::getTextureCoordinates(const simd::float3& point,
                                                              const Triangle& triangle) const {
    simd::float3 barycentric = barycentricCoordinates(point, triangle);

    return triangle.uv0 * barycentric.x +
           triangle.uv1 * barycentric.y +
           triangle.uv2 * barycentric.z;
}
