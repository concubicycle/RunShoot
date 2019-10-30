//
// Created by sava on 10/25/19.
//

#include <asset/component_loader.hpp>


using bitshift_to_component_loader =
    std::unordered_map<component_shift, std::function<void(const json&, ecs::entity&, string_table&)>>;

void load_transform(const json &j, ecs::entity &e, string_table& hashes)
{
    auto &transform = e.get_component<ecs::transform_component>();
    transform.pos.x = j["x"].get<float>();
    transform.pos.y = j["y"].get<float>();
    transform.pos.z = j["z"].get<float>();
    transform.pitch = j["pitch"].get<float>();
    transform.yaw = j["yaw"].get<float>();
    transform.roll = j["roll"].get<float>();
    transform.scale_x = j["scale_x"].get<float>();
    transform.scale_y = j["scale_y"].get<float>();
    transform.scale_z = j["scale_z"].get<float>();
}

void load_render(const json &j, ecs::entity &e, string_table& hashes)
{
    auto &r = e.get_component<ecs::render_component_old>();
    std::string path = j["mesh_path"].get<std::string>();

    assert (path.length() < 255);

    memcpy(r.mesh_path, path.c_str(), path.length());
    r.mesh_path[path.length()] = '\0';

    r.mesh_format = asset::mesh_type(j["mesh_format"].get<unsigned int>());
}

void load_render_ogl(const json &j, ecs::entity &e, string_table& hashes)
{
    auto &r = e.get_component<ecs::render_component_ogl>();
    std::string path = j["mesh_path"].get<std::string>();

    r.mesh_path_hash = hashes.hash_and_store(path);
    r.mesh_format = asset::mesh_type(j["mesh_format"].get<unsigned int>());
}


void load_camera(const json &j, ecs::entity &e, string_table& hashes)
{
    auto &c = e.get_component<ecs::camera_component>();
    c.position.x = j["position"]["x"].get<float>();
    c.position.y = j["position"]["y"].get<float>();
    c.position.z = j["position"]["z"].get<float>();

    c.pitch = j["pitch"].get<float>();
    c.yaw = j["yaw"].get<float>();
    c.roll = j["roll"].get<float>();

    c.fov = j["fov"].get<float>();
    c.near = j["near"].get<float>();
    c.far = j["far"].get<float>();

    c.mode = (ecs::camera_component::camera_mode) j["mode"].get<int>();
}

void load_aabbs(const json &j, ecs::entity &e, string_table& hashes)
{
    auto &a = e.get_component<ecs::aabb_component>();
    auto colliders = j["colliders"];

    std::uint32_t count = 0;

    for(auto& aabb : colliders)
    {
        auto min = aabb["min"];
        auto max = aabb["max"];

        a.colliders[count++] =  {
            glm::vec3(min[0].get<float>(), min[1].get<float>(), min[2].get<float>()),
            glm::vec3(max[0].get<float>(), max[1].get<float>(), max[2].get<float>()),
        };
    }
}

void load_physical_properties(const json &j, ecs::entity &e, string_table& hashes)
{
    auto &p = e.get_component<ecs::physical_properties_component>();
    p.velocity = glm::vec3(
        j["velocity"][0].get<float>(),
        j["velocity"][1].get<float>(),
        j["velocity"][2].get<float>());
}


bitshift_to_component_loader asset::component_loader::loader_functions
{
    { ecs::transform_component::component_bitshift, load_transform },
    { ecs::render_component_old::component_bitshift, load_render },
    { ecs::camera_component::component_bitshift, load_camera },
    { ecs::render_component_ogl::component_bitshift, load_render_ogl },
    { ecs::aabb_component::component_bitshift, load_aabbs },
    { ecs::physical_properties_component::component_bitshift, load_physical_properties },
};


asset::component_loader::component_loader(string_table &string_table) : _string_table(string_table)
{}

void
asset::component_loader::load_component(
    ecs::entity &e,
    const json &j,
    component_shift component_bitshift) const
{
    assert(loader_functions.find(component_bitshift) != loader_functions.end());

    auto it = loader_functions.find(component_bitshift);
    it->second(j, e, _string_table);
}

