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

typedef enum {
	DR_YES = 0,
	DR_NO = 1,
	DR_OK = 0,
	DR_CANCEL = 1
} dialogResult_t;

typedef enum {
	DT_INFO,
	DT_WARNING,
	DT_ERROR,
	DT_YES_NO,
	DT_OK_CANCEL
} dialogType_t;

};

#endif