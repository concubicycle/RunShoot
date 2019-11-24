//
// Created by sava on 10/8/19.
//

#ifndef __ENTITY_WORLD_H_
#define __ENTITY_WORLD_H_

#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <vector>
#include <functional>

#include <events/event_exchange.hpp>

#include "entity.hpp"
#include "ecs_types.hpp"
#include "entity_factory.hpp"

namespace ecs
{
    /**
     * This is the primary class for storing and querying entities.
     */
    class entity_world
    {
    public:
        entity_world(entity_factory& world, events::event_exchange& events) :
            _entity_factory(world),
            _events(events)
        {
        }

        entity& add_entity(component_bitset archetype_id, entity_id id);
        entity& add_entity(component_bitset archetype_id);

        void remove_entity(entity_id entity_id);


        entity& get_entity(entity_id id);

        // should make sure that the entity systems have references to don't
        // get cleaned from under them. copy-assign the new entity into the old
        // on that's in _entity_lookup.
        template <class TComponent>
        void add_component(entity& e)
        {
            auto& existing_ref = _entity_lookup.find(e.id())->second;
            auto new_entity = _entity_factory.add_component<TComponent>(e);
            existing_ref = new_entity;
        }

        /**
         * We'll probably want to iterate all entities often. but we don't want to
         * create and return an array of them, and we don't really wait to return
         * _entity_lookup. Two options I see is a separate std::vector<entity&> that
         * we can return, or this - a ForEach(x => { ... }) kind of approach.
         *
         * @param callback will be called for each entity
         */
        void for_all_entities(std::function<void(entity&)> callback);
        /*
         * TODO:
         *  Come up with a decent way to query entities based on components. You can iterate through all of them
         *  and test for present components in the client code, but it would be cleaner to have some sort of a
         *  structure that you can iterate over.
         */

    private:
        entity_factory &_entity_factory;
        events::event_exchange& _events;

        std::unordered_map<entity_id, entity> _entity_lookup;
        static std::atomic_uint next_entity_id;
    };
}

#endif //__ENTITY_WORLD_H_
