//
// Created by sava on 11/24/19.
//

#ifndef __OVERLAY_HPP_
#define __OVERLAY_HPP_

#include <vector>
#include <vertex_pc.h>
#include <glbinding/gl/gl.h>
#include <glm/gtx/rotate_vector.hpp>
#include "shader_set.hpp"

using namespace gl;

/**
 * THIS SHOULD BE REMOVED
 * It's a hardcoded crosshair for the RunShoot game,that I'm writing
 * because the game's due in a week and I don't feel like trying to
 * do a proper UI/HUD system
 */

class overlay_temporary
{
public:
    overlay_temporary(const rendering::shader_set& shader_set) : _shader_set(shader_set)
    {
        auto right = _vertices;
        auto bottom = _vertices;
        auto left = _vertices;

        for (auto& v : right)
            v.position = glm::rotateZ(v.position, -glm::half_pi<float>());

        for (auto& v : bottom)
            v.position = glm::rotateZ(v.position, glm::pi<float>());

        for (auto& v : left)
            v.position = glm::rotateZ(v.position, glm::half_pi<float>());

        _vertices.reserve(_vertices.size() * 4);
        _vertices.insert(_vertices.end(), right.begin(), right.end());
        _vertices.insert(_vertices.end(), bottom.begin(), bottom.end());
        _vertices.insert(_vertices.end(), left.begin(), left.end());

        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(ogllib::vertex_pc), _vertices.data(), GL_STATIC_DRAW);

        _shader_set.overlay().bind();
        _shader_set.overlay().set_attrib_pointers();

        glGenVertexArrays(1, &_health_vao);
        glGenBuffers(1, &_health_vbo);
        glBindVertexArray(_health_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _health_vbo);
        glBufferData(GL_ARRAY_BUFFER, _health_bar.size() * sizeof(ogllib::vertex_pc), _health_bar.data(), GL_STATIC_DRAW);
    }

    void draw (float screen_width, float screen_height)
    {
        auto& shader = _shader_set.overlay();

        auto half_w = screen_width / 2.f;
        auto half_h = screen_height / 2.f;

        auto aspect = screen_width / screen_height;
        auto projection = glm::ortho(-half_w, half_w, -half_h, half_h);
        auto model = glm::mat4(1.f);

        shader.bind();
        shader.set_uniform("projection", projection);
        shader.set_uniform("model", model);

        bind();
        glDrawArrays(GL_TRIANGLES, 0, 24);
        unbind();

        bind_health();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        unbind();

    }


private:
    const rendering::shader_set& _shader_set;
    GLuint _vbo;
    GLuint _vao;

    GLuint _health_vbo;
    GLuint _health_vao;



    void bind()
    {
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    }

    void unbind()
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    void bind_health()
    {
        glBindVertexArray(_health_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _health_vbo);
    }


    std::vector<ogllib::vertex_pc> _vertices {
        { 1.f, 3.f, -5.f, 0.f, 1.f, 0.f, 1.f },
        { 1.f, 10.f, -5.f, 0.f, 1.f, 0.f, 1.f },
        { -1.f, 3.f, -5.f, 0.f, 1.f, 0.f, 1.f },

        { -1.f, 3.f, -5.f, 0.f, 1.f, 0.f, 1.f },
        { 1.f, 10.f, -5.f, 0.f, 1.f, 0.f, 1.f },
        { -1.f, 10.f, -5.f, 0.f, 1.f, 0.f, 1.f }
    };

    std::vector<ogllib::vertex_pc> _health_bar {
        { 10.f, -10.f, -5.f, 1.f, 0.f, 0.f, 1.f },
        { 10.f, 10.f, -5.f, 1.f, 0.f, 0.f, 1.f },
        { -10.f, -10.f, -5.f, 1.f, 0.f, 0.f, 1.f },

        { -10.f, -10.f, -5.f, 1.f, 0.f, 0.f, 1.f },
        { 10.f, 10.f, -5.f, 1.f, 0.f, 0.f, 1.f },
        { -10.f, 10.f, -5.f, 1.f, 0.f, 0.f, 1.f }
    };
};

#endif //__OVERLAY_HPP_
