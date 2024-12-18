//
//  intersector.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <optional>

#include "sphereintersector.h"
#include "triangleintersector.h"
#include "../scene/rayintersection.h"

class Intersector {
public:
    std::optional<RayIntersection> closestIntersection(Scene const & s, Ray const & r) {
        std::optional<RayIntersection> closest = std::nullopt;
        for (Sphere const & sphere : s.spheres) {
            std::vector<float> allIntersections = sIntersector.intersect<float>(r, sphere);
            if (allIntersections.empty()) {
                continue;
            }
            float t = allIntersections[0];
            simd_float3 point = r.origin + r.direction * t;
            simd_float3 normal = simd_normalize(point - sphere.center);
            simd::float2 uv = sIntersector.getTextureCoordinates(point, sphere);
            assignIfCloser(closest, RayIntersection(normal, point, uv, t));
        }
        for (Triangle const & triangle : s.triangles) {
            std::optional<float> intersection = tIntersector.intersect(r, triangle);
            if (!intersection) {
                continue;
            }
            float t = (*intersection);
            simd_float3 point = r.origin + r.direction * t;
            simd::float2 uv = tIntersector.getTextureCoordinates(point, triangle);
            assignIfCloser(closest, RayIntersection(triangle.normal, point, uv, t));
        }
        
        return closest;
    }
private:
    void assignIfCloser(std::optional<RayIntersection> & currentBest,
                        RayIntersection const & newIntersection) {
        if (!currentBest || currentBest->t > newIntersection.t) {
            currentBest = newIntersection;
        }
    }
    SphereIntersector sIntersector;
    TriangleIntersector tIntersector;
};
