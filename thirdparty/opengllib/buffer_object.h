#ifndef __BUFFER_OBJECT_H_
#define __BUFFER_OBJECT_H_

#include <opengl_afx.h>

namespace ogllib
{

class buffer_object
{
private:
	// NOTE: does not guarantee that object is currently bound, only
	// that it has been bound in the past.
	bool _has_been_bound = false;
	bool _has_been_buffered = false;

protected:
	GLuint _id;

	virtual void onBuffer() = 0;
	virtual void onBind() = 0;
	virtual void onUnbind() = 0;

public:
	void bind()
	{
		if (!_has_been_bound)
			onBind();

		_has_been_bound = true;
	}

	void unbind()
	{
		onUnbind();
		_has_been_bound = false;
	}

	void buffer()
	{
		if (!_has_been_buffered)
			onBuffer();

		_has_been_buffered = true;
	}

	bool is_bound() const { return _has_been_bound; }
	bool is_buffered() const { return _has_been_buffered; }
};

} // namespace ogllib

#endif
