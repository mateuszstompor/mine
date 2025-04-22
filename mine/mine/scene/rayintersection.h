//
//  rayintersection.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>

#include <simd/simd.h>

#include "../assertion/finite.h"
#include "look/material.h"

namespace mine {
    struct RayIntersection {
        simd::float3 T;
        simd::float3 B;
        simd::float3 N;
        simd::float3 point;
        simd::float2 uv;
        std::shared_ptr<Material> material;
        std::shared_ptr<simd::float3> lightColor;
        float t;
        RayIntersection(simd::float3 const & proposedT,
                        simd::float3 const & proposedB,
                        simd::float3 const & proposedN,
                        simd::float3 const & proposedPoint,
                        simd::float2 const & proposedUV,
                        std::shared_ptr<Material> const & proposedMaterial,
                        std::shared_ptr<simd::float3> const & proposedLightColor,
                        float const & proposedTValue)
        : T{proposedT}
        , B{proposedB}
        , N{proposedN}
        , point{proposedPoint}
        , uv{proposedUV}
        , material{proposedMaterial}
        , lightColor{proposedLightColor}
        , t{proposedTValue}
        {
            assertFinite(proposedT);
            assertFinite(proposedB);
            assertFinite(proposedN);
            assertFinite(proposedPoint);
            assertFinite(proposedUV);
            assert(simd::isfinite(proposedTValue));
        }
    };
}
