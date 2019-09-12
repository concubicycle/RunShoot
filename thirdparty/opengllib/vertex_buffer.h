#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include "opengl_afx.h"
#include "program_info.h"
#include "buffer_object.h"

#include <vector>
#include <functional>

template<typename TFormat>
class VertexBuffer : public BufferObject
{
private:
	std::vector<TFormat> _data;

	
protected:
	void onBind() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);		
	}

	void onBuffer() override
	{
		auto byteCount = sizeof(_data.front()) * _data.size();
		glBufferData(GL_ARRAY_BUFFER, byteCount, &_data.front(), GL_STATIC_DRAW);
	}

	void onUnbind() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


public :
	VertexBuffer()
	{
		glGenBuffers(1, &_id);
	}

	~VertexBuffer()
	{
		glDeleteBuffers(1, &_id);
	}
	

	void setAttribPointers(const ProgramInfo& info)
	{
		_array.setAttribPointer(info);
	}
	

	void initData(int size, std::vector<TFormat> data)
	{
		_data = data;	
	}

	std::vector<TFormat>& data()
	{
		return _data;
	}
};

#endif