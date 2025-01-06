//
//  equal.h
//
//  Created on 06/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cassert>

namespace mine {
    template <typename T>
    inline void assertInClosedRange(T actual,
                                    T lowerBound,
                                    T higherBounad,
                                    T epsilon = 1e-6) {
        assert(actual >= lowerBound - epsilon &&
               actual <= higherBounad + epsilon)
    }
}
