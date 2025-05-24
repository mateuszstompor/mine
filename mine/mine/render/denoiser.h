//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <optional>
#include <OpenImageDenoise/oidn.hpp>
#include "../texture/bitmap.h"

namespace mine {
    class Denoiser {
    public:
        Denoiser() = default;
        bool denoise(RGBFloat32Bitmap & lightenScene,
                     std::optional<std::pair<RGBFloat32Bitmap, RGBFloat32Bitmap>> const & normalsAndAlbedo) const;
    };
}
