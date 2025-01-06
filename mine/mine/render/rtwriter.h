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

#include "../config.h"
#include "../utilities/region.h"
#include "../texture/cgbitmap.h"
#include "raytracer.h"

namespace mine {
    struct RTWriter {
        RTWriter(Config const & config);
        ~RTWriter();
            
        void capture(Scene const & scene);
        CGBitmap cgbitmap;
        
    private:
        std::vector<Region<uint16_t>> randomizedRegions();
        std::vector<Region<uint16_t>> divideIntoRegions();
        
        RayTracer rt;
        Config config;
        NSOperationQueue * queue;
    };
}
