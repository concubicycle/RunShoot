//
// Created by sava on 10/23/19.
//

#ifndef __MODEL_HPP_
#define __MODEL_HPP_

#include <vector>
#include "multi_texture_mesh.hpp"

namespace models
{

    template <class TVertex>
    class model
    {
    public:
        std::vector<multi_texture_mesh<TVertex>> meshes;
        std::string directory;
    };

}

#endif //__MODEL_HPP_
