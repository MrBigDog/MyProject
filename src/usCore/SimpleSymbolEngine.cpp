//#include "stdafx.h"
#include "SimpleSymbolEngine.h"

#include <windows.h>
#include <psapi.h>

#include <iostream>
#include <sstream>
#include <cstddef>

#include <dbghelp.h>

#pragma comment( lib, "dbghelp" )

static char const szRCSID[] = "$Id: SimpleSymbolEngine.cpp,v 1.4 2005/05/04 21:52:05 Eleanor Exp $";

//////////////////////////////////////////////////////////////////////////////////////
// Singleton for the engine (SymInitialize doesn't support multiple calls)
SimpleSymbolEngine& SimpleSymbolEngine::instance()
{
static SimpleSymbolEngine theEngine;
    return theEngine;
}

/////////////////////////////////////////////////////////////////////////////////////
SimpleSymbolEngine::SimpleSymbolEngine()
{
    hProcess = GetCurrentProcess();

    DWORD dwOpts = SymGetOptions();
    dwOpts |= SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS;
    SymSetOptions ( dwOpts );

    ::SymInitialize( hProcess, 0, true );
}

/////////////////////////////////////////////////////////////////////////////////////
SimpleSymbolEngine::~SimpleSymbolEngine()
{
    ::SymCleanup( hProcess );
}

/////////////////////////////////////////////////////////////////////////////////////
std::string SimpleSymbolEngine::addressToString( PVOID address )
{
    std::ostringstream oss;

    // First the raw address
    oss << "0x" << address;

    // Then any name for the symbol
    struct tagSymInfo
    {
        IMAGEHLP_SYMBOL symInfo;
        char nameBuffer[ 4 * 256 ];
    } SymInfo = { { sizeof( IMAGEHLP_SYMBOL ) } };

    IMAGEHLP_SYMBOL * pSym = &SymInfo.symInfo;
    pSym->MaxNameLength = sizeof( SymInfo ) - offsetof( tagSymInfo, symInfo.Name );

#ifndef WIN64
    DWORD dwDisplacement;
    if ( SymGetSymFromAddr( hProcess, (DWORD)address, &dwDisplacement, pSym) )
    {
        oss << " " << pSym->Name;
		if ( dwDisplacement != 0 )
		oss << "+0x" << std::hex << dwDisplacement << std::dec;
    }
        
    // Finally any file/line number
    IMAGEHLP_LINE lineInfo = { sizeof( IMAGEHLP_LINE ) };
    if ( SymGetLineFromAddr( hProcess, (DWORD)address, &dwDisplacement, &lineInfo ) )
    {
        char const *pDelim = strrchr( lineInfo.FileName, '\\' );
        oss << " at " << ( pDelim ? pDelim + 1 : lineInfo.FileName ) << "(" << lineInfo.LineNumber << ")";
    }
#else
	DWORD64 dwDisplacement;
	if ( SymGetSymFromAddr( hProcess, (DWORD64)address, &dwDisplacement, pSym) )
	{
		oss << " " << pSym->Name;
		if ( dwDisplacement != 0 )
			oss << "+0x" << std::hex << dwDisplacement << std::dec;
	}

	// Finally any file/line number
	IMAGEHLP_LINE lineInfo = { sizeof( IMAGEHLP_LINE ) };
	DWORD dwDisplacement2;
	if ( SymGetLineFromAddr( hProcess, (DWORD64)address, &dwDisplacement2, &lineInfo ) )
	{
		char const *pDelim = strrchr( lineInfo.FileName, '\\' );
		oss << " at " << ( pDelim ? pDelim + 1 : lineInfo.FileName ) << "(" << lineInfo.LineNumber << ")";
	}
#endif
    return oss.str();
}

/////////////////////////////////////////////////////////////////////////////////////
// StackTrace: try to trace the stack to the given output
void SimpleSymbolEngine::StackTrace( PCONTEXT pContext, std::ostream & os )
{
    os << "  Frame       Code address\n";

#ifndef WIN64
    STACKFRAME stackFrame = {0};

    stackFrame.AddrPC.Offset = pContext->Eip;
    stackFrame.AddrPC.Mode = AddrModeFlat;

    stackFrame.AddrFrame.Offset = pContext->Ebp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;

    stackFrame.AddrStack.Offset = pContext->Esp;
    stackFrame.AddrStack.Mode = AddrModeFlat;

    while ( ::StackWalk(
       IMAGE_FILE_MACHINE_I386,
       hProcess,
       GetCurrentThread(), // this value doesn't matter much if previous one is a real handle
       &stackFrame, 
       pContext,
       NULL,
       ::SymFunctionTableAccess,
       ::SymGetModuleBase,
       NULL ) )
    {
        os << "  0x" << (PVOID) stackFrame.AddrFrame.Offset << "  " << addressToString( (PVOID)stackFrame.AddrPC.Offset ) << "\n";
    }
#else
	STACKFRAME64 stackFrame = {0};

	stackFrame.AddrPC.Offset = pContext-> Rip;
	stackFrame.AddrPC.Mode = AddrModeFlat;

	stackFrame.AddrFrame.Offset = pContext->Rsp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;

	stackFrame.AddrStack.Offset = pContext->Rsp;
	stackFrame.AddrStack.Mode = AddrModeFlat;

	while ( ::StackWalk(
		IMAGE_FILE_MACHINE_AMD64,
		hProcess,
		GetCurrentThread(), // this value doesn't matter much if previous one is a real handle
		&stackFrame, 
		pContext,
		NULL,
		::SymFunctionTableAccess,
		::SymGetModuleBase,
		NULL ) )
	{
		os << "  0x" << (PVOID) stackFrame.AddrFrame.Offset << "  " << addressToString( (PVOID)stackFrame.AddrPC.Offset ) << "\n";
	}
#endif

    os.flush();
}
