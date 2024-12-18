//
//  triangleobject.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include "look/material.h"
#include "triangle.h"

struct TriangleObject {
    Triangle triangle;
    std::shared_ptr<Material> material;
};
