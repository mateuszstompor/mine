//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "texturesampler.h"

#include "./core/nearestsampler.h"
#include "./core/linearsampler.h"

#include "./conversion/pcoordinatesconverter.h"
#include "./conversion/ctecoordinatesconverter.h"
#include "./conversion/rcoordinatesconverter.h"
#include "./conversion/rmcoordinatesconverter.h"

mine::TextureSampler::TextureSampler(Filter wantedFilter, Edge wantedEdge)
: edge{wantedEdge} {
    converter = buildConverter(wantedEdge);
    sampler = buildSampler(wantedFilter);
}

simd::float4 mine::TextureSampler::sample(simd::float2 uv,
                                          std::shared_ptr<Bitmap> const & texture) {
    if (edge == Edge::ZeroEdge) {
        if (uv.x > 1.0f || uv.x < 0.0f) {
            return simd::make_float4(0.0f, 0.0f, 0.0f, 1.0f);
        }
        if (uv.y > 1.0f || uv.y < 0.0f) {
            return simd::make_float4(0.0f, 0.0f, 0.0f, 1.0f);
        }
    }
    simd::float2 convertedCoordinates = converter->convert(uv);
    return sampler->sample(convertedCoordinates, texture);
}

std::unique_ptr<mine::Sampler> mine::TextureSampler::buildSampler(Filter filter) {
    if (filter == Filter::Linear) {
        return std::make_unique<LinearSampler>();
    } else {
        return std::make_unique<NearestSampler>();
    }
}

std::unique_ptr<mine::CoordinatesConverter> mine::TextureSampler::buildConverter(Edge edge) {
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
