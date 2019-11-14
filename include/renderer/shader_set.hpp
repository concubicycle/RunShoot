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
    public:
        shader_set() :
            _simple("./assets/shaders/simple.vert", "./assets/shaders/simple.frag"),
            _ptx2d_basic("./assets/shaders/ptx2d_basic.vert", "./assets/shaders/ptx2d_basic.frag"),
            _ptx2d_pvm("./assets/shaders/ptx2d_pvm.vert", "./assets/shaders/ptx2d_pvm.frag"),
            _default("./assets/shaders/default.vert", "./assets/shaders/default.frag"),
            _skybox("./assets/shaders/skybox.vert", "./assets/shaders/skybox.frag"){}

        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_ptx2d> &simple() const { return _simple; }
        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_ptx2d> &ptx2d_basic() const { return _ptx2d_basic; }
        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_ptx2d> &ptx2d_pvm() const { return _ptx2d_pvm; }
        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_pntx2d> &default_shader() const { return _default; }
        [[nodiscard]] const ogllib::shader_program<ogllib::vertex_p> &skybox() const { return _skybox; }

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
    };

}

#endif //__SHADER_SET_HPP_
