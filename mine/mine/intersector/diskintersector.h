//
//  diskintersector.h
//
//  Created on 20/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <simd/simd.h>
#include <optional>

#include "../scene/ray/ray.h"
#include "../scene/disk.h"
#include "../assertion/equal.h"

namespace mine {
    class DiskIntersector {
    public:
        std::optional<float> intersect(const Ray& ray,
                                       const Disk& disk);
    };
}
