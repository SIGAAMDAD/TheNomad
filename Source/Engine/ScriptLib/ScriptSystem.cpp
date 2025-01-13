#include <Engine/Core/Logging/Logger.h>
#include "ScriptSystem.h"
#include <Engine/Core/ConsoleManager.h>
#include <Engine/Core/FileSystem/FileList.h>

namespace SIREngine::ScriptLib {

SIRENGINE_DEFINE_LOG_CATEGORY( ScriptSystem, ELogLevel::Info );

static bool bAlwaysCompileModules = false;

CVarRef<bool> ml_AlwaysCompileModules(
	"ml.AlwaysCompileModules",
	bAlwaysCompileModules,
	Cvar_Save,
	"Forces recompilation of modules each time the engine launches.",
	CVG_RENDERER
);

void CScriptSystem::LoadModList( void )
{
	SIRENGINE_LOG_LEVEL( ScriptSystem, ELogLevel::Info, "Fetching module loadlist..." );
}

void CScriptSystem::Init( void )
{
	SIRENGINE_LOG_LEVEL( ScriptSystem, ELogLevel::Info, "Initializing AngelScript engine..." );

	m_pEngine = asCreateScriptEngine( ANGELSCRIPT_VERSION );
	if ( !m_pEngine ) {

	}
}

void CScriptSystem::Shutdown( void )
{
	SIRENGINE_LOG_LEVEL( ScriptSystem, ELogLevel::Info, "Releasing AngelScript objects..." );
	if ( m_pEngine ) {
		m_pEngine->Release();
	}
}

void CScriptSystem::Restart( void )
{
	Shutdown();
	Init();
}

void CScriptSystem::Frame( uint32_t nFrameTic )
{
}

void CScriptSystem::RegisterCvars( void )
{
	ml_AlwaysCompileModules.Register();
}

};