//
//  sphereobject.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include "look/material.h"
#include "sphere.h"

namespace mine {
    struct SphereObject {
        Sphere sphere;
        std::shared_ptr<mine::Material> material;
    };
}
