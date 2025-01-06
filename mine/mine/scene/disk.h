//
//  disk.h
//
//  Created on 20/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../assertion/equal.h"

namespace mine {
    struct Disk {
        simd::float3 origin;
        simd::float3 normal;
        float radius;
        
        Disk(simd::float3 const & origin,
             simd::float3 const & normal,
             float radius)
        : origin{origin}
        , normal{normal}
        , radius{radius} {
            assertEqual(simd::length(normal), 1.0f);
        }
    };
}
