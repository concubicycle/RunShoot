#ifndef __TEXTURE_FILTERING_MODE_H__
#define __TEXTURE_FILTERING_MODE_H__

#include "opengl_afx.h"

enum TextureFilteringMode
{
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
	LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
	NearestMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
	LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
	NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR
};


#endif