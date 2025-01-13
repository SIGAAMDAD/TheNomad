#include <Engine/Core/Core.h>
#include "Util.h"
#include "ConsoleManager.h"
#include <nlohmann/json.hpp>
#include <locale.h>
#include <Engine/Core/FileSystem/CachedFile.h>
#include <Engine/Core/FileSystem/FileWriter.h>

namespace SIREngine {

SIRENGINE_DEFINE_LOG_CATEGORY( ConsoleSystem, ELogLevel::Info );

CConsoleManager *g_pConsoleManager;

static const char *GetConfigSectionName( CvarGroup_t nGroup )
{
	switch ( nGroup ) {
	case CVG_FILESYSTEM:
		return "FileSystemConfig";
	case CVG_RENDERER:
		return "RendererConfig";
	case CVG_SYSTEMINFO:
		return "EngineConfig";
	case CVG_USERINFO:
		return "RuntimeConfig";
	case CVG_SOUNDSYSTEM:
		return "SoundSystemConfig";
	default:
		break;
	};
	return "MiscConfig";
}

CConsoleManager::CConsoleManager( void )
{
	m_pSavedLocale = setlocale( LC_NUMERIC, NULL );
	setlocale( LC_NUMERIC, "C" );
}

CConsoleManager::~CConsoleManager()
{
	for ( auto& it : m_ObjectList ) {
		if ( it.second->AsVariable() ) {
			if ( it.second->AsVariable()->GetGroup() == CVG_USERINFO || it.second->AsVariable()->GetGroup() == CVG_NONE ) {
				delete it.second; // runtime allocated cvar
			}
		} else {
			delete it.second; // command
		}
	}
	setlocale( LC_NUMERIC, m_pSavedLocale );
}

IConsoleVar *CConsoleManager::RegisterCVar( const char *pName, bool bValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVar<bool>( pName, bValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%s\"", pName, SIREngine_BoolToString( bValue ) );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVar( const char *pName, uint32_t nValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVar<uint32_t>( pName, nValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%u\"", pName, nValue );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVar( const char *pName, uint64_t nValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVar<uint64_t>( pName, nValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%lu\"", pName, nValue );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVar( const char *pName, int32_t nValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVar<int32_t>( pName, nValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%i\"", pName, nValue );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVar( const char *pName, int64_t nValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVar<int64_t>( pName, nValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%li\"", pName, nValue );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVar( const char *pName, float fValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVar<float>( pName, fValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%0.02f\"", pName, fValue );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVar( const char *pName, const CString& value, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVar<CString>( pName, value, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%s\"", pName, value.c_str() );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

void CConsoleManager::RegisterCVarDefaultValue( IConsoleVar *pCvar )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Info, "Registering CVar \"%s\"...", pCvar->GetName().c_str() );
	
	auto it = m_ObjectList.find( pCvar->GetName() );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pCvar->GetName() );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
	}
	
	m_ObjectList.try_emplace( pCvar->GetName(), pCvar );
}

IConsoleVar *CConsoleManager::RegisterCVarRef( const char *pName, bool& bValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVarRef<bool>( pName, bValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%s\"", pName, SIREngine_BoolToString( bValue ) );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVarRef( const char *pName, uint32_t& nValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVarRef<uint32_t>( pName, nValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%u\"", pName, nValue );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVarRef( const char *pName, uint64_t& nValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVarRef<uint64_t>( pName, nValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%lu\"", pName, nValue );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVarRef( const char *pName, int32_t& nValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );
	
	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVarRef<int32_t>( pName, nValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%i\"", pName, nValue );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVarRef( const char *pName, int64_t& nValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVarRef<int64_t>( pName, nValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%li\"", pName, nValue );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVarRef( const char *pName, float& fValue, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVarRef<float>( pName, fValue, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%0.02f\"", pName, fValue );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}

IConsoleVar *CConsoleManager::RegisterCVarRef( const char *pName, CString& value, uint32_t nFlags, const char *pDescription, CvarGroup_t nGroup )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pName );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pName );
		if ( !it->second->AsVariable() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterCVarRef: called on IConsoleObject that isn't a valid cvar" );
		}
		return it->second->AsVariable();
	}

	IConsoleVar *pCvar = new CVarRef<CString>( pName, value, nFlags, pDescription, nGroup );
	SIRENGINE_LOG( "Registered CVarRef object \"%s\" with defaultValue \"%s\"", pName, value.c_str() );

	m_ObjectList.try_emplace( pName, pCvar );

	return pCvar;
}


void CConsoleManager::RegisterConsoleCommand( IConsoleCmd *pCommand )
{
	CThreadAutoLock<CThreadMutex> _( m_hObjectLock );

	auto it = m_ObjectList.find( pCommand->GetName() );
	if ( it != m_ObjectList.end() ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Warning, "IConsoleObject \"%s\" already registered", pCommand->GetName().c_str() );
		if ( !it->second->AsCommand() ) {
			SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "CConsoleManager::RegisterConsoleCommand: called on IConsoleObject that isn't a valid command" );
		}
	}

	m_ObjectList.try_emplace( pCommand->GetName(), pCommand );
}

IConsoleCmd *CConsoleManager::GetConsoleCommand( const CString& commandName )
{

}
/*
void CConsoleManager::RegisterCVar( IConsoleVar *pCvar )
{
	if ( m_ObjectList.find( pCvar->GetName() ) == m_ObjectList.end() ) {
		m_ObjectList.try_emplace( pCvar->GetName(), pCvar );

		if ( pCvar->GetFlags() & Cvar_Save ) {
			if ( m_pConfigLoader->HasValue( GetConfigSectionName( pCvar->GetGroup() ), pCvar->GetName() ) ) {
				switch ( pCvar->GetType() ) {
				case CvarType_Bool:
					pCvar->SetValue( m_pConfigLoader->GetBool( GetConfigSectionName( pCvar->GetGroup() ), pCvar->GetName() ) );
					break;
				case CvarType_Int:
					pCvar->SetValue( m_pConfigLoader->GetInt( GetConfigSectionName( pCvar->GetGroup() ), pCvar->GetName() ) );
					break;
				case CvarType_UInt:
					pCvar->SetValue( m_pConfigLoader->GetUInt( GetConfigSectionName( pCvar->GetGroup() ), pCvar->GetName() ) );
					break;
				case CvarType_Float:
					pCvar->SetValue( m_pConfigLoader->GetFloat( GetConfigSectionName( pCvar->GetGroup() ), pCvar->GetName() ) );
					break;
				case CvarType_FilePath:
					pCvar->SetValue( m_pConfigLoader->GetString( GetConfigSectionName( pCvar->GetGroup() ), pCvar->GetName() ).c_str() );
					break;
				case CvarType_String:
					pCvar->SetValue( m_pConfigLoader->GetString( GetConfigSectionName( pCvar->GetGroup() ), pCvar->GetName() ) );
					break;
				};
			}
		}
	}
}
*/

void CConsoleManager::LoadConfig( const CFilePath& filePath )
{
	const char *pSectionName;

	SIRENGINE_LOG( "Loading Engine Configuration..." );
	
	CCachedFile file( filePath );

	if ( !file.GetSize() || !file.GetBuffer() ) {
		return;
	}

	nlohmann::basic_json<std::map, std::vector, CString> configLoader;
	try {
		configLoader = nlohmann::basic_json<std::map, std::vector, CString>::parse( (const char *)file.GetBuffer(),
			(const char *)file.GetBuffer() + file.GetSize() );
	} catch ( const nlohmann::json::exception& e ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "Error parsing engine configuration: [%i] %s", e.id, e.what() );
	}

	for ( auto& it : m_ObjectList ) {
		if ( !it.second->AsVariable() ) {
			continue; // its a command
		}

		IConsoleVar *pCvar = it.second->AsVariable();
		if ( !( pCvar->GetFlags() & Cvar_Save ) ) {
			continue;
		}

		pSectionName = GetConfigSectionName( pCvar->GetGroup() );

		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Info, "Loading cvar value \"%s\" from section \"%s\"...", pCvar->GetName().c_str(),
			pSectionName  );
		
		if ( !configLoader.contains( pSectionName ) ) {
			continue;
		}
		if ( !configLoader[ pSectionName ].contains( pCvar->GetName() ) ) {
			continue;
		}

		switch ( pCvar->GetType() ) {
		case CvarType_Bool:
			pCvar->SetValue( (bool)configLoader[ pSectionName ][ pCvar->GetName() ] );
			break;
		case CvarType_Int:
			pCvar->SetValue( (int32_t)configLoader[ pSectionName ][ pCvar->GetName() ] );
			break;
		case CvarType_UInt:
			pCvar->SetValue( (uint32_t)configLoader[ pSectionName ][ pCvar->GetName() ] );
			break;
		case CvarType_Float:
			pCvar->SetValue( (float)configLoader[ pSectionName ][ pCvar->GetName() ] );
			break;
		case CvarType_FilePath:
			pCvar->SetValue( CFilePath( configLoader[ pSectionName ][ pCvar->GetName() ].get<CString>().c_str() ) );
			break;
		case CvarType_String:
			pCvar->SetValue( configLoader[ pSectionName][ pCvar->GetName() ].get<CString>() );
			break;
		};
	}

	SIRENGINE_LOG( "Done." );
}

void CConsoleManager::SaveConfig( const CFilePath& filePath ) const
{
	SIRENGINE_LOG( "Saving Engine Configuration..." );

	nlohmann::basic_json<std::map, std::vector, CString> data;

	for ( const auto& var : m_ObjectList ) {
		if ( !var.second->AsVariable() ) {
			continue; // its a command
		}

		IConsoleVar *pCvar = var.second->AsVariable();
		if ( !( pCvar->GetFlags() & Cvar_Save ) ) {
			continue;
		}
		SIRENGINE_LOG( "\"%s\" = \"%s\"", pCvar->GetName().c_str(), pCvar->GetStringValue().c_str() );

		switch ( pCvar->GetType() ) {
		case CvarType_Bool: {
			const CVar<bool> *pValue = dynamic_cast<CVar<bool> *>( pCvar );
			if ( pValue ) {
				data[ GetConfigSectionName( pCvar->GetGroup() ) ][ pCvar->GetName() ] = pValue->GetValue();
			} else {
				data[ GetConfigSectionName( pCvar->GetGroup() ) ][ pCvar->GetName() ] = dynamic_cast<CVarRef<bool> *>( pCvar )->GetValue();
			}
			break; }
		case CvarType_Int: {
			const CVar<int32_t> *pValue = dynamic_cast<CVar<int32_t> *>( pCvar );
			if ( pValue ) {
				data[ GetConfigSectionName( pCvar->GetGroup() ) ][ pCvar->GetName() ] = pValue->GetValue();
			} else {
				data[ GetConfigSectionName( pCvar->GetGroup() ) ][ pCvar->GetName() ] = dynamic_cast<CVarRef<int32_t> *>( pCvar )->GetValue();
			}
			break; }
		case CvarType_UInt: {
			const CVar<uint32_t> *pValue = dynamic_cast<CVar<uint32_t> *>( pCvar );
			if ( pValue ) {
				data[ GetConfigSectionName( pCvar->GetGroup() ) ][ pCvar->GetName() ] = pValue->GetValue();
			} else {
				data[ GetConfigSectionName( pCvar->GetGroup() ) ][ pCvar->GetName() ] = dynamic_cast<CVarRef<uint32_t> *>( pCvar )->GetValue();
			}
			break; }
		case CvarType_Float: {
			const CVar<float> *pValue = dynamic_cast<CVar<float> *>( pCvar );
			if ( pValue ) {
				data[ GetConfigSectionName( pCvar->GetGroup() ) ][ pCvar->GetName() ] = pValue->GetValue();
			} else {
				data[ GetConfigSectionName( pCvar->GetGroup() ) ][ pCvar->GetName() ] = dynamic_cast<CVarRef<float> *>( pCvar )->GetValue();
			}
			break; }
		case CvarType_FilePath:
		case CvarType_String:
			data[ GetConfigSectionName( pCvar->GetGroup() ) ][ pCvar->GetName() ] = pCvar->GetStringValue();
			break;
		};
	}

	const CString& buffer = data.dump( 1, '\t' );

	CFileWriter writer( filePath );
	if ( !writer.Write( buffer.data(), buffer.size() ) ) {
		SIRENGINE_LOG_LEVEL( ConsoleSystem, ELogLevel::Error, "Error writing engine configuration" );
	}
}

const char *CConsoleManager::SkipWhitespace( const char *pText )
{
	while ( 1 ) {
		while ( *pText && *pText <= ' ' ) {
			pText++;
		}
		if ( !*pText ) {
			return NULL;
		}
		
		// single-line comments
		if ( pText[0] == '/' && pText[1] == '/' ) {
			if ( pText < m_CommandLine.begin() + 3 ) {
				return NULL;
			}
		}

		// multiline comments
		if ( pText[0] == '/' && pText[1] == '*' ) {
			while ( *pText && ( pText[0] != '*' || pText[1] != '/' ) ) {
				pText++;
			}
			if ( !*pText ) {
				return NULL;
			}
			pText += 2;
		} else {
			break;
		}
	}
	return pText;
}

void CConsoleManager::ExecuteCommand( const char *pText )
{
	const char *pInput;
	char *pOut;
	char szCommandArgs[8192];

	CThreadAutoLock<CThreadMutex> _( m_hCommandLock );

	SIREngine_strncpyz( m_CommandLine.begin(), pText, m_CommandLine.size() );
	pInput = m_CommandLine.begin();
	pOut = szCommandArgs;

	while ( 1 ) {
		pInput = SkipWhitespace( pInput );

		if ( !pInput ) {
			return; // all tokens processed
		}

		// parse a quoted string
		if ( *pInput == '\"' ) {
			m_CommandArgs[ m_CommandArgc.load() ] = pOut;
			m_CommandArgc.fetch_add( 1 );
			pInput++;

			while ( *pInput && *pInput != '\"' ) {
				*pOut++ = *pInput++;
			}

			*pOut = '\0';

			if ( !*pInput ) {
				return;
			}

			pInput++;
			continue;
		}

		m_CommandArgs[ m_CommandArgc.load() ] = pOut;
		m_CommandArgc.fetch_add( 1 );

		// skip until whitespace
		while ( *pInput > ' ' ) {
			if ( pInput[0] == '\"' ) {
				break;
			}

			if ( pInput[0] == '/' && pInput[1] == '/' ) {
				if ( pInput < m_CommandLine.begin() + 3 ) {
					break;
				}
			}

			if ( pInput[0] == '/' && pInput[1] == '*' ) {
				break;
			}

			*pOut++ = *pInput++;
		}

		*pOut++ = '\0';

		if ( !*pInput ) {
			return; // all tokens parsed
		}
	}
}

};