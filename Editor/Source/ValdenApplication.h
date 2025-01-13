#ifndef __VALDEN_APPLICATION_H__
#define __VALDEN_APPLICATION_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/EngineSystem.h>
#include "EditorLayer.h"

using namespace SIREngine;

namespace Valden {

class CApplication : public IEngineSystem
{
public:
	CApplication( void );
	virtual ~CApplication() override;
	
	virtual void Init( void ) override;
	virtual void Shutdown( void ) override;
	virtual void Restart( void ) override;
	virtual void Frame( uint32_t nFrameTic ) override;
	virtual void RegisterCvars( void ) override;

	SIRENGINE_FORCEINLINE void PushLayer( const eastl::shared_ptr<IEditorLayer>& layer ) {
		m_LayerStack.emplace_back( layer );
		layer->Init();
	}
private:
	eastl::vector<eastl::shared_ptr<IEditorLayer>> m_LayerStack;
};

};

#endif