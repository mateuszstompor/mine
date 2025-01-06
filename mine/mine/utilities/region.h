//
//  region.h
//
//  Created on 06/01/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "closedrange.h"

namespace mine {
    template <typename T>
    struct Region {
        ClosedRange<T> x;
        ClosedRange<T> y;
    };
}
