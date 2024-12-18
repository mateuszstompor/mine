//
//  scenebuilder.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <simd/simd.h>

#include <vector>

#include "./scene/triangle.h"
#include "./scene/scene.h"

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
        auto cornellBox = openCornellBox();
        Triangle front11({
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale, -1.0 * backScale),
            simd::make_float3(-1.0 * mainScale, -1.0 * mainScale, -1.0 * backScale),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale, -1.0 * backScale)
        }, {
            simd::make_float2(0, 0),
            simd::make_float2(0, 1),
            simd::make_float2(1, 1)
        });
        
        Triangle front12({
            simd::make_float3( 1.0 * mainScale,  1.0 * mainScale, -1.0 * backScale),
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale, -1.0 * backScale),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale, -1.0 * backScale)
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
        auto triangles = closedCornellBox();
        std::vector<OmniLight> lights = {
            OmniLight(Sphere({0, 60, 120}, 5), 10000.0)
        };
        std::vector<Sphere> spheres = {
            Sphere({50, -60, 160}, 40),
            Sphere({-50, -60, 120}, 40)
        };
        Scene s{};
        s.triangles = triangles;
        s.omnilights = lights;
        s.spheres = spheres;
        return s;
    }
};

