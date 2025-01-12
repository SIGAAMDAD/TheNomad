#ifndef __SIRENGINE_FILE_LIST_H__
#define __SIRENGINE_FILE_LIST_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "FilePath.h"
#include "../Compiler.h"

namespace SIREngine {

class CFileList
{
public:
	CFileList( const CFilePath& directory );
	CFileList( void ) = default;
	~CFileList() = default;

	void LoadList( const CFilePath& directory, bool bDirectoriesOnly = false );

	size_t FileCount( void ) const;
	const eastl::vector<CFilePath>& GetList( void ) const;
	const CFilePath& GetPath( void ) const;
	bool IsValid( void ) const;
private:
	CFilePath m_BasePath;
	eastl::vector<CFilePath> m_List;
};

SIRENGINE_FORCEINLINE const CFilePath& CFileList::GetPath( void ) const
{
	return m_BasePath;
}

SIRENGINE_FORCEINLINE size_t CFileList::FileCount( void ) const
{
	return m_List.size();
}

SIRENGINE_FORCEINLINE const eastl::vector<CFilePath>& CFileList::GetList( void ) const
{
	return m_List;
}

SIRENGINE_FORCEINLINE bool CFileList::IsValid( void ) const
{
	return m_BasePath.size() != 0 && FileCount() != 0;
}

};

#endif