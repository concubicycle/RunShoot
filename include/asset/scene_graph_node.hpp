//
// Created by sava on 11/18/19.
//

#ifndef __SCENE_GRAPH_NODE_HPP_
#define __SCENE_GRAPH_NODE_HPP_

#include <glm/mat4x4.hpp>

#include <vector>
#include <functional>

template <typename TData>
class scene_graph_node
{
    using traverse_callback = std::function<void(TData&,glm::mat4&)>;

public:
    explicit scene_graph_node(TData& data) : _data(data) {}
    explicit scene_graph_node(TData& data, glm::mat4 transform) : _data(data), _transform(transform) {}

    [[nodiscard]] const glm::mat4& transform() const { return _transform; }

    void traverse(traverse_callback callback)
    {
        traverse_recurse(_transform, callback);
    }

    void add_child(TData& data)
    {
        _children.emplace_back(data);
    }

    void add_child(TData& data, glm::mat4 transform)
    {
        _children.emplace_back(data, transform);
    }

private:
    TData& _data;
    std::vector<scene_graph_node<TData>> _children;
    glm::mat4 _transform {1.f};

    void traverse_recurse(glm::mat4 accum, traverse_callback callback)
    {
        accum = _transform * accum;
        callback (_data, accum);
        for (auto& c : _children) c.traverse_recurse(accum, callback);
    }
};

#endif //__SCENE_GRAPH_NODE_HPP_
