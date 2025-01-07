//
//  sphereintersector.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <vector>

#include "../scene/ray/ray.h"
#include "../scene/sphere.h"

namespace mine {
    class SphereIntersector {
    public:
        std::vector<float> intersect(const Ray& ray,
                                     const Sphere& sphere,
                                     float epsilon = 1e-6) const;
    };
}
