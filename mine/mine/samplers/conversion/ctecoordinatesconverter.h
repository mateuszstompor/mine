//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "coordinatesconverter.h"

namespace mine {
    class ClampToEdgeCoordinatesConverter: public CoordinatesConverter {
        simd::float2 lowerBound = simd_make_float2(0.0f, 0.0f);
        simd::float2 higherBound = simd_make_float2(1.0f, 1.0f);
    public:
        simd::float2 convert(simd::float2 coordinates) override {
            return simd::clamp(coordinates, lowerBound, higherBound);
        }
    };
}
