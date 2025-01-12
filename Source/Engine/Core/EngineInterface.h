#ifndef __SIRENGINE_ENGINE_INTERFACE_H__
#define __SIRENGINE_ENGINE_INTERFACE_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "Compiler.h"

namespace SIREngine {

class IEngineInterface
{
public:
	IEngineInterface( void )
	{ }
	virtual ~IEngineInterface()
	{ }

	virtual void Init( void ) = 0;
	virtual void Shutdown( void ) = 0;
	virtual void Restart( void ) = 0;
};

};

#endif