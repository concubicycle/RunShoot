#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include <opengl_afx.h>
#include <buffer_object.h>

#include <vector>
#include <functional>

namespace ogllib
{

template <typename TFormat>
class vertex_buffer : public buffer_object
{
private:
	const std::vector<TFormat> &_data;

protected:
	void onBind() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	void onBuffer()
	{
		auto byteCount = sizeof(_data.front()) * _data.size();
		glBufferData(GL_ARRAY_BUFFER, byteCount, &_data.front(), GL_STATIC_DRAW);
	}

	void onUnbind() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

public:
	vertex_buffer(const std::vector<TFormat> &data) : _data(data)
	{
		glGenBuffers(1, &_id);
	}

	~vertex_buffer()
	{
		glDeleteBuffers(1, &_id);
	}

	std::vector<TFormat> &data()
	{
		return _data;
	}
};

} // namespace ogllib

#endif