//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <vector>

#include "transformnodecontents.h"

#include "../triangle.h"
#include "../look/material.h"

namespace mine {
    struct TriangleNodeContents: public TransformNodeContents {
        std::vector<Triangle> triangles;
        std::shared_ptr<Material> material;
    };
}
