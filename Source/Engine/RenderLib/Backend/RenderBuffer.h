#ifndef __SIRENGINE_RENDERBUFFER_H__
#define __SIRENGINE_RENDERBUFFER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "../RenderCore.h"

namespace SIREngine::RenderLib {

class IRenderBuffer
{
public:
	IRenderBuffer( void );
	IRenderBuffer( const void *pData, size_t nSize );
	virtual ~IRenderBuffer();

	virtual void SetData( const void *pData, size_t nSize ) = 0;
};

};

#endif