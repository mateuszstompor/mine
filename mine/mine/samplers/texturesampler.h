//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "nearestsampler.h"
#include "linearsampler.h"

namespace mine {
    enum class Filter {
        Linear,
        Nearest
    };
    class TextureSampler {
    private:
        Filter filter;
        LinearSampler linearSampler;
        NearestSampler nearestSampler;
    public:
        TextureSampler(Filter wantedFilter)
        : filter{wantedFilter} {
            
        }
        simd::float4 sample(float u, float v, mine::Bitmap const & texture) {
            if (filter == Filter::Linear) {
                return linearSampler.sample(u, v, texture);
            } else {
                return nearestSampler.sample(u, v, texture);
            }
        }
    };
}
