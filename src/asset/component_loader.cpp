//
// Created by sava on 10/25/19.
//

#include <asset/component_loader.hpp>
#include <spdlog/spdlog.h>


using bitshift_to_component_loader =
std::unordered_map<component_shift, std::function<void(const json &, ecs::entity &, string_table &)>>;

void load_transform(const json &j, ecs::entity &e, string_table &hashes)
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

void load_render_ogl(const json &j, ecs::entity &e, string_table &hashes)
{
    auto &r = e.get_component<ecs::render_component_ogl>();
    auto path = j["mesh_path"].get<std::string>();

    r.mesh_path_hash = hashes.hash_and_store(path);
    r.mesh_format = asset::mesh_type(j["mesh_format"].get<unsigned int>());

    if (j.find("shader") != j.end())
        r.shader.emplace(j["shader"].get<std::string>());

    if (j.find("billboard") != j.end())
        r.billboard = j["billboard"].get<bool>();

    if (j.find("hue") != j.end())
        r.hue.emplace(
            j["hue"][0].get<float>(),
            j["hue"][1].get<float>(),
            j["hue"][2].get<float>());
}

#undef near
#undef far
void load_camera(const json &j, ecs::entity &e, string_table &hashes)
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

    c.skybox_left = j["skybox_left"].get<std::string>();
    c.skybox_top = j["skybox_top"].get<std::string>();
    c.skybox_front = j["skybox_front"].get<std::string>();
    c.skybox_bottom = j["skybox_bottom"].get<std::string>();
    c.skybox_right = j["skybox_right"].get<std::string>();
    c.skybox_back = j["skybox_back"].get<std::string>();

    c.mode = (ecs::camera_component::camera_mode) j["mode"].get<int>();
}

void load_punctual_light(const json &j, ecs::entity &e, string_table &hashes)
{
    auto &l = e.get_component<ecs::punctual_light_component>();

    l.color = {
        j["color"][0].get<float>(),
        j["color"][1].get<float>(),
        j["color"][2].get<float>()
    };

    l.intensity = j["intensity"].get<float>();
}

void load_aabb(const json &j, ecs::entity &e, string_table &hashes)
{
    auto &a = e.get_component<ecs::aabb_collider_component>();
    auto colliders = j["colliders"];

    a.count = 0;

    for (auto &aabb : colliders)
    {
        if (a.count == MAX_COLLIDER_SHAPES)
        {
            spdlog::warn("Collider limit exceeded for entity {0}!", e.id());
            break;
        }

        auto min = aabb["min"];
        auto max = aabb["max"];
        auto is_trigger = aabb.value("is_trigger", false);
        physics_models::aabb shape;
        shape.min = glm::vec3(min[0].get<float>(), min[1].get<float>(), min[2].get<float>());
        shape.max = glm::vec3(max[0].get<float>(), max[1].get<float>(), max[2].get<float>());
        a.colliders[a.count++] = physics_models::aabb_collider(shape, is_trigger);
    }
}

void load_sphere(const json &j, ecs::entity &e, string_table &hashes)
{
    auto &s = e.get_component<ecs::sphere_collider_component>();
    auto colliders = j["colliders"];

    s.count = 0;

    for (auto &sphere : colliders)
    {
        if (s.count == MAX_COLLIDER_SHAPES)
        {
            spdlog::warn("Collider limit exceeded for entity {0}!", e.id());
            break;
        }

        auto center = sphere["center"];
        auto radius = sphere["radius"];
        auto is_trigger = sphere.value("is_trigger", false);
        physics_models::sphere shape;
        shape.center = glm::vec3(center[0].get<float>(), center[1].get<float>(), center[2].get<float>());
        shape.radius = radius.get<float>();
        s.colliders[s.count++] = physics_models::sphere_collider(shape, is_trigger);
    }
}

void load_rigid_body(const json &j, ecs::entity &e, string_table &hashes)
{
    auto &p = e.get_component<ecs::rigid_body_component>();
    p.velocity = glm::vec3(
        j["velocity"][0].get<float>(),
        j["velocity"][1].get<float>(),
        j["velocity"][2].get<float>());

    p.is_kinematic = j["is_kinematic"].get<bool>();
    float mass = j["mass"].get<float>();
    p.set_mass(mass);

    if (j.find("gravity") != j.end())
    {
        p.gravity = glm::vec3(
            j["gravity"][0].get<float>(),
            j["gravity"][1].get<float>(),
            j["gravity"][2].get<float>()
            );
    }
}


bitshift_to_component_loader asset::component_loader::loader_functions
    {
        {ecs::transform_component::component_bitshift, load_transform},
        {ecs::camera_component::component_bitshift, load_camera},
        {ecs::render_component_ogl::component_bitshift, load_render_ogl},
        {ecs::aabb_collider_component::component_bitshift, load_aabb},
        {ecs::sphere_collider_component::component_bitshift, load_sphere},
        {ecs::punctual_light_component::component_bitshift, load_punctual_light },
        {ecs::rigid_body_component::component_bitshift, load_rigid_body},
    };


asset::component_loader::component_loader(string_table &string_table) : _string_table(string_table) {}

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

