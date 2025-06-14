#ifndef __VALDEN_CONTENT_BROWSER_H__
#define __VALDEN_CONTENT_BROWSER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "EditorLayer.h"
#include "ContentBrowser/FileTree.h"
#include "ContentBrowser/BrowserInstance.h"

namespace Valden {

class CContentBrowser : public IEditorLayer
{
public:
	CContentBrowser( void );
	virtual ~CContentBrowser() override;

	virtual void Init( void ) override;
	virtual void Shutdown( void ) override;
	virtual bool Render( void ) override;

	void AddInstance( const CFilePath& path );
private:
	eastl::vector<eastl::unique_ptr<CBrowserInstance>> m_WindowList;
};

extern eastl::shared_ptr<CContentBrowser> g_pContentBrowser;

};

#endif