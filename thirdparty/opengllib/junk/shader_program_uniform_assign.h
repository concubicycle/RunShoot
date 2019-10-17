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

	void operator=(T &rhs)
	{
		//static_assert(false, "Undefined uniform asignment");
	}

private:
	int _location;
};

// A proxy class that implements [], and returns corresponding assignment proxy.
// Allows user to do:
//     Uniforms<GLuint> uniforms = shaderProgram.getUniforms<GLuint>();
//     uniforms["uniformName"] = value;
template <class TProgram, typename TUniform>
class Uniforms
{
	friend TProgram;

private:
	const program_info &_info;

	Uniforms(const program_info &info)
		: _info(info)
	{
	}

public:
	AssignmentProxy<TUniform> operator[](const std::string &uniformName) const
	{
		auto uniformLocation = _info.getUniformLocation(uniformName);

		// todo: optimize so we only have one instance os AssignmentProxy per type
		AssignmentProxy<TUniform> assignProxy(uniformLocation);

		return assignProxy;
	}
};

} // namespace ogllib

#endif