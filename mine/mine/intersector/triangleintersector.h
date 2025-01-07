//
//  triangleintersector.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <cmath>
#include <cassert>
#include <array>
#include <simd/simd.h>

#include "../scene/triangle.h"
#include "../scene/ray/ray.h"

namespace mine {
    class TriangleIntersector {
    public:
        std::optional<float> intersect(const Ray& ray,
                                       const Triangle& triangle,
                                       float epsilon = 1e-3) const;
    };
}
