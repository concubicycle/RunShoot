#include "asset/basic_mesh_reader.hpp"

#include <fstream>
#include <spdlog/spdlog.h>

inline bool exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);

}

models::mesh<ogllib::vertex_p> readers::basic_mesh_reader::read_mesh_p(std::string filename)
{
    models::mesh<ogllib::vertex_p> mesh;

    std::fstream input_stream(filename);
    json j;	
	
	try {
		j << input_stream;
	}
	catch (json::parse_error& e)
	{
		spdlog::error("{0}\n", e.what());
		throw;
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
