#ifndef __SIRENGINE_FILE_WRITER_H__
#define __SIRENGINE_FILE_WRITER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/FileSystem/FilePath.h>

namespace SIREngine {

class CFileWriter
{
public:
	CFileWriter( const CFilePath& filePath );
	CFileWriter( void );
	~CFileWriter();

	bool Open( const CFilePath& filePath );
	void Close( void );

	size_t Write( const void *pBuffer, size_t nBytes );
	void Printf( const char *fmt, ... ) SIRENGINE_ATTRIBUTE(format(printf, 2, 3));

	bool IsOpen( void ) const;
	const CFilePath& GetPath( void ) const;
private:
	CFilePath m_Path;
	FILE *m_pStream;
};

SIRENGINE_FORCEINLINE const CFilePath& CFileWriter::GetPath( void ) const
{
	return m_Path;
}

SIRENGINE_FORCEINLINE bool CFileWriter::IsOpen( void ) const
{
	return m_pStream != NULL;
}

};

#endif