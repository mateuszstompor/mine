//
//  ray.cpp
//
//  Created on 06/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "../../assertion/equal.h"

#include "ray.h"

mine::Ray::Ray(simd_float3 const & origin,
               simd_float3 const & direction)
: origin{origin}
, direction{direction} {
    assertEqual(simd::length(direction), 1.0f);
}
