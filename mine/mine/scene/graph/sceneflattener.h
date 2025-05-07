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
                process(graph.root, outputScene);
            }
            outputScene.environmentMap = graph.environment;
            return outputScene;
        }
    private:
        void process(std::unique_ptr<BaseNode> const & node, Scene & output) {
            if (Node<SphereNodeContents> * sphereNode = dynamic_cast<Node<SphereNodeContents>*>(node.get())) {
                output.spheres.push_back(sphereNode->data.sphereObject);
            }
            
            for (std::unique_ptr<BaseNode> const & child : node->getChildren()) {
                process(child, output);
            }
        }
    };
}
