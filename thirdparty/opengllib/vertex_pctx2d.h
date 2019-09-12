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

#ifndef VERTEX_PCTEX_
#define VERTEX_PCTEX_

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

class VertexPCTx2d
{
public:
	VertexPCTx2d(
		float x = 0, float y = 0, float z = 0,
		float r = 0, float g = 0, float b = 0, float a = 0,
		float u = 0, float v = 0)
		: position(x, y, z), color(r, g, b, a), textureCoordinates2d(u, v) {}


	glm::vec3 position;
	glm::vec4 color; 
	glm::vec2 textureCoordinates2d;
};



#endif