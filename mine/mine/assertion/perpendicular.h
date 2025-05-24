//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cassert>
#include <cmath>
#include <simd/simd.h>

namespace mine {
    inline void assertPerpendicular(simd::float3 const & v1,
                                    simd::float3 const & v2,
                                    float epsilon = 1e-4f) {
        float dotResult = std::abs(simd::dot(v1, v2));
        assert(dotResult < epsilon);
        static_cast<void>(dotResult);
    }
}
