#include "FileSystem.h"
#include <Engine/Core/Platform.h>
#include <boost/filesystem.hpp>
#include <Engine/Core/Util.h>

namespace SIREngine {

CFileSystem *g_pFileSystem;

SIRENGINE_DEFINE_LOG_CATEGORY( FileSystem, ELogLevel::Info );

CFileSystem::CFileSystem( void )
{
}

void CFileSystem::Init( void )
{
	SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Info, "Initializing FileSystem..." );
	SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Info, "GameDirectory: %s", System::GetCurrentPath().c_str() );

	CFileList *thisDirectory = new CFileList;
	thisDirectory->LoadList( System::GetCurrentPath(), true );

	m_DirectoryCache.reserve( thisDirectory->FileCount() );
	m_DirectoryCache.try_emplace( thisDirectory->GetPath(), thisDirectory );
	for ( const auto& it : thisDirectory->GetList() ) {
		SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Info, "Adding directory \"%s\" to file cache...", it.c_str() );
		m_DirectoryCache.try_emplace( it, new CFileList( it ) );
	}
}

void CFileSystem::Shutdown( void )
{
	for ( auto& it : m_DirectoryCache ) {
		delete it.second;
	}
}

void CFileSystem::Restart( void )
{
	Shutdown();
	Init();
}

void CFileSystem::CreateDirectoryTree( const char *pDirectoryPath )
{
	char szPath[ SIRENGINE_MAX_OSPATH*2+1 ];
	char *ofs;

	SIREngine_strncpyz( szPath, pDirectoryPath, sizeof( szPath ) );

	for ( ofs = szPath + 1; *ofs; ofs++ ) {
		if ( *ofs == SIRENGINE_PATH_SEPERATOR ) {
			// create the directory
			*ofs = '\0';
			System::CreateDirectory( szPath );
			*ofs = SIRENGINE_PATH_SEPERATOR;
		}
	}
}

FILE *CFileSystem::OpenFile( const CFilePath& filePath, EFileMode nMode )
{
	if ( !g_pFileSystem ) {
		return NULL;
	}

	FILE *pStream;
	CFilePath searchPath;

	switch ( nMode ) {
	case EFileMode::Write:
		searchPath = BuildAssetPath( System::GetCurrentPath(), filePath );
		pStream = fopen( searchPath.c_str(), "wb" );
		if ( pStream ) {
			SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Info, "Opened file \"%s\"", searchPath.c_str() );
			return pStream;
		} else {
			CreateDirectoryTree( searchPath.c_str() );

			pStream = fopen( searchPath.c_str(), "wb" );
			if ( !pStream ) {
				SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Warning, "Couldn't find file \"%s\"", filePath.c_str() );
				return NULL;
			}
		}
		break;
	case EFileMode::Read:
		for ( const auto& it : m_DirectoryCache ) {
			searchPath = BuildAssetPath( it.first, filePath );

			SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Info, "Searching for file in \"%s\"...", searchPath.c_str() );

			pStream = fopen( searchPath.c_str(), "rb" );
			if ( pStream ) {
				SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Info, "Opened file \"%s\"", searchPath.c_str() );
				return pStream;
			}
		}
		SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Warning, "Couldn't find file \"%s\"", filePath.c_str() );
		break;
	case EFileMode::Append:
		break;
	default:
		SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Error, "Invalid fileMode \"i\"", (int)nMode );
		return NULL;
	};
	return NULL;
}

const CFilePath CFileSystem::BuildAssetPath( const CFilePath& directory, const CFilePath& fileName )
{
	CFilePath path;

	if ( fileName.size() ) {
		path.sprintf( "%s/%s", directory.c_str(), fileName.c_str() );
	} else {
		return directory;
	}
	return path;
}

};