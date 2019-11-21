//
// Created by sava on 11/20/19.
//

#ifndef __SCENE_GRAPH_NODE_BASE_HPP_
#define __SCENE_GRAPH_NODE_BASE_HPP_

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
        virtual void add_child(TData &data, TId id) = 0;
        virtual void add_child(TData &data, TId id, glm::mat4 transform) = 0;
        virtual void insert(TData& data, TId id, TId parent_id) = 0;
        virtual void insert(TData& data, TId id, TId parent_id, glm::mat4 transform) = 0;

        virtual glm::mat4& transform() = 0;

        virtual glm::mat4 absolute_transform() const = 0;

    private:


    };
}

#endif //__SCENE_GRAPH_NODE_BASE_HPP_
