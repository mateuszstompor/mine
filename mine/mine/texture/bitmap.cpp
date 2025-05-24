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

mine::RGBAUint8Bitmap::RGBAUint8Bitmap(simd_float4 const & color)
: RGBAUint8Bitmap(1, 1) {
    set(0, 0, color);
}

mine::RGBAUint8Bitmap::RGBAUint8Bitmap(uint16_t width,
                                       uint16_t height)
: Bitmap(width, height, BitmapType::RGBAUInt8)
, data(static_cast<int>(width) *
       static_cast<int>(height) *
       static_cast<int>(bytesPerPixel))
{
    // Empty
}

mine::RGBAUint8Bitmap::RGBAUint8Bitmap(uint8_t const * rawData,
                                       uint16_t width,
                                       uint16_t height)
: RGBAUint8Bitmap(width, height)
{
    std::memcpy(data.data(),
                rawData,
                width * height * bytesPerPixel);
}

simd::float4 mine::RGBAUint8Bitmap::get(uint16_t x, uint16_t y) const {
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

void mine::RGBAUint8Bitmap::set(uint16_t x,
                                uint16_t y,
                                simd_float4 const & normalized) {
    uint32_t offset = (y * width + x) * channels;
    data[offset]     = floatToByte(normalized.x);
    data[offset + 1] = floatToByte(normalized.y);
    data[offset + 2] = floatToByte(normalized.z);
    data[offset + 3] = floatToByte(normalized.w);
}

// Float32

mine::RGBAFloat32Bitmap::RGBAFloat32Bitmap(simd_float4 const & color)
: RGBAFloat32Bitmap(1, 1) {
    set(0, 0, color);
}

mine::RGBAFloat32Bitmap::RGBAFloat32Bitmap(uint16_t width, uint16_t height)
: Bitmap(width, height, BitmapType::RGBAFloat32)
, data(static_cast<int>(width) *
       static_cast<int>(height) *
       static_cast<int>(bytesPerPixel))
{
    // Empty
}

simd::float4 mine::RGBAFloat32Bitmap::get(uint16_t x, uint16_t y) const {
    assert(x < width);
    assert(y < height);
    uint32_t offset = (y * width + x) * channels;
    return simd::make_float4(data[offset],
                             data[offset + 1],
                             data[offset + 2],
                             data[offset + 3]);
}

void mine::RGBAFloat32Bitmap::set(uint16_t x,
                                  uint16_t y,
                                  simd_float4 const & normalized) {
    uint32_t offset = (y * width + x) * channels;
    data[offset]     = normalized.x;
    data[offset + 1] = normalized.y;
    data[offset + 2] = normalized.z;
    data[offset + 3] = normalized.w;
}

// RGB Float32

mine::RGBFloat32Bitmap::RGBFloat32Bitmap(simd::float3 const & color)
: RGBFloat32Bitmap(1, 1) {
    set(0, 0, simd_make_float4(color, 1.0f));
}

mine::RGBFloat32Bitmap::RGBFloat32Bitmap(uint16_t width, uint16_t height)
: Bitmap(width, height, BitmapType::RGBFloat32)
, data(static_cast<int>(width) *
       static_cast<int>(height) *
       static_cast<int>(bytesPerPixel))
{
    // Empty
}

simd::float4 mine::RGBFloat32Bitmap::get(uint16_t x, uint16_t y) const {
    assert(x < width);
    assert(y < height);
    uint32_t offset = (y * width + x) * channels;
    return simd::make_float4(data[offset],
                             data[offset + 1],
                             data[offset + 2],
                             1.0f);
}

void mine::RGBFloat32Bitmap::set(uint16_t x,
                                 uint16_t y,
                                 simd::float4 const & normalized) {
    uint32_t offset = (y * width + x) * channels;
    data[offset]     = normalized.x;
    data[offset + 1] = normalized.y;
    data[offset + 2] = normalized.z;
}
