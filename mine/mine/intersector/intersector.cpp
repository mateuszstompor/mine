//
//  intersector.cpp
//
//  Created on 07/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "intersector.h"

#include "../coordinates/spherecoordinates.h"
#include "../coordinates/trianglecoordinates.h"

std::optional<mine::RayIntersection> mine::Intersector::closestIntersection(mine::Scene const & s,
                                                                            Ray const & r) {
    std::optional<RayIntersection> closest = std::nullopt;
    for (OmniLight const & sObject : s.omnilights) {
        auto const & sphere = sObject.representation;
        std::optional<float> intersection = sIntersector.closestIntersection(r, sphere);
        if (!intersection) {
            continue;
        }
        float t = *intersection;
        simd_float3 point = r.origin + r.direction * t;
        simd_float3 normal = simd_normalize(point - sphere.center);
        // TODO: Figure out
        // simd::float2 uv = sIntersector.getTextureCoordinates(point, sphere);
        simd::float2 uv = simd_float2(0);
        simd::float3 A = simd::make_float3(0, 0, 1) != normal ? simd::make_float3(0, 0, 1) : simd::make_float3(1, 0, 0);
        simd::float3 tmp = simd::cross(A, normal);
        simd::float3 tangent = tmp / simd::length(tmp);
        simd::float3 bitangent = simd::normalize(simd::cross(normal, tangent));
        assignIfCloser(closest, RayIntersection(tangent, bitangent, normal, point, uv, nullptr, t));
    }
    for (SphereObject const & sObject : s.spheres) {
        auto const & sphere = sObject.sphere;
        std::optional<float> intersection = sIntersector.closestIntersection(r, sphere);
        if (!intersection) {
            continue;
        }
        float t = *intersection;
        simd_float3 point = r.origin + r.direction * t;
        simd_float3 normal = simd_normalize(point - sphere.center);
        SphereCoordinates sc;
        simd::float2 uv = sc.getTextureCoordinates(point, sphere);
        simd::float3 A = simd::make_float3(0, 0, 1) != normal ? simd::make_float3(0, 0, 1) : simd::make_float3(1, 0, 0);
        simd::float3 tmp = simd::cross(A, normal);
        simd::float3 tangent = tmp / simd::length(tmp);
        simd::float3 bitangent = simd::normalize(simd::cross(normal, tangent));
        assignIfCloser(closest, RayIntersection(tangent, bitangent, normal, point, uv, sObject.material, t));
    }
    for (TriangleObject const & tObject : s.triangles) {
        std::optional<float> intersection = tIntersector.intersect(r, tObject.triangle);
        if (!intersection) {
            continue;
        }
        float t = (*intersection);
        simd_float3 point = r.origin + r.direction * t;
        TriangleCoordinates tc;
        simd::float2 uv = tc.getTextureCoordinates(point, tObject.triangle);
        
        assignIfCloser(closest, RayIntersection(tObject.triangle.tangent,
                                                tObject.triangle.bitangent,
                                                tObject.triangle.normal,
                                                point,
                                                uv,
                                                tObject.material,
                                                t));
    }
    
    if (closest != std::nullopt) {
        assert(closest->t >= 0);
    }
    return closest;
}

void mine::Intersector::assignIfCloser(std::optional<RayIntersection> & currentBest,
                                       RayIntersection const & newIntersection) {
    if (!currentBest || (currentBest->t > newIntersection.t && newIntersection.t >= 0)) {
        currentBest = newIntersection;
    }
}
