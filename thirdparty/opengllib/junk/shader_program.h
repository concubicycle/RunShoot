//#ifndef SHADER_PROGRAM_H__
//#define SHADER_PROGRAM_H__
//
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <stdlib.h>
//#include <vector>
//#include <memory>
//
//#include <opengl_afx.h>
//#include <program_info.h>
//#include <vertex_buffer.h>
//#include <shader.h>
//#include <vertex_array.h>
//
//#include <shader_program_uniform_assign.h>
//
//#include <spdlog/spdlog.h>
//#include <vertex_pctx2d.h>
//#include <vertex_ptx2d.h>
//#include <vertex_pc.h>
//#include "file_read_std.h"
//
//
//using namespace gl;
//
//namespace ogllib
//{
//
//    template<class TVertexFormat>
//    class shader_program
//    {
//        static constexpr const unsigned int GPU_INFOLOG_BUFFER_SIZE = 512;
//
//    public:
//        shader_program(std::string vertex_shader_path, std::string fragment_shader_path)
//        {
//            _id = glCreateProgram();
//            build(vertex_shader_path, fragment_shader_path);
//        }
//
//        ~shader_program()
//        {
//            glDeleteProgram(_id); // Delete the shader program
//        }
//
//        void set_attrib_pointers()
//        {
//            assert(false);
//        }
//
//
//        template<typename TUniform>
//        Uniforms<shader_program<TVertexFormat>, TUniform> getUniforms()
//        {
//            return getUniforms<TUniform>(_info);
//        }
//
//        template<typename TUniform>
//        void setUniform(const std::string &name, TUniform value)
//        {
//            getUniforms()[name] = value;
//        }
//
//        //COMPILATION
//        void build(std::string vertex_shader_path, std::string fragment_shader_path)
//        {
//            shader<FileReadStd> vertex(GL_VERTEX_SHADER, vertex_shader_path);
//            shader<FileReadStd> fragment(GL_FRAGMENT_SHADER, fragment_shader_path);
//
//            vertex.compile();
//            glAttachShader(_id, vertex.id());
//
//            fragment.compile();
//            glAttachShader(_id, fragment.id());
//
//            link();
//
//            glDetachShader(_id, vertex.id());
//            glDetachShader(_id, fragment.id());
//        }
//
//        void link()
//        {
//            if (_compiled)
//                return;
//
//            glLinkProgram(_id);
//            validate_program();
//
//            _info.initialize(_id);
//
//            _compiled = true;
//        }
//
//        // BINDING
//        void bind()
//        {
//            if (!_compiled)
//                std::cerr << "Attempting to bind non-compiled program.";
//
//            glUseProgram(_id);
//        }
//
//        void unbind()
//        {
//            glUseProgram(0);
//        }
//
//        // GETTERS/SETTERS
//        unsigned int getId() { return _id; }
//
//        program_info &getInfo() { return _info; }
//
//    private:
//        ogllib::program_info _info;
//
//
//        unsigned int _id = 0;   // The shader program identifier
//        bool _compiled = false; // Whether or not we have initialized the shader
//
//        void validate_program()
//        {
//            char buffer[GPU_INFOLOG_BUFFER_SIZE];
//            GLsizei length = 0;
//            GLint link_status;
//            GLint validate_status;
//
//            memset(buffer, 0, GPU_INFOLOG_BUFFER_SIZE);
//
//            glGetProgramiv(_id, GL_LINK_STATUS, &link_status);
//            if (!link_status)
//            {
//                glGetProgramInfoLog(_id, GPU_INFOLOG_BUFFER_SIZE, &length, buffer);
//                spdlog::info("Error linking program {0:d}. Link error:{1:d} \n", _id, buffer);
//            }
//
//            glValidateProgram(_id);
//            glGetProgramiv(_id, GL_VALIDATE_STATUS, &validate_status);
//            if (validate_status == 0)
//            {
//                spdlog::error("Error validating program {0:d} \n.", _id);
//            } else
//            {
//                glGetProgramInfoLog(_id, GPU_INFOLOG_BUFFER_SIZE, &length, buffer);
//                spdlog::info("Shader program {0:d} built successfully. Info log: {1}", _id, buffer);
//            }
//        }
//
//        template<typename TUniform>
//        static Uniforms<shader_program<TVertexFormat>, TUniform> &getUniforms(program_info &info)
//        {
//            static Uniforms<shader_program<TVertexFormat>, TUniform> proxy(info); //swag
//            return proxy;
//        }
//    };
//
//
//    template<> void shader_program<ogllib::vertex_pc>::set_attrib_pointers()
//    {
//        auto posIndex = _info.getAttribLocation("position");
//        auto colIndex = _info.getAttribLocation("rgba");
//
//        size_t posOffset = offsetof(ogllib::vertex_pc, position);
//        size_t colOffset = offsetof(ogllib::vertex_pc, color);
//
//        glVertexAttribPointer(posIndex, 3, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_pc),
//            reinterpret_cast<void *>(posOffset));
//        glEnableVertexAttribArray(posIndex);
//
//        glVertexAttribPointer(colIndex, 4, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_pc),
//            reinterpret_cast<void *>(colOffset));
//        glEnableVertexAttribArray(colIndex);
//    }
//
////    template<>
////    class shader_program<ogllib::vertex_ptx2d>
////    {
////    public:
////            void set_attrib_pointers();
////    };
//
//    template<>
//    void shader_program<ogllib::vertex_ptx2d>::set_attrib_pointers()
//    {
//        auto posIndex = _info.getAttribLocation("position");
//        auto txIndex = _info.getAttribLocation("texcoords_2d");
//
//        size_t posOffset = offsetof(ogllib::vertex_ptx2d, position);
//        size_t txOffset = offsetof(ogllib::vertex_ptx2d, textureCoordinates2d);
//
//        glVertexAttribPointer(posIndex, 3, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_ptx2d),
//            reinterpret_cast<void *>(posOffset));
//        glEnableVertexAttribArray(posIndex);
//
//        glVertexAttribPointer(txIndex, 2, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_ptx2d),
//            reinterpret_cast<void *>(txOffset));
//        glEnableVertexAttribArray(txIndex);
//    }
//
//    template<>
//    void shader_program<ogllib::vertex_pctx2d>::set_attrib_pointers()
//    {
//        auto posIndex = _info.getAttribLocation("position");
//        auto colIndex = _info.getAttribLocation("rgba");
//        auto txIndex = _info.getAttribLocation("texcoords_2d");
//
//        size_t posOffset = offsetof(ogllib::vertex_pctx2d, position);
//        size_t colOffset = offsetof(ogllib::vertex_pctx2d, color);
//        size_t txOffset = offsetof(ogllib::vertex_pctx2d, textureCoordinates2d);
//
//        glVertexAttribPointer(posIndex, 3, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_pctx2d),
//            reinterpret_cast<void *>(posOffset));
//        glEnableVertexAttribArray(posIndex);
//
//        glVertexAttribPointer(colIndex, 4, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_pctx2d),
//            reinterpret_cast<void *>(colOffset));
//        glEnableVertexAttribArray(colIndex);
//
//        glVertexAttribPointer(txIndex, 2, gl::GLenum::GL_FLOAT, GL_FALSE, sizeof(ogllib::vertex_pctx2d),
//            reinterpret_cast<void *>(txOffset));
//        glEnableVertexAttribArray(txIndex);
//    }
//
//
//} // namespace ogllib
//
//
//
//
//
//namespace ogllib
//{
//
//}
//
//#endif
