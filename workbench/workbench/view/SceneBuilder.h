//
//  scenebuilder.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <simd/simd.h>
#include <mine/mine.h>

#include <vector>

using namespace mine;

class SceneBuilder {
public:
    static constexpr double mainScale = 100.0;
    static constexpr double backScale = 200.0;
    
    static std::vector<Triangle> openCornellBox() {
        // Back
        Triangle back1({
            simd::make_float3(-1.0 * mainScale, -1.0 * mainScale, 1.0 * backScale),
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale, 1.0 * backScale),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale, 1.0 * backScale)
        }, {
            simd::make_float2(0, 1),
            simd::make_float2(0, 0),
            simd::make_float2(1, 1)
        });
        
        Triangle back2({
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale, 1.0 * backScale),
            simd::make_float3( 1.0 * mainScale,  1.0 * mainScale, 1.0 * backScale),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale, 1.0 * backScale)
        }, {
            simd::make_float2(0, 0),
            simd::make_float2(1, 0),
            simd::make_float2(1, 1)
        });
        
        // Other faces
        Triangle left3({
            simd::make_float3(-1.0 * mainScale, -1.0 * mainScale, -1.0 * backScale),
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale, -1.0 * backScale),
            simd::make_float3(-1.0 * mainScale, -1.0 * mainScale,  1.0 * backScale)
        }, {
            simd::make_float2(0, 1),
            simd::make_float2(0, 0),
            simd::make_float2(1, 1)
        });
        
        Triangle left4({
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale, -1.0 * backScale),
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale,  1.0 * backScale),
            simd::make_float3(-1.0 * mainScale, -1.0 * mainScale,  1.0 * backScale)
        }, {
            simd::make_float2(0, 0),
            simd::make_float2(1, 0),
            simd::make_float2(1, 1)
        });
        
        Triangle right5({
            simd::make_float3( 1.0 * mainScale,  1.0 * mainScale, -1.0 * backScale),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale, -1.0 * backScale),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale,  1.0 * backScale)
        }, {
            simd::make_float2(0, 1),
            simd::make_float2(0, 0),
            simd::make_float2(1, 0)
        });
        
        Triangle right6({
            simd::make_float3( 1.0 * mainScale,  1.0 * mainScale,  1.0 * backScale),
            simd::make_float3( 1.0 * mainScale,  1.0 * mainScale, -1.0 * backScale),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale,  1.0 * backScale)
        }, {
            simd::make_float2(0, 0),
            simd::make_float2(1, 0),
            simd::make_float2(0, 1)
        });
        
        Triangle bottom7({
            simd::make_float3( -1.0 * mainScale,  -1.0 * mainScale, -1.0 * backScale),
            simd::make_float3( -1.0 * mainScale, -1.0 * mainScale, 1.0 * backScale),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale,  -1.0 * backScale)
        }, {
            simd::make_float2(0, 1),
            simd::make_float2(0, 0),
            simd::make_float2(1, 1)
        });
        
        Triangle bottom8({
            simd::make_float3( 1.0 * mainScale,  -1.0 * mainScale,  -1.0 * backScale),
            simd::make_float3( -1.0 * mainScale,  -1.0 * mainScale, 1.0 * backScale),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale,  1.0 * backScale)
        }, {
            simd::make_float2(1, 1),
            simd::make_float2(0, 0),
            simd::make_float2(1, 0)
        });
        
        Triangle top9({
            simd::make_float3( 1.0 * mainScale, 1.0 * mainScale,  -1.0 * backScale),
            simd::make_float3( -1.0 * mainScale,  1.0 * mainScale, 1.0 * backScale),
            simd::make_float3( -1.0 * mainScale, 1.0 * mainScale, -1.0 * backScale)
        }, {
            simd::make_float2(0, 0),
            simd::make_float2(0, 1),
            simd::make_float2(1, 1)
        });
        
        Triangle top10({
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale, 1.0 * backScale),
            simd::make_float3(1.0 * mainScale, 1.0 * mainScale, -1.0 * backScale),
            simd::make_float3(1.0 * mainScale, 1.0 * mainScale, 1.0 * backScale)
        }, {
            simd::make_float2(0, 0),
            simd::make_float2(1, 1),
            simd::make_float2(1, 0)
        });
        
        // Combine all triangles into a single vector
        return {back1, back2, left3, left4, right5, right6, bottom7, bottom8, top9, top10};
    }
    
    static std::vector<Triangle> closedCornellBox() {
        float offset = 190.0f;
        auto cornellBox = openCornellBox();
        Triangle front11({
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale, -1.0 * backScale + offset),
            simd::make_float3(-1.0 * mainScale, -1.0 * mainScale, -1.0 * backScale + offset),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale, -1.0 * backScale + offset)
        }, {
            simd::make_float2(0, 0),
            simd::make_float2(0, 1),
            simd::make_float2(1, 1)
        });
        
        Triangle front12({
            simd::make_float3( 1.0 * mainScale,  1.0 * mainScale, -1.0 * backScale + offset),
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale, -1.0 * backScale + offset),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale, -1.0 * backScale + offset)
        }, {
            simd::make_float2(1, 0),
            simd::make_float2(0, 0),
            simd::make_float2(1, 1)
        });
        
        cornellBox.push_back(front11);
        cornellBox.push_back(front12);
        return cornellBox;
    }
    
    static Scene cornellBox() {
        std::vector<Triangle> triangles = closedCornellBox();
        std::vector<TriangleObject> tObjects;
        
        std::vector<OmniLight> lights = {
//            OmniLight(Sphere({80, 100, 100}, 10), 15000.0),
//            OmniLight(Sphere({0, 100, 100}, 10), 15000.0),
//            OmniLight(Sphere({-80, 100, 100}, 10), 15000.0),
//            OmniLight(Sphere({80, 100, 140}, 10), 15000.0),
            OmniLight(Sphere({0, 100, 140}, 10), 30000.0)
//            OmniLight(Sphere({-80, 100, 140}, 10), 15000.0)
        };
        
        Scene s{};
        
        auto white = std::make_shared<Material>(Bitmap(simd_make_float4(0.9, 0.9, 0.9, 1.0)),
                                                Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                                Bitmap(simd_make_float4(0.0, 0.0, 0.0, 1.0)),
                                                Bitmap::defaultNormalMap());
        
        auto blue = std::make_shared<Material>(Bitmap(simd_make_float4(0.1, 0.1, 0.9, 1.0)),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                               Bitmap(simd_make_float4(0.0, 0.0, 0.0, 1.0)),
                                               Bitmap::defaultNormalMap());
        
        auto red = std::make_shared<Material>(Bitmap(simd_make_float4(0.9, 0.1, 0.1, 1.0)),
                                              Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                              Bitmap(simd_make_float4(0.0, 0.0, 0.0, 1.0)),
                                              Bitmap::defaultNormalMap());
        
        auto mirror = std::make_shared<Material>(Bitmap(simd_make_float4(0.9, 0.1, 0.1, 1.0)),
                                                 Bitmap(simd_make_float4(0.0, 0.0, 0.0, 1.0)),
                                                 Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                                 Bitmap::defaultNormalMap());
        
        auto metal = std::make_shared<Material>(*BitmapLoader::load("Metal055A_2K-JPG_Color.jpg"),
                                                *BitmapLoader::load("Metal055A_2K-JPG_Roughness.jpg"),
                                                *BitmapLoader::load("Metal055A_2K-JPG_Metalness.jpg"),
                                                *BitmapLoader::load("Metal055A_2K-JPG_NormalGL.jpg"));
        
        for (auto const & triangle : triangles) {
            tObjects.push_back(TriangleObject(triangle, white));
        }
        tObjects[2].material = red;
        tObjects[3].material = red;
        
        tObjects[4].material = blue;
        tObjects[5].material = blue;
        
        std::vector<SphereObject> spheres = {
            SphereObject(Sphere({50, -60, 160}, 40), metal),
            SphereObject(Sphere({-50, -60, 120}, 40), white),
            SphereObject(Sphere({0, -80, 170}, 20), mirror)
        };
        
        s.triangles = tObjects;
        s.omnilights = lights;
        s.spheres = spheres;
        return s;
    }
};

