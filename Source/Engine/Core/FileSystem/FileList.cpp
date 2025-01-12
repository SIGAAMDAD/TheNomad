#include "FileList.h"
#include "FileSystem.h"
#include <Engine/Core/Compiler.h>
#include <Engine/Core/System/GenericApplication/GenericApplication.h>

#if defined(SIRENGINE_PLATFORM_POSIX)
#include <dirent.h>
#endif

namespace SIREngine {

CFileList::CFileList( const CFilePath& directory )
	: m_BasePath( directory )
{
	LoadList( directory );
}

void CFileList::LoadList( const CFilePath& directory, bool bDirectoriesOnly )
{
	m_List = System::ListFiles( directory, bDirectoriesOnly );
}

};