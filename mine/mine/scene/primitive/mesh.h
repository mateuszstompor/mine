//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <vector>

#include "./submesh.h"
#include "./vertex.h"

namespace mine {
    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<Submesh> submeshes;
    };
}
