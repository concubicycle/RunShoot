
#ifndef __TEXTURE_3D_H_
#define __TEXTURE_3D_H_

#include <vector>
#include <memory>

#include "glm\gtc\noise.hpp"

template <typename TTexelComponent, typename TId>
class Texture3D
{

public:
	/*	NOTE FOR OPENGL: Textures MUST have dimensions that are powers of 2	*/
	Texture3D(TId id, unsigned int width, unsigned int height, unsigned int layers, std::shared_ptr<TTexelComponent> data) : _width(width),
																															 _height(height),
																															 _layers(layers),
																															 _id(id),
																															 _textureData(data)
	{
	}

	/*	NOTE FOR OPENGL: Textures MUST have dimensions that are powers of 2	*/
	Texture3D(TId id, unsigned int width, unsigned int height, unsigned int layers) : _width(width),
																					  _height(height),
																					  _layers(layers),
																					  _id(id),
																					  _textureData(width * height * layers * sizeof(TTexelComponent))
	{
	}

	std::shared_ptr<TTexelComponent> get(int x, int y, int layer)
	{
		return &(_textureData[(_width * _height * layer + _width * y + x) * sizeof(TTexelComponent)]);
	}

	std::shared_ptr<TTexelComponent> getLayer(int layer)
	{
		return &(_textureData[_width * _height * layer * sizeof(TTexelComponent)]);
	}

	void createSimplexNoise(float frequency, float amplitude)
	{
		float max = 0;

		//fractal increasing frequency and decreasing amplitude?

		for (std::sizeT z = 0; z < _layers; ++z)
			for (std::sizeT y = 0; y < _height; ++y)
				for (std::sizeT x = 0; x < _width; ++x)
				{
					float texelComponent = get(x, y, z)[0];
					float evaluateSimplex = glm::simplex(glm::vec3(x * frequency, y * frequency, z * frequency));
					float ampCoefficient = amplitude / 2.0f + (amplitude / 2.0f - 1.0f);
					float color = texelComponent + evaluateSimplex * ampCoefficient;

					if (color > max)
						max = color;

					for (int i = 0; i < sizeof(TTexelComponent); i++)
						get(x, y, z)[i] = color;
				}
	}

	std::shared_ptr<TTexelComponent> getData() const
	{
		return _textureData;
	}

	//getters
	std::shared_ptr<TTexelComponent> getDataVectorPtr() { return _textureData; }

	unsigned int get_width() { return _width; }
	unsigned int getHeight() { return _height; }
	unsigned int getNu_layers() { return _layers; }

	TId getId() { return _id; }

private:
	std::shared_ptr<TTexelComponent> _textureData;

	TId _id;
	unsigned int _width;
	unsigned int _height;
	unsigned int _layers;
};

#endif
