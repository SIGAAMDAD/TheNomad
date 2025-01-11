#include "GenericApplication.h"

void *operator new[]( unsigned long size, char const*, int, unsigned int, char const*, int )
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

};