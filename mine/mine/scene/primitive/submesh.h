//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <vector>

#include "../look/material.h"

namespace mine {
    struct Submesh {
        std::vector<uint32_t> indices;
        std::shared_ptr<Material> material;
    };
}
