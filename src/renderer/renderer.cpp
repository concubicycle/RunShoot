#include "renderer/renderer.hpp"



#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>


#include <vectormath.h>


bool rendering::renderer::init()
{
    gl::glViewport(0, 0, _config.width(), _config.height());
    return true;
}

void rendering::renderer::init_render_component(ecs::render_component &r, models::mesh<ogllib::vertex_p>& mesh)
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


void rendering::renderer::init_render_component(ecs::render_component &r, models::textured_mesh<ogllib::vertex_ptx2d>& mesh)
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

    texture.unload_tex();

    r.gl_texture_id = texture.id();
    r.element_count = mesh.mesh_data.indices.size();

    _ptx2d_pvm.bind();
    glUniform1i(glGetUniformLocation(_ptx2d_pvm.getId(), "model_texture"), 0);
}

void rendering::renderer::draw_scene(asset::scene &scene)
{
    gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    gl::glClear(gl::ClearBufferMask::GL_COLOR_BUFFER_BIT);

    auto entities = scene.entity_world();

    // for now, just go through all entities,
    // and draw them if they have required components.
    entities.for_all_entities([this] (ecs::entity& e) {
        auto is_rt =
            (e.archetype_id() & ecs::render_component::archetype_bit) &&
            (e.archetype_id() & ecs::transform_component::archetype_bit);

        if (!is_rt) return;

        auto& r = e.get_component<ecs::render_component>();
        auto& t = e.get_component<ecs::transform_component>();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, r.gl_texture_id);

        auto model = glm::mat4(1.f);
        model = glm::scale(model, glm::vec3(t.scale_x, t.scale_y, t.scale_z)) * model;
        model = glm::eulerAngleYXZ(t.yaw, t.pitch, t.roll) * model;
        set_translation(model, t.x, t.y, t.z);

        // TODO: Replace with camera singleton component
        auto aspect = (float)_config.width() / (float)_config.height();
        auto projection = glm::perspective(_config.fov(), aspect, 0.1f, 1000.f);

        auto view = glm::mat4(1.f);
        set_translation(view, 0.f, 0.f, -3.f); // make sure camera can at least see stuff at (0,0,0)

        auto projection_view_model = projection * view * model;

        _ptx2d_pvm.bind();
        _ptx2d_pvm.getUniforms<glm::mat4>()["projection_view_model"] = projection_view_model;

        r.vao.bind();
        glDrawElements(GL_TRIANGLES, r.element_count, GL_UNSIGNED_INT, 0);
    });
}

