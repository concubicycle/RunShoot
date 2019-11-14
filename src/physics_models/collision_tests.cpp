//
// Created by sava on 11/6/19.
//

#include <physics_models/collision_tests.hpp>
#include <cstdlib>
#include <glm/ext/scalar_constants.hpp>

#include <iostream>
#include <glm/gtx/string_cast.hpp>


physics_models::contact physics_models::intersect(aabb& c0, aabb& c1, glm::vec3& combined_velocity)
{
    float t_first = contact::Intersecting, t_last = 999999.f;
    float t_max = 1.000001f;
    std::uint32_t collision_axis_index = 0;
    glm::vec3 penetration(0.f);

    for(int i = 0; i < 3; ++i)
    {
        float c0_min = c0.min[i];
        float c0_max = c0.max[i];
        float c1_min = c1.min[i];
        float c1_max = c1.max[i];
        float c1_speed = combined_velocity[i];

        if (c1_max < c0_min) // c1 on left
        {
            if (c1_speed <= 0) return contact::None;
            float t = (c0_min - c1_max) / c1_speed;

            if (t > t_first)
            {
                collision_axis_index = i;
                t_first = t;
            }

            if (t_first > t_max) return contact::None;

            t = (c0_max - c1_min);
            if (t < t_last) t_last = t;
            if (t_first > t_last) return contact::None;
        }
        else if (c0_max < c1_min)
        {
            if (c1_speed >= 0) return contact::None;
            float t = (c0_max - c1_min) / c1_speed;

            if (t > t_first)
            {
                collision_axis_index = i;
                t_first = t;
            }

            if (t_first > t_max) return contact::None;

            t = (c0_min - c1_max) / c1_speed;
            if (t < t_last) t_last = t;
            if (t_first > t_last) return contact::None;
        }
        else
        {
            if (c1_max == c0_min)
                penetration[i] = -ContactNudge;
            else if (c0_max == c1_min)
                penetration[i] = ContactNudge;
            else if (c0_max > c1_max)
                penetration[i] = c0_min - c1_max;
            else
                penetration[i] = c1_min - c0_max;

            if (c1_speed > 0)
            {
                float t = (c0_max - c1_min) / c1_speed;
                if (t < t_last) t_last = t;
                if (t_first > t_last) return contact::None;
            }
            else if (c1_speed < 0)
            {
                float t = (c0_min - c1_max) / c1_speed;
                if (t < t_last) t_last = t;
                if (t_first > t_last) return contact::None;
            }
        }
    }

    bool intersect_at_start =
        penetration[0] != 0 &&
        penetration[1] != 0 &&
        penetration[2] != 0;

    if (intersect_at_start)
    {
        float min_pen = std::abs(penetration[0]);
        int min_pen_i = 0;

        // zero out non-min penetration components
        for (int i = 1; i < 3; ++i)
        {
            auto pen = std::abs(penetration[i]);

            if (pen < min_pen)
            {
                min_pen = pen;
                min_pen_i = i;
            }
        }

        for (int i = 0; i < 3; i++)
            if (i != min_pen_i)
                penetration[i] = 0;

        return contact(penetration);
    }

    glm::vec3 collision_axis(0.f);
    collision_axis[collision_axis_index] = 1.f;

    return contact(t_first, collision_axis);
}

physics_models::contact physics_models::intersect(aabb& c0, sphere& c1, glm::vec3& combined_velocity)
{
    return contact(); // unimplemented
}

physics_models::contact physics_models::intersect(sphere& c0, sphere& c1, glm::vec3& combined_velocity)
{
    return contact(); // unimplemented
}

