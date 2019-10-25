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
#include "shader_set.hpp"


namespace rendering
{
class renderer
{
public:
    renderer(
        const core::startup_config &config,
        const core::system_info& system_info,
        const shader_set& shader_set,
        events::event_exchange& events);

    ~renderer();

    bool init();

    void draw_scene(asset::scene& scene);

private:
    const core::startup_config &_config;
    const core::system_info& _system_info;
    const shader_set& _shaders;
    events::event_exchange& _events;

    ecs::entity* _camera_entity;
    listener_id _cam_listener_id;
};
} // namespace rendering

#endif
