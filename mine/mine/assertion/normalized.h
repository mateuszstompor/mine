//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cassert>
#include <cmath>
#include <simd/simd.h>

namespace mine {
    inline void assertNormalized(simd::float3 const & v, float epsilon = 1e-1f) {
        float length = simd::length(v);
        assert(1.0f - length < epsilon);
    }
}
