//
// Created by sava on 11/20/19.
//

#ifndef __SCENE_GRAPH_NODE_BASE_HPP_
#define __SCENE_GRAPH_NODE_BASE_HPP_

#include <optional>

#include <glm/mat4x4.hpp>
#include <functional>

namespace scene_graph
{
    template <typename TData, typename TId>
    class scene_graph_node_base
    {
    public:
        using TNode = scene_graph_node_base<TData, TId>;
        using traverse_callback = std::function<void(TData &, glm::mat4 &)>;

        virtual void traverse(traverse_callback callback) = 0;
        virtual TNode& add_child(TData &data, TId id) = 0;


        virtual std::optional<std::reference_wrapper<TNode>> insert(TData& data, TId id, TId parent_id) = 0;

        virtual glm::mat4 transform() const = 0;
        virtual glm::mat4 absolute_transform() const = 0;

        virtual void remove(scene_graph_node_base<TData, TId>& node) = 0;
        virtual void remove_from_parent() = 0;
        virtual TId id() const = 0;

        virtual void print(std::string prefix) = 0;


        bool operator==(const scene_graph_node_base<TData, TId>& obj2) const
        {
            return id() == obj2.id();
        }
    };
}

#endif //__SCENE_GRAPH_NODE_BASE_HPP_
