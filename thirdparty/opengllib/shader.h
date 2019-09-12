//==============================================================================
// perfVis performance visualisation framework
//
// Copyright (c) 2014-2016 RWTH Aachen University, Germany,
// Virtual Reality & Immersive Visualisation Group.
//==============================================================================
//                                License
//
// This framework is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// In the future, we may decide to add a commercial license
// at our own discretion without further notice.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//==============================================================================

#ifndef PV_SHADER__H_
#define PV_SHADER__H_

#include <fstream>
#include <iostream>

#include "opengl_afx.h"

class ShaderBase
{
public:
	virtual GLuint getId() const = 0;
	virtual void compile() = 0;
	virtual bool isCompiled() const = 0;
};

template<typename FileReadPolicy>
class Shader : private FileReadPolicy, public ShaderBase
{
	using FileReadPolicy::readFile;

private:
	GLuint _id;
	const GLenum _type; 
	bool _isCompiled = false;

	std::string _fileName;

	/**
	Given a shader and the filename associated with it, validateShader will
	then get information from OpenGl on whether or not the shader was compiled successfully
	and if it wasn't, it will output the file with the problem, as well as the problem.
	*/
	void validateShader() const
	{
		const unsigned int BUFFER_SIZE = 512;
		char buffer[BUFFER_SIZE];

		memset(buffer, 0, BUFFER_SIZE);
		GLsizei length = 0;

		glGetShaderInfoLog(_id, BUFFER_SIZE, &length, buffer); // Ask OpenGL to give us the log associated with the shader

		if (length > 0) // If we have any information to display
			std::cerr << "Shader " << _id << " (" << _fileName << ") compile error: " << buffer << std::endl; // Output the information
	}

	

public:
	Shader(GLenum type)
		: _type(type)
	{		
		_id = glCreateShader(_type);
	}

	Shader(GLenum type, const char* fname) : Shader(type)
	{		
		fromFile(fname);
	}
		

	~Shader()
	{
		glDeleteShader(_id);
	}

	void fromFile(const char* fname)
	{		
		_fileName = std::string(fname);
		std::string shader_string = readFile(fname);

		if (shader_string.empty())
			std::cerr << "Failed to compile shader: " << fname << std::endl;

		const char *c_str = shader_string.c_str();
		glShaderSource(_id, 1, &c_str, nullptr);		
	}

	void fromString(const std::string& shader_string)
	{
		_filename = "NOT FROM FILE";		
		const char *c_str = shader_string.c_str();
		glShaderSource(_id, 1, &c_str, nullptr);
	}

	void compile() override
	{
		if (_isCompiled) return;

		glCompileShader(_id);
		validateShader();
		_isCompiled = true;
	}


	GLuint getId() const override { return _id; }
	GLenum getType() const { return _type; }
	bool isCompiled() const override{ return _isCompiled; }
};

#endif
