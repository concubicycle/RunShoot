#ifndef __SHADER_PROGRAM_UNIFORM_ASIGN_H__
#define __SHADER_PROGRAM_UNIFORM_ASIGN_H__

#include <opengl_afx.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <program_info.h>

using namespace gl;

namespace ogllib
{

// Proxy for asigning OpenGL GLSL uniforms (GPU program global variables)
template <typename T>
struct AssignmentProxy
{

	AssignmentProxy(int location) : _location(location) {}

	void operator=(T &rhs);

private:
	int _location;
};

template <>
void ogllib::AssignmentProxy<glm::mat4>::operator=(glm::mat4 &val);

template <>
void ogllib::AssignmentProxy<GLfloat>::operator=(GLfloat &val);

template <>
void ogllib::AssignmentProxy<glm::vec3>::operator=(glm::vec3 &val);



// A proxy class that implements [], and returns corresponding assignment proxy.
// Allows user to do:
//     Uniforms<GLuint> uniforms = shaderProgram.get_uniforms<GLuint>();
//     uniforms["uniformName"] = value;
template <class TProgram, typename TUniform>
class Uniforms
{
	friend TProgram;

private:
	program_info &_info;

	Uniforms(program_info &info)
		: _info(info)
	{
	}

public:
	AssignmentProxy<TUniform> operator[](const std::string &uniformName) const
	{
		auto uniformLocation = _info.getUniformLocation(uniformName);
		AssignmentProxy<TUniform> assignProxy(uniformLocation);
		return assignProxy;
	}
};
   
} // namespace ogllib

#endif