//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "cooktorrance.h"

float32_t mine::distributionGGX(float32_t alpha,
                                simd::float3 const & n,
                                simd::float3 const & h) {
    assertNormalized(n);
    assertNormalized(h);
    
    alpha = std::max(alpha, 1e-3f);
    float32_t nDotH = std::max(simd::dot(n, h), 0.0f);
    float32_t alphaSq = alpha * alpha;
    float32_t nDotH2 = nDotH * nDotH;
    float32_t denom = nDotH2 * (alphaSq - 1.0f) + 1.0f;
    float32_t result = alphaSq / (M_PI * denom * denom);
    
    assertFinite(result);
    
    return result;
}

simd::float3 mine::fresnelSchlick(const simd::float3& f0,
                                  const simd::float3& v,
                                  const simd::float3& h) {
    float cosTheta = simd::max(simd::dot(v, h), 0.0f);
    return f0 + (1.0f - f0) * std::pow(1.0f - cosTheta, 5.0f);
}

float mine::geometrySchlickGGX(const simd::float3& v, const simd::float3& n, float k) {
    float nDotV = std::max(simd::dot(n, v), 0.0f);
    return nDotV / (nDotV * (1.0f - k) + k);
}

float mine::geometrySmith(const simd::float3& v, const simd::float3& n, const simd::float3& l, float k) {
    assertNormalized(v);
    assertNormalized(n);
    assertNormalized(l);
    
    float gV = geometrySchlickGGX(v, n, k);
    float gL = geometrySchlickGGX(l, n, k);
    return gV * gL;
}

simd::float3 mine::cookTorrance(const simd::float3& v,
                                const simd::float3& n,
                                const simd::float3& l,
                                const simd::float3& albedo,
                                float metalness,
                                float roughness) {
    assertNormalized(v);
    assertNormalized(n);
    assertNormalized(l);
    
    simd::float3 diffuse = albedo / M_PI;
    float alpha = std::pow(roughness, 2.0f);
    simd::float3 h = simd::normalize(l + v);
    float d = distributionGGX(alpha, n, h);
    
    simd::float3 f0 = simd::lerp(simd::float3(0.04f),
                                 albedo,
                                 simd::float3(metalness));
    
    simd::float3 f = fresnelSchlick(f0, v, h);
    
    simd::float3 kS = f;
    simd::float3 kD = simd::float3(1.0) - kS;
    kD *= 1.0 - metalness;
    
    float k = std::pow(alpha + 1.0f, 2.0f) / 8.0f;
    float g = geometrySmith(v, n, l, k);
    const float epsilon = 1e-5;
    
    float denominator = std::max(4.0f * simd::dot(n, l) * simd::dot(n, v), epsilon);
    simd::float3 specular = (f * g * d) / denominator;
    simd::float3 diffuseComponent = kD * diffuse;
    simd::float3 specularComponent = kS * specular;
    assertFinite(diffuseComponent);
    assertFinite(specularComponent);
    return diffuseComponent + specularComponent;
}
