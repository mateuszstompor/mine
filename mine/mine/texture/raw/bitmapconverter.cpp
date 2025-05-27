//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "bitmapconverter.h"

mine::RGBAFloat32Bitmap mine::BitmapConverter::convert(RGBAUInt8Bitmap const & bitmap) const {
    RGBAFloat32Bitmap result(bitmap.width, bitmap.height);
    for (uint16_t x = 0; x < bitmap.width; ++x) {
        for (uint16_t y = 0; y < bitmap.height; ++y) {
            simd::float4 color = bitmap.get(x, y);
            result.set(x, y, color);
        }
    }
    return result;
}

mine::RGBAUInt8Bitmap mine::BitmapConverter::convert(RGBAFloat32Bitmap const & bitmap) const {
    RGBAUInt8Bitmap result(bitmap.width, bitmap.height);
    for (uint16_t x = 0; x < bitmap.width; ++x) {
        for (uint16_t y = 0; y < bitmap.height; ++y) {
            simd::float4 color = bitmap.get(x, y);
            result.set(x, y, color);
        }
    }
    return result;
}

mine::RGBFloat32Bitmap mine::BitmapConverter::dropAlpha(RGBAFloat32Bitmap const & bitmap) const {
    RGBFloat32Bitmap result(bitmap.width, bitmap.height);
    for (uint16_t x = 0; x < bitmap.width; ++x) {
        for (uint16_t y = 0; y < bitmap.height; ++y) {
            simd::float4 color = bitmap.get(x, y);
            result.set(x, y, color);
        }
    }
    return result;
}

mine::RGBAFloat32Bitmap mine::BitmapConverter::extendAlpha(RGBFloat32Bitmap const & bitmap) const {
    RGBAFloat32Bitmap result(bitmap.width, bitmap.height);
    for (uint16_t x = 0; x < bitmap.width; ++x) {
        for (uint16_t y = 0; y < bitmap.height; ++y) {
            simd::float4 color = bitmap.get(x, y);
            result.set(x, y, color);
        }
    }
    return result;
}
