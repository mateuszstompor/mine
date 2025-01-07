//
//  sphereintersector.cpp
//
//  Created on 07/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "sphereintersector.h"

std::vector<float> mine::SphereIntersector::intersect(const Ray& ray, const Sphere& sphere, float epsilon) const {
    auto oc = ray.origin - sphere.center;
    float k1 = simd::dot(ray.direction, ray.direction);
    float k2 = 2 * simd::dot(oc, ray.direction);
    float k3 = simd::dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = k2 * k2 - 4 * k1 * k3;
    
    if (discriminant < 0) {
        return {};  // No intersection
    }
    
    if (std::abs(discriminant) < epsilon) {
        float t = -k2 / (2 * k1);
        return {t};  // One intersection
    }
    
    float sqrtDiscriminant = std::sqrt(discriminant);
    float t1 = (-k2 + sqrtDiscriminant) / (2 * k1);
    float t2 = (-k2 - sqrtDiscriminant) / (2 * k1);
    return t1 > t2 ? std::vector<float>{t2, t1} : std::vector<float>{t1, t2};  // Return sorted intersections
}
