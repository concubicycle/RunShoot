//
// Created by sava on 9/21/19.
//

#ifndef ECS_DEV_BASIC_COMPONENTS_H
#define ECS_DEV_BASIC_COMPONENTS_H

#include <cmath>
#include <cstring>
#include <vertex_array.h>
#include <cstdint>
#include <optional>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <asset/mesh_type.hpp>

#include <physics_models/aabb.hpp>
#include <physics_models/collider.hpp>
#include <physics_models/aabb_collider.hpp>
#include <physics_models/sphere_collider.hpp>

#include <cubemap.hpp>
#include <vectormath.h>
#include <ecs/component.hpp>
#include <ecs/components/mesh_opengl.hpp>



#define MAX_BASIC_COMPONENTS 16
#define MAX_RENDER_MESHES 32

/*
 * Each collider component will have an array of shapes, to allow for
 * more than one of each collider type.
 */
#define MAX_COLLIDER_SHAPES 48



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



        [[nodiscard]] glm::mat4 to_mat4() const
        {
            auto model = glm::mat4(1.f);
            model = glm::scale(model, glm::vec3(scale_x, scale_y, scale_z));
            model = glm::eulerAngleYXZ(yaw, pitch, roll) * model;
            set_translation(model, pos);
            return model;
        }
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

        std::optional<ogllib::cubemap> skybox;
        std::string skybox_left;
        std::string skybox_top;
        std::string skybox_front;
        std::string skybox_bottom;
        std::string skybox_right;
        std::string skybox_back;


        glm::mat3 right_up_fwd()
        {
            float cos_p = std::cos(pitch);
            float sin_p = std::sin(pitch);
            float sin_y = std::sin(yaw);
            float cosy = std::cos(yaw);

            glm::vec3 fwd(sin_y * cos_p, sin_p, -cos_p * cosy);
            glm::vec3 up(-sin_y * sin_p, cos_p, sin_p * cosy);
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

    /////////////////////// AABB Collider List component //////////////////////
    struct aabb_collider_component : public component<aabb_collider_component>
    {
        physics_models::aabb_collider colliders[MAX_COLLIDER_SHAPES];
        uint32_t count = 0;
    };

    ////////////////////// Sphere Collider List component //////////////////////
    struct sphere_collider_component : public component<sphere_collider_component>
    {
        physics_models::sphere_collider colliders[MAX_COLLIDER_SHAPES];
        uint32_t count = 0;
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

        bool is_kinematic;

        float mass() { return _mass; }
        float mass_inverse() { return _mass_inverse; }

        void set_mass(float mass)
        {
            _mass = mass;
            _mass_inverse = mass == 0 ? 0 : 1.f / mass;
        }

    private:
        float _mass;
        float _mass_inverse;
    };
} // namespace ecs

#endif //ECS_DEV_BASIC_COMPONENTS_H
