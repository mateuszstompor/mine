//
//  rngstd.cpp
//
//  Created on 06/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "rngstd.h"

mine::RNGSTD::RNGSTD()
: generator{std::random_device{}()}, distribution{0.0f, 1.0f} {
    
}

float mine::RNGSTD::random() {
    return distribution(generator);
};
