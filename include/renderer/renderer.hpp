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
#include <shader.h>
#include <shader_program.h>

#include <ecs/components/basic_components.hpp>
#include <asset/mesh.hpp>
#include <asset/textured_mesh.hpp>
#include <asset/texture_manager.hpp>
#include <asset/scene.hpp>

#include <shader_program_uniform_assign.h>

namespace rendering
{
class renderer
{
public:
    renderer(
        const core::startup_config &config,
        const core::system_info& system_info,
        events::event_exchange& events,
        asset::texture_manager& textures);

    ~renderer();

    bool init();

    void init_render_component(ecs::render_component& r, models::mesh<ogllib::vertex_p>& mesh);
    void init_render_component(ecs::render_component& r, models::textured_mesh<ogllib::vertex_ptx2d>& mesh);

    void draw_scene(asset::scene& scene);

private:
    const core::startup_config &_config;
    events::event_exchange _events;
    asset::texture_manager &_textures;
    const core::system_info& _system_info;

    ecs::entity* _camera_entity;
    listener_id _cam_listener_id;

    // Program taking NDC position
    ogllib::shader_program<ogllib::vertex_ptx2d> _simple;

    // Program taking NDC position and texture coords 2d
    ogllib::shader_program<ogllib::vertex_ptx2d> _ptx2d_basic;

    // Program taking position and 2d tex cords, and a proj*view*model matrix.
    ogllib::shader_program<ogllib::vertex_ptx2d> _ptx2d_pvm;
};
} // namespace rendering

#endif
