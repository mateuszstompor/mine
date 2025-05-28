//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <cstdint>
#include <cmath>
#include <optional>

#include <simd/simd.h>

#include "../scene/ray/ray.h"
#include "../scene/scene.h"
#include "../intersector/intersector.h"
#include "../scene/rayintersection.h"
#include "../samplers/texturesampler.h"
#include "../sampling/hemisphere.h"
#include "../coordinates/diskcoordinates.h"
#include "../coordinates/spherecoordinates.h"
#include "../rng/rngstd.h"
#include "../config.h"
#include "cooktorrance.h"
#include "metadata.h"
#include "transmission.h"

namespace mine {
    class RayTracer {
    public:
        simd::float3 trace(Ray const & r,
                           mine::Scene const & scene,
                           mine::Config const & config,
                           int currentDepth,
                           Metadata const & metadata);
        
        simd::float3 traceNormal(Ray const & r,
                                 mine::Scene const & scene);
        
        simd::float3 traceAlbedo(Ray const & r,
                                 mine::Scene const & scene);
    private:
        RNGSTD rng;
        Intersector intersector;
        TextureSampler sampler{Filter::Linear, Edge::ClampToEdge};
        SphereCoordinates sc;
    };
}
