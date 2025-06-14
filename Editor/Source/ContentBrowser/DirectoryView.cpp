#include "DirectoryView.h"

namespace Valden {

void CDirectoryView::Draw( ImGuiTreeNodeFlags nFlags )
{
	if ( ImGui::TreeNodeEx( (void *)m_Data.Name.c_str(), nFlags, "" ) ) {
		
	}
}

};