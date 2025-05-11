//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <map>
#include <memory>
#include <string>
#include <optional>
#include <ModelIO/ModelIO.h>

#include "../texture/bitmap.h"
#include "../scene/look/material.h"

namespace mine {
    class MaterialLoader {
    public:
        MaterialLoader() = default;
        std::shared_ptr<Material> loadMaterial(MDLMaterial * material);
    private:
        std::optional<Bitmap> loadBitmap(MDLMaterialProperty * property);
        std::map<std::string, std::shared_ptr<Material>> cache;
        Bitmap loadPropertyTypeTexture(MDLMaterialProperty * property);
        Bitmap loadPropertyTypeColor(MDLMaterialProperty * property);
    };
}
