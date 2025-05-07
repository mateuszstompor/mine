//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "transformnodecontents.h"

#include "../sphereobject.h"

namespace mine {
    struct SphereNodeContents: public TransformNodeContents {
        SphereObject sphereObject;
    };
}
