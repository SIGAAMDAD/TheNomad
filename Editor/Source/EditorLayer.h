#ifndef __VALDEN_UI_LAYER_H__
#define __VALDEN_UI_LAYER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

namespace Valden {

class IEditorLayer
{
public:
	IEditorLayer( void )
		: m_bOpen( false )
	{ }
	virtual ~IEditorLayer()
	{ }

	virtual void Init( void ) = 0;
	virtual void Shutdown( void ) = 0;
	virtual void Render( void ) = 0;
protected:
	bool m_bOpen;
};

};

#endif