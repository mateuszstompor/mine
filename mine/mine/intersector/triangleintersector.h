//
//  triangleintersector.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <cmath>
#include <cassert>
#include <array>
#include <simd/simd.h>

#include "../scene/triangle.h"
#include "../scene/ray/ray.h"

namespace mine {
    class TriangleIntersector {
    public:
        std::optional<float> intersect(const Ray& ray, const Triangle& triangle, float epsilon = 1e-3) const {
            simd::float3 origin = (triangle.v0 + triangle.v1 + triangle.v2) / 3.0;
            float a = simd::dot(origin - ray.origin, triangle.normal);
            float b = simd::dot(ray.direction, triangle.normal);
            
            if (b == 0.0) {
                return std::nullopt;
            }
            
            float t = a / b;
            if (t < 0) {
                return std::nullopt;
            }
            
            simd::float3 point = ray.origin + ray.direction * t;
            
            simd::float3 v2v0 = triangle.v2 - triangle.v0;
            simd::float3 pV0 = point - triangle.v0;
            simd::float3 c1 = simd::cross(pV0, v2v0);
            
            if (simd::dot(c1, triangle.normal) < -epsilon) {
                return std::nullopt;
            }
            
            simd::float3 v1v0 = triangle.v1 - triangle.v0;
            simd::float3 c2 = simd::cross(v1v0, pV0);
            
            if (simd::dot(c2, triangle.normal) < -epsilon) {
                return std::nullopt;
            }
            
            simd::float3 v2v1 = triangle.v2 - triangle.v1;
            simd::float3 pV1 = point - triangle.v1;
            simd::float3 c3 = simd::cross(v2v1, pV1);
            
            if (simd::dot(c3, triangle.normal) < -epsilon) {
                return std::nullopt;
            }
            
            assert(std::isfinite(t));
            return t;
        }
        
        float area(const simd::float3& v0, const simd::float3& v1, const simd::float3& v2) const {
            return simd::length(simd::cross(v1 - v0, v2 - v0)) * 0.5;
        }
        
        simd::float3 barycentricCoordinates(const simd::float3& point, const Triangle& triangle) const {
            const simd::float3& v0 = triangle.v0;
            const simd::float3& v1 = triangle.v1;
            const simd::float3& v2 = triangle.v2;
            
            float abc = area(v0, v1, v2);
            float bcp = area(v1, v2, point);
            float cap = area(v2, v0, point);
            float abp = area(v0, v1, point);
            
            float u = bcp / abc;
            float v = cap / abc;
            float w = abp / abc;
            
            return simd::make_float3(u, v, w);
        }
        
        simd::float3 barycentricToCartesian(const simd::float3& x, const simd::float3& v0, const simd::float3& v1, const simd::float3& v2) const {
            float x1 = v0.x * x.x + v1.x * x.y + v2.x * x.z;
            float y1 = v0.y * x.x + v1.y * x.y + v2.y * x.z;
            float z1 = v0.z * x.x + v1.z * x.y + v2.z * x.z;
            
            return simd::make_float3(x1, y1, z1);
        }
        
        simd::float2 getTextureCoordinates(const simd::float3& point, const Triangle& triangle) const {
            simd::float3 barycentric = barycentricCoordinates(point, triangle);
            
            float u = 1.0 - barycentric.y - barycentric.z;
            float v = 1.0 - barycentric.x - barycentric.z;
            float w = 1.0 - barycentric.x - barycentric.y;
            
            simd::float3 cartesian = barycentricToCartesian(simd::make_float3(u, v, w),
                                                             simd::make_float3(triangle.uv0, 0.0),
                                                             simd::make_float3(triangle.uv1, 0.0),
                                                             simd::make_float3(triangle.uv2, 0.0));
            
            return simd::make_float2(cartesian.x, cartesian.y);
        }
    };
}
