//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <vector>

#include "bitmap.h"

namespace mine {
    struct RGBFloat32Bitmap : public Bitmap  {
        static constexpr uint8_t bytesPerPixel = 12;
        static constexpr uint8_t channels = 3;
        std::vector<float32_t> data;
        
        RGBFloat32Bitmap(simd::float3 const & color);
        
        RGBFloat32Bitmap(const RGBFloat32Bitmap & other) = default;
        
        RGBFloat32Bitmap & operator=(const RGBFloat32Bitmap & other) = default;
        
        RGBFloat32Bitmap(uint16_t width, uint16_t height);
        
        simd::float4 get(uint16_t x, uint16_t y) const override;
        
        void set(uint16_t x,
                 uint16_t y,
                 simd::float4 const & normalized) override;
    };
}
