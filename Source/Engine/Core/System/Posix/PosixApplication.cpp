#include "PosixApplication.h"
#include <Engine/Core/Pch.h>
#include <Engine/Core/Util.h>
#include <Engine/Core/Platform.h>
#include <Engine/Core/Logging/Logger.h>
#include <Engine/Core/FileSystem/FileSystem.h>
#include <Engine/Core/ConsoleManager.h>
#include <Engine/RenderLib/Backend/RenderContext.h>
#include <Engine/Core/Events/EventManager.h>
#include <Engine/Core/Input/InputManager.h>
#include <sys/stat.h>
#include <errno.h>
#include <libgen.h>
#include <dirent.h>
#include <signal.h>

namespace SIREngine::System {

static CFilePath CurrentPath;
static eastl::vector<CString> CommandLine;
int myargc;
char **myargv;

SIRENGINE_DEFINE_LOG_CATEGORY( System, ELogLevel::Info );

extern "C" void InitCrashHandler( void );
extern "C" void DumpStacktrace( void );

PosixApplication::PosixApplication( void )
{
}

PosixApplication::~PosixApplication()
{
	g_bExitApp.store( true );

	g_pConsoleManager->SaveConfig( "Config/config.json" );
	Input::g_pInputManager->Shutdown();
	Events::g_pEventManager->Shutdown();
	RenderLib::g_pContext->Shutdown();
	delete RenderLib::g_pContext;
	CLogManager::ShutdownLogger();
	g_pFileSystem->Shutdown();

	g_pApplication = NULL;
}

void PosixApplication::Run( void )
{
	while ( !g_bExitApp.load() ) {
		Events::g_pEventManager->Frame( 0 );
		if ( g_bExitApp.load() ) {
			continue;
		}
		Input::g_pInputManager->Frame( 0 );

		RenderLib::g_pContext->BeginFrame();

		RenderLib::g_pContext->EndFrame();
	}
}

void Error( const char *pError )
{
	FileWrite( pError, strlen( pError ), SIRENGINE_STDERR_HANDLE );

	DumpStacktrace();
	delete g_pApplication;

	_Exit( EXIT_FAILURE );
}

size_t FileWrite( const void *pBuffer, size_t nBytes, FileHandle_t hFile )
{
	return write( (int)(uintptr_t)hFile, pBuffer, nBytes );
}

size_t FileRead( void *pBuffer, size_t nBytes, FileHandle_t hFile )
{
	return read( (int)(uintptr_t)hFile, pBuffer, nBytes );
}

const CFilePath& GetCurrentPath( void )
{
	if ( CurrentPath.size() ) {
		return CurrentPath;
	}

	char pwd[ SIRENGINE_MAX_OSPATH ];

	// more reliable, linux-specific
	if ( readlink( "/proc/self/exe", pwd, sizeof( pwd ) - 1 ) != -1 ) {
		pwd[ sizeof( pwd ) - 1 ] = '\0';
		dirname( pwd );
		CurrentPath = pwd;
		return CurrentPath;
	}
	if ( !getcwd( pwd, sizeof( pwd ) ) ) {
		*pwd = '\0';
	}
	CurrentPath = pwd;

	return CurrentPath;
}

static void Catch_Signal( int signum )
{
	SIRENGINE_LOG_LEVEL( System, ELogLevel::Info, "Caught signal %i from system", signum );
	switch ( signum ) {
	case SIGINT:
	case SIGKILL:
	case SIGSTOP:
		g_bExitApp.store( true );
		break;
	};
}

void PosixApplication::Init( void )
{
	g_bExitApp.store( false );

	InitCrashHandler();

	static CFileSystem fileSystem;
	g_pFileSystem = &fileSystem;

	static CConsoleManager consoleManager;
	g_pConsoleManager = &consoleManager;

	static Events::CEventManager eventManager;
	Events::g_pEventManager = &eventManager;

	static Input::CInputManager inputManager;
	Input::g_pInputManager = &inputManager;

	RenderLib::ContextInfo_t contextInfo;
	contextInfo.pszWindowName = "TheNomad v1.2.0";
	contextInfo.nWindowPositionX = SDL_WINDOWPOS_CENTERED;
	contextInfo.nWindowPositionY = SDL_WINDOWPOS_CENTERED;
	contextInfo.nWindowWidth = 1280;
	contextInfo.nWindowHeight = 720;
	contextInfo.nAppVersion = SIRENGINE_MAKE_VERSION( 1, 2, 0 );
	RenderLib::g_pContext = RenderLib::IRenderContext::CreateContext( contextInfo );

	GetCurrentPath();

	signal( SIGKILL, Catch_Signal );
	signal( SIGINT, Catch_Signal );
	signal( SIGSTOP, Catch_Signal );

	// initialize the filesystem first so that we can open a logfile
	g_pFileSystem->Init();
	CLogManager::LaunchLoggingThread();

	RenderLib::g_pContext->RegisterCvars();
	Events::g_pEventManager->RegisterCvars();
	Input::g_pInputManager->RegisterCvars();

	// now load the configuration to overwrite any cvars
	g_pConsoleManager->LoadConfig( "config.json" );

	// initialize the rest of the engine
	RenderLib::g_pContext->Init();
	Events::g_pEventManager->Init();
	Input::g_pInputManager->Init();

	Events::g_pEventManager->AddEventListener( eastl::make_shared<Events::CEventListener>(
		"ApplicationListener", Events::EventType_Quit, IGenericApplication::QuitGame
	) );

	SIRENGINE_LOG_LEVEL( System, ELogLevel::Info, "SIREngine MetaData:" );
	SIRENGINE_LOG_LEVEL( System, ELogLevel::Info, "  Engine Version: " SIRENGINE_VERSION_STRING );
}

bool CreateDirectory( const char *pDirectory )
{
	int ret = mkdir( pDirectory, 0777 );
	if ( mkdir( pDirectory, 0777 ) != 0 ) {
		if ( errno == EEXIST ) {
			return true;
		} else {
			SIRENGINE_LOG_LEVEL( System, ELogLevel::Error, "CreateDirectory: Error creating directory \"%s\" - %s",
				pDirectory, strerror( errno ) );
		}
	}
	return true;
}

eastl::vector<CFilePath> ListFiles( const CFilePath& directory, bool bDirectoryOnly )
{
	eastl::vector<CFilePath> files;
	DIR *dir;
	char szSearchPath[ SIRENGINE_MAX_OSPATH*2+1 ];
	char szDirectory[ SIRENGINE_MAX_OSPATH*2+1 ];
	struct dirent *d;
	struct stat buf;

	if ( ( dir = opendir( directory.c_str() ) ) == NULL ) {
		SIRENGINE_LOG_LEVEL( System, ELogLevel::Error, "Error opening directory \"%s\": %s", directory.c_str(), strerror( errno ) );
		return files;
	}

	while ( ( d = readdir( dir ) ) != NULL ) {
		SIREngine_snprintf( szSearchPath, sizeof( szSearchPath ) - 1, "%s/%s", directory.c_str(), d->d_name );
		if ( stat( szSearchPath, &buf ) == -1 ) {
			continue;
		}

		if ( ( bDirectoryOnly && !( buf.st_mode & S_IFDIR ) ) || ( !bDirectoryOnly && ( buf.st_mode & S_IFDIR ) )
			|| ( bDirectoryOnly && ( buf.st_mode & S_IFREG ) ) )
		{
			continue;
		}
		else if ( d->d_name[0] == '.' || ( d->d_name[0] == '.' && d->d_name[1] == '.' ) ) {
			continue;
		}

		files.emplace_back( szSearchPath );
	}
	closedir( dir );

	// sort alphabetically
	eastl::sort( files.begin(), files.end(), [&]( const CFilePath& a, const CFilePath& b ) -> bool {
		return a > b;    
	} );

	return files;
};

const eastl::vector<CString>& GetCommandLine( void )
{
	return CommandLine;
}

bool CheckCommandParm( const CString& name )
{
	return eastl::find( CommandLine.cbegin(), CommandLine.cend(), name ) != CommandLine.cend();
}

};

int main( int argc, char **argv )
{
	using namespace SIREngine::System;

	myargc = argc;
	myargv = argv;

	CommandLine.reserve( argc );
	for ( int i = 0; i < argc; i++ ) {
		CommandLine.emplace_back( argv[i] );
	}

	g_pApplication = new PosixApplication();

	seteuid( getuid() );
	setenv( "LC_NUMERIC", "C", 1 );

	g_pApplication->Init();
	g_pApplication->Run();
	if ( g_pApplication ) {
		delete g_pApplication;
	}

	_Exit( EXIT_SUCCESS );

	// never reached
	return EXIT_SUCCESS;
}