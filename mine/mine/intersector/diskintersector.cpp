//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "diskintersector.h"

std::optional<float> mine::DiskIntersector::intersect(const mine::Ray& ray,
                                                      const mine::Disk& disk) {
    float a = simd::dot(disk.origin - ray.origin, disk.normal);
    float b = simd::dot(ray.direction, disk.normal);
    assert(b != 0);
    float t = a / b;
    
    simd_float3 point = ray.origin + ray.direction * t;
    simd_float3 distance = point - disk.origin;
    
    if (simd::dot(distance, distance) <= pow(disk.radius, 2)) {
        assert(simd::length(point - disk.origin) <= disk.radius);
        return t;
    }
    return std::nullopt;
}
