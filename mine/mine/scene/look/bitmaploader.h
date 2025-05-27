//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include <CoreGraphics/CoreGraphics.h>
#include <Foundation/Foundation.h>

#include "../../texture/raw/bitmap.h"
#include "../../texture/raw/rgbauint8bitmap.h"

namespace mine {
    class BitmapLoader {
    public:
        static std::optional<RGBAUInt8Bitmap> load(const std::string &name);
        
        static void saveBitmapAsPPM(RGBAUInt8Bitmap const & bitmap, std::string const & filePath);
        static void dumpScreenshot(RGBAUInt8Bitmap const & bitmap, uint32_t iteration);
    private:
        static CGImageRef loadTextureAsBitmap(const std::string &imageName);

        static NSData *convertToBitmapData(CGImageRef cgImage);
    };
}
