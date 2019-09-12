#ifndef __OPENGL_TEX_2D__H_
#define __OPENGL_TEX_2D__H_

#include "opengl_afx.h"
#include "texture_2d.h"

#include "glm/gtx/raw_data.hpp"
#include "texture_wrap_mode.h"
#include "texture_filtering_mode.h"
#include "texture_magmin.h"

#include <string>
#include <memory>

template <typename TTexelComponent, unsigned char NComponents>
class OpenGlTexture2D : Texture2D<TTexelComponent, NComponents, GLuint>
{
private:
	TextureWrapMode _wrapMode;
	TextureFilteringMode _filteringMode;
	bool _isBound;

public:
	/*	NOTE FOR OPENGL: Textures MUST have dimensions that are powers of 2	*/
	OpenGlTexture2D(unsigned int width, unsigned int height) : Texture2D<TTexelComponent, NComponents, GLuint>(generate(), width, height), _isBound(false)
	{
	}

	/*	NOTE FOR OPENGL: Textures MUST have dimensions that are powers of 2	*/
	OpenGlTexture2D(unsigned int width, unsigned int height, std::shared_ptr<TTexelComponent> data) : Texture2D<TTexelComponent, NComponents, GLuint>(generate(), width, height, data), _isBound(false)
	{
	}

	OpenGlTexture2D(const std::string &fileName) : Texture2D<TTexelComponent, NComponents, GLuint>(generate(), fileName), _isBound(false)
	{
	}

	static GLuint generate()
	{
		GLuint idBuff;
		glGenTextures(1, &idBuff);
		return idBuff;
	}

	void bind()
	{
		glBindTexture(GL_TEXTURE_2D, getId());
		_isBound = true;
	}

	void unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		_isBound = false;
	}

	void buffer()
	{
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGBA,
					 getWidth(),
					 getHeight(),
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 _textureData.get());
	}

	// OpenGL Stuff
	void generateMipMaps() const
	{
		if (!_isBound)
			el::Loggers::getLogger("default")->warn("Attempting to call generateMipMaps() on unbound texture");

		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void setWrapMode(TextureWrapMode mode)
	{
		if (!_isBound)
			el::Loggers::getLogger("default")->warn("Attempting to call generateMipMaps() on unbound texture");

		_wrapMode = mode;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
	}

	void setFilteringMode(TextureFilteringMode mode)
	{
		if (!_isBound)
			el::Loggers::getLogger("default")->warn("Attempting to call setFilteringMode() on unbound texture");

		_filteringMode = mode;
		glTexParameteri(GL_TEXTURE_2D, Magnifying, mode);
		glTexParameteri(GL_TEXTURE_2D, Minifying, mode);
	}

	void setFilteringMode(TextureMagMin magmin, TextureFilteringMode mode)
	{
		if (!_isBound)
			el::Loggers::getLogger("default")->warn("Attempting to call setFilteringMode() on unbound texture");

		_filteringMode = mode;
		glTexParameteri(GL_TEXTURE_2D, magmin, mode);
	}
};

typedef OpenGlTexture2D<glm::byte, 4> tex2;

#endif
