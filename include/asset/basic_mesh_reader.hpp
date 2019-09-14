#ifndef __BASIC_MESH_READER_H_
#define __BASIC_MESH_READER_H_

#include <nlohmann/json.hpp>
#include <vertex_p.h>
#include <asset/mesh.hpp>
#include <string>
#include <tl/expected.hpp>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

namespace readers
{
enum mesh_read_error
{
    BadFile
};

using mesh_result = tl::expected<models::mesh<ogllib::vertex_p>, mesh_read_error>;

class basic_mesh_reader
{
public:
    basic_mesh_reader() {}

    tl::expected<models::mesh<ogllib::vertex_p>, mesh_read_error> read_mesh_p(std::string filename);
};

} // namespace readers

#endif
