#include <Engine/Core/Util.h>
#include "RenderContext.h"
#include "Vulkan/VulkanContext.h"
#include "OpenGL/OpenGLContext.h"

namespace SIREngine::RenderLib {

CVar<uint32_t> vid_WindowMode(
	"vid.WindowMode",
	(uint32_t)EWindowMode::Windowed,
	Cvar_Save,
	"Sets the engine's window mode.",
	CVG_RENDERER
);

CVar<uint32_t> vid_RenderAPI(
	"vid.RenderAPI",
	(uint32_t)ERenderAPI::Vulkan,
	Cvar_Save,
	"Sets the engine's rendering api.",
	CVG_RENDERER
);

CVar<int32_t> vid_VSync(
	"vid.VSync",
	(int32_t)EVSyncMode::Disabled,
	Cvar_Save,
	"Sets vertical synchronization mode.",
	CVG_RENDERER
);

CVar<uint32_t> r_AntiAliasType(
	"r.AntiAliasType",
	(uint32_t)EAntiAliasType::None,
	Cvar_Save,
	"Sets the type of antialiasing used in the rendering engine.",
	CVG_RENDERER
);

CVar<uint32_t> r_AntiAliasQuality(
	"r.AntiAliasQuality",
	0,
	Cvar_Save,
	"Sets the quality of antialiasing.",
	CVG_RENDERER
);

IRenderContext *g_pContext;

SIRENGINE_DEFINE_LOG_CATEGORY( RenderBackend, ELogLevel::Info );

IRenderContext::IRenderContext( void )
{
}

IRenderContext::~IRenderContext()
{
}

void IRenderContext::RegisterCvars( void )
{
}

void RegisterCvars( void )
{
	vid_WindowMode.Register();
	vid_RenderAPI.Register();
	vid_VSync.Register();

	r_AntiAliasType.Register();
	r_AntiAliasQuality.Register();
}

void IRenderContext::Init( void )
{
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Fatal, "Error initialization SDL2 Video SubSystem: %s", SDL_GetError() );
	}
	SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Info, "Initializing RenderBackend..." );
	if ( !CreateWindow() ) {
		SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Fatal, "Error creating SDL2 window: %s", SDL_GetError() );
	}
}

void IRenderContext::Shutdown( void )
{
	ShutdownBackend();
	if ( m_pWindow ) {
		SDL_DestroyWindow( m_pWindow );
	}
	m_pWindow = NULL;
	SDL_QuitSubSystem( SDL_INIT_VIDEO );
}

void IRenderContext::Frame( uint32_t nFrameTic )
{
}

void IRenderContext::Restart( void )
{
}

IRenderContext *IRenderContext::CreateContext( const ContextInfo_t& contextInfo )
{
	switch ( ERenderAPI( vid_RenderAPI.GetValue() ) ) {
	case ERenderAPI::OpenGL:
		return new OpenGL::GLContext( contextInfo );
	case ERenderAPI::Vulkan:
		return new Vulkan::VKContext( contextInfo );
	};
	return NULL;
}

};