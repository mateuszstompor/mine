//
//  sphereintersector.cpp
//
//  Created on 07/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <vector>
#include <algorithm>

#include "../assertion/equal.h"
#include "sphereintersector.h"

std::optional<float> mine::SphereIntersector::closestIntersection(const Ray& ray,
                                                                  const Sphere& sphere,
                                                                  float epsilon) const {
    assertEqual(simd::length(ray.direction), 1.0f, 1e-4f);
    auto oc = ray.origin - sphere.center;
    float k2 = 2 * simd::dot(oc, ray.direction);
    float k3 = simd::dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = k2 * k2 - 4 * k3;
    
    if (discriminant < 0) {
        return std::nullopt;  // No intersection
    }
    
    if (discriminant < epsilon) {
        float t = -k2 / 2.0f;
        return t >= 0 ? std::make_optional(t) : std::nullopt;  // One intersection
    }
    
    float sqrtDiscriminant = std::sqrt(discriminant);
    float t2 = (-k2 - sqrtDiscriminant) / 2.0f;
    if (t2 >= 0) return std::make_optional(t2);
    float t1 = (-k2 + sqrtDiscriminant) / 2.0f;
    return t1 >= 0 ? std::make_optional(t1) : std::nullopt;
}
