//
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <cstdint>
#include <vector>
#include <cstring>

#include <simd/simd.h>

namespace mine {
    enum class BitmapType {
        RGBAUInt8,
        RGBAFloat32,
        RGBFloat32
    };
    struct Bitmap {
        BitmapType type;
        uint16_t width;
        uint16_t height;

        Bitmap(uint16_t width, uint16_t height, BitmapType type)
        : width{width}
        , height{height}
        , type{type} {
            // empty
        }
        virtual simd::float4 get(uint16_t x, uint16_t y) const = 0;
        virtual void set(uint16_t x, uint16_t y, simd::float4 const & normalizedColor) = 0;
        virtual ~Bitmap() = default;
    };
    struct RGBAUint8Bitmap : public Bitmap  {
        static constexpr uint8_t bytesPerPixel = 4;
        static constexpr uint8_t channels = 4;
        std::vector<uint8_t> data;
        
        RGBAUint8Bitmap(simd::float4 const & color);
        
        RGBAUint8Bitmap(const RGBAUint8Bitmap & other) = default;
        
        RGBAUint8Bitmap & operator=(const RGBAUint8Bitmap & other) = default;
        
        RGBAUint8Bitmap(uint16_t width,
                        uint16_t height);
        
        RGBAUint8Bitmap(uint8_t const * rawData,
                        uint16_t width,
                        uint16_t height);
                
        simd::float4 get(uint16_t x, uint16_t y) const override;
                
        void set(uint16_t x,
                 uint16_t y,
                 simd::float4 const & normalized) override;
    };
    
    struct RGBAFloat32Bitmap : public Bitmap  {
        static constexpr uint8_t bytesPerPixel = 16;
        static constexpr uint8_t channels = 4;
        std::vector<float32_t> data;
        
        RGBAFloat32Bitmap(simd::float4 const & color);
        
        RGBAFloat32Bitmap(const RGBAFloat32Bitmap & other) = default;
        
        RGBAFloat32Bitmap & operator=(const RGBAFloat32Bitmap & other) = default;
        
        RGBAFloat32Bitmap(uint16_t width, uint16_t height);
        
        simd::float4 get(uint16_t x, uint16_t y) const override;
        
        void set(uint16_t x,
                 uint16_t y,
                 simd::float4 const & normalized) override;
    };

    struct RGBFloat32Bitmap : public Bitmap  {
        static constexpr uint8_t bytesPerPixel = 12;
        static constexpr uint8_t channels = 3;
        std::vector<float32_t> data;
        
        RGBFloat32Bitmap(simd::float3 const & color);
        
        RGBFloat32Bitmap(const RGBFloat32Bitmap & other) = default;
        
        RGBFloat32Bitmap & operator=(const RGBFloat32Bitmap & other) = default;
        
        RGBFloat32Bitmap(uint16_t width, uint16_t height);
        
        simd::float4 get(uint16_t x, uint16_t y) const override;
        
        void set(uint16_t x,
                 uint16_t y,
                 simd::float4 const & normalized) override;
    };

    struct BitmapConverter {
        RGBAFloat32Bitmap convert(RGBAUint8Bitmap const & bitmap) {
            RGBAFloat32Bitmap result(bitmap.width, bitmap.height);
            for (uint16_t x = 0; x < bitmap.width; ++x) {
                for (uint16_t y = 0; y < bitmap.height; ++y) {
                    simd::float4 color = bitmap.get(x, y);
                    result.set(x, y, color);
                }
            }
            return result;
        }
        
        RGBAUint8Bitmap convert(RGBAFloat32Bitmap const & bitmap) {
            RGBAUint8Bitmap result(bitmap.width, bitmap.height);
            for (uint16_t x = 0; x < bitmap.width; ++x) {
                for (uint16_t y = 0; y < bitmap.height; ++y) {
                    simd::float4 color = bitmap.get(x, y);
                    result.set(x, y, color);
                }
            }
            return result;
        }
        
        RGBFloat32Bitmap dropAlpha(RGBAFloat32Bitmap const & bitmap) {
            RGBFloat32Bitmap result(bitmap.width, bitmap.height);
            for (uint16_t x = 0; x < bitmap.width; ++x) {
                for (uint16_t y = 0; y < bitmap.height; ++y) {
                    simd::float4 color = bitmap.get(x, y);
                    result.set(x, y, color);
                }
            }
            return result;
        }
        
        RGBAFloat32Bitmap extendAlpha(RGBFloat32Bitmap const & bitmap) {
            RGBAFloat32Bitmap result(bitmap.width, bitmap.height);
            for (uint16_t x = 0; x < bitmap.width; ++x) {
                for (uint16_t y = 0; y < bitmap.height; ++y) {
                    simd::float4 color = bitmap.get(x, y);
                    result.set(x, y, color);
                }
            }
            return result;
        }
    };
}
