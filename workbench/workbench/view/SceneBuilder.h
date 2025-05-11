//
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
    
    static std::vector<Triangle> backBoard() {
        // Back
        Triangle back1({
            simd::make_float3(-1.0 * mainScale, -1.0 * mainScale, 1.0 * backScale),
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale, 1.0 * backScale),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale, 1.0 * backScale)
        }, {
            simd::make_float2(-5, 5),
            simd::make_float2(-5, -5),
            simd::make_float2(5, 5)
        });
        
        Triangle back2({
            simd::make_float3(-1.0 * mainScale,  1.0 * mainScale, 1.0 * backScale),
            simd::make_float3( 1.0 * mainScale,  1.0 * mainScale, 1.0 * backScale),
            simd::make_float3( 1.0 * mainScale, -1.0 * mainScale, 1.0 * backScale)
        }, {
            simd::make_float2(-5, -5),
            simd::make_float2(5, -5),
            simd::make_float2(5, 5)
        });
        
        return {back1, back2};
    }
    
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
            OmniLight(Sphere({80, -80, 100}, 10), 1500, {1, 0, 0}),
            OmniLight(Sphere({0, -80, 100}, 10), 5000, {0, 1, 0}),
            OmniLight(Sphere({-80, -80, 100}, 10), 1500, {0, 0, 1}),
            OmniLight(Sphere({80, 80, 140}, 10), 15000, {1, 1, 1}),
            OmniLight(Sphere({0, 80, 140}, 10), 15000, {1, 1, 1}),
            OmniLight(Sphere({-80, 80, 140}, 10), 15000, {1, 1, 1})
        };
        
        Scene s{};
        
        auto white = std::make_shared<Material>(Bitmap(simd_make_float4(0.9, 0.9, 0.9, 1.0)),
                                                Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                                Bitmap(simd_make_float4(0.0, 0.0, 0.0, 1.0)),
                                                Bitmap::defaultNormalMap(),
                                                Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                                Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)));
        
        auto blue = std::make_shared<Material>(Bitmap(simd_make_float4(0.1, 0.1, 0.9, 1.0)),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                               Bitmap(simd_make_float4(0.0, 0.0, 0.0, 1.0)),
                                               Bitmap::defaultNormalMap(),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)));
        
        auto red = std::make_shared<Material>(Bitmap(simd_make_float4(0.9, 0.1, 0.1, 1.0)),
                                              Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                              Bitmap(simd_make_float4(0.0, 0.0, 0.0, 1.0)),
                                              Bitmap::defaultNormalMap(),
                                              Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                              Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)));
        
        auto mirror = std::make_shared<Material>(Bitmap(simd_make_float4(0.9, 0.1, 0.1, 1.0)),
                                                 Bitmap(simd_make_float4(0.0, 0.0, 0.0, 1.0)),
                                                 Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                                 Bitmap::defaultNormalMap(),
                                                 Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                                 Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)));
        
        auto metal = std::make_shared<Material>(*BitmapLoader::load("Metal055A_2K-JPG_Color.jpg"),
                                                *BitmapLoader::load("Metal055A_2K-JPG_Roughness.jpg"),
                                                *BitmapLoader::load("Metal055A_2K-JPG_Metalness.jpg"),
                                                *BitmapLoader::load("Metal055A_2K-JPG_NormalGL.jpg"),
                                                Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                                Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)));
        
        auto metal2 = std::make_shared<Material>(*BitmapLoader::load("Metal048C_2K-JPG_Color.jpg"),
                                                 *BitmapLoader::load("Metal048C_2K-JPG_Roughness.jpg"),
                                                 *BitmapLoader::load("Metal048C_2K-JPG_Metalness.jpg"),
                                                 *BitmapLoader::load("Metal048C_2K-JPG_NormalGL.jpg"),
                                                 Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                                 Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)));
        
        auto onyx = std::make_shared<Material>(*BitmapLoader::load("Onyx011_2K-JPG_Color.jpg"),
                                               *BitmapLoader::load("Onyx011_2K-JPG_Roughness.jpg"),
                                               Bitmap(simd_make_float4(0.0, 0.0, 0.0, 1.0)),
                                               *BitmapLoader::load("Onyx011_2K-JPG_NormalGL.jpg"),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)));
        
        auto tile = std::make_shared<Material>(*BitmapLoader::load("Tiles074_2K-JPG_Color.jpg"),
                                               *BitmapLoader::load("Tiles074_2K-JPG_Roughness.jpg"),
                                               Bitmap(simd_make_float4(0.0, 0.0, 0.0, 1.0)),
                                               *BitmapLoader::load("Tiles074_2K-JPG_NormalGL.jpg"),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)));
        
        auto gold = std::make_shared<Material>(*BitmapLoader::load("Metal048A_2K-JPG_Color.jpg"),
                                               *BitmapLoader::load("Metal048A_2K-JPG_Roughness.jpg"),
                                               *BitmapLoader::load("Metal048A_2K-JPG_Metalness.jpg"),
                                               *BitmapLoader::load("Metal048A_2K-JPG_NormalGL.jpg"),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)));
        
        auto darkTile = std::make_shared<Material>(*BitmapLoader::load("Tiles129B_2K-JPG_Color.jpg"),
                                                   *BitmapLoader::load("Tiles129B_2K-JPG_Roughness.jpg"),
                                                   Bitmap(simd_make_float4(0.0, 0.0, 0.0, 1.0)),
                                                   *BitmapLoader::load("Tiles129B_2K-JPG_NormalGL.jpg"),
                                                   Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                                   Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)));
        
        for (auto const & triangle : triangles) {
            tObjects.push_back(TriangleObject(triangle, white));
        }
        tObjects[2].material = red;
        tObjects[3].material = red;
        
        tObjects[4].material = blue;
        tObjects[5].material = blue;
        
        mirror->ior = 1.5f;
        
        std::vector<SphereObject> spheres = {
            SphereObject(Sphere({60, -60, 160}, 25), metal),
            SphereObject(Sphere({0, -60, 160}, 25), onyx),
            SphereObject(Sphere({-60, -60, 160}, 25), metal2),
            SphereObject(Sphere({60, 20, 160}, 25), gold),
            SphereObject(Sphere({0, 20, 160}, 25), mirror),
            SphereObject(Sphere({-60, 20, 160}, 25), tile)
        };
        s.environmentMap = std::make_optional(*BitmapLoader::load("autumn_field_puresky.jpg"));
        s.triangles = tObjects;
        s.omnilights = lights;
        s.spheres = spheres;
        return s;
    }
    
    static Scene buildHouseGraph() {
        NSBundle * bundle = [NSBundle mainBundle];
        NSString * path = [bundle pathForResource:@"3D_Design_Mini_House" ofType:@"usdz"];
        ModelLoader loader;
        SceneGraph loadedGraph = loader.load([path UTF8String]);
        std::unique_ptr<BaseNode> & root = loadedGraph.root;
        Node<TransformNodeContents> * transformNode = (Node<TransformNodeContents> *)(root.get());
        simd::float4x4 s = scale(simd_make_float3(10, 10, 10));
        float angle = M_PI + M_PI/4.0f;
        simd_quatf q = simd_quaternion(angle, simd_make_float3(0, 1, 0));
        simd_float3x3 rotMatrix = simd_matrix3x3(q);
        simd_float4x4 M = simd_matrix(
            simd_make_float4(rotMatrix.columns[0], 0.0f),
            simd_make_float4(rotMatrix.columns[1], 0.0f),
            simd_make_float4(rotMatrix.columns[2], 0.0f),
            simd_make_float4(0.0f, 0.0f, 0.0f, 1.0f)
        );
        transformNode->data.transform = translation(simd_make_float3(0, -20, 70)) * s * M;
        loadedGraph.environment = std::make_optional(*BitmapLoader::load("kiara_1_dawn.jpg"));
        return SceneFlattener().flatten(loadedGraph);
    }
    
    static Scene buildGraph() {
        SceneGraph g;
        auto gold = std::make_shared<Material>(*BitmapLoader::load("Metal048A_2K-JPG_Color.jpg"),
                                               *BitmapLoader::load("Metal048A_2K-JPG_Roughness.jpg"),
                                               *BitmapLoader::load("Metal048A_2K-JPG_Metalness.jpg"),
                                               *BitmapLoader::load("Metal048A_2K-JPG_NormalGL.jpg"),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)),
                                               Bitmap(simd_make_float4(1.0, 1.0, 1.0, 1.0)));
        SphereObject sphere(Sphere({0, 0, 0}, 25), gold);
        SphereNodeContents sphereContents;
        sphereContents.sphereObject = sphere;
        
        auto rootNode = std::make_unique<Node<TransformNodeContents>>();
        rootNode->data.transform = translation(simd_make_float3(0, 0, 0));
        g.root = std::move(rootNode);
        
        auto sphereTransform = std::make_unique<Node<TransformNodeContents>>();
        sphereTransform->data.transform = translation(simd_make_float3(60, 20, 160));
        sphereTransform->addChild(std::make_unique<Node<SphereNodeContents>>(sphereContents));
        g.root->addChild(std::move(sphereTransform));
        
        g.environment = std::make_optional(*BitmapLoader::load("kiara_1_dawn.jpg"));
        return SceneFlattener().flatten(g);
    }
};

