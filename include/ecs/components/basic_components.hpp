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

#include "../ecs_types.hpp"
#include "../component.hpp"
#include "mesh_opengl.hpp"


#define MAX_BASIC_COMPONENTS 16
#define MAX_RENDER_MESHES 32


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


    /////////////// Deprecated OpenGL render component ///////////////
    struct render_component_old : public component<render_component_old>
    {
        char mesh_path[256];
        asset::mesh_type mesh_format;
        ogllib::vertex_array vao;
        std::uint32_t gl_texture_id;
        std::uint32_t element_count;
    };

    ////////////////// Actual OpenGL render component //////////////////
    struct render_component_ogl : public component<render_component_ogl>
    {
        mesh_opengl meshes[MAX_RENDER_MESHES];
        std::uint32_t mesh_count;
        std::uint64_t mesh_path_hash;
        asset::mesh_type mesh_format;
    };


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
} // namespace ecs

#endif //ECS_DEV_BASIC_COMPONENTS_H
