//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <simd/simd.h>

#include "sampler.h"

#include "../../texture/raw/bitmap.h"

namespace mine {
    class NearestSampler: public Sampler {
    public:
        simd::float4 sample(simd::float2 uv,
                            std::shared_ptr<Bitmap> const & texture) override;
    };
}
