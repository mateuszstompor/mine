//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "coordinatesconverter.h"

namespace mine {
    class PassthroughCoordinatesConverter: public CoordinatesConverter {
    public:
        simd::float2 convert(simd::float2 coordinates) override {
            return coordinates;
        }
    };
}
