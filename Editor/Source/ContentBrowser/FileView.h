#ifndef __VALDEN_FILE_VIEW_H__
#define __VALDEN_FILE_VIEW_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Compiler.h>
#include <Engine/Core/Types.h>
#include "ContentView.h"

using namespace SIREngine;

namespace Valden {

typedef struct FileView_s {
	CString DirectoryName;
	CString Path;

	inline bool operator==( const FileView_s& other ) const
	{ return DirectoryName == other.DirectoryName && Path == other.Path; }
} FileView_t;

class CFileView : public IContentView
{
public:
	CFileView( void )
	{ }
	virtual ~CFileView() override
	{ }

	virtual bool IsDirectory( void ) const override;
	virtual void Draw( ImGuiTreeNodeFlags nFlags ) override;
private:
	FileView_t m_Data;
};

SIRENGINE_FORCEINLINE SIRENGINE_CONSTEXPR bool CFileView::IsDirectory( void ) const
{
	return false;
}

};

#endif