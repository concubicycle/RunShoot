#ifndef __TEXTURED_MESH_H_
#define __TEXTURED_MESH_H_

#include <asset/mesh.hpp>
#include <string>

namespace models
{
template <typename TVertex>
struct textured_mesh
{
    mesh<TVertex> mesh_data;
    std::string texture_filename;
};
} // namespace models

#endif
