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
                     std::optional<std::pair<RGBFloat32Bitmap, RGBFloat32Bitmap>> const & normalsAndAlbedo) const {
            oidn::DeviceRef device = oidn::newDevice(oidn::DeviceType::CPU);
            device.commit();


            oidn::BufferRef colorBuf  = device.newBuffer(lightenScene.width * lightenScene.height * 3 * sizeof(float));
            
            float * colorPtr = (float*)colorBuf.getData();
            std::memcpy(colorPtr, lightenScene.data.data(), lightenScene.width * lightenScene.height * 3 * sizeof(float));
            
            oidn::FilterRef filter = device.newFilter("RT");
            
            
            if (normalsAndAlbedo) {
                RGBFloat32Bitmap normalsRGB = std::get<0>(*normalsAndAlbedo);
                RGBFloat32Bitmap albedoRGB = std::get<1>(*normalsAndAlbedo);
                oidn::BufferRef normalsBuf  = device.newBuffer(normalsRGB.width * normalsRGB.height * 3 * sizeof(float));
                oidn::BufferRef albedoBuf  = device.newBuffer(albedoRGB.width * albedoRGB.height * 3 * sizeof(float));
                
                float * normalPtr = (float*)normalsBuf.getData();
                std::memcpy(normalPtr, normalsRGB.data.data(), normalsRGB.width * normalsRGB.height * 3 * sizeof(float));
                
                float * albedoPtr = (float*)albedoBuf.getData();
                std::memcpy(albedoPtr, albedoRGB.data.data(), albedoRGB.width * albedoRGB.height * 3 * sizeof(float));
                
                filter.setImage("normal", normalsBuf,  oidn::Format::Float3, normalsRGB.width, normalsRGB.height); // denoised beauty
                filter.setImage("albedo", albedoBuf,  oidn::Format::Float3, albedoRGB.width, albedoRGB.height); // denoised beauty
            }
            
            filter.setImage("color",  colorBuf,  oidn::Format::Float3, lightenScene.width, lightenScene.height);
            filter.setImage("output", colorBuf,  oidn::Format::Float3, lightenScene.width, lightenScene.height);
            

            filter.set("hdr", true);
            filter.commit();

            filter.execute();

            const char* errorMessage;
            if (device.getError(errorMessage) != oidn::Error::None) {
                std::cout << "Error: " << errorMessage << std::endl;
                return false;
            }
            
            colorPtr = (float*)colorBuf.getData();
            std::memcpy(lightenScene.data.data(), colorPtr, lightenScene.width * lightenScene.height * 3 * sizeof(float));
            
            return true;
        }
    };
}
