//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#pragma once

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
                parentTransform = contents.transform * parentTransform;
                SphereObject outputObject = contents.sphereObject;
                outputObject.sphere.center = (parentTransform * simd_make_float4(outputObject.sphere.center, 1.0f)).xyz;
                output.spheres.push_back(outputObject);
            } else if (Node<TransformNodeContents> * transformNode = dynamic_cast<Node<TransformNodeContents>*>(node.get())) {
                TransformNodeContents const & contents = transformNode->data;
                parentTransform = contents.transform * parentTransform;
            }
            for (std::unique_ptr<BaseNode> const & child : node->getChildren()) {
                process(child, parentTransform, output);
            }
        }
    };
}
