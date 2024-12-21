//
//  cgbitmap.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <CoreGraphics/CoreGraphics.h>
#include "bitmap.h"

struct CGBitmap {
    Bitmap bitmap;
    CGColorSpaceRef colorSpace;
    CGContextRef bitmapContext;
    CGBitmap(uint16_t width, uint16_t height, uint8_t bytesPerPixel)
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
    CGBitmap & operator=(CGBitmap const &) = delete;
    ~CGBitmap() {
        CGContextRelease(bitmapContext);
        CGColorSpaceRelease(colorSpace);
    }
};
