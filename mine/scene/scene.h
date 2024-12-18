//
//  scene.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <vector>

#include "light/omnilight.h"
#include "sphere.h"
#include "camera.h"

struct Scene {
    Camera camera;
    std::vector<OmniLight> omnilights;
    std::vector<Sphere> spheres;
};
