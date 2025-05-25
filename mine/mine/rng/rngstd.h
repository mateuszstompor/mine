//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <random>
#include <simd/simd.h>

namespace mine {
    class RNGSTD {
    public:
        RNGSTD();
        float random();
        simd::float2 random2();
    private:
        std::mt19937 generator;
        std::uniform_real_distribution<float> distribution;
    };
}
