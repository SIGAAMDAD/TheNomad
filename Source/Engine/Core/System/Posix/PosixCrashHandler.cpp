#include <glob.h>
#include <cxxabi.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <backtrace.h>
#include <sys/prctl.h>
#include <Engine/Core/Compiler.h>
#include <Engine/Core/Platform.h>
#include <Engine/Core/Logging/Logger.h>
#include <Engine/Core/System/GenericApplication/GenericApplication.h>
#include <Engine/Core/Util.h>
#include "PosixApplication.h"

#define OOM_MEMORY_BACKUP_POOL_SIZE 4*1024

namespace SIREngine::System {

static char argv0[ 4096 ];

void *pOOMBackup;
size_t nOOMBackupSize;

static const char s_szCrashSwitch[] = "--cc-handle-crash";
static const char s_szFatalError[] = "\n\n*** FATAL ERROR ***\n";
static const char s_szPipeError[] = "!!! Failed to create pipe\n";
static const char s_szForkError[] = "!!! Failed to fork debug process\n";
static const char s_szExecError[] = "!!! Failed to exec debug process\n";

static struct {
	int signum;
	pid_t pid;
	int hasSigInfo;
	siginfo_t siginfo;
	char szBuffer[4096];
} crash_info;

static const struct {
	const char *name;
	int signum;
} signals[] = {
	{ "Segmentation fault", SIGSEGV },
	{ "Illegal instruction", SIGILL },
	{ "FPU exception", SIGFPE },
	{ "System BUS error", SIGBUS },
	{ NULL, 0 }
};

static const struct {
	int code;
	const char *name;
} sigill_codes[] = {
#if !defined(SIRENGINE_PLATFORM_FREEBSD)
	{ ILL_ILLOPC, "Illegal opcode" },
	{ ILL_ILLOPN, "Illegal operand" },
	{ ILL_ILLADR, "Illegal addressing mode" },
	{ ILL_ILLTRP, "Illegal trap" },
	{ ILL_PRVOPC, "Privileged opcode" },
	{ ILL_PRVREG, "Privileged register" },
	{ ILL_COPROC, "Coprocessor error" },
	{ ILL_BADSTK, "Internal stack error" },
#endif
	{ 0, NULL }
};

static const struct {
	int code;
	const char *name;
} sigfpe_codes[] = {
	{ FPE_INTDIV, "Integer divide by zero" },
	{ FPE_INTOVF, "Integer overflow" },
	{ FPE_FLTDIV, "Floating point divide by zero" },
	{ FPE_FLTOVF, "Floating point overflow" },
	{ FPE_FLTUND, "Floating point underflow" },
	{ FPE_FLTRES, "Floating point inexact result" },
	{ FPE_FLTINV, "Floating point invalid operation" },
	{ FPE_FLTSUB, "Subscript out of range" },
	{ 0, NULL }
};

static const struct {
	int code;
	const char *name;
} sigsegv_codes[] = {
#if !defined(SIRENGINE_PLATFORM_FREEBSD)
	{ SEGV_MAPERR, "Address not mapped to object" },
	{ SEGV_ACCERR, "Invalid permissions for mapped object" },
#endif
	{ 0, NULL }
};


static const struct {
	int code;
	const char *name;
} sigbus_codes[] = {
#ifndef __FreeBSD__
	{ BUS_ADRALN, "Invalid address alignment" },
	{ BUS_ADRERR, "Non-existent physical address" },
	{ BUS_OBJERR, "Object specific hardware error" },
#endif
	{ 0, NULL }
};

static int (*cc_user_info)( char *, char * );

extern "C" int I_FileAvailable( const char *filename )
{
	char szCommand[4096];
	snprintf( szCommand, sizeof( szCommand ) - 1, "which %s >/dev/null 2>&1", filename );

	if ( FILE* f = popen( szCommand, "r" ) ) {
		int status = pclose( f );
		return WIFEXITED( status ) && WEXITSTATUS( status ) == 0;
	}

	return 0;
}

static void gdb_info( pid_t pid )
{
	char respfile[64];
	char cmd_buf[128];
	FILE *f;
	int fd;

	/* Create a temp file to put gdb commands into */
	strcpy( respfile, "gdb-respfile-XXXXXX" );
	if ( ( fd = mkstemp( respfile ) ) >= 0 && ( f = fdopen( fd, "w" ) ) != NULL ) {
		fprintf( f, "attach %d\n"
				   "shell echo \"\"\n"
				   "shell echo \"* Loaded Libraries\"\n"
				   "info sharedlibrary\n"
				   "shell echo \"\"\n"
				   "shell echo \"* Threads\"\n"
				   "info threads\n"
				   "shell echo \"\"\n"
				   "shell echo \"* FPU Status\"\n"
				   "info float\n"
				   "shell echo \"\"\n"
				   "shell echo \"* Registers\"\n"
				   "info registers\n"
				   "shell echo \"\"\n"
				   "shell echo \"* Backtrace\"\n"
				   "thread apply all backtrace full\n"
				   "detach\n"
				   "quit\n", pid );
		fclose( f );

		/* Run gdb and print process info. */
		snprintf( cmd_buf, sizeof( cmd_buf ), "gdb --quiet --batch --command=%s", respfile );
		printf( "Executing: %s\n", cmd_buf );
		fflush( stdout );

		system( cmd_buf );
		/* Clean up */
		remove( respfile );
	}
	else {
		/* Error creating temp file */
		if ( fd >= 0 ) {
			close( fd );
			remove( respfile );
		}
		printf( "!!! Could not create gdb command file\n" );
	}
	fflush( stdout );
}

#define MAX_SYMBOL_LENGTH 4096

static bool32 g_bBacktraceError = false;
static FILE *g_pBacktraceOutput = NULL;
static backtrace_state *g_pBacktraceState = NULL;

static void bt_error_callback( void *data, const char *msg, int errnum )
{
	SIRENGINE_WARNING( "libbacktrace ERROR: %d - %s", errnum, msg );
	g_bBacktraceError = true;
}

static void bt_syminfo_callback( void *data, uintptr_t pc, const char *symname,
								 uintptr_t symval, uintptr_t symsize )
{
	if ( g_bBacktraceError ) {
		return;
	}

	if ( symname != NULL ) {
		int status;
		// [glnomad] 10/6/2023: fixed buffer instead of malloc'd buffer, risky however
		char name[MAX_SYMBOL_LENGTH];
		memset( name, 0, sizeof( name ) );
		size_t length = sizeof( name );
		abi::__cxa_demangle( symname, name, &length, &status );
		if ( name[0] ) {
			symname = name;
		}
		if ( g_pBacktraceOutput ) {
			fprintf( g_pBacktraceOutput, "  %-8zu %s\n", pc, symname );
		}
		SIRENGINE_LOG_LEVEL( System, ELogLevel::Info, "  %-8zu %s", pc, symname );
	} else {
		if ( g_pBacktraceOutput ) {
			fprintf( g_pBacktraceOutput, "%-8zu (unknown symbol)\n", pc );
		}
		SIRENGINE_LOG_LEVEL( System, ELogLevel::Info, "  %-8zu (unknown symbol)", pc );
	}
}

static int bt_pcinfo_callback( void *data, uintptr_t pc, const char *filename, int lineno, const char *function )
{
	if ( g_bBacktraceError ) {
		return 0;
	}

	if ( data != NULL ) {
		int *hadInfo = (int *)data;
		*hadInfo = (function != NULL);
	}

	if ( function != NULL ) {
		int status;
		// [glnomad] 10/6/2023: fixed buffer instead of malloc'd buffer, risky however
		char name[MAX_SYMBOL_LENGTH];
		memset( name, 0, sizeof( name ) );
		size_t length = sizeof( name );
		abi::__cxa_demangle( function, name, &length, &status );
		if ( name[0] ) {
			function = name;
		}

		const char *fileNameSrc = strstr( filename, "/src/" );
		if ( fileNameSrc != NULL ) {
			filename = fileNameSrc + 1; // I want "src/bla/blub.cpp:42"
		}
		if ( g_pBacktraceOutput ) {
			fprintf( g_pBacktraceOutput, "  %-8zu %-16s:%-8d %s\n", pc, filename, lineno, function );
		}
		SIRENGINE_LOG_LEVEL( System, ELogLevel::Info, "  %-8zu %-16s:%-8d %s", pc, filename, lineno, function );
	}

	return 0;
}

static void bt_error_dummy( void *data, const char *msg, int errnum )
{
	//CrashPrintf("ERROR-DUMMY: %d - %s\n", errnum, msg);
}

static int bt_simple_callback( void *data, uintptr_t pc )
{
	int pcInfoWorked;

	pcInfoWorked = 0;
	// if this fails, the executable doesn't have debug info, that's ok (=> use bt_error_dummy())
	backtrace_pcinfo( g_pBacktraceState, pc, bt_pcinfo_callback, bt_error_dummy, &pcInfoWorked );
	if ( !pcInfoWorked ) { // no debug info? use normal symbols instead
		// yes, it would be easier to call backtrace_syminfo() in bt_pcinfo_callback() if function == NULL,
		// but some libbacktrace versions (e.g. in Ubuntu 18.04's g++-7) don't call bt_pcinfo_callback
		// at all if no debug info was available - which is also the reason backtrace_full() can't be used..
		backtrace_syminfo( g_pBacktraceState, pc, bt_syminfo_callback, bt_error_callback, NULL );
	}

	return 0;
}

extern "C" void DumpStacktrace( void )
{
	if ( g_pBacktraceState != NULL ) {
		backtrace_simple( g_pBacktraceState, 3, bt_simple_callback, bt_error_callback, NULL );
	}
}

void CatchSignal( int nSignum, siginfo_t *pSigInfo, void *pContext )
{
	pid_t debugPid;
	int fd[2];

	// make sure the effective uid is the real uid
	if ( getuid() != geteuid() ) {
		raise( nSignum );
		return;
	}

	FileWrite( s_szFatalError, sizeof( s_szFatalError ) - 1, SIRENGINE_STDERR_HANDLE );
	if ( pipe( fd ) == -1 ) {
		FileWrite( s_szPipeError, sizeof( s_szPipeError ) - 1, SIRENGINE_STDERR_HANDLE );
		raise( nSignum );
		return;
	}

	crash_info.signum = nSignum;
	crash_info.pid = getpid();
	crash_info.hasSigInfo = !!pSigInfo;
	if ( pSigInfo ) {
		crash_info.siginfo = *pSigInfo;
	}
	if ( cc_user_info ) {
		cc_user_info( crash_info.szBuffer, crash_info.szBuffer + sizeof( crash_info.szBuffer ) );
	}

	switch ( ( debugPid = fork() ) ) {
	case -1:
		FileWrite( s_szForkError, sizeof( s_szForkError ) - 1, SIRENGINE_STDERR_HANDLE );
		raise( nSignum );
		return;
	case 0:
		dup2( fd[0], STDIN_FILENO );
		close( fd[0] );
		close( fd[1] );

		execl( argv0, argv0, s_szCrashSwitch, NULL );
		
		FileWrite( s_szExecError, sizeof( s_szExecError ) - 1, SIRENGINE_STDERR_HANDLE );
		_exit( EXIT_FAILURE );
	default:
#if defined(SIRENGINE_PLATFORM_LINUX)
		prctl( PR_SET_PTRACER, debugPid, 0, 0, 0 );
#endif
		FileWrite( &crash_info, sizeof( crash_info ), (void *)(uintptr_t)fd[1] );
		close( fd[0] );
		close( fd[1] );

		// wait, we'll be killed when gdb is done
		do {
			int status;
			if ( waitpid( debugPid, &status, 0 ) == debugPid && ( WIFEXITED( status ) || WIFSIGNALED( status ) ) ) {
				// the debug process died before it could kill us
				raise( nSignum );
				break;
			}
		} while ( 1 );
	};
}

static void sys_info( void )
{
	system( "echo \"System: `uname -a`\"" );
	putchar( '\n' );
	fflush( stdout );
}

static void crash_handler( const char *logfile )
{
	const char *sigdesc = "";
	int i;

	if ( fread( &crash_info, sizeof( crash_info ), 1, stdin ) != 1 ) {
		printf( "!!! Failed to retrieve info from crashed process\n" );
		exit( 1 );
	}

	/* Get the signal description */
	for( i = 0; signals[i].name; ++i ) {
		if ( signals[i].signum == crash_info.signum ) {
			sigdesc = signals[i].name;
			break;
		}
	}

	if ( crash_info.hasSigInfo ) {
		switch ( crash_info.signum ) {
		case SIGSEGV:
			for( i = 0; sigsegv_codes[i].name; ++i ) {
				if ( sigsegv_codes[i].code == crash_info.siginfo.si_code ) {
					sigdesc = sigsegv_codes[i].name;
					break;
				}
			}
			break;
		case SIGFPE:
			for ( i = 0; sigfpe_codes[i].name; ++i ) {
				if ( sigfpe_codes[i].code == crash_info.siginfo.si_code ) {
					sigdesc = sigfpe_codes[i].name;
					break;
				}
			}
			break;
		case SIGILL:
			for ( i = 0; sigill_codes[i].name; ++i ) {
				if ( sigill_codes[i].code == crash_info.siginfo.si_code ) {
					sigdesc = sigill_codes[i].name;
					break;
				}
			}
			break;
		case SIGBUS:
			for ( i = 0; sigbus_codes[i].name; ++i ) {
				if ( sigbus_codes[i].code == crash_info.siginfo.si_code ) {
					sigdesc = sigbus_codes[i].name;
					break;
				}
			}
			break;
		}
	}

	DumpStacktrace();

	printf( "%s (signal %i)\n", sigdesc, crash_info.signum );
	if ( crash_info.hasSigInfo ) {
		printf( "Address: %p\n\n", crash_info.siginfo.si_addr );
	}

	if ( logfile ) {
		/* Create crash log file and redirect shell output to it */
		if ( freopen( logfile, "wa", stdout ) != stdout ) {
			printf( "!!! Could not create %s following signal\n", logfile );
			exit( 1 );
		}
		printf( "Generating %s and killing process %d, please wait... ", logfile, crash_info.pid );

		printf( "*** Fatal Error ***\n"
			   "%s (signal %i)\n", sigdesc, crash_info.signum );
		if ( crash_info.hasSigInfo ) {
			printf( "Address: %p\n", crash_info.siginfo.si_addr );
		}
		printf( "" );
		fflush( stdout );
	}

	sys_info();

	crash_info.szBuffer[ sizeof( crash_info.szBuffer ) - 1 ] = '\0';
	printf( "%s\n", crash_info.szBuffer );
	fflush( stdout );

	if ( crash_info.pid > 0 ) {
		gdb_info( crash_info.pid );
		kill( crash_info.pid, SIGKILL );
	}

	if ( logfile ) {
		char buf[512];

		if ( I_FileAvailable( "gxmessage" ) ) {
			SIREngine_snprintf( buf, sizeof( buf ) - 1, "gxmessage -buttons \"Okay:0\" -geometry 800x600 -title \"Fatal Error\" -center -file \"%s\"", logfile );
		}
		else if ( I_FileAvailable( "kdialog" ) ) {
			SIREngine_snprintf( buf, sizeof( buf ) - 1, "kdialog --title \"Fatal Error\" --textbox \"%s\" 800 600", logfile );
		}
		else {
			SIREngine_snprintf( buf, sizeof( buf ) - 1, "xmessage -buttons \"Okay:0\" -center -file \"%s\"", logfile );
		}

		system( buf );
	}
	exit( 0 );
}

int cc_install_handlers( int argc, char **argv, int num_signals, int *signals, const char *logfile, int (*user_info)(char *, char * ) )
{
	struct sigaction sa;
	stack_t altss;
	int retval;
	static char *altstack = (char *)alloca( SIGSTKSZ );

	if ( argc == 2 && strcmp( argv[1], s_szCrashSwitch ) == 0 ) {
		crash_handler( logfile );
	}

	cc_user_info = user_info;

	if ( argv[0][0] == '/' ) {
		snprintf( argv0, sizeof( argv0 ), "%s", argv[0] );
	} else {
		getcwd( argv0, sizeof( argv0 ) );
		retval = strlen( argv0 );
		snprintf( argv0 + retval, sizeof( argv0 ) - retval, "/%s", argv[0] );
	}

	/* Set an alternate signal stack so SIGSEGVs caused by stack overflows
	 * still run */
	altss.ss_sp = altstack;
	altss.ss_flags = 0;
	altss.ss_size = SIGSTKSZ;
	sigaltstack( &altss, NULL );

	memset( &sa, 0, sizeof( sa ) );
	sa.sa_sigaction = CatchSignal;
	sa.sa_flags = SA_RESETHAND | SA_NODEFER | SA_SIGINFO | SA_ONSTACK;
	sigemptyset( &sa.sa_mask );

	retval = 0;
	while ( num_signals-- ) {
		if ( ( *signals != SIGSEGV && *signals != SIGILL && *signals != SIGFPE &&
			*signals != SIGBUS ) || sigaction( *signals, &sa, NULL ) == -1 )
		{
			*signals = 0;
			retval = -1;
		}
		++signals;
	}
	return retval;
}

extern int myargc;
extern char **myargv;

static int GetCrashInfo( char *pBuffer, char *pEnd )
{
	return strlen( pBuffer );
}

extern "C" void InitCrashHandler( void )
{
	SIRENGINE_LOG_LEVEL( System, ELogLevel::Info, "Initializing Platform Crash Handler..." );

	g_pBacktraceState = backtrace_create_state( "SIREngine", false, bt_error_callback, NULL );

#if !defined(SIRENGINE_PLATFORM_APPLE)
	{
		int s[4] = { SIGSEGV, SIGILL, SIGFPE, SIGBUS };
		cc_install_handlers( myargc, myargv, 4, s, "SIREngine-crash.log", GetCrashInfo );
	}
#endif

	nOOMBackupSize = OOM_MEMORY_BACKUP_POOL_SIZE;
	pOOMBackup = malloc( nOOMBackupSize );
}

static char execBuffer[ 1024 ];
static char *execBufferPointer;
static char *execArgv[ 16 ];
static int execArgc;

/*
==============
Sys_ClearExecBuffer
==============
*/
static void Sys_ClearExecBuffer( void )
{
	execBufferPointer = execBuffer;
	memset( execArgv, 0, sizeof( execArgv ) );
	execArgc = 0;
}

/*
==============
Sys_AppendToExecBuffer
==============
*/
static void Sys_AppendToExecBuffer( const char *text )
{
	size_t size = sizeof( execBuffer ) - ( execBufferPointer - execBuffer );
	int length = strlen( text ) + 1;

	if ( length > size || execArgc >= SIREngine_StaticArrayLength( execArgv ) ) {
		return;
	}

	SIREngine_strncpyz( execBufferPointer, text, size );
	execArgv[ execArgc++ ] = execBufferPointer;

	execBufferPointer += length;
}

/*
==============
Sys_Exec
==============
*/
static int Sys_Exec( void )
{
	pid_t pid = fork( );

	if ( pid < 0 ) {
		return -1;
	}

	if ( pid ) {
		// Parent
		int exitCode;

		wait( &exitCode );

		return WEXITSTATUS( exitCode );
	}
	else {
		// Child
		execvp( execArgv[ 0 ], execArgv );

		// Failed to execute
		exit( -1 );

		return -1;
	}
}

/*
==============
Sys_ZenityCommand
==============
*/
static void Sys_ZenityCommand( dialogType_t type, const char *message, const char *title )
{
	Sys_ClearExecBuffer();
	Sys_AppendToExecBuffer( "zenity" );

	switch ( type ) {
	default:
	case DT_INFO:      Sys_AppendToExecBuffer( "--info" ); break;
	case DT_WARNING:   Sys_AppendToExecBuffer( "--warning" ); break;
	case DT_ERROR:     Sys_AppendToExecBuffer( "--error" ); break;
	case DT_YES_NO:
		Sys_AppendToExecBuffer( "--question" );
		Sys_AppendToExecBuffer( "--ok-label=Yes" );
		Sys_AppendToExecBuffer( "--cancel-label=No" );
		break;
	case DT_OK_CANCEL:
		Sys_AppendToExecBuffer( "--question" );
		Sys_AppendToExecBuffer( "--ok-label=OK" );
		Sys_AppendToExecBuffer( "--cancel-label=Cancel" );
		break;
	};

	Sys_AppendToExecBuffer( SIRENGINE_TEMP_VSTRING( "--text=%s", message ) );
	Sys_AppendToExecBuffer( SIRENGINE_TEMP_VSTRING( "--title=%s", title ) );
}

/*
==============
Sys_KdialogCommand
==============
*/
static void Sys_KdialogCommand( dialogType_t type, const char *message, const char *title )
{
	Sys_ClearExecBuffer( );
	Sys_AppendToExecBuffer( "kdialog" );

	switch( type ) {
	default:
	case DT_INFO:      Sys_AppendToExecBuffer( "--msgbox" ); break;
	case DT_WARNING:   Sys_AppendToExecBuffer( "--sorry" ); break;
	case DT_ERROR:     Sys_AppendToExecBuffer( "--error" ); break;
	case DT_YES_NO:    Sys_AppendToExecBuffer( "--warningyesno" ); break;
	case DT_OK_CANCEL: Sys_AppendToExecBuffer( "--warningcontinuecancel" ); break;
	};

	Sys_AppendToExecBuffer( message );
	Sys_AppendToExecBuffer( SIRENGINE_TEMP_VSTRING( "--title=%s", title ) );
}

/*
==============
Sys_XmessageCommand
==============
*/
static void Sys_XmessageCommand( dialogType_t type, const char *message, const char *title )
{
	Sys_ClearExecBuffer( );
	Sys_AppendToExecBuffer( "xmessage" );
	Sys_AppendToExecBuffer( "-buttons" );

	switch ( type ) {
	default:           Sys_AppendToExecBuffer( "OK:0" ); break;
	case DT_YES_NO:    Sys_AppendToExecBuffer( "Yes:0,No:1" ); break;
	case DT_OK_CANCEL: Sys_AppendToExecBuffer( "OK:0,Cancel:1" ); break;
	};

	Sys_AppendToExecBuffer( "-center" );
	Sys_AppendToExecBuffer( message );
}

/*
==============
Sys_Dialog

Display a *nix dialog box
==============
*/
dialogResult_t Sys_Dialog( dialogType_t type, const char *message, const char *title )
{
	typedef enum
	{
		NONE = 0,
		ZENITY,
		KDIALOG,
		XMESSAGE,
		NUM_DIALOG_PROGRAMS
	} dialogCommandType_t;
	typedef void (*dialogCommandBuilder_t)( dialogType_t, const char *, const char * );

	const char				*session = getenv( "DESKTOP_SESSION" );
	bool32					tried[ NUM_DIALOG_PROGRAMS ] = { false };
	dialogCommandBuilder_t	commands[ NUM_DIALOG_PROGRAMS ] = { NULL };
	dialogCommandType_t		preferredCommandType = NONE;
	int						i;

	commands[ ZENITY ] = &Sys_ZenityCommand;
	commands[ KDIALOG ] = &Sys_KdialogCommand;
	commands[ XMESSAGE ] = &Sys_XmessageCommand;

	// This may not be the best way
	if ( !SIREngine_stricmp( session, "gnome" ) ) {
		preferredCommandType = ZENITY;
	} else if ( !SIREngine_stricmp( session, "kde" ) ) {
		preferredCommandType = KDIALOG;
	}

	for ( i = NONE + 1; i < NUM_DIALOG_PROGRAMS; i++ ) {
		if ( preferredCommandType != NONE && preferredCommandType != i ) {
			continue;
		}

		if ( !tried[ i ] ) {
			int exitCode;

			commands[ i ]( type, message, title );
			exitCode = Sys_Exec( );

			if ( exitCode >= 0 ) {
				switch ( type ) {
				case DT_YES_NO:    return exitCode ? DR_NO : DR_YES;
				case DT_OK_CANCEL: return exitCode ? DR_CANCEL : DR_OK;
				default:           return DR_OK;
				};
			}

			tried[ i ] = true;

			// The preference failed, so start again in order
			if ( preferredCommandType != NONE ) {
				preferredCommandType = NONE;
				i = NONE + 1;
			}
		}
	}

	SIRENGINE_LOG_LEVEL( System, ELogLevel::Warning, "failed to show a dialog" );
	return DR_OK;
}

};