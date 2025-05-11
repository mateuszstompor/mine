//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "../../texture/bitmap.h"

namespace mine {
    struct Material {
        Bitmap albedo;
        Bitmap roughness;
        Bitmap metalness;
        Bitmap normal;
        Bitmap ior;
        Bitmap opacity;
    };
}
