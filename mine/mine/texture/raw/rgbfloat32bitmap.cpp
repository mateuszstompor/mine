//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <cassert>

#include "rgbfloat32bitmap.h"

mine::RGBFloat32Bitmap::RGBFloat32Bitmap(simd::float3 const & color)
: RGBFloat32Bitmap(1, 1) {
    set(0, 0, simd::make_float4(color, 1.0f));
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
