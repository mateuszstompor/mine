//
//  ray.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

struct Ray {
    Ray(simd_float3 origin,
        simd_float3 direction)
    : origin{origin}
    , direction{direction} {
        assert(simd::length(direction) > 0.99f && simd::length(direction) < 1.01f);
    }
    simd_float3 origin;
    simd_float3 direction;
};
