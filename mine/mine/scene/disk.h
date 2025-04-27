//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cassert>
#include <simd/simd.h>

#include "../assertion/equal.h"
#include "../general/normal.h"

namespace mine {
    struct Disk {
        simd::float3 origin;
        simd::float3 normal;
        simd::float3 tangent;
        simd::float3 bitangent;
        float radius;
        
        Disk(simd::float3 const & origin,
             simd::float3 const & normal,
             float radius)
        : origin{origin}
        , normal{normal}
        , radius{radius} {
            assertEqual(simd::length(normal), 1.0f, 1e-4f);
            assert(radius > 0.0f);
            generateTBForNormal(tangent, bitangent, normal);
        }
    };
}
