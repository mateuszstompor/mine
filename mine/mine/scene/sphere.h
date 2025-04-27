//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

namespace mine {
    struct Sphere {
        simd_float3 center;
        float radius;
    };
}
