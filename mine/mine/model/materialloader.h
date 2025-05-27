//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <map>
#include <memory>
#include <string>
#include <optional>
#include <ModelIO/ModelIO.h>

#include "../texture/raw/bitmap.h"
#include "../scene/look/material.h"

namespace mine {
    class MaterialLoader {
    public:
        MaterialLoader() = default;
        std::shared_ptr<Material> loadMaterial(MDLMaterial * material);
    private:
        std::optional<RGBAFloat32Bitmap> loadBitmap(MDLMaterialProperty * property);
        std::map<std::string, std::shared_ptr<Material>> cache;
        RGBAUInt8Bitmap loadPropertyTypeTexture(MDLMaterialProperty * property);
        RGBAFloat32Bitmap loadPropertyTypeColor(MDLMaterialProperty * property);
    };
}
