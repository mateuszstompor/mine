//
//  material.h
//
//  Created on 18/12/2024.
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
    };
}
