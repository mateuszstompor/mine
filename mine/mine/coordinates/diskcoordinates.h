//
//  diskintersector.h
//
//  Created on 06/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../scene/disk.h"

namespace mine {
    class DiskCoordinates {
    public:
        simd_float2 getPolarCoordinates(const simd_float3& point,
                                        const Disk& disk);
        
        simd_float3 polarToCartesian(float r,
                                     float theta,
                                     Disk const & disk);

        simd_float2 getTextureCoordinates(const simd_float2& polarCoordinates,
                                          const Disk& disk);

        simd_float2 getTextureCoordinates(const simd_float3& point,
                                          const Disk& disk);
    };
}
