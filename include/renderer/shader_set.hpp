//
// Created by sava on 10/24/19.
//

#ifndef __SHADER_SET_HPP_
#define __SHADER_SET_HPP_

#include <shader_program.h>
#include <vertex_pntx2d.h>

namespace rendering
{

    class shader_set
    {
        using shader_getter=const ogllib::shader_program_base& (shader_set::*)(void);
    public:
        shader_set() :
            _simple("./assets/shaders/simple.vert", "./assets/shaders/simple.frag"),
            _ptx2d_basic("./assets/shaders/ptx2d_basic.vert", "./assets/shaders/ptx2d_basic.frag"),
            _ptx2d_pvm("./assets/shaders/ptx2d_pvm.vert", "./assets/shaders/ptx2d_pvm.frag"),
            _default("./assets/shaders/default.vert", "./assets/shaders/default.frag"),
            _skybox("./assets/shaders/skybox.vert", "./assets/shaders/skybox.frag"),
            _debug("./assets/shaders/debug.vert", "./assets/shaders/debug.frag"),
            _overlay("./assets/shaders/overlay.vert", "./assets/shaders/overlay.frag"),
            _glare("./assets/shaders/glare.vert", "./assets/shaders/glare.frag")
        {}

        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_ptx2d> &simple() const { return _simple; }
        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_ptx2d> &ptx2d_basic() const { return _ptx2d_basic; }
        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_ptx2d> &ptx2d_pvm() const { return _ptx2d_pvm; }
        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_pntx2d> &default_shader() const { return _default; }
        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_p> &skybox() const { return _skybox; }
        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_p> &debug() const { return _debug; }
        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_pc> &overlay() const { return _overlay; }
        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_ptx2d> &glare() const { return _glare; }

        const ogllib::shader_program_base& get_program(std::string shader_name) const
        {
            if (shader_name == "simple") return simple();
            if (shader_name == "ptx2d_basic") return ptx2d_basic();
            if (shader_name == "ptx2d_pvm") return ptx2d_pvm();
            if (shader_name == "default_shader") return default_shader();
            if (shader_name == "skybox") return skybox();
            if (shader_name == "debug") return debug();
            if (shader_name == "overlay") return overlay();
            if (shader_name == "glare") return glare();

            throw "Unknown shader program";
        }

    private:
        // Program taking NDC position
        ogllib::shader_program<ogllib::vertex_ptx2d> _simple;

        // Program taking NDC position and texture coords 2d
        ogllib::shader_program<ogllib::vertex_ptx2d> _ptx2d_basic;

        // Program taking position and 2d tex cords, and a proj*view*model matrix.
        ogllib::shader_program<ogllib::vertex_ptx2d> _ptx2d_pvm;

        // Program taking position, normal, and 2d tex cords,
        // and a proj*view*model matrix, along with a normal transform matrix.
        ogllib::shader_program<ogllib::vertex_pntx2d> _default;

        ogllib::shader_program<ogllib::vertex_p> _skybox;

        ogllib::shader_program<ogllib::vertex_p> _debug;

        ogllib::shader_program<ogllib::vertex_pc> _overlay;

        ogllib::shader_program<ogllib::vertex_ptx2d> _glare;
    };

}

#endif //__SHADER_SET_HPP_
