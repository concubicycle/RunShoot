//
// Created by sava on 11/1/19.
//

#ifndef __SPHERE_HPP_
#define __SPHERE_HPP_


namespace physics_models
{
    struct sphere
    {
        sphere() : center(0.f), radius(1) {}

        glm::vec3 center;
        float radius;
    };
}


#endif //__SPHERE_HPP_
