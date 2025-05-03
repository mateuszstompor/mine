//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <simd/simd.h>

namespace mine {
    class Sampler {
    public:
        virtual simd::float4 sample(float u, float v, const mine::Bitmap & texture) = 0;
        virtual ~Sampler() = default;
    };
}
