//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <simd/simd.h>

namespace mine {
    class Sampler {
    public:
        virtual simd::float4 sample(simd::float2 const & uv,
                                    mine::Bitmap const * texture) = 0;
        virtual ~Sampler() = default;
    };
}
