#ifndef __OPENGL_TEXTURE_H__
#define __OPENGL_TEXTURE_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/FileSystem/FilePath.h>

namespace SIREngine::RenderLib::OpenGL {

class GLTexture
{
public:
	GLTexture( void );
	~GLTexture();

	bool Load( const CFilePath& path );
private:
	GLuint m_nTextureID;
};

};

#endif