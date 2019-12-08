#include <core/scene_tracker.hpp>

core::scene_tracker::scene_tracker(std::string first_scene, events::event_exchange &events) : _events(events)
{
    auto scene_change = std::function([this](const std::string& scene) { on_scene_change(scene); });
    auto exit = std::function([this]() { on_exit(); });
    _scene_change_listener_id = _events.subscribe<const std::string&>(events::scene_change, scene_change);
    _exit_listener_id = _events.subscribe(events::exit, exit);

    _next = first_scene;
}

core::scene_tracker::~scene_tracker()
{
    _events.unsubscribe(events::scene_change, _scene_change_listener_id);
    _events.unsubscribe(events::exit, _exit_listener_id);
}

void core::scene_tracker::on_exit()
{
    _next.reset();
}

void core::scene_tracker::on_scene_change(const std::string &new_scene)
{
    _next = new_scene;
}
