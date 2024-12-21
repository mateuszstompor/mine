//
//  BitmapView.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <AppKit/AppKit.h>
#include <mine/mine.h>

#include "SceneBuilder.h"

@interface BitmapView : NSView
{
    mine::RTWriter * writer;
    Scene scene;
}
@end

@implementation BitmapView

- (instancetype)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        mine::Config c(1000,
                       1000,
                       100,
                       2,
                       2,
                       100,
                       1);
        writer = new mine::RTWriter(c);
        scene = SceneBuilder::cornellBox();
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
            writer->capture(scene);
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
    delete writer;
    [super dealloc];
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGImageRef image = CGBitmapContextCreateImage(writer->cgbitmap.bitmapContext);
    CGContextDrawImage(context, self.bounds, image);
    CGImageRelease(image);
}

@end
