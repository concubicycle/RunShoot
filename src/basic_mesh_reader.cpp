#include "asset/basic_mesh_reader.hpp"

#include <fstream>

models::mesh<ogllib::vertex_p> readers::basic_mesh_reader::read_mesh_p(std::string filename)
{
    models::mesh<ogllib::vertex_p> mesh;

    std::ifstream input_stream(filename);
    json j;
    input_stream >> j;

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
