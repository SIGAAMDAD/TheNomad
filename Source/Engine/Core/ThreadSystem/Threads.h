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
	CThreadMutex( void );
	~CThreadMutex();

	void Lock( void );
	bool TryLock( void );
	void Unlock( void );

	SDL_mutex *GetData( void );
private:
	void ErrorCheck( const char *pFunction, int nErrorCode );

	SDL_mutex *m_pMutex;
};

SIRENGINE_FORCEINLINE void CThreadMutex::Lock( void )
{
	ErrorCheck( "SDL_LockMutex()", SDL_LockMutex( m_pMutex ) );
}

SIRENGINE_FORCEINLINE bool CThreadMutex::TryLock( void )
{
	return SDL_TryLockMutex( m_pMutex ) == 0;
}

SIRENGINE_FORCEINLINE void CThreadMutex::Unlock( void )
{
	ErrorCheck( "SDL_UnlockMutex()", SDL_UnlockMutex( m_pMutex ) );
}

SIRENGINE_FORCEINLINE SDL_mutex *CThreadMutex::GetData( void )
{
	return m_pMutex;
}

class CThreadCondVar
{
public:
	CThreadCondVar( void );
	~CThreadCondVar();

	void Wait( CThreadMutex& mutex );
	void Wait( CThreadMutex& mutex, uint64_t nTimeout );
	void Signal( void );
	void Broadcast( void );
private:
	void ErrorCheck( const char *pFunction, int nErrorCode );

	SDL_cond *m_pCondVar;
};

SIRENGINE_FORCEINLINE void CThreadCondVar::Wait( CThreadMutex& mutex )
{
	ErrorCheck( "SDL_CondWait()", SDL_CondWait( m_pCondVar, mutex.GetData() ) );
}

SIRENGINE_FORCEINLINE void CThreadCondVar::Wait( CThreadMutex& mutex, uint64_t nTimeout )
{
	ErrorCheck( "SDL_CondWaitTimeout()", SDL_CondWaitTimeout( m_pCondVar, mutex.GetData(), nTimeout ) );
}

SIRENGINE_FORCEINLINE void CThreadCondVar::Signal( void )
{
	ErrorCheck( "SDL_CondSignal()", SDL_CondSignal( m_pCondVar ) );
}

SIRENGINE_FORCEINLINE void CThreadCondVar::Broadcast( void )
{
	ErrorCheck( "SDL_CondBroadcast()", SDL_CondBroadcast( m_pCondVar ) );
}

template<typename T>
class CThreadAutoLock
{
public:
	SIRENGINE_FORCEINLINE CThreadAutoLock( CThreadMutex& mutex )
		: m_pMutex( eastl::addressof( mutex ) )
	{ m_pMutex->Lock(); }
	SIRENGINE_FORCEINLINE ~CThreadAutoLock()
	{ m_pMutex->Unlock(); }
private:
	CThreadMutex *m_pMutex;
};

class CThread
{
public:
	CThread( void );
	~CThread();
	
	void Run( int (*hFunction)( void * ), const CString& name, void *pData = NULL );
	void Join( uint64_t nTimeout = (uint64_t)-1 );
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