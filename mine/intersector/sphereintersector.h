//
//  sphereintersector.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

class SphereIntersector {
public:
    template<typename T = float>
    std::vector<T> intersect(const Ray& ray, const Sphere& sphere) const {
        auto oc = ray.origin - sphere.center;
        T k1 = simd::dot(ray.direction, ray.direction);
        T k2 = 2 * simd::dot(oc, ray.direction);
        T k3 = simd::dot(oc, oc) - sphere.radius * sphere.radius;
        T discriminant = k2 * k2 - 4 * k1 * k3;
        
        if (discriminant < 0) {
            return {};
        }
        if (discriminant == 0) {
            T t = -k2 / (2 * k1);
            return {t};
        }
        T t1 = (-k2 + std::sqrt(discriminant)) / (2 * k1);
        T t2 = (-k2 - std::sqrt(discriminant)) / (2 * k1);
        return t1 > t2 ? std::vector<T>{t2, t1} : std::vector<T>{t1, t2};
    }
    bool isOnSphere(const simd::float3& point, const Sphere& sphere, float epsilon) {
        double a = std::pow(point.x - sphere.center.x, 2);
        double b = std::pow(point.y - sphere.center.y, 2);
        double c = std::pow(point.z - sphere.center.z, 2);
        double d = std::pow(sphere.radius, 2);
        double e = a + b + c;
        return std::abs(d - e) < epsilon;
    }
    simd::float3 sphericalToCartesian(float r,
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

    simd::float2 getSphericalCoordinates(const simd::float3& nonCenteredPoint, const Sphere& sphere) {
        assert(isOnSphere(nonCenteredPoint, sphere, 1e-1));
        
        simd::float3 point = nonCenteredPoint - sphere.center;
        double phi = std::atan2(point.z, point.x);
        double theta = std::acos(point.y / sphere.radius);
        
        assert(std::isfinite(theta));
        assert(std::isfinite(phi));
        
        return simd::make_float2(phi, theta);
    }
    
    simd::float2 getTextureCoordinates(const simd::float2& sphericalCoordinates) {
        double phi = sphericalCoordinates.x;
        double theta = sphericalCoordinates.y;
        double u = phi / (2 * M_PI) + 0.5;
        double v = 1.0 - theta / M_PI;
        
        assert(std::isfinite(u));
        assert(std::isfinite(v));
        
        return simd::make_float2(u, v);
    }
    
    simd::float2 getTextureCoordinates(const simd::float3& point, const Sphere& sphere) {
        simd::float2 coordinates = getSphericalCoordinates(point, sphere);
        return getTextureCoordinates(coordinates);
    }
};
