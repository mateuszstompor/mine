//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <AppKit/AppKit.h>
#include <mine/mine.h>
#include <simd/simd.h>

#include "SceneBuilder.h"

@interface BitmapView : NSView
{
    mine::RTWriter * writer;
    NSTextField * mousePositionView;
    NSTimer * timer;
    Scene scene;
}
@end

@implementation BitmapView

- (instancetype)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        mine::Config c(// width
                       1000,
                       // height
                       1000,
                       // rays per pixel
                       1,
                       // shadow samples
                       1,
                       // indirect light sampling
                       1,
                       // reflections
                       true,
                       // tile size
                       100,
                       // depth
                       3,
                       // threads
                       8);
        writer = new mine::RTWriter(c);
        scene = SceneBuilder::buildCameraLensGraph();
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
            writer->capture(scene);
        });
        timer = [NSTimer scheduledTimerWithTimeInterval:0.1
                                                repeats:YES
                                                  block:^(NSTimer * _Nonnull timer) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self setNeedsDisplay:YES];
            });
        }];
    }
    
    mousePositionView = [self buildPositionView];
    [self addSubview:mousePositionView];
    return self;
}

- (NSTextField *)buildPositionView {
    NSTextField * mousePositionView = [[[NSTextField alloc] initWithFrame:NSMakeRect(5, 5, 0, 0)] autorelease];
    [mousePositionView setTextColor:[NSColor whiteColor]];
    [mousePositionView setEditable:NO];
    [mousePositionView setSelectable:NO];
    [mousePositionView setBezeled:NO];
    [mousePositionView setBordered:NO];
    mousePositionView.layer.backgroundColor = [[NSColor clearColor] CGColor];
    mousePositionView.attributedStringValue = [[[NSAttributedString alloc] initWithString:@"Waiting for move..."] autorelease];
    [mousePositionView sizeToFit];
    return mousePositionView;
}

- (void)mouseUp:(NSEvent *)event {
    NSPoint pointInView = [self convertPoint:[event locationInWindow] fromView:nil];
    simd::float2 normalizedPosition = simd::make_float2((float)pointInView.x / self.bounds.size.width,
                                                        (float)pointInView.y / self.bounds.size.height);
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        writer->capturePixel(scene, normalizedPosition);
    });
}

- (void)updateTrackingAreas {
    [super updateTrackingAreas];

    for (NSTrackingArea *area in [self trackingAreas]) {
        [self removeTrackingArea:area];
    }

    NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
                                                                options:NSTrackingMouseMoved | NSTrackingActiveInKeyWindow | NSTrackingInVisibleRect
                                                                  owner:self
                                                               userInfo:nil];
    [self addTrackingArea:trackingArea];
    [self.window makeFirstResponder:self];
}

- (void)updateMessageWithPoint: (NSPoint) pointInView {
    simd::float2 normalizedPosition = simd::make_float2((float)pointInView.x / self.bounds.size.width,
                                                        (float)pointInView.y / self.bounds.size.height);
    simd::float2 texturePosition = simd::make_float2(normalizedPosition.x * writer->accumulator.width,
                                                     normalizedPosition.y * writer->accumulator.height);
    NSString * newMessage = [[[NSString alloc] initWithFormat: @"X: %i, Y: %i Xn: %.2f, Yn: %.2f Xt: %i, Yt: %i",
                              (int)pointInView.x, (int)pointInView.y,
                              normalizedPosition.x, normalizedPosition.y,
                              (int)texturePosition.x, (int)texturePosition.y] autorelease];
    mousePositionView.attributedStringValue = [[[NSAttributedString alloc] initWithString:newMessage] autorelease];
    [mousePositionView sizeToFit];
}

- (void)mouseMoved:(NSEvent *)event {
    NSPoint pointInView = [self convertPoint:[event locationInWindow] fromView:nil];
    [self updateMessageWithPoint: pointInView];
}

- (void)dealloc
{
    delete writer;
    [timer invalidate];
    [super dealloc];
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGBitmap bitmap = writer->getBitmap();
    CGImageRef image = CGBitmapContextCreateImage(bitmap.bitmapContext);

    CGContextSaveGState(context);
    CGContextTranslateCTM(context, 0, self.bounds.size.height);
    CGContextScaleCTM(context, 1.0, -1.0);

    CGContextDrawImage(context, self.bounds, image);

    CGContextRestoreGState(context);
    CGImageRelease(image);
}

@end
