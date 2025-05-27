//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <vector>

#include "bitmap.h"

namespace mine {
    struct RGBAFloat32Bitmap : public Bitmap  {
        static constexpr uint8_t bytesPerPixel = 16;
        static constexpr uint8_t channels = 4;
        std::vector<float32_t> data;
        
        RGBAFloat32Bitmap(simd::float4 const & color);
        
        RGBAFloat32Bitmap(const RGBAFloat32Bitmap & other) = default;
        
        RGBAFloat32Bitmap & operator=(const RGBAFloat32Bitmap & other) = default;
        
        RGBAFloat32Bitmap(uint16_t width, uint16_t height);
        
        simd::float4 get(uint16_t x, uint16_t y) const override;
        
        void set(uint16_t x,
                 uint16_t y,
                 simd::float4 const & normalized) override;
    };
}
