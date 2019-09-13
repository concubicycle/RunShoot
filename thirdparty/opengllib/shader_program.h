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

namespace ogllib
{

template <typename TVertexFromat>
class shader_program
{
	static constexpr const unsigned int GPU_INFOLOG_BUFFER_SIZE = 512;

private:
	ogllib::program_info _info;
	std::vector<shader_base *> _shaders;

	unsigned int _id = -1;  // The shader program identifier
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
		if (validate_status == GL_FALSE)
		{
			spdlog::error("Error validating program {0:d} \n.", _id);
		}
		else
		{
			glGetProgramInfoLog(_id, GPU_INFOLOG_BUFFER_SIZE, &length, buffer);
			spdlog::info("Shader program {0:d} built successfully. Info log: {1}", _id, buffer);
		}
	}

	template <typename TUniform>
	static Uniforms<shader_program<TVertexFromat>, TUniform> &getUniforms(program_info &info)
	{
		static Uniforms<shader_program<TVertexFromat>, TUniform> proxy(info); //swag
		return proxy;
	}

public:
	shader_program(shader_base *vertex_shader, shader_base *fragment_shader)
	{
		_id = glCreateProgram(); // Create a GLSL program

		addShader(vertex_shader);
		addShader(fragment_shader);
	}

	~shader_program()
	{
		for (auto &shader : _shaders)
			glDetachShader(_id, shader->id());

		glDeleteProgram(_id); // Delete the shader program
	}

	template <typename TUniform>
	Uniforms<shader_program<TVertexFromat>, TUniform> getUniforms()
	{
		return getUniforms<TUniform>(_info);
	}

	template <typename TUniform>
	void setUniform(const std::string &name, TUniform value)
	{
		getUniforms()[name] = value;
	}

	//COMPILATION
	void addShader(shader_base *shader)
	{
		_shaders.push_back(shader);
	}

	void compile()
	{
		if (_compiled)
			return;

		// compile shaders
		for (auto shader : _shaders)
			shader->compile();

		// attach shaders
		for (auto shader : _shaders)
			glAttachShader(_id, shader->id());

		glLinkProgram(_id);
		validate_program();

		_info.initialize(_id);

		_compiled = true;
	}

	// RENDERING
	void setupVao(vertex_array<TVertexFromat> &vao)
	{
		vao.bind();
		vao.bindGlObjects();
		vao.bufferGlObjects();

		set_attrib_pointers();

		vao.unbind();
	}

	// BINDING
	void bind()
	{
		if (!_compiled)
			std::cerr << "Attemtping to bind uncompiled program.";

		glUseProgram(_id);
	}

	void unbind()
	{
		glUseProgram(0);
	}

	// must be implemented for each vertex format TVertexFromat
	void set_attrib_pointers()
	{
		static_assert(true, "Please define a setAttribPointer() implementation for all TVertexFormats");
		//	static_assert(false, "Please define a setAttribPointer() implementation for " + typeid(TVertexFromat).name());
	}

	// GETTERS/SETTERS
	unsigned int getId() { return _id; }
	program_info &getInfo() { return _info; }
};

} // namespace ogllib
#endif
