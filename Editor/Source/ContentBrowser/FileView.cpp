#include "FileView.h"

namespace Valden {

void CFileView::Draw( ImGuiTreeNodeFlags nFlags )
{
	if ( ImGui::TreeNodeEx( (void *)m_Data.DirectoryName.c_str(), nFlags ))
}

};