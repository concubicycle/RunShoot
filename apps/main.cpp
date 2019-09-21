#include <stdio.h>
#include <iostream>
#include <chrono>

/////////////
#include <GLFW/glfw3.h>

#include <glbinding-aux/Meta.h>
#include <glbinding/Version.h>
#include <glbinding/glbinding.h>

#include <glbinding/gl/gl.h>

#include <glbinding-aux/ValidVersions.h>
#include <glbinding-aux/types_to_string.h>

using namespace gl;
using namespace glbinding;
using GLenum = gl::GLenum;
//////////////

#include <core/startup_config.hpp>
#include <core/frame_timer.hpp>
#include <core/frame_limiter.hpp>

#include <renderer/renderer.hpp>
#include <asset/basic_mesh_reader.hpp>

#include <shader_program.h>
#include <shader.h>
#include <vertex_buffer.h>
#include <index_buffer.h>
#include <file_read_std.h>
#include <opengl_texture_2d.h>
#include <asset/textured_mesh.hpp>

#include <shader_program_specializations.h>

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <array>

#include <ecs/game_object_factory.hpp>

using namespace ogllib;

struct scene_data
{
    GLFWwindow *window;
    core::frame_timer &timer;
    core::frame_limiter &limiter;
    ogllib::shader_program<ogllib::vertex_p> &program;
    ogllib::vertex_array<ogllib::vertex_p> &vao;
    ogllib::index_buffer &ebo;

    ogllib::shader_program<ogllib::vertex_ptx2d> &tex_program;
    opengl_texture_2d<glm::byte, 3> &texture;
    ogllib::vertex_array<ogllib::vertex_ptx2d> &vao_tex;
    ogllib::index_buffer &ebo_tex;
};

GLFWwindow *set_up_glfw(std::uint32_t width, std::uint32_t height);

// put all OpenGL allocations here, so that they're destructed before main() tears down everything else.
void run_game(core::startup_config &conf, GLFWwindow *window);
void render_loop(scene_data &data);

void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void try_assimp()
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile("./assets/models/Avocado/glTF/Avocado.gltf", aiProcess_ValidateDataStructure);
}

int main()
{
    ecs::archetype_store archetype_store;
    ecs::component_store component_store;
    ecs::game_object_factory factory(archetype_store, component_store);

    core::startup_config conf = core::startup_config();
    conf.load();

    auto window = set_up_glfw(conf.width(), conf.height());

    try_assimp();

    if (window == NULL)
        return -1;

    run_game(conf, window);

    glfwTerminate();
    spdlog::shutdown();
    return 0;
}

void run_game(core::startup_config &conf, GLFWwindow *window)
{
    core::frame_timer timer;
    core::frame_limiter limiter(timer, 60);

    rendering::renderer renderer(conf);
    renderer.init();

    readers::basic_mesh_reader reader;
    auto mesh = reader.read_mesh_p(std::string("./assets/models/square.json"));
    auto tex_mesh = reader.read_mesh_ptx2d("./assets/models/triangle_textured.json");

    auto error = mesh.error();
    std::cout << error;
    error = tex_mesh.error();
    std::cout << error;

    ogllib::shader<FileReadStd> vert(GL_VERTEX_SHADER);
    ogllib::shader<FileReadStd> frag(GL_FRAGMENT_SHADER);
    vert.from_file("./assets/shaders/simple.vert");
    frag.from_file("./assets/shaders/simple.frag");
    ogllib::shader<FileReadStd> vert_ptx2d(GL_VERTEX_SHADER);
    ogllib::shader<FileReadStd> frag_ptx2d(GL_FRAGMENT_SHADER);
    vert_ptx2d.from_file("./assets/shaders/ptx2d_basic.vert");
    frag_ptx2d.from_file("./assets/shaders/ptx2d_basic.frag");

    ogllib::shader_program<ogllib::vertex_p> program(&vert, &frag);
    ogllib::shader_program<ogllib::vertex_ptx2d> tex_program(&vert_ptx2d, &frag_ptx2d);
    program.compile();
    tex_program.compile();

    ogllib::vertex_buffer<ogllib::vertex_p> vbo(mesh->vertices);
    ogllib::vertex_array<ogllib::vertex_p> vao;
    ogllib::index_buffer ebo(mesh->indices);

    vao.bind();

    vbo.bind();
    vbo.buffer();

    ebo.bind();
    ebo.buffer();

    program.bind();
    program.set_attrib_pointers();

    vbo.unbind();
    ebo.unbind();
    vao.unbind();
    program.unbind();

    auto texture = opengl_texture_2d<glm::byte, 3>(tex_mesh->texture_filename);
    ogllib::vertex_buffer<ogllib::vertex_ptx2d> vbo_tex(tex_mesh->mesh_data.vertices);
    ogllib::vertex_array<ogllib::vertex_ptx2d> vao_tex;
    ogllib::index_buffer ebo_tex(tex_mesh->mesh_data.indices);

    vao_tex.bind();
    vbo_tex.bind();
    vbo_tex.buffer();
    ebo_tex.bind();
    ebo_tex.buffer();

    texture.bind();
    texture.set_texture_wrap(GL_REPEAT);
    texture.set_filtering(GL_LINEAR, GL_LINEAR);
    texture.buffer();
    texture.free_texture_data();

    //texture.gen_mip_maps();

    tex_program.bind();
    tex_program.set_attrib_pointers();

    vbo_tex.unbind();
    ebo_tex.unbind();
    vao_tex.unbind();
    tex_program.unbind();

    scene_data data = {window, timer, limiter, program, vao, ebo, tex_program, texture, vao_tex, ebo_tex};

    render_loop(data);
}

void render_loop(scene_data &data)
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(data.window))
    {
        data.timer.start();

        process_input(data.window);

        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::glClear(gl::ClearBufferMask::GL_COLOR_BUFFER_BIT);

        data.tex_program.bind();
        data.texture.bind();
        data.vao_tex.bind();
        data.ebo_tex.bind();

        gl::glDrawElements(gl::GLenum::GL_TRIANGLES, 6, gl::GLenum::GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(data.window);

        /* Poll for and process events */
        glfwPollEvents();

        data.limiter.wait_remainder();

        data.timer.end();
        //std::cout << timer.frame_info() << std::endl;
    }
}

GLFWwindow *set_up_glfw(std::uint32_t width, std::uint32_t height)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return NULL;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "RunShoot", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glbinding::initialize(glfwGetProcAddress, true);

    /* Prevent framerate cap by gpu driver (don't wait for vblank before returning form glfwSwapBuffers) */
    glfwSwapInterval(0);

    return window;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)(window);                      // suppress unused param
    gl::glViewport(0, 0, width, height); //glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}