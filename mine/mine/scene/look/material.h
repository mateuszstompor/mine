//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include "../../texture/bitmap.h"

namespace mine {
    struct Material {
        std::shared_ptr<Bitmap> albedo;
        std::shared_ptr<Bitmap> roughness;
        std::shared_ptr<Bitmap> metalness;
        std::shared_ptr<Bitmap> normal;
        std::shared_ptr<Bitmap> ior;
        std::shared_ptr<Bitmap> opacity;
        
        Material(RGBAUint8Bitmap const & providedAlbedo,
                 RGBAUint8Bitmap const & providedRoughness,
                 RGBAUint8Bitmap const & providedMetalness,
                 RGBAUint8Bitmap const & providedNormal,
                 RGBAUint8Bitmap const & providedIOR,
                 RGBAUint8Bitmap const & providedOpacity) {
            albedo = std::make_shared<RGBAUint8Bitmap>(providedAlbedo);
            roughness = std::make_shared<RGBAUint8Bitmap>(providedRoughness);
            metalness = std::make_shared<RGBAUint8Bitmap>(providedMetalness);
            normal = std::make_shared<RGBAUint8Bitmap>(providedNormal);
            ior = std::make_shared<RGBAUint8Bitmap>(providedIOR);
            opacity = std::make_shared<RGBAUint8Bitmap>(providedOpacity);
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
