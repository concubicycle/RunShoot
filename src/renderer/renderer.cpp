#include <cstdlib>

#include "renderer/renderer.hpp"
#include <asset/scene.hpp>

#include <glbinding/gl/gl.h>
#include <glm/gtx/euler_angles.hpp>
#include <utility>

#include <vectormath.h>

using namespace gl;

void
MessageCallback(GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar *message,
                const void *userParam)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            spdlog::info("GL Callback: {0}", message);
            return;
        default:
            spdlog::error("GL callback. type = {0}, severity = {1}, message = {2}", type, severity, message);
    }
}


rendering::renderer::renderer(
    const core::startup_config &config,
    const core::system_info &system_info,
    const shader_set &shaders,
    events::event_exchange &events) :
    _config(config),
    _system_info(system_info),
    _shaders(shaders),
    _events(events),
    _camera_entity(nullptr),
    _overlay(shaders)
{
    auto grab_cam_fn = std::function<void(ecs::entity &)>([this](ecs::entity &e) { grab_entity(e); });
    auto forget_cam_fn = std::function<void(ecs::entity &)>([this](ecs::entity &e) { forget_entity(e); });
    _cam_listener_id = _events.subscribe<ecs::entity &>(events::entity_created, grab_cam_fn);
    _cam_remove_listener_id = _events.subscribe(events::entity_destroyed, forget_cam_fn);

    if (_config.fullscreen())
    {
        _screen_width = (float)_system_info.monitor_width();
        _screen_height = (float)_system_info.monitor_height();
    } else
    {
        _screen_width = (float) _config.width();
        _screen_height = (float)_config.height();
    }

    std::function<void(std::string name, float prop)> cam_prop_set([this](std::string name, float prop){
        if (_camera_entity == nullptr) return;

        auto& cam = _camera_entity->get_component<ecs::camera_component>();
        cam.set_float(std::move(name), prop);
    });

    _cam_float_set_id = _events.subscribe<std::string, float>(events::camera_prop_set, cam_prop_set);

    _lights.reserve(MinLightReferences);
}

rendering::renderer::~renderer()
{
    _events.unsubscribe(events::entity_created, _cam_listener_id);
    _events.unsubscribe(events::entity_destroyed, _cam_remove_listener_id);
    _events.unsubscribe(events::camera_prop_set, _cam_float_set_id);
}


bool rendering::renderer::init()
{
    if (!_config.fullscreen())
        glViewport(0, 0, _config.width(), _config.height());
    else
        glViewport(0, 0, _system_info.monitor_width(), _system_info.monitor_height());

    gl::glClearColor(0.2f, 0.1f, 0.3f, 0.f);

    if (_config.backface_culling())
    {
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
    }

    glEnable(GL_DEPTH_TEST);

    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}


void rendering::renderer::draw_scene(asset::scene &scene)
{
    if (_camera_entity == nullptr)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto &entities = scene.entity_world();

    draw_skybox();

    scene.scene_graph().traverse([this](ecs::entity &e, glm::mat4& model) {
        if (!e.active()) return;

        auto is_rt = e.has<ecs::render_component_ogl>() && e.has<ecs::transform_component>();
        if (!is_rt) return;

        auto &cam = _camera_entity->get_component<ecs::camera_component>();
        auto &r = e.get_component<ecs::render_component_ogl>();
        auto &t = e.get_component<ecs::transform_component>();

        auto model_inverse = glm::inverse(model);

        auto aspect = _screen_width / _screen_height;
        auto projection = glm::perspective(cam.fov, aspect, cam.near, cam.far);

        auto view = cam.view();

        if (r.billboard)
        {
            glm::vec3 a(0.f, 0.f, 1.f);
            glm::vec3 a_p = glm::normalize(cam.position - glm::vec3(model[3]));
            float theta = acos(glm::dot(a, a_p));
            glm::vec3 axis = glm::cross(a, a_p);
            glm::mat4 rotation = glm::rotate(theta, axis);
            auto pos = model[3];
            model = rotation;
            set_translation(model, pos);
        }

        auto light_pos = glm::vec3(0, 10.f, 10.f);
        auto light_color = glm::vec3(0.5, 0.5, 0.5);
        auto ambient_light = glm::vec3(0.3, 0.31, 0.38);
        auto specular = glm::vec3(0.01f);

        if (r.mesh_format == asset::mesh_type::GLTF2)
        {
            auto& shader = _shaders.default_shader();
            shader.bind();

            shader.set_uniform("model", model);
            shader.set_uniform("view", view);
            shader.set_uniform("projection", projection);
            shader.set_uniform("model_inverse", model_inverse);
            shader.set_uniform("view_pos", cam.position);
            shader.set_uniform("ambient_light", ambient_light);
            shader.set_uniform("specular", specular);
            shader.set_uniform("shininess", 0.2f);

            set_light_uniforms(shader);

            for (int i = 0; i < cam.float_count; ++i)
                shader.set_uniform(cam.float_props[i].name, cam.float_props[i].data);

            for (std::uint32_t i = 0; i < r.mesh_count; ++i)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, r.meshes[i].diffuse_texture_id);

                glBindVertexArray(r.meshes[i].vao);
                glDrawElements(GL_TRIANGLES, r.meshes[i].element_count, GL_UNSIGNED_INT, nullptr);
            }

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
            shader.unbind();
        } else
        {
            const ogllib::shader_program_base& shader = r.shader
                ? _shaders.get_program(*(r.shader))
                : _shaders.get_program("ptx2d_pvm");

            auto projection_view_model = projection * view * model;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, r.meshes[0].diffuse_texture_id);

            shader.bind();

            shader.set_uniform("projection_view_model", projection_view_model);

            if (r.hue) shader.set_uniform("hue", *r.hue);

            glBindVertexArray(r.meshes[0].vao);
            glDrawElements(GL_TRIANGLES, r.meshes[0].element_count, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);

            shader.unbind();
        }
    });

    _overlay.draw(_screen_width, _screen_height);
}

void rendering::renderer::resize(std::uint32_t width, std::uint32_t height)
{
    glViewport(0, 0, width, height);
}

void rendering::renderer::grab_entity(ecs::entity &e)
{
    if (e.archetype_id() & ecs::camera_component::archetype_bit)
    {
        _camera_entity = &e;

        auto& cam = e.get_component<ecs::camera_component>();
        cam.skybox.emplace(
            cam.skybox_left,
            cam.skybox_top,
            cam.skybox_front,
            cam.skybox_bottom,
            cam.skybox_right,
            cam.skybox_back);

        cam.skybox->bind();
        _shaders.skybox().set_attrib_pointers();
        _shaders.skybox().bind();
        _shaders.skybox().set_uniform("skybox", 0);
        cam.skybox->unbind();
    }

    if (e.has<ecs::punctual_light_component>())
    {
        _lights.emplace_back(e);
    }
}

void rendering::renderer::forget_entity(ecs::entity &e)
{
    if (_camera_entity != nullptr && _camera_entity->id() == e.id())
    {
        _camera_entity = nullptr;
    }

    if (e.has<ecs::punctual_light_component>())
    {
        int found_ind = -1;
        for(int i = 0; i < _lights.size(); ++i)
        {
            if (_lights[i].get().id() == e.id())
            {
                found_ind = i;
                break;
            }
        }
        if (found_ind > -1)
        {
            _lights.erase(_lights.begin() + found_ind);
        }
    }
}

void rendering::renderer::draw_skybox()
{
    auto& cam = _camera_entity->get_component<ecs::camera_component>();

    auto aspect = _screen_width / _screen_height;
    auto projection = glm::perspective(cam.fov, aspect, cam.near, cam.far);

    auto cam_basis = cam.right_up_fwd();
    auto view = glm::lookAt(cam.position, cam.position + cam_basis[2], cam_basis[1]);
    set_translation(view, 0, 0, 0);

    glDepthMask(GL_FALSE);

    _shaders.skybox().bind();
    _shaders.skybox().set_uniform("view", view);
    _shaders.skybox().set_uniform("projection", projection);

    for (int i = 0; i < cam.float_count; ++i)
        _shaders.skybox().set_uniform(cam.float_props[i].name, cam.float_props[i].data);

    glActiveTexture(GL_TEXTURE0);
    cam.skybox->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    cam.skybox->unbind();

    glDepthMask(GL_TRUE);
}

void rendering::renderer::set_light_uniforms(const ogllib::shader_program_base &shader)
{
    shader.set_uniform("pointLightCount", (GLint)(_lights.size()));

    for (int i = 0; i < _lights.size(); ++i)
    {
        auto& light_e = _lights[i].get();

        if (!light_e.active())
        {
            glm::vec3 zero(0.f);
            shader.set_uniform("pointLights[" + std::to_string(i) + "].color", zero);
            shader.set_uniform("pointLights[" + std::to_string(i) + "].light_pos", zero);
            shader.set_uniform("pointLights[" + std::to_string(i) + "].intensity", 0.f);
        } else
        {
            auto &l = light_e.get_component<ecs::punctual_light_component>();
            auto &light_t = light_e.get_component<ecs::transform_component>();

            shader.set_uniform("pointLights[" + std::to_string(i) + "].color", l.color);
            shader.set_uniform("pointLights[" + std::to_string(i) + "].light_pos", light_t.pos);
            shader.set_uniform("pointLights[" + std::to_string(i) + "].intensity", l.intensity);
        }
        if (i == 7) break;
    }
}

