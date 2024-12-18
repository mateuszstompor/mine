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
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
            [self generateBitmapData];
        });
        [NSTimer scheduledTimerWithTimeInterval:0.1 repeats:YES block:^(NSTimer * _Nonnull timer) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self setNeedsDisplay:YES];
            });
        }];
    }
    return self;
}

- (void)dealloc
{
    delete cgbitmap;
    [super dealloc];
}

- (void)generateBitmapData {
    Scene s;
    s.spheres.push_back(Sphere({0, 0, 100}, 30));
    s.spheres.push_back(Sphere({-30, 30, 80}, 40));
    s.omnilights.push_back(OmniLight(Sphere({0, 80, 0}, 30),
                                     100,
                                     {1, 0, 0}));
    uint16_t width = cgbitmap->bitmap.width;
    uint16_t height = cgbitmap->bitmap.height;
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            simd_float4 color = rt.trace(x, y, width, height, s);
            cgbitmap->bitmap.setNormalizedRGBA(x, height - y - 1, color);
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
