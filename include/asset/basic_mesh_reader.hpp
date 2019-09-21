#ifndef __BASIC_MESH_READER_H_
#define __BASIC_MESH_READER_H_

#include <nlohmann/json.hpp>
#include <vertex_p.h>
#include <vertex_ptx2d.h>
#include <asset/mesh.hpp>
#include <asset/textured_mesh.hpp>
#include <string>
#include <tl/expected.hpp>
#include <spdlog/spdlog.h>

#include <asset/mesh_read_error.hpp>

using json = nlohmann::json;

namespace readers
{

using mesh_result_p = tl::expected<models::mesh<ogllib::vertex_p>, mesh_read_error>;
using mesh_result_ptx2d = tl::expected<models::textured_mesh<ogllib::vertex_ptx2d>, mesh_read_error>;

class basic_mesh_reader
{
public:
    basic_mesh_reader() {}

    mesh_result_p read_mesh_p(std::string filename);
    mesh_result_ptx2d read_mesh_ptx2d(std::string filename);
};

} // namespace readers

#endif
