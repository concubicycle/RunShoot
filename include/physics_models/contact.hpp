//
// Created by sava on 11/1/19.
//

#ifndef __CONTACT_HPP_
#define __CONTACT_HPP_

namespace physics_models
{
    class contact
    {
    public:
        static const unsigned int NoCollision = 99999;
        static const contact None;

        contact() : _time(NoCollision) {}
        explicit contact(float t) : _time(t) {}

        float time() const { return _time; }

        bool did_collide() { return _time <= 1; }

    private:
        float _time;
    };

    struct contact_compare {
        bool operator() (const contact& lhs, const contact& rhs) const {
            return lhs.time() < rhs.time();
        }
    };
}

#endif //__CONTACT_HPP_
