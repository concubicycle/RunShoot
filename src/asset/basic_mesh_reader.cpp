#include <asset/basic_mesh_reader.hpp>
#include <fstream>

bool exists(const std::string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

using mesh_result_p = tl::expected<models::mesh<ogllib::vertex_p>, asset::mesh_read_error>;
using mesh_result_ptx2d = tl::expected<models::textured_mesh<ogllib::vertex_ptx2d>, asset::mesh_read_error>;

mesh_result_p asset::basic_mesh_reader::read_mesh_p(std::string filename) const
{
    models::mesh<ogllib::vertex_p> mesh;

    std::ifstream input_stream(filename);
    json j;
    try
    {
        j = json::parse(input_stream);
    }
    catch (json::parse_error &e)
    {
        spdlog::error("{0}\n", e.what());
        return tl::unexpected<mesh_read_error>(mesh_read_error::BadFile);
    }

    for (auto v : j["vertices"])
    {
        mesh.vertices.push_back(ogllib::vertex_p(
            v["x"].get<float>(),
            v["y"].get<float>(),
            v["z"].get<float>()));
    }

    for (auto i : j["indices"])
    {
        mesh.indices.emplace_back(i.get<std::uint32_t>());
    }

    return mesh;
}

mesh_result_ptx2d asset::basic_mesh_reader::read_mesh_ptx2d(std::string filename) const
{
    models::textured_mesh<ogllib::vertex_ptx2d> mesh;

    std::ifstream input_stream(filename);
    json j;
    try
    {
		input_stream >> j;
    }
    catch (json::parse_error &e)
    {
        spdlog::error("{0}\n", e.what());
        return tl::unexpected<mesh_read_error>(mesh_read_error::BadFile);
    }

    for (auto v : j["vertices"])
    {
        mesh.mesh_data.vertices.push_back(ogllib::vertex_ptx2d(
            v["x"].get<float>(),
            v["y"].get<float>(),
            v["z"].get<float>(),
            v["u"].get<float>(),
            v["v"].get<float>()));
    }

    for (auto i : j["indices"])
    {
        mesh.mesh_data.indices.emplace_back(i.get<std::uint32_t>());
    }

    mesh.texture_filename = j["texture"].get<std::string>();

    return mesh;
}

