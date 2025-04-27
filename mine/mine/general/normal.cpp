//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "normal.h"

void mine::generateTBForNormal(simd::float3 & tangent,
                               simd::float3 & bitangent,
                               simd::float3 const & normal) {
    if (std::abs(normal.z) < 0.999f) {
        tangent = simd::normalize(simd::cross(normal, simd::make_float3(0, 0, 1)));
    } else {
        tangent = simd::normalize(simd::cross(normal, simd::make_float3(0, 1, 0)));
    }
    bitangent = simd::normalize(simd::cross(normal, tangent));
    // Validation
    assertEqual(simd::length(normal), 1.0f, 1e-4f);
    assertFinite(normal);
    assertFinite(tangent);
    assertFinite(bitangent);
}
