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
        for (SphereObject const & sObject : s.spheres) {
            auto const & sphere = sObject.sphere;
            std::vector<float> allIntersections = sIntersector.intersect<float>(r, sphere);
            if (allIntersections.empty()) {
                continue;
            }
            float t = allIntersections[0];
            simd_float3 point = r.origin + r.direction * t;
            simd_float3 normal = simd_normalize(point - sphere.center);
            simd::float2 uv = sIntersector.getTextureCoordinates(point, sphere);
            assignIfCloser(closest, RayIntersection(normal, point, uv, sObject.material, t));
        }
        for (TriangleObject const & tObject : s.triangles) {
            std::optional<float> intersection = tIntersector.intersect(r, tObject.triangle);
            if (!intersection) {
                continue;
            }
            float t = (*intersection);
            simd_float3 point = r.origin + r.direction * t;
            simd::float2 uv = tIntersector.getTextureCoordinates(point, tObject.triangle);
            assignIfCloser(closest, RayIntersection(tObject.triangle.normal, point, uv, tObject.material, t));
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
