//
// Created by sava on 11/19/19.
//

#ifndef __SCENE_GRAPH_ROOT_HPP_
#define __SCENE_GRAPH_ROOT_HPP_


#include <vector>
#include "scene_graph_node.hpp"
#include "scene_graph_node_base.hpp"

namespace asset
{

    template<typename TData, typename TId>
    class scene_graph_root : public scene_graph_node_base<TData, TId>
    {
        using TNode = scene_graph_node<TData, TId>;
        using traverse_callback = typename scene_graph_node_base<TData, TId>::traverse_callback;

    public:
        void traverse(traverse_callback callback) override
        {
            for (auto& c : _children) c.traverse(callback);
        }

        void add_child(TData &data, TId id) override
        {
            _children.emplace_back(data, id, *this);
        }

        void add_child(TData &data, TId id, glm::mat4 transform) override
        {
            _children.emplace_back(data, id, transform, *this);
        }

        void insert(TData& data, TId id, TId parent_id) override
        {
            for (auto& c : _children) c.insert(data, id, parent_id);
        }

        void insert(TData& data, TId id, TId parent_id, glm::mat4 transform) override
        {
            for (auto& c : _children) c.insert(data, id, parent_id, transform);
        }

        [[nodiscard]] glm::mat4& transform() override { return  _transform; }
        [[nodiscard]] glm::mat4 absolute_transform() const override { return _transform; }

    private:
        std::vector<TNode> _children {};
        glm::mat4 _transform{1.f};
    };

}

#endif //__SCENE_GRAPH_ROOT_HPP_
