#ifndef __RENDERER_H_
#define __RENDERER_H_


#include <vector>
#include <cstdint>
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


#include <ecs/components/basic_components.hpp>
#include <asset/mesh.hpp>
#include <asset/textured_mesh.hpp>
#include <asset/texture_manager.hpp>
#include <asset/scene.hpp>
#include <asset/asset_types.hpp>

#include <renderer/shader_set.hpp>
#include "skybox.hpp"


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
    void resize(std::uint32_t width, std::uint32_t height);

  //  void (*build_framebuffer_callback(rendering::renderer& r))(GLFWwindow*, int, int);



private:
    const core::startup_config &_config;
    const core::system_info& _system_info;
    const shader_set& _shaders;
    events::event_exchange& _events;

    ecs::entity* _camera_entity;
    listener_id _cam_listener_id;
    listener_id _cam_remove_listener_id;

    float _screen_width, _screen_height;

    skybox sky;

    void grab_entity(ecs::entity& e);
    void forget_entity(ecs::entity& e);
};
} // namespace rendering

#endif
