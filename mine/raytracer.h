//
//  raytracer.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <cstdint>
#include <cmath>
#include <optional>

#include <simd/simd.h>

#include "scene/ray.h"
#include "scene/scene.h"
#include "intersector/intersector.h"
#include "scene/rayintersection.h"
#include "linearsampler.h"

class RayTracer {
public:
    float distributionGGX(float alpha, const simd_float3& n, const simd_float3& h) {
        float nDotH = std::max(simd::dot(n, h), 0.0f);
        float alphaSq = alpha * alpha;
        float denom = (nDotH * nDotH * (alphaSq - 1.0f) + 1.0f);
        return alphaSq / (M_PI * denom * denom);
    }

    float fresnelSchlick(float f0, const simd_float3& v, const simd_float3& h) {
        float cosTheta = simd::clamp(simd::dot(v, h), 0.0f, 1.0f);
        return f0 + (1.0f - f0) * std::pow(1.0f - cosTheta, 5.0f);
    }

    float geometrySchlickGGX(const simd_float3& v, const simd_float3& n, float k) {
        float nDotV = std::max(simd::dot(n, v), 0.0f);
        return nDotV / (nDotV * (1.0 - k) + k);
    }

    float geometrySmith(const simd_float3& v, const simd_float3& n, const simd_float3& l, float k) {
        float gV = geometrySchlickGGX(v, n, k);
        float gL = geometrySchlickGGX(l, n, k);
        return gV * gL;
    }

    simd_float3 cookTorrance(const simd_float3& v,
                             const simd_float3& n,
                             const simd_float3& h,
                             const simd_float3& l,
                             const simd_float3& albedo,
                             float metalness,
                             float roughness,
                             float li) {
        simd_float3 diffuse = albedo / M_PI;
        float lamberts = simd::clamp(simd::dot(l, n), 0.0f, 1.0f);
        float alpha = std::pow(roughness, 2);
        float d = distributionGGX(alpha, n, h);
        
        simd_float3 f0;
        f0.x = simd::lerp(0.04f, albedo.x, metalness);
        f0.y = simd::lerp(0.04f, albedo.y, metalness);
        f0.z = simd::lerp(0.04f, albedo.z, metalness);

        simd_float3 f;
        f.x = fresnelSchlick(f0.x, v, h);
        f.y = fresnelSchlick(f0.y, v, h);
        f.z = fresnelSchlick(f0.z, v, h);
        
        simd_float3 kS = f;
        simd_float3 kD = simd::make_float3(1.0, 1.0, 1.0) - kS;
        kD *= 1.0 - metalness;

        float k = std::pow(alpha + 1.0, 2) / 8.0;
        float g = geometrySmith(v, n, l, k);
        const float epsilon = 1e-5;

        simd_float3 specular = (f * g * d) / std::max(4.0f * simd::dot(n, l) * simd::dot(n, v), epsilon);

        simd_float3 direct = kD * diffuse + kS * specular;

        return direct * lamberts * li;
    }
    simd_float4 trace(uint16_t x,
                      uint16_t y,
                      uint16_t width,
                      uint16_t height,
                      Scene const & scene) {
        Ray r = scene.camera.ray(x, y, width, height);
        std::optional<RayIntersection> closest = intersector.closestIntersection(scene, r);
        if (closest == std::nullopt) {
            return simd_make_float4(0, 0, 0, 1);
        }
        
        simd_float3 point = closest->point;
        simd_float2 uv = closest->uv;
        
        simd::float3 albedo = sampler.sample(uv[0], uv[1], closest->material->albedo).xyz;
        simd::float3 normal = sampler.sample(uv[0], uv[1], closest->material->normal).xyz;
        float metalness = sampler.sample(uv[0], uv[1], closest->material->metalness).x;
        float roughness = sampler.sample(uv[0], uv[1], closest->material->roughness).x;
        
        normal = (normal * 2.0f) - 1.0f;
        
        simd::float3x3 tbn(closest->T, closest->B, closest->N);
        normal = tbn * normal;
        
        simd::float3 accumulatedColor = simd::float3(0);
        
        for (auto const & light: scene.omnilights) {
            
            simd_float3 l = light.representation.center - point;
            simd_float3 ln = simd::normalize(l);
            float l2 = simd::dot(l, l);
            
            float li = light.intensity * 1.0f/l2;
            
            simd_float3 v = simd::normalize(r.origin - closest->point);
            simd_float3 h = simd::normalize(l + v);
            
            
            accumulatedColor += cookTorrance(v, normal,
                                             h, ln,
                                             albedo,
                                             metalness,
                                             roughness,
                                             li);
        }
        return simd_make_float4(simd::clamp(accumulatedColor,
                                            simd::float3(0.0f),
                                            simd::float3(1.0f)), 1.0f);
    }
private:
    Intersector intersector;
    LinearSampler sampler;
};
