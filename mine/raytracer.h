//
//  raytracer.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <cstdint>

#include <simd/simd.h>

#include "scene/ray.h"
#include "scene/scene.h"
#include "intersector/intersector.h"
#include "scene/rayintersection.h"
#include "nearestsampler.h"

class RayTracer {
public:
    
    simd_float4 trace(uint16_t x,
                      uint16_t y,
                      uint16_t width,
                      uint16_t height,
                      Scene const & scene) {
        
        
        simd_float4 color{0, 0, 0, 1};
        
        Ray r = scene.camera.ray(x, y, width, height);
        std::optional<RayIntersection> closest = intersector.closestIntersection(scene, r);
        if (closest != std::nullopt) {
            simd_float3 point = closest->point;
            simd_float2 uv = closest->uv;
            simd::float4 albedo = sampler.sample(uv[0], uv[1], closest->material->albedo);
            
            simd_float3 l = simd_normalize(scene.omnilights[0].representation.center - point);
            float cosine = simd::clamp(simd::dot(closest->N, l), 0.0f, 1.0f);
            return simd_make_float4(albedo.xyz * cosine, 1);
        }
        
        return color;
    }
private:
    Intersector intersector;
    NearestSampler sampler;
};
