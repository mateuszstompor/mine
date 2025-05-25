//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

namespace mine {
    simd::float3 sampleHemisphere(const simd::float3& normal, float u, float v);

    simd_float3 sampleHemisphereGGXVNDF(const simd_float3& v,
                                        const simd_float3& n,
                                        float roughness,
                                        float u1,
                                        float u2);
};
