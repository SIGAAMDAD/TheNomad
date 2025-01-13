#ifndef __VALDEN_FILE_TREE_H__
#define __VALDEN_FILE_TREE_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/FileSystem/FileList.h>
#include "DirectoryView.h"
#include "FileView.h"

using namespace SIREngine;

namespace Valden {

class CFileTree : public CFileList
{
public:
	CFileTree( void );
	~CFileTree();

	void Draw( void );
private:
	eastl::unordered_map<CFilePath, IContentView> m_DirectoryList;
};

};

#endif
