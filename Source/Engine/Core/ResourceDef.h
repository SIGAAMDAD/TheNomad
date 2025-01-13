#ifndef __SIRENGINE_RESOURCE_DEF_H__
#define __SIRENGINE_RESOURCE_DEF_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/FileSystem/FilePath.h>

namespace SIREngine {

enum class EResourceType : uint32_t
{
	

	Count
};

class CResourceDef
{
public:
	CResourceDef( void );
	CResourceDef( const CFilePath& path );
	~CResourceDef();

	const CFilePath& GetPath( void ) const;
protected:
	CFilePath m_Path;
};

};

#endif