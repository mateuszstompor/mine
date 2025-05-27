//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <vector>
#include <cstdint>

#include "bitmap.h"

namespace mine {
    struct RGBAUInt8Bitmap : public Bitmap  {
        static constexpr uint8_t bytesPerPixel = 4;
        static constexpr uint8_t channels = 4;
        std::vector<uint8_t> data;
        
        RGBAUInt8Bitmap(simd::float4 const & color);
        
        RGBAUInt8Bitmap(const RGBAUInt8Bitmap & other) = default;
        
        RGBAUInt8Bitmap & operator=(const RGBAUInt8Bitmap & other) = default;
        
        RGBAUInt8Bitmap(uint16_t width,
                        uint16_t height);
        
        RGBAUInt8Bitmap(uint8_t const * rawData,
                        uint16_t width,
                        uint16_t height);
                
        simd::float4 get(uint16_t x, uint16_t y) const override;
                
        void set(uint16_t x,
                 uint16_t y,
                 simd::float4 const & normalized) override;
    };
}
