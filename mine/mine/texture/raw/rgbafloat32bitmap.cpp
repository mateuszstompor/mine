//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <cassert>

#include "rgbafloat32bitmap.h"

mine::RGBAFloat32Bitmap::RGBAFloat32Bitmap(simd::float4 const & color)
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
                                  simd::float4 const & normalized) {
    uint32_t offset = (y * width + x) * channels;
    data[offset]     = normalized.x;
    data[offset + 1] = normalized.y;
    data[offset + 2] = normalized.z;
    data[offset + 3] = normalized.w;
}
