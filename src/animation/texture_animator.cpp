#include <animation/texture_animator.hpp>

animation::texture_animator::texture_animator(events::event_exchange &events) : _events(events)
{
    auto grab_cam_fn = std::function([this](ecs::entity &e) { grab_entity(e); });
    auto forget_cam_fn = std::function([this](ecs::entity &e) { forget_entity(e); });
    _entity_create_listener_id = _events.subscribe<ecs::entity &>(events::entity_created, grab_cam_fn);
    _entity_destroy_listener_id = _events.subscribe<ecs::entity &>(events::entity_destroyed, forget_cam_fn);
}


animation::texture_animator::~texture_animator()
{
    _events.unsubscribe(events::entity_created, _entity_create_listener_id);
    _events.unsubscribe(events::entity_destroyed, _entity_destroy_listener_id);
}


void animation::texture_animator::grab_entity(ecs::entity &e)
{
    if (e.has<ecs::billboard_animation_component>())
        _entities.emplace_back(e);
}

void animation::texture_animator::forget_entity(ecs::entity &e)
{
    auto it = std::find_if(
        _entities.begin(), _entities.end(),
        [&e](ecs::entity &x) { return x.id() == e.id(); });

    if (it != _entities.end()) _entities.erase(it);
}

void animation::texture_animator::update(float dt)
{
    for (auto& e : _entities)
    {
        auto& component = e.get().get_component<ecs::billboard_animation_component>();

        if (component.current_state != component.last_state)
        {
            component.reset_time();
        }

        component.last_state = component.current_state;

        component.increment_time(dt);
    }
}
