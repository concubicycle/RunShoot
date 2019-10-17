//
// Created by sava on 10/17/19.
//

#include "shader_program.h"
#include "vertex_p.h"
#include "vertex_pc.h"
#include "vertex_ptx2d.h"
#include "vertex_pctx2d.h"


namespace ogllib
{
    template<>
    void shader_program<ogllib::vertex_pc>::set_attrib_pointers()
    {
        auto posIndex = _info.getAttribLocation("position");
        auto colIndex = _info.getAttribLocation("rgba");

        size_t posOffset = offsetof(ogllib::vertex_pc, position);
        size_t colOffset = offsetof(ogllib::vertex_pc, color);

        glVertexAttribPointer(posIndex, 3, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_pc),
            reinterpret_cast<void *>(posOffset));
        glEnableVertexAttribArray(posIndex);

        glVertexAttribPointer(colIndex, 4, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_pc),
            reinterpret_cast<void *>(colOffset));
        glEnableVertexAttribArray(colIndex);
    }

    template<>
    void shader_program<ogllib::vertex_ptx2d>::set_attrib_pointers()
    {
        auto posIndex = _info.getAttribLocation("position");
        auto txIndex = _info.getAttribLocation("texcoords_2d");

        size_t posOffset = offsetof(ogllib::vertex_ptx2d, position);
        size_t txOffset = offsetof(ogllib::vertex_ptx2d, textureCoordinates2d);

        glVertexAttribPointer(posIndex, 3, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_ptx2d),
            reinterpret_cast<void *>(posOffset));
        glEnableVertexAttribArray(posIndex);

        glVertexAttribPointer(txIndex, 2, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_ptx2d),
            reinterpret_cast<void *>(txOffset));
        glEnableVertexAttribArray(txIndex);
    }

    template<>
    void shader_program<ogllib::vertex_pctx2d>::set_attrib_pointers()
    {
        auto posIndex = _info.getAttribLocation("position");
        auto colIndex = _info.getAttribLocation("rgba");
        auto txIndex = _info.getAttribLocation("texcoords_2d");

        size_t posOffset = offsetof(ogllib::vertex_pctx2d, position);
        size_t colOffset = offsetof(ogllib::vertex_pctx2d, color);
        size_t txOffset = offsetof(ogllib::vertex_pctx2d, textureCoordinates2d);

        glVertexAttribPointer(posIndex, 3, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_pctx2d),
            reinterpret_cast<void *>(posOffset));
        glEnableVertexAttribArray(posIndex);

        glVertexAttribPointer(colIndex, 4, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_pctx2d),
            reinterpret_cast<void *>(colOffset));
        glEnableVertexAttribArray(colIndex);

        glVertexAttribPointer(txIndex, 2, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_pctx2d),
            reinterpret_cast<void *>(txOffset));
        glEnableVertexAttribArray(txIndex);
    }


}