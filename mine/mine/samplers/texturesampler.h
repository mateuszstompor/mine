//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include "edge.h"
#include "filter.h"

#include "./core/sampler.h"

#include "./conversion/coordinatesconverter.h"

namespace mine {
    class TextureSampler {
    private:
        Edge edge;
        std::unique_ptr<CoordinatesConverter> converter;
        std::unique_ptr<Sampler> sampler;
    public:
        TextureSampler(Filter wantedFilter, Edge wantedEdge);
        simd::float4 sample(simd::float2 uv,
                            mine::Bitmap const * texture);
    private:
        static std::unique_ptr<Sampler> buildSampler(Filter filter);
        static std::unique_ptr<CoordinatesConverter> buildConverter(Edge edge);
    };
}
