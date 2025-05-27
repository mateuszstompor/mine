//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <optional>

#include "graphnode.h"
#include "nodecontents.h"

#include "../../texture/raw/bitmap.h"

namespace mine {
    struct SceneGraph {
        std::shared_ptr<Bitmap> environment = nullptr;
        std::unique_ptr<BaseNode> root;
    };
}
