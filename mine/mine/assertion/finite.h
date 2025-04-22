//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cassert>
#include <simd/simd.h>

namespace mine {
    inline void assertFinite(simd::float3 const & value) {
        assert(simd::isfinite(value.x));
        assert(simd::isfinite(value.y));
        assert(simd::isfinite(value.z));
    }
    inline void assertFinite(simd::float2 const & value) {
        assert(simd::isfinite(value.x));
        assert(simd::isfinite(value.y));
    }
}
