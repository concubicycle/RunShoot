#ifndef __PV_SHADER__H_
#define __PV_SHADER__H_

#include <fstream>
#include <iostream>

#include "opengl_afx.h"
#include <spdlog/spdlog.h>

using namespace gl;

namespace ogllib
{
class shader_base
{
public:
	virtual GLuint id() const = 0;
	virtual void compile() = 0;
	virtual bool is_compiled() const = 0;
};

template <typename FileReadPolicy>
class shader : private FileReadPolicy, public shader_base
{
	static constexpr const unsigned int GPU_INFOLOG_BUFFER_SIZE = 512;

	using FileReadPolicy::readFile;

private:
	GLuint _id;
	const GLenum _type;
	bool _is_compiled = false;

	std::string _filename;

	void validate_shader()
	{
		GLsizei length = 0;
		char buffer[GPU_INFOLOG_BUFFER_SIZE];

		memset(buffer, 0, GPU_INFOLOG_BUFFER_SIZE);
		glGetShaderInfoLog(_id, GPU_INFOLOG_BUFFER_SIZE, &length, buffer);

		if (length > 0)
		{
			spdlog::error(buffer);
		}
		else
		{
			_is_compiled = true;
			spdlog::info("Shader {0} ({1:d}) compiled without issue.", _filename, _id);
		}
	}

public:
	shader(gl::GLenum type)
		: _type(type)
	{
		_id = glCreateShader(_type);
	}

	shader(gl::GLenum type, const char *fname) : shader(type)
	{
		from_file(fname);
	}

	~shader()
	{
		glDeleteShader(_id);
	}

	void from_file(const char *fname)
	{
		_filename = std::string(fname);
		std::string shader_string = readFile(fname);

		if (shader_string.empty())
			spdlog::error("Failed to compile shader {0}.", fname);

		const char *c_str = shader_string.c_str();
		glShaderSource(_id, 1, &c_str, nullptr);
	}

	void from_string(const std::string &shader_string)
	{
		_filename = "NOT FROM FILE";
		const char *c_str = shader_string.c_str();
		glShaderSource(_id, 1, &c_str, nullptr);
	}

	void compile() override
	{
		if (_is_compiled)
			if (_is_compiled)
				return;

		glCompileShader(_id);
		validate_shader();
	}

	GLuint id() const override { return _id; }
	GLenum type() const { return _type; }
	bool is_compiled() const override { return _is_compiled; }
};

} // namespace ogllib
#endif
