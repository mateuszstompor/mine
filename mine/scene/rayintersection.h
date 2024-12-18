//
//  rayintersection.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

struct RayIntersection {
    simd_float3 N;
    simd_float3 point;
    simd_float2 uv;
    float t;
};
