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

#ifndef VERTEX_PTEX_
#define VERTEX_PTEX_

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace ogllib
{

struct vertex_ptx2d
{
	vertex_ptx2d(
		float x = 0, float y = 0, float z = 0,
		float u = 0, float v = 0)
		: position(x, y, z), textureCoordinates2d(u, v) {}

	glm::vec3 position;
	glm::vec2 textureCoordinates2d;
};

} // namespace ogllib

#endif