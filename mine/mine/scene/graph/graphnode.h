#pragma once

#include <memory>
#include <vector>

namespace mine {
    struct BaseNode {
        virtual ~BaseNode() = default;
        std::unique_ptr<BaseNode> & addChild(std::unique_ptr<BaseNode> child) {
            child->setParent(this);
            return doAddChild(std::move(child));
        }
        virtual const std::vector<std::unique_ptr<BaseNode>>& getChildren() const = 0;
    protected:
        virtual void setParent(BaseNode* p) = 0;
        virtual std::unique_ptr<BaseNode> & doAddChild(std::unique_ptr<BaseNode> child) = 0;
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
        std::unique_ptr<BaseNode> & doAddChild(std::unique_ptr<BaseNode> child) override {
            children.push_back(std::move(child));
            return *(--children.end());
        }
    public:
        T data;
    private:
        BaseNode* parent = nullptr;
        std::vector<std::unique_ptr<BaseNode>> children;
    };
}
