#ifndef __SIRENGINE_ENGINE_SYSTEM_H__
#define __SIRENGINE_ENGINE_SYSTEM_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "Compiler.h"
#include "EngineInterface.h"

namespace SIREngine {

class IEngineSystem : public IEngineInterface
{
public:
	IEngineSystem( void )
	{ }
	virtual ~IEngineSystem()
	{ }

	virtual void Init( void ) = 0;
	virtual void Shutdown( void ) = 0;
	virtual void Restart( void ) = 0;
	virtual void Frame( uint32_t nFrameTic ) = 0;
	virtual void RegisterCvars( void ) = 0;
};

};

#endif