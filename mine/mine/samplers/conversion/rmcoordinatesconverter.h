//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "coordinatesconverter.h"

namespace mine {
    class RepeatMirroredCoordinatesConverter : public CoordinatesConverter {
    public:
        simd::float2 convert(simd::float2 coordinates) override {
            simd::float2 integerPart = simd::floor(coordinates);
            simd::float2 fractionalPart = simd::fract(coordinates);

            float x = ((int)integerPart.x % 2 == 0) ? fractionalPart.x : 1.0f - fractionalPart.x;
            float y = ((int)integerPart.y % 2 == 0) ? fractionalPart.y : 1.0f - fractionalPart.y;

            return simd_make_float2(x, y);
        }
    };
}
