//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <cassert>

#include "rgbauint8bitmap.h"

inline static uint8_t floatToByte(float x) {
    return static_cast<uint8_t>(std::round(std::clamp(x * 255.0f, 0.0f, 255.0f)));
}

mine::RGBAUInt8Bitmap::RGBAUInt8Bitmap(simd::float4 const & color)
: RGBAUInt8Bitmap(1, 1) {
    set(0, 0, color);
}

mine::RGBAUInt8Bitmap::RGBAUInt8Bitmap(uint16_t width,
                                       uint16_t height)
: Bitmap(width, height, BitmapType::RGBAUInt8)
, data(static_cast<int>(width) *
       static_cast<int>(height) *
       static_cast<int>(bytesPerPixel))
{
    // Empty
}

mine::RGBAUInt8Bitmap::RGBAUInt8Bitmap(uint8_t const * rawData,
                                       uint16_t width,
                                       uint16_t height)
: RGBAUInt8Bitmap(width, height)
{
    std::memcpy(data.data(),
                rawData,
                width * height * bytesPerPixel);
}

simd::float4 mine::RGBAUInt8Bitmap::get(uint16_t x, uint16_t y) const {
    assert(x < width);
    assert(y < height);
    uint32_t offset = (y * width + x) * channels;
    return simd::make_float4(
                             data[offset] / 255.0f,
                             data[offset + 1] / 255.0f,
                             data[offset + 2] / 255.0f,
                             data[offset + 3] / 255.0f
                             );
}

void mine::RGBAUInt8Bitmap::set(uint16_t x,
                                uint16_t y,
                                simd::float4 const & normalized) {
    uint32_t offset = (y * width + x) * channels;
    data[offset]     = floatToByte(normalized.x);
    data[offset + 1] = floatToByte(normalized.y);
    data[offset + 2] = floatToByte(normalized.z);
    data[offset + 3] = floatToByte(normalized.w);
}
