#ifndef __TEXTURE_2D_H_
#define __TEXTURE_2D_H_

#include <vector>
#include <memory>
#include <limits.h>

#include "glm/gtc/noise.hpp"
#include "glm/vec2.hpp"

#include "logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
	TTexelComponent is usually glm::byte. NComponentsPerTexel is usually bytes per texel. 
*/
template <typename TTexelComponent, unsigned char NComponentsPerTexel, typename TId>
class Texture2D
{

public:
	/*	NOTE FOR OPENGL: Textures MUST have dimensions that are powers of 2	*/
	Texture2D(TId id, unsigned int width, unsigned int height) : _width(width),
																 _height(height),
																 _id(id),
																 _textureData(width * height * sizeof(TTexelComponent) * NComponentsPerTexel)
	{
	}

	/*	NOTE FOR OPENGL: Textures MUST have dimensions that are powers of 2	*/
	Texture2D(TId id, unsigned int width, unsigned int height, std::shared_ptr<TTexelComponent> data) : _width(width),
																										_height(height),
																										_id(id),
																										_textureData(data)
	{
	}

	Texture2D(TId id, const std::string &fileName)
		: _id(id)
	{
		if (sizeof(TTexelComponent) * CHAR_BIT != 8)
			_logger->warn("stb_image expects 8 bit components, but the size expected by Texture2D is %i bits", sizeof(TTexelComponent) * CHAR_BIT != 8);

		int bytesPerTexel; //# 8-bit components per pixel ...
		const char *str = fileName.c_str();
		unsigned char *data = stbi_load(str, (int *)(&_width), (int *)(&_height), &bytesPerTexel, (int)NComponentsPerTexel);

		unsigned int size = _width * _height;

		// fill up alpha channel with 255 if we didn't read it
		auto cpertex = NComponentsPerTexel;

		if (NComponentsPerTexel == 4 && bytesPerTexel < 4)
			for (int i = 3; i < size; i += 4)
				data[i] = 255;

		_textureData = std::shared_ptr<TTexelComponent>(data,
														[](TTexelComponent *dataPtr) -> void {
															free(dataPtr);
														});
	}

	~Texture2D()
	{
		_textureData.reset();
	}

	std::shared_ptr<TTexelComponent> get(int x, int y, int layer)
	{
		return &(_textureData[(_width * y + x) * sizeof(TTexelComponent)]);
	}

	void createSimplexNoise(float frequency, float amplitude)
	{
		float max = 0;

		//fractal increasing frequency and decreasing amplitude?
		for (std::sizeT y = 0; y < _height; ++y)
			for (std::sizeT x = 0; x < _width; ++x)
			{
				float texelComponent = get(x, y, z)[0];
				float evaluateSimplex = glm::simplex(glm::vec3(x * frequency, y * frequency, 0.5f));
				float ampCoefficient = amplitude / 2.0f + (amplitude / 2.0f - 1.0f);

				float color = texelComponent + evaluateSimplex * ampCoefficient;

				if (color > max)
					max = color;

				for (int i = 0; i < sizeof(NComponentsPerTexel); i++)
					get(x, y, z)[i] = color;
			}
	}

	std::shared_ptr<TTexelComponent> getDataVectorPtr() { return _textureData; }

	unsigned int getWidth() { return _width; }
	unsigned int getHeight() { return _height; }
	unsigned int getNu_layers() { return _layers; }

	TId getId() const
	{
		return _id;
	}

protected:
	std::shared_ptr<TTexelComponent> _textureData;

private:
	TId _id;

	unsigned int _width;
	unsigned int _height;

	el::Logger *_logger = el::Loggers::getLogger("default");
};

#endif