//
// Created by sava on 11/12/19.
//

#ifndef __SKYBOX_HPP_
#define __SKYBOX_HPP_

#include <string>
#include <vector>

#include <shader_program.h>
#include <vertex_p.h>
#include <stb_image.h>
#include <vectormath.h>

namespace rendering
{
    class skybox
    {

    public:
        skybox() :
            _shader("./assets/shaders/skybox.vert", "./assets/shaders/skybox.frag")
        {
            float skyboxVertices[] = {
                // positions
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
            };

            std::string skybox_left = "./assets/textures/left.png";
            std::string skybox_top = "./assets/textures/top.png";
            std::string skybox_front = "./assets/textures/front.png";
            std::string skybox_bottom = "./assets/textures/bottom.png";
            std::string skybox_right = "./assets/textures/right.png";
            std::string skybox_back = "./assets/textures/back.png";

            // check this
            glEnable(GL_DEPTH_TEST);

            // skybox VAO
            glGenVertexArrays(1, &skyboxVAO);
            glGenBuffers(1, &skyboxVBO);
            glBindVertexArray(skyboxVAO);
            glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


            std::vector<std::string> faces
            {
               skybox_right,
                skybox_left,
                skybox_top,
                skybox_bottom,
                skybox_front,
                skybox_back
            };
            cubemapTexture = loadCubemap(faces);

            // shader configuration
            // --------------------
            _shader.bind();
            _shader.set_uniform("skybox", 0);
        }


        void draw(ecs::camera_component& cam, float screen_width, float screen_height)
        {
            auto aspect = screen_width / screen_height;
            auto projection = glm::perspective(cam.fov, aspect, cam.near, cam.far);

            auto cam_basis = cam.right_up_fwd();
            auto view = glm::lookAt(cam.position, cam.position + cam_basis[2], cam_basis[1]);
            set_translation(view, 0, 0, 0);

            glDepthMask(GL_FALSE);

            glDepthFunc(GL_LEQUAL);
            _shader.bind();
            _shader.set_uniform("view", view);
            _shader.set_uniform("projection", projection);
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);

            glDepthMask(GL_TRUE);
        }

    private:
        ogllib::shader_program<ogllib::vertex_p> _shader;
        unsigned int skyboxVAO, skyboxVBO;
        unsigned int cubemapTexture;

        unsigned int loadCubemap(std::vector<std::string> faces)
        {
            unsigned int textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

            int width, height, nrChannels;
            for (unsigned int i = 0; i < faces.size(); i++)
            {
                unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
                if (data)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    stbi_image_free(data);
                }
                else
                {
                    std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                    stbi_image_free(data);
                }
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            return textureID;
        }

    };

}

#endif //__SKYBOX_HPP_
