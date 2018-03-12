/**@add by Felix
*@brief ¶ÑÕ»×·×ÙÀà
*/
#ifndef SIMPLESYMBOLENGINE_H
#define SIMPLESYMBOLENGINE_H
#include <usCore\Export.h>
#include <iosfwd>
#include <string>
#include <windows.h>
#include <dbghelp.h>

/** Symbol Engine wrapper to assist with processing PDB information */
class USCORE_EXPORT SimpleSymbolEngine
{
public:
    /** Get the symbol engine for this process */
    static SimpleSymbolEngine &instance();

    /** Convert an address to a string */
    std::string addressToString( PVOID address );

    /** Provide a stack trace for the specified stack frame */
    void StackTrace( PCONTEXT pContext, std::ostream & os );

private:
    /* don't copy or assign */
    SimpleSymbolEngine( SimpleSymbolEngine const & );
    SimpleSymbolEngine& operator=( SimpleSymbolEngine const & );

    /* Construct wrapper for this process */
    SimpleSymbolEngine();

    HANDLE hProcess;

public: // Work around for MSVC 6 bug
    /* Destroy information for this process */
    ~SimpleSymbolEngine();
private:
};

#endif // SIMPLESYMBOLENGINE_H
