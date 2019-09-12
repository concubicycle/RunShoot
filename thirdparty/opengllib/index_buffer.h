#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <vector>

#include <opengllib/opengl_afx.h>
#include "buffer_object.h"

// Or "element" onBuffer
class IndexBuffer : public BufferObject
{
private:
	std::vector<GLuint> _data;

protected:
	void onBind() override
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}

	void onBuffer() override
	{
		auto byteCount = sizeof(_data.front()) * _data.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteCount, &_data.front(), GL_STATIC_DRAW);
	}

	void onUnbind() override
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

public:
	IndexBuffer()
	{
		glGenBuffers(1, &_id);
	}

	~IndexBuffer()
	{
		glDeleteBuffers(1, &_id);
	}

	void generate()
	{
		glGenBuffers(1, &_id);
	}

	void initData(int size, std::vector<GLuint> data)
	{
		_data = data;
	}

	std::vector<GLuint> &data()
	{
		return _data;
	}

	bool empty() const { return _data.empty(); }
};

#endif
