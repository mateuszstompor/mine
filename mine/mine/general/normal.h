//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <simd/simd.h>

#include "../assertion/equal.h"
#include "../assertion/finite.h"

namespace mine {
    void generateTBForNormal(simd::float3 & tangent,
                             simd::float3 & bitangent,
                             simd::float3 const & normal);
}
