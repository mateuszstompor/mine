//
//  bitmap.h
//
//  Created on 06/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "bitmap.h"

mine::Bitmap::Bitmap(simd_float4 color)
: Bitmap(1, 1, 4) {
    *(data.data()) = color.x * 255;
    *(data.data() + 1) = color.y * 255;
    *(data.data() + 2) = color.z * 255;
    *(data.data() + 3) = color.w * 255;
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
    return simd::make_float4(
     data[(y * width + x) * bytesPerPixel] / 255.0,
     data[(y * width + x) * bytesPerPixel + 1] / 255.0,
     data[(y * width + x) * bytesPerPixel + 2] / 255.0,
     data[(y * width + x) * bytesPerPixel + 3] / 255.0
    );
}

mine::Bitmap mine::Bitmap::defaultNormalMap() {
    return Bitmap(simd_make_float4((simd_make_float3(0, 0, 1) + 1) / 2.0, 1));
}

void mine::Bitmap::setNormalizedRGBA(uint16_t x,
                                     uint16_t y,
                                     simd_float4 const & normalized) {
    data[(y * width + x) * bytesPerPixel] = normalized.x * 255;
    data[(y * width + x) * bytesPerPixel + 1] = normalized.y * 255;
    data[(y * width + x) * bytesPerPixel + 2] = normalized.z * 255;
    data[(y * width + x) * bytesPerPixel + 3] = normalized.w * 255;
}
