//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <functional>

#include "../texture/bitmap.h"

// Forward declaration
namespace oidn {
    class FilterRef;
    class DeviceRef;
};

namespace mine {
    class Denoiser {
    public:
        Denoiser() = default;
        bool denoise(RGBFloat32Bitmap & lightenScene) const;
        bool denoise(RGBFloat32Bitmap & lightenScene,
                     RGBFloat32Bitmap const & normals,
                     RGBFloat32Bitmap const & albedo) const;
    private:
        bool denoise(RGBFloat32Bitmap & lightenScene,
                     std::function<void(oidn::FilterRef &, oidn::DeviceRef &)> filter) const;
    };
}
