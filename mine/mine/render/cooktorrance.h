//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cmath>
#include <algorithm>
#include <simd/simd.h>

#include "../assertion/finite.h"
#include "../assertion/normalized.h"

namespace mine {
    float32_t distributionGGX(float32_t alpha,
                              simd::float3 const & n,
                              simd::float3 const & h);

    simd::float3 fresnelSchlick(const simd::float3& f0,
                                const simd::float3& v,
                                const simd::float3& h);

    float geometrySchlickGGX(const simd::float3& v, const simd::float3& n, float k);

    float geometrySmith(const simd::float3& v, const simd::float3& n, const simd::float3& l, float k);

    simd::float3 cookTorrance(const simd::float3& v,
                              const simd::float3& n,
                              const simd::float3& l,
                              const simd::float3& albedo,
                              float metalness,
                              float roughness);
}
