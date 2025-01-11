#include "PosixApplication.h"
#include <Engine/Core/Platform.h>
#include <Engine/Core/Logging/Logger.h>

namespace SIREngine::System {

PosixApplication::PosixApplication( void )
{
}

PosixApplication::~PosixApplication()
{
}

void PosixApplication::Run( void )
{
}

void PosixApplication::Init( void )
{
	CLogManager::LaunchLoggingThread();
}

};

int main( int argc, char **argv )
{
	eastl::vector<CString> commandLine;

	for ( int i = 0; i < argc; i++ ) {

	}

	using namespace SIREngine::System;
	g_pApplication = new PosixApplication();

	seteuid( getuid() );
	setenv( "LC_NUMERIC", "C", 1 );

	g_pApplication->Init();
	g_pApplication->Run();
	delete g_pApplication;

	_Exit( EXIT_SUCCESS );

	// never reached
	return EXIT_SUCCESS;
}