//
// Created by sava on 9/21/19.
//

#ifndef ECS_DEV_BASIC_COMPONENTS_H
#define ECS_DEV_BASIC_COMPONENTS_H

#include <cstring>
#include <vertex_array.h>
#include <cstdint>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <asset/mesh_type.hpp>

#include <physics_models/aabb.hpp>

#include "../ecs_types.hpp"
#include "../component.hpp"
#include "mesh_opengl.hpp"


#define MAX_BASIC_COMPONENTS 16
#define MAX_RENDER_MESHES 32
#define MAX_AABB 64


namespace ecs
{
    /////////////////////// Transform component ///////////////////////
    struct transform_component : public component<transform_component>
    {
        glm::vec3 pos;

        float pitch = 0;
        float yaw = 0;
        float roll = 0;
        float scale_x = 0;
        float scale_y = 0;
        float scale_z = 0;
    };

    ////////////////// Actual OpenGL render component //////////////////
    struct render_component_ogl : public component<render_component_ogl>
    {
        mesh_opengl meshes[MAX_RENDER_MESHES];
        std::uint32_t mesh_count;
        std::uint64_t mesh_path_hash;
        asset::mesh_type mesh_format;
    };

#undef near
#undef far

    ///////////////////////// Camera component /////////////////////////
    struct camera_component : public component<camera_component>
    {
        enum camera_mode
        {
            perspective = 1,
            orthographic = 2
        };

        glm::vec3 position = glm::vec3(0.f);
        camera_mode mode = perspective;
        float pitch = 0, yaw = 0, roll = 0;
        float fov = 45.f, near = 0.1f, far = 1000.f;

        glm::mat3 right_up_fwd()
        {
            float cosp = cos(pitch);
            float sinp = sin(pitch);
            float siny = sin(yaw);
            float cosy = cos(yaw);

            glm::vec3 fwd(siny * cosp, sinp, -cosp * cosy);
            glm::vec3 up(-siny * sinp, cosp, sinp * cosy);
            return glm::mat3(glm::cross(fwd, up), up, fwd);
        }
    };


    /////////////////////// Punctual Light component ///////////////////////
    struct punctual_light_component : public component<punctual_light_component>
    {
        glm::vec3 color = glm::vec3(1.f);
        glm::vec3 position = glm::vec3(0.f);
        float intensity = 100.f;
    };

    ////////////////////////// AABB list component //////////////////////////
    struct aabb_component : public component<aabb_component>
    {
        physics_models::aabb colliders[MAX_AABB];
        uint32_t aabb_count = 0;
    };

    ///////////////////////// Rigid Body component /////////////////////////
    struct rigid_body_component : public component<rigid_body_component>
    {
        glm::vec3 position;
        glm::vec3 previous_position;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec3 force;
        glm::vec3 gravity = glm::vec3(0, -9.8f, 0);

        float mass() { return _mass; }
        float mass_inverse() { return _mass_inverse; }

        void set_mass(float mass)
        {
            _mass = mass;
            _mass_inverse = 1.f / mass;
        }

    private:
        float _mass;
        float _mass_inverse;
    };
} // namespace ecs

#endif //ECS_DEV_BASIC_COMPONENTS_H
