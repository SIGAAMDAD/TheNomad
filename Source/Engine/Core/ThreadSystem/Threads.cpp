#include <Engine/Core/Logging/Logger.h>
#include "Threads.h"

namespace SIREngine {

SIRENGINE_DEFINE_LOG_CATEGORY( ThreadSystem, ELogLevel::Info );

void CThread::Run( int (*hFunction)( void * ), const CString& name, void *pData )
{
	m_pThread = SDL_CreateThread( hFunction, name.c_str(), pData );
	if ( !m_pThread ) {
		SIRENGINE_LOG_LEVEL( ThreadSystem, ELogLevel::Fatal, "Error creating thread \"%s\": %s", name.c_str(), SDL_GetError() );
	}
}

void CThread::Join( uint64_t nTimeout )
{
	if ( !m_pThread ) {
		return;
	}
	
	int threadStatus;
	SDL_WaitThread( m_pThread, &threadStatus );
}

};