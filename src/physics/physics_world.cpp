//
// Created by sava on 10/28/19.
//

#include <physics/physics_world.hpp>

physics::physics_world::physics_world(events::event_exchange &events) : _events(events)
{
    auto grab_entity = [this](ecs::entity &e) {
        auto has_colliders = e.archetype_id() & ecs::aabb_component::archetype_bit;
        auto is_physical = e.archetype_id() & ecs::aabb_component::archetype_bit &&
                           e.archetype_id() & ecs::physical_properties_component::archetype_bit;

        if (has_colliders)
            _collision_entities.push_back(e);

        if (is_physical)
            _physical_entities.push_back(e);
    };

    auto forget_entity = [this](ecs::entity &e) {
        auto col_it = std::find_if(
            _collision_entities.begin(), _collision_entities.end(),
            [&e](ecs::entity &x) { return x.id() == e.id(); });

        auto phys_it = std::find_if(
            _physical_entities.begin(), _physical_entities.end(),
            [&e](ecs::entity &x) { return x.id() == e.id(); });

        if (col_it != _collision_entities.end()) _collision_entities.erase(col_it);
        if (phys_it != _physical_entities.end()) _physical_entities.erase(phys_it);
    };


    _entity_create_listener_id = _events.subscribe(events::entity_created,
        std::function<void(ecs::entity &)>(grab_entity));

    _entity_destroy_listener_id = _events.subscribe(events::entity_destroyed,
        std::function<void(ecs::entity &)>(forget_entity));
}


physics::physics_world::~physics_world()
{
    _events.unsubscribe(events::entity_created, _entity_create_listener_id);
    _events.unsubscribe(events::entity_destroyed, _entity_destroy_listener_id);
}


void physics::physics_world::update()
{

}

