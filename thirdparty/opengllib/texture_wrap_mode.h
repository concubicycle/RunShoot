#ifndef __TEXTURE_WRAP_MODE_H__
#define __TEXTURE_WRAP_MODE_H__

#include "opengl_afx.h"
using namespace gl;

enum TextureWrapMode
{
	Repeat = GL_REPEAT,
	MirroredRepeat = GL_MIRRORED_REPEAT,
	ClampToEdge = GL_CLAMP_TO_EDGE,
	ClampToBorder = GL_CLAMP_TO_BORDER
};

#endif