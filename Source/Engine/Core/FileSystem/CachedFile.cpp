#include "CachedFile.h"
#include <Engine/Core/Platform.h>
#include "FileReader.h"

#if defined(SIRENGINE_PLATFORM_POSIX)
#include <sys/mman.h>
#include <fcntl.h>
#endif

namespace SIREngine {

CCachedFile::CCachedFile( void )
	: m_pBuffer( NULL ), m_nSize( 0 )
{
}

CCachedFile::CCachedFile( const CFilePath& filePath )
	: m_pBuffer( NULL ), m_nSize( 0 )
{
	Load( filePath );
}

CCachedFile::~CCachedFile()
{
	Close();
}

bool CCachedFile::Load( const CFilePath& filePath )
{
	m_nSize = 0;
	m_pBuffer = NULL;

	CFileReader reader( filePath );

	if ( !reader.IsOpen() ) {
		SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Warning, "Couldn't load cached file \"%s\"!\n", filePath.c_str() );
		return false;
	}
	m_nSize = reader.GetSize();

#if defined(SIRENGINE_PLATFORM_WINDOWS)
#if (_MSC_VER <= 1900) || WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
	m_pBuffer = VirtualAlloc( NULL, m_nSize, MEM_RESERVE, PAGE_READWRITE );
#else
	m_pBuffer = VirtualAllocFromApp( NULL, m_nSize, MEM_RESERVE, PAGE_READWRITE );
#endif
	if ( m_pBuffer == NULL ) {
		SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Error, "Error mapping file \"%s\"!", filePath.c_str() );
		return false;
	}
#elif defined(SIRENGINE_PLATFORM_POSIX)
	m_pBuffer = mmap( NULL, m_nSize, PROT_READ, MAP_SHARED, fileno( reader.GetStream() ), 0 );
	if ( m_pBuffer == MAP_FAILED ) {
		SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Error, "Error mapping file \"%s\"!", filePath.c_str() );
		return false;
	}
#endif
	SIRENGINE_LOG_LEVEL( FileSystem, ELogLevel::Info, "Loaded cached virtual file \"%s\" (%lu bytes).\n", filePath.c_str(), m_nSize );
	return true;
}

void CCachedFile::Close( void )
{
#if defined(SIRENGINE_PLATFORM_WINDOWS)
	if ( m_pBuffer ) {
		VirtualFree( m_pBuffer, m_nSize, MEM_RELEASE );
	}
#elif defined(SIRENGINE_PLATFORM_POSIX)
	if ( m_pBuffer ) {
		munmap( m_pBuffer, m_nSize );
	}
#endif
	m_pBuffer = NULL;
	m_nSize = 0;
}

};