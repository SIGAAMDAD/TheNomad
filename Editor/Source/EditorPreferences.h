#ifndef __EDITOR_CONFIG_H__
#define __EDITOR_CONFIG_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "EditorLayer.h"

namespace Valden {

class CEditorPreferences : public IEditorLayer
{
public:
	CEditorPreferences( void )
	{ }

	virtual void Init( void ) override;
	virtual void Shutdown( void ) override;
	virtual bool Render( void ) override;
private:
};

};

#endif