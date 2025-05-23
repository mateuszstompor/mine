//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "bitmaploader.h"

#include <fstream>
#include <Cocoa/Cocoa.h>

std::optional<mine::RGBAUint8Bitmap> mine::BitmapLoader::load(const std::string &name) {
    CGImageRef cgImage = loadTextureAsBitmap(name);
    if (!cgImage) return std::nullopt;

    NSData *bitmapData = convertToBitmapData(cgImage);
    if (!bitmapData) return std::nullopt;

    size_t width = CGImageGetWidth(cgImage);
    size_t height = CGImageGetHeight(cgImage);
    CGImageRelease(cgImage);

    return RGBAUint8Bitmap(static_cast<const uint8_t *>([bitmapData bytes]),
                           width,
                           height);
}

CGImageRef mine::BitmapLoader::loadTextureAsBitmap(const std::string &imageName) {
    NSString *nsName = [NSString stringWithUTF8String:imageName.c_str()];
    NSImage *image = [NSImage imageNamed:nsName];
    if (!image) return nullptr;

    NSRect rect = NSMakeRect(0, 0, image.size.width, image.size.height);
    CGImageRef cgImage = [image CGImageForProposedRect:&rect context:nil hints:nil];
    return cgImage;
}

NSData * mine::BitmapLoader::convertToBitmapData(CGImageRef cgImage) {
    size_t width = CGImageGetWidth(cgImage);
    size_t height = CGImageGetHeight(cgImage);
    size_t bytesPerPixel = 4;
    size_t bitsPerComponent = 8;
    size_t bytesPerRow = bytesPerPixel * width;

    NSMutableData *rawData = [NSMutableData dataWithLength:width * height * bytesPerPixel];
    // Detect and use monochrome or whatever is suggested
    // CGColorSpaceRef colorSpace = CGImageGetColorSpace(cgImage);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
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

void mine::BitmapLoader::saveBitmapAsPPM(RGBAUint8Bitmap const & bitmap, const std::string& filePath) {
    if (bitmap.bytesPerPixel < 3) {
        throw std::runtime_error("Bitmap must have at least 3 bytes per pixel (RGB)");
    }

    std::ofstream outputStream(filePath, std::ios::binary);
    if (!outputStream) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    // Write PPM header (P6 format)
    outputStream << "P6\n" << bitmap.width << " " << bitmap.height << "\n255\n";

    for (uint16_t y = 0; y < bitmap.height; ++y) {
        for (uint16_t x = 0; x < bitmap.width; ++x) {
            const uint8_t* pixel = &bitmap.data[(y * bitmap.width + x) * bitmap.bytesPerPixel];
            // write RGB only
            outputStream.write(reinterpret_cast<const char*>(pixel), 3);
        }
    }

    outputStream.close();
}

void mine::BitmapLoader::dumpScreenshot(RGBAUint8Bitmap const & bitmap, uint32_t iteration) {
    NSDate *now = [NSDate date];
    NSDateFormatter *formatter = [[[NSDateFormatter alloc] init] autorelease];
    [formatter setDateFormat:@"yyyy-MM-dd"];
    NSString * dateString = [formatter stringFromDate:now];
    NSString * containerFolderName = [NSString stringWithFormat:@"Mine-%@", dateString];
    NSString * folderPath = [@"/tmp" stringByAppendingPathComponent:containerFolderName];
    NSFileManager * manager = [NSFileManager defaultManager];
    NSString * filepath = [NSString stringWithFormat:@"%@/%i.ppm", folderPath, iteration];
    if (![manager fileExistsAtPath:folderPath]) {
        [manager createDirectoryAtPath:folderPath
           withIntermediateDirectories:YES
                            attributes:nil
                                 error:nil];
    }
    BitmapLoader::saveBitmapAsPPM(bitmap, [filepath UTF8String]);
}
