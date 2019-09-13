/*
Naming:

Suffix		Type			Name				Semantic
--------------------------------------------------------
P			vec3			position			position
C			vec4			rgba				color_diffuse
N			vec3			normal				normal
Tx2d		vec2			texcoords_2d		texture_coordinates_2d
Tx3d		vec3			texcoords_3d		texture_coordinates_3d
...

*/

#ifndef VERTEX_P_
#define VERTEX_P_

#include "glm/vec3.hpp"
#include "opengl_afx.h"

namespace ogllib
{

struct vertex_p
{
	vertex_p(float x = 0, float y = 0, float z = 0)
		: position(x, y, z) {}

	glm::vec3 position;
};

} // namespace ogllib

#endif