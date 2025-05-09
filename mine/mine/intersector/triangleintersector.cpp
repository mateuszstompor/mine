//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "triangleintersector.h"

std::optional<float> mine::TriangleIntersector::intersect(Ray const & ray,
                                                          Triangle const & triangle) const {
    const simd::float3 edge1 = triangle.v1 - triangle.v0;
    const simd::float3 edge2 = triangle.v2 - triangle.v0;

    const simd::float3 h = simd::cross(ray.direction, edge2);
    const float a = simd::dot(edge1, h);

    if (std::abs(a) < 0) {
        return std::nullopt;  // Ray is parallel to triangle.
    }

    const float f = 1.0f / a;
    const simd::float3 s = ray.origin - triangle.v0;
    const float u = f * simd::dot(s, h);
    if (u < 0.0f || u > 1.0f) {
        return std::nullopt;
    }

    const simd::float3 q = simd::cross(s, edge1);
    const float v = f * simd::dot(ray.direction, q);
    if (v < 0.0f || u + v > 1.0f) {
        return std::nullopt;
    }

    const float t = f * simd::dot(edge2, q);
    if (t < 0.0f) {
        return std::nullopt;  // Triangle is behind the ray.
    }

    assert(std::isfinite(t));
    return t;
}
