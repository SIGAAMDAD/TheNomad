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

enum class EVSyncMode : int32_t
{
	Disabled = -1,
	Adaptive = 0,
	Enabled = 1
};

enum class EAntiAliasType : uint32_t
{
	None,

	FXAA,
	MSAA,
	SSAA,
	TSAA,
	SMAA,
	DLSS
};

enum class Detail : uint32_t
{
	Lowest,
	Low,
	Normal,
	High,
	VeryHigh,
	GPUvsGod
};

enum class TextureFilterMode : uint32_t
{
	Bilinear,
	Nearest,
	Trilinear,
	Anisotropic2X,
	Anisotropic4X,
	Anisotropic8X,
	Anisotropic16X
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
	virtual void RegisterCvars( void ) override;

	virtual void BeginFrame( void ) = 0;
	virtual void EndFrame( void ) = 0;

	static IRenderContext *CreateContext( const ContextInfo_t& contextInfo );
protected:
	virtual bool CreateWindow( void ) = 0;
	virtual void ShutdownBackend( void ) = 0;
	virtual void RegisterBackendCvars( void ) = 0;

	SDL_Window *m_pWindow;
	ContextInfo_t m_ContextData;
};

extern IRenderContext *g_pContext;

extern CVar<uint32_t> r_AntiAliasType;
extern CVar<uint32_t> r_AntiAliasQuality;
extern CVar<uint32_t> r_TextureDetail;
extern CVar<uint32_t> r_TextureFiltering;

extern CVar<int32_t> vid_VSync;
extern CVar<uint32_t> vid_RenderAPI;
extern CVar<uint32_t> vid_WindowMode;

SIRENGINE_DECLARE_LOG_CATEGORY( RenderBackend, ELogLevel::Info );

};

#endif