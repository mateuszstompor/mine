//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <CoreGraphics/CoreGraphics.h>

#include "raw/bitmap.h"
#include "raw/rgbauint8bitmap.h"

namespace mine {
    struct CGBitmap {
        RGBAUInt8Bitmap bitmap;
        CGColorSpaceRef colorSpace;
        CGContextRef bitmapContext;
        
        CGBitmap(uint16_t width,
                 uint16_t height,
                 uint8_t bytesPerPixel);
        
        CGBitmap(RGBAUInt8Bitmap const & bitmap);
        
        CGBitmap & operator=(CGBitmap const &) = delete;
        ~CGBitmap();
    };
}
