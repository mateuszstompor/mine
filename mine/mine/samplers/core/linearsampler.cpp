//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "linearsampler.h"

#include "../../assertion/range.h"

simd::float4 mine::LinearSampler::sample(simd::float2 uv,
                                         std::shared_ptr<Bitmap> const & texture) {
    assertInClosedRange(uv.x, 0.0f, 1.0f);
    assertInClosedRange(uv.y, 0.0f, 1.0f);
    float x = uv.x * (texture->width - 1);
    float y = uv.y * (texture->height - 1);

    float fX = std::floor(x);
    float cX = std::ceil(x);
    float fY = std::floor(y);
    float cY = std::ceil(y);
    
    simd::float4 p1 = texture->get(fX, fY);
    simd::float4 p2 = texture->get(cX, fY);
    
    simd::float4 p3 = texture->get(fX, cY);
    simd::float4 p4 = texture->get(cX, cY);
    
    float xFrac = x - fX;
    float yFrac = y - fY;
    
    simd::float4 f1 = simd::lerp(p1, p2, simd::float4(xFrac));
    simd::float4 f2 = simd::lerp(p3, p4, simd::float4(xFrac));
    simd::float4 f3 = simd::lerp(f1, f2, simd::float4(yFrac));

    return f3;
}
