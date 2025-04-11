//
//  sphereintersector.cpp
//
//  Created on 07/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <vector>
#include <algorithm>

#include "sphereintersector.h"

std::optional<float> mine::SphereIntersector::closestIntersection(const Ray& ray, const Sphere& sphere, float epsilon) const {
    auto oc = ray.origin - sphere.center;
    float k1 = simd::dot(ray.direction, ray.direction);
    float k2 = 2 * simd::dot(oc, ray.direction);
    float k3 = simd::dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = k2 * k2 - 4 * k1 * k3;
    
    if (discriminant < 0) {
        return std::nullopt;  // No intersection
    }
    
    if (std::abs(discriminant) < epsilon) {
        float t = -k2 / (2 * k1);
        return t >= 0 ? std::make_optional(t) : std::nullopt;  // One intersection
    }
    
    float sqrtDiscriminant = std::sqrt(discriminant);
    float t1 = (-k2 + sqrtDiscriminant) / (2 * k1);
    float t2 = (-k2 - sqrtDiscriminant) / (2 * k1);
    std::vector<float> solutions;
    solutions.reserve(2);
    if (t1 > 0) {
        solutions.push_back(t1);
    }
    if (t2 > 0) {
        solutions.push_back(t2);
    }
    std::sort(solutions.begin(), solutions.end(), [](float a, float b) {
        return a < b;
    });
    return solutions.empty() ? std::nullopt : std::make_optional(solutions[0]);
}
