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

#include "vertex_p.h"
#include "vertex_pc.h"
#include "vertex_pctx2d.h"
#include "vertex_ptx2d.h"

#include "shader_program.h"

namespace ogllib
{
template <>
void shader_program<ogllib::vertex_p>::set_attrib_pointers()
{
	auto index = _info.getAttribLocation("position");
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(index);
}

template <>
void shader_program<VertexPC>::set_attrib_pointers()
{
	auto posIndex = _info.getAttribLocation("position");
	auto colIndex = _info.getAttribLocation("rgba");

	size_t posOffset = offsetof(VertexPC, position);
	size_t colOffset = offsetof(VertexPC, color);

	glVertexAttribPointer(posIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), reinterpret_cast<void *>(posOffset));
	glEnableVertexAttribArray(posIndex);

	glVertexAttribPointer(colIndex, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPC), reinterpret_cast<void *>(colOffset));
	glEnableVertexAttribArray(colIndex);
}

template <>
void shader_program<VertexPTx2d>::set_attrib_pointers()
{
	auto posIndex = _info.getAttribLocation("position");
	auto txIndex = _info.getAttribLocation("texcoords_2d");

	size_t posOffset = offsetof(VertexPTx2d, position);
	size_t txOffset = offsetof(VertexPTx2d, textureCoordinates2d);

	glVertexAttribPointer(posIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPTx2d), reinterpret_cast<void *>(posOffset));
	glEnableVertexAttribArray(posIndex);

	glVertexAttribPointer(txIndex, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTx2d), reinterpret_cast<void *>(txOffset));
	glEnableVertexAttribArray(txIndex);
}

template <>
void shader_program<VertexPCTx2d>::set_attrib_pointers()
{
	auto posIndex = _info.getAttribLocation("position");
	auto colIndex = _info.getAttribLocation("rgba");
	auto txIndex = _info.getAttribLocation("texcoords_2d");

	size_t posOffset = offsetof(VertexPCTx2d, position);
	size_t colOffset = offsetof(VertexPCTx2d, color);
	size_t txOffset = offsetof(VertexPCTx2d, textureCoordinates2d);

	glVertexAttribPointer(posIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPCTx2d), reinterpret_cast<void *>(posOffset));
	glEnableVertexAttribArray(posIndex);

	glVertexAttribPointer(colIndex, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPCTx2d), reinterpret_cast<void *>(colOffset));
	glEnableVertexAttribArray(colIndex);

	glVertexAttribPointer(txIndex, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPCTx2d), reinterpret_cast<void *>(txOffset));
	glEnableVertexAttribArray(txIndex);
}

} // namespace ogllib