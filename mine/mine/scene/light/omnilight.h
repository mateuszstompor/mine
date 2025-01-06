//
//  omnilight.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../sphere.h"

namespace mine {
    struct OmniLight {
        Sphere representation;
        float intensity;
        simd_float3 color;
    };
}
