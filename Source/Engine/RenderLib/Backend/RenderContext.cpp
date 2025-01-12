#include <Engine/Core/Util.h>
#include "RenderContext.h"
#include "Vulkan/VulkanContext.h"

namespace SIREngine::RenderLib {

CVar<uint32_t> r_WindowMode(
	"vid.WindowMode",
	(uint32_t)EWindowMode::BorderlessFullscreen,
	Cvar_Save,
	"Sets the engine's window mode",
	CVG_RENDERER
);

CVar<uint32_t> r_RenderAPI(
	"vid.RenderAPI",
	(uint32_t)ERenderAPI::Vulkan,
	Cvar_Save,
	"Sets the engine's rendering api",
	CVG_RENDERER
);

CVar<int32_t> r_VSync(
	"vid.VSync",
	(int32_t)EVSyncMode::Disabled,
	Cvar_Save,
	"Sets vertical synchronization mode",
	CVG_RENDERER
);

IRenderContext *g_pContext;

SIRENGINE_DEFINE_LOG_CATEGORY( RenderBackend, ELogLevel::Info );

IRenderContext::IRenderContext( void )
{
	r_WindowMode.Register();
	r_RenderAPI.Register();
}

IRenderContext::~IRenderContext()
{
	Shutdown();
}

void IRenderContext::Init( void )
{
	SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Info, "Initializing RenderBackend...\n" );
	if ( !CreateWindow() ) {
		SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Fatal, "Error creating SDL2 window: %s", SDL_GetError() );
	}
}

void IRenderContext::Shutdown( void )
{
	if ( m_pWindow ) {
		SDL_DestroyWindow( m_pWindow );
	}
	m_pWindow = NULL;
}

void IRenderContext::Frame( uint32_t nFrameTic )
{
}

void IRenderContext::Restart( void )
{
}

IRenderContext *IRenderContext::CreateContext( const ContextInfo_t& contextInfo )
{
	switch ( ERenderAPI( r_RenderAPI.GetValue() ) ) {
	case ERenderAPI::OpenGL:
		SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Fatal, "OpenGL API not supported yet!" );
	case ERenderAPI::Vulkan:
		return new Vulkan::VKContext( contextInfo );
	};
	return NULL;
}

};