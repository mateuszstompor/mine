//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#import <spdlog/spdlog.h>

#include "modelloader.h"
#include "../scene/primitive/mesh.h"

#import <ModelIO/ModelIO.h>
#import <Metal/Metal.h>

mine::SceneGraph mine::ModelLoader::load(std::string const & path) {
    SceneGraph graph;
    graph.root = std::make_unique<Node<TransformNodeContents>>();
    NSString *nsPath = [NSString stringWithUTF8String:path.c_str()];
    NSURL *url = [NSURL fileURLWithPath:nsPath];
    
    MDLAsset *asset = [[[MDLAsset alloc] initWithURL:url] autorelease];
    [asset loadTextures];
    if (asset.count == 0) {
        spdlog::error("Failed to load model at path: {0}", path);
        return graph;
    }

    for (NSUInteger i = 0; i < asset.count; ++i) {
        MDLObject *rootObject = [asset objectAtIndex:i];
        processObject(rootObject, graph.root);
    }
    return graph;
}

std::unique_ptr<mine::BaseNode> mine::ModelLoader::loadTransformNode(MDLObject *object) {
    assert(object != nil);
    simd::float4x4 nodeTransform = matrix_identity_float4x4;
    if ([object transform]) {
        id<MDLTransformComponent> transform = [object transform];
        assert(![transform resetsTransform]);
        nodeTransform = [transform matrix];
    }
    std::unique_ptr<Node<TransformNodeContents>> tn = std::make_unique<Node<TransformNodeContents>>();
    tn->data.name = [object.name UTF8String];
    tn->data.transform = nodeTransform;
    return tn;
}

std::unique_ptr<mine::BaseNode> mine::ModelLoader::loadMeshNode(MDLMesh * object) {
    assert(object != nil);
    simd::float4x4 nodeTransform = matrix_identity_float4x4;
    if ([object transform]) {
        id<MDLTransformComponent> transform = [object transform];
        assert(![transform resetsTransform]);
        nodeTransform = [transform matrix];
    }
    MDLMesh *mesh = (MDLMesh *)object;
    spdlog::info("Processing mesh: {0}", [mesh.name UTF8String]);
    Mesh m;
    if ([mesh vertexAttributeDataForAttributeNamed:MDLVertexAttributeTextureCoordinate] == nil) {
        spdlog::info("Generating uv coordinates");
        [mesh addUnwrappedTextureCoordinatesForAttributeNamed:MDLVertexAttributeTextureCoordinate];
    }

    if ([mesh vertexAttributeDataForAttributeNamed:MDLVertexAttributeTangent] == nil) {
        spdlog::info("Generating tangents and bitangents");
        [mesh addOrthTanBasisForTextureCoordinateAttributeNamed:MDLVertexAttributeTextureCoordinate
                                           normalAttributeNamed:MDLVertexAttributeNormal
                                          tangentAttributeNamed:MDLVertexAttributeTangent];
    }
            
    MDLVertexAttributeData *positions =
        [mesh vertexAttributeDataForAttributeNamed:MDLVertexAttributePosition];
    MDLVertexAttributeData *normals =
        [mesh vertexAttributeDataForAttributeNamed:MDLVertexAttributeNormal];
    MDLVertexAttributeData *tangent =
        [mesh vertexAttributeDataForAttributeNamed:MDLVertexAttributeTangent];
    MDLVertexAttributeData *uvs =
    [mesh vertexAttributeDataForAttributeNamed:MDLVertexAttributeTextureCoordinate];
    
    assert(positions.format == MDLVertexFormatFloat3);
    assert(normals.format == MDLVertexFormatFloat3);
    assert(tangent.format == MDLVertexFormatFloat4);
    assert(uvs.format == MDLVertexFormatFloat2);
    
    uint8_t * positionsBuffer = reinterpret_cast<uint8_t *>(positions.dataStart);
    uint8_t * normalsBuffer = reinterpret_cast<uint8_t *>(normals.dataStart);
    uint8_t * tangentsBuffer = reinterpret_cast<uint8_t *>(tangent.dataStart);
    uint8_t * uvsBuffer = reinterpret_cast<uint8_t *>(uvs.dataStart);
    
    m.vertices.reserve(mesh.vertexCount);
    for (int i = 0; i < mesh.vertexCount; ++i) {
        simd::float3 position = *((simd::float3 *)positionsBuffer);
        simd::float3 normal = *((simd::float3 *)normalsBuffer);
        simd::float4 tangent4f = *((simd::float4 *)tangentsBuffer);
        assert(tangent4f.w != 0.0f);
        simd::float3 tangent3f = simd::normalize(tangent4f.xyz / tangent4f.w);
        
        simd::float3 bitangent = simd::cross(normal, tangent3f);
        
        simd::float2 uv = *((simd::float2 *)uvsBuffer);
        
        m.vertices.push_back(Vertex {
            position,
            tangent3f,
            bitangent,
            normal,
            uv
        });
        
        positionsBuffer += positions.stride;
        normalsBuffer += normals.stride;
        tangentsBuffer += tangent.stride;
        uvsBuffer += uvs.stride;
        
    }
    assert(m.vertices.size() == object.vertexCount);
    assert(mesh.submeshes);
    m.submeshes.reserve(mesh.submeshes.count);
    for (MDLSubmesh *submesh in mesh.submeshes) {
        assert(submesh.indexBuffer.length / 4 == submesh.indexCount);
        assert(submesh.indexType == MDLIndexBitDepthUInt32);
        assert(submesh.geometryType == MDLGeometryTypeTriangles);
        
        Submesh sm;
        sm.indices.resize(submesh.indexCount);
        void * destination = sm.indices.data();
        
        void const * source = submesh.indexBuffer.map.bytes;
        NSUInteger length = submesh.indexBuffer.length;
        std::memcpy(destination, source, length);
        MDLMaterial *material = submesh.material;
        
        spdlog::info("Material, name: {0}", [material.name UTF8String]);
        sm.material = loadMaterial(material);
        m.submeshes.push_back(sm);
    }
    spdlog::info("Mesh: {0}, has {1} vertices and {2} submeshes",
    [mesh.name UTF8String],
    m.vertices.size(),
    m.submeshes.size());
    std::unique_ptr<Node<MeshNodeContents>> mn = std::make_unique<Node<MeshNodeContents>>();
    mn->data.name = [object.name UTF8String];
    mn->data.transform = nodeTransform;
    mn->data.mesh = m;
    return mn;
}

std::unique_ptr<mine::BaseNode> mine::ModelLoader::loadGenericNode(MDLObject * node) {
    spdlog::info("Processing node: {0}", [node.name UTF8String]);
    if ([node isKindOfClass:[MDLMesh class]]) {
        return loadMeshNode((MDLMesh *)node);
    } else {
        return loadTransformNode(node);
    }
    assert(false);
    return nullptr;
}

void mine::ModelLoader::processObject(MDLObject *object, std::unique_ptr<BaseNode> & currentParent) {
    assert(currentParent != nullptr);
    std::unique_ptr<BaseNode> loadedNode = loadGenericNode(object);
    assert(loadedNode != nullptr);
    std::unique_ptr<BaseNode> & addedChild = currentParent->addChild(std::move(loadedNode));
    for (MDLObject *child in object.children.objects) {
        processObject(child, addedChild);
    }
}

std::shared_ptr<mine::Material> mine::ModelLoader::loadMaterial(MDLMaterial * material) {
    std::optional<Bitmap> albedo = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticBaseColor]);
    if (!albedo) {
        albedo = Bitmap(simd_make_float4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    std::optional<Bitmap> normal = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticTangentSpaceNormal]);
    if (!normal) {
        normal = Bitmap::defaultNormalMap();
    }
    std::optional<Bitmap> roughness = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticRoughness]);
    if (!roughness) {
        roughness = Bitmap(simd_make_float4(0.2f, 0.2f, 0.2f, 1.0f));
    }
    std::optional<Bitmap> metalness = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticMetallic]);
    if (!metalness) {
        metalness = Bitmap(simd_make_float4(0.5f, 0.5f, 0.5f, 1.0f));
    }
    std::optional<Bitmap> opacity = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticOpacity]);
    if (!opacity) {
        opacity = Bitmap(simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    std::optional<Bitmap> ior = loadBitmap([material propertyWithSemantic:MDLMaterialSemanticMaterialIndexOfRefraction]);
    if (!ior) {
        ior = Bitmap(simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    return std::make_shared<Material>(*albedo,
                                      *roughness,
                                      *metalness,
                                      *normal,
                                      *ior,
                                      *opacity);
}

std::optional<mine::Bitmap> mine::ModelLoader::loadBitmap(MDLMaterialProperty * property) {
    if (property == nil) {
        return std::nullopt;
    }
    if (property.type == MDLMaterialPropertyTypeFloat3) {
        simd::float3 storedValue = property.float3Value;
        return Bitmap(simd_make_float4(storedValue, 1.0));
    } else if (property.type == MDLMaterialPropertyTypeColor) {
        CGColorRef color = property.color;
        const CGFloat *components = CGColorGetComponents(color);
        size_t numberOfComponents = CGColorGetNumberOfComponents(color);

        CGFloat r, g, b;

        // Handle RGB and grayscale color spaces
        if (numberOfComponents == 2) {
            // Grayscale + alpha
            r = g = b = components[0];
        } else if (numberOfComponents >= 3) {
            r = components[0];
            g = components[1];
            b = components[2];
        } else {
            // Fallback or error handling
            r = g = b = 0.0;
        }
        return Bitmap(simd_make_float4(r, g, b, 1.0));
    } else if (property.type == MDLMaterialPropertyTypeString) {
        return std::nullopt;
    } else if (property.type == MDLMaterialPropertyTypeFloat) {
        float value = [property floatValue];
        return Bitmap(simd_make_float4(value, value, value, 1.0));
    } else if (property.type == MDLMaterialPropertyTypeTexture) {
        MDLTexture *mdlTexture = property.textureSamplerValue.texture;
        CGImageRef cgImage = [mdlTexture imageFromTexture];
        if (cgImage) {
            size_t width = CGImageGetWidth(cgImage);
            size_t height = CGImageGetHeight(cgImage);
            
            size_t bpp = 4; // assuming RGBA
            size_t bytesPerRow = width * bpp;

            std::vector<uint8_t> pixelData(width * height * bpp);
            CGContextRef context = CGBitmapContextCreate(pixelData.data(),
                                                         width,
                                                         height,
                                                         8,
                                                         bytesPerRow,
                                                         CGImageGetColorSpace(cgImage),
                                                         kCGImageAlphaPremultipliedLast);

            if (context) {
                CGContextDrawImage(context, CGRectMake(0, 0, width, height), cgImage);
                CGContextRelease(context);
                
                mine::Bitmap bitmap(
                    pixelData.data(),
                    (uint16_t)width,
                    (uint16_t)height,
                    (uint8_t)bpp
                );
                return bitmap;
            }
        }
    }
    assert(false);
    return std::nullopt;
}
