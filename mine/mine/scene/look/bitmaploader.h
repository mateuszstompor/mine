//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cstdarg>
#include <optional>

#include <CoreGraphics/CoreGraphics.h>
#include <Foundation/Foundation.h>

#include "../../texture/bitmap.h"

namespace mine {
    class BitmapLoader {
    public:
        static std::optional<RGBAUint8Bitmap> load(const std::string &name);
        
        static void saveBitmapAsPPM(RGBAUint8Bitmap const & bitmap, std::string const & filePath);
        static void dumpScreenshot(RGBAUint8Bitmap const & bitmap, uint32_t iteration);
    private:
        static CGImageRef loadTextureAsBitmap(const std::string &imageName);

        static NSData *convertToBitmapData(CGImageRef cgImage);
    };
}
