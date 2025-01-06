//
//  rayintersection.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include <simd/simd.h>

#include "look/material.h"

struct RayIntersection {
    simd_float3 T;
    simd_float3 B;
    simd_float3 N;
    simd_float3 point;
    simd_float2 uv;
    std::shared_ptr<mine::Material> material;
    float t;
};
