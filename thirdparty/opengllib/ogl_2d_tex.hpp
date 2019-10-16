//
// Created by sava on 10/16/19.
//

#ifndef __OPEN_GL_TEXTURE_HPP_
#define __OPEN_GL_TEXTURE_HPP_

#include "opengl_afx.h"
#include "spdlog/spdlog.h"


namespace ogllib
{
    using namespace gl;

    class ogl_2d_tex
    {
    public:
        ogl_2d_tex(
            std::shared_ptr<std::uint8_t> bytes,
            std::uint32_t width,
            std::uint32_t height,
            std::uint8_t components_per_texel) :
            _bytes(bytes),
            _width(width),
            _height(height),
            _components_per_texel(components_per_texel),
            _color_type(_components_per_texel == 3 ? gl::GLenum::GL_RGB : gl::GLenum::GL_RGBA )
        {
            gl::glGenTextures(1, &_id);
        }

        GLuint id() { return _id; }

        void bind()
        {
            gl::glBindTexture(GL_TEXTURE_2D, this->id());
            _is_bound = true;
        }

        void unbind()
        {
            gl::glBindTexture(GL_TEXTURE_2D, 0);
            _is_bound = false;
        }

        void buffer()
        {
            auto data = _bytes.get();

            glTexImage2D(GL_TEXTURE_2D,
                0,
                _color_type,
                _width,
                _height,
                0,
                _color_type,
                GL_UNSIGNED_BYTE,
                data);
        }

        void gen_mip_maps() const
        {
            if (!_is_bound)
                spdlog::warn("Attempting to call gen_mip_maps() on unbound texture");

            gl::glGenerateMipmap(GL_TEXTURE_2D);
        }

        void set_texture_wrap(gl::GLenum mode) const
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
        }

        void set_filtering(gl::GLenum min, gl::GLenum mag) const
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
        }

        void unload_tex()
        {
            _bytes.reset();
        }

    private:
        bool _is_bound = false;

        std::shared_ptr<std::uint8_t> _bytes;
        std::uint32_t _width ;
        std::uint32_t _height;
        std::uint8_t _components_per_texel;

        gl::GLuint _id;
        gl::GLenum _color_type;
    };


}


#endif //__OPEN_GL_TEXTURE_HPP_
