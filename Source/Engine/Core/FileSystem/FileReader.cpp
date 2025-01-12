#include "FileReader.h"
#include <Engine/Core/FileSystem/FileSystem.h>
#include <Engine/Core/Logging/Logger.h>

namespace SIREngine {

CFileReader::CFileReader( const CFilePath& filePath )
	: m_pStream( NULL )
{
	Open( filePath );
}

CFileReader::CFileReader( void )
	: m_pStream( NULL )
{
}

CFileReader::~CFileReader()
{
	Close();
}

bool CFileReader::Open( const CFilePath& filePath )
{
	SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Verbose, "Opening file \"%s\"...", filePath.c_str() );

	m_pStream = g_pFileSystem->OpenFile( filePath.c_str(), EFileMode::Read );
	if ( !m_pStream ) {
		SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Warning, "Couldn't open file \"%s\" in readonly mode!\n", filePath.c_str() );
		return false;
	}

	m_Path = filePath;
	return true;
}

bool CFileReader::Read( void *pBuffer, size_t nBytes )
{
	if ( !m_pStream ) {
		SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Warning, "Attempted read from invalid file stream.\n" );
		return false;
	}
	if ( fread( pBuffer, nBytes, 1, m_pStream ) != nBytes ) {
		return false;
	}
	return true;
}

void CFileReader::Close( void )
{
	if ( !m_pStream ) {
		return;
	}
	fclose( m_pStream );
	m_pStream = NULL;
}

};