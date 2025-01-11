#ifndef __SIRENGINE_THREADS_H__
#define __SIRENGINE_THREADS_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include "../Pch.h"
#include "../Compiler.h"
#include "../Types.h"
#include <EASTL/atomic.h>

namespace SIREngine {

template<typename T>
using CThreadAtomic = eastl::atomic<T>;

class CThreadMutex
{
public:
private:
};

class CThread
{
public:
	CThread( void );
	~CThread();
	
	void Run( int (*hFunction)( void * ), const CString& name, void *pData = NULL );
	void Join( uint64_t nTimeout = INFINITY );
private:
	SDL_Thread *m_pThread;
};

SIRENGINE_FORCEINLINE CThread::CThread( void )
	: m_pThread( NULL )
{
}

SIRENGINE_FORCEINLINE CThread::~CThread()
{
}

};

#endif