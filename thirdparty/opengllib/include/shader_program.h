#ifndef SHADER_PROGRAM_H__
#define SHADER_PROGRAM_H__

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <memory>

#include <opengl_afx.h>
#include <program_info.h>
#include <vertex_buffer.h>
#include <shader.h>
#include <vertex_array.h>

#include <spdlog/spdlog.h>
#include <vertex_pctx2d.h>
#include <vertex_ptx2d.h>
#include <vertex_pc.h>
#include <glm/gtc/type_ptr.hpp>
#include "file_read_std.h"


using namespace gl;

namespace ogllib
{
    class shader_program_base
    {
    public:
        virtual ~shader_program_base() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void set_uniform(const std::string& name, GLfloat val) const = 0;
        virtual	void set_uniform(const std::string& name, glm::mat4& val) const = 0;
        virtual void set_uniform(const std::string& name, glm::vec3& val) const = 0;
        virtual void set_uniform(const std::string& name, glm::vec2& val) const =0;
        virtual void set_uniform(const std::string& name, GLint val) const = 0;
    };

    template<class TVertexFormat>
    class shader_program : public shader_program_base
    {
        static constexpr const unsigned int GPU_INFO_BUFFER_SIZE = 512;

    public:
        shader_program(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
        {
            _id = glCreateProgram();
            build(vertex_shader_path, fragment_shader_path);
        }

        ~shader_program() override
        {
            glDeleteProgram(_id); // Delete the shader program
        }

        void set_attrib_pointers() const;
        
		
		void set_uniform(const std::string& name, GLfloat val) const override
        {
            auto loc = _info.getUniformLocation(name);
            if (loc == -1) return;
			glUniform1f(loc, val);
		}

		void set_uniform(const std::string& name, glm::mat4& val) const override
		{
		    auto loc = _info.getUniformLocation(name);
		    if (loc == -1) return;
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
		}

		void set_uniform(const std::string& name, glm::vec3& val) const override
        {
            auto loc = _info.getUniformLocation(name);
            if (loc == -1) return;
			glUniform3fv(loc, 1, glm::value_ptr(val));
		}

        void set_uniform(const std::string& name, glm::vec2& val) const override
        {
            auto loc = _info.getUniformLocation(name);
            if (loc == -1) return;
            glUniform2fv(loc, 1, glm::value_ptr(val));
        }

        void set_uniform(const std::string& name, GLint val) const override
        {
            auto loc = _info.getUniformLocation(name);
            if (loc == -1) return;
            glUniform1i(loc, val);
        }
		

        //COMPILATION
        void build(std::string vertex_shader_path, std::string fragment_shader_path)
        {
            shader<FileReadStd> vertex(GL_VERTEX_SHADER, std::move(vertex_shader_path));
            shader<FileReadStd> fragment(GL_FRAGMENT_SHADER, std::move(fragment_shader_path));

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
        void bind() const override
        {
            if (!_compiled)
                std::cerr << "Attempting to bind non-compiled program.";

            glUseProgram(_id);
        }

        void unbind() const override
        {
            glUseProgram(0);
        }


        [[nodiscard]] unsigned int id() const { return _id; }

        program_info &get_info();

    private:
        ogllib::program_info _info;
        unsigned int _id = 0;   // The shader program identifier
        bool _compiled = false; // Whether or not we have initialized the shader

        void validate_program()
        {
            char buffer[GPU_INFO_BUFFER_SIZE];
            GLsizei length = 0;
            GLint link_status;
            GLint validate_status;

            memset(buffer, 0, GPU_INFO_BUFFER_SIZE);

            glGetProgramiv(_id, GL_LINK_STATUS, &link_status);
            if (!link_status)
            {
                glGetProgramInfoLog(_id, GPU_INFO_BUFFER_SIZE, &length, buffer);
                spdlog::error("Error linking program {0}. Link error:{1} \n", _id, buffer);
            }

            glValidateProgram(_id);
            glGetProgramiv(_id, GL_VALIDATE_STATUS, &validate_status);
            if (validate_status == 0)
            {
                spdlog::error("Error validating program {0} \n.", _id);
            } else
            {
                glGetProgramInfoLog(_id, GPU_INFO_BUFFER_SIZE, &length, buffer);
                spdlog::info("Shader program {0} built successfully. Info log: {1}", _id, buffer);
            }
        }
    };
} // namespace ogllib

#endif
