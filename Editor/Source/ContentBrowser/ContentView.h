#ifndef __VALDEN_ASSET_VIEW_H__
#define __VALDEN_ASSET_VIEW_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/FileSystem/FilePath.h>
#include <imgui/imgui.h>

using namespace SIREngine;

namespace Valden {

class IContentView
{
public:
	IContentView( void )
	{ }
	virtual ~IContentView()
	{ }

	virtual bool IsDirectory( void ) const = 0;
	virtual void Draw( ImGuiTreeNodeFlags nFlags ) = 0;
protected:
	CFilePath m_Path;
};

};

#endif