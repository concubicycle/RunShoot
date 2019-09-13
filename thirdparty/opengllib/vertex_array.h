#ifndef __VERTEX_ARRAY_H_
#define __VERTEX_ARRAY_H_

#include <glm/vec3.hpp>
#include <opengl_afx.h>
#include <vertex_buffer.h>
#include <index_buffer.h>
#include <vector>

namespace ogllib
{

template <typename TFormat>
class vertex_array
{
private:
	//vertex_buffer<TFormat> _vbo;
	//index_buffer _ebo;

	GLuint _id;

public:
	vertex_array()
	{
		glGenVertexArrays(1, &_id);
	}

	~vertex_array()
	{
		glDeleteVertexArrays(1, &_id);
	}

	// void bufferGlObjects()
	// {
	// 	_vbo.buffer();

	// 	if (!(_ebo.empty()))
	// 		_ebo.buffer();
	// }

	// void bindGlObjects()
	// {
	// 	_vbo.bind();

	// 	if (!(_ebo.empty()))
	// 		_ebo.bind();
	// }

	void bind() const
	{
		glBindVertexArray(_id);
	}

	void unbind() const
	{
		glBindVertexArray(0);
	}

	//const std::vector<TFormat> &vertexData() { return _vbo.data(); }
	//const std::vector<GLuint> &indexData() { return _ebo.data(); }
};

} // namespace ogllib

#endif