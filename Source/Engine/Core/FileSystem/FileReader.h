#ifndef __SIRENGINE_FILE_READER_H__
#define __SIRENGINE_FILE_READER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/FileSystem/FilePath.h>

namespace SIREngine {

class CFileReader
{
public:
	CFileReader( const CFilePath& filePath );
	CFileReader( void );
	~CFileReader();

	bool Open( const CFilePath& filePath );
	void Close( void );
	size_t GetSize( void ) const;
	FILE *GetStream( void );

	bool Read( void *pBuffer, size_t nBytes );

	bool IsOpen( void ) const;
	const CFilePath& GetPath( void ) const;
private:
	CFilePath m_Path;
	FILE *m_pStream;
};

SIRENGINE_FORCEINLINE FILE *CFileReader::GetStream( void )
{
	return m_pStream;
}

SIRENGINE_FORCEINLINE size_t CFileReader::GetSize( void ) const
{
	if ( !m_pStream ) {
		return 0;
	}

	fseek( m_pStream, 0L, SEEK_END );
	size_t nSize = ftell( m_pStream );
	fseek( m_pStream, 0L, SEEK_SET );

	return nSize;
}

SIRENGINE_FORCEINLINE const CFilePath& CFileReader::GetPath( void ) const
{
	return m_Path;
}

SIRENGINE_FORCEINLINE bool CFileReader::IsOpen( void ) const
{
	return m_pStream != NULL;
}

};

#endif