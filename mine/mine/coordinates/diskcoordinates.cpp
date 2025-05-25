//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "diskcoordinates.h"

#include "../assertion/equal.h"
#include "../assertion/range.h"

simd::float2 mine::DiskCoordinates::getPolarCoordinates(simd::float3 const & point,
                                                        Disk const & disk) {
    return simd::float2 {
        // Angle
        std::atan2(point.y, point.x),
        // Radius
        simd::length(point - disk.origin)
    };
}

simd::float3 mine::DiskCoordinates::polarToCartesian(float r,
                                                     float theta,
                                                     Disk const & disk) {
    assertInClosedRange(theta, 0.0f, 2.0f * static_cast<float>(M_PI));
    assertInClosedRange(r, 0.0f, disk.radius);
    
    float x = r * cos(theta);
    float y = r * sin(theta);
    float z = 0;
    
    simd::float3x3 tbn(disk.tangent, disk.bitangent, disk.normal);
    
    return tbn * simd::make_float3(x, y, z) + disk.origin;
}

simd::float2 mine::DiskCoordinates::getTextureCoordinates(simd::float2 const & polarCoordinates,
                                                          Disk const & disk) {
    assert(disk.radius > 0.0f);
    simd::float2 uvCoordinates = {
        (polarCoordinates.x + static_cast<float>(M_PI)) / (2.0f * static_cast<float>(M_PI)),
        polarCoordinates.y / disk.radius
    };
    assertEachInClosedRange(uvCoordinates, simd::make_float2(0.0f, 1.0f));
    return uvCoordinates;
}

simd::float2 mine::DiskCoordinates::getTextureCoordinates(simd::float3 const & point,
                                                          Disk const & disk) {
    simd::float2 polar = getPolarCoordinates(point, disk);
    simd::float2 uvCoordinates = getTextureCoordinates(polar, disk);
    assertEachInClosedRange(uvCoordinates, simd::make_float2(0.0f, 1.0f));
    return uvCoordinates;
}
