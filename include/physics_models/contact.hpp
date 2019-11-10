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

        explicit contact(float t, glm::vec3 collision_axis) : _time(t), _collision_axis(collision_axis) {}
        explicit contact(glm::vec3 penetration) : _collision_axis(penetration), _time(Intersecting)  {}

        float time() const { return _time; }
        const glm::vec3& collision_axis() const { return _collision_axis; }

        inline float decrement_time(float frame_time)
        {
            return frame_time - _time;
        }

    private:
        float _time;

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
