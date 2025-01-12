#include "Logger.h"
#include <Engine/Core/Util.h>
#include <boost/lockfree/queue.hpp>
#include <EASTL/chrono.h>
#include <boost/thread.hpp>
#include <Engine/Core/ThreadSystem/Threads.h>
#include <Engine/Core/System/GenericApplication/GenericApplication.h>
#include <Engine/Core/FileSystem/FileWriter.h>
#include <Engine/Core/ConsoleManager.h>

#define TTY_COLOR_BLACK "30"
#define TTY_COLOR_RED "31"
#define TTY_COLOR_GREEN "32"
#define TTY_COLOR_YELLOW "33"
#define TTY_COLOR_BLUE "34"
#define TTY_COLOR_MAGENTA "35"
#define TTY_COLOR_CYAN "36"
#define TTY_COLOR_WHITE "0"

namespace SIREngine {

SIREngine::CLogManager CLogManager::g_Logger;

struct CMessage {
	CMessage( void )
	{ }
	CMessage( const char *pszMessage )
	{
		SIREngine_strncpyz( szMessage, pszMessage, sizeof( szMessage ) );
		nStringLength = ::strlen( szMessage );
	}

	char szMessage[2048];
	uint64_t nStringLength;
};

static CThread s_LogThread;
static boost::lockfree::queue<CMessage, boost::lockfree::capacity<1024>> LogMessageQueue;
//static eastl::fixed_vector<CMessage, 256> LogMessageQueue;
static CThreadMutex s_LoggerLock;

// if true, each log will include the call source
bool CLogManager::bLogIncludeFileInfo = false;

// if true, each log will include the time
bool CLogManager::bLogIncludeTimeInfo = true;

static CFileWriter s_LogFile;

static CVar<uint32_t> e_LogLevel(
	"e.LogLevel",
	ELogLevel::Verbose,
	Cvar_Save,
	"Sets the global log level of the engine.",
	CVG_SYSTEMINFO
);

static CVar<bool> e_LogToFile(
	"e.LogToFile",
	true,
	Cvar_Save,
	"Outputs log data to a file.",
	CVG_SYSTEMINFO
);

static CVarRef<bool> e_LogIncludeFileInfo(
	"e.LogIncludeFileInfo",
	CLogManager::bLogIncludeFileInfo,
	Cvar_Default,
	"If true, each log will include the call source",
	CVG_SYSTEMINFO
);

static CVarRef<bool> e_LogIncludeTimeInfo(
	"e.LogIncludeTimeInfo",
	CLogManager::bLogIncludeTimeInfo,
	Cvar_Default,
	"If true, each log will include the time",
	CVG_SYSTEMINFO
);

CLogCategory::CLogCategory( const CString& categoryName, ELogLevel::Type nDefaultVerbosity )
	: m_nVerbosityLevel( nDefaultVerbosity ),
	m_Name( categoryName )
{
}

CLogCategory::~CLogCategory()
{
}

void CLogCategory::SetVerbosity( ELogLevel::Type nVerbosity )
{
}

CLogManager::CLogManager( void )
{
}

CLogManager::~CLogManager()
{
}

void CLogManager::LaunchLoggingThread( void )
{
	e_LogLevel.Register();
	e_LogToFile.Register();
	e_LogIncludeFileInfo.Register();
	e_LogIncludeTimeInfo.Register();

	s_LogThread.Run( CLogManager::LogThread, "LoggingThread" );

	if ( e_LogToFile.GetValue() ) {
		s_LogFile.Open( "Config/debug.log" );
	}
}

void CLogManager::ShutdownLogger( void )
{
	s_LogFile.Close();
	s_LogThread.Join();
}

static const char *GetTime( void )
{
	static char szBuffer[128];
	time_t current;
	struct tm ts;

	current = time( NULL );
	localtime_r( &current, &ts );

	strftime( szBuffer, sizeof( szBuffer ) - 1, "%Y.%m.%d-%H.%M.%S", &ts );
	SIREngine_snprintf_append( szBuffer, sizeof( szBuffer ) - 1, ":%3li", current );

	return szBuffer;
}

void SIRENGINE_ATTRIBUTE(format(printf, 3, 4)) CLogManager::LogInfo( const LogData_t& data,
	const char *fmt, ... )
{
	if ( e_LogLevel.GetValue() < ELogLevel::Info ) {
		return;
	}

	va_list argptr;
	char msg[8192];
	char buf[20000];
	int len;

	va_start( argptr, fmt );
	SIREngine_Vsnprintf( msg, sizeof( msg ) - 1, fmt, argptr );
	va_end( argptr );

	len = SIREngine_snprintf( buf, sizeof( buf ) - 1, "%s %s\n", GetExtraString( data.pFileName, data.pFunction, data.nLineNumber ),
		msg );
	
	LogMessageQueue.push( CMessage( buf ) );
}

void SIRENGINE_ATTRIBUTE(format(printf, 3, 4)) CLogManager::LogWarning( const LogData_t& data,
	const char *fmt, ... )
{
	if ( e_LogLevel.GetValue() < ELogLevel::Warning ) {
		return;
	}

	va_list argptr;
	char msg[8192];
	char buf[20000];
	int len;

	va_start( argptr, fmt );
	SIREngine_Vsnprintf( msg, sizeof( msg ) - 1, fmt, argptr );
	va_end( argptr );

	len = SIREngine_snprintf( buf, sizeof( buf ) - 1,
		"\x1B[" TTY_COLOR_RED "m WARNING \x1B[" TTY_COLOR_YELLOW "m"
		" %s %s \x1B[0m\n", GetExtraString( data.pFileName, data.pFunction, data.nLineNumber ), msg );
	
	LogMessageQueue.push( CMessage( buf ) );
}

void SIRENGINE_ATTRIBUTE(format(printf, 3, 4)) CLogManager::SendNotification( const LogData_t& data,
	const char *fmt, ... )
{
	if ( e_LogLevel.GetValue() < ELogLevel::Spam ) {
		return;
	}

	va_list argptr;
	char msg[8192];
	char buf[20000];
	int len;

	va_start( argptr, fmt );
	SIREngine_Vsnprintf( msg, sizeof( msg ) - 1, fmt, argptr );
	va_end( argptr );

	len = SIREngine_snprintf( buf, sizeof( buf ) - 1, "[NOTIFICATION] %s %s\n",
		GetExtraString( data.pFileName, data.pFunction, data.nLineNumber ),
		msg );
	
	LogMessageQueue.push( CMessage( buf ) );
}

void SIRENGINE_ATTRIBUTE(format(printf, 3, 4)) CLogManager::LogCategory( const LogData_t& data, const char *fmt, ... )
{
	if ( data.pCategory->GetVerbosity() < data.nLevel ) {
		return;
	}

	va_list argptr;
	char msg[8192];
	char buf[20000];
	int len;

	va_start( argptr, fmt );
	SIREngine_Vsnprintf( msg, sizeof( msg ) - 1, fmt, argptr );
	va_end( argptr );

	switch ( ( data.pCategory->GetVerbosity() & ELogLevel::VerbosityMask ) ) {
	case ELogLevel::Error:
		len = SIREngine_snprintf( buf, sizeof( buf ) - 1,
			"\x1B[" TTY_COLOR_RED "mERROR \x1B[" TTY_COLOR_YELLOW "m"
			" %s%s: %s \x1B[0m\n"
			, GetExtraString( data.pFileName, data.pFunction, data.nLineNumber ), data.pCategory->GetCategoryName().c_str(), msg
		);
		break;
	case ELogLevel::Warning:
		len = SIREngine_snprintf( buf, sizeof( buf ) - 1,
			"\x1B[" TTY_COLOR_RED "mWARNING \x1B[" TTY_COLOR_YELLOW "m"
			" %s%s: %s \x1B[0m\n"
			, GetExtraString( data.pFileName, data.pFunction, data.nLineNumber ), data.pCategory->GetCategoryName().c_str(), msg
		);
		break;
	case ELogLevel::Info:
	case ELogLevel::Verbose:
	case ELogLevel::Spam:
		len = SIREngine_snprintf( buf, sizeof( buf ) - 1,
			"%s%s: %s\n"
			, GetExtraString( data.pFileName, data.pFunction, data.nLineNumber ),
			data.pCategory->GetCategoryName().c_str(), msg
		);
		break;
	case ELogLevel::Developer:
		len = SIREngine_snprintf( buf, sizeof( buf ) - 1,
			"\x1B[" TTY_COLOR_CYAN "mDEVELOPER \x1B[" TTY_COLOR_MAGENTA "m"
			" %s%s: %s \x1B[0m\n"
			, GetExtraString( data.pFileName, data.pFunction, data.nLineNumber ), data.pCategory->GetCategoryName().c_str(), msg
		);
		break;
	case ELogLevel::Fatal:
		len = SIREngine_snprintf( buf, sizeof( buf ) - 1,
			"\x1B[" TTY_COLOR_RED "mFATAL ERROR \x1B[" TTY_COLOR_RED "m"
			" %s%s: %s \x1B[0m\n"
			, GetExtraString( data.pFileName, data.pFunction, data.nLineNumber ), data.pCategory->GetCategoryName().c_str(), msg
		);
//		Application::Get()->Shutdown();
		break;
	};
	
	LogMessageQueue.push( CMessage( buf ) );

}

void SIRENGINE_ATTRIBUTE(format(printf, 3, 4)) CLogManager::LogError( const LogData_t& data, const char *fmt, ... )
{
	if ( e_LogLevel.GetValue() < ELogLevel::Error ) {
		return;
	}

	va_list argptr;
	char msg[8192];

	va_start( argptr, fmt );
	SIREngine_Vsnprintf( msg, sizeof( msg ) - 1, fmt, argptr );
	va_end( argptr );

//	Application::g_bExitApp.store( true );

//	g_pApplication->Error( msg );
}

const char *CLogManager::GetExtraString( const char *pFileName, const char *pFunction, uint64_t nLineNumber )
{
	static char szExtraString[1024];
	
	szExtraString[0] = '\0';
	if ( bLogIncludeFileInfo ) {
		SIREngine_snprintf_append( szExtraString, sizeof( szExtraString ) - 1, "(%s:%lu)", pFileName, nLineNumber );
	}
	if ( bLogIncludeTimeInfo ) {
		SIREngine_snprintf_append( szExtraString, sizeof( szExtraString ) - 1, "[%s]", GetTime() );
	}

	return szExtraString;
}

int CLogManager::LogThread( void *unused )
{
	CThreadMutex lock;
	CMessage queueMessage;

	while ( 1 ) {
		while ( LogMessageQueue.pop( queueMessage ) ) {
			System::FileWrite( queueMessage.szMessage, queueMessage.nStringLength, SIRENGINE_STDOUT_HANDLE );
			if ( s_LogFile.IsOpen() ) {
				s_LogFile.Write( queueMessage.szMessage, queueMessage.nStringLength );
		    }
		}
		
		if ( System::g_bExitApp.load() ) {
			break;
		} else {
			boost::this_thread::sleep_for( boost::chrono::milliseconds( 10 ) );
		}
	}
	return 0;
}

};