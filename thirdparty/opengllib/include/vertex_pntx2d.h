/*
Naming:

Suffix		Type			Name				Semantic
--------------------------------------------------------
p			vec3			position			position
c			vec4			rgba				color_diffuse
n			vec3			normal			    normal
tx2d		vec2			texcoords_2d		texcoords_2d
tx3d		vec3			texcoords_3d		texcoords_3d
...

*/
#ifndef __VERTEX_PNTX2D_H_
#define __VERTEX_PNTX2D_H_

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace ogllib
{
    struct vertex_pntx2d
    {
        vertex_pntx2d(
            float x = 0, float y = 0, float z = 0,
            float nx = 0, float ny = 0, float nz = 0,
            float u = 0, float v = 0)
            : position(x, y, z), normal(nx, ny, nz), textureCoordinates2d(u, v) {}

        vertex_pntx2d(
            glm::vec3 pos,
            glm::vec3 n,
            glm::vec2 tex)
            : position(pos), normal(n), textureCoordinates2d(tex) {}

        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinates2d;
    };

} // namespace ogllib

#endif //__VERTEX_PNTX2D_H_
