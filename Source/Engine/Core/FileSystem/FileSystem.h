#ifndef __SIRENGINE_FILE_SYSTEM_H__
#define __SIRENGINE_FILE_SYSTEM_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Pch.h>
#include <Engine/Core/EngineSystem.h>
#include "FileList.h"
#include "FilePath.h"
#include <Engine/Core/Logging/Logger.h>

namespace SIREngine {

enum class EFileMode : uint8_t
{
	Read,
	Write,
	Append
};

class CFileSystem : public IEngineInterface
{
public:
	CFileSystem( void );
	virtual ~CFileSystem() = default;

	virtual void Init( void ) override;
	virtual void Shutdown( void ) override;
	virtual void Restart( void ) override;

	void CreateDirectoryTree( const char *pDirectoryPath );
	FILE *OpenFile( const CFilePath& filePath, EFileMode nMode );
	const CFilePath BuildAssetPath( const CFilePath& directory, const CFilePath& fileName );
private:
	eastl::unordered_map<CFilePath, CFileList *> m_DirectoryCache;
};

extern CFileSystem *g_pFileSystem;

SIRENGINE_DECLARE_LOG_CATEGORY( FileSystem, ELogLevel::Info );

};

#endif