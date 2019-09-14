/*
Naming:

Suffix		Type			Name				Semantic
--------------------------------------------------------
p			vec3			position			position
c			vec4			rgba				color_diffuse
n			vec3			normal			    normal
tx2d		vec2			texcoords_2d		texture_coordinates_2d
tx3d		vec3			texcoords_3d		texture_coordinates_3d
...

*/

#ifndef VERTEX_PC_
#define VERTEX_PC_

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "opengl_afx.h"

namespace ogllib
{

struct vertex_pc
{
public:
	vertex_pc(float x = 0, float y = 0, float z = 0,
			  float r = 0, float g = 0, float b = 0, float a = 1.0f)
		: position(x, y, z), color(r, g, b, a) {}

	glm::vec3 position;
	glm::vec4 color;
};

} // namespace ogllib
#endif