//
// Created by sava on 11/22/19.
//

#ifndef __DEBUG_DRAWER_HPP_
#define __DEBUG_DRAWER_HPP_

#include <vector>
#include <events/event_exchange.hpp>
#include "shader_set.hpp"


namespace rendering
{

    class debug_drawer
    {
    public:
        std::vector<ogllib::vertex_p> vertices{
            // positions
            {-1.0f, 1.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f},

            {-1.0f, -1.0f, 1.0f},
            {-1.0f, -1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f},
            {-1.0f, 1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},

            {1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},

            {-1.0f, -1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},

            {-1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, -1.0f},

            {-1.0f, -1.0f, -1.0f},
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f}
        };


        debug_drawer(
            const core::startup_config &config,
            const core::system_info &system_info,
            events::event_exchange& events,
            shader_set &shaders,
            core::input_manager& input)
            : _events(events)
            , _shaders(shaders)
            , _config(config)
            , _input(input)
        {
            auto grab_cam_fn = std::function([this](ecs::entity &e) { grab_entity(e); });
            auto forget_cam_fn = std::function([this](ecs::entity &e) { forget_entity(e); });
            _entity_created_listener_id = _events.subscribe<ecs::entity &>(events::entity_created, grab_cam_fn);
            _entity_destroyed_listener_id = _events.subscribe(events::entity_destroyed, forget_cam_fn);

            glGenVertexArrays(1, &_vao);
            glGenBuffers(1, &_vbo);
            glBindVertexArray(_vao);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ogllib::vertex_p), vertices.data(), GL_STATIC_DRAW);

            _shaders.debug().bind();
            _shaders.debug().set_attrib_pointers();

            if (config.fullscreen())
            {
                _screen_width = (float)system_info.monitor_width();
                _screen_height = (float)system_info.monitor_height();
            } else
            {
                _screen_width = (float) config.width();
                _screen_height = (float)config.height();
            }
        }

        ~debug_drawer()
        {
            _events.unsubscribe(events::entity_created, _entity_created_listener_id);
            _events.unsubscribe(events::entity_destroyed, _entity_destroyed_listener_id);
        }

        void update()
        {
            if (_input.was_key_pressed(GLFW_KEY_F1))
                _enabled = !_enabled;

            if (!_enabled) return;

            for (auto &e : _entities)
            {
                auto &aabbs = e.get().get_component<ecs::aabb_collider_component>();

                for (unsigned int i = 0; i < aabbs.count; ++i)
                {
                    auto &min = aabbs.colliders[i].shape().min;
                    auto &max = aabbs.colliders[i].shape().max;

                    float width = max.x - min.x;
                    float height = max.y - min.y;
                    float depth = max.z - min.z;
                    float x_scale = width / 2.f;
                    float y_scale = height / 2.f;
                    float z_scale = depth / 2.f;

                    glm::vec3 center = min + 0.5f * (max - min);
                    auto model = glm::translate(center) * glm::scale(glm::vec3(x_scale, y_scale, z_scale));

                    auto& cam = _camera_ptr->get_component<ecs::camera_component>();

                    auto aspect = _screen_width / _screen_height;
                    auto projection = glm::perspective(cam.fov, aspect, cam.near, cam.far);

                    auto cam_basis = cam.right_up_fwd();
                    auto view = glm::lookAt(cam.position, cam.position + cam_basis[2], cam_basis[1]);


                    glClear(GL_DEPTH_BUFFER_BIT);
                    glDisable(GL_CULL_FACE);
                    glDepthMask(GL_FALSE);
                    glDepthFunc(GL_LEQUAL);

                    _shaders.debug().bind();
                    _shaders.debug().set_uniform("view", view);
                    _shaders.debug().set_uniform("projection", projection);
                    _shaders.debug().set_uniform("model", model);

                    bind();

                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                    unbind();
                    glDepthFunc(GL_LESS);
                    glDepthMask(GL_TRUE);

                    if (_config.backface_culling())
                    {
                        glEnable(GL_CULL_FACE);
                        glFrontFace(GL_CCW);
                    }
                }
            }
        }

        void bind()
        {
            glBindVertexArray(_vao);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        }

        void unbind()
        {
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }


    private:
        std::vector<std::reference_wrapper<ecs::entity>> _entities;
        ecs::entity *_camera_ptr = nullptr;

        events::event_exchange &_events;
        shader_set &_shaders;
        const core::startup_config &_config;


        listener_id _entity_created_listener_id;
        listener_id _entity_destroyed_listener_id;

        float _screen_width, _screen_height;
        bool _enabled {false};

        core::input_manager& _input;

        GLuint _vao{};
        GLuint _vbo{};

        void grab_entity(ecs::entity &e)
        {
            if (e.has<ecs::aabb_collider_component>())
                _entities.emplace_back(e);

            if (e.has<ecs::camera_component>())
                _camera_ptr = &e;
        }

        void forget_entity(ecs::entity &e)
        {
            auto it = std::find_if(
                _entities.begin(), _entities.end(),
                [&e](ecs::entity &x) { return x.id() == e.id(); });

            if (it != _entities.end()) _entities.erase(it);

            if (_camera_ptr != nullptr && _camera_ptr->id() == e.id())
                _camera_ptr = nullptr;
        }
    };

}

#endif //__DEBUG_DRAWER_HPP_
