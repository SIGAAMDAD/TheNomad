#ifndef __SIRENGINE_SCRIPT_SYSTEM_H__
#define __SIRENGINE_SCRIPT_SYSTEM_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Pch.h>
#include <Engine/Core/Types.h>
#include <Engine/Core/EngineSystem.h>
#include <Engine/Core/ThreadSystem/Threads.h>
#include <angelscript.h>
#include "ScriptModule.h"

namespace SIREngine::ScriptLib {

class CScriptSystem : public IEngineSystem
{
public:
	CScriptSystem( void );
	virtual ~CScriptSystem() override;

	virtual void Init( void ) override;
	virtual void Shutdown( void ) override;
	virtual void Restart( void ) override;
	virtual void Frame( uint32_t nFrameTic ) override;
	virtual void RegisterCvars( void ) override;
private:
	void LoadModList( void );

	eastl::vector<eastl::shared_ptr<CScriptModule>> m_ModuleList;
	CThread m_GameThread;

	asIScriptEngine *m_pEngine;
};

extern CScriptSystem *g_pScriptSystem;

};

#endif