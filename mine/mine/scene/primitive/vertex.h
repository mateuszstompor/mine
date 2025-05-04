//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../../assertion/finite.h"

namespace mine {
    struct Vertex {
        simd::float3 position;
        simd::float3 tangent;
        simd::float3 bitangent;
        simd::float3 normal;
        simd::float2 uv;
        
        Vertex(simd::float3 newPosition,
               simd::float3 newTangent,
               simd::float3 newNormal,
               simd::float2 newUV)
        : position{newPosition}
        , tangent{newTangent}
        , normal{newNormal}
        , bitangent{simd::normalize(simd::cross(newNormal, newTangent))}
        , uv{newUV} {
            assertFinite(position);
            assertFinite(tangent);
            assertFinite(normal);
            assertFinite(bitangent);
            assertFinite(uv);
        }
    };
}
