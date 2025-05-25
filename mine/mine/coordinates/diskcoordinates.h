//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../scene/disk.h"

namespace mine {
    class DiskCoordinates {
    public:
        simd::float2 getPolarCoordinates(simd::float3 const & point,
                                         Disk const & disk);
        
        simd::float3 polarToCartesian(float r,
                                      float theta,
                                      Disk const & disk);

        simd::float2 getTextureCoordinates(simd::float2 const & polarCoordinates,
                                           Disk const & disk);

        simd::float2 getTextureCoordinates(simd::float3 const & point,
                                           Disk const & disk);
    };
}
