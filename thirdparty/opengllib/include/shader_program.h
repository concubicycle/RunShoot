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

    template<class TVertexFormat>
    class shader_program
    {
        static constexpr const unsigned int GPU_INFO_BUFFER_SIZE = 512;

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

        void set_attrib_pointers() const;
        
		
		void set_uniform(const std::string& name, GLfloat val) const
		{
			glUniform1f(_info.getUniformLocation(name), val);
		}

		void set_uniform(const std::string& name, glm::mat4& val) const
		{
			glUniformMatrix4fv(_info.getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(val));
		}

		void set_uniform(const std::string& name, glm::vec3& val) const
		{
			glUniform3fv(_info.getUniformLocation(name), 1, glm::value_ptr(val));
		}

        void set_uniform(const std::string& name, GLint val) const
        {
            glUniform1i(_info.getUniformLocation(name), val);
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
        void bind() const
        {
            if (!_compiled)
                std::cerr << "Attempting to bind non-compiled program.";

            glUseProgram(_id);
        }

        void unbind() const
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
