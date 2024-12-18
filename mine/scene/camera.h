//
//  camera.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

struct Camera {
    Ray ray(int x, int y, int width, int height) const {
        float newX = (static_cast<float>(x) / (width - 1)) * 2 - 1;
        float aspect = static_cast<float>(width) / height;
        float newY = (static_cast<float>(y) / (height - 1)) / aspect * 2 - 1;

        simd_float3 origin = simd_make_float3(0, 0, 0);
        simd_float3 newP = simd_make_float3(newX, newY, 1.0);
        simd_float3 direction = simd_normalize(newP - origin);

        return Ray{origin, direction};
    }
};
