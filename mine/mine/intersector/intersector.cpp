//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <tuple>
#include <cstdlib>

#include "intersector.h"

#include "../general/normal.h"


std::optional<mine::RayIntersection> mine::Intersector::closestIntersection(mine::Scene const & s,
                                                                            Ray const & r) {
    float closestT = std::numeric_limits<float>::infinity();
    void const * closestObject = nullptr;
    IntersectionKind closestKind = IntersectionKind::invalid;
    for (OmniLight const & sObject : s.omnilights) {
        float intersection = sIntersector.closestIntersection(r, sObject.representation);
        if (intersection < 0) {
            continue;
        }
        if (closestT > intersection) {
            closestT = intersection;
            closestObject = reinterpret_cast<void const *>(&sObject);
            closestKind = IntersectionKind::sphereLight;
        }
    }
    for (SphereObject const & sObject : s.spheres) {
        float intersection = sIntersector.closestIntersection(r, sObject.sphere);
        if (intersection < 0) {
            continue;
        }
        if (closestT > intersection) {
            closestT = intersection;
            closestObject = reinterpret_cast<void const *>(&sObject);
            closestKind = IntersectionKind::sphere;
        }
    }
    for (TriangleObject const & tObject : s.triangles) {
        float intersection = tIntersector.intersect(r, tObject.triangle);
        if (intersection < 0) {
            continue;
        }
        if (closestT > intersection) {
            closestT = intersection;
            closestObject = reinterpret_cast<void const *>(&tObject);
            closestKind = IntersectionKind::triangle;
        }
        
    }
    if (closestT == std::numeric_limits<float>::infinity()) {
        return std::nullopt;
    }
    assert(closestT >= 0 && "Must be in front of the ray origin");
    simd::float3 point = r.origin + r.direction * closestT;
    if (closestKind == IntersectionKind::sphereLight) {
        OmniLight const & sObject = * reinterpret_cast<OmniLight const *>(closestObject);
        Sphere const & sphere = sObject.representation;
        simd::float3 normal = simd::normalize(point - sphere.center);
        simd::float3 tangent, bitangent;
        mine::generateTBForNormal(tangent, bitangent, normal);
        return RayIntersection(tangent,
                               bitangent,
                               normal,
                               point,
                               simd::make_float2(0.0f, 0.0f),
                               nullptr,
                               sObject.color,
                               closestT);
    } else if (closestKind == IntersectionKind::sphere) {
        SphereObject const & sObject = *reinterpret_cast<SphereObject const *>(closestObject);
        Sphere const & sphere = sObject.sphere;
        simd::float3 normal = simd::normalize(point - sphere.center);
        simd::float2 uv = sCoordinates.getTextureCoordinates(point, sphere);
        simd::float3 tangent, bitangent;
        mine::generateTBForNormal(tangent, bitangent, normal);
        return RayIntersection(tangent,
                               bitangent,
                               normal,
                               point,
                               uv,
                               sObject.material.get(),
                               std::nullopt,
                               closestT);
    } else {
        TriangleObject const & tObject = *reinterpret_cast<TriangleObject const *>(closestObject);
        simd::float2 uv = tCoordinates.getTextureCoordinates(point, tObject.triangle);
        return RayIntersection(tObject.triangle.tangent,
                               tObject.triangle.bitangent,
                               tObject.triangle.normal,
                               point,
                               uv,
                               tObject.material.get(),
                               std::nullopt,
                               closestT);
    }
    return std::nullopt;
}
