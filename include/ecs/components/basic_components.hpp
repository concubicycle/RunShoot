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
#include "camera_property.hpp"


#define MAX_BASIC_COMPONENTS 16
#define MAX_RENDER_MESHES 32
#define NUM_CAM_PROPS 8

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

        void set_from_mat4(glm::mat4& mat)
        {
            pos = mat[3];
            glm::mat3 rot(mat);
            float r31 = rot[0][2];
            float r32 = rot[1][2];
            float r33 = rot[2][2];
            float r21 = rot[0][1];
            float r11 = rot[0][0];
            float r12 = rot[1][0];
            float r13 = rot[2][0];

            if (r31 != 1 && r31 != -1)
            {
                float yaw1 = -std::asin(r31);
                float yaw2 = glm::pi<float>() - yaw1;
                float cosyaw1 = std::cos(yaw1);
                float cosyaw2 = std::cos(yaw2);
                float pitch1 = std::atan2(r32 / cosyaw1, r33 / cosyaw1);
                float pitch2 = std::atan2(r32 / cosyaw2, r33 / cosyaw2);
                float roll1 = std::atan2(r21 / cosyaw1, r11 / cosyaw1);
                float roll2 = std::atan2(r21 / cosyaw2, r11 / cosyaw2);
                pitch = pitch1;
                yaw = yaw1;
                roll = roll1;
            }
            else
            {
                roll = 0;
                if (r31 == -1)
                {
                    yaw = glm::pi<float>() / 2.f;
                    pitch = roll + std::atan2(r12, r13);
                } else
                {
                    yaw = glm::pi<float>() / -2.f;
                    pitch = roll + std::atan2(-r12, -r13);
                }
            }
        }
    };

    ////////////////// Actual OpenGL render component //////////////////
    struct render_component_ogl : public component<render_component_ogl>
    {
        mesh_opengl meshes[MAX_RENDER_MESHES];
        std::optional<glm::vec3> hue;
        std::optional<std::string> shader;
        std::uint32_t mesh_count;
        std::uint64_t mesh_path_hash;
        asset::mesh_type mesh_format;
        bool billboard {false};
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

        camera_property<float> float_props[NUM_CAM_PROPS];
        std::uint8_t float_count {0};

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

        glm::mat4 view()
        {
            float cos_p = std::cos(pitch);
            float sin_p = std::sin(pitch);
            float sin_y = std::sin(yaw);
            float cosy = std::cos(yaw);
            glm::vec3 fwd(sin_y * cos_p, sin_p, -cos_p * cosy);
            glm::vec3 up(-sin_y * sin_p, cos_p, sin_p * cosy);

            auto cam_basis = right_up_fwd();
            return glm::lookAt(position, position + fwd, up);
        }

        glm::vec3 fwd()
        {
            float cos_p = std::cos(pitch);
            float sin_p = std::sin(pitch);
            float sin_y = std::sin(yaw);
            float cosy = std::cos(yaw);
            return glm::vec3(sin_y * cos_p, sin_p, -cos_p * cosy);
        }

        // TODO: redesign, templatize
        void set_float(std::string name, float val)
        {
            for (int i = 0; i < float_count; ++i)
            {
                if (float_props[i].name == name)
                {
                    float_props[i].data = val;
                    return;
                }
            }

            float_props[float_count].name = name;
            float_props[float_count++].data = val;
        }

        float get_float(std::string name)
        {
            for (int i = 0; i < float_count; ++i)
                if (float_props[i].name == name)
                    return float_props[i].data;

            return 0;
        }
    };


    /////////////////////// Punctual Light component ///////////////////////
    struct punctual_light_component : public component<punctual_light_component>
    {
        glm::vec3 color = glm::vec3(1.f);
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
