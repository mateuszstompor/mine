//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cstdint>

namespace mine {
    struct Config {
        uint16_t width;
        uint16_t height;
        uint32_t raysPerPixel;
        // Per Light
        uint16_t shadowSamples;
        // Per Intersections
        uint16_t indirectLightSamples;
        // Cast reflection ray
        bool reflections;
        uint16_t regionSide;
        uint8_t depth;
        uint8_t threads;
    };
}
