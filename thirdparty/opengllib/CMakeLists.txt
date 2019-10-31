

add_library(ogllib STATIC
            camera.cpp
            program_info.cpp
            shader_program.cpp
            vectormath.cpp


            include/buffer_object.h
            include/camera.h
            include/file_read_std.h
            include/index_buffer.h
            include/ogl_2d_tex.h
            include/opengl_afx.h
            include/opengl_texture_2d.h
            include/program_info.h
            include/shader.h
            include/shader_program.h
            include/stb_image.h
            include/texture_2d.h
            include/texture_3d.h
            include/vectormath.h
            include/vertex_array.h
            include/vertex_buffer.h
            include/vertex_p.h
            include/vertex_pc.h
            include/vertex_pctx2d.h
            include/vertex_ptx2d.h
            include/vertex_pntx2d.h)

target_include_directories(ogllib PUBLIC ./include ../../external/glbinding/include)
