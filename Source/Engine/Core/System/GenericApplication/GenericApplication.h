#ifndef __SIRENGINE_GENERIC_APPLICATION_H__
#define __SIRENGINE_GENERIC_APPLICATION_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Pch.h>
#include <Engine/Core/Types.h>
#include <Engine/Core/ThreadSystem/Threads.h>

namespace SIREngine::System {

class IGenericApplication
{
public:
	IGenericApplication( void );
	virtual ~IGenericApplication();

	virtual void Init( void ) = 0;
	virtual void Run( void ) = 0;
private:
};

extern IGenericApplication *g_pApplication;
extern CThreadAtomic<bool> g_bExitApp;

};

#endif