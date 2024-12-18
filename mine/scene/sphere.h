//
//  sphere.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <simd/simd.h>

struct Sphere {
    float radius;
    simd_float3 origin;
};
