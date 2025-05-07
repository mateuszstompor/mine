//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <string>
#include <simd/simd.h>

namespace mine {
    struct TransformNodeContents {
        std::string name = "";
        simd::float4x4 transform = matrix_identity_float4x4;
        virtual ~TransformNodeContents() = default;
    };
}
