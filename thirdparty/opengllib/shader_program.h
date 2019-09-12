#ifndef SHADER_PROGRAM_H__
#define SHADER_PROGRAM_H__

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <memory>

#include "opengl_afx.h"
#include "program_info.h"
#include "vertex_buffer.h"
#include "shader.h"
#include "vertex_array.h"
#include "logger.h"

#include "shader_program_uniform_assign.h"



template <typename TVertexFromat>
class ShaderProgram  
{

private:	
	el::Logger& _logger = *(el::Loggers::getLogger("default"));

	ProgramInfo _info;
	std::vector<ShaderBase*> _shaders;

	unsigned int _id; // The shader program identifier
	bool _compiled; // Whether or not we have initialized the shader
	

	void validateProgram()
	{
		const unsigned int BUFFER_SIZE = 512;
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		GLsizei length = 0;
		GLint success;

		glGetProgramiv(_id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(_id, BUFFER_SIZE, &length, buffer); // Ask OpenGL to give us the log associated with the program		
			_logger.error("Error linking program %i. Link error: %i \n", _id, buffer);
		}

		glValidateProgram(_id); // Get OpenGL to try validating the program
		GLint status;

		glGetProgramiv(_id, GL_VALIDATE_STATUS, &status); // Find out if the shader program validated correctly
		if (status == GL_FALSE) // If there was a problem validating
			_logger.error("Error validating program %i \n.", _id);
		else
			_logger.info("Shader program %i built successfully.", _id);
	}

	template<typename TUniform>
	static Uniforms<ShaderProgram<TVertexFromat>, TUniform>& getUniforms(ProgramInfo& info)
	{
		static  Uniforms<ShaderProgram<TVertexFromat>, TUniform> proxy(info); //swag        
		return proxy;
	}

	// must be implemented for each vertex format TVertexFromat
	void setAttribPointers()
	{
		static_assert(false, "Please define a setAttribPointer() implementation for " + typeid(TVertexFromat).name());
	}

public:
		

	ShaderProgram(ShaderBase* vertexShader, ShaderBase* fragmentShader)
	{
		_id = glCreateProgram(); // Create a GLSL program		

		addShader(vertexShader);
		addShader(fragmentShader);
	}
	

	~ShaderProgram()
	{
		for (auto& shader : _shaders)
			glDetachShader(_id, shader->getId());
				
		glDeleteProgram(_id); // Delete the shader program
	}
	
	template<typename TUniform>
	Uniforms<ShaderProgram<TVertexFromat>, TUniform> getUniforms()
	{
		return getUniforms<TUniform>(_info);
	}

	template<typename TUniform>
	void setUniform(const std::string& name, TUniform value)
	{
		///TODO #1: 
		getUniforms()[name] = value; 
	}




	//COMPILATION
	void addShader(ShaderBase* shader)
	{		
		_shaders.push_back(shader);
	}

	void compile()
	{
		if (_compiled) // If we have already initialized the shader
			return;

		// compile shaders
		for (auto& shader : _shaders)
			shader->compile();

		// attach shaders
		for (auto& shader : _shaders)
			glAttachShader(_id, shader->getId());

		glLinkProgram(_id); // Link the vertex and fragment shaders in the program
		validateProgram(); // Validate the shader program

		_info.initialize(_id);

		_compiled = true; // Mark that we have initialized the shader
	}	

	// RENDERING
	void setupVao(VertexArray<TVertexFromat>& vao)
	{
		vao.bind();
		vao.bindGlObjects();
		vao.bufferGlObjects();

		setAttribPointers();

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
	

	// GETTERS/SETTERS
	unsigned int getId() { return _id; }
	ProgramInfo& getInfo() { return _info; }
};





#include "shader_program_specializations.h"

#endif
