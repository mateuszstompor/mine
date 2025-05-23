//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <assert.h>
#include <algorithm>
#include <cmath>

#include "bitmap.h"

inline static uint8_t floatToByte(float x) {
    return static_cast<uint8_t>(std::round(std::clamp(x * 255.0f, 0.0f, 255.0f)));
}

mine::Bitmap::Bitmap(simd_float4 color)
: Bitmap(1, 1, 4) {
    setNormalizedRGBA(0, 0, color);
}

mine::Bitmap::Bitmap(uint16_t width,
                     uint16_t height,
                     uint8_t bytesPerPixel)
: width{width}
, height{height}
, bytesPerPixel{bytesPerPixel}
, data(static_cast<int>(width) *
       static_cast<int>(height) *
       static_cast<int>(bytesPerPixel))
{
    // Empty
}

mine::Bitmap::Bitmap(uint8_t const * rawData,
                     uint16_t width,
                     uint16_t height,
                     uint8_t bytesPerPixel)
: Bitmap(width, height, bytesPerPixel)
{
    std::memcpy(data.data(),
                rawData,
                width * height * bytesPerPixel);
}

uint8_t& mine::Bitmap::at(uint16_t x,
                          uint16_t y,
                          uint8_t channel) {
    return data[(y * width + x) * bytesPerPixel + channel];
}

simd::float4 mine::Bitmap::colorAt(uint16_t x, uint16_t y) const {
    assert(x < width);
    assert(y < height);
    uint32_t offset = (y * width + x) * bytesPerPixel;
    return simd::make_float4(
     data[offset] / 255.0f,
     data[offset + 1] / 255.0f,
     data[offset + 2] / 255.0f,
     data[offset + 3] / 255.0f
    );
}

mine::Bitmap mine::Bitmap::defaultNormalMap() {
    simd::float3 rgb = (simd_make_float3(0.0f, 0.0f, 1.0f) + 1.0f) / 2.0f;
    simd::float4 rgba = simd_make_float4(rgb, 1.0f);
    return Bitmap(rgba);
}

void mine::Bitmap::setNormalizedRGBA(uint16_t x,
                                     uint16_t y,
                                     simd_float4 const & normalized) {
    uint32_t offset = (y * width + x) * bytesPerPixel;
    data[offset]     = floatToByte(normalized.x);
    data[offset + 1] = floatToByte(normalized.y);
    data[offset + 2] = floatToByte(normalized.z);
    data[offset + 3] = floatToByte(normalized.w);
}
