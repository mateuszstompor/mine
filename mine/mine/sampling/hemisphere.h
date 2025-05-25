//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

namespace mine {
    simd::float3 sampleHemisphere(simd::float3 const & normal,
                                  simd::float2 uv);

    simd::float3 sampleHemisphereGGXVNDF(simd::float3 const & view,
                                         simd::float3 const & normal,
                                         simd::float3x3 const & tbn,
                                         float roughness,
                                         simd::float2 uv);
};
