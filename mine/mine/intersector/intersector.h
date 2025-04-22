//
//  intersector.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <optional>

#include "sphereintersector.h"
#include "triangleintersector.h"

#include "../coordinates/spherecoordinates.h"
#include "../coordinates/trianglecoordinates.h"

#include "../scene/rayintersection.h"
#include "../scene/scene.h"

namespace mine {
    class Intersector {
        enum class IntersectionKind {
            sphere,
            sphereLight,
            triangle,
            invalid
        };
    public:
        std::optional<RayIntersection> closestIntersection(Scene const & s,
                                                           Ray const & r);
    private:
        SphereIntersector sIntersector;
        SphereCoordinates sCoordinates;
        TriangleIntersector tIntersector;
        TriangleCoordinates tCoordinates;
    };
}
