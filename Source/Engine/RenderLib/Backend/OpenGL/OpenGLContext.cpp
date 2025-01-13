#include "OpenGLContext.h"
#include "ngl.h"

extern void GL_LoadProcs( void );

namespace SIREngine::RenderLib::OpenGL {

GLContext *g_pGLContext;

SIRENGINE_DEFINE_LOG_CATEGORY( GLBackend, ELogLevel::Info );

GLContext::GLContext( const ContextInfo_t& contextInfo )
	: IRenderContext()
{
	m_ContextData = contextInfo;
}

bool GLContext::CreateWindow( void )
{
	Uint32 flags;
	uint32_t nBorderMode;

	g_pGLContext = dynamic_cast<GLContext *>( g_pContext );

	flags = SDL_WINDOW_OPENGL;
	nBorderMode = vid_WindowMode.GetValue();

	if ( nBorderMode % 2 ) {
		// its borderless
		SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Info, "Creating borderless window." );
		flags |= SDL_WINDOW_BORDERLESS;
	}
	if ( vid_WindowMode.GetValue() >= (uint32_t)EWindowMode::Fullscreen ) {
		SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Info, "Creating fullscreen window." );
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	} else {
	}
	flags |= SDL_WINDOW_SHOWN;

	m_pWindow = SDL_CreateWindow( m_ContextData.pszWindowName, m_ContextData.nWindowPositionX, m_ContextData.nWindowPositionY,
		m_ContextData.nWindowWidth, m_ContextData.nWindowHeight, flags );
	if ( !m_pWindow ) {
		SIRENGINE_LOG_LEVEL( GLBackend, ELogLevel::Fatal, "Error creating SDL2 OpenGL window: %s", SDL_GetError() );
	}
	
	m_pContext = SDL_GL_CreateContext( m_pWindow );
	if ( !m_pContext ) {
		SIRENGINE_LOG_LEVEL( GLBackend, ELogLevel::Fatal, "Error creating SDL2 OpenGL Context: %s", SDL_GetError() );
	}

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 5 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, SDL_TRUE );
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, SDL_TRUE );

	SDL_GL_MakeCurrent( m_pWindow, m_pContext );
	SDL_GL_SetSwapInterval( vid_VSync.GetValue() );

	GL_LoadProcs();

	return true;
}

void GLContext::BeginFrame( void )
{
	SDL_GL_MakeCurrent( m_pWindow, m_pContext );
	nglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	nglViewport( 0, 0, m_ContextData.nWindowWidth, m_ContextData.nWindowHeight );
	nglScissor( 0, 0, m_ContextData.nWindowWidth, m_ContextData.nWindowHeight );

	nglDisable( GL_STENCIL_TEST );
	nglEnable( GL_DEPTH_TEST );
	nglEnable( GL_SCISSOR_TEST );
	nglEnable( GL_BLEND );
}

void GLContext::EndFrame( void )
{
	SDL_GL_MakeCurrent( m_pWindow, m_pContext );
	nglFlush();
	SDL_GL_SwapWindow( m_pWindow );
}

void GLContext::ShutdownBackend( void )
{
	SDL_GL_DeleteContext( m_pContext );
}

void GLContext::RegisterBackendCvars( void )
{
}

};