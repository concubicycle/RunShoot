#ifndef __MESH_H_
#define __MESH_H_

#include <vector>

namespace models
{
template <typename TVertex>
class mesh
{
public:
    const std::vector<TVertex> &vertices() { return _verties; }

private:
    std::vector<TVertex> _verties
};
} // namespace models

#endif
