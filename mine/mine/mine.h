//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "config.h"

#include "scene/triangle.h"
#include "scene/sphere.h"
#include "scene/scene.h"

#include "render/rtwriter.h"

#include "model/modelloader.h"

#include "scene/light/omnilight.h"
#include "scene/look/bitmaploader.h"

#include "scene/graph/scenegraph.h"
#include "scene/graph/sceneflattener.h"

#include "math/constants.h"
#include "math/transform.h"

#include "texture/bitmap.h"
#include "texture/color.h"
