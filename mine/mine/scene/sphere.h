//
//  sphere.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

struct Sphere {
    simd_float3 center;
    float radius;
};
