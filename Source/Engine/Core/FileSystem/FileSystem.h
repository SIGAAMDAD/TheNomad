#ifndef __SIRENGINE_FILE_SYSTEM_H__
#define __SIRENGINE_FILE_SYSTEM_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "../Pch.h"
#include "../EngineSystem.h"
#include "FileList.h"
#include "FilePath.h"

namespace SIREngine {

class CFileSystem : public IEngineInterface
{
public:
	CFileSystem( void );

	virtual void Init( void ) override;
	virtual void Shutdown( void ) override;
private:
	eastl::vector_map<CFilePath, CFileList> m_DirectoryCache;
};

};

#endif