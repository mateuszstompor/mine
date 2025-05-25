//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "rngstd.h"

mine::RNGSTD::RNGSTD()
: generator{std::random_device{}()}
, distribution{0.0f, 1.0f} {
    
}

float mine::RNGSTD::random() {
    return distribution(generator);
};

simd::float2 mine::RNGSTD::random2() {
    return simd::make_float2(random(), random());
}
