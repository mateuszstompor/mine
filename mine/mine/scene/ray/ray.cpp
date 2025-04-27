//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "../../assertion/equal.h"

#include "ray.h"

mine::Ray::Ray(simd_float3 const & origin,
               simd_float3 const & direction)
: origin{origin}
, direction{direction} {
    assertEqual(simd::length(direction), 1.0f, 1e-5f);
}
