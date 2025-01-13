#ifndef __SIRENGINE_OPENGL_CONTEXT_H__
#define __SIRENGINE_OPENGL_CONTEXT_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "../RenderContext.h"
#include <SDL2/SDL_opengl.h>

namespace SIREngine::RenderLib::OpenGL {

class GLContext : public IRenderContext
{
public:
	GLContext( const ContextInfo_t& contextInfo );

	virtual void BeginFrame( void ) override;
	virtual void EndFrame( void ) override;

	SDL_GLContext GetContext( void );
private:
	virtual bool CreateWindow( void ) override;
	virtual void ShutdownBackend( void ) override;
	virtual void RegisterBackendCvars( void ) override;

	SDL_GLContext m_pContext;
};

SIRENGINE_DECLARE_LOG_CATEGORY( GLBackend, ELogLevel::Info );

extern GLContext *g_pGLContext;

SIRENGINE_FORCEINLINE SDL_GLContext GLContext::GetContext( void )
{
	return m_pContext;
}

};

#endif