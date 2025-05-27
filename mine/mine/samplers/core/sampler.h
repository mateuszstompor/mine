//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <simd/simd.h>

#include "../../texture/raw/bitmap.h"

namespace mine {
    class Sampler {
    public:
        virtual simd::float4 sample(simd::float2 uv,
                                    std::shared_ptr<Bitmap> const & texture) = 0;
        virtual ~Sampler() = default;
    };
}
