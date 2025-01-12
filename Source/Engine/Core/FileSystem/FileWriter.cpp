#include "FileWriter.h"
#include <Engine/Core/Compiler.h>
#include <Engine/Core/FileSystem/FileSystem.h>
#include <Engine/Core/Logging/Logger.h>

namespace SIREngine {


CFileWriter::CFileWriter( const CFilePath& filePath )
	: m_pStream( NULL )
{
	Open( filePath );
}

CFileWriter::CFileWriter( void )
	: m_pStream( NULL )
{
}

CFileWriter::~CFileWriter()
{
	Close();
}

bool CFileWriter::Open( const CFilePath& filePath )
{
	SIRENGINE_NOTIFICATION( "Opening file \"%s\"...\n", filePath.c_str() );

	m_pStream = g_pFileSystem->OpenFile( filePath.c_str(), EFileMode::Write );
	if ( !m_pStream ) {
		SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Warning, "Couldn't open file \"%s\" writeonly mode!\n", filePath.c_str() );
		return false;
	}

	m_Path = filePath;
	return true;
}

void CFileWriter::Close( void )
{
	if ( !m_pStream ) {
		return;
	}
	fclose( m_pStream );
	m_pStream = NULL;
}

size_t CFileWriter::Write( const void *pBuffer, size_t nBytes )
{
    const uint8_t *buf = (const uint8_t *)pBuffer;
    size_t nRemaining, nBlockSize, nWritten;
    int tries;

    if ( m_pStream == NULL ) {
        return 0;
    }
    if ( !pBuffer || !nBytes ) {
        return 0;
    }

    nRemaining = nBytes;
    tries = 0;

    while ( nRemaining ) {
        nBlockSize = nRemaining;
        nWritten = fwrite( buf, 1, nBlockSize, m_pStream );
        if ( nWritten == 0 ) {
            if ( !tries ) {
                tries = 1;
            } else {
                SIRENGINE_WARNING( "CFileWriter::Write: wrote 0 bytes" );
                return 0;
            }
        }

        buf += nWritten;
        nRemaining -= nWritten;
    }

    return nBytes;
}

void SIRENGINE_ATTRIBUTE(format(printf, 2, 3)) CFileWriter::Printf( const char *fmt, ... )
{
    va_list argptr;
    char msg[8192];
    int length;

    va_start( argptr, fmt );
    length = SIREngine_Vsnprintf( msg, sizeof( msg ) - 1, fmt, argptr );
    va_end( argptr );

    Write( msg, length );
}

};