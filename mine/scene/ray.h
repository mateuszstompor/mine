//
//  ray.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <simd/simd.h>

struct Ray {
    simd_float3 origin;
    simd_float3 direction;
};
