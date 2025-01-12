#ifndef __SIRENGINE_GENERIC_APPLICATION_H__
#define __SIRENGINE_GENERIC_APPLICATION_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Pch.h>
#include <Engine/Core/Types.h>
#include <Engine/Core/ThreadSystem/Threads.h>
#include <Engine/Core/FileSystem/FileSystem.h>
#include <Engine/Core/EngineInterface.h>

namespace SIREngine::System {

typedef void* FileHandle_t;

class IGenericApplication
{
public:
	IGenericApplication( void );
	virtual ~IGenericApplication();

	virtual void Init( void ) = 0;
	virtual void Run( void ) = 0;
};

bool CreateDirectory( const char *pDirectory );
size_t FileWrite( const void *pBuffer, size_t nBytes, FileHandle_t hFile );
size_t FileRead( void *pBuffer, size_t nBytes, FileHandle_t hFile );
const CFilePath& GetCurrentPath( void );
const eastl::vector<CString>& GetCommandLine( void );
bool CheckCommandParm( const CString& name );
eastl::vector<CFilePath> ListFiles( const CFilePath& directory, bool bDirectoryOnly );

extern IGenericApplication *g_pApplication;
extern CThreadAtomic<bool> g_bExitApp;

};

#endif