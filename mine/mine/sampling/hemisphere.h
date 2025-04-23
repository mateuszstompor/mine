#pragma once

#include <iostream>
#include <cmath>

inline simd::float3 sampleHemisphere(const simd::float3& normal,
                                     float u,
                                     float v,
                                     float power = 1.0f) {
    // Spherical coordinates for hemisphere
    float theta = std::acos(std::pow(u, 1.0f / (power + 1.0f))); // Distribution along the hemisphere
    float phi = 2.0f * M_PI * v;                     // Full circle for azimuth
    
    // Convert spherical to Cartesian coordinates
    float x = std::sin(theta) * std::cos(phi);
    float y = std::sin(theta) * std::sin(phi);
    float z = std::cos(theta);
    
    // Local sample direction
    simd::float3 sample = simd_make_float3(x, y, z);
        
    assert(simd::isfinite(sample.x));
    assert(simd::isfinite(sample.y));
    assert(simd::isfinite(sample.z));
    return sample;
}

inline simd_float3 sampleHemisphereGGXVNDF(const simd_float3& v,
                                           const simd_float3& n,
                                           float roughness,
                                           float u1,
                                           float u2) {
    // Transform view vector to tangent space
    simd_float3 up = fabs(n.z) < 0.999f ? simd_make_float3(0.0f, 0.0f, 1.0f) : simd_make_float3(1.0f, 0.0f, 0.0f);
    simd_float3 t = simd::normalize(simd::cross(up, n));
    simd_float3 b = simd::cross(n, t);
    simd_float3 vLocal = simd::normalize(simd_make_float3(
        simd::dot(v, t),
        simd::dot(v, b),
        simd::dot(v, n)
    ));

    // Stretch view
    simd_float3 vStretched = simd::normalize(simd_make_float3(roughness * vLocal.x, roughness * vLocal.y, vLocal.z));

    // Orthonormal basis
    float lensq = vStretched.x * vStretched.x + vStretched.y * vStretched.y;
    simd_float3 t1 = lensq > 0 ? simd_make_float3(-vStretched.y, vStretched.x, 0.0f) / std::sqrt(lensq)
                               : simd_make_float3(1.0f, 0.0f, 0.0f);
    simd_float3 t2 = simd::cross(vStretched, t1);

    // Sample point on hemisphere
    float r = std::sqrt(u1);
    float phi = 2.0f * M_PI * u2;
    float x = r * std::cos(phi);
    float y = r * std::sin(phi);
    float z = std::sqrt(std::max(0.0f, 1.0f - x * x - y * y));

    // Half vector in stretched space
    simd_float3 hStretched = x * t1 + y * t2 + z * vStretched;

    // Unstretch
    simd_float3 h = simd::normalize(simd_make_float3(roughness * hStretched.x,
                                                     roughness * hStretched.y,
                                                     std::max(0.0f, hStretched.z)));

    // Transform h back to world space
    simd_float3 hWorld = simd::normalize(h.x * t + h.y * b + h.z * n);

    // Reflect view over sampled normal
    simd::float3 result = simd::reflect(-v, hWorld);
    assert(!simd::isnan(result.x));
    assert(!simd::isnan(result.y));
    assert(!simd::isnan(result.z));
    return result;
}
