//
//  trianglecoordinates.h
//
//  Created on 07/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../scene/triangle.h"

namespace mine {
    class TriangleCoordinates {
    public:
        float area(const simd::float3& v0, const simd::float3& v1, const simd::float3& v2) const;
        
        simd::float3 barycentricCoordinates(const simd::float3& point, const Triangle& triangle) const;
        
        simd::float3 barycentricToCartesian(const simd::float3& x,
                                            const simd::float3& v0,
                                            const simd::float3& v1,
                                            const simd::float3& v2) const;
        
        simd::float2 getTextureCoordinates(const simd::float3& point, const Triangle& triangle) const;
    };
}
