#include "stdex/stack.h"

#if defined _DEBUG
#   if (defined _M_IX86 || defined _M_X64 || defined _M_IA64) && defined WIN32

#   include <windows.h>
#   include <stdio.h>

#   include <DbgHelp.h>
#   include <TlHelp32.h>
#   include <Psapi.h>

//////////////////////////////////////////////////////////////////////////
// dbghelp.dll
//////////////////////////////////////////////////////////////////////////
typedef VOID( __stdcall * TRtlCaptureContext )(OUT PCONTEXT ContextRecord);
typedef BOOL( __stdcall * TSymCleanup )(IN HANDLE hProcess);
typedef PVOID( __stdcall * TSymFunctionTableAccess64 )(HANDLE hProcess, DWORD64 AddrBase);
typedef BOOL( __stdcall * TSymGetLineFromAddr64 )(IN HANDLE hProcess, IN DWORD64 dwAddr,
    OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE64 Line);
typedef DWORD64( __stdcall * TSymGetModuleBase64 )(IN HANDLE hProcess, IN DWORD64 dwAddr);

typedef BOOL( __stdcall * TSymGetModuleInfo64 )(IN HANDLE hProcess, IN DWORD64 dwAddr, OUT PIMAGEHLP_MODULE64 ModuleInfo);
typedef DWORD( __stdcall * TSymGetOptions )(VOID);
typedef BOOL( __stdcall * TSymGetSymFromAddr64 )(IN HANDLE hProcess, IN DWORD64 dwAddr,
    OUT PDWORD64 pdwDisplacement, OUT PIMAGEHLP_SYMBOL64 Symbol);
typedef BOOL( __stdcall * TSymInitialize )(IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess);
typedef DWORD64( __stdcall * TSymLoadModule64 )(IN HANDLE hProcess, IN HANDLE hFile,
    IN PCSTR ImageName, IN PCSTR ModuleName, IN DWORD64 BaseOfDll, IN DWORD SizeOfDll);
typedef DWORD( __stdcall * TSymSetOptions )(IN DWORD SymOptions);
typedef BOOL( __stdcall * TStackWalk64 )(
    DWORD MachineType,
    HANDLE hProcess,
    HANDLE hThread,
    LPSTACKFRAME64 StackFrame,
    PVOID ContextRecord,
    PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
    PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
    PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
    PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress);
typedef DWORD( __stdcall WINAPI * TUnDecorateSymbolName )(PCSTR DecoratedName, PSTR UnDecoratedName,
    DWORD UndecoratedLength, DWORD Flags);
typedef BOOL( __stdcall WINAPI * TSymGetSearchPath )(HANDLE hProcess, PSTR SearchPath, DWORD SearchPathLength);

//////////////////////////////////////////////////////////////////////////
// psapi.dll
//////////////////////////////////////////////////////////////////////////
typedef BOOL( __stdcall * TEnumProcessModules )(HANDLE hProcess, HMODULE * lphModule, DWORD cb, LPDWORD lpcbNeeded);
typedef DWORD( __stdcall * TGetModuleFileNameEx )(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize);
typedef DWORD( __stdcall * TGetModuleBaseName )(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize);
typedef BOOL( __stdcall * TGetModuleInformation )(HANDLE hProcess, HMODULE hModule, LPMODULEINFO pmi, DWORD nSize);
//////////////////////////////////////////////////////////////////////////
static BOOL GetModuleListPSAPI( TSymLoadModule64 pSymLoadModule64, HANDLE hProcess, stdex::log_callstack_t _log, void * _ud )
{
    HINSTANCE hPsapi = LoadLibraryW( L"psapi.dll" );

    if( hPsapi == NULL )
    {
        (*_log)(_ud, "invalid open psapi.dll");

        return FALSE;
    }

    TEnumProcessModules pEnumProcessModules = (TEnumProcessModules)GetProcAddress( hPsapi, "EnumProcessModules" );
    TGetModuleFileNameEx pGetModuleFileNameExA = (TGetModuleFileNameEx)GetProcAddress( hPsapi, "GetModuleFileNameExA" );
    TGetModuleBaseName pGetModuleBaseNameA = (TGetModuleFileNameEx)GetProcAddress( hPsapi, "GetModuleBaseNameA" );
    TGetModuleInformation pGetModuleInformation = (TGetModuleInformation)GetProcAddress( hPsapi, "GetModuleInformation" );

    if( (pEnumProcessModules == NULL) || (pGetModuleFileNameExA == NULL) || (pGetModuleBaseNameA == NULL) || (pGetModuleInformation == NULL) )
    {
        (*_log)(_ud, "invalid load function psapi.dll");

        FreeLibrary( hPsapi );

        return FALSE;
    }

    HMODULE hMods[2048];

    DWORD cbNeeded;
    if( pEnumProcessModules( hProcess, hMods, sizeof( hMods ), &cbNeeded ) == FALSE )
    {
        (*_log)(_ud, "invalid enum process modules psapi.dll");

        FreeLibrary( hPsapi );

        return FALSE;
    }

    if( cbNeeded > 1024 )
    {
        (*_log)(_ud, "invalid needed > 1024 psapi.dll");

        FreeLibrary( hPsapi );

        return FALSE;
    }

    for( DWORD i = 0; i < cbNeeded / sizeof( HMODULE ); i++ )
    {
        MODULEINFO mi;
        if( pGetModuleInformation( hProcess, hMods[i], &mi, sizeof mi ) == FALSE )
        {
            (*_log)(_ud, "invalid get module information psapi.dll");

            return FALSE;
        }

        char mFileName[MAX_PATH] = {};
        DWORD FileNameLen = pGetModuleFileNameExA( hProcess, hMods[i], mFileName, MAX_PATH );

        char mBaseName[MAX_PATH] = {};
        DWORD BaseNameLen = pGetModuleBaseNameA( hProcess, hMods[i], mBaseName, MAX_PATH );

        if( FileNameLen <= 0 || BaseNameLen <= 0 )
        {
            (*_log)(_ud, "invalid file base name len psapi.dll");

            return FALSE;
        }

        if( pSymLoadModule64( hProcess, 0, mFileName, mBaseName, (DWORD64)mi.lpBaseOfDll, mi.SizeOfImage ) == FALSE )
        {
            (*_log)(_ud, "invalid load module file '%s' base '%s' psapi.dll", mFileName, mBaseName);

            return FALSE;
        }
    }

    FreeLibrary( hPsapi );

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
static BOOL GetModuleInfo( TSymGetModuleInfo64 pSymGetModuleInfo64, HANDLE hProcess, DWORD64 baseAddr, PIMAGEHLP_MODULE64 pModuleInfo )
{
    if( pSymGetModuleInfo64 == NULL )
    {
        return FALSE;
    }

    pModuleInfo->SizeOfStruct = sizeof( IMAGEHLP_MODULE64 );

    unsigned char pData[4096]; // reserve enough memory, so the bug in v6.3.5.1 does not lead to memory-overwrites...
    memcpy( pData, pModuleInfo, sizeof( IMAGEHLP_MODULE64 ) );

    if( pSymGetModuleInfo64( hProcess, baseAddr, (IMAGEHLP_MODULE64 *)pData ) == FALSE )
    {
        return FALSE;
    }

    memcpy( pModuleInfo, pData, sizeof( IMAGEHLP_MODULE64 ) );
    pModuleInfo->SizeOfStruct = sizeof( IMAGEHLP_MODULE64 );

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
static BOOL __stdcall ReadMemoryRoutine(
    HANDLE      hProcess,
    DWORD64     qwBaseAddress,
    PVOID       lpBuffer,
    DWORD       nSize,
    LPDWORD     lpNumberOfBytesRead
)
{
    SIZE_T st;
    BOOL bRet = ReadProcessMemory( hProcess, (LPVOID)qwBaseAddress, lpBuffer, nSize, &st );
    *lpNumberOfBytesRead = (DWORD)st;

    return bRet;
}
//////////////////////////////////////////////////////////////////////////
#define STACKWALK_MAX_NAMELEN 1024
//////////////////////////////////////////////////////////////////////////
typedef struct _CallstackEntry
{
    DWORD64 offset;
    CHAR name[STACKWALK_MAX_NAMELEN];
    CHAR undName[STACKWALK_MAX_NAMELEN];
    CHAR undFullName[STACKWALK_MAX_NAMELEN];
    DWORD lineNumber;
    CHAR lineFileName[STACKWALK_MAX_NAMELEN];
    CHAR moduleName[STACKWALK_MAX_NAMELEN];
} CallstackEntry;
//////////////////////////////////////////////////////////////////////////
enum CallstackEntryType
{
    firstEntry, nextEntry, lastEntry
};
//////////////////////////////////////////////////////////////////////////
static void OnCallstackEntry( std::string & _stack, CallstackEntry & entry )
{
    CHAR buffer[STACKWALK_MAX_NAMELEN];
    if( entry.offset != 0 )
    {
        if( entry.name[0] == 0 )
        {
            strcpy( entry.name, "(function-name not available)" );
        }

        if( entry.undName[0] != 0 )
        {
            strcpy( entry.name, entry.undName );
        }

        if( entry.undFullName[0] != 0 )
        {
            strcpy( entry.name, entry.undFullName );
        }

        if( entry.lineFileName[0] == 0 )
        {
            strcpy( entry.lineFileName, "(filename not available)" );

            if( entry.moduleName[0] == 0 )
            {
                strcpy( entry.moduleName, "(module-name not available)" );
            }

            sprintf( buffer, "%p (%s): %s (%d): %s\n", (LPVOID)entry.offset, entry.moduleName, entry.lineFileName, entry.lineNumber, entry.name );
        }
        else
        {
            sprintf( buffer, "%s (%d): %s\n", entry.lineFileName, entry.lineNumber, entry.name );
        }

        _stack.append( buffer );
    }
}
//////////////////////////////////////////////////////////////////////////
static bool GetCallstack( std::string & _stack, stdex::log_callstack_t _log, void * _ud, PCONTEXT _context, HMODULE hDbhHelp, HMODULE hKernel32, HANDLE hThread, HANDLE hProcess )
{
    TRtlCaptureContext pRtlCaptureContext = (TRtlCaptureContext)GetProcAddress( hKernel32, "RtlCaptureContext" );
    TStackWalk64 pStackWalk64 = (TStackWalk64)GetProcAddress( hDbhHelp, "StackWalk64" );
    TSymGetOptions pSymGetOptions = (TSymGetOptions)GetProcAddress( hDbhHelp, "SymGetOptions" );
    TSymSetOptions pSymSetOptions = (TSymSetOptions)GetProcAddress( hDbhHelp, "SymSetOptions" );

    TSymFunctionTableAccess64 pSymFunctionTableAccess64 = (TSymFunctionTableAccess64)GetProcAddress( hDbhHelp, "SymFunctionTableAccess64" );
    TSymGetLineFromAddr64 pSymGetLineFromAddr64 = (TSymGetLineFromAddr64)GetProcAddress( hDbhHelp, "SymGetLineFromAddr64" );
    TSymGetModuleBase64 pSymGetModuleBase64 = (TSymGetModuleBase64)GetProcAddress( hDbhHelp, "SymGetModuleBase64" );
    TSymGetModuleInfo64 pSymGetModuleInfo64 = (TSymGetModuleInfo64)GetProcAddress( hDbhHelp, "SymGetModuleInfo64" );
    TSymGetSymFromAddr64 pSymGetSymFromAddr64 = (TSymGetSymFromAddr64)GetProcAddress( hDbhHelp, "SymGetSymFromAddr64" );
    TUnDecorateSymbolName pUnDecorateSymbolName = (TUnDecorateSymbolName)GetProcAddress( hDbhHelp, "UnDecorateSymbolName" );
    TSymLoadModule64 pSymLoadModule64 = (TSymLoadModule64)GetProcAddress( hDbhHelp, "SymLoadModule64" );
    TSymGetSearchPath pSymGetSearchPath = (TSymGetSearchPath)GetProcAddress( hDbhHelp, "SymGetSearchPath" );

    if( pRtlCaptureContext == NULL || pStackWalk64 == NULL || pSymGetOptions == NULL ||
        pSymSetOptions == NULL || pSymFunctionTableAccess64 == NULL || pSymGetModuleBase64 == NULL ||
        pSymGetModuleInfo64 == NULL || pSymGetSymFromAddr64 == NULL || pUnDecorateSymbolName == NULL || pSymLoadModule64 == NULL ||
        pSymGetSearchPath == NULL )
    {
        (*_log)(_ud, "invalid get function");

        return false;
    }

    DWORD symOptions = pSymGetOptions();
    symOptions |= SYMOPT_LOAD_LINES;
    symOptions |= SYMOPT_FAIL_CRITICAL_ERRORS;

    symOptions = pSymSetOptions( symOptions );

    if( GetModuleListPSAPI( pSymLoadModule64, hProcess, _log, _ud ) == FALSE )
    {
        (*_log)(_ud, "invalid GetModuleListPSAPI");

        return false;
    }

    CONTEXT contex;

    if( _context == NULL )
    {
        memset( &contex, 0, sizeof( CONTEXT ) );
        contex.ContextFlags = CONTEXT_FULL;
        (*pRtlCaptureContext)(&contex);
    }
    else
    {
        contex = *_context;
    }

    STACKFRAME64 frame;
    memset( &frame, 0, sizeof( frame ) );

    DWORD imageType = IMAGE_FILE_MACHINE_I386;

#ifdef _M_IX86
    imageType = IMAGE_FILE_MACHINE_I386;
    frame.AddrPC.Offset = contex.Eip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = contex.Ebp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = contex.Esp;
    frame.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
    imageType = IMAGE_FILE_MACHINE_AMD64;
    frame.AddrPC.Offset = contex.Rip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = contex.Rsp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = contex.Rsp;
    frame.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
    imageType = IMAGE_FILE_MACHINE_IA64;
    frame.AddrPC.Offset = contex.StIIP;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = contex.IntSp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrBStore.Offset = contex.RsBSP;
    frame.AddrBStore.Mode = AddrModeFlat;
    frame.AddrStack.Offset = contex.IntSp;
    frame.AddrStack.Mode = AddrModeFlat;
#else
#error "Platform not supported!"
#endif

    unsigned char pSymBuff[sizeof( IMAGEHLP_SYMBOL64 ) + STACKWALK_MAX_NAMELEN];
    IMAGEHLP_SYMBOL64 * pSym = (IMAGEHLP_SYMBOL64 *)pSymBuff;
    memset( pSym, 0, sizeof( IMAGEHLP_SYMBOL64 ) + STACKWALK_MAX_NAMELEN );

    pSym->SizeOfStruct = sizeof( IMAGEHLP_SYMBOL64 );
    pSym->MaxNameLength = STACKWALK_MAX_NAMELEN;

    IMAGEHLP_LINE64 Line;
    memset( &Line, 0, sizeof( Line ) );
    Line.SizeOfStruct = sizeof( Line );

    IMAGEHLP_MODULE64 Module;
    memset( &Module, 0, sizeof( Module ) );
    Module.SizeOfStruct = sizeof( Module );

    for( int frameNum = 0;; ++frameNum )
    {
        if( pStackWalk64( imageType, hProcess, hThread, &frame, &contex, &ReadMemoryRoutine, pSymFunctionTableAccess64, pSymGetModuleBase64, NULL ) == FALSE )
        {
            (*_log)(_ud, "invalid pStackWalk64");

            return false;
        }

        if( frame.AddrPC.Offset == frame.AddrReturn.Offset )
        {
            (*_log)(_ud, "invalid frame.AddrPC.Offset == frame.AddrReturn.Offset");

            return false;
        }

        CallstackEntry csEntry;
        memset( &csEntry, 0, sizeof( csEntry ) );
        csEntry.offset = frame.AddrPC.Offset;

        if( frame.AddrPC.Offset != 0 )
        {
            DWORD64 offsetFromSmybol;
            if( pSymGetSymFromAddr64( hProcess, frame.AddrPC.Offset, &offsetFromSmybol, pSym ) == TRUE )
            {
                strcpy( csEntry.name, pSym->Name );
                pUnDecorateSymbolName( pSym->Name, csEntry.undName, STACKWALK_MAX_NAMELEN, UNDNAME_NAME_ONLY );
                pUnDecorateSymbolName( pSym->Name, csEntry.undFullName, STACKWALK_MAX_NAMELEN, UNDNAME_COMPLETE );
            }

            if( pSymGetLineFromAddr64 != NULL )
            {
                DWORD offsetFromLine;
                if( pSymGetLineFromAddr64( hProcess, frame.AddrPC.Offset, &offsetFromLine, &Line ) == TRUE )
                {
                    csEntry.lineNumber = Line.LineNumber;
                    strcpy( csEntry.lineFileName, Line.FileName );
                }
            }

            if( GetModuleInfo( pSymGetModuleInfo64, hProcess, frame.AddrPC.Offset, &Module ) == TRUE )
            {
                strcpy( csEntry.moduleName, Module.ModuleName );
            }
        }

        OnCallstackEntry( _stack, csEntry );

        if( frame.AddrReturn.Offset == 0 )
        {
            break;
        }
    }

    return true;
}
//////////////////////////////////////////////////////////////////////////
namespace stdex
{
    bool get_callstack( std::string & _stack, log_callstack_t _log, void * _ud, void * _context )
    {
        HANDLE hThread = GetCurrentThread();
        HANDLE hProcess = GetCurrentProcess();

        HMODULE hDbhHelp = LoadLibraryW( L"dbghelp.dll" );

        if( hDbhHelp == NULL )
        {
            (*_log)(_ud, "invalid load dbghelp.dll");

            return false;
        }

        TSymInitialize pSymInitialize = (TSymInitialize)GetProcAddress( hDbhHelp, "SymInitialize" );
        TSymCleanup pSymCleanup = (TSymCleanup)GetProcAddress( hDbhHelp, "SymCleanup" );

        if( pSymInitialize == NULL || pSymCleanup == NULL )
        {
            (*_log)(_ud, "invalid get dbghelp.dll [SymInitialize||SymCleanup]");

            FreeLibrary( hDbhHelp );

            return false;
        }

        HMODULE hKernel32 = LoadLibraryW( L"Kernel32.dll" );

        if( hKernel32 == NULL )
        {
            FreeLibrary( hDbhHelp );

            (*_log)(_ud, "invalid load Kernel32.dll");

            return false;
        }

        if( pSymInitialize( hProcess, NULL, FALSE ) == FALSE )
        {
            (*_log)(_ud, "invalid SymInitialize");

            FreeLibrary( hDbhHelp );
            FreeLibrary( hKernel32 );

            return false;
        }

        bool successful = GetCallstack( _stack, _log, _ud, (PCONTEXT)_context, hDbhHelp, hKernel32, hThread, hProcess );

        pSymCleanup( hProcess );

        FreeLibrary( hDbhHelp );
        FreeLibrary( hKernel32 );

        return successful;
    }
}
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
namespace stdex
{
    bool get_callstack( std::string & _stack, log_callstack_t _log, void * _ud, void * _context )
    {
        (void)_stack;
        (void)_context;

        (*_log)(_ud, "this platform not support callstack");

        return false;
    }
}
#endif
#else
//////////////////////////////////////////////////////////////////////////
namespace stdex
{
    bool get_callstack( std::string & _stack, log_callstack_t _log, void * _ud, void * _context )
    {
        (void)_stack;
        (void)_log;
        (void)_ud;
        (void)_context;

        (*_log)(_ud, "this platform not support callstack");

        return false;
    }
}
#endif