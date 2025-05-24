//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "sampler.h"

#include "../../texture/bitmap.h"

namespace mine {
    class LinearSampler: public Sampler {
    public:
        simd::float4 sample(simd::float2 uv,
                            Bitmap const * texture) override;
    };
}
