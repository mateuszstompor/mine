//
//  cgbitmap.mm
//
//  Created on 06/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "cgbitmap.h"

mine::CGBitmap::CGBitmap(uint16_t width,
                   uint16_t height,
                   uint8_t bytesPerPixel)
: bitmap(width, height, bytesPerPixel) {
    colorSpace = CGColorSpaceCreateDeviceRGB();
    bitmapContext = CGBitmapContextCreate(bitmap.data.data(),
                                          bitmap.width,
                                          bitmap.height,
                                          8,
                                          bitmap.bytesPerPixel * bitmap.width,
                                          colorSpace,
                                          kCGImageAlphaPremultipliedLast);
}

mine::CGBitmap::~CGBitmap() {
    CGContextRelease(bitmapContext);
    CGColorSpaceRelease(colorSpace);
}
