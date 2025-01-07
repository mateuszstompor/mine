//
//  ray.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

namespace mine {
    struct Ray {
        Ray(simd_float3 const & origin,
            simd_float3 const & direction);
        simd_float3 origin;
        simd_float3 direction;
    };
}
