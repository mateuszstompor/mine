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
#include "hemisphere.h"
#include "rng/rngstd.h"

struct Metadata {
    int x;
    int y;
};

class RayTracer {
public:
    float distributionGGX(float alpha, const simd_float3& n, const simd_float3& h) {
        float nDotH = std::max(simd::dot(n, h), 0.0f);
        float alphaSq = alpha * alpha;
        float denom = (nDotH * nDotH * (alphaSq - 1.0f) + 1.0f);
        return alphaSq / (M_PI * denom * denom);
    }

    simd_float3 fresnelSchlick(const simd_float3& f0,
                               const simd_float3& v,
                               const simd_float3& h) {
        float cosTheta = simd::max(simd::dot(v, h), 0.0f);
        return f0 + (1.0f - f0) * std::pow(1.0f - cosTheta, 5.0f);
    }

    float geometrySchlickGGX(const simd_float3& v, const simd_float3& n, float k) {
        float nDotV = std::max(simd::dot(n, v), 0.0f);
        return nDotV / (nDotV * (1.0f - k) + k);
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
        float lamberts = simd::max(simd::dot(l, n), 0.0f);
        float alpha = std::pow(roughness, 2.0f);
        float d = distributionGGX(alpha, n, h);
        
        simd_float3 f0 = simd::lerp(simd_float3(0.04f),
                                    albedo,
                                    simd_float3(metalness));

        simd_float3 f = fresnelSchlick(f0, v, h);
        
        simd_float3 kS = f;
        simd_float3 kD = simd::float3(1.0) - kS;
        kD *= 1.0 - metalness;

        float k = std::pow(alpha + 1.0f, 2.0f) / 8.0f;
        float g = geometrySmith(v, n, l, k);
        const float epsilon = 1e-5;

        float denominator = std::max(4.0f * simd::dot(n, l) * simd::dot(n, v), epsilon);
        simd_float3 specular = (f * g * d) / denominator;

        simd_float3 direct = kD * diffuse + kS * specular;

        return direct * lamberts * li;
    }
    simd_float4 trace(Ray const & r,
                      Scene const & scene,
                      int depth,
                      Metadata const & metadata) {
//        int range = 10;
//        int minx = 300;
//        int miy = 340;
//        if (metadata.x > minx && metadata.x < minx + range &&
//            metadata.y > miy && metadata.y < miy + range) {
//            return simd_make_float4(1, 1, 0, 1);
//        }
        
        if (depth < 0) {
            return simd_make_float4(simd::float3(0), 1.0f);
        }
        
        std::optional<RayIntersection> closest = intersector.closestIntersection(scene, r);
        if (closest == std::nullopt) {
            return simd_make_float4(0, 0, 0, 1);
        }
        
        if (closest->material == nil) {
            return simd_make_float4(1, 1, 1, 1);
        }
        
        simd_float3 point = closest->point;
        simd_float2 uv = closest->uv;
        
        simd::float3 albedo = sampler.sample(uv[0], uv[1], closest->material->albedo).xyz;
        simd::float3 normal = sampler.sample(uv[0], uv[1], closest->material->normal).xyz;
        float metalness = sampler.sample(uv[0], uv[1], closest->material->metalness).x;
        float roughness = sampler.sample(uv[0], uv[1], closest->material->roughness).x;
        
        normal = (normal * 2.0f) - 1.0f;
        
        simd::float3x3 tbn(closest->T,
                           closest->B,
                           closest->N);
        
        normal = simd::normalize(tbn * normal);
        
        simd::float3 accumulatedColor = simd::float3(0);
        
        for (auto const & light: scene.omnilights) {
            
            int shadowSamples = 4;
            float shadowInfluence = 0.0f;

            if (shadowSamples > 0) {
                for (int i = 0; i < shadowSamples; ++i) {
                    if (metadata.x == 300 && metadata.y == 340) {
                        printf("");
                    }
                    simd::float3 lightCenter = light.representation.center;
                    simd::float3 toCenter = simd::normalize(lightCenter - closest->point);
                                    
                    Disk d(lightCenter, -toCenter, light.representation.radius);
                    
                    DiskIntersector di;
                    float radius = rng.random() * light.representation.radius;
                    float theta = rng.random() * 2 * M_PI;
                    
                    
                    simd::float3 randomCartesian =  di.polarToCartesian(radius, theta, d);
                    assert(SphereIntersector().isInsideSphere(randomCartesian, light.representation));
//                    simd::float3 newdir = toCenter;
                    simd::float3 newdir = simd::normalize(randomCartesian - closest->point);
                    
                    Ray newray(closest->point + newdir, newdir);
                    std::optional<RayIntersection> inttt = intersector.closestIntersection(scene, newray);
                    
                    assert(inttt != std::nullopt);
                    if (inttt->material != nullptr) {
                        shadowInfluence += 1;
                    }
                }
                
                shadowInfluence /= float(shadowSamples);
                assert(shadowInfluence <= 1.0);
            }
            
            
            simd_float3 l = light.representation.center - point;
            simd_float3 ln = simd::normalize(l);
            float l2 = simd::dot(l, l);
            
            float li = light.intensity * 1.0f/l2;
            
            simd_float3 v = simd::normalize(r.origin - closest->point);
            simd_float3 h = simd::normalize(l + v);
            
            
            accumulatedColor += (1.0f - shadowInfluence ) * cookTorrance(v, normal,
                                                                h, ln,
                                                                albedo,
                                                                metalness,
                                                                roughness,
                                                                li);
        }
        
        
        simd::float3 totalIndirect(0);
        int samplesTotal = 4;
        if (samplesTotal > 0) {
            for (int i = 0; i < samplesTotal; ++i) {
                simd::float3 newDirection = sampleHemisphere(normal,
                                                             rng.random(),
                                                             rng.random());
                Ray newRay(closest->point + newDirection * 1e-5, newDirection);
                float affect = std::max(simd::dot(normal, newDirection), 0.0f);
                totalIndirect += trace(newRay, scene, depth - 1, metadata).xyz * affect;
            }
            totalIndirect /= static_cast<float>(samplesTotal);
        }
        
        return simd_make_float4(simd::clamp(accumulatedColor + totalIndirect,
                                            simd::float3(0.0f),
                                            simd::float3(1.0f)), 1.0f);
    }
private:
    RNGSTD rng;
    Intersector intersector;
    LinearSampler sampler;
};
