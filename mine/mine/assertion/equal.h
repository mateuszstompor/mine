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
    inline void assertEqual(T actual, T expected, T epsilon = 1e-6) {
        assert(actual - epsilon < expected &&
               actual + epsilon > expected);
    }
}
