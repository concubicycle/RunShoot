#ifndef __TEXTURE_2D_H_
#define __TEXTURE_2D_H_

#include <vector>
#include <memory>
#include <limits.h>

#include <spdlog/spdlog.h>

#include "glm/gtc/noise.hpp"
#include "glm/vec2.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

/*
	TTexelComponent is usually glm::byte. NComponentsPerTexel is usually bytes per texel. 
*/
template<typename TTexelComponent, unsigned char NComponentsPerTexel, typename TId>
class texture_2d
{

public:
    /*	NOTE FOR OPENGL: Textures MUST have dimensions that are powers of 2	*/
    texture_2d(TId id, unsigned int width, unsigned int height) : _width(width),
                                                                  _height(height),
                                                                  _id(id),
                                                                  _textureData(
                                                                      width * height * sizeof(TTexelComponent) *
                                                                      NComponentsPerTexel)
    {
    }

    /*	NOTE FOR OPENGL: Textures MUST have dimensions that are powers of 2	*/
    texture_2d(TId id, unsigned int width, unsigned int height, std::shared_ptr<TTexelComponent> data) : _width(width),
                                                                                                         _height(
                                                                                                             height),
                                                                                                         _id(id),
                                                                                                         _textureData(
                                                                                                             data)
    {
    }

    texture_2d(TId id, const std::string &fileName)
        : _id(id)
    {
        if (sizeof(TTexelComponent) * CHAR_BIT != 8)
            spdlog::warn("stb_image expects 8 bit components, but the size expected by texture_2d is {0} bits",
                sizeof(TTexelComponent) * CHAR_BIT != 8);

        int bytesPerTexel; //# 8-bit components per pixel ...
        const char *str = fileName.c_str();

        unsigned char *data = stbi_load(
            str,
            (int *) (&_width),
            (int *) (&_height),
            &bytesPerTexel,
            (int) NComponentsPerTexel);

        unsigned int size = _width * _height;

        // fill up alpha channel with 255 if we didn't read it
        auto cpertex = NComponentsPerTexel;

        if (NComponentsPerTexel == 4 && bytesPerTexel < 4)
            for (int i = 3; i < size; i += 4)
                data[i] = 255;

        _textureData = std::shared_ptr<TTexelComponent>(data,
            [](TTexelComponent *dataPtr) -> void
            {
                stbi_image_free(dataPtr);
            });
    }

    ~texture_2d()
    {
        _textureData.reset();
    }

    std::shared_ptr<TTexelComponent> get(int x, int y)
    {
        return &(_textureData[(_width * y + x) * sizeof(TTexelComponent)]);
    }

    void createSimplexNoise(float frequency, float amplitude)
    {
        float max = 0;

        //fractal increasing frequency and decreasing amplitude?
        for (std::size_t y = 0; y < _height; ++y)
            for (std::size_t x = 0; x < _width; ++x)
            {
                float texelComponent = get(x, y)[0];
                float evaluateSimplex = glm::simplex(glm::vec3(x * frequency, y * frequency, 0.5f));
                float ampCoefficient = amplitude / 2.0f + (amplitude / 2.0f - 1.0f);

                float color = texelComponent + evaluateSimplex * ampCoefficient;

                if (color > max)
                    max = color;

                for (int i = 0; i < sizeof(NComponentsPerTexel); i++)
                    get(x, y)[i] = color;
            }
    }

    std::shared_ptr<TTexelComponent> getDataVectorPtr() { return _textureData; }

    unsigned int getWidth() { return _width; }

    unsigned int getHeight() { return _height; }

    TId getId() const
    {
        return _id;
    }

    void free_texture_data()
    {
        _textureData.reset();
    }

protected:
    std::shared_ptr<TTexelComponent> _textureData;

private:
    TId _id;

    unsigned int _width;
    unsigned int _height;
};

#endif