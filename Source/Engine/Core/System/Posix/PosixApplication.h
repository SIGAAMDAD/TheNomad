#ifndef __SIRENGINE_POSIX_APPLICATION_H__
#define __SIRENGINE_POSIX_APPLICATION_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/System/GenericApplication/GenericApplication.h>

namespace SIREngine::System {

class PosixApplication : public IGenericApplication
{
public:
	PosixApplication( void );
	virtual ~PosixApplication() override;

	virtual void Init( void ) override;
	virtual void Run( void ) override;
private:
};

};

#endif