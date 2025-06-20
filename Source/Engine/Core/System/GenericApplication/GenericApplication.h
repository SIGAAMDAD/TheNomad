#ifndef __SIRENGINE_GENERIC_APPLICATION_H__
#define __SIRENGINE_GENERIC_APPLICATION_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Pch.h>
#include <Engine/Core/Types.h>
#include <Engine/Core/Compiler.h>
#include <Engine/Core/ThreadSystem/Threads.h>
#include <Engine/Core/FileSystem/FileSystem.h>
#include <Engine/Core/EngineInterface.h>
#include <Engine/Core/Events/EventBase.h>

namespace SIREngine::System {

typedef void* FileHandle_t;

class IGenericApplication
{
public:
	IGenericApplication( void );
	virtual ~IGenericApplication();

	virtual void Init( void ) = 0;
	virtual void Run( void ) = 0;

	static void QuitGame( const Events::IEventBase *pEventData );
};

typedef struct FileStats_s {
	uint64_t ModificationTime;
	uint64_t CreationTime;
	uint64_t Size;
} FileStats_t;

bool GetFileStats( FileStats_t *pStats, const CFilePath& path );
void Error( const char *pError );
bool CreateDirectory( const char *pDirectory );
size_t FileWrite( const void *pBuffer, size_t nBytes, FileHandle_t hFile );
size_t FileRead( void *pBuffer, size_t nBytes, FileHandle_t hFile );
const CFilePath& GetCurrentPath( void );
const eastl::vector<CString>& GetCommandLine( void );
bool CheckCommandParm( const CString& name );
eastl::vector<CFilePath> ListFiles( const CFilePath& directory, bool bDirectoryOnly );

extern IGenericApplication *g_pApplication;
extern CThreadAtomic<bool> g_bExitApp;

SIRENGINE_DECLARE_LOG_CATEGORY( System, ELogLevel::Info );

};

#endif