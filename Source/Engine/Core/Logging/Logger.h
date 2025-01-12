#ifndef __SIRENGINE_LOGGER_H__
#define __SIRENGINE_LOGGER_H__

#if defined(SIRENGINE_PRAGMA_ONCE_SUPPORTED)
	#pragma once
#endif

#include <Engine/Core/Types.h>
#include <Engine/Core/Compiler.h>
#include <stdint.h>
#include <EASTL/queue.h>

namespace ELogLevel {
	enum Type : uint32_t {
		NoLogging = 0,

		Fatal			= 1,
		Error			= 2,
		Warning			= 3,
		Developer		= 4,
		Info			= 5,
		Verbose			= 6,
		Spam			= 7,

		All = Spam,
		NumLevels,
		VerbosityMask   = 0xf,
		BreakOnLog		= 0x40
	};
};

static_assert( ELogLevel::NumLevels - 1 < ELogLevel::VerbosityMask, "Bad verbosity mask." );

namespace SIREngine {

	class CLogCategory
	{
	public:
		CLogCategory( const CString& categoryName, ELogLevel::Type nDefaultVerbosity );
		~CLogCategory();

		SIRENGINE_FORCEINLINE SIRENGINE_CONSTEXPR bool IsSuppressed( ELogLevel::Type nLevel ) const
		{ return !( ( m_nVerbosityLevel & ELogLevel::VerbosityMask ) <= nLevel ); }

		SIRENGINE_FORCEINLINE SIRENGINE_CONSTEXPR const CString& GetCategoryName( void ) const
		{ return m_Name; }
		SIRENGINE_FORCEINLINE SIRENGINE_CONSTEXPR ELogLevel::Type GetVerbosity( void ) const
		{ return m_nVerbosityLevel; }

		void SetVerbosity( ELogLevel::Type nVerbosity );
	private:
		ELogLevel::Type m_nVerbosityLevel;
		CString m_Name;
	};

	typedef struct LogData {
		LogData( const char *_pFileName, const char *_pFunction, uint64_t _nLineNumber )
			: pFileName( _pFileName ), pFunction( _pFunction ), nLineNumber( _nLineNumber )
		{ }
		LogData( const char *_pFileName, const char *_pFunction, uint64_t _nLineNumber, CLogCategory *_pCategory, ELogLevel::Type _nType )
			: pFileName( _pFileName ), pFunction( _pFunction ), nLineNumber( _nLineNumber ), pCategory( _pCategory ), nLevel( _nType )
		{ }

		const char *pFileName;
		const char *pFunction;
		uint64_t nLineNumber;
		CLogCategory *pCategory;
		ELogLevel::Type nLevel;
	} LogData_t;

	class CLogManager
	{
	public:
		CLogManager( void );
		~CLogManager();

		void LogInfo( const LogData_t& data, const char *fmt, ... ) SIRENGINE_ATTRIBUTE(format(printf, 3, 4));
		void LogWarning( const LogData_t& data, const char *fmt, ... ) SIRENGINE_ATTRIBUTE(format(printf, 3, 4));
		void LogError( const LogData_t& data, const char *fmt, ... ) SIRENGINE_ATTRIBUTE(format(printf, 3, 4));
		void SendNotification( const LogData_t& data, const char *fmt, ... ) SIRENGINE_ATTRIBUTE(format(printf, 3, 4));
		void LogCategory( const LogData_t& data, const char *fmt, ... ) SIRENGINE_ATTRIBUTE(format(printf, 3, 4));

		static void LaunchLoggingThread( void );
		static void ShutdownLogger( void );

		static CLogManager g_Logger;

		static bool bLogIncludeFileInfo;
		static bool bLogIncludeTimeInfo;
	private:
		static int LogThread( void *unused );

		static const char *GetExtraString( const char *pFileName, const char *pFunction, uint64_t nLineNumber );
	};
};

#include "LoggerMacros.h"

#endif