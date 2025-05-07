#pragma once

#include <memory>
#include <vector>

namespace mine {
    struct BaseNode {
        virtual ~BaseNode() = default;
        void addChild(std::unique_ptr<BaseNode> child) {
            child->setParent(this);
            doAddChild(std::move(child));
        }
        virtual const std::vector<std::unique_ptr<BaseNode>>& getChildren() const = 0;
    protected:
        virtual void setParent(BaseNode* p) = 0;
        virtual void doAddChild(std::unique_ptr<BaseNode> child) = 0;
    };

    template<typename T>
    struct Node : public BaseNode {
        Node(const T & newData)
        : data{newData} {
            
        }
        Node()
        : data{} {
            
        }
        const std::vector<std::unique_ptr<BaseNode>>& getChildren() const override {
            return children;
        }
    protected:
        void setParent(BaseNode* p) override {
            parent = p;
        }
        void doAddChild(std::unique_ptr<BaseNode> child) override {
            children.push_back(std::move(child));
        }
    public:
        T data;
    private:
        BaseNode* parent = nullptr;
        std::vector<std::unique_ptr<BaseNode>> children;
    };
}
