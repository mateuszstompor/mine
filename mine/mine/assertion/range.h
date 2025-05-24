//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>
#include <cassert>

namespace mine {
    template <typename T>
    inline void assertInClosedRange(T actual,
                                    T lowerBound,
                                    T higherBounad,
                                    T epsilon = 1e-6) {
        assert(actual >= lowerBound - epsilon &&
               actual <= higherBounad + epsilon);
    }

    inline void assertEachInClosedRange(simd::float2 const & actual,
                                        simd::float2 const & range,
                                        float epsilon = 1e-6f) {
        assert(actual.x >= range.x - epsilon &&
               actual.x <= range.y + epsilon);
        assert(actual.y >= range.x - epsilon &&
               actual.y <= range.y + epsilon);
    }
}
