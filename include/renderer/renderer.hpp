#ifndef __RENDERER_H_
#define __RENDERER_H_

#include <opengl_afx.h>

#include <core/startup_config.hpp>
#include <core/scene.h>

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
        asset::texture_manager& textures) :
        _config(config),
        _textures(textures),
        _simple("./assets/shaders/simple.vert", "./assets/shaders/simple.frag"),
        _ptx2d_basic("./assets/shaders/ptx2d_basic.vert", "./assets/shaders/ptx2d_basic.frag"),
        _ptx2d_pvm("./assets/shaders/ptx2d_pvm.vert", "./assets/shaders/ptx2d_pvm.frag")
        {}

    bool init();

    void init_render_component(ecs::render_component& r, models::mesh<ogllib::vertex_p>& mesh);
    void init_render_component(ecs::render_component& r, models::textured_mesh<ogllib::vertex_ptx2d>& mesh);

    void draw_scene(asset::scene& scene);

private:
    const core::startup_config &_config;
    asset::texture_manager &_textures;

    // Program taking NDC position
    ogllib::shader_program<ogllib::vertex_ptx2d> _simple;

    // Program taking NDC position and texture coords 2d
    ogllib::shader_program<ogllib::vertex_ptx2d> _ptx2d_basic;

    // Program taking position and 2d tex cords, and a proj*view*model matrix.
    ogllib::shader_program<ogllib::vertex_ptx2d> _ptx2d_pvm;
};
} // namespace rendering

#endif
