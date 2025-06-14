#include "ContentBrowser.h"

namespace Valden {

CContentBrowser::CContentBrowser( void )
	: IEditorLayer()
{
}

CContentBrowser::~CContentBrowser()
{
}
	
void CContentBrowser::Init( void )
{
	m_bOpen = true;
}

void CContentBrowser::Shutdown( void )
{
}

void CContentBrowser::AddInstance( const CFilePath& path )
{
	m_WindowList.emplace_back( eastl::make_unique<CBrowserInstance>( path ) );
}

bool CContentBrowser::Render( void )
{
	for ( auto& it : m_WindowList ) {
		it->Draw();
	}
	return true;
}

};