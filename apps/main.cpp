#include <iostream>
#include <string>

/////////////
#include <GLFW/glfw3.h>
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

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
#include <ogl_2d_tex.hpp>
#include <shader_program_specializations.h>
#include <asset/texture_manager.hpp>

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
    ogllib::ogl_2d_tex& texture;
    ogllib::vertex_array<ogllib::vertex_ptx2d> &vao_tex;
    ogllib::index_buffer &ebo_tex;
};

GLFWwindow *set_up_glfw(std::uint32_t width, std::uint32_t height);

// put all OpenGL allocations here, so that they're destructed before main() tears down everything else.
void run_game(core::startup_config &conf, GLFWwindow *window);
void render_loop(scene_data &data);

void process_input(GLFWwindow *window);




int main()
{
    core::startup_config conf = core::startup_config();
    conf.load();

    auto window = set_up_glfw(conf.width(), conf.height());

    if (window == nullptr)
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

    asset::texture_manager textures;

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
    ogllib::shader<FileReadStd> vert_ptx2d(GL_VERTEX_SHADER);
    ogllib::shader<FileReadStd> frag_ptx2d(GL_FRAGMENT_SHADER);

    vert.from_file("./assets/shaders/simple.vert");
    frag.from_file("./assets/shaders/simple.frag");
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

    auto& texture = textures.load_texture(tex_mesh->texture_filename); //tex2_rgb(tex_mesh->texture_filename);

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

    texture.gen_mip_maps();

    tex_program.bind();
    tex_program.set_attrib_pointers();

    vbo_tex.unbind();
    ebo_tex.unbind();
    vao_tex.unbind();

    texture.unload_tex();

    scene_data data = {window, timer, limiter, program, vao, ebo, tex_program, texture, vao_tex, ebo_tex};

    render_loop(data);
}

void render_loop(scene_data &data)
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(data.window))
    {
        data.timer.start();

        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::glClear(gl::ClearBufferMask::GL_COLOR_BUFFER_BIT);

        data.tex_program.bind();
        data.texture.bind();
        data.vao_tex.bind();
        data.ebo_tex.bind();

        gl::glDrawElements(gl::GLenum::GL_TRIANGLES, 6, gl::GLenum::GL_UNSIGNED_INT, nullptr);

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
        return nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "RunShoot", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glbinding::initialize(glfwGetProcAddress, true);

    /* Prevent framerate cap by gpu driver (don't wait for vblank before returning form glfwSwapBuffers) */
    glfwSwapInterval(0);

    return window;
}
