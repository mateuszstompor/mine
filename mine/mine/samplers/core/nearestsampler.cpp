//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "nearestsampler.h"

#include "../../assertion/range.h"

simd::float4 mine::NearestSampler::sample(simd::float2 uv,
                                          std::shared_ptr<Bitmap> const & texture) {
    assertInClosedRange(uv.x, 0.0f, 1.0f);
    assertInClosedRange(uv.y, 0.0f, 1.0f);
    
    uint16_t x = (texture->width - 1) * uv.x;
    uint16_t y = (texture->height - 1) * uv.y;
    
    return texture->get(x, y);
}
