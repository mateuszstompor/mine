//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "rgbafloat32bitmap.h"
#include "rgbfloat32bitmap.h"
#include "rgbauint8bitmap.h"

namespace mine {
    struct BitmapConverter {
        RGBAFloat32Bitmap convert(RGBAUInt8Bitmap const & bitmap) const;
        RGBAUInt8Bitmap convert(RGBAFloat32Bitmap const & bitmap) const;
        RGBFloat32Bitmap dropAlpha(RGBAFloat32Bitmap const & bitmap) const;
        RGBAFloat32Bitmap extendAlpha(RGBFloat32Bitmap const & bitmap) const;
    };
}
