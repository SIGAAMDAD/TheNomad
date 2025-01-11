#ifndef __SIRENGINE_FILE_LIST_H__
#define __SIRENGINE_FILE_LIST_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "FilePath.h"
#include "../Compiler.h"

namespace SIREngine {

class CFileList : public eastl::vector<CFilePath>
{
public:
	CFileList( const CFilePath& directory );
	~CFileList() = default;

	const CFilePath& GetPath( void ) const;
private:
	CFilePath m_BasePath;
};

SIRENGINE_FORCEINLINE const CFilePath& CFileList::GetPath( void ) const
{
	return m_BasePath;
}

};

#endif