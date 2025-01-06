//
//  cgbitmap.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <CoreGraphics/CoreGraphics.h>

#include "bitmap.h"

namespace mine {
    struct CGBitmap {
        Bitmap bitmap;
        CGColorSpaceRef colorSpace;
        CGContextRef bitmapContext;
        
        CGBitmap(uint16_t width,
                 uint16_t height,
                 uint8_t bytesPerPixel);
        
        CGBitmap & operator=(CGBitmap const &) = delete;
        ~CGBitmap();
    };
}
