//
//  raytracer.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <cstdint>

#include <simd/simd.h>

#include "scene/ray.h"
#include "scene/sphere.h"


class SphereIntersector {
public:
    std::vector<double> intersect(const Ray& ray, const Sphere& sphere) const {
        auto oc = ray.origin - sphere.center;
        double k1 = simd::dot(ray.direction, ray.direction);
        double k2 = 2 * simd::dot(oc, ray.direction);
        double k3 = simd::dot(oc, oc) - sphere.radius * sphere.radius;
        double discriminant = k2 * k2 - 4 * k1 * k3;

        if (discriminant < 0) {
            return {};
        }
        if (discriminant == 0) {
            double t = -k2 / (2 * k1);
            return {t};
        }
        double t1 = (-k2 + std::sqrt(discriminant)) / (2 * k1);
        double t2 = (-k2 - std::sqrt(discriminant)) / (2 * k1);
        return t1 > t2 ? std::vector<double>{t2, t1} : std::vector<double>{t1, t2};
    }

//    bool isOnSphere(const std::array<double, 3>& point, const Sphere& sphere, double epsilon) const {
//        double a = std::pow(point[0] - sphere.center[0], 2);
//        double b = std::pow(point[1] - sphere.center[1], 2);
//        double c = std::pow(point[2] - sphere.center[2], 2);
//        double d = std::pow(sphere.radius, 2);
//        double e = a + b + c;
//        return std::abs(d - e) < epsilon;
//    }

//    std::array<double, 2> getSphericalCoordinates(const std::array<double, 3>& nonCenteredPoint, const Sphere& sphere) const {
//        assert(isOnSphere(nonCenteredPoint, sphere, 0.1));
//        auto point = subtract(nonCenteredPoint, sphere.center);
//        double phi = std::atan2(point[2], point[0]);
//        double theta = std::acos(point[1] / sphere.radius);
//        assert(std::isfinite(theta));
//        assert(std::isfinite(phi));
//        return {phi, theta};
//    }
//
//    std::array<double, 2> getTextureCoordinates(const std::array<double, 2>& sphericalCoordinates) const {
//        double phi = sphericalCoordinates[0];
//        double theta = sphericalCoordinates[1];
//        double u = phi / (2 * M_PI) + 0.5;
//        double v = 1.0 - theta / M_PI;
//        assert(std::isfinite(u));
//        assert(std::isfinite(v));
//        return {u, v};
//    }
//
//    std::array<double, 2> getTextureCoordinates(const std::array<double, 3>& point, const Sphere& sphere) const {
//        auto coordinates = getSphericalCoordinates(point, sphere);
//        return getTextureCoordinates(coordinates);
//    }

private:
//    std::array<double, 3> subtract(const std::array<double, 3>& a, const std::array<double, 3>& b) const {
//        return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
//    }
//
//    double dot(const std::array<double, 3>& a, const std::array<double, 3>& b) const {
//        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
//    }
};

class RayTracer {
public:
    simd_float4 trace(uint16_t x, uint16_t y) {
        simd_float4 color{0, 0, 0, 1};
        Ray r(simd_make_float3(x, y, 0),
              simd_normalize(simd_make_float3(0, 0, 1)));
        Sphere s(40, {500, 500, 200});
        
        SphereIntersector i;
        auto t = i.intersect(r, s);
        if (t.size() != 0) {
            return {1, 0, 0, 1};
        }
        
        
        return color;
    }
};
