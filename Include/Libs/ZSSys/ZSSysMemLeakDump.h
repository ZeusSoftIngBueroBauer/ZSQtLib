/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license restrictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

/*==============================================================================

This header file must be included after all Qt header files have been included
as there a lot of macros and preprocessor symbols within the CRT and Qt header
files defining and redefining memory allocation functions leading to compile errors.

The following are example code lines on how to use this module together with the
CRT heap reporting functions:

    #include "ZSAppExample\Include\ZSExampleApp.h"
    #include "ZSSys\ZSSysMemLeakDump.h" // must be incluced after all Qt header files

    int main( int argc, char *argv[] )
    {
        int iAppResult = 0;

        ZS::DbExample::CDbApplication* pAppDbExample = new ZS::DbExample::CDbApplication(argc, argv);
        iAppResult = pAppDbExample->exec();

        delete pAppDbExample;
        pAppDbExample = nullptr;

        #ifdef _WINDOWS
        #ifdef _DEBUG
        _CrtDoForAllClientObjects(ZS::dumpClientHook, nullptr);
        #endif
        #endif

        return iAppResult;
    }

How to exclude memory blocks from being dumped:
-----------------------------------------------

Sometimes memory blocks allocated with the new operator should not be freed during
runtime of the system (e.g. internal heap objects or static class variables). But
those memory blocks would also be reported as detected memory leaks. The example
code lines below exclude the memory block from the memory leak dump:

    #ifdef ZSSys_MemLeakDump_h
    #pragma push_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")
    #pragma warning( disable : 4005 )
    #define _ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE 0
    #endif

    pi = new int[10];

    #ifdef ZSSys_MemLeakDump_h
    #pragma warning( default : 4005 )
    #pragma pop_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")
    #endif

==============================================================================*/

#ifndef ZSSys_MemLeakDump_h
#define ZSSys_MemLeakDump_h

namespace ZS
{
namespace System
{
} // namespace System
} // namespace ZS

#ifdef _WINDOWS

#ifdef _DEBUG

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#define _INTERNAL_IFSTRIP_
#include <crtdbg.h>
#include <new>

#define _ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE 0x0004

inline void* __cdecl operator new( size_t cb, int nBlockUse, const char* szFileName, int nLine, void* )
{
    void* res = operator new(cb, nBlockUse, szFileName, nLine);
    if( res == 0 ) throw std::bad_alloc();
    return res;
}
inline void* __cdecl operator new[]( size_t cb, int nBlockUse, const char* szFileName, int nLine, void* )
{
    void* res = operator new(cb, nBlockUse, szFileName, nLine);
    if( res == 0 ) throw std::bad_alloc();
    return res;
}

#if _MSC_VER >= 1200
inline void __cdecl operator delete( void* _P, int, const char*, int, void* )
{
    ::operator delete(_P);
}
inline void __cdecl operator delete[]( void* _P, int, const char *, int, void* )
{
    ::operator delete[](_P);
}
#endif  // _MSC_VER >= 1200

#define _DEBUG_NEW new(_CLIENT_BLOCK|(_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE<<16),__FILE__,__LINE__)
#ifndef ZSSys_DbgNew_cpp
#define new _DEBUG_NEW
#endif

namespace ZS
{
#ifdef ZSSYSDLL_EXPORTS
    void __declspec(dllexport) dumpClientHook( void* i_pvUserData, size_t i_nSizeInBytes );
    void __declspec(dllexport) dumpClientHook( void* i_pvUserData, void* i_pvContext );
#else
    void __declspec(dllimport) dumpClientHook( void* i_pvUserData, size_t i_nSizeInBytes );
    void __declspec(dllimport) dumpClientHook( void* i_pvUserData, void* i_pvContext );
#endif

} // namespace ZS

#endif // #ifdef _DEBUG

#endif // #ifdef _WINDOWS

#endif // #ifndef ZSSys_MemLeakDump_h
