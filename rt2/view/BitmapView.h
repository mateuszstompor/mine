//
//  BitmapView.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#import <AppKit/AppKit.h>
#import "../../mine/cgbitmap.h"
#import "../../mine/raytracer.h"

@interface BitmapView : NSView
{
    CGBitmap * cgbitmap;
    RayTracer rt;
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
            simd_float4 color = rt.trace(x, y);
            cgbitmap->bitmap.setNormalizedRGBA(x, y, color);
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
