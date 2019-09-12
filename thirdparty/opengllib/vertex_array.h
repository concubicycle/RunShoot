#ifndef VERTEX_BINDER_H
#define VERTEX_BINDER_H

#include "glm/vec3.hpp"
#include "program_info.h"
#include "opengl_afx.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include <vector>

template <typename TFormat>
class VertexArray
{
private:

	VertexBuffer<TFormat> _vbo; //vertex onBuffer object
	IndexBuffer _ebo; //element onBuffer object

	GLuint _id;
	

public:
	VertexArray() 
	{
		glGenVertexArrays(1, &_id);
	}	

	~VertexArray()
	{
		glDeleteVertexArrays(1, &_id);
	}	

	void bufferGlObjects()
	{		
		_vbo.buffer();

		if(!(_ebo.empty()))
			_ebo.buffer();
	}

	void bindGlObjects()
	{
		_vbo.bind();

		if(!(_ebo.empty()))
			_ebo.bind();
	}

	void bind() const
	{
		glBindVertexArray(_id);			
	}

	void unbind() const
	{
		glBindVertexArray(0);
	}

	std::vector<TFormat>& vertexData() { return _vbo.data(); }
	std::vector<GLuint>& indexData() { return _ebo.data(); }
};

#endif