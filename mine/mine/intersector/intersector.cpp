//
//  intersector.cpp
//
//  Created on 07/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <tuple>

#include "intersector.h"

#include "../coordinates/spherecoordinates.h"
#include "../coordinates/trianglecoordinates.h"

std::optional<mine::RayIntersection> mine::Intersector::closestIntersection(mine::Scene const & s,
                                                                            Ray const & r) {
    std::tuple<float, void *, IntersectionKind> closest(std::numeric_limits<float>::infinity(),
                                                        nullptr,
                                                        IntersectionKind::invalid);
    for (OmniLight const & sObject : s.omnilights) {
        auto const & sphere = sObject.representation;
        std::optional<float> intersection = sIntersector.closestIntersection(r, sphere);
        if (!intersection) {
            continue;
        }
        if (std::get<0>(closest) > *intersection) {
            closest = std::make_tuple(*intersection,
                                      (void *)&sObject,
                                      IntersectionKind::sphereLight);
        }
    }
    for (SphereObject const & sObject : s.spheres) {
        auto const & sphere = sObject.sphere;
        std::optional<float> intersection = sIntersector.closestIntersection(r, sphere);
        if (!intersection) {
            continue;
        }
        if (std::get<0>(closest) > *intersection) {
            closest = std::make_tuple(*intersection,
                                      (void *)&sObject,
                                      IntersectionKind::sphere);
        }
    }
    for (TriangleObject const & tObject : s.triangles) {
        std::optional<float> intersection = tIntersector.intersect(r, tObject.triangle);
        if (!intersection) {
            continue;
        }
        if (std::get<0>(closest) > *intersection) {
            closest = std::make_tuple(*intersection,
                                      (void *)&tObject,
                                      IntersectionKind::triangle);
        }
        
    }
    if (std::get<0>(closest) == std::numeric_limits<float>::infinity()) {
        return std::nullopt;
    }
    assert(std::get<0>(closest) >= 0);
    if (std::get<2>(closest) == IntersectionKind::sphereLight) {
        OmniLight & sObject = *(OmniLight *)std::get<1>(closest);
        Sphere & sphere = sObject.representation;
        float t = std::get<0>(closest);
        simd_float3 point = r.origin + r.direction * t;
        simd_float3 normal = simd_normalize(point - sphere.center);
        simd::float2 uv = simd_float2(0);
        simd::float3 A = simd::make_float3(0, 0, 1) != normal ? simd::make_float3(0, 0, 1) : simd::make_float3(1, 0, 0);
        simd::float3 tmp = simd::cross(A, normal);
        simd::float3 tangent = tmp / simd::length(tmp);
        simd::float3 bitangent = simd::normalize(simd::cross(normal, tangent));
        return RayIntersection(tangent,
                               bitangent,
                               normal,
                               point,
                               uv,
                               nullptr,
                               std::make_shared<simd::float3>(sObject.color),
                               t);
    } else if (std::get<2>(closest) == IntersectionKind::sphere) {
        float t = std::get<0>(closest);
        SphereObject & sObject = *(SphereObject *)std::get<1>(closest);
        Sphere & sphere = sObject.sphere;
        simd_float3 point = r.origin + r.direction * t;
        simd_float3 normal = simd_normalize(point - sphere.center);
        SphereCoordinates sc;
        simd::float2 uv = sc.getTextureCoordinates(point, sphere);
        simd::float3 A = simd::make_float3(0, 0, 1) != normal ? simd::make_float3(0, 0, 1) : simd::make_float3(1, 0, 0);
        simd::float3 tmp = simd::cross(A, normal);
        simd::float3 tangent = tmp / simd::length(tmp);
        simd::float3 bitangent = simd::normalize(simd::cross(normal, tangent));
        return RayIntersection(tangent, bitangent, normal, point, uv, sObject.material, nullptr, t);
    } else {
        float t = std::get<0>(closest);
        TriangleObject & tObject = *(TriangleObject *)std::get<1>(closest);
        simd_float3 point = r.origin + r.direction * t;
        TriangleCoordinates tc;
        simd::float2 uv = tc.getTextureCoordinates(point, tObject.triangle);
        return RayIntersection(tObject.triangle.tangent,
                               tObject.triangle.bitangent,
                               tObject.triangle.normal,
                               point,
                               uv,
                               tObject.material,
                               nullptr,
                               t);
    }
}
