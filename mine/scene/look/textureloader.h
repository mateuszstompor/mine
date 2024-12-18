//
//  materialloader.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <optional>

#include "../../bitmap.h"

class BitmapLoader {
public:
    static std::optional<Bitmap> load(const std::string &name) {
        CGImageRef cgImage = loadTextureAsBitmap(name);
        if (!cgImage) return std::nullopt;

        NSData *bitmapData = convertToBitmapData(cgImage);
        if (!bitmapData) return std::nullopt;

        size_t width = CGImageGetWidth(cgImage);
        size_t height = CGImageGetHeight(cgImage);
        CGImageRelease(cgImage);

        return Bitmap(static_cast<const uint8_t *>([bitmapData bytes]),
                      width,
                      height,
                      4);
    }

private:
    static CGImageRef loadTextureAsBitmap(const std::string &imageName) {
        NSString *nsName = [NSString stringWithUTF8String:imageName.c_str()];
        NSImage *image = [NSImage imageNamed:nsName];
        if (!image) return nullptr;

        NSRect rect = NSMakeRect(0, 0, image.size.width, image.size.height);
        CGImageRef cgImage = [image CGImageForProposedRect:&rect context:nil hints:nil];
        return cgImage;
    }

    static NSData *convertToBitmapData(CGImageRef cgImage) {
        size_t width = CGImageGetWidth(cgImage);
        size_t height = CGImageGetHeight(cgImage);
        size_t bytesPerPixel = 4;
        size_t bitsPerComponent = 8;
        size_t bytesPerRow = bytesPerPixel * width;

        NSMutableData *rawData = [NSMutableData dataWithLength:width * height * bytesPerPixel];
        CGColorSpaceRef colorSpace = CGImageGetColorSpace(cgImage);
        if (!colorSpace) return nil;

        CGContextRef context = CGBitmapContextCreate(rawData.mutableBytes,
                                                     width,
                                                     height,
                                                     bitsPerComponent,
                                                     bytesPerRow,
                                                     colorSpace,
                                                     kCGImageAlphaPremultipliedLast);
        if (!context) return nil;

        CGRect rect = CGRectMake(0, 0, width, height);
        CGContextDrawImage(context, rect, cgImage);
        CGContextRelease(context);

        return rawData;
    }
};
