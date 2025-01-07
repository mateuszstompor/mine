//
//  diskintersector.cpp
//
//  Created on 06/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "diskcoordinates.h"

#include "../assertion/equal.h"
#include "../assertion/range.h"

simd_float2 mine::DiskCoordinates::getPolarCoordinates(const simd_float3& point, const Disk& disk) {
    float angle = std::atan2(point.y, point.x);
    float radius = simd::length(point - disk.origin);
    return { angle, radius };
}

simd_float3 mine::DiskCoordinates::polarToCartesian(float r, float theta, Disk const & disk) {
    assertInClosedRange(theta, 0.0f, 2.0f * static_cast<float>(M_PI));
    assertInClosedRange(r, 0.0f, disk.radius);
    assert(disk.normal.z != 0 && "disk.normal.z must not be zero to avoid division by zero");

    
    float x = r * cos(theta);
    float y = r * sin(theta);
    float z = 0;
    
    simd::float3 N = simd::normalize(disk.normal);
    
    simd::float3 T1 = (abs(N.x) > 0.5f)
                       ? simd_make_float3(0, 1, 0)
                       : simd_make_float3(1, 0, 0);
    
    simd::float3 T = simd::normalize(simd::cross(N, T1));
    simd::float3 B = simd::normalize(simd::cross(T, N));

    simd::float3x3 tbn(T, B, N);
    
    return tbn * simd::make_float3(x, y, z) + disk.origin;
}

simd_float2 mine::DiskCoordinates::getTextureCoordinates(const simd_float2& polarCoordinates, const Disk& disk) {
    float v = polarCoordinates[1] / disk.radius;
    float u = (polarCoordinates[0] + M_PI) / (2 * M_PI);
    return { u, v };
}

simd_float2 mine::DiskCoordinates::getTextureCoordinates(const simd_float3& point, const Disk& disk) {
    simd_float2 polar = getPolarCoordinates(point, disk);
    simd_float2 uvCoordinates = getTextureCoordinates(polar, disk);
    assert(uvCoordinates[0] >= 0 && uvCoordinates[0] <= 1);
    assert(uvCoordinates[1] >= 0 && uvCoordinates[1] <= 1);
    return uvCoordinates;
}
