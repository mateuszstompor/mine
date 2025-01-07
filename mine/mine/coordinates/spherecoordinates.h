//
//  spherecoordinates.h
//
//  Created on 07/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../scene/sphere.h"

namespace mine {
    class SphereCoordinates {
    public:
        // Contains
        bool isOnSphere(const simd::float3& point, const Sphere& sphere, float epsilon);
        bool isInsideSphere(simd::float3 const & point, Sphere const & sphere);
        
        // Conversion
        simd::float3 sphericalToCartesian(float r,
                                          float theta,
                                          float phi);

        simd::float2 getSphericalCoordinates(const simd::float3& nonCenteredPoint, const Sphere& sphere);
        
        
        
        simd::float2 getTextureCoordinates(const simd::float2& sphericalCoordinates);
        
        simd::float2 getTextureCoordinates(const simd::float3& point, const Sphere& sphere);
    };
}
