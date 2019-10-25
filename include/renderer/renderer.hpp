#ifndef __RENDERER_H_
#define __RENDERER_H_

#include <vector>

#include <opengl_afx.h>

#include <core/startup_config.hpp>
#include <core/scene.h>
#include <core/system_info.hpp>

#include <vertex_buffer.h>
#include <vertex_p.h>
#include <vertex_ptx2d.h>
#include <vertex_pntx2d.h>
#include <shader.h>
#include <shader_program.h>
#include <shader_program_uniform_assign.h>

#include <ecs/components/basic_components.hpp>
#include <asset/mesh.hpp>
#include <asset/textured_mesh.hpp>
#include <asset/texture_manager.hpp>
#include <asset/scene.hpp>
#include <asset/asset_types.hpp>



namespace rendering
{
class renderer
{
public:
    renderer(
        const core::startup_config &config,
        const core::system_info& system_info,
        events::event_exchange& events,
        asset::texture_manager& textures,
        const string_table& _string_table);

    ~renderer();

    bool init();

    void init_render_component(ecs::render_component_old& r, models::mesh<ogllib::vertex_p>& mesh);
    void init_render_component(ecs::render_component_old& r, models::textured_mesh<ogllib::vertex_ptx2d>& mesh);
    void init_render_component(ecs::render_component_ogl &r, asset::assimp_model &mesh);
    void init_render_component(ecs::render_component_ogl &r, models::textured_mesh<ogllib::vertex_ptx2d>& mesh);

    void draw_scene(asset::scene& scene);
    void draw_scene_new(asset::scene& scene);

private:
    const core::startup_config &_config;
    const core::system_info& _system_info;
    const string_table& _string_table;
    events::event_exchange& _events;
    asset::texture_manager &_textures;

    ecs::entity* _camera_entity;
    listener_id _cam_listener_id;


    // shader programs. todo: manage this better
    // Program taking NDC position
    ogllib::shader_program<ogllib::vertex_ptx2d> _simple;

    // Program taking NDC position and texture coords 2d
    ogllib::shader_program<ogllib::vertex_ptx2d> _ptx2d_basic;

    // Program taking position and 2d tex cords, and a proj*view*model matrix.
    ogllib::shader_program<ogllib::vertex_ptx2d> _ptx2d_pvm;

    // Program taking position, normal, and 2d tex cords,
    // and a proj*view*model matrix, along with a normal transform matrix.
    ogllib::shader_program<ogllib::vertex_pntx2d> _default;


    void init_mesh(
        ecs::render_component_ogl &r,
        models::textured_mesh<ogllib::vertex_ptx2d>& mesh,
        std::uint32_t mesh_index);

    void init_mesh(
        ecs::render_component_ogl &r,
        asset::assimp_mesh& mesh,
        std::uint32_t mesh_index);


};
} // namespace rendering

#endif
