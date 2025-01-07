//
//  bitmaploader.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <optional>

#include <CoreGraphics/CoreGraphics.h>
#include <Foundation/Foundation.h>

#include "../../texture/bitmap.h"

namespace mine {
    class BitmapLoader {
    public:
        static std::optional<Bitmap> load(const std::string &name);

    private:
        static CGImageRef loadTextureAsBitmap(const std::string &imageName);

        static NSData *convertToBitmapData(CGImageRef cgImage);
    };
}
