//
// Created by sava on 11/5/19.
//

#ifndef __COLLIDER_HPP_
#define __COLLIDER_HPP_

#include "contact.hpp"
#include "sphere.hpp"

namespace physics_models
{
    class sphere_collider;
    class aabb_collider;

    class shape_visitor
    {
    public:
        virtual contact visit(aabb_collider& collider) = 0;
        virtual contact visit(sphere_collider& collider) = 0;
    };

    class shape_element
    {
    public:
        virtual contact accept(shape_visitor &visitor) = 0;
    };

    class collider : public shape_element, public shape_visitor
    {};


    class aabb_collider : public collider
    {
    public:
        aabb_collider() {}
        aabb_collider(aabb shape) : _shape(shape) {}

        contact visit(aabb_collider& aabb)
        {
            return contact();
        }

        contact visit(sphere_collider& aabb)
        {
            return contact();
        }

        contact accept(shape_visitor &visitor)
        {
            return visitor.visit(*this);
        }

    private:
        aabb _shape;
    };

    class sphere_collider : public collider
    {
    public:
        sphere_collider() {}
        sphere_collider(sphere shape) : _shape(shape) {}

        contact visit(aabb_collider& aabb)
        {
            return contact();
        }

        contact visit(sphere_collider& sphere)
        {
            return contact();
        }

        contact accept(shape_visitor& visitor)
        {
            return visitor.visit(*this);
        }

    private:
        sphere _shape;
    };

}

#endif //__COLLIDER_HPP_
