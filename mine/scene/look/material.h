//
//  material.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../Bitmap"

struct Material {
    Bitmap albedo;
    Bitmap roughness;
    Bitmap metalness;
    Bitmap normal;
};
