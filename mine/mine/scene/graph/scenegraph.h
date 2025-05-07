//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <memory>
#include <optional>

#include "graphnode.h"
#include "nodecontents.h"

#include "../../texture/bitmap.h"

namespace mine {
    struct SceneGraph {
        std::optional<Bitmap> environment = std::nullopt;
        std::unique_ptr<BaseNode> root;
    };
}
