//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "../../texture/raw/bitmap.h"
#include "../../texture/raw/rgbauint8bitmap.h"
#include "../../texture/raw/rgbafloat32bitmap.h"

namespace mine {
    struct Material {
        std::shared_ptr<Bitmap> albedo;
        std::shared_ptr<Bitmap> roughness;
        std::shared_ptr<Bitmap> metalness;
        std::shared_ptr<Bitmap> normal;
        std::shared_ptr<Bitmap> ior;
        std::shared_ptr<Bitmap> opacity;
        
        Material(RGBAUInt8Bitmap const & providedAlbedo,
                 RGBAUInt8Bitmap const & providedRoughness,
                 RGBAUInt8Bitmap const & providedMetalness,
                 RGBAUInt8Bitmap const & providedNormal,
                 RGBAUInt8Bitmap const & providedIOR,
                 RGBAUInt8Bitmap const & providedOpacity) {
            albedo = std::make_shared<RGBAUInt8Bitmap>(providedAlbedo);
            roughness = std::make_shared<RGBAUInt8Bitmap>(providedRoughness);
            metalness = std::make_shared<RGBAUInt8Bitmap>(providedMetalness);
            normal = std::make_shared<RGBAUInt8Bitmap>(providedNormal);
            ior = std::make_shared<RGBAUInt8Bitmap>(providedIOR);
            opacity = std::make_shared<RGBAUInt8Bitmap>(providedOpacity);
        }
        Material(RGBAFloat32Bitmap const & providedAlbedo,
                 RGBAFloat32Bitmap const & providedRoughness,
                 RGBAFloat32Bitmap const & providedMetalness,
                 RGBAFloat32Bitmap const & providedNormal,
                 RGBAFloat32Bitmap const & providedIOR,
                 RGBAFloat32Bitmap const & providedOpacity) {
            albedo = std::make_shared<RGBAFloat32Bitmap>(providedAlbedo);
            roughness = std::make_shared<RGBAFloat32Bitmap>(providedRoughness);
            metalness = std::make_shared<RGBAFloat32Bitmap>(providedMetalness);
            normal = std::make_shared<RGBAFloat32Bitmap>(providedNormal);
            ior = std::make_shared<RGBAFloat32Bitmap>(providedIOR);
            opacity = std::make_shared<RGBAFloat32Bitmap>(providedOpacity);
        }
    };
}
