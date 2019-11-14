//
// Created by sava on 10/29/19.
//

#include <set>

#include <nlohmann/json.hpp>
using nlohmann::json;


#include <asset/assimp_loader.hpp>
#include <asset/basic_mesh_reader.hpp>


#include <bits/stdc++.h>
#include <glm/gtc/epsilon.hpp>

const float Epsilon = 10.0f;


bool compare_by_y(glm::vec3 a, glm::vec3 b)
{
    return a.y < b.y;
}

bool compare_by_z(glm::vec3 a, glm::vec3 b)
{
    return a.z > b.z;
}


class aabb_adder
{
public:

private:

};



int main()
{
    json aabbs;
    std::set<float> y_values;

    asset::assimp_loader assimp_reader;

    auto mesh_ret = assimp_reader.load_model("./assets/models/seg2/seg2.gltf", asset::mesh_type::GLTF2);
    auto mesh = *mesh_ret;

    std::vector<glm::vec3> vertices;

    for(auto& v : mesh.meshes[0].mesh_data.vertices)
    {
        vertices.push_back(v.position);
        y_values.insert(round(v.position.y));
    }

    // determine the y extent of the ground block:
    // 1 sort vertices by height (y). The last element's height is then the height of
    // ridge.
    // go back until you hit the next-highest vertex. that's ymax.
    // ymin is the first vertex's y.
    float ground_y_min, ground_y_max, ridge_height;

    sort(vertices.begin(), vertices.end(), compare_by_y);

    ridge_height = vertices.back().y;
    ground_y_min = vertices.front().y;



    for (auto it = vertices.rbegin(); it != vertices.rend(); ++it)
    {
        if (glm::epsilonNotEqual(ridge_height, it->y, Epsilon) && ground_y_max == -999)
        {
            ground_y_max = it->y;
            break;
        }
    }

    // NOTE: Set this manually, trying to determine ground height from mesh does not work.
    ground_y_max = 1.f;

    // now, sort by z, and walk along z, finding spans along each equal-ish z value
    sort(vertices.begin(), vertices.end(), compare_by_z);

    float xmin = 999999, xmax = -9999999;
    float last_xmin = 999999, last_xmax = -9999999;

    float current_z = vertices.front().z;
    float last_span_change_z = current_z;

    for(auto& v : vertices)
    {
        if (glm::epsilonNotEqual(current_z, v.z, Epsilon))
        {
            break;
        }
        else
        {
            if (v.x < xmin) xmin = v.x;
            if (v.x > xmax) xmax = v.x;
        }

        current_z = v.z;
    }

    last_xmin = xmin;
    last_xmax = xmax;
    current_z = vertices.front().z;

    for(auto& v : vertices)
    {
        if (glm::epsilonNotEqual(current_z, v.z, Epsilon))
        {
            // we've moved to next z value. check if the span we've built up is different from last
            bool span_changed =   glm::epsilonNotEqual(xmin, last_xmin, Epsilon) ||
                                  glm::epsilonNotEqual(xmax, last_xmax, Epsilon);
            if (span_changed)
            {
                float box_zmin = last_span_change_z;
                float box_zmax = current_z;
                float box_xmin = last_xmin;
                float box_xmax = last_xmax;

                aabbs.push_back({
                    {"min", {box_xmin, ground_y_min, box_zmax }},
                    {"max", {box_xmax, ground_y_max, box_zmin }}
                });

                last_xmax = xmax;
                last_xmin = xmin;
                last_span_change_z = current_z;
                xmin = 99999999;
                xmax = -99999999;
            }
        }
        else
        {
            if (v.x < xmin) xmin = v.x;
            if (v.x > xmax) xmax = v.x;
        }

        current_z = v.z;
    }

    float last_z = vertices.back().z;

    aabbs.push_back({
        {"min", {last_xmin, ground_y_min, last_z }},
        {"max", {last_xmax, ground_y_max, last_span_change_z }}
    });

    std::ofstream o("aabbs.json");
    o << std::setw(4) << aabbs << std::endl;
};
