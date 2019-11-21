//
// Created by sava on 10/8/19.
//

#ifndef __ENTITY_H_
#define __ENTITY_H_

#include "chunk_component_accessor.hpp"
#include "ecs_types.hpp"
#include <optional>
#include <scene/scene_graph_node.hpp>

namespace ecs
{
    class entity
    {
    public:
        // TODO: Fix design, this is a quick hack
        scene_graph::scene_graph_node_base<entity, entity_id>* graph_node = nullptr;

        entity(
            entity_id id,
            component_bitset archetype_id,
            std::uint8_t *chunk_ptr);

        entity(const entity& other);

        [[nodiscard]] entity_id id() const { return _id; }
        [[nodiscard]] void *ptr() const { return _accessor.ptr(); }
        [[nodiscard]] component_bitset archetype_id() const { return _archetype_id; }
        [[nodiscard]] bool has(component_bitset component_bit) const { return _archetype_id & component_bit; }

        void destroy();
        void copy_components_from(entity& other);

        template<typename T>
        T &get_component()
        {
            return *(_accessor.get_component<T>());
        }

        template<typename T>
        std::optional<std::reference_wrapper<T>> get_component_opt()
        {
            return has<T>()
                   ? std::optional<std::reference_wrapper<T>>(*(_accessor.get_component<T>()))
                   : std::optional<std::reference_wrapper<T>>();
        }

        template<class T>
        bool has()
        {
            return _archetype_id & component<T>::archetype_bit;
        }


    private:
        entity_id _id;
        component_bitset  _archetype_id;
        chunk_component_accessor _accessor;
        bool _active;
    };
}


#endif //__ENTITY_H_
