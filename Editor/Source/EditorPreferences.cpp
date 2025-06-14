#include "EditorPreferences.h"
#include <imgui/imgui.h>

namespace Valden {

void CEditorPreferences::Init( void )
{
	m_bOpen = true;
}

void CEditorPreferences::Shutdown( void )
{
	m_bOpen = false;
}

bool CEditorPreferences::Render( void )
{
	if ( ImGui::Begin( "Editor Preferences", &m_bOpen ) ) {
		ImGui::End();
	}

	return m_bOpen;
}

};