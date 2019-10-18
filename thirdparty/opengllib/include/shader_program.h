#ifndef SHADER_PROGRAM_H__
#define SHADER_PROGRAM_H__

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <memory>

#include <opengl_afx.h>
#include <program_info.h>
#include <vertex_buffer.h>
#include <shader.h>
#include <vertex_array.h>

#include <shader_program_uniform_assign.h>

#include <spdlog/spdlog.h>
#include <vertex_pctx2d.h>
#include <vertex_ptx2d.h>
#include <vertex_pc.h>
#include "file_read_std.h"


using namespace gl;

namespace ogllib
{

    template<class TVertexFormat>
    class shader_program
    {
        static constexpr const unsigned int GPU_INFOLOG_BUFFER_SIZE = 512;

    public:
        shader_program(std::string vertex_shader_path, std::string fragment_shader_path)
        {
            _id = glCreateProgram();
            build(vertex_shader_path, fragment_shader_path);
        }

        ~shader_program()
        {
            glDeleteProgram(_id); // Delete the shader program
        }

        void set_attrib_pointers();


        template<typename TUniform>
        Uniforms<shader_program<TVertexFormat>, TUniform>& get_uniforms()
        {
            return get_uniforms<TUniform>(_info);
        }

        template<typename TUniform>
        void set_uniform(const std::string &name, TUniform value)
        {
            get_uniforms()[name] = value;
        }

        //COMPILATION
        void build(std::string vertex_shader_path, std::string fragment_shader_path)
        {
            shader<FileReadStd> vertex(GL_VERTEX_SHADER, vertex_shader_path);
            shader<FileReadStd> fragment(GL_FRAGMENT_SHADER, fragment_shader_path);

            vertex.compile();
            glAttachShader(_id, vertex.id());

            fragment.compile();
            glAttachShader(_id, fragment.id());

            link();

            glDetachShader(_id, vertex.id());
            glDetachShader(_id, fragment.id());
        }

        void link()
        {
            if (_compiled)
                return;

            glLinkProgram(_id);
            validate_program();

            _info.initialize(_id);

            _compiled = true;
        }

        // BINDING
        void bind()
        {
            if (!_compiled)
                std::cerr << "Attempting to bind non-compiled program.";

            glUseProgram(_id);
        }

        void unbind()
        {
            glUseProgram(0);
        }

        // GETTERS/SETTERS
        unsigned int getId() { return _id; }

        program_info &getInfo() { return _info; }

    private:
        ogllib::program_info _info;
        unsigned int _id = 0;   // The shader program identifier
        bool _compiled = false; // Whether or not we have initialized the shader

        void validate_program()
        {
            char buffer[GPU_INFOLOG_BUFFER_SIZE];
            GLsizei length = 0;
            GLint link_status;
            GLint validate_status;

            memset(buffer, 0, GPU_INFOLOG_BUFFER_SIZE);

            glGetProgramiv(_id, GL_LINK_STATUS, &link_status);
            if (!link_status)
            {
                glGetProgramInfoLog(_id, GPU_INFOLOG_BUFFER_SIZE, &length, buffer);
                spdlog::info("Error linking program {0:d}. Link error:{1:d} \n", _id, buffer);
            }

            glValidateProgram(_id);
            glGetProgramiv(_id, GL_VALIDATE_STATUS, &validate_status);
            if (validate_status == 0)
            {
                spdlog::error("Error validating program {0:d} \n.", _id);
            } else
            {
                glGetProgramInfoLog(_id, GPU_INFOLOG_BUFFER_SIZE, &length, buffer);
                spdlog::info("Shader program {0:d} built successfully. Info log: {1}", _id, buffer);
            }
        }

        template<typename TUniform>
        static Uniforms<shader_program<TVertexFormat>, TUniform> &get_uniforms(program_info &info)
        {
            static std::unordered_map<std::uint32_t, Uniforms<shader_program<TVertexFormat>, TUniform>> proxies;
            auto p = proxies.find(info.id());
            if (p == proxies.end())
            {
                proxies.insert({info.id(), Uniforms<shader_program<TVertexFormat>, TUniform>(info)});
            }

            return proxies.find(info.id())->second;
        }
    };
} // namespace ogllib

#endif
