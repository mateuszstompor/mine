//
//  BitmapView.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#import <AppKit/AppKit.h>
#import "../../mine/cgbitmap.h"

@interface BitmapView : NSView
{
    CGBitmap * cgbitmap;
}
@end

@implementation BitmapView

- (instancetype)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        cgbitmap = new CGBitmap(1000, 1000, 4);
        [self generateBitmapData];
    }
    return self;
}

- (void)dealloc
{
    delete cgbitmap;
    [super dealloc];
}

- (void)generateBitmapData {
    for (size_t y = 0; y < cgbitmap->bitmap.height; y++) {
        for (size_t x = 0; x < cgbitmap->bitmap.width; x++) {
            size_t index = (y * cgbitmap->bitmap.width + x) * 4;
            cgbitmap->bitmap.data[index] = (uint8_t)(x % 255);
            cgbitmap->bitmap.data[index + 1] = (uint8_t)(y % 255);
            cgbitmap->bitmap.data[index + 2] = 128;
            cgbitmap->bitmap.data[index + 3] = 255;
        }
    }
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGImageRef image = CGBitmapContextCreateImage(cgbitmap->bitmapContext);
    CGContextDrawImage(context, self.bounds, image);
    CGImageRelease(image);
}

@end
