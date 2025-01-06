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
    
    // Align the sample to the hemisphere normal
    simd::float3 tangent, bitangent;
    if (std::abs(normal.x) > std::abs(normal.z)) {
        tangent = simd::normalize(simd_make_float3(-normal.y, normal.x, 0.0f));
    } else {
        tangent = simd::normalize(simd_make_float3(0.0f, -normal.z, normal.y));
    }
    bitangent = simd::cross(normal, tangent);
    
    // Transform the sample to align with the hemisphere's normal
    return sample.x * tangent + sample.y * bitangent + sample.z * normal;
}
