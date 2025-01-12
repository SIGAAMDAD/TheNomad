#include <Engine/Core/Logging/Logger.h>
#include "Threads.h"

namespace SIREngine {

SIRENGINE_DEFINE_LOG_CATEGORY( ThreadSystem, ELogLevel::Info );

CThreadMutex::CThreadMutex( void )
{
	m_pMutex = SDL_CreateMutex();
	if ( !m_pMutex ) {
		SIRENGINE_LOG_LEVEL( ThreadSystem, ELogLevel::Fatal, "Error creating SDL2 mutex: %s", SDL_GetError() );
	}
}

CThreadMutex::~CThreadMutex()
{
	if ( m_pMutex ) {
		SDL_DestroyMutex( m_pMutex );
	}
	m_pMutex = NULL;
}

void CThreadMutex::ErrorCheck( const char *pFunction, int nErrorCode )
{
	if ( nErrorCode == -1 ) {
		SIRENGINE_LOG_LEVEL( ThreadSystem, ELogLevel::Fatal, "%s failed: %s", pFunction, SDL_GetError() );
	}
}

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