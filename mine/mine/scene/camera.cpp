//
//  camera.cpp
//
//  Created on 17/04/2025.
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <cassert>

#include "camera.h"

mine::Camera::Camera(int antialiasRange)
: antialiasRange{antialiasRange} {

}

mine::Ray mine::Camera::ray(int x, int y, int width, int height) {
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    float newX = (static_cast<float>(x) / (width - 1)) * 2 - 1;
    float newXPlusOne = (static_cast<float>(x + 1) / (width - 1)) * 2 - 1;
    float aspect = static_cast<float>(width) / height;
    float newY = (static_cast<float>(y) / (height - 1)) / aspect * 2 - 1;
    float newYPlusOne = (static_cast<float>(y + 1) / (height - 1)) / aspect * 2 - 1;
    float spaceX = (newXPlusOne - newX) * antialiasRange;
    float spaceY = (newYPlusOne - newY) * antialiasRange;
    float displacementX = rng.random() * spaceX * 2 - spaceX;
    float displacementY = rng.random() * spaceY * 2 - spaceY;
    simd_float3 origin = simd_make_float3(0, 0, 0);
    simd_float3 newP = simd_make_float3(newX + displacementX, newY + displacementY, 1.0);
    simd_float3 direction = simd_normalize(newP - origin);
    return Ray{origin, direction};
}
