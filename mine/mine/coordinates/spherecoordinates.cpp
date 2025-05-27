//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "spherecoordinates.h"

#include <cassert>

#include "../general/angle.h"
#include "../assertion/equal.h"
#include "../assertion/range.h"
#include "../math/constants.h"

bool mine::SphereCoordinates::isOnSphere(const simd::float3& point,
                                         const Sphere& sphere,
                                         float epsilon) const {
    simd::float3 diff = point - sphere.center;
    float distSquared = simd::dot(diff, diff);
    float radiusSquared = sphere.radius * sphere.radius;
    return std::abs(distSquared - radiusSquared) < epsilon;
}

simd::float3 mine::SphereCoordinates::sphericalToCartesian(float r,
                                                           float thetaDegrees,
                                                           float phiDegrees) const {
    // Convert angles from degrees to radians if necessary
    // Comment this out if theta and phi are already in radians
    assert(r > 0.0f);
    float theta = degreesToRadians(thetaDegrees);
    float phi = degreesToRadians(phiDegrees);

    float sinPhi = std::sin(phi);
    float sinPhiR = r * sinPhi;
    
    return simd::make_float3(sinPhiR * std::cos(theta),
                             sinPhiR * std::sin(theta),
                             r * std::cos(phi));
}

simd::float2 mine::SphereCoordinates::getSphericalCoordinates(const simd::float3& nonCenteredPoint,
                                                              const Sphere& sphere) const {
    assert(isOnSphere(nonCenteredPoint, sphere, 5e-1f));
    
    simd::float3 point = nonCenteredPoint - sphere.center;
    float phi = std::atan2(point.z, point.x);
    float thetaInput = simd::clamp(point.y / sphere.radius, -1.0f, 1.0f);
    float theta = std::acos(thetaInput);
    
    assert(std::isfinite(theta));
    assert(std::isfinite(phi));
    
    return simd::make_float2(phi, theta);
}

bool mine::SphereCoordinates::isInsideSphere(simd::float3 const & point,
                                             Sphere const & sphere) const {
    float a = point.x - sphere.center.x;
    float b = point.y - sphere.center.y;
    float c = point.z - sphere.center.z;
    float sum = pow(a, 2.0f) + pow(b, 2.0f) + pow(c, 2.0f);
    float radiusSquared = pow(sphere.radius, 2.0f);
    return sum <= radiusSquared;
}

simd::float2 mine::SphereCoordinates::getTextureCoordinates(simd::float2 sphericalCoordinates) const {
    float u = sphericalCoordinates.x * INV_TWO_PI + 0.5f;
    float v = 1.0f - sphericalCoordinates.y * INV_PI;
    
    assert(std::isfinite(u));
    assert(std::isfinite(v));
    
    return simd::make_float2(u, v);
}

simd::float2 mine::SphereCoordinates::getTextureCoordinates(const simd::float3& point,
                                                            const Sphere& sphere) const {
    simd::float2 coordinates = getSphericalCoordinates(point, sphere);
    return getTextureCoordinates(coordinates);
}

simd::float2 mine::SphereCoordinates::getEquirectangularCoordinates(const simd::float3& direction) const {
    assertEqual(simd::length(direction), 1.0f, 1e-4f);
    assertInClosedRange(direction.y, -1.0f, 1.0f);
    float u = atan2(direction.z, direction.x) * INV_TWO_PI + 0.5f;
    float v = acos(direction.y) * INV_PI;
    return simd::make_float2(u, v);
}
