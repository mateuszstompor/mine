//
//  rngstd.h
//
//  Created on 20/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

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
