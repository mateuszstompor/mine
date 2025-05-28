//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <cassert>

#include "camera.h"

mine::Camera::Camera(float32_t antialiasRange)
: antialiasRange{antialiasRange} {

}

mine::Ray mine::Camera::ray(uint16_t x,
                            uint16_t y,
                            uint16_t width,
                            uint16_t height) {
    assert(x < width);
    assert(y < height);
    
    float spaceX = 2.0f / width;
    float spaceY = 2.0f / height;
    
    float displacementX = rng.random() * spaceX - spaceX / 2.0f;
    float displacementY = rng.random() * spaceY - spaceY / 2.0f;

    float aspect = static_cast<float>(width) / height;
    float ndcX = (static_cast<float>(x) / (width - 1)) * 2 - 1;
    float ndcY = (static_cast<float>(y) / (height - 1)) * 2 - 1;
    
    float ndcXCenter = ndcX + 0.5f * spaceX;
    float ndcYCenter = ndcY + 0.5f * spaceY;

    simd::float3 origin = simd::make_float3(0, 0, 0);
    simd::float3 newP = simd::make_float3(ndcXCenter * aspect + displacementX * antialiasRange,
                                          ndcYCenter + displacementY * antialiasRange,
                                          1.0f);
    simd::float3 direction = simd::normalize(newP - origin);
    
    return Ray{origin, direction};
}
