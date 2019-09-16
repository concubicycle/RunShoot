#include <asset/basic_mesh_reader.hpp>
#include <fstream>

bool exists(const std::string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

using mesh_result = tl::expected<models::mesh<ogllib::vertex_p>, readers::mesh_read_error>;

mesh_result readers::basic_mesh_reader::read_mesh_p(std::string filename)
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
