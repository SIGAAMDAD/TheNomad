#include "RenderContext.h"

namespace SIREngine::RenderLib {

SIRENGINE_DEFINE_LOG_CATEGORY( RenderBackend, ELogLevel::Info );

IRenderContext::IRenderContext( const ContextInfo_t& contextInfo )
	: m_ContextData( contextInfo )
{
	SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Info, "Initializing RenderBackend...\n" );
	if ( !CreateWindow() ) {
		SIRENGINE_LOG_LEVEL( RenderBackend, ELogLevel::Fatal, "Error creating SDL2 window: %s", SDL_GetError() );
	}
}

};