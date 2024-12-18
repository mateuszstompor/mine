//
//  intersector.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <optional>

#include "sphereintersector.h"
#include "../scene/rayintersection.h"

class Intersector {
public:
    std::optional<RayIntersection> closestIntersection(Scene const & s, Ray const & r) {
        for (Sphere const & sphere : s.spheres) {
            std::vector<double> allIntersections = sIntersector.intersect(r, sphere);
            for (double t : allIntersections) {
                simd_float3 point = r.origin + r.direction * t;
                simd_float3 normal = point - sphere.center;
                return RayIntersection(normal, point, t);
            }
        }
        return std::nullopt;
    }
private:
    SphereIntersector sIntersector;
};
