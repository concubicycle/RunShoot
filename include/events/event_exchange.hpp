//
// Created by sava on 10/20/19.
//

#ifndef __EVENT_EXCHANGE_HPP_
#define __EVENT_EXCHANGE_HPP_

#include <events/event.hpp>
#include <events/event_type.hpp>
#include <tuple>
#include <physics/entity_contact.hpp>


namespace events
{
    typedef event<ecs::entity&> entity_event;
    typedef event<const physics::entity_contact&, float> collision_event;

    typedef std::tuple<
        std::unordered_map<event_type, entity_event>,
        std::unordered_map<event_type, collision_event>> event_maps;

    class event_exchange
    {
    public:

        template <class... TArgs>
        listener_id subscribe(event_type type, std::function<void(TArgs...)> f)
        {
            auto& e = find_event<TArgs...>(type);
            return e.add_listener(f);
        }

        void unsubscribe(event_type type, listener_id id)
        {
            unsubscribe_all(_event_maps, type, id);
        }

        void update(float_seconds dt)
        {
            update_all(_event_maps, dt);
        }

        template <class... TArgs>
        void invoke(event_type type, TArgs... args)
        {
            auto& e = find_event<TArgs...>(type);
            e.invoke(args...);
        }

        template <typename TEvent, class... TArgs>
        void invoke(TEvent type, TArgs... args)
        {
            auto& e = find_event<TArgs...>(static_cast<event_type>(type));
            e.invoke(args...);
        }

        template <class... TArgs>
        void invoke_delayed(event_type type, float_seconds delay, TArgs... args)
        {
            auto& e = find_event<TArgs...>(type);
            e.invoke_delayed(delay, args...);
        }

    private:
        event_maps _event_maps;

        template <class... TArgs>
        event<TArgs...>& find_event(event_type type)
        {
            auto& events = std::get<std::unordered_map<event_type, event<TArgs...>>>(_event_maps);
            auto it = events.find(type);
            if (it == events.end())
                events.insert(std::pair<event_type, event<TArgs...>>(type, event<TArgs...>()));

            it = events.find(type);
            return it->second;
        }

        template<int index, typename... Ts>
        struct unsubscribe_all_recurse {
            void operator() (event_maps& tuple, event_type type, listener_id id)
            {
                auto& e_map = std::get<index>(tuple);
                auto it_type = e_map.find(type);
                if (it_type != e_map.end())
                {
                    it_type->second.remove_listener(id);
                    return;
                }
                unsubscribe_all_recurse<index - 1, Ts...>{}(tuple, type, id);
            }
        };

        template<typename... Ts>
        struct unsubscribe_all_recurse<0, Ts...> {
            void operator() (event_maps& tuple, event_type type, listener_id id) {
                auto& e_map = std::get<0>(tuple);
                auto it_type = e_map.find(type);
                if (it_type != e_map.end())
                    it_type->second.remove_listener(id);
            }
        };

        template<typename... Ts>
        void unsubscribe_all(event_maps& tuple, event_type type, listener_id id) {
            const auto size = std::tuple_size<event_maps>::value;
            unsubscribe_all_recurse<size - 1, Ts...>{}(tuple, type, id);
        }

        template<int index, typename... Ts>
        struct update_all_recurse {
            void operator() (event_maps& tuple, float_seconds dt)
            {
                auto& e_map = std::get<index>(tuple);
                for (auto& pair : e_map)
                    pair.second.update(dt);

                update_all_recurse<index - 1, Ts...>{}(tuple, dt);
            }
        };

        template<typename... Ts>
        struct update_all_recurse<0, Ts...> {
            void operator() (event_maps& tuple, float_seconds dt) {
                auto& e_map = std::get<0>(tuple);
                for (auto& pair : e_map)
                    pair.second.update(dt);

            }
        };

        template<typename... Ts>
        void update_all(event_maps& tuple, float_seconds dt) {
            const auto size = std::tuple_size<event_maps>::value;
            update_all_recurse<size - 1, Ts...>{}(tuple, dt);
        }
    };
}
#endif //__EVENT_EXCHANGE_HPP_
