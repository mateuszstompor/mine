//
//  camera.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "ray/ray.h"
#include "../rng/rngstd.h"

namespace mine {
    struct Camera {
        Camera(int antialiasRange = 1);
        Ray ray(int x, int y, int width, int height);
    private:
        int antialiasRange;
        RNGSTD rng;
    };
}
