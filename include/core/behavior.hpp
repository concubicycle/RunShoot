//
// Created by sava on 10/20/19.
//

#ifndef __BEHAVIOR_HPP_
#define __BEHAVIOR_HPP_

#include <vector>

#include <ecs/ecs_types.hpp>
#include <ecs/entity.hpp>
#include <events/event_exchange.hpp>
#include "behavior_context.hpp"

namespace core
{
    class behavior
    {
    public:
        explicit behavior(events::event_exchange &events) : _events(events)
        {
            std::function<void(ecs::entity&)> on_create =
                std::bind(&behavior::_on_entity_created, this, std::placeholders::_1);

            std::function<void(ecs::entity&)> on_destroy =
                std::bind(&behavior::on_entity_destroyed, this, std::placeholders::_1);

            _listener_id_create = events.subscribe<ecs::entity&>(events::event_type::entity_created, on_create);
            _listener_id_destroy = events.subscribe<ecs::entity&>(events::event_type::entity_destroyed, on_destroy);
        }

        virtual ~behavior()
        {
            _events.unsubscribe(events::event_type::entity_created, _listener_id_create);
            _events.unsubscribe(events::event_type::entity_destroyed, _listener_id_destroy);
        }

        virtual component_bitset required_components() const = 0;


        void update(behavior_context &ctx)
        {
            for (auto& e : _entities)
                update_single(e, ctx);
        }

    protected:
        virtual void update_single(ecs::entity& e, behavior_context &ctx) = 0;
        virtual void on_entity_created(ecs::entity& e) {}
        events::event_exchange& _events;

    private:

        std::vector<std::reference_wrapper<ecs::entity>> _entities;
        listener_id _listener_id_create;
        listener_id _listener_id_destroy;

        void _on_entity_created(ecs::entity& e)
        {
            auto req_comps = required_components();
            auto e_comps = e.archetype_id();
            if ((req_comps & e_comps) == req_comps)
            {
                _entities.emplace_back(e);
                on_entity_created(e);
            }
        }

        void on_entity_destroyed(ecs::entity& e)
        {
            auto it = std::find_if(
                _entities.begin(), _entities.end(),
                [&e](ecs::entity& x) { return x.id() == e.id();});

            if (it != _entities.end())
            {
                _entities.erase(it);
            }
        }
    };
}

#endif //__BEHAVIOR_HPP_
