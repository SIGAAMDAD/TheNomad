#ifndef __SIRENGINE_OPENGL_BUFFER_H__
#define __SIRENGINE_OPENGL_BUFFER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "../RenderContext.h"
#include "OpenGLContext.h"

namespace SIREngine::RenderLib::OpenGL {

class GLBuffer
{
public:
	GLBuffer( void );
	~GLBuffer();

	void Allocate( const void *pData, size_t nSize, EBufferType nType );
	void Release( void );
	void Bind( void );
private:
	GLuint m_nBufferID;
};

};

#endif