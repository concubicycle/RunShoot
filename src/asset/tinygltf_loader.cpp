////
//// Created by sava on 10/23/19.
////
//
//
//#include <asset/tinygltf_loader.hpp>
//#include <asset/mesh_type.hpp>
//#include <spdlog/spdlog.h>
//#include <ecs/components/basic_components.hpp>
//#include <platform/symbols.hpp>
//
//
//bool loadModel(tinygltf::Model &model, const char *filename) {
//    tinygltf::TinyGLTF loader;
//    std::string err;
//    std::string warn;
//
//    bool res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
//    if (!warn.empty())
//        spdlog::warn("WARN: {0}", warn);
//
//    if (!err.empty())
//        spdlog::error("ERR: {0}", err);
//
//    return res;
//}
//
//
//
//
//tl::expected<asset::gltf_model, asset::mesh_read_error> asset::tinygltf_loader::load_model(
//    const std::string& path) const
//{
//    return load_glf2(path);
//}
//
//asset::tinygltf_loader::mesh_result asset::tinygltf_loader::load_glf2(const std::string& path) const
//{
////    Assimp::Importer importer;
////    const aiScene *scene = importer.ReadFile(
////        path,
////        aiProcess_ValidateDataStructure | aiProcess_FixInfacingNormals);
////
////    auto error = importer.GetErrorString();
////
////    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
////    {
////        spdlog::error(error);
////        return tl::unexpected<mesh_read_error>(mesh_read_error::BadFile);
////    }
//
//    tinygltf::Model tgf_model;
//    if (!loadModel(tgf_model, path.c_str()))
//        return tl::unexpected<mesh_read_error>(mesh_read_error::BadFile);
//
//
//    gltf_model model;
//    model.directory = path.substr(0, path.find_last_of('/'));
//
//    // WIP.... TODO: finish
//
//    //process_node(scene->mRootNode, scene, model, path);
//
//    return model;
//}
//
//void asset::tinygltf_loader::process_node(
////    aiNode *node,
////    const aiScene *scene,
//    gltf_model& model,
//    const std::string& mesh_path) const
//{
////    for(unsigned int i = 0; i < node->mNumMeshes; i++)
////    {
////        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
////        model.meshes.push_back(process_mesh(mesh, scene, mesh_path));
////    }
////
////    for(unsigned int i = 0; i < node->mNumChildren; i++)
////    {
////        process_node(node->mChildren[i], scene, model, mesh_path);
////    }
//}
//
//asset::gltf_mesh asset::tinygltf_loader::process_mesh(
//
//    const std::string& mesh_path) const
//{
//    gltf_mesh result;
//
////    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
////    {
////        auto tex = mesh->mTextureCoords[0] == nullptr
////                   ? glm::vec2(0)
////                   : glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
////
////        result.mesh_data.vertices.emplace_back(
////            glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
////            glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
////            tex);
////    }
////
////    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
////    {
////        aiFace face = mesh->mFaces[i];
////        for(unsigned int j = 0; j < face.mNumIndices; j++)
////            result.mesh_data.indices.push_back(face.mIndices[j]);
////    }
////
////    if(mesh->mMaterialIndex >= 0)
////    {
////        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
////        std::vector<models::texture> diffuse = load_material_textures(
////            material,
////            aiTextureType_DIFFUSE,
////            "texture_diffuse",
////            mesh_path);
////
////        result.textures.insert(result.textures.end(), diffuse.begin(), diffuse.end());
////
////        std::vector<models::texture> specular = load_material_textures(
////            material,
////            aiTextureType_SPECULAR,
////            "texture_specular",
////            mesh_path);
////
////        result.textures.insert(result.textures.end(), specular.begin(), specular.end());
////    }
//
//    return result;
//}
//
//std::vector<models::texture>
//asset::tinygltf_loader::load_material_textures(
////    aiMaterial *mat,
////    aiTextureType type,
//    const std::string& typeName,
//    const std::string& mesh_path) const
//{
//    std::vector<models::texture> textures;
//
//    const size_t last_slash_idx = mesh_path.find_last_of("/\\");
//    std::string directory = std::string::npos != last_slash_idx
//        ? mesh_path.substr(0, last_slash_idx)
//        : ".";
//
//    directory += os::PathSeparator;
////
////    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
////    {
////        aiString str;
////        mat->GetTexture(type, i, &str);
////        models::texture texture;
////        //texture.id = TextureFromFile(str.C_Str(), directory);
////        texture.type = typeName;
////        texture.path = directory + str.data;
////        textures.push_back(texture);
////    }
//
//    return textures;
//}
