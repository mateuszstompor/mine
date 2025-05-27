//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../scene/triangle.h"

namespace mine {
    class TriangleCoordinates {
    public:
        float area(simd::float3 const & v0,
                   simd::float3 const & v1,
                   simd::float3 const & v2) const;
        
        simd::float3 barycentricCoordinates(simd::float3 const & point,
                                            Triangle const & triangle) const;
        
        simd::float3 barycentricToCartesian(simd::float3 const & x,
                                            simd::float3 const & v0,
                                            simd::float3 const & v1,
                                            simd::float3 const & v2) const;
        
        simd::float2 getTextureCoordinates(simd::float3 const & point,
                                           Triangle const & triangle) const;
    };
}
