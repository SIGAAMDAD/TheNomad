#ifndef __SIRENGINE_RENDER_CONTEXT_H__
#define __SIRENGINE_RENDER_CONTEXT_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Core.h>
#include <Engine/Core/EngineSystem.h>
#include <Engine/Core/ConsoleManager.h>

#include <SDL2/SDL.h>

namespace SIREngine::RenderLib {

typedef struct ContextInfo_s {
	const char *pszWindowName;
	uint32_t nAppVersion;
	uint32_t nWindowWidth;
	uint32_t nWindowHeight;
	uint32_t nWindowPositionX;
	uint32_t nWindowPositionY;
} ContextInfo_t;

enum class ERenderAPI : uint32_t
{
	OpenGL,
	Vulkan,
	SDL2_Software,
	DirectX11,
	DirectX12
};

enum class EWindowMode : uint32_t
{
	Windowed,
	BorderlessWindowed,
	Fullscreen,
	BorderlessFullscreen
};

class IRenderContext : public IEngineSystem
{
public:
	IRenderContext( void );
	virtual ~IRenderContext();

	virtual void Init( void ) override;
	virtual void Shutdown( void ) override;
	virtual void Frame( uint32_t nFrameTic ) override;
	virtual void Restart( void ) override;

	static IRenderContext *CreateContext( const ContextInfo_t& contextInfo );
protected:
	virtual bool CreateWindow( void ) = 0;
	virtual void ShutdownBackend( void ) = 0;

	SDL_Window *m_pWindow;
	ContextInfo_t m_ContextData;
};

extern CVar<uint32_t> r_RenderAPI;
extern CVar<uint32_t> r_WindowMode;

SIRENGINE_DECLARE_LOG_CATEGORY( RenderBackend, ELogLevel::Info );

};

#endif