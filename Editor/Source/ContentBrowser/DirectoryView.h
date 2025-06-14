#ifndef __VALDEN_DIRECTORY_VIEW_H__
#define __VALDEN_DIRECTORY_VIEW_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Compiler.h>
#include <Engine/Core/Types.h>
#include <Engine/Core/FileSystem/FilePath.h>
#include "FileView.h"
#include "ContentView.h"

using namespace SIREngine;

namespace Valden {

typedef struct DirectoryView_s {
	CFilePath Path;
	CString Name;
	DirectoryView_s *Parent;
	eastl::vector<DirectoryView_s *> Children;
	eastl::vector<FileView_t *> FileList;

	inline bool operator==( const DirectoryView_s& other ) const
	{ return Path == other.Path && Name == other.Name; }
	inline bool HasChild( const DirectoryView_s *fileView ) const
	{ return eastl::find( Children.cbegin(), Children.cend(), fileView ) != Children.cend(); }
	inline bool HasParent( DirectoryView_s *directory )
	{
		for ( DirectoryView_s *it = Parent; it != NULL; it = it->Parent ) {
			if ( it == directory ) {
				return true;
			}
		}
		return false;
	}
} DirectoryView_t;

class CDirectoryView : public IContentView
{
public:
	CDirectoryView( void )
	{ }
	virtual ~CDirectoryView() override
	{ }

	virtual bool IsDirectory( void ) const override;
	virtual void Draw( ImGuiTreeNodeFlags nFlags ) override;

	DirectoryView_t& GetData( void );
private:
	DirectoryView_t m_Data;
};

SIRENGINE_FORCEINLINE bool CDirectoryView::IsDirectory( void ) const
{
	return true;
}

SIRENGINE_FORCEINLINE DirectoryView_t& CDirectoryView::GetData( void )
{
	return m_Data;
}

};

#endif