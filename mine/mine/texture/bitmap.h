//
//  bitmap.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cstdint>
#include <vector>
#include <cstring>

#include <simd/simd.h>

namespace mine {
    struct Bitmap {
        const uint16_t width;
        const uint16_t height;
        const uint8_t bytesPerPixel;
        std::vector<uint8_t> data;
        
        Bitmap(simd_float4 color);
        
        Bitmap(uint16_t width,
               uint16_t height,
               uint8_t bytesPerPixel);
        
        Bitmap(uint8_t const * rawData,
               uint16_t width,
               uint16_t height,
               uint8_t bytesPerPixel);
        
        uint8_t& at(uint16_t x, uint16_t y, uint8_t channel = 0);
        
        simd::float4 colorAt(uint16_t x, uint16_t y) const;
        
        static Bitmap defaultNormalMap();
        
        void setNormalizedRGBA(uint16_t x,
                               uint16_t y,
                               simd_float4 const & normalized);
    };
}
