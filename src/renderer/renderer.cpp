#include "renderer/renderer.hpp"

#include <glbinding/gl/gl.h>
#include <glm/gtx/euler_angles.hpp>

#include <vectormath.h>

using namespace gl;

rendering::renderer::renderer(
    const core::startup_config &config,
    const core::system_info &system_info,
    events::event_exchange &events,
    asset::texture_manager &textures,
    const string_table& string_table) :
    _config(config),
    _system_info(system_info),
    _string_table(string_table),
    _events(events),
    _textures(textures),
    _camera_entity(nullptr),
    _simple("./assets/shaders/simple.vert", "./assets/shaders/simple.frag"),
    _ptx2d_basic("./assets/shaders/ptx2d_basic.vert", "./assets/shaders/ptx2d_basic.frag"),
    _ptx2d_pvm("./assets/shaders/ptx2d_pvm.vert", "./assets/shaders/ptx2d_pvm.frag"),
    _default("./assets/shaders/default.vert", "./assets/shaders/default.frag")
{
    auto grab_camera = [this](ecs::entity &e)
    {
        if (e.archetype_id() & ecs::camera_component::archetype_bit)
            _camera_entity = &e;
    };

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

//    if (_config.backface_culling())
//    {
//        glEnable(GL_CULL_FACE);
//        glFrontFace(GL_CCW);
//    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return true;
}

void rendering::renderer::init_render_component(ecs::render_component_old &r, models::mesh<ogllib::vertex_p> &mesh)
{
    ogllib::vertex_buffer<ogllib::vertex_p> vbo(mesh.vertices);
    ogllib::index_buffer ebo(mesh.indices);

    r.vao.bind();
    vbo.bind();
    vbo.buffer();

    ebo.bind();
    ebo.buffer();

    _simple.bind();
    _simple.set_attrib_pointers();

    vbo.unbind();
    ebo.unbind();
    r.vao.unbind();
    _simple.unbind();
}

void
rendering::renderer::init_render_component(ecs::render_component_old &r, models::textured_mesh<ogllib::vertex_ptx2d> &mesh)
{
    ogllib::vertex_buffer<ogllib::vertex_ptx2d> vbo(mesh.mesh_data.vertices);
    r.vao.generate();
    ogllib::index_buffer ebo(mesh.mesh_data.indices);

    r.vao.bind();

    vbo.bind();
    vbo.buffer();

    ebo.bind();
    ebo.buffer();

    _ptx2d_pvm.set_attrib_pointers();

    glEnableVertexAttribArray(0);

    r.vao.unbind();
    vbo.unbind();
    ebo.unbind();

    auto texture = _textures.load_texture(mesh.texture_filename);
    texture.bind();
    texture.set_texture_wrap(GL_REPEAT);
    texture.set_filtering(GL_LINEAR, GL_LINEAR);
    texture.buffer();
    texture.gen_mip_maps();

    r.gl_texture_id = texture.id();
    r.element_count = (std::uint32_t) mesh.mesh_data.indices.size();

    _ptx2d_pvm.bind();
    glUniform1i(glGetUniformLocation(_ptx2d_pvm.getId(), "model_texture"), 0);
}

void rendering::renderer::draw_scene(asset::scene &scene)
{
    if (_camera_entity == nullptr)
        return;

    gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto entities = scene.entity_world();

    // for now, just go through all entities,
    // and draw them if they have required components.
    entities.for_all_entities([this](ecs::entity &e)
    {
        auto is_rt =
            (e.archetype_id() & ecs::render_component_old::archetype_bit) &&
            (e.archetype_id() & ecs::transform_component::archetype_bit);

        if (!is_rt) return;

        float width, height;

        // todo: break out to separate method
        if (!_config.fullscreen())
        {
            width = _system_info.monitor_width();
            height = _system_info.monitor_height();
        }
        else
        {
            width = _config.width();
            height = _config.height();
        }

        auto &cam = _camera_entity->get_component<ecs::camera_component>();

        auto &r = e.get_component<ecs::render_component_old>();
        auto &t = e.get_component<ecs::transform_component>();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, r.gl_texture_id);

        auto model = glm::mat4(1.f);
        model = glm::scale(model, glm::vec3(t.scale_x, t.scale_y, t.scale_z)) * model;
        model = glm::eulerAngleYXZ(t.yaw, t.pitch, t.roll) * model;
        set_translation(model, t.pos);

        auto aspect = width / height;
        auto projection = glm::perspective(cam.fov, aspect, cam.near, cam.far);
        auto cam_basis = cam.right_up_fwd();
        auto view = glm::lookAt(cam.position, cam.position + cam_basis[2], cam_basis[1]);

        auto projection_view_model = projection * view * model;

        _ptx2d_pvm.bind();
        auto &uniforms = _ptx2d_pvm.get_uniforms<glm::mat4>();
        uniforms["projection_view_model"] = projection_view_model;

        r.vao.bind();
        glDrawElements(GL_TRIANGLES, r.element_count, GL_UNSIGNED_INT, 0);
    });
}

void rendering::renderer::draw_scene_new(asset::scene &scene)
{
    if (_camera_entity == nullptr)
        return;

    gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto entities = scene.entity_world();

    // for now, just go through all entities,
    // and draw them if they have required components.
    entities.for_all_entities([this](ecs::entity &e)
    {
        auto is_rt =
            (e.archetype_id() & ecs::render_component_ogl::archetype_bit) &&
            (e.archetype_id() & ecs::transform_component::archetype_bit);

        if (!is_rt) return;

        float width, height;

        // todo: break out to separate method
        if (_config.fullscreen())
        {
            width = _system_info.monitor_width();
            height = _system_info.monitor_height();
        }
        else
        {
            width = _config.width();
            height = _config.height();
        }

        auto &cam = _camera_entity->get_component<ecs::camera_component>();

        auto &r = e.get_component<ecs::render_component_ogl>();
        auto &t = e.get_component<ecs::transform_component>();

        auto model = glm::mat4(1.f);
        model = glm::scale(model, glm::vec3(t.scale_x, t.scale_y, t.scale_z)) * model;
        model = glm::eulerAngleYXZ(t.yaw, t.pitch, t.roll) * model;
        set_translation(model, t.pos);

        auto model_inverse = glm::inverse(model);

        auto aspect = width / height;
        auto projection = glm::perspective(cam.fov, aspect, cam.near, cam.far);

        auto cam_basis = cam.right_up_fwd();
        auto view = glm::lookAt(cam.position, cam.position + cam_basis[2], cam_basis[1]);

        if (r.mesh_format == asset::mesh_type::GLTF2)
        {
            _default.bind();

            auto mat_uniforms = _default.get_uniforms<glm::mat4>();
            mat_uniforms["model"] = model;
            mat_uniforms["view"] = view;
            mat_uniforms["projection"] = projection;
            mat_uniforms["model_inverse"] = model_inverse;

            auto vec3_uniforms = _default.get_uniforms<glm::vec3>();
            auto light_pos = glm::vec3(0, 100.f, 0);
            auto light_color = glm::vec3(0.5, 0.5, 0.5);
            auto specular = glm::vec3(0.1f);

            vec3_uniforms["view_pos"] = cam.position;
            vec3_uniforms["light_pos"] = light_pos;
            vec3_uniforms["point_light"] = light_color;
            vec3_uniforms["ambient_light"] = light_color;
            vec3_uniforms["specular"] = specular;

            auto float_uniforms = _default.get_uniforms<float>();
            float_uniforms["shininess"] = 0.2f;

            for (int i = 0; i < r.mesh_count; ++i)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, r.meshes[i].diffuse_texture_id);

                glBindVertexArray(r.meshes[i].vao);

                glDrawElements(GL_TRIANGLES, r.meshes[i].element_count, GL_UNSIGNED_INT, 0);
            }
        } else
        {
            auto projection_view_model = projection * view * model;
            _ptx2d_pvm.bind();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, r.meshes[0].diffuse_texture_id);

            auto &uniforms = _ptx2d_pvm.get_uniforms<glm::mat4>();
            uniforms["projection_view_model"] = projection_view_model;

            glBindVertexArray(r.meshes[0].vao);
            glDrawElements(GL_TRIANGLES, r.meshes[0].element_count, GL_UNSIGNED_INT, 0);
        }
    });
}



void rendering::renderer::init_render_component(ecs::render_component_ogl &r, asset::assimp_model &model)
{
    std::uint32_t mesh_count = 0;
    for (auto& m : model.meshes)
    {
        init_mesh(r, m, mesh_count++);
    }
}

void rendering::renderer::init_render_component(ecs::render_component_ogl &r,
                                                models::textured_mesh<ogllib::vertex_ptx2d> &mesh)
{
    init_mesh(r, mesh, 0);
}

void rendering::renderer::init_mesh(
    ecs::render_component_ogl &r,
    models::textured_mesh<ogllib::vertex_ptx2d> &mesh,
    std::uint32_t mesh_index)
{
    ogllib::vertex_buffer<ogllib::vertex_ptx2d> vbo(mesh.mesh_data.vertices);
    ogllib::vertex_array vao;
    vao.generate();
    ogllib::index_buffer ebo(mesh.mesh_data.indices);

    vao.bind();

    vbo.bind();
    vbo.buffer();

    ebo.bind();
    ebo.buffer();

    _ptx2d_pvm.set_attrib_pointers();

    glEnableVertexAttribArray(0);

    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    auto texture = _textures.load_texture(mesh.texture_filename);
    texture.bind();
    texture.set_texture_wrap(GL_REPEAT);
    texture.set_filtering(GL_LINEAR, GL_LINEAR);
    texture.buffer();
    texture.gen_mip_maps();

    r.meshes[mesh_index].vao = vao.id();
    r.meshes[mesh_index].vbo = vbo.id(); // note, this will be deleted
    r.meshes[mesh_index].ebo = ebo.id(); // note, this will be deleted
    r.meshes[mesh_index].element_count = (std::uint32_t) mesh.mesh_data.indices.size();
    r.meshes[mesh_index].diffuse_texture_id = texture.id();
    r.mesh_count = 1;

    _ptx2d_pvm.bind();
    glUniform1i(glGetUniformLocation(_ptx2d_pvm.getId(), "model_texture"), 0);
}

void rendering::renderer::init_mesh(
    ecs::render_component_ogl &r,
    asset::assimp_mesh &mesh,
    std::uint32_t mesh_index)
{
    ogllib::vertex_buffer<ogllib::vertex_pntx2d> vbo(mesh.mesh_data.vertices);
    ogllib::vertex_array vao;
    vao.generate();
    ogllib::index_buffer ebo(mesh.mesh_data.indices);

    vao.bind();

    vbo.bind();
    vbo.buffer();

    ebo.bind();
    ebo.buffer();

    _default.set_attrib_pointers();

    glEnableVertexAttribArray(0);

    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    // todo: account for multiple textures
    auto texture = _textures.load_texture(mesh.textures[0].path);
    texture.bind();
    texture.set_texture_wrap(GL_REPEAT);
    texture.set_filtering(GL_LINEAR, GL_LINEAR);
    texture.buffer();
    texture.gen_mip_maps();

    r.meshes[mesh_index].vao = vao.id();
    r.meshes[mesh_index].vbo = vbo.id(); // note, this will be deleted
    r.meshes[mesh_index].ebo = ebo.id(); // note, this will be deleted
    r.meshes[mesh_index].element_count = (std::uint32_t) mesh.mesh_data.indices.size();
    r.meshes[mesh_index].diffuse_texture_id = texture.id();

    _default.bind();
    glUniform1i(glGetUniformLocation(_ptx2d_pvm.getId(), "diffuse_texture"), 0);
}
