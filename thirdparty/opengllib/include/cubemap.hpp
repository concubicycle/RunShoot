//
// Created by sava on 11/14/19.
//

#ifndef __CUBEMAP_HPP_
#define __CUBEMAP_HPP_

#include <opengl_afx.h>
#include "vertex_p.h"

using namespace gl;


namespace ogllib
{

    class cubemap
    {
        static const vertex_p vertices[];

    public:
        cubemap(
            std::string left,
            std::string top,
            std::string front,
            std::string bottom,
            std::string right,
            std::string back);

        void bind();
        void unbind();
    private:
        GLuint _vao;
        GLuint _vbo;
        GLuint _texture_id;

        unsigned int load_cubemap(std::vector<std::string> faces);
    };

}

#endif //__CUBEMAP_HPP_
