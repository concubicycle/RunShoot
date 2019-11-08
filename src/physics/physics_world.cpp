//
// Created by sava on 10/28/19.
//

#include <physics/physics_world.hpp>
#include <physics/collider_iterator.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/epsilon.hpp>

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

    // apply gravity only once
    for(auto& e : _physical_entities)
    {
        integrate(e, frame_time);
    }

    // detect collisions...
    auto n = _collision_entities.size();
    for(int i=0; i<n; ++i)
        for(int j=i+1; j<n; ++j)
            _contacts.emplace_back(
                _collision_entities[i].get(),
                _collision_entities[j].get(),
                _collisions.check_collision_and_generate_contact(
                    _collision_entities[i],
                    _collision_entities[j],
                    frame_time));

    resolve_collisions(frame_time);

    for(auto& e : _physical_entities)
    {
        auto& rb = e.get().get_component<ecs::rigid_body_component>();

        if (e.get().has<ecs::transform_component>())
        {
            auto& t = e.get().get_component<ecs::transform_component>();
            t.pos = rb.position;
        }
    }
}

void physics::physics_world::resolve_collisions(float frame_time)
{
    while (_contacts.size() > 0 && frame_time >= 0)
    {
        auto first_col = std::min_element(_contacts.begin(), _contacts.end(), entity_contact::compare);
        auto& collision = *first_col;

        if (first_col == _contacts.end() || first_col->contact().time() > 1.f) // no more collisions this frame
        {
            for (auto &e : _physical_entities)
            {
                integrate_position(e, frame_time);
            }
            _contacts.clear(); // discard the rest if any
            return;
        }

        float t = first_col->contact().time();

        if (t == physics_models::contact::Intersecting)
            resolve_collision_discrete(first_col);
        else
            resolve_collision_continuous(frame_time, first_col);

        frame_time -= t;
    }
}


void physics::physics_world::integrate(ecs::entity &e, float frame_time)
{
    auto& phys_props = e.get_component<ecs::rigid_body_component>();

    if (phys_props.is_kinematic) return;

    phys_props.force += phys_props.gravity / 100.f;
    phys_props.previous_position = phys_props.position;
    phys_props.acceleration += phys_props.mass_inverse() * phys_props.force;
    phys_props.force = glm::vec3(0);
    phys_props.velocity += phys_props.acceleration * frame_time; // not accurate
}

void physics::physics_world::integrate_position(ecs::entity &e, float frame_time)
{
    auto& phys_props = e.get_component<ecs::rigid_body_component>();

    if (phys_props.is_kinematic) return;

    phys_props.position += phys_props.velocity * frame_time;
    update_collider_positions(e);
}


void physics::physics_world::grab_entity(ecs::entity &e)
{
    auto has_colliders = e.has<ecs::aabb_collider_component>() ||
        e.has<ecs::sphere_collider_component>();

    auto is_physical = e.has<ecs::rigid_body_component>();

    if (has_colliders)
        _collision_entities.push_back(e);

    if (is_physical)
    {
        auto& t = e.get_component<ecs::transform_component>();
        auto& rb = e.get_component<ecs::rigid_body_component>();
        rb.position = t.pos;

        // update collider positions/rotations(later)
        collider_iterator it(e);
        physics_models::collider* cursor;

        while (it.end() != (cursor = it.get_next()))
            cursor->set_position(t.pos);

        _physical_entities.push_back(e);
    }
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

void physics::physics_world::resolve_collision_continuous(
    float frame_time,
    std::vector<physics::entity_contact>::iterator first_col)
{
    float t_col = first_col->contact().time() - ContinuousCollisionResolutionBias;

    // move everything up to time of collision minus epsilon
    for(auto& e : _physical_entities)
    {
        integrate_position(e, t_col * frame_time);
    }

    frame_time -= t_col;

    // kill velocity in direction of each other.
    resolve_velocity(*first_col, first_col->one());
    resolve_velocity(*first_col, first_col->two());

    _contacts.erase(first_col);

    auto id1 = first_col->one().id();
    auto id2 = first_col->two().id();

    // re-run collisions for collided entities - new collisions may occur
    for (auto& it : _collision_entities)
    {
        if (it.get().id() != id1)
        {
            _contacts.emplace_back(
                it,
                first_col->one(),
                _collisions.check_collision_and_generate_contact(it, first_col->one(), frame_time));
        }

        if (it.get().id() != id2)
        {
            _contacts.emplace_back(
                it,
                first_col->two(),
                _collisions.check_collision_and_generate_contact(it, first_col->two(), frame_time));
        }
    }
}

void physics::physics_world::resolve_velocity(const physics::entity_contact &collision, ecs::entity& e)
{
    if (!e.has<ecs::rigid_body_component>()) return;

    auto& rb1 = e.get_component<ecs::rigid_body_component>();
    auto axis = collision.contact().collision_axis();
    auto n = glm::normalize(axis);

    // is this good math? project the velocity of each onto the face of the polygon
    rb1.velocity -= n * glm::dot(n, rb1.velocity);

    // zero out force, just don't mess with the physics for collided pairs anymore
    rb1.force.x = rb1.force.y = rb1.force.z = 0.f;
}

void
physics::physics_world::resolve_collision_discrete(std::vector<physics::entity_contact>::iterator first_col)
{
    bool one_has_rb = first_col->one().has<ecs::rigid_body_component>();
    bool two_has_rb = first_col->two().has<ecs::rigid_body_component>();

    if ( (one_has_rb && two_has_rb) || !(one_has_rb || two_has_rb) )
    {
        auto& rb0 = first_col->one().get_component<ecs::rigid_body_component>();
        auto& rb1 = first_col->two().get_component<ecs::rigid_body_component>();
        auto c0_move = first_col->contact().collision_axis() / 2.f;
        auto c1_move = first_col->contact().collision_axis() / -2.f;
        rb0.position += c0_move;
        rb1.position += c1_move;
        resolve_velocity(*first_col, first_col->one());
        resolve_velocity(*first_col, first_col->two());
        update_collider_positions(first_col->one());
        update_collider_positions(first_col->two());
    }
    else if (one_has_rb)
    {
        auto& rb0 = first_col->one().get_component<ecs::rigid_body_component>();
        auto c0_move = first_col->contact().collision_axis();
        rb0.position += c0_move;
        resolve_velocity(*first_col, first_col->one());
        update_collider_positions(first_col->one());
    }
    else
    {
        auto& rb1 = first_col->two().get_component<ecs::rigid_body_component>();
        auto c1_move = first_col->contact().collision_axis();
        rb1.position += c1_move;
        resolve_velocity(*first_col, first_col->two());
        update_collider_positions(first_col->two());
    }

    _contacts.erase(first_col);
}

void physics::physics_world::update_collider_positions(ecs::entity &e)
{
    collider_iterator it(e);
    auto& rb = e.get_component<ecs::rigid_body_component>();
    physics_models::collider* cursor;
    while (it.end() != (cursor = it.get_next()))
        cursor->set_position(rb.position);
}

