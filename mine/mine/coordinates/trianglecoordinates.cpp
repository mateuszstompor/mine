//
//  trianglecoordinates.cpp
//
//  Created on 07/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "trianglecoordinates.h"

float mine::TriangleCoordinates::area(const simd::float3& v0, const simd::float3& v1,
                                      const simd::float3& v2) const {
    return simd::length(simd::cross(v1 - v0, v2 - v0)) * 0.5;
}

simd::float3 mine::TriangleCoordinates::barycentricCoordinates(const simd::float3& point,
                                                               const Triangle& triangle) const {
    const simd::float3& v0 = triangle.v0;
    const simd::float3& v1 = triangle.v1;
    const simd::float3& v2 = triangle.v2;
    
    float abc = area(v0, v1, v2);
    float bcp = area(v1, v2, point);
    float cap = area(v2, v0, point);
    float abp = area(v0, v1, point);
    
    float u = bcp / abc;
    float v = cap / abc;
    float w = abp / abc;
    
    return simd::make_float3(u, v, w);
}

simd::float3 mine::TriangleCoordinates::barycentricToCartesian(const simd::float3& x,
                                                               const simd::float3& v0,
                                                               const simd::float3& v1,
                                                               const simd::float3& v2) const {
    float x1 = v0.x * x.x + v1.x * x.y + v2.x * x.z;
    float y1 = v0.y * x.x + v1.y * x.y + v2.y * x.z;
    float z1 = v0.z * x.x + v1.z * x.y + v2.z * x.z;
    
    return simd::make_float3(x1, y1, z1);
}

simd::float2 mine::TriangleCoordinates::getTextureCoordinates(const simd::float3& point,
                                                              const Triangle& triangle) const {
    simd::float3 barycentric = barycentricCoordinates(point, triangle);
    
    float u = 1.0 - barycentric.y - barycentric.z;
    float v = 1.0 - barycentric.x - barycentric.z;
    float w = 1.0 - barycentric.x - barycentric.y;
    
    simd::float3 cartesian = barycentricToCartesian(simd::make_float3(u, v, w),
                                                     simd::make_float3(triangle.uv0, 0.0),
                                                     simd::make_float3(triangle.uv1, 0.0),
                                                     simd::make_float3(triangle.uv2, 0.0));
    
    return simd::make_float2(cartesian.x, cartesian.y);
}
