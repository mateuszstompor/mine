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
};

