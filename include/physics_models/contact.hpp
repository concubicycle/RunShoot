//
// Created by sava on 11/1/19.
//

#ifndef __CONTACT_HPP_
#define __CONTACT_HPP_

#include <glm/vec3.hpp>

namespace physics_models
{
    class contact
    {
    public:
        static const int NoCollision = 99999;
        static const int Intersecting = 0;
        static const contact None;

        contact() : _time(NoCollision) {}

        explicit contact(float t, glm::vec3 collision_axis, bool is_trigger_contact = false) :
            _time(t),
            _collision_axis(collision_axis),
            _is_trigger_contact(is_trigger_contact) {}

        explicit contact(glm::vec3 penetration, bool is_trigger_contact = false) :
            _collision_axis(penetration),
            _time(Intersecting),
            _is_trigger_contact(is_trigger_contact) {}

        [[nodiscard]] float time() const { return _time; }
        [[nodiscard]] const glm::vec3& collision_axis() const { return _collision_axis; }
        [[nodiscard]] bool is_trigger_contact() const { return _is_trigger_contact; }

        void set_is_trigger(bool val) { _is_trigger_contact = val; }

        void decrement_time(float val)
        {
            _time -= val;
        }

    private:
        float _time;
        bool _is_trigger_contact{};

        // either the penetration vector or the collision axis.
        glm::vec3 _collision_axis {};

    };

    struct contact_compare {
        bool operator() (const contact& lhs, const contact& rhs) const {
            return lhs.time() < rhs.time();
        }
        static bool compare(const contact& lhs, const contact& rhs) {
            return lhs.time() < rhs.time();
        }
    };
}

#endif //__CONTACT_HPP_
