//
// Created by sava on 11/18/19.
//

#ifndef __SCENE_GRAPH_NODE_HPP_
#define __SCENE_GRAPH_NODE_HPP_

#include <glm/mat4x4.hpp>

#include <vector>
#include <functional>
#include "scene_graph_node_base.hpp"

namespace scene_graph
{
    template<typename TData, typename TId>
    class scene_graph_node : public scene_graph_node_base<TData, TId>
    {
        using traverse_callback = typename scene_graph_node_base<TData, TId>::traverse_callback;
        using TNode = scene_graph_node<TData, TId>;
        using TNodeBase = scene_graph_node_base<TData, TId>;


    public:

        explicit scene_graph_node(TData &data, TId id, scene_graph_node_base<TData, TId>& parent) :
            _id(id),
            _data(data),
            _parent(parent) {}

        explicit scene_graph_node(TData &data, TId id, glm::mat4 &transform, scene_graph_node_base<TData, TId>& parent) :
            _transform(transform),
            _id(id),
            _data(data),
            _parent(parent) {}

        scene_graph_node(const TNode& other) :
            _transform(other._transform),
            _data(other._data),
            _id(other._id),
            _parent(other._parent)
        {
        }

        scene_graph_node& operator=(const TNode& other) = delete;


        void traverse(traverse_callback callback) override
        {
            traverse_recurse(glm::mat4(1.f), callback);
        }

        TNodeBase& add_child(TData &data, TId id) override
        {
            _children.emplace_back(data, id, *this);
            return _children.back();
        }

        TNodeBase& add_child(TData &data, TId id, glm::mat4 transform) override
        {
            _children.emplace_back(data, id, transform, *this);
            return _children.back();
        }

        std::optional<std::reference_wrapper<TNodeBase>> insert(TData &data, TId id, TId parent_id) override
        {
            if (parent_id == _id)
            {
                return std::optional<std::reference_wrapper<TNodeBase>>(add_child(data, id));
            }
            else
            {
                for (auto &c : _children)
                {
                    auto opt = c.insert(data, id, parent_id);
                    if (opt) return opt;
                }
            }

            return std::optional<std::reference_wrapper<TNodeBase>>();
        }

        std::optional<std::reference_wrapper<TNodeBase>> insert(TData &data, TId id, TId parent_id, glm::mat4 transform) override
        {
            if (parent_id == _id)
            {
                return std::optional<std::reference_wrapper<TNodeBase>>(add_child(data, id, transform));
            }
            else
            {
                for (auto &c : _children)
                {
                    auto opt = c.insert(data, id, parent_id, transform);
                    if (opt) return opt;
                }
            }

            return std::optional<std::reference_wrapper<TNodeBase>>();
        }

        [[nodiscard]] glm::mat4 &transform() override { return _transform; }
        [[nodiscard]] glm::mat4 absolute_transform() const override { return _parent.absolute_transform() * _transform; }

    private:
        std::vector<scene_graph_node<TData, TId>> _children;


        glm::mat4 _transform{1.f};
        TId _id;
        TData &_data;
        scene_graph_node_base<TData, TId>& _parent;


        void traverse_recurse(glm::mat4 accum, traverse_callback callback)
        {
            accum = _transform * accum;
            callback(_data, accum);
            for (auto &c : _children) c.traverse_recurse(accum, callback);
        }
    };

}

#endif //__SCENE_GRAPH_NODE_HPP_
