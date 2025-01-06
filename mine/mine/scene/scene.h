//
//  scene.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

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
    };
}
