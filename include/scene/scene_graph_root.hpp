//
// Created by sava on 11/19/19.
//

#ifndef __SCENE_GRAPH_ROOT_HPP_
#define __SCENE_GRAPH_ROOT_HPP_


#include <list>
#include "scene_graph_node.hpp"
#include "scene_graph_node_base.hpp"

namespace scene_graph
{

    template<typename TData, typename TId>
    class scene_graph_root : public scene_graph_node_base<TData, TId>
    {
        using TNode = scene_graph_node<TData, TId>;
        using TNodeBase = scene_graph_node_base<TData, TId>;
        using traverse_callback = typename scene_graph_node_base<TData, TId>::traverse_callback;

    public:
        scene_graph_root(std::function<glm::mat4(TData&)> extract_transform) : _extract_transform(extract_transform)
        {
        }

        scene_graph_root(const scene_graph_root<TData, TId>& other) = delete;

        scene_graph_root(scene_graph_root&& other) :
            _extract_transform(other._extract_transform),
            _children(std::move(other._children)),
            _transform(other._transform) {}


        void traverse(traverse_callback callback) override
        {
            for (auto& c : _children) c.traverse(callback);
        }

        TNodeBase& add_child(TData &data, TId id) override
        {
            _children.emplace_back(data, id, _extract_transform, *this);
            return _children.back();
        }

        std::optional<std::reference_wrapper<TNodeBase>> insert(TData& data, TId id, TId parent_id) override
        {
            for (auto& c : _children)
            {
                auto opt = c.insert(data, id, parent_id);
                if (opt) return opt;
            }
            return std::optional<std::reference_wrapper<TNodeBase>>();
        }

        void remove(TNodeBase& node) override
        {
            _children.remove_if([&node](TNode& child) { return child.id() == node.id(); });
        }

        void remove_from_parent() override
        {
        }

        void print(std::string prefix) override
        {
            std::cout << "root" << std::endl;
            for (auto& c : _children) c.print("\t");
        }


        [[nodiscard]] TId id() const override { return -1; }
        [[nodiscard]] glm::mat4 transform() const override { return  _transform; }
        [[nodiscard]] glm::mat4 absolute_transform() const override { return _transform; }

    private:
        std::list<TNode> _children {};

        glm::mat4 _transform{1.f};
        std::function<glm::mat4(TData&)> _extract_transform;

    };

}

#endif //__SCENE_GRAPH_ROOT_HPP_
