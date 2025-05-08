//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <vector>

#include "transformnodecontents.h"

#include "../primitive/mesh.h"

namespace mine {
    struct MeshNodeContents: public TransformNodeContents {
        Mesh mesh;
    };
}
