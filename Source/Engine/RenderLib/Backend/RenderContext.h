#ifndef __SIRENGINE_RENDER_CONTEXT_H__
#define __SIRENGINE_RENDER_CONTEXT_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Core.h>
#include <Engine/Core/EngineSystem.h>

#include <SDL2/SDL.h>

namespace SIREngine::RenderLib {

typedef struct ContextInfo_s {
	const char *pszWindowName;
	uint32_t nWindowWidth;
	uint32_t nWindowHeight;
	uint32_t nWindowPositionX;
	uint32_t nWindowPositionY;
} ContextInfo_t;

class IRenderContext : public IEngineSystem
{
public:
	IRenderContext( const ContextInfo_t& contextInfo );
	virtual ~IRenderContext();

	virtual void Init( void ) override;
	virtual void Shutdown( void ) override;
	virtual void Frame( uint32_t nFrameTic ) override;
	virtual void Restart( void ) override;
private:
	virtual bool CreateWindow( void ) = 0;

	SDL_Window *m_pWindow;
	ContextInfo_t m_ContextData;
};

SIRENGINE_DECLARE_LOG_CATEGORY( RenderBackend, ELogLevel::Info );

};

#endif