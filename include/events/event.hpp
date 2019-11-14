//
// Created by sava on 10/20/19.
//

#ifndef __EVENT_HPP_
#define __EVENT_HPP_

#include <unordered_map>
#include <events/listener.hpp>
#include <atomic>


namespace events
{

    template<class... TArgs>
    class event
    {

    public:
        using listener_type = listener<TArgs...>;

        listener_id add_listener(std::function<void(TArgs...)> f)
        {
            listener_id id = ++next_listener_id;

            _listeners.emplace(std::piecewise_construct,
                std::forward_as_tuple(id),
                std::forward_as_tuple(f, id));

            return id;
        }

        void remove_listener(listener_id id)
        {
            _listeners.erase(id);
        }

        void invoke(TArgs... args)
        {
            if (_listeners.empty()) return;

            for(auto& pair : _listeners)
            {
                listener<TArgs...> l = pair.second;
                l(args...);
            }
        }

    private:
        std::unordered_map<listener_id, listener_type> _listeners = {};



        static std::atomic_uint next_listener_id;
    };

    template<class... TArgs>
    std::atomic_uint event<TArgs...>::next_listener_id(0);


}
#endif //__EVENT_HPP_
