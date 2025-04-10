//
//  rtwriter.h
//
//  Created on 20/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
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
            
        void capture(Scene const & scene);
        void capturePixel(Scene const & scene,
                          simd::float2 const & coordinate);
        CGBitmap cgbitmap;
        
    private:
        std::vector<Region<uint16_t>> randomizedRegions();
        std::vector<Region<uint16_t>> divideIntoRegions();
        void captureRegion(Region<uint16_t> const & region,
                           Scene const & scene,
                           uint16_t iteration);
        RayTracer rt;
        Config config;
        NSOperationQueue * queue;
    };
}
