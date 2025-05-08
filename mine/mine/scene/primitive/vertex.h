//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../../assertion/finite.h"
#include "../../assertion/normalized.h"
#include "../../assertion/perpendicular.h"
#include "../../math/transform.h"

namespace mine {
    struct Vertex {
        simd::float3 position;
        simd::float3 tangent;
        simd::float3 bitangent;
        simd::float3 normal;
        simd::float2 uv;
        
        Vertex(simd::float3 newPosition,
               simd::float3 newTangent,
               simd::float3 newBitangent,
               simd::float3 newNormal,
               simd::float2 newUV)
        : position{newPosition}
        , tangent{newTangent}
        , normal{newNormal}
        , bitangent{newBitangent}
        , uv{newUV} {
            assertFinite(position);
            assertFinite(tangent);
            assertFinite(normal);
            assertFinite(bitangent);
            assertFinite(uv);
            assertNormalized(tangent);
            assertNormalized(bitangent);
            assertNormalized(normal);
            assertPerpendicular(normal, tangent);
            assertPerpendicular(normal, bitangent);
            assertPerpendicular(tangent, bitangent);
        }
    };
}

inline mine::Vertex operator*(simd::float4x4 const & matrix, mine::Vertex const & vertex) {
    mine::assertNormalized(vertex.normal);
    mine::assertNormalized(vertex.tangent);
    mine::assertNormalized(vertex.bitangent);
    
    simd::float4 position4 = simd_make_float4(vertex.position, 1.0f);
    position4 = matrix * position4;
    
    assert(position4.w == 1.0f);
    
    position4 /= position4.w;
    
    simd::float3x3 rotationMatrix = mine::float4x4_to_float3x3(matrix);
    rotationMatrix = simd::transpose(simd::inverse(rotationMatrix));
    mine::Vertex result = vertex;
    result.normal = simd::normalize(rotationMatrix * result.normal);
    result.tangent = simd::normalize(rotationMatrix * result.tangent);
    result.bitangent = simd::normalize(rotationMatrix * result.bitangent);
    result.position = position4.xyz;
    
    mine::assertNormalized(result.normal);
    mine::assertNormalized(result.tangent);
    mine::assertNormalized(result.bitangent);
    return result;
}
