#ifndef __BASIC_MESH_READER_H_
#define __BASIC_MESH_READER_H_

#include <nlohmann/json.hpp>
#include <vertex_p.h>
#include <asset/mesh.hpp>
#include <string>

using json = nlohmann::json;

namespace readers
{

class basic_mesh_reader
{
public:
    basic_mesh_reader() {}

    models::mesh<ogllib::vertex_p> read_mesh_p(std::string filename);
};

} // namespace readers

#endif
