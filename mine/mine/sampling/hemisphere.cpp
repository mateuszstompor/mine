//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "hemisphere.h"

#include <cassert>
#include <cmath>

#include "../math/constants.h"
#include "../assertion/finite.h"

// Cosine-weighted hemisphere sampling
simd::float3 mine::sampleHemisphere(const simd::float3& normal, simd::float2 uv) {
    float theta = std::acos(std::sqrt(1.0f - uv.x));
    float phi = TWO_PI * uv.y;
    
    float x = std::sin(theta) * std::cos(phi);
    float y = std::sin(theta) * std::sin(phi);
    float z = std::cos(theta);
    
    simd::float3 sample = simd_make_float3(x, y, z);
    
    assertFinite(sample);
    
    return sample;
}

simd_float3 mine::sampleHemisphereGGXVNDF(simd_float3 const & view,
                                          simd_float3 const & normal,
                                          float roughness,
                                          simd::float2 uv) {
    // Transform view vector to tangent space
    simd_float3 up = fabs(normal.z) < 0.999f ? simd_make_float3(0.0f, 0.0f, 1.0f) : simd_make_float3(1.0f, 0.0f, 0.0f);
    simd_float3 t = simd::normalize(simd::cross(up, normal));
    simd_float3 b = simd::cross(normal, t);
    simd_float3 vLocal = simd::normalize(simd_make_float3(
                                                          simd::dot(view, t),
                                                          simd::dot(view, b),
                                                          simd::dot(view, normal)
                                                          ));
    
    // Stretch view
    simd_float3 vStretched = simd::normalize(simd_make_float3(roughness * vLocal.x, roughness * vLocal.y, vLocal.z));
    
    // Orthonormal basis
    float lensq = vStretched.x * vStretched.x + vStretched.y * vStretched.y;
    simd_float3 t1 = lensq > 0 ? simd_make_float3(-vStretched.y, vStretched.x, 0.0f) / std::sqrt(lensq)
    : simd_make_float3(1.0f, 0.0f, 0.0f);
    simd_float3 t2 = simd::cross(vStretched, t1);
    
    // Sample point on hemisphere
    float r = std::sqrt(uv.x);
    float phi = TWO_PI * uv.y;
    float x = r * std::cos(phi);
    float y = r * std::sin(phi);
    float z = std::sqrt(std::fmax(0.0f, 1.0f - x * x - y * y));
    
    // Half vector in stretched space
    simd_float3 hStretched = x * t1 + y * t2 + z * vStretched;
    
    // Unstretch
    simd_float3 h = simd::normalize(simd_make_float3(roughness * hStretched.x,
                                                     roughness * hStretched.y,
                                                     std::fmax(0.0f, hStretched.z)));
    
    // Transform h back to world space
    simd_float3 hWorld = simd::normalize(h.x * t + h.y * b + h.z * normal);
    
    // Reflect view over sampled normal
    simd::float3 result = simd::reflect(-view, hWorld);
    
    assertFinite(result);
    
    return result;
}
