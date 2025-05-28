//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "ray/ray.h"
#include "../rng/rngstd.h"

namespace mine {
    struct Camera {
        Camera(float32_t antialiasRange = 1.0f);
        Ray ray(uint16_t x,
                uint16_t y,
                uint16_t width,
                uint16_t height);
    private:
        float32_t antialiasRange;
        RNGSTD rng;
    };
}
