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

#include "../scene/ray/ray.h"
#include "../scene/scene.h"
#include "../intersector/intersector.h"
#include "../scene/rayintersection.h"
#include "../samplers/linearsampler.h"
#include "../sampling/hemisphere.h"
#include "../coordinates/diskcoordinates.h"
#include "../coordinates/spherecoordinates.h"
#include "../rng/rngstd.h"
#include "../config.h"

namespace mine {
    struct Metadata {
        int x;
        int y;
    };

    class RayTracer {
    public:
        std::pair<bool, bool> refract(const simd::float3& I, const simd::float3& N, float ior, simd::float3& outRefracted) {
            float cosi = simd::clamp(simd::dot(I, N), -1.0f, 1.0f);
            float etai = 1.0f, etat = ior;
            simd::float3 n = N;
            
            bool swap = false;
            if (cosi < 0.0f) {
                cosi = -cosi;
            } else {
                std::swap(etai, etat);
                n = -N;
                swap = true;
            }

            float eta = etai / etat;
            float k = 1.0f - eta * eta * (1.0f - cosi * cosi);

            if (k < 0) {
                return std::make_pair(false, false); // Total internal reflection
            }

            outRefracted = eta * I + (eta * cosi - sqrtf(k)) * n;
            return std::make_pair(true, swap);
        }
        
        float fresnel(const simd::float3& I, const simd::float3& N, float ior) {
            float cosi = simd::clamp(simd::dot(I, N), -1.0f, 1.0f);
            float etai = 1.0f;
            float etat = ior;

            if (cosi > 0.0f) {
                std::swap(etai, etat);
            }

            // Compute sine of transmission angle using Snell’s law
            float sint = etai / etat * sqrtf(fmaxf(0.0f, 1.0f - cosi * cosi));

            // Total internal reflection
            if (sint >= 1.0f) {
                return 1.0f;
            }

            float cost = sqrtf(fmaxf(0.0f, 1.0f - sint * sint));
            cosi = fabsf(cosi);

            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            return (Rs * Rs + Rp * Rp) * 0.5f;
        }
        
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
                                 const simd_float3& l,
                                 const simd_float3& albedo,
                                 float metalness,
                                 float roughness) {
            simd_float3 diffuse = albedo / M_PI;
            float alpha = std::pow(roughness, 2.0f);
            simd_float3 h = simd::normalize(l + v);
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

            return kD * diffuse + kS * specular;
        }
        simd_float4 trace(Ray const & r,
                          mine::Scene const & scene,
                          mine::Config const & config,
                          int currentDepth,
                          Metadata const & metadata) {
            if (currentDepth < 0) {
                return simd_make_float4(simd::float3(0), 1.0f);
            }
            
            std::optional<RayIntersection> closest = intersector.closestIntersection(scene, r);
            if (closest == std::nullopt) {
                if (scene.environmentMap) {
                    simd::float2 uv = sc.getEquirectangularCoordinates(r.direction);
                    simd::float3 environment = sampler.sample(uv.x, uv.y,
                                                              *scene.environmentMap).xyz;
                    return simd_make_float4(environment, 1);
                } else {
                    return simd_make_float4(0, 0, 0, 1);
                }
            }
            
            if (closest->material == nullptr) {
                return simd_make_float4(*(closest->lightColor), 1);
            }
            
            simd_float3 point = closest->point;
            simd_float2 uv = closest->uv;
            
            simd::float3 albedo = sampler.sample(uv[0], uv[1], closest->material->albedo).xyz;
            simd::float3 normal = sampler.sample(uv[0], uv[1], closest->material->normal).xyz;
            float metalness = sampler.sample(uv[0], uv[1], closest->material->metalness).x;
            float roughness = sampler.sample(uv[0], uv[1], closest->material->roughness).x;
            float ior = closest->material->ior;
            
            normal = (normal * 2.0f) - 1.0f;
            
            simd::float3x3 tbn(closest->T,
                               closest->B,
                               closest->N);
            
            normal = simd::normalize(tbn * normal);
            
            simd::float3 f0 = simd::lerp(simd::float3(0.04f), albedo, simd::float3(metalness));
            simd_float3 kS = fresnelSchlick(f0, -r.direction, normal);
            simd_float3 kD = simd::float3(1.0) - kS;
            kD *= 1.0 - metalness;
            
            simd::float3 accumulatedColor = simd::float3(0);
            
            for (auto const & light: scene.omnilights) {
                
                float shadowInfluence = 0.0f;

                if (config.shadowSamples > 0) {
                    for (int i = 0; i < config.shadowSamples; ++i) {
                        simd::float3 lightCenter = light.representation.center;
                        simd::float3 toCenter = simd::normalize(lightCenter - closest->point);
                                        
                        Disk d(lightCenter, -toCenter, light.representation.radius);
                        
                        float radius = rng.random() * light.representation.radius;
                        float theta = rng.random() * 2 * M_PI;
                        
                        DiskCoordinates dc;
                        
                        simd::float3 randomCartesian =  dc.polarToCartesian(radius, theta, d);
                        
                        // TODO: Check it
                        // assert(SphereIntersector().isInsideSphere(randomCartesian, light.representation));

                        simd::float3 newDirection = simd::normalize(randomCartesian - closest->point);
                        
                        Ray newray(closest->point + normal * 1e-4f, newDirection);
                        std::optional<RayIntersection> inttt = intersector.closestIntersection(scene, newray);
                        
                        assert(inttt != std::nullopt);
                        if (inttt->material != nullptr) {
                            shadowInfluence += 1;
                        }
                    }
                    
                    shadowInfluence /= float(config.shadowSamples);
                    assert(shadowInfluence <= 1.0);
                }
                
                simd_float3 l = light.representation.center - point;
                simd_float3 ln = simd::normalize(l);
                float l2 = fmax(simd::dot(l, l), 1e-5);
                
                float li = light.intensity * 1.0f/l2;
                
                simd_float3 v = -r.direction;
                float lamberts = simd::max(simd::dot(ln, normal), 0.0f);
                auto brdf = cookTorrance(v,
                                         normal,
                                         ln,
                                         albedo,
                                         metalness,
                                         roughness);
                accumulatedColor += (1.0f - shadowInfluence) * light.color * brdf * li * lamberts;
            }
            
            simd::float3 totalIndirect(0);
            for (int i = 0; i < config.indirectLightSamples; ++i) {
                simd::float3 sample = sampleHemisphere(normal, rng.random(), rng.random());
                simd::float3 newDirection = simd::normalize(tbn * sample);
                float cosTheta = std::max(simd::dot(normal, newDirection), 0.0f);

                Ray newRay(closest->point + normal * 1e-4, newDirection);
                simd::float3 incoming = trace(newRay, scene, config, currentDepth - 1, metadata).xyz;

                totalIndirect += incoming * albedo * 2.0f * cosTheta;
            }
            totalIndirect /= static_cast<float>(config.indirectLightSamples);
            totalIndirect *= kD; // Only apply indirect to diffuse component
            
            simd::float3 reflectedColor(0);
            if (config.reflections) {
                simd_float3 reflectDir(0);
                if (roughness > 0) {
                    reflectDir = sampleHemisphereGGXVNDF(-r.direction,
                                                         normal,
                                                         roughness * roughness,
                                                         rng.random(),
                                                         rng.random());
                } else {
                    reflectDir = simd::reflect(r.direction, normal);
                }
                Ray newRay(point + normal * 1e-4f, reflectDir);
                reflectedColor = trace(newRay,
                                       scene,
                                       config,
                                       currentDepth - 1,
                                       metadata).xyz;
                
                reflectedColor *= kS;
            }
            
            simd::float3 refracted = simd_make_float3(0, 0, 0);
            simd::float3 refractedColor = simd_make_float3(0, 0, 0);
            simd::float3 perturbedNormal = normal;
            if (roughness > 0) {
                simd::float3 sample = sampleHemisphere(normal, rng.random(), rng.random());
                simd::float3 newDirection = simd::normalize(tbn * sample);
                perturbedNormal = simd::normalize(normal + roughness * newDirection);
            }
            auto refract_r = refract(simd::normalize(r.direction), simd::normalize(perturbedNormal), ior, refracted);
            float reflectionFactor = 1.0f;
            if (ior != 0 && std::get<0>(refract_r)) {
                reflectedColor = simd_make_float3(0, 0, 0);
                Ray newRay(closest->point + perturbedNormal * 1e-4 * (((int)std::get<1>(refract_r)) ? 1 : -1), simd::normalize(refracted));
                refractedColor = trace(newRay,
                                       scene,
                                       config,
                                       currentDepth - 1,
                                       metadata).xyz;
                refractedColor *= albedo;
                reflectionFactor = fresnel(r.direction, perturbedNormal, ior);
            }
            
            simd_float3 totalColor = accumulatedColor +
            reflectedColor * reflectionFactor +
            refractedColor * (1 - reflectionFactor) +
            totalIndirect;
            return simd_make_float4(totalColor, 1.0f);
        }
    private:
        RNGSTD rng;
        Intersector intersector;
        LinearSampler sampler;
        SphereCoordinates sc;
    };
}
