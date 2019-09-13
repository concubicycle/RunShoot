#ifndef __MESH_H_
#define __MESH_H_

#include <vector>
#include <cstdint>

namespace models
{

template <typename TVertex>
class mesh
{
public:
    mesh() {}

    std::vector<TVertex> vertices;
    std::vector<std::uint32_t> indices;
};

} // namespace models

#endif
