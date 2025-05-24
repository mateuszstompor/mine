//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <OpenImageDenoise/oidn.hpp>

#include "denoiser.h"


bool mine::Denoiser::denoise(RGBFloat32Bitmap & lightenScene,
                             RGBFloat32Bitmap const & normals,
                             RGBFloat32Bitmap const & albedo) const {
    return denoise(lightenScene, [&](oidn::FilterRef & filter, oidn::DeviceRef & device) {
        {
            oidn::BufferRef normalsBuf  = createBufferFromBitmap(normals, device);
            filter.setImage("normal", normalsBuf,  oidn::Format::Float3, normals.width, normals.height);
        }
        
        {
            oidn::BufferRef albedoBuf  = createBufferFromBitmap(albedo, device);
            filter.setImage("albedo", albedoBuf,  oidn::Format::Float3, albedo.width, albedo.height);
        }
    });
}

bool mine::Denoiser::denoise(RGBFloat32Bitmap & lightenScene) const {
    return denoise(lightenScene, [](oidn::FilterRef &, oidn::DeviceRef &){ });
}

void mine::Denoiser::dump(oidn::BufferRef const & source, RGBFloat32Bitmap & destination) const {
    std::size_t bytesConsumed = destination.width * destination.height * 3 * sizeof(float32_t);
    float * rawBufferPointer = static_cast<float32_t *>(source.getData());
    std::memcpy(destination.data.data(), rawBufferPointer, bytesConsumed);
}

oidn::BufferRef mine::Denoiser::createBufferFromBitmap(RGBFloat32Bitmap const & bitmap, oidn::DeviceRef & device) const {
    std::size_t bytesTotal = bitmap.width * bitmap.height * 3 * sizeof(float32_t);
    oidn::BufferRef buffer  = device.newBuffer(bytesTotal);
    float32_t * bufferRawPointer = static_cast<float32_t *>(buffer.getData());
    std::memcpy(bufferRawPointer, bitmap.data.data(), bytesTotal);
    return buffer;
}

bool mine::Denoiser::denoise(RGBFloat32Bitmap & lightenScene,
                             std::function<void(oidn::FilterRef &, oidn::DeviceRef &)> filterHook) const {
    oidn::DeviceRef device = oidn::newDevice(oidn::DeviceType::CPU);
    device.commit();
    
    oidn::BufferRef colorBuf  = createBufferFromBitmap(lightenScene, device);
        
    oidn::FilterRef filter = device.newFilter("RT");
    
    filterHook(filter, device);
    
    filter.setImage("color",  colorBuf,  oidn::Format::Float3, lightenScene.width, lightenScene.height);
    filter.setImage("output", colorBuf,  oidn::Format::Float3, lightenScene.width, lightenScene.height);
    
    filter.set("hdr", true);
    filter.commit();
    
    filter.execute();
    
    const char* errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None) {
        return false;
    }
    
    dump(colorBuf, lightenScene);
    
    return true;
}
