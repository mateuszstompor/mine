//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "coordinatesconverter.h"

namespace mine {
    class RepeatCoordinatesConverter: public CoordinatesConverter {
    public:
        simd::float2 convert(simd::float2 coordinates) override {
            simd::float2 newCoordinates = simd::fract(coordinates);
            return newCoordinates;
        }
    };
}
