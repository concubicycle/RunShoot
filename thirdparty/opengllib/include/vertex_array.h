#ifndef __VERTEX_ARRAY_H_
#define __VERTEX_ARRAY_H_

#include <glm/vec3.hpp>
#include <opengl_afx.h>
#include <vertex_buffer.h>
#include <index_buffer.h>
#include <vector>


namespace ogllib
{


class vertex_array
{
private:
	GLuint _id;

public:

	void generate()
    {
        glGenVertexArrays(1, &_id);
    }

    void delete_vao()
    {
	    glDeleteVertexArrays(1, &_id);
    }

	void bind() const
	{
		glBindVertexArray(_id);
	}

	void unbind() const
	{
		glBindVertexArray(0);
	}

	GLuint id() { return _id; }
};

} // namespace ogllib

#endif