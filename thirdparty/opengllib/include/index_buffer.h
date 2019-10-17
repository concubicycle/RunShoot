#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <vector>

#include <opengl_afx.h>
#include "buffer_object.h"

namespace ogllib
{

// Or "element" onBuffer
class index_buffer : public ogllib::buffer_object
{
private:
	const std::vector<GLuint> &_data;

protected:
	void onBind() override
	{
		gl::glBindBuffer(gl::GLenum::GL_ELEMENT_ARRAY_BUFFER, _id);
	}

	void onBuffer() override
	{
		auto byteCount = sizeof(_data.front()) * _data.size();
		gl::glBufferData(gl::GLenum::GL_ELEMENT_ARRAY_BUFFER, byteCount, &_data.front(), gl::GLenum::GL_STATIC_DRAW);
	}

	void onUnbind() override
	{
		gl::glBindBuffer(gl::GLenum::GL_ELEMENT_ARRAY_BUFFER, 0);
	}

public:
	index_buffer(std::vector<GLuint> const &data) : _data(data)
	{
		gl::glGenBuffers(1, &_id);
	}

	~index_buffer()
	{
		gl::glDeleteBuffers(1, &_id);
	}

	void generate()
	{
		gl::glGenBuffers(1, &_id);
	}

	const std::vector<GLuint> &data()
	{
		return _data;
	}

	bool empty() const { return _data.empty(); }
};

} // namespace ogllib
#endif
