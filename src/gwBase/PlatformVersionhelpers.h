#ifndef GWUTIL_VERSIONHELPERS_H
#define GWUTIL_VERSIONHELPERS_H 1

#include <sdkddkver.h>

#ifdef __cplusplus
#define VERSIONHELPERAPI inline bool
#else
#define VERSIONHELPERAPI FORCEINLINE BOOL
#endif // __cplusplus

/* Windows 8/8.1 version numbers, not defined in the Windows 7 SDK. */
#ifndef _WIN32_WINNT_WIN8
#define _WIN32_WINNT_WIN8 0x0602
#endif
#ifndef _WIN32_WINNT_WINBLUE
#define _WIN32_WINNT_WINBLUE 0x0603
#endif

VERSIONHELPERAPI IsWindowsVersionOrGreater(WORD wMajorVersion,
    WORD wMinorVersion, WORD wServicePackMajor)
{
    OSVERSIONINFOEXW osVersionInfo;
    DWORDLONG dwlConditionMask = 0;
    ZeroMemory(&osVersionInfo, sizeof(osVersionInfo));
    osVersionInfo.dwOSVersionInfoSize = sizeof(osVersionInfo);
    osVersionInfo.dwMajorVersion = wMajorVersion;
    osVersionInfo.dwMinorVersion = wMinorVersion;
    osVersionInfo.wServicePackMajor = wServicePackMajor;
    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
    return VerifyVersionInfoW(&osVersionInfo, VER_MAJORVERSION
        | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask);
}

VERSIONHELPERAPI IsWindowsXPOrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP),
        LOBYTE(_WIN32_WINNT_WINXP), 0);
}

VERSIONHELPERAPI IsWindowsXPSP1OrGreater() 
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP),
        LOBYTE(_WIN32_WINNT_WINXP), 1);
}

VERSIONHELPERAPI IsWindowsXPSP2OrGreater() 
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP),
        LOBYTE(_WIN32_WINNT_WINXP), 2);
}

VERSIONHELPERAPI IsWindowsXPSP3OrGreater() 
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP),
        LOBYTE(_WIN32_WINNT_WINXP), 3);
}

VERSIONHELPERAPI IsWindowsVistaOrGreater() 
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA),
        LOBYTE(_WIN32_WINNT_VISTA), 0);
}

VERSIONHELPERAPI IsWindowsVistaSP1OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA),
        LOBYTE(_WIN32_WINNT_VISTA), 1);
}

VERSIONHELPERAPI IsWindowsVistaSP2OrGreater() 
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA),
        LOBYTE(_WIN32_WINNT_VISTA), 2);
}

VERSIONHELPERAPI IsWindows7OrGreater() 
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7),
        LOBYTE(_WIN32_WINNT_WIN7), 0);
}

VERSIONHELPERAPI IsWindows7SP1OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7),
        LOBYTE(_WIN32_WINNT_WIN7), 1);
}

VERSIONHELPERAPI IsWindows8OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN8),
        LOBYTE(_WIN32_WINNT_WIN8), 0);
}

VERSIONHELPERAPI IsWindows8Point1OrGreater() 
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINBLUE),
        LOBYTE(_WIN32_WINNT_WINBLUE), 0);
}

VERSIONHELPERAPI IsWindowsServer() 
{
    OSVERSIONINFOEXW osVersionInfo;
    DWORDLONG dwlConditionMask = 0;
    ZeroMemory(&osVersionInfo, sizeof(osVersionInfo));
    osVersionInfo.dwOSVersionInfoSize = sizeof(osVersionInfo);
    osVersionInfo.wProductType = VER_NT_WORKSTATION;
    VER_SET_CONDITION(dwlConditionMask, VER_PRODUCT_TYPE, VER_EQUAL);
    return !VerifyVersionInfoW(&osVersionInfo, VER_PRODUCT_TYPE, dwlConditionMask);
}
#endif // GWUTIL_VERSIONHELPERS_H
