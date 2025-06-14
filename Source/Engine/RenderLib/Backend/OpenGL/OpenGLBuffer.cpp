#include "OpenGLBuffer.h"
#include "ngl.h"

namespace SIREngine::RenderLib::OpenGL {

void GLBuffer::Allocate( const void *pData, size_t nSize, EBufferType nType )
{
	nglCreateBuffers( 1, &m_nBufferID );

	switch ( nType ) {
	case EBufferType::Vertex:
	case EBufferType::Index: {
		nglNamedBufferData( m_nBufferID, nSize, pData, GL_STREAM_DRAW );
		break; }
	case EBufferType::Uniform:
	case EBufferType::ShaderStorage: {
		nglNamedBufferStorage( m_nBufferID, nSize, pData, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT );
		break; }
	};
}

void GLBuffer::Release( void )
{
}

void GLBuffer::Bind( void )
{
}

};