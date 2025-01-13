#ifndef __SIRENGINE_BINDING_MANAGER_H__
#define __SIRENGINE_BINDING_MANAGER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Events/KeyCodes.h>
#include <Engine/Core/Logging/Logger.h>
#include <nlohmann/json.hpp>
#include <EASTL/unordered_map.h>
#include <EASTL/unique_ptr.h>

#define MAX_BIND_NAME 128

namespace SIREngine::Input {
	typedef struct Binding_s {
		Binding_s( const CString& name, Events::KeyNum_t nKeyNum )
			: Name( name ), KeyID( nKeyNum )
		{ }

		CString Name;
		Events::KeyNum_t KeyID;
	} Binding_t;

	class CBindSet
	{
	public:
		CBindSet( void )
		{ }
		~CBindSet()
		{ }

		bool Load( const nlohmann::basic_json<std::map, std::vector, CString>& data );

		SIRENGINE_FORCEINLINE const eastl::unordered_map<Events::KeyNum_t, Binding_t>& GetBinds( void ) const
		{ return m_Bindings; }

		SIRENGINE_FORCEINLINE const Binding_t *GetBind( Events::KeyNum_t nKeyID ) const
		{
			auto it = m_Bindings.find( nKeyID );
			if ( it != m_Bindings.end() ) {
				return &it->second;
			}
			return NULL;
		}
		SIRENGINE_FORCEINLINE Binding_t *GetBind( Events::KeyNum_t nKeyID )
		{
			auto it = m_Bindings.find( nKeyID );
			if ( it != m_Bindings.end() ) {
				return &it->second;
			}
			return NULL;
		}
	private:
		eastl::unordered_map<Events::KeyNum_t, Binding_t> m_Bindings;
	};

	class CBindingManager
	{
	public:
		CBindingManager( void );
		~CBindingManager();

		void LoadBindings( void );

		SIRENGINE_FORCEINLINE const eastl::unique_ptr<CBindSet>& GetSet( const CString& name ) const
		{
			auto it = m_BindMap.find( name );
			if ( it != m_BindMap.end() ) {
				return it->second;
			}
			SIRENGINE_WARNING( "No bindset for mapping \"%s\"!", name.c_str() );
			return NULL;
		}
	private:
		eastl::unordered_map<CString, eastl::unique_ptr<CBindSet>> m_BindMap;
	};
};

#endif