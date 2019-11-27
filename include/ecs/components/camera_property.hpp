//
// Created by sava on 11/27/19.
//

#ifndef __CAMERA_PROPERTY_HPP_
#define __CAMERA_PROPERTY_HPP_

#include <string>

namespace ecs
{

    template<typename T>
    struct camera_property
    {
        std::string name;
        T data;
    };


}

#endif //__CAMERA_PROPERTY_HPP_
