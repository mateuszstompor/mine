//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <Foundation/Foundation.h>

#include <vector>
#include <cstdint>
#include <simd/simd.h>

#include "../config.h"
#include "../utilities/region.h"
#include "../texture/cgbitmap.h"
#include "raytracer.h"

namespace mine {
    struct RTWriter {
        RTWriter(Config const & config);
        ~RTWriter();
            
        void capture(Scene & scene);
        void capturePixel(Scene & scene,
                          simd::float2 const & coordinate);
        RGBAFloat32Bitmap accumulator;
        CGBitmap getBitmap();
        
    private:
        std::vector<Region<uint16_t>> randomizedRegions();
        std::vector<Region<uint16_t>> divideIntoRegions();
        void captureRegion(Region<uint16_t> const & region,
                           Scene & scene,
                           uint32_t iteration);
        RayTracer rt;
        Config config;
        NSOperationQueue * queue;
    };
}
