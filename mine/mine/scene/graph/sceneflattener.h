//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

#include <stddef.h>

#include "../scene.h"

#include "scenegraph.h"
#include "nodecontents.h"

namespace mine {
    class SceneFlattener {
    public:
        SceneFlattener() = default;
        Scene flatten(SceneGraph const & graph) {
            Scene outputScene;
            if (graph.root != nullptr) {
                process(graph.root, matrix_identity_float4x4, outputScene);
            }
            outputScene.environmentMap = graph.environment;
            return outputScene;
        }
    private:
        void process(std::unique_ptr<BaseNode> const & node, simd::float4x4 parentTransform, Scene & output) {
            if (Node<SphereNodeContents> * sphereNode = dynamic_cast<Node<SphereNodeContents>*>(node.get())) {
                SphereNodeContents const & contents = sphereNode->data;
                parentTransform = parentTransform * contents.transform;
                SphereObject outputObject = contents.sphereObject;
                outputObject.sphere.center = (parentTransform * simd_make_float4(outputObject.sphere.center, 1.0f)).xyz;
                output.spheres.push_back(outputObject);
            } else if (Node<TransformNodeContents> * transformNode = dynamic_cast<Node<TransformNodeContents>*>(node.get())) {
                TransformNodeContents const & contents = transformNode->data;
                parentTransform = parentTransform * contents.transform;
            } else if (Node<MeshNodeContents> * meshNode = dynamic_cast<Node<MeshNodeContents>*>(node.get())) {
                MeshNodeContents const & contents = meshNode->data;
                parentTransform = parentTransform * contents.transform;
                Mesh const & mesh = contents.mesh;
                for (Submesh const & submesh : mesh.submeshes) {
                    for (std::size_t vertexIndex = 0; vertexIndex < submesh.indices.size() - 3; vertexIndex += 3) {
                        uint32_t id1 = submesh.indices[vertexIndex];
                        uint32_t id2 = submesh.indices[vertexIndex + 1];
                        uint32_t id3 = submesh.indices[vertexIndex + 2];
                        
                        Vertex const & v1 = parentTransform * mesh.vertices[id1];
                        Vertex const & v2 = parentTransform * mesh.vertices[id2];
                        Vertex const & v3 = parentTransform * mesh.vertices[id3];
                        
                        
                        TriangleObject to(Triangle(v1, v2, v3),
                                          submesh.material);
                        
                        output.triangles.push_back(to);
                    }
                }
            }
            for (std::unique_ptr<BaseNode> const & child : node->getChildren()) {
                process(child, parentTransform, output);
            }
        }
    };
}
