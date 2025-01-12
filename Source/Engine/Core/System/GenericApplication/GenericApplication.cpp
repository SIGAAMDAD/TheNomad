#include "GenericApplication.h"

void *operator new[]( unsigned long size, char const*, int, unsigned int, char const*, int )
{
	return ::operator new[]( size );
}

void *operator new[](unsigned long size, unsigned long, unsigned long, char const*, int, unsigned int, char const*, int)
{
	return ::operator new[]( size );
}

namespace SIREngine::System {

IGenericApplication *g_pApplication = NULL;
CThreadAtomic<bool> g_bExitApp;

IGenericApplication::IGenericApplication( void )
{
	g_bExitApp.store( false );
}

IGenericApplication::~IGenericApplication()
{
	g_bExitApp.store( true );
}

void IGenericApplication::QuitGame( const Events::IEventBase *pEventData )
{
	assert( pEventData->GetType() == Events::EventType_Quit );
	delete g_pApplication;
}

};