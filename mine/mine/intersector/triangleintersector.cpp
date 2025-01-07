//
//  triangleintersector.cpp
//  mine
//
//  Created by Motion VFX on 07/01/2025.
//

#include "triangleintersector.h"

std::optional<float> mine::TriangleIntersector::intersect(const Ray& ray,
                                                          const Triangle& triangle,
                                                          float epsilon) const {
    simd::float3 origin = (triangle.v0 + triangle.v1 + triangle.v2) / 3.0;
    float a = simd::dot(origin - ray.origin, triangle.normal);
    float b = simd::dot(ray.direction, triangle.normal);
    
    if (b == 0.0) {
        return std::nullopt;
    }
    
    float t = a / b;
    if (t < 0) {
        return std::nullopt;
    }
    
    simd::float3 point = ray.origin + ray.direction * t;
    
    simd::float3 v2v0 = triangle.v2 - triangle.v0;
    simd::float3 pV0 = point - triangle.v0;
    simd::float3 c1 = simd::cross(pV0, v2v0);
    
    if (simd::dot(c1, triangle.normal) < -epsilon) {
        return std::nullopt;
    }
    
    simd::float3 v1v0 = triangle.v1 - triangle.v0;
    simd::float3 c2 = simd::cross(v1v0, pV0);
    
    if (simd::dot(c2, triangle.normal) < -epsilon) {
        return std::nullopt;
    }
    
    simd::float3 v2v1 = triangle.v2 - triangle.v1;
    simd::float3 pV1 = point - triangle.v1;
    simd::float3 c3 = simd::cross(v2v1, pV1);
    
    if (simd::dot(c3, triangle.normal) < -epsilon) {
        return std::nullopt;
    }
    
    assert(std::isfinite(t));
    return t;
}
