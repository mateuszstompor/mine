//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <tuple>

#include "intersector.h"

#include "../general/normal.h"


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
    float t = std::get<0>(closest);
    if (t == std::numeric_limits<float>::infinity()) {
        return std::nullopt;
    }
    assert(t >= 0);
    simd_float3 point = r.origin + r.direction * t;
    if (std::get<2>(closest) == IntersectionKind::sphereLight) {
        OmniLight & sObject = *(OmniLight *)std::get<1>(closest);
        Sphere & sphere = sObject.representation;
        simd_float3 normal = simd_normalize(point - sphere.center);
        simd::float2 uv = simd_float2(0);
        simd::float3 tangent, bitangent;
        mine::generateTBForNormal(tangent, bitangent, normal);
        return RayIntersection(tangent,
                               bitangent,
                               normal,
                               point,
                               uv,
                               nullptr,
                               std::make_shared<simd::float3>(sObject.color),
                               t);
    } else if (std::get<2>(closest) == IntersectionKind::sphere) {
        SphereObject & sObject = *(SphereObject *)std::get<1>(closest);
        Sphere & sphere = sObject.sphere;
        simd_float3 normal = simd_normalize(point - sphere.center);
        simd::float2 uv = sCoordinates.getTextureCoordinates(point, sphere);
        simd::float3 tangent, bitangent;
        mine::generateTBForNormal(tangent, bitangent, normal);
        return RayIntersection(tangent, bitangent, normal, point, uv, sObject.material, nullptr, t);
    } else {
        TriangleObject & tObject = *(TriangleObject *)std::get<1>(closest);
        simd::float2 uv = tCoordinates.getTextureCoordinates(point, tObject.triangle);
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
