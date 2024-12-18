//
//  bitmap.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <cstdint>
#include <vector>
#include <simd/simd.h>

struct Bitmap {
    const uint16_t width;
    const uint16_t height;
    const uint8_t bytesPerPixel;
    std::vector<uint8_t> data;
    
    Bitmap(uint16_t width, uint16_t height, uint8_t bytesPerPixel)
    : width{width}
    , height{height}
    , bytesPerPixel{bytesPerPixel}
    , data(static_cast<int>(width) * static_cast<int>(height) * static_cast<int>(bytesPerPixel))
    {
        // Empty
    }
    
    uint8_t& at(uint16_t x, uint16_t y) {
        return data[(y * width + x) * bytesPerPixel];
    }
    
    void setNormalizedRGBA(uint16_t x, uint16_t y, simd_float4 const & normalized) {
        data[(y * width + x) * bytesPerPixel] = normalized.x * 255;
        data[(y * width + x) * bytesPerPixel + 1] = normalized.y * 255;
        data[(y * width + x) * bytesPerPixel + 2] = normalized.z * 255;
        data[(y * width + x) * bytesPerPixel + 3] = normalized.w * 255;
    }
};
