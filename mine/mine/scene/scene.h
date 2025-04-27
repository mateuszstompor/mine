//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <optional>
#include <vector>

#include "light/omnilight.h"
#include "sphereobject.h"
#include "camera.h"
#include "triangleobject.h"

namespace mine {
    struct Scene {
        Camera camera;
        std::vector<OmniLight> omnilights;
        std::vector<SphereObject> spheres;
        std::vector<TriangleObject> triangles;
        std::optional<Bitmap> environmentMap = std::nullopt;
    };
}
