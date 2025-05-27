//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "cgbitmap.h"

mine::CGBitmap::CGBitmap(uint16_t width,
                         uint16_t height,
                         uint8_t bytesPerPixel)
: bitmap(width, height) {
    colorSpace = CGColorSpaceCreateDeviceRGB();
    bitmapContext = CGBitmapContextCreate(bitmap.data.data(),
                                          bitmap.width,
                                          bitmap.height,
                                          8,
                                          bitmap.bytesPerPixel * bitmap.width,
                                          colorSpace,
                                          kCGImageAlphaPremultipliedLast);
}

mine::CGBitmap::CGBitmap(RGBAUInt8Bitmap const & providedBitmap)
: CGBitmap(providedBitmap.width, providedBitmap.height, 4) {
    std::memcpy(bitmap.data.data(),
                providedBitmap.data.data(),
                providedBitmap.data.size());
}

mine::CGBitmap::~CGBitmap() {
    CGContextRelease(bitmapContext);
    CGColorSpaceRelease(colorSpace);
}
