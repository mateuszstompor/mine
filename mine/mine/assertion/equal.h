//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cassert>
#include <simd/simd.h>

namespace mine {
    template <typename T>
    inline void assertEqual(T actual, T expected, T epsilon = 1e-6) {
        assert(actual - epsilon < expected &&
               actual + epsilon > expected);
    }
    inline void greaterEqual(simd::float3 const & value, float than) {
        assert(value.x >= than);
        assert(value.y >= than);
        assert(value.z >= than);
    }
    inline void greaterEqualZero(simd::float3 const & value) {
        greaterEqual(value, 0.0f);
    }
}
