//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include "./core/nearestsampler.h"
#include "./core/linearsampler.h"

#include "./conversion/pcoordinatesconverter.h"
#include "./conversion/ctecoordinatesconverter.h"
#include "./conversion/rcoordinatesconverter.h"
#include "./conversion/rmcoordinatesconverter.h"

namespace mine {
    enum class Filter {
        Linear,
        Nearest
    };
    enum class Edge {
        ClampToEdge,
        ZeroEdge,
        Repeat,
        RepeatMirrored,
    };
    class TextureSampler {
    private:
        Edge edge;
        std::unique_ptr<CoordinatesConverter> converter;
        std::unique_ptr<Sampler> sampler;
    public:
        TextureSampler(Filter wantedFilter, Edge wantedEdge)
        : edge{wantedEdge} {
            converter = buildConverter(wantedEdge);
            sampler = buildSampler(wantedFilter);
        }
        simd::float4 sample(float u, float v, mine::Bitmap const & texture) {
            if (edge == Edge::ZeroEdge) {
                if (u > 1.0f || u < 0.0f) {
                    return simd_make_float4(0.0f, 0.0f, 0.0f, 1.0f);
                }
                if (v > 1.0f || v < 0.0f) {
                    return simd_make_float4(0.0f, 0.0f, 0.0f, 1.0f);
                }
            }
            simd::float2 convertedCoordinates = converter->convert(simd_make_float2(u, v));
            return sampler->sample(convertedCoordinates.x, convertedCoordinates.y, texture);
        }
    private:
        static std::unique_ptr<Sampler> buildSampler(Filter filter) {
            if (filter == Filter::Linear) {
                return std::make_unique<LinearSampler>();
            } else {
                return std::make_unique<NearestSampler>();
            }
        }
        static std::unique_ptr<CoordinatesConverter> buildConverter(Edge edge) {
            switch (edge) {
            case Edge::ZeroEdge:
                return std::make_unique<PassthroughCoordinatesConverter>();
            case Edge::ClampToEdge:
                return std::make_unique<ClampToEdgeCoordinatesConverter>();
            case Edge::RepeatMirrored:
                return std::make_unique<RepeatMirroredCoordinatesConverter>();
            case Edge::Repeat:
                return std::make_unique<RepeatCoordinatesConverter>();
            }
        }
    };
}
