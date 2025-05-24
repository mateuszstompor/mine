//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "../../texture/bitmap.h"

namespace mine {
    class Sampler {
    public:
        virtual simd::float4 sample(simd::float2 uv,
                                    mine::Bitmap const * texture) = 0;
        virtual ~Sampler() = default;
    };
}
