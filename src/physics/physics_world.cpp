//
// Created by sava on 10/28/19.
//

#include <physics/physics_world.hpp>

physics::physics_world::physics_world(events::event_exchange &events,
                                      physics::collisions &collisions) :
                                      _events(events),
                                      _collisions(collisions)
{
    auto grab_cam_fn = std::function<void(ecs::entity&)>([this](ecs::entity& e){ grab_entity(e); });
    auto forget_cam_fn = std::function<void(ecs::entity&)>([this](ecs::entity& e){ forget_entity(e); });
    _entity_create_listener_id = _events.subscribe<ecs::entity&>(events::entity_created, grab_cam_fn);
    _entity_destroy_listener_id = _events.subscribe(events::entity_destroyed, forget_cam_fn);

    _contacts.reserve(NumContactsReserved);
    _collision_entities.reserve(NumEntitiesReserved);
    _physical_entities.reserve(NumEntitiesReserved);
}


physics::physics_world::~physics_world()
{
    _events.unsubscribe(events::entity_created, _entity_create_listener_id);
    _events.unsubscribe(events::entity_destroyed, _entity_destroy_listener_id);
}


void physics::physics_world::update(float frame_time)
{
    _contacts.clear();

    for(auto& e : _physical_entities)
        integrate(e, frame_time);

    // detect collisions...
    for (auto& it1 : _collision_entities)
        for (auto& it2 : _collision_entities)
            _contacts.push_back(_collisions.check_collision_and_generate_contact(it1, it2));

    // resolve collisions...
}

void physics::physics_world::integrate(ecs::entity &e, float frame_time)
{
    auto phys_props = e.get_component<ecs::rigid_body_component>();

    phys_props.previous_position = phys_props.position;
    phys_props.force += phys_props.gravity;
    phys_props.acceleration += phys_props.mass_inverse() * phys_props.force;
    phys_props.force = glm::vec3(0);
    phys_props.position = phys_props.velocity * frame_time + phys_props.position;

    if (e.has<ecs::transform_component>())
    {
        auto t = e.get_component<ecs::transform_component>();
        t.pos = phys_props.position;
    }
}

void physics::physics_world::grab_entity(ecs::entity &e)
{
    auto has_colliders = e.archetype_id() & ecs::aabb_collider_component::archetype_bit;
    auto is_physical = e.archetype_id() & ecs::aabb_collider_component::archetype_bit &&
                       e.archetype_id() & ecs::rigid_body_component::archetype_bit;
    if (has_colliders)
        _collision_entities.push_back(e);

    if (is_physical)
        _physical_entities.push_back(e);
}

void physics::physics_world::forget_entity(ecs::entity &e)
{
    auto col_it = std::find_if(
        _collision_entities.begin(), _collision_entities.end(),
        [&e](ecs::entity &x) { return x.id() == e.id(); });

    auto phys_it = std::find_if(
        _physical_entities.begin(), _physical_entities.end(),
        [&e](ecs::entity &x) { return x.id() == e.id(); });

    if (col_it != _collision_entities.end()) _collision_entities.erase(col_it);
    if (phys_it != _physical_entities.end()) _physical_entities.erase(phys_it);
}
