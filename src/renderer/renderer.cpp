#include "renderer/renderer.hpp"

#include <glbinding/gl/gl.h>
#include <glm/gtx/euler_angles.hpp>

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
    _camera_entity(nullptr)
{
    auto grab_camera = [this](ecs::entity &e) {
        if (e.archetype_id() & ecs::camera_component::archetype_bit)
            _camera_entity = &e;
    };

    // todo: if camera is destroyed, set pointer to null
    _cam_listener_id = _events.subscribe(events::entity_created, std::function<void(ecs::entity &)>(grab_camera));
}

rendering::renderer::~renderer()
{
    _events.unsubscribe(events::entity_created, _cam_listener_id);
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
    glDepthFunc(GL_LESS);

    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    return true;
}


void rendering::renderer::draw_scene(asset::scene &scene)
{
    if (_camera_entity == nullptr)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto &entities = scene.entity_world();

    // for now, just go through all entities,
    // and draw them if they have required components.
    entities.for_all_entities([this](ecs::entity &e) {
        auto is_rt =
            (e.archetype_id() & ecs::render_component_ogl::archetype_bit) &&
            (e.archetype_id() & ecs::transform_component::archetype_bit);

        if (!is_rt) return;

        std::uint32_t width, height;

        // todo: break out to separate method
        if (_config.fullscreen())
        {
            width = _system_info.monitor_width();
            height = _system_info.monitor_height();
        } else
        {
            width = _config.width();
            height = _config.height();
        }

        auto &cam = _camera_entity->get_component<ecs::camera_component>();

        auto &r = e.get_component<ecs::render_component_ogl>();
        auto &t = e.get_component<ecs::transform_component>();

        auto model = glm::mat4(1.f);
        model = glm::scale(model, glm::vec3(t.scale_x, t.scale_y, t.scale_z));
        model = glm::eulerAngleYXZ(t.yaw, t.pitch, t.roll) * model;
        set_translation(model, t.pos);

        auto model_inverse = glm::inverse(model);

        auto aspect = (float)width / (float)height;
        auto projection = glm::perspective(cam.fov, aspect, cam.near, cam.far);

        auto cam_basis = cam.right_up_fwd();
        auto view = glm::lookAt(cam.position, cam.position + cam_basis[2], cam_basis[1]);

		auto light_pos = glm::vec3(0, 10.f, 10.f);
		auto light_color = glm::vec3(0.5, 0.5, 0.5);
		auto ambient_light = glm::vec3(0.2, 0.1, 0.2);
		auto specular = glm::vec3(0.03f);

        if (r.mesh_format == asset::mesh_type::GLTF2)
        {
            auto& shader = _shaders.default_shader();
            shader.bind();
            
			shader.set_uniform("model", model);
			shader.set_uniform("view", view);
			shader.set_uniform("projection", projection);
			shader.set_uniform("model_inverse", model_inverse);
			shader.set_uniform("view_pos", cam.position);
			shader.set_uniform("light_pos", light_pos);
			shader.set_uniform("point_light", light_color);
			shader.set_uniform("ambient_light", ambient_light);
			shader.set_uniform("specular", specular);            
			shader.set_uniform("shininess", 0.2f);
            shader.set_uniform("light_intensity", 100.f);

            for (std::uint32_t i = 0; i < r.mesh_count; ++i)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, r.meshes[i].diffuse_texture_id);

                glBindVertexArray(r.meshes[i].vao);
                glDrawElements(GL_TRIANGLES, r.meshes[i].element_count, GL_UNSIGNED_INT, 0);
            }

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
            shader.unbind();
        } else
        {
            auto& shader = _shaders.ptx2d_pvm();

            auto projection_view_model = projection * view * model;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, r.meshes[0].diffuse_texture_id);

            shader.bind();

            shader.set_uniform("projection_view_model", projection_view_model);

            glBindVertexArray(r.meshes[0].vao);
            glDrawElements(GL_TRIANGLES, r.meshes[0].element_count, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            shader.unbind();
        }

        auto err = glGetError();

    });
}

void rendering::renderer::resize(std::uint32_t width, std::uint32_t height)
{
    glViewport(0, 0, width, height);
}

