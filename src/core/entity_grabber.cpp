//
// Created by sava on 12/3/19.
//

#include <core/entity_grabber.hpp>

core::entity_grabber::entity_grabber(
    events::event_exchange &events,
    std::function<bool(ecs::entity&)> predicate)
    : _events(events)
    , _predicate(predicate)
{
    auto grab = std::function([this](ecs::entity &e) { grab_entity(e); });
    auto forget = std::function([this](ecs::entity &e) { forget_entity(e); });
    _grab_listener_id = _events.subscribe<ecs::entity &>(events::entity_created, grab);
    _forget_listener_id = _events.subscribe(events::entity_destroyed, forget);
}

core::entity_grabber::~entity_grabber()
{
    _events.unsubscribe(events::entity_created, _grab_listener_id);
    _events.unsubscribe(events::entity_destroyed, _forget_listener_id);
}



void core::entity_grabber::grab_entity(ecs::entity &e)
{
    if (_predicate(e)) _entities.emplace_back(e);
}

void core::entity_grabber::forget_entity(ecs::entity &e)
{
    auto it = std::find_if(
        _entities.begin(), _entities.end(),
        [&e](ecs::entity &x) { return x.id() == e.id(); });

    if (it != _entities.end()) _entities.erase(it);
}

std::optional<std::reference_wrapper<ecs::entity>> core::entity_grabber::get_single()
{
    return _entities.empty() ? std::optional<std::reference_wrapper<ecs::entity>>() : _entities[0];
}
