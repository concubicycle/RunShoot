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

#ifndef VERTEX_PCTEX_
#define VERTEX_PCTEX_

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace ogllib
{

struct vertex_pctx2d
{
	vertex_pctx2d(
		float x = 0, float y = 0, float z = 0,
		float r = 0, float g = 0, float b = 0, float a = 0,
		float u = 0, float v = 0)
		: position(x, y, z), color(r, g, b, a), textureCoordinates2d(u, v) {}

	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 textureCoordinates2d;
};

} // namespace ogllib

#endif