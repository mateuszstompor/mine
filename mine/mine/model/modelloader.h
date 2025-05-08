#pragma once

#include <optional>
#include <Foundation/Foundation.h>
#include <ModelIO/ModelIO.h>

#include "../texture/bitmap.h"
#include "../scene/look/material.h"
#include "../scene/graph/scenegraph.h"

namespace mine {
    class ModelLoader {
    public:
        SceneGraph load(std::string const & path);
    private:
        void processObject(MDLObject * object, std::unique_ptr<BaseNode> & currentParent);
        std::optional<Bitmap> loadBitmap(MDLMaterialProperty * property);
        std::shared_ptr<Material> loadMaterial(MDLMaterial * material);
        std::unique_ptr<BaseNode> loadMeshNode(MDLMesh * node);
        std::unique_ptr<BaseNode> loadTransformNode(MDLObject * node);
        std::unique_ptr<BaseNode> loadGenericNode(MDLObject * node);
    };
}
