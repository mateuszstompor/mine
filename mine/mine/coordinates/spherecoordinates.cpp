//
//  spherecoordinates.cpp
//
//  Created on 07/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "spherecoordinates.h"

#include <cassert>

bool mine::SphereCoordinates::isOnSphere(const simd::float3& point, const Sphere& sphere, float epsilon) {
    double a = std::pow(point.x - sphere.center.x, 2);
    double b = std::pow(point.y - sphere.center.y, 2);
    double c = std::pow(point.z - sphere.center.z, 2);
    double d = std::pow(sphere.radius, 2);
    double e = a + b + c;
    return std::abs(d - e) < epsilon;
}
simd::float3 mine::SphereCoordinates::sphericalToCartesian(float r,
                                  float theta,
                                  float phi) {
    // Convert angles from degrees to radians if necessary
    // Comment this out if theta and phi are already in radians
    theta = theta * M_PI / 180.0;
    phi = phi * M_PI / 180.0;
    
    return simd::make_float3(r * std::sin(phi) * std::cos(theta),
                             r * std::sin(phi) * std::sin(theta),
                             r * std::cos(phi));
}

simd::float2 mine::SphereCoordinates::getSphericalCoordinates(const simd::float3& nonCenteredPoint, const Sphere& sphere) {
    assert(isOnSphere(nonCenteredPoint, sphere, 1e-1));
    
    simd::float3 point = nonCenteredPoint - sphere.center;
    double phi = std::atan2(point.z, point.x);
    double theta = std::acos(point.y / sphere.radius);
    
    assert(std::isfinite(theta));
    assert(std::isfinite(phi));
    
    return simd::make_float2(phi, theta);
}

bool mine::SphereCoordinates::isInsideSphere(simd::float3 const & point, Sphere const & sphere) {
    float a = point.x - sphere.center.x;
    float b = point.y - sphere.center.y;
    float c = point.z - sphere.center.z;
    float sum = pow(a, 2) + pow(b, 2) + pow(c, 2);
    float radiusSquared = pow(sphere.radius, 2);
    bool inside = sum <= radiusSquared;
    if (!inside) {
        return false;
    }
    return true;
}

simd::float2 mine::SphereCoordinates::getTextureCoordinates(const simd::float2& sphericalCoordinates) {
    double phi = sphericalCoordinates.x;
    double theta = sphericalCoordinates.y;
    double u = phi / (2 * M_PI) + 0.5;
    double v = 1.0 - theta / M_PI;
    
    assert(std::isfinite(u));
    assert(std::isfinite(v));
    
    return simd::make_float2(u, v);
}

simd::float2 mine::SphereCoordinates::getTextureCoordinates(const simd::float3& point, const Sphere& sphere) {
    simd::float2 coordinates = getSphericalCoordinates(point, sphere);
    return getTextureCoordinates(coordinates);
}
