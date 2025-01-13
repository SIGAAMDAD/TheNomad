#ifndef __VALDEN_UI_LAYER_H__
#define __VALDEN_UI_LAYER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Compiler.h>

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
	virtual bool Render( void ) = 0;

	SIRENGINE_FORCEINLINE bool IsActive( void ) const
	{
		return m_bOpen;
	}
protected:
	bool m_bOpen;
};

};

#endif