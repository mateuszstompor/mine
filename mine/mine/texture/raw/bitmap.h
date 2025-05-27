//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cstdint>

#include <simd/simd.h>

#include "bitmaptype.h"

namespace mine {
    struct Bitmap {
        BitmapType type;
        uint16_t width;
        uint16_t height;

        Bitmap(uint16_t width, uint16_t height, BitmapType type)
        : width{width}
        , height{height}
        , type{type} {
            // empty
        }
        
        virtual simd::float4 get(uint16_t x, uint16_t y) const = 0;
        virtual void set(uint16_t x, uint16_t y, simd::float4 const & normalizedColor) = 0;
        virtual ~Bitmap() = default;
    };
}
