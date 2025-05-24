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
            oidn::BufferRef normalsBuf  = device.newBuffer(normals.width * normals.height * 3 * sizeof(float));
            float * normalPtr = (float*)normalsBuf.getData();
            std::memcpy(normalPtr, normals.data.data(), normals.width * normals.height * 3 * sizeof(float));
            filter.setImage("normal", normalsBuf,  oidn::Format::Float3, normals.width, normals.height);
        }
        
        {
            oidn::BufferRef albedoBuf  = device.newBuffer(albedo.width * albedo.height * 3 * sizeof(float));
            float * albedoPtr = (float*)albedoBuf.getData();
            std::memcpy(albedoPtr, albedo.data.data(), albedo.width * albedo.height * 3 * sizeof(float));
            filter.setImage("albedo", albedoBuf,  oidn::Format::Float3, albedo.width, albedo.height);
        }
    });
}

bool mine::Denoiser::denoise(RGBFloat32Bitmap & lightenScene) const {
    return denoise(lightenScene, [](oidn::FilterRef &, oidn::DeviceRef &){ });
}

bool mine::Denoiser::denoise(RGBFloat32Bitmap & lightenScene,
                             std::function<void(oidn::FilterRef &, oidn::DeviceRef &)> filterHook) const {
    oidn::DeviceRef device = oidn::newDevice(oidn::DeviceType::CPU);
    device.commit();
    
    
    oidn::BufferRef colorBuf  = device.newBuffer(lightenScene.width * lightenScene.height * 3 * sizeof(float));
    
    float * colorPtr = (float*)colorBuf.getData();
    std::memcpy(colorPtr, lightenScene.data.data(), lightenScene.width * lightenScene.height * 3 * sizeof(float));
    
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
    
    colorPtr = (float*)colorBuf.getData();
    std::memcpy(lightenScene.data.data(), colorPtr, lightenScene.width * lightenScene.height * 3 * sizeof(float));
    return true;
}
