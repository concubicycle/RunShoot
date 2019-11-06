////
//// Created by sava on 10/23/19.
////
//
//#ifndef RS__tinygltf_MESH_LOADER_HPP_
//#define RS__tinygltf_MESH_LOADER_HPP_
//
//#include <vector>
//
//#include <vertex_pntx2d.h>
//#include <asset/model.hpp>
//#include <asset/mesh_type.hpp>
//#include <tl/expected.hpp>
//
//#define TINYGLTF_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include <tiny_gltf.h>
//
//#include "mesh_read_error.hpp"
//#include "asset_types.hpp"
//#include <ecs/components/basic_components.hpp>
//
//namespace asset
//{
//    class tinygltf_loader
//    {
//        using mesh_result = tl::expected<gltf_model, mesh_read_error>;
//
//    public:
//        mesh_result load_model(const std::string& path) const;
//
//    private:
//        mesh_result load_glf2(const std::string& path) const;
//
//        void process_node(
//
//            gltf_model& model,
//            const std::string& mesh_path) const;
//
//        gltf_mesh process_mesh(
//
//            const std::string& mesh_path) const;
//
//        std::vector<models::texture> load_material_textures(
//
//            const std::string& typeName,
//            const std::string& mesh_path) const;
//    };
//}
//#endif //__tinygltf_LOADER_HPP_
