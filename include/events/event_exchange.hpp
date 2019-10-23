//
// Created by sava on 10/20/19.
//

#ifndef __EVENT_EXCHANGE_HPP_
#define __EVENT_EXCHANGE_HPP_

#include <events/event.hpp>
#include <events/event_type.hpp>


namespace events
{
    class event_exchange
    {
    public:

        template <class... TArgs>
        listener_id subscribe(event_type type, std::function<void(TArgs...)> f)
        {
            auto& e = find_event<TArgs...>(type);
            return e.add_listener(f);
        }

        template <class... TArgs>
        void unsubscribe(event_type type, listener_id id)
        {
            auto& e = find_event<TArgs...>(type);
            e.remove_listener(id);
        }

        template <class... TArgs>
        void invoke(event_type type, TArgs... args)
        {
            auto& e = find_event<TArgs...>(type);
            e.invoke(args...);
        }

    private:

        template <class... TArgs>
        event<TArgs...>& find_event(event_type type)
        {
            static std::unordered_map<event_type, event<TArgs...>> type_to_event;
            auto it = type_to_event.find(type);
            if (it == type_to_event.end())
            {
                type_to_event.insert(std::pair<event_type, event<TArgs...>>(type, event<TArgs...>()));
            }

            it = type_to_event.find(type);
            return it->second;
        }

    };

}
#endif //__EVENT_EXCHANGE_HPP_
