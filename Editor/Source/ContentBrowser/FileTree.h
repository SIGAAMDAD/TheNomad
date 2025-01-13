#ifndef __VALDEN_FILE_TREE_H__
#define __VALDEN_FILE_TREE_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/FileSystem/FileList.h>

using namespace SIREngine;

namespace Valden {

class CFileTree : public CFileList
{
public:
	CFileTree( void );
	~CFileTree();

	void Draw( void );
};

};

#endif
