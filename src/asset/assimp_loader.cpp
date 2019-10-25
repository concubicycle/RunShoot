//
// Created by sava on 10/23/19.
//

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <asset/assimp_loader.hpp>
#include <asset/mesh_type.hpp>
#include <spdlog/spdlog.h>
#include <ecs/components/basic_components.hpp>

tl::expected<asset::assimp_model, asset::mesh_read_error> asset::assimp_loader::load_model(
    std::string path,
    asset::mesh_type mesh_type)
{
    switch (mesh_type)
    {
        case asset::GLTF2:
            return load_glf2(path);
    }

    return load_glf2(path); //temp
}

asset::assimp_loader::mesh_result asset::assimp_loader::load_glf2(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        path,
        aiProcess_ValidateDataStructure |
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_ValidateDataStructure);

    auto error = importer.GetErrorString();

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        spdlog::error(error);
        return tl::unexpected<mesh_read_error>(mesh_read_error::BadFile);
    }

    assimp_model model;
    model.directory = path.substr(0, path.find_last_of('/'));
    process_node(scene->mRootNode, scene, model);
    return model;
}

void asset::assimp_loader::process_node(aiNode *node, const aiScene *scene, assimp_model& model)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        model.meshes.push_back(process_mesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene, model);
    }
}

asset::assimp_mesh asset::assimp_loader::process_mesh(aiMesh *mesh, const aiScene *scene)
{
    assimp_mesh result;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        auto tex = mesh->mTextureCoords[0] == NULL
                   ? glm::vec2(0)
                   : glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

        result.mesh_data.vertices.emplace_back(
            glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
            glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
            tex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            result.mesh_data.indices.push_back(face.mIndices[j]);
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<models::texture> diffuse = load_material_textures(
            material,
            aiTextureType_DIFFUSE,
            "texture_diffuse");

        result.textures.insert(result.textures.end(), diffuse.begin(), diffuse.end());

        std::vector<models::texture> specular = load_material_textures(
            material,
            aiTextureType_SPECULAR,
            "texture_specular");

        result.textures.insert(result.textures.end(), specular.begin(), specular.end());
    }

    return result;
}

std::vector<models::texture>
asset::assimp_loader::load_material_textures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<models::texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        models::texture texture;
        //texture.id = TextureFromFile(str.C_Str(), directory);
        texture.type = typeName;
        texture.path = str.data;
        textures.push_back(texture);
    }
    return textures;
}
