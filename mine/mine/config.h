//
//  config.h
//  mine
//
//  Created by Motion VFX on 21/12/2024.
//

#pragma once

#include <cstdint>

namespace mine {
    struct Config {
        uint16_t width;
        uint16_t height;
        uint16_t raysPerPixel;
        // Per Light
        uint16_t shadowSamples;
        // Per Intersections
        uint16_t indirectLightSamples;
        uint16_t regionSide;
        uint8_t depth;
        uint8_t threads;
    };
}
