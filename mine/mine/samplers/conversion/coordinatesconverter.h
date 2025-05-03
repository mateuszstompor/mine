//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

namespace mine {
    class CoordinatesConverter {
    public:
        virtual simd::float2 convert(simd::float2 coordinates) = 0;
        virtual ~CoordinatesConverter() = default;
    };
}
