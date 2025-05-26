//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../sphere.h"

namespace mine {
    struct OmniLight {
        Sphere representation;
        float intensity;
        simd::float3 color;
    };
}
