//
// Created by sava on 9/21/19.
//

#ifndef ECS_DEV_BASIC_COMPONENTS_H
#define ECS_DEV_BASIC_COMPONENTS_H

#include <cstring>


#include "../ecs_types.hpp"
#include "../component.hpp"


namespace ecs
{

    /** This is a dummy component, made to test in place construction. Do not use it for anything. */
    class matrix_component : public component<matrix_component>
    {
    public:
        matrix_component()
        {
            memset(&m, 0, sizeof(float) * 16);
            m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
        }

        float m[4][4];
    };

    class transform_component : public component<transform_component>
    {
    public:
        float x = 0;
        float y = 0;
        float z = 0;
        float pitch = 0;
        float yaw = 0;
        float roll = 0;
    };



    class render_component : public component<render_component>
    {
    public:
        float a;
        float b;
    };


} // namespace ecs

#endif //ECS_DEV_BASIC_COMPONENTS_H
