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
        
        Vertex(simd::float3 const & newPosition,
               simd::float3 const & newTangent,
               simd::float3 const & newBitangent,
               simd::float3 const & newNormal,
               simd::float2 newUV);
    };
}

mine::Vertex operator*(simd::float4x4 const & matrix, mine::Vertex const & vertex);
