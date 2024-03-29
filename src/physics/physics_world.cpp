//
// Created by sava on 10/28/19.
//

#include <physics/physics_world.hpp>
#include <physics/collider_iterator.hpp>
#include <spdlog/spdlog.h>

physics::physics_world::physics_world(events::event_exchange &events,
                                      physics::collisions &collisions) :
    _events(events),
    _collisions(collisions)
{
    auto grab_cam_fn = std::function([this](ecs::entity &e) { grab_entity(e); });
    auto forget_cam_fn = std::function([this](ecs::entity &e) { forget_entity(e); });
    _entity_create_listener_id = _events.subscribe<ecs::entity &>(events::entity_created, grab_cam_fn);
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

    for (auto &e : _collision_entities)
        sync_to_transform(e);

    for (auto &e : _physical_entities)
    {
        auto &rb = e.get().get_component<ecs::rigid_body_component>();
        rb.force += rb.mass() * rb.gravity;
        integrate(e, frame_time);
    }

    detect_collisions();
    resolve_collisions(frame_time);
    update_transforms();
}

void physics::physics_world::detect_collisions()
{
    auto n = _collision_entities.size();
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = i + 1; j < n; ++j)
        {
            auto &e1 = _collision_entities[i].get();
            auto &e2 = _collision_entities[j].get();
            _contacts.emplace_back(e1, e2, _collisions.check_collision_and_generate_contact(e1, e2));
        }
    }

    _contacts.erase(std::remove_if(_contacts.begin(), _contacts.end(),
        [](entity_contact &c) { return c.is_no_collision();}), _contacts.end());
}

void physics::physics_world::resolve_collisions(float frame_time)
{
    while (!_contacts.empty() && frame_time >= 0)
    {
        auto first_col = std::min_element(_contacts.begin(), _contacts.end(), entity_contact::compare);
        auto collision = *first_col;

        if (first_col == _contacts.end() || first_col->contact().time() > frame_time) // no more collisions this frame
        {
            for (auto &e : _physical_entities)
                integrate_position(e, frame_time);

            _contacts.clear();
            return;
        }

        float dt = first_col->contact().time();
        bool is_trigger_collision = first_col->contact().is_trigger_contact();

        if (is_trigger_collision)
            resolve_collision_trigger(first_col);
        else if (dt == physics_models::contact::Intersecting)
            resolve_collision_discrete(first_col);
        else
            resolve_collision_continuous(first_col);

        _events.invoke<const entity_contact &, float>(events::collision, collision, dt);

        frame_time -= dt;
    }

    if (frame_time > 0)
        for (auto &e : _physical_entities)
            integrate_position(e, frame_time);
}


void physics::physics_world::integrate(ecs::entity &e, float frame_time)
{
    auto &rb = e.get_component<ecs::rigid_body_component>();
    rb.previous_position = rb.position;
    rb.acceleration += rb.mass_inverse() * rb.force;
    rb.velocity += rb.acceleration * frame_time; // not accurate
    rb.force = rb.acceleration = glm::vec3(0);
}

void physics::physics_world::integrate_position(ecs::entity &e, float frame_time)
{
    auto &rb = e.get_component<ecs::rigid_body_component>();

    if (rb.is_kinematic) return;

    rb.position += rb.velocity * frame_time;
    update_collider_positions(e);
}

void physics::physics_world::grab_entity(ecs::entity &e)
{
    auto has_colliders = e.has<ecs::aabb_collider_component>() ||
                         e.has<ecs::sphere_collider_component>();

    auto is_physical = e.has<ecs::rigid_body_component>();

    auto transform = e.graph_node->absolute_transform();

    if (has_colliders)
        _collision_entities.emplace_back(e);

    if (is_physical)
    {
        auto &rb = e.get_component<ecs::rigid_body_component>();
        rb.position = transform[3];

        // update collider positions/rotations(later)
        collider_iterator it(e);
        physics_models::collider *cursor;

        while (it.end() != (cursor = it.get_next()))
            cursor->set_transform(transform);

        _physical_entities.emplace_back(e);
    } else
    {
        collider_iterator it(e);
        physics_models::collider *cursor;
        while (it.end() != (cursor = it.get_next()))
            cursor->set_transform(transform);
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

    _contacts.erase(std::remove_if(
        _contacts.begin(),
        _contacts.end(),
        [&e](entity_contact &c) {
            return c.one().id() == e.id() || c.two().id() == e.id();}), _contacts.end());
}

void physics::physics_world::resolve_collision_continuous(std::vector<physics::entity_contact>::iterator first_col)
{
    auto &e1 = first_col->one();
    auto &e2 = first_col->two();
    float dt = first_col->contact().time() - ContinuousCollisionResolutionBias;

    // move everything up to time of collision. add some bias for collided entities.
    for (auto &e : _physical_entities)
        integrate_position(e, dt);

    for (auto &c : _contacts)
        c.decrement_time(dt);

    // kill velocity in direction of each other.
    resolve_velocity(*first_col, first_col->one());
    resolve_velocity(*first_col, first_col->two());

    _contacts.erase(first_col);

    // re-run collisions for collided entities - new collisions may occur
    for (auto &it : _collision_entities)
    {
        if (it.get().id() != e1.id())
            _contacts.emplace_back(it, e1, _collisions.check_collision_and_generate_contact(it, e1));

        if (it.get().id() != e2.id())
            _contacts.emplace_back(it, e2, _collisions.check_collision_and_generate_contact(it, e2));
    }
}

void physics::physics_world::resolve_velocity(const physics::entity_contact &collision, ecs::entity &e)
{
    if (!e.has<ecs::rigid_body_component>()) return;

    auto &rb1 = e.get_component<ecs::rigid_body_component>();
    if (rb1.is_kinematic) return;

    auto axis = collision.collision_axis();
    auto n = glm::normalize(axis);

    if (glm::all(glm::isnan(n)))
    {
        spdlog::warn("NaN in resolve_velocity() for the collision normal");
        return;
    }

    // is this good math? project the velocity of each onto the face of the polygon
    // later: the math is fine, the approach is wrong. you can't just kill both velocities -
    // you have to do this in terms of forces.
    rb1.velocity -= n * glm::dot(n, rb1.velocity);

    // zero out force, just don't mess with the physics for collided pairs anymore
    rb1.force.x = rb1.force.y = rb1.force.z = 0.f;
}

void
physics::physics_world::resolve_collision_discrete(std::vector<physics::entity_contact>::iterator first_col)
{
    auto rb0 = first_col->one().get_component_opt<ecs::rigid_body_component>();
    auto rb1 = first_col->two().get_component_opt<ecs::rigid_body_component>();
    auto rb0_physical = rb0 && !(rb0->get().is_kinematic);
    auto rb1_physical = rb1 && !(rb1->get().is_kinematic);

    if (!(rb0_physical || rb1_physical))
    {
        _contacts.erase(first_col);
        return;
    }

    if (rb0_physical && rb1_physical)
    {
        auto c0_move = first_col->contact().collision_axis() / 2.0001f;
        auto c1_move = first_col->contact().collision_axis() / -2.0001f;
        rb0->get().position += c0_move;
        rb1->get().position += c1_move;
        resolve_velocity(*first_col, first_col->one());
        resolve_velocity(*first_col, first_col->two());
        update_collider_positions(first_col->one());
        update_collider_positions(first_col->two());
    } else if (rb0_physical)
    {
        auto c0_move = first_col->contact().collision_axis() * -1.01f;
        rb0->get().position += c0_move;
        resolve_velocity(*first_col, first_col->one());
        update_collider_positions(first_col->one());
    } else
    {
        auto c1_move = first_col->contact().collision_axis() * -1.01f;
        rb1->get().position += c1_move;
        resolve_velocity(*first_col, first_col->two());
        update_collider_positions(first_col->two());
    }

    _contacts.erase(first_col);
}

void physics::physics_world::resolve_collision_trigger(std::vector<physics::entity_contact>::iterator first_col)
{
    float dt = first_col->contact().time();

    // move everything up to time of collision. add some bias for collided entities.
    for (auto &e : _physical_entities)
        integrate_position(e, dt);

    for (auto &c : _contacts)
        c.decrement_time(dt);

    _contacts.erase(first_col);
}

void physics::physics_world::update_collider_positions(ecs::entity &e)
{
    collider_iterator it(e);
    auto &rb = e.get_component<ecs::rigid_body_component>();

    physics_models::collider *cursor;
    while (it.end() != (cursor = it.get_next()))
        cursor->set_position(rb.position);
}

void physics::physics_world::update_transforms()
{
    for (auto &e : _physical_entities)
    {
        if (!e.get().has<ecs::transform_component>()) continue;

        auto &t = e.get().get_component<ecs::transform_component>();
        auto &rb = e.get().get_component<ecs::rigid_body_component>();

        if (rb.is_kinematic)
            rb.position = t.pos;
        else
            t.pos = rb.position;
    }
}

void physics::physics_world::sync_to_transform(ecs::entity &e)
{
    auto rb_opt = e.get_component_opt<ecs::rigid_body_component>();

    auto transform = e.graph_node->absolute_transform();

    if (rb_opt)
        rb_opt->get().position = transform[3];

    collider_iterator it(e);
    physics_models::collider *cursor;
    while (it.end() != (cursor = it.get_next()))
        cursor->set_transform(transform);
}

void physics::physics_world::raycast(
    physics_models::ray ray,
    const std::function<void(ecs::entity &)>& callback)
{
    for (auto &e : _collision_entities)
    {
        collider_iterator it(e);
        physics_models::collider *cursor;
        while (it.end() != (cursor = it.get_next()))
        {
            auto t = cursor->intersect_ray(ray);
            if (t) callback(e);
        }
    }
}

void physics::physics_world::raycast(
    physics_models::ray ray,
    const std::function<void(ecs::entity &)>& callback,
    component_bitset archetype)
{
    for (auto &e : _collision_entities)
    {
        if ((e.get().archetype_id() & archetype) == 0) continue;

        collider_iterator it(e);
        physics_models::collider *cursor;
        while (it.end() != (cursor = it.get_next()))
        {
            auto t = cursor->intersect_ray(ray);
            if (t) callback(e);
        }
    }
}

