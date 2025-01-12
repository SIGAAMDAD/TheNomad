#ifndef __SIRENGINE_EVENT_MANAGER_H__
#define __SIRENGINE_EVENT_MANAGER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/EngineSystem.h>
#include "EventBase.h"
#include "WindowEvent.h"
#include <SDL2/SDL_keycode.h>
#include <EASTL/shared_ptr.h>
#include "EventData.h"

#include "KeyCodes.h"

namespace SIREngine::Events {

class IEventListener
{
public:
	IEventListener( const CString& name, EventType_t nType )
		: m_Name( name ), m_nCaptureType( nType )
	{ }
	virtual ~IEventListener()
	{ }

	virtual void Dispatch( const IEventBase *pEventData ) = 0;

	SIRENGINE_FORCEINLINE const CString& GetName( void ) const
	{ return m_Name; }
	SIRENGINE_FORCEINLINE EventType_t GetType( void ) const
	{ return m_nCaptureType; }
protected:
	CString m_Name;
	EventType_t m_nCaptureType;
};

class CEventListener : public IEventListener
{
public:
	template<typename Fn, typename... Args>
	CEventListener( const CString& name, EventType_t nType, Fn&& pCallback, Args&&... args )
		: IEventListener( name, nType ),
		m_Function( [&]( const IEventBase *pEventData ){ pCallback( eastl::forward<Args>( args )..., pEventData ); } )
	{ }
	template<typename Fn>
	CEventListener( const CString& name, EventType_t nType, Fn&& pCallback )
		: IEventListener( name, nType ),
		m_Function( [&]( const IEventBase *pEventData ){ pCallback( pEventData ); } )
	{ }
	virtual ~CEventListener() override
	{ }

	inline virtual void Dispatch( const IEventBase *pBase ) override
	{ m_Function( pBase ); }
private:
	eastl::function<void( const IEventBase * )> m_Function;
};

class CEventManager : public IEngineSystem
{
	using CEventListenerList = eastl::unordered_map<CString, eastl::shared_ptr<IEventListener>>;
public:
	CEventManager( void );
	virtual ~CEventManager() override;

	virtual void Init( void ) override;
	virtual void Shutdown( void ) override;
	virtual void Restart( void ) override;
	virtual void Frame( uint32_t nFrameTic ) override;
	virtual void RegisterCvars( void ) override;

	void AddEventListener( const eastl::shared_ptr<IEventListener>& listener );
	void PushEvent( uint64_t nTime, const CEventData& pData );

	static CEventData CreateKeyEvent( bool bState, KeyNum_t nKeyID );
	static CEventData CreateWindowEvent( WindowEventType_t nEventType, int32_t nValue1, int32_t nValue2 );
	static CEventData CreateGamepadEvent( void );
	static CEventData CreateJoystickEvent( void );
	static CEventData CreateTouchEvent( float x, float y, uint32_t nFingerState );
	static CEventData CreateMouseEvent( int x, int y );
	static CEventData CreateActionEvent( void );
	static CEventData CreateControllerStatusEvent( bool bStatus, int32_t nDeviceID );
	static CEventData CreateEmptyEvent( void );
private:
	void PumpEvents( void );
	const CEventData& GetEvent( void );

	eastl::array<CEventListenerList, NumEventTypes> m_EventBindings;

	eastl::vector<CEventData> m_EventBuffer;
	int64_t m_nBufferedEventsHead;
	int64_t m_nBufferedEventsTail;
};

extern CEventManager *g_pEventManager;
extern CQuitEvent g_QuitEvent;

};

#endif