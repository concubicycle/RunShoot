/*
Naming:

Suffix		Type			Name				Semantic
--------------------------------------------------------
P			vec3			position			position
C			vec4			rgba				color_diffuse
N			vec3			normal			normal
Tx2d		vec2			texcoords_2d		texture_coordinates_2d
Tx3d		vec3			texcoords_3d		texture_coordinates_3d
...

*/


#ifndef VERTEX_PC_
#define VERTEX_PC_

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "opengl_afx.h"

class VertexPC
{
public:
	VertexPC(float x = 0, float y = 0, float z = 0,
		float r = 0, float g = 0, float b = 0, float a = 1.0f)
		: position(x, y, z), color(r, g, b, a) {}

	glm::vec3 position;
	glm::vec4 color;
};



#endif