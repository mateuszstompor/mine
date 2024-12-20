//
//  RNGSTD.h
//
//  Created on 20/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <random>

class RNGSTD {
public:
    RNGSTD() : generator{std::random_device{}()}, distribution{0.0f, 1.0f} {
        
    }
    float random() {
        return distribution(generator);
    };
private:
    std::mt19937 generator;
    std::uniform_real_distribution<float> distribution;
};
