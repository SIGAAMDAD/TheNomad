#include "PosixApplication.h"
#include <Engine/Core/Pch.h>
#include <Engine/Core/Util.h>
#include <Engine/Core/Platform.h>
#include <Engine/Core/Logging/Logger.h>
#include <Engine/Core/FileSystem/FileSystem.h>
#include <Engine/Core/ConsoleManager.h>
#include <sys/stat.h>
#include <errno.h>
#include <libgen.h>
#include <dirent.h>

namespace SIREngine::System {

static CFilePath CurrentPath;

SIRENGINE_DEFINE_LOG_CATEGORY( System, ELogLevel::Info );

PosixApplication::PosixApplication( void )
{
}

PosixApplication::~PosixApplication()
{
	g_bExitApp.store( true );
	g_pConsoleManager->SaveConfig( "config.ini" );
	CLogManager::ShutdownLogger();
}

void PosixApplication::Run( void )
{
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

void PosixApplication::Init( void )
{
	g_bExitApp.store( false );

	static CFileSystem fileSystem;
	g_pFileSystem = &fileSystem;

	static CConsoleManager consoleManager;
	g_pConsoleManager = &consoleManager;

	GetCurrentPath();

	g_pFileSystem->Init();

	CLogManager::LaunchLoggingThread();
	g_pConsoleManager->LoadConfig( "config.ini" );
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

};

int main( int argc, char **argv )
{
	eastl::vector<CString> commandLine;

	commandLine.reserve( argc );
	for ( int i = 0; i < argc; i++ ) {
		commandLine.emplace_back( argv[i] );
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