

add_library(ogllib STATIC
            camera.cpp
            program_info.cpp
            shader_program.cpp
            shader_program_uniform_assign.cpp
            vectormath.cpp)

target_include_directories(ogllib PUBLIC ./include)
