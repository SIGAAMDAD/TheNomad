#ifndef __SIRENGINE_CONTROLLER_EVENT_H__
#define __SIRENGINE_CONTROLLER_EVENT_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Compiler.h>
#include "EventBase.h"
#include "KeyCodes.h"
#include <SDL2/SDL_gamecontroller.h>

namespace SIREngine::Events {

class CGamepadEvent : public IEventBase
{
public:
	CGamepadEvent( void )
	{ }
	virtual ~CGamepadEvent() override
	{ }

	virtual void Send( void ) override
	{ }

	SIRENGINE_FORCEINLINE virtual const char *GetName( void ) const override
	{ return "GamepadEvent"; }
	SIRENGINE_FORCEINLINE virtual EventType_t GetType( void ) const override
	{ return EventType_Gamepad; }
private:
	KeyNum_t m_nButton;
};

};

#endif