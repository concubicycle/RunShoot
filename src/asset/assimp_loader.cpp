//
// Created by sava on 10/23/19.
//

#include <asset/assimp_loader.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

tl::expected<asset::assimp_model, asset::mesh_read_error> asset::assimp_loader::load_model(std::string path)
{
    assimp_model retval;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_ValidateDataStructure);
//        aiProcess_Triangulate |
//        aiProcess_FlipUVs |
//        aiProcess_ValidateDataStructure);

    auto error = importer.GetErrorString();

    auto textures = scene->mTextures;

    return  models::model<ogllib::vertex_pntx2d>();
}
