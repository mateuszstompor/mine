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
        static std::optional<Bitmap> load(const std::string &name);
        
        static void saveBitmapAsPPM(Bitmap const & bitmap, std::string const & filePath);
        static void dumpScreenshot(Bitmap const & bitmap, uint32_t iteration);
    private:
        static CGImageRef loadTextureAsBitmap(const std::string &imageName);

        static NSData *convertToBitmapData(CGImageRef cgImage);
    };
}
