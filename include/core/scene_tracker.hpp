//
// Created by sava on 12/8/19.
//

#ifndef __SCENE_TRACKER_HPP_
#define __SCENE_TRACKER_HPP_

#include <optional>
#include <string>
#include <events/event_exchange.hpp>

namespace core
{

    class scene_tracker
    {
    public:
        explicit scene_tracker(std::string first_scene, events::event_exchange &events);

        ~scene_tracker();

        bool has_next() const { return _next.has_value(); }

        std::string next()
        {
            auto str = *_next;
            _next.reset();
            return str;
        }


    private:
        std::optional<std::string> _next;

        events::event_exchange &_events;
        listener_id _scene_change_listener_id;
        listener_id _exit_listener_id;

        void on_exit();

        void on_scene_change(const std::string &new_scene);
    };


}
#endif //__SCENE_TRACKER_HPP_
