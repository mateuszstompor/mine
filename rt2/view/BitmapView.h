//
//  BitmapView.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#import <AppKit/AppKit.h>

#import "../../mine/cgbitmap.h"
#import "../../mine/raytracer.h"
#import "../../mine/scenebuilder.h"
#import "../../mine/scene/look/textureloader.h"

@interface BitmapView : NSView
{
    CGBitmap * cgbitmap;
    RayTracer rt;
    Scene scene;
}
@end

@implementation BitmapView

- (instancetype)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        cgbitmap = new CGBitmap(1000, 1000, 4);
        scene = SceneBuilder::cornellBox();
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
    uint16_t width = cgbitmap->bitmap.width;
    uint16_t height = cgbitmap->bitmap.height;
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            simd_float4 color = rt.trace(x, y, width, height, scene);
            
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
