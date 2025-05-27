//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../scene/sphere.h"

namespace mine {
    class SphereCoordinates {
    public:
        // Contains
        bool isOnSphere(simd::float3 const & point,
                        Sphere const & sphere,
                        float epsilon = 1e-4f) const;
        
        bool isInsideSphere(simd::float3 const & point,
                            Sphere const & sphere) const;
        
        // Conversion
        simd::float3 sphericalToCartesian(float r,
                                          float theta,
                                          float phi) const;

        simd::float2 getSphericalCoordinates(simd::float3 const & nonCenteredPoint,
                                             Sphere const & sphere) const;
        
        simd::float2 getTextureCoordinates(simd::float2 sphericalCoordinates) const;
        
        simd::float2 getTextureCoordinates(simd::float3 const & point,
                                           Sphere const & sphere) const;
        
        simd::float2 getEquirectangularCoordinates(simd::float3 const & direction) const;
    };
}
