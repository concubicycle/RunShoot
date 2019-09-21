#ifndef __OPENGL_TEX_2D__H_
#define __OPENGL_TEX_2D__H_

#include "opengl_afx.h"
#include "texture_2d.h"

#include "glm/gtx/raw_data.hpp"

#include <string>
#include <memory>

#include <spdlog/spdlog.h>

using namespace gl;

template <typename TTexelComponent, unsigned char NComponents>
class opengl_texture_2d : public texture_2d<TTexelComponent, NComponents, GLuint>
{
private:
	gl::GLenum _wrapMode;
	gl::GLenum _filteringMode;
	bool _isBound;

public:
	/*	NOTE FOR OPENGL: Textures MUST have dimensions that are powers of 2	*/
	opengl_texture_2d(
		unsigned int width,
		unsigned int height) : texture_2d<TTexelComponent, NComponents, GLuint>(generate(), width, height), _isBound(false)
	{
	}

	/*	NOTE FOR OPENGL: Textures MUST have dimensions that are powers of 2	*/
	opengl_texture_2d(unsigned int width, unsigned int height, std::shared_ptr<TTexelComponent> data) : texture_2d<TTexelComponent, NComponents, GLuint>(generate(), width, height, data), _isBound(false)
	{
	}

	opengl_texture_2d(const std::string &fileName) : texture_2d<TTexelComponent, NComponents, GLuint>(generate(), fileName), _isBound(false)
	{
	}

	static GLuint generate()
	{
		GLuint idBuff;
		gl::glGenTextures(1, &idBuff);
		return idBuff;
	}

	void bind()
	{
		gl::glBindTexture(GL_TEXTURE_2D, this->getId());
		_isBound = true;
	}

	void unbind()
	{
		gl::glBindTexture(GL_TEXTURE_2D, 0);
		_isBound = false;
	}

	void buffer()
	{
		auto data = this->_textureData.get();
		auto width = this->getWidth();
		auto height = this->getHeight();

		gl::glTexImage2D(GL_TEXTURE_2D,
						 0,
						 GL_RGBA,
						 width,
						 height,
						 0,
						 GL_RGBA,
						 GL_UNSIGNED_BYTE,
						 data);
	}

	// OpenGL Stuff
	void generateMipMaps() const
	{
		if (!_isBound)
			spdlog::warn("Attempting to call generateMipMaps() on unbound texture");

		gl::glGenerateMipmap(GL_TEXTURE_2D);
	}

	void setWrapMode(gl::GLenum mode)
	{
		if (!_isBound)
			spdlog::warn("Attempting to call generateMipMaps() on unbound texture");

		_wrapMode = mode;
		gl::glTexParameteri(gl::GLenum::GL_TEXTURE_2D, gl::GLenum::GL_TEXTURE_WRAP_S, mode);
		gl::glTexParameteri(gl::GLenum::GL_TEXTURE_2D, gl::GLenum::GL_TEXTURE_WRAP_T, mode);
	}
};

typedef opengl_texture_2d<glm::byte, 4> tex2;

#endif
