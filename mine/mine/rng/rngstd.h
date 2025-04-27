//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <random>

namespace mine {
    class RNGSTD {
    public:
        RNGSTD();
        float random();
    private:
        std::mt19937 generator;
        std::uniform_real_distribution<float> distribution;
    };
}
