#ifndef __SIRENGINE_INPUT_MANAGER_H__
#define __SIRENGINE_INPUT_MANAGER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/EngineSystem.h>
#include <Engine/Core/Pch.h>
#include <Engine/Core/Config.h>
#include "BindingManager.h"

namespace SIREngine::Input {
	class CInputDevice
	{
	public:
		enum class EType : uint32_t {
			Keyboard = 0,
			Controller,
			TouchScreen
		};
	public:
		CInputDevice( void )
			: m_nType( EType::Keyboard ), m_nDeviceIndex( 0 ), m_pController( NULL )
		{ }
		~CInputDevice()
		{ Shutdown(); }

		void Init( EType nInitialType, int32_t nIndex );
		void Shutdown( void );
		void ApplyHaptic( int32_t nDuration, float nIntensity );

		SIRENGINE_FORCEINLINE bool IsGamepad( void ) const
		{ return m_pController->m_pGamepad != NULL; }

		SIRENGINE_FORCEINLINE EType GetType( void ) const
		{ return m_nType; }
		SIRENGINE_FORCEINLINE int32_t GetIndex( void ) const
		{ return m_nDeviceIndex; }

		SIRENGINE_FORCEINLINE bool ValidController( void ) const
		{ return m_pController != NULL; }
		SIRENGINE_FORCEINLINE SDL_JoystickID GetInstanceID( void ) const
		{ return m_pController != NULL ? m_pController->m_hJoystickID : -1; }
		SIRENGINE_FORCEINLINE const eastl::unique_ptr<CBindSet>& GetBindings( void ) const
		{ return m_pBindSet; }
		SIRENGINE_FORCEINLINE eastl::unique_ptr<CBindSet>& GetBindings( void )
		{ return m_pBindSet; }
	private:
		void InitKeyboard( void );
		void InitController( void );
		void InitTouchDevice( void );

		typedef struct {
			SDL_Joystick *m_pJoystick;
			SDL_GameController *m_pGamepad;
			SDL_Haptic *m_pHaptic;
			SDL_JoystickID m_hJoystickID;
		} JoystickData_t;

		EType m_nType;
		int32_t m_nDeviceIndex;
		JoystickData_t *m_pController;

		SDL_TouchID m_hTouchDevice;
	    SDL_FingerID m_hTouchFinger;

		eastl::unique_ptr<CBindSet> m_pBindSet;
	};

	class CInputManager : public IEngineSystem
	{
	public:
		CInputManager( void );
		virtual ~CInputManager() override;

		virtual void Init( void ) override;
		virtual void Shutdown( void ) override;
		virtual void Frame( uint32_t nFrameTic ) override;
		virtual void Restart( void ) override;
		virtual void RegisterCvars( void ) override;
	private:
		static void KeyboardEventListener( const Events::IEventBase *pEventData );
		static void GamepadEventListener( const Events::IEventBase *pEventData );
		static void ControllerStatusListener( const Events::IEventBase *pEventData );

		CInputDevice *GetKeyboard( void );

		void OnKeyDown( Events::KeyNum_t nKeyID );
		void OnKeyUp( Events::KeyNum_t nKeyID );

		void OnAddController( int32_t nDeviceID );
		void OnRemoveController( int32_t nDeviceID );

		eastl::array<CInputDevice, SIRENGINE_MAX_COOP_PLAYERS> m_szInputDevices;
	};

	extern CInputManager *g_pInputManager;
};

#endif