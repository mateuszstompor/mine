//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "raytracer.h"

simd::float3 mine::RayTracer::trace(Ray const & r,
                                    mine::Scene const & scene,
                                    mine::Config const & config,
                                    int currentDepth,
                                    Metadata const & metadata) {
    if (currentDepth < 0) {
        return simd::make_float3(0.0f, 0.0f, 0.0f);
    }
    
    std::optional<RayIntersection> closest = intersector.closestIntersection(scene, r);
    if (closest == std::nullopt) {
        if (scene.environmentMap != nullptr) {
            simd::float2 uv = sc.getEquirectangularCoordinates(r.direction);
            return sampler.sample(uv, scene.environmentMap).xyz;
        } else {
            return simd::make_float3(0.0f, 0.0f, 0.0f);
        }
    }
    
    if (closest->material == nullptr) {
        return *(closest->lightColor);
    }
    
    simd::float3 point = closest->point;
    simd::float2 uv = closest->uv;
    
    simd::float4 color = sampler.sample(uv, closest->material->albedo);
    simd::float3 albedo = color.xyz;
    float alpha = color.a;
    simd::float3 normal = sampler.sample(uv, closest->material->normal).xyz;
    float metalness = sampler.sample(uv, closest->material->metalness).x;
    float roughness = sampler.sample(uv, closest->material->roughness).x;
    float ior = sampler.sample(uv, closest->material->ior).x;
    float opacity = sampler.sample(uv, closest->material->opacity).x;
    
    normal = (normal * 2.0f) - 1.0f;
    
    simd::float3x3 tbn(closest->T,
                       closest->B,
                       closest->N);
    
    normal = simd::normalize(tbn * normal);
    
    simd::float3 backgroundColor = simd::make_float3(0.0f, 0.0f, 0.0f);
    if (alpha < 1.0f || opacity < 1.0f) {
        Ray newRay(closest->point + r.direction * 1e-4, r.direction);
        backgroundColor = trace(newRay, scene, config, currentDepth - 1, metadata);
    }
    
    simd::float3 f0 = simd::lerp(simd::float3(0.04f), albedo, simd::float3(metalness));
    simd::float3 kS = fresnelSchlick(f0, -r.direction, normal);
    simd::float3 kD = simd::float3(1.0) - kS;
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
        
        simd::float3 l = light.representation.center - point;
        simd::float3 ln = simd::normalize(l);
        float l2 = fmax(simd::dot(l, l), 1e-5);
        
        float li = light.intensity * 1.0f/l2;
        
        simd::float3 v = -r.direction;
        float lamberts = simd::max(simd::dot(ln, normal), 0.0f);
        simd::float3 brdf = cookTorrance(v,
                                         normal,
                                         ln,
                                         albedo,
                                         metalness,
                                         roughness);
        accumulatedColor += (1.0f - shadowInfluence) * light.color * brdf * li * lamberts;
        assertFinite(accumulatedColor);
    }
    
    simd::float3 totalIndirect(0);
    if (config.indirectLightSamples > 0) {
        for (int i = 0; i < config.indirectLightSamples; ++i) {
            simd::float3 sample = sampleHemisphere(normal, rng.random2());
            simd::float3 newDirection = simd::normalize(tbn * sample);
            Ray newRay(closest->point + normal * 1e-4, newDirection);
            simd::float3 incoming = trace(newRay,
                                          scene,
                                          config,
                                          currentDepth - 1,
                                          metadata).xyz;
            totalIndirect += incoming;
        }
        totalIndirect /= static_cast<float>(config.indirectLightSamples);
        totalIndirect *= M_PI;
        totalIndirect *= kD * albedo;
    }
    
    simd::float3 reflectedColor(0);
    if (config.reflections) {
        simd::float3 reflectDir(0);
        if (roughness > 0) {
            reflectDir = sampleHemisphereGGXVNDF(-r.direction,
                                                 normal,
                                                 tbn,
                                                 roughness * roughness,
                                                 rng.random2());
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
    
    simd::float3 refracted = simd::make_float3(0, 0, 0);
    simd::float3 refractedColor = simd::make_float3(0, 0, 0);
    simd::float3 perturbedNormal = normal;
    if (roughness > 0) {
        simd::float3 sample = sampleHemisphere(normal, rng.random2());
        simd::float3 newDirection = simd::normalize(tbn * sample);
        perturbedNormal = simd::normalize(normal + roughness * newDirection);
    }
    auto refract_r = refract(simd::normalize(r.direction), simd::normalize(perturbedNormal), ior, refracted);
    float reflectionFactor = 1.0f;
    if (ior != 1.0f && std::get<0>(refract_r)) {
        reflectedColor = simd::make_float3(0, 0, 0);
        Ray newRay(closest->point + perturbedNormal * 1e-4 * (((int)std::get<1>(refract_r)) ? 1 : -1), simd::normalize(refracted));
        refractedColor = trace(newRay,
                               scene,
                               config,
                               currentDepth - 1,
                               metadata).xyz;
        refractedColor *= albedo;
        reflectionFactor = fresnel(r.direction, perturbedNormal, ior);
    }
    
    {
        simd::float3 aC = accumulatedColor;
        simd::float3 rfC = reflectedColor * reflectionFactor;
        simd::float3 rrC = refractedColor * (1 - reflectionFactor);
        simd::float3 iC = totalIndirect;
        assertFinite(aC);
        greaterEqualZero(aC);
        assertFinite(rfC);
        greaterEqualZero(rfC);
        assertFinite(rrC);
        greaterEqualZero(rrC);
        assertFinite(iC);
        greaterEqualZero(iC);
        simd::float3 foregroundColor = aC + rfC + rrC + iC;
        return foregroundColor * alpha + (1.0f - opacity) * backgroundColor * (1.0f - alpha);
    }
}

simd::float3 mine::RayTracer::traceNormal(Ray const & r,
                                          mine::Scene const & scene) {
    std::optional<RayIntersection> closest = intersector.closestIntersection(scene, r);

    if (closest == std::nullopt) {
        return simd::make_float3(0.0f, 0.0f, 0.0f);
    }
    
    simd::float3 normal;
    
    if (closest->material == nullptr) {
        normal = simd::make_float3(0.0f, 0.0f, 1.0f);
    } else {
        normal = sampler.sample(closest->uv, closest->material->normal).xyz;
        normal = (normal * 2.0f) - 1.0f;
    }
    
    simd::float3x3 tbn(closest->T,
                       closest->B,
                       closest->N);
    
    return simd::normalize(tbn * normal);
}

simd::float3 mine::RayTracer::traceAlbedo(Ray const & r,
                                          mine::Scene const & scene) {
    std::optional<RayIntersection> closest = intersector.closestIntersection(scene, r);
    
    if (closest == std::nullopt) {
        return simd::make_float3(0.0f, 0.0f, 0.0f);
    }
    
    if (closest->material == nullptr) {
        return *(closest->lightColor);
    }
    
    simd::float4 albedo = sampler.sample(closest->uv, closest->material->albedo);
    return albedo.xyz;
}
