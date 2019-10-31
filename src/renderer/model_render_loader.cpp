//
// Created by sava on 10/24/19.
//

#include <renderer/model_render_loader.hpp>
#include <ecs/components/basic_components.hpp>


rendering::model_render_loader::model_render_loader(
    const shader_set &shaders,
    const asset::basic_mesh_reader& reader,
    const asset::assimp_loader& assimp_loader,
    string_table &hashes,
    asset::texture_manager& textures) :
    _shaders(shaders),
    _reader(reader),
    _assimp_loader(assimp_loader),
    _string_table(hashes),
    _textures(textures)
{}


void rendering::model_render_loader::init_render_component(
    ecs::render_component_ogl &r,
    asset::assimp_model &model)
{
    r.mesh_count = 0;
    for (auto& m : model.meshes)
    {
        init_mesh(r, m, r.mesh_count++);
    }
}

void rendering::model_render_loader::init_render_component(
    ecs::render_component_ogl &r,
    models::textured_mesh<ogllib::vertex_ptx2d> &mesh)
{
    init_mesh(r, mesh, 0);
}

void rendering::model_render_loader::init_mesh(
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

    _shaders.ptx2d_pvm().set_attrib_pointers();

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

    _shaders.ptx2d_pvm().bind();
    glUniform1i(glGetUniformLocation(_shaders.ptx2d_pvm().id(), "model_texture"), 0);
    _shaders.ptx2d_pvm().unbind();
    texture.unbind();
}


void rendering::model_render_loader::init_mesh(
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

    _shaders.default_shader().set_attrib_pointers();

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

    _shaders.default_shader().bind();
    glUniform1i(glGetUniformLocation(_shaders.default_shader().id(), "diffuse_texture"), 0);
    _shaders.default_shader().unbind();
    texture.unbind();
}


void model_load_error(asset::mesh_read_error err)
{
    spdlog::error("Mesh read error: {0}", err);
}

void rendering::model_render_loader::init_entity_world_render_components(ecs::entity_world world)
{
    world.for_all_entities([this](ecs::entity &e)
    {
        using basic_mesh = models::textured_mesh<ogllib::vertex_ptx2d>;

        if (!e.has<ecs::render_component_ogl>()) return;
        auto &r = e.get_component<ecs::render_component_ogl>();

        auto mesh_type = r.mesh_format;
        auto mesh_path = _string_table[r.mesh_path_hash];

        auto ptx2d_load = [this, &r](models::textured_mesh<ogllib::vertex_ptx2d> mesh)
        {
            init_render_component(r, mesh);
        };

        auto assimp_load = [this, &r](asset::assimp_model mesh)
        {
            init_render_component(r, mesh);
        };


        switch (r.mesh_format)
        {
            case asset::mesh_type::P_TX2D:
                _reader.read_mesh_ptx2d(mesh_path).map(ptx2d_load).map_error(model_load_error);
                break;
            case asset::mesh_type::GLTF2:
                _assimp_loader.load_model(mesh_path, mesh_type).map(assimp_load).map_error(model_load_error);
                break;
        }
    });
}
