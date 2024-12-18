#import <AppKit/AppKit.h>

@interface BitmapView : NSView
@property (nonatomic, assign) size_t width;
@property (nonatomic, assign) size_t height;
@property (nonatomic) uint8_t *bitmapData;
@end

@implementation BitmapView

- (instancetype)initWithFrame:(NSRect)frame width:(size_t)width height:(size_t)height {
    self = [super initWithFrame:frame];
    if (self) {
        _width = width;
        _height = height;
        _bitmapData = malloc(width * height * 4); // RGBA 8-bit per channel
        [self generateBitmapData]; // Optional: Fill the bitmap with data
    }
    return self;
}

- (void)dealloc {
    if (_bitmapData) {
        free(_bitmapData);
    }
}

// Fill bitmap data with a pattern or colors
- (void)generateBitmapData {
    for (size_t y = 0; y < self.height; y++) {
        for (size_t x = 0; x < self.width; x++) {
            size_t index = (y * self.width + x) * 4;
            _bitmapData[index] = (uint8_t)(x % 255);     // Red
            _bitmapData[index + 1] = (uint8_t)(y % 255); // Green
            _bitmapData[index + 2] = 128;                // Blue
            _bitmapData[index + 3] = 255;                // Alpha
        }
    }
}

// Draw the bitmap
- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef bitmapContext = CGBitmapContextCreate(
        _bitmapData,
        _width,
        _height,
        8, // Bits per component
        _width * 4, // Bytes per row
        colorSpace,
        kCGImageAlphaPremultipliedLast
    );

    CGImageRef image = CGBitmapContextCreateImage(bitmapContext);
    CGContextDrawImage(context, self.bounds, image);
    
    CGImageRelease(image);
    CGContextRelease(bitmapContext);
    CGColorSpaceRelease(colorSpace);
}

@end
