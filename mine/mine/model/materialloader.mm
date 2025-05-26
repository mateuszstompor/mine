//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "../texture/color.h"
#include "materialloader.h"

std::optional<mine::RGBAFloat32Bitmap> mine::MaterialLoader::loadBitmap(MDLMaterialProperty * property) {
    if (property == nil) {
        return std::nullopt;
    } else if (property.type == MDLMaterialPropertyTypeFloat3) {
        simd::float3 storedValue = property.float3Value;
        return RGBAFloat32Bitmap(simd::make_float4(storedValue, 1.0));
    } else if (property.type == MDLMaterialPropertyTypeColor) {
        return loadPropertyTypeColor(property);
    } else if (property.type == MDLMaterialPropertyTypeString) {
        return std::nullopt;
    } else if (property.type == MDLMaterialPropertyTypeFloat) {
        float value = [property floatValue];
        return RGBAFloat32Bitmap(simd::make_float4(value, value, value, 1.0));
    } else if (property.type == MDLMaterialPropertyTypeTexture) {
        BitmapConverter converter;
        return converter.convert(loadPropertyTypeTexture(property));
    } else {
        // Not handled
        assert(false);
        return std::nullopt;
    }
}

mine::RGBAFloat32Bitmap mine::MaterialLoader::loadPropertyTypeColor(MDLMaterialProperty * property) {
    CGColorRef color = property.color;
    const CGFloat *components = CGColorGetComponents(color);
    size_t numberOfComponents = CGColorGetNumberOfComponents(color);

    CGFloat r, g, b, a;
    if (numberOfComponents == 1) {
        r = g = b = components[0];
        a = 1.0f;
    } else if (numberOfComponents == 2) {
        // Grayscale
        r = g = b = components[0];
        a = components[1];
    } else if (numberOfComponents == 3) {
        r = components[0];
        g = components[1];
        b = components[2];
        a = 1.0f;
    } else if (numberOfComponents == 4) {
        r = components[0];
        g = components[1];
        b = components[2];
        a = components[3];
    } else {
        r = g = b = a = 0.0;
    }
    return RGBAFloat32Bitmap(simd::make_float4(r, g, b, a));
}

mine::RGBAUint8Bitmap mine::MaterialLoader::loadPropertyTypeTexture(MDLMaterialProperty * property) {
    MDLTexture *mdlTexture = property.textureSamplerValue.texture;
    assert([mdlTexture channelEncoding] == MDLTextureChannelEncodingUint8);
    CGImageRef cgImage = [mdlTexture imageFromTexture];
    size_t width = CGImageGetWidth(cgImage);
    size_t height = CGImageGetHeight(cgImage);
    
    size_t bytesPerPixel = 4; // assuming RGBA
    size_t bytesPerRow = width * bytesPerPixel;

    std::vector<uint8_t> pixelData(width * height * bytesPerPixel);
    CGContextRef context = CGBitmapContextCreate(pixelData.data(),
                                                 width,
                                                 height,
                                                 8,
                                                 bytesPerRow,
                                                 CGImageGetColorSpace(cgImage),
                                                 kCGImageAlphaPremultipliedLast);
    
    CGContextTranslateCTM(context, 0, height);
    CGContextScaleCTM(context, 1.0, -1.0);
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), cgImage);
    CGContextRelease(context);
    
    mine::RGBAUint8Bitmap bitmap(
        pixelData.data(),
        (uint16_t)width,
        (uint16_t)height
    );
    return bitmap;
}

std::shared_ptr<mine::Material> mine::MaterialLoader::loadMaterial(MDLMaterial * material) {
    std::string materialName = [[material name] UTF8String];
    if (cache.contains(materialName)) {
        return cache[materialName];
    }
    std::optional<RGBAFloat32Bitmap> albedo = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticBaseColor]);
    if (!albedo) {
        albedo = RGBAFloat32Bitmap(simd::make_float4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    std::optional<RGBAFloat32Bitmap> normal = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticTangentSpaceNormal]);
    if (!normal) {
        normal = RGBAFloat32Bitmap(Color::defaultNormalMap());
    }
    std::optional<RGBAFloat32Bitmap> roughness = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticRoughness]);
    if (!roughness) {
        roughness = RGBAFloat32Bitmap(simd::make_float4(0.2f, 0.2f, 0.2f, 1.0f));
    }
    std::optional<RGBAFloat32Bitmap> metalness = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticMetallic]);
    if (!metalness) {
        metalness = RGBAFloat32Bitmap(simd::make_float4(0.5f, 0.5f, 0.5f, 1.0f));
    }
    std::optional<RGBAFloat32Bitmap> opacity = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticOpacity]);
    if (!opacity) {
        opacity = RGBAFloat32Bitmap(simd::make_float4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    std::optional<RGBAFloat32Bitmap> ior = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticMaterialIndexOfRefraction]);
    if (!ior) {
        ior = RGBAFloat32Bitmap(simd::make_float4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    std::shared_ptr<mine::Material> createdMaterial = std::make_shared<Material>(*albedo,
                                                                                 *roughness,
                                                                                 *metalness,
                                                                                 *normal,
                                                                                 *ior,
                                                                                 *opacity);
    
    cache[materialName] = createdMaterial;
    return createdMaterial;
}
