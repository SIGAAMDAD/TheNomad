#include "OpenGLTexture.h"
#include "ngl.h"

namespace SIREngine::RenderLib::OpenGL {

GLTexture::GLTexture( void )
{
	nglCreateTextures( GL_TEXTURE_2D, 1, &m_nTextureID );
}

GLTexture::~GLTexture()
{
	nglDeleteTextures( 1, &m_nTextureID );
}

bool GLTexture::Load( const CFilePath& path )
{

	nglBindTextureUnit( GL_TEXTURE0, m_nTextureID );

	nglTextureParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	nglTextureParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	nglTextureParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );

	nglActiveTexture( GL_TEXTURE0 );
	nglBindTexture( GL_TEXTURE_2D, m_nTextureID );
	nglBindTexture( GL_TEXTURE_2D, 0 );

	return true;
}

};