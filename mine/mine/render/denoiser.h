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
    class BufferRef;
};

namespace mine {
    class Denoiser {
    public:
        Denoiser() = default;
        bool denoise(RGBFloat32Bitmap & lightenScene) const;
        bool denoise(RGBFloat32Bitmap & lightenScene,
                     RGBFloat32Bitmap const & normals,
                     RGBFloat32Bitmap const & albedo) const;
        oidn::BufferRef createBufferFromBitmap(RGBFloat32Bitmap const & bitmap,
                                               oidn::DeviceRef const & device) const;
        void dump(oidn::BufferRef const & source, RGBFloat32Bitmap & destination) const;
    private:
        bool denoise(RGBFloat32Bitmap & lightenScene,
                     std::function<void(oidn::FilterRef &, oidn::DeviceRef &)> filter) const;
    };
}
