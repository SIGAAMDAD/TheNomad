#ifndef __SIRENGINE_CACHED_FILE_H__
#define __SIRENGINE_CACHED_FILE_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "FilePath.h"

namespace SIREngine {

class CCachedFile
{
public:
	CCachedFile( void );
	CCachedFile( const CFilePath& filePath );
	~CCachedFile();

	bool Load( const CFilePath& filePath );
	void Close( void );

	bool IsOpen( void ) const;
	size_t GetSize( void ) const;
	const void *GetBuffer( void ) const;
	void *GetBuffer( void );
private:
	CFilePath m_Path;
	void *m_pBuffer;
	size_t m_nSize;
};

SIRENGINE_FORCEINLINE bool CCachedFile::IsOpen( void ) const
{
	return m_pBuffer != NULL;
}

SIRENGINE_FORCEINLINE size_t CCachedFile::GetSize( void ) const
{
	return m_nSize;
}

SIRENGINE_FORCEINLINE const void *CCachedFile::GetBuffer( void ) const
{
	return m_pBuffer;
}

SIRENGINE_FORCEINLINE void *CCachedFile::GetBuffer( void )
{
	return m_pBuffer;
}

};

#endif