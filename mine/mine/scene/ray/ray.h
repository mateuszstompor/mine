//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

namespace mine {
    struct Ray {
        Ray(simd::float3 const & origin,
            simd::float3 const & direction);
        simd::float3 origin;
        simd::float3 direction;
    };
}
