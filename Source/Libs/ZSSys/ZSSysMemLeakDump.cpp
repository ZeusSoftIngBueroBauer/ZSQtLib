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

#ifdef _WINDOWS

#ifdef _DEBUG

/* Visual Studio Internal version numbering

The predefined macro _MSC_VER indicates the major and minor version numbers of the Visual C++ compiler.
The macro's value is an integer literal in which the last two digits indicate the minor version number
and the preceding digits indicate the major version number.

From Visual Studio 2017, _MSC_VER is incremented monotonically at every Visual C++ toolset update.
Thus, for example, the version of MSVC++ 14.1 that ships with Visual Studio 2017 version 15.3.0 sets
_MSC_VER to 1911. Microsoft recommends using the >= operator to test the value of _MSC_VER.

Here are values of _MSC_VER for various versions of the Visual C++ compiler:

MSC    1.0   _MSC_VER == 100
MSC    2.0   _MSC_VER == 200
MSC    3.0   _MSC_VER == 300
MSC    4.0   _MSC_VER == 400
MSC    5.0   _MSC_VER == 500
MSC    6.0   _MSC_VER == 600
MSC    7.0   _MSC_VER == 700
MSVC++ 1.0   _MSC_VER == 800
MSVC++ 2.0   _MSC_VER == 900
MSVC++ 4.0   _MSC_VER == 1000 (Developer Studio 4.0)
MSVC++ 4.2   _MSC_VER == 1020 (Developer Studio 4.2)
MSVC++ 5.0   _MSC_VER == 1100 (Visual Studio 97 version 5.0)
MSVC++ 6.0   _MSC_VER == 1200 (Visual Studio 6.0 version 6.0)
MSVC++ 7.0   _MSC_VER == 1300 (Visual Studio .NET 2002 version 7.0)
MSVC++ 7.1   _MSC_VER == 1310 (Visual Studio .NET 2003 version 7.1)
MSVC++ 8.0   _MSC_VER == 1400 (Visual Studio 2005 version 8.0)
MSVC++ 9.0   _MSC_VER == 1500 (Visual Studio 2008 version 9.0)
MSVC++ 10.0  _MSC_VER == 1600 (Visual Studio 2010 version 10.0)
MSVC++ 11.0  _MSC_VER == 1700 (Visual Studio 2012 version 11.0)
MSVC++ 12.0  _MSC_VER == 1800 (Visual Studio 2013 version 12.0)
MSVC++ 14.0  _MSC_VER == 1900 (Visual Studio 2015 version 14.0)
MSVC++ 14.1  _MSC_VER == 1910 (Visual Studio 2017 version 15.0)
MSVC++ 14.11 _MSC_VER == 1911 (Visual Studio 2017 version 15.3)
MSVC++ 14.12 _MSC_VER == 1912 (Visual Studio 2017 version 15.5)
MSVC++ 14.13 _MSC_VER == 1913 (Visual Studio 2017 version 15.6)
MSVC++ 14.14 _MSC_VER == 1914 (Visual Studio 2017 version 15.7)
MSVC++ 14.15 _MSC_VER == 1915 (Visual Studio 2017 version 15.8)
MSVC++ 14.16 _MSC_VER == 1916 (Visual Studio 2017 version 15.9)
MSVC++ 14.16 _MSC_VER == 1916 (Visual Studio 2017 version 15.9.6)
*/

#define __STR__(x) #x
#define __STRING__(x) __STR__(x)

#pragma message("MSC_VER: " __STRING__(_MSC_VER))

#if _MSC_VER != 1800
#define _CRTBLD
#endif

#define ZSSys_DbgNew_cpp
#include "ZSSys/ZSSysMemLeakDump.h"

#if _MSC_VER == 1310 || _MSC_VER == 1400 || _MSC_VER == 1500 || _MSC_VER == 1600 || _MSC_VER == 1700 \
 || _MSC_VER == 1800 || _MSC_VER == 1900 || _MSC_VER == 1915 || _MSC_VER == 1916 || _MSC_VER == 1928 || _MSC_VER == 1929
    #define nNoMansLandSize 4 //lint !e1923
    typedef struct _CrtMemBlockHeader
    {
        struct _CrtMemBlockHeader * pBlockHeaderNext;
        struct _CrtMemBlockHeader * pBlockHeaderPrev;
        char *                      szFileName;
        int                         nLine;
        #ifdef _WIN64
            /* These items are reversed on Win64 to eliminate gaps in the struct
             * and ensure that sizeof(struct)%16 == 0, so 16-byte alignment is
             * maintained in the debug heap.
            */
            int                         nBlockUse;
            size_t                      nDataSize;
        #else  /* _WIN64 */
            size_t                      nDataSize;
            int                         nBlockUse;
        #endif  /* _WIN64 */
        long                        lRequest;
        unsigned char               gap[nNoMansLandSize];
        /* followed by:
            *  unsigned char           data[nDataSize];
            *  unsigned char           anotherGap[nNoMansLandSize];
            */
    } _CrtMemBlockHeader;
#else
    #error Please verify that the block header structure remains the same for other compiler versions.
    #error See header file dgbint.h in directory VC/crt/src or debug_heap.cpp in Windows Kits/10/Source/....
#endif

//------------------------------------------------------------------------------
void __cdecl ZS::dumpClientHook( void* i_pvUserData, size_t i_nSizeInBytes )
//------------------------------------------------------------------------------
{
    //int iMSCVersion   = _MSC_VER;
    int iRepType      = _CrtReportBlockType(i_pvUserData);
    int iBlockType    = _BLOCK_TYPE(iRepType);
    int iBlockSubType = _BLOCK_SUBTYPE(iRepType);

    if( iBlockType == _CLIENT_BLOCK && iBlockSubType == _SMSYSDBGNEW_CLIENT_BLOCK_SUBTYPE )
    {
        _RPT4(_CRT_WARN,
              "LeakDumper found block at %p: type %d, subtype %d, size %d\n",
              i_pvUserData,
              iBlockType,
              iBlockSubType,
              i_nSizeInBytes );
    }
}

//------------------------------------------------------------------------------
void __cdecl ZS::dumpClientHook( void* i_pvUserData, void* /*i_pvContext*/ )
//------------------------------------------------------------------------------
{
    //int          iMSCVersion   = _MSC_VER;
    int          iRepType      = _CrtReportBlockType(i_pvUserData);
    int          iBlockType    = _BLOCK_TYPE(iRepType);
    int          iBlockSubType = _BLOCK_SUBTYPE(iRepType);
    unsigned int uSize;
    long         lRequestNumber;
    char*        szFileName;
    int          iLineNumber;

    if( iBlockType == _CLIENT_BLOCK && iBlockSubType == _SMSYSDBGNEW_CLIENT_BLOCK_SUBTYPE )
    {
        const _CrtMemBlockHeader* pHdr = static_cast<_CrtMemBlockHeader*>(i_pvUserData);
        pHdr--;

        uSize = static_cast<unsigned int>(pHdr->nDataSize);

        if( _CrtIsMemoryBlock(
            /* pvUserData     */ i_pvUserData,
            /* uSize          */ uSize,
            /* lRequestNumber */ &lRequestNumber,
            /* szFileName     */ &szFileName,
            /* iLineNumber    */ &iLineNumber ) )
        {
            try
            {
                _RPT3(
                    _CRT_WARN,
                    "%s (%d) : Detected memory leak at %p: type CLIENT, subtype SMSYSDBGNEW\n",
                    szFileName,
                    iLineNumber,
                    i_pvUserData );
            }
            catch( ... )
            {
                _RPT2(
                    _CRT_WARN,
                    "Detected memory leak at %p: type CLIENT, subtype SMSYSDBGNEW\n"
                    "  Allocated at line number %d from any file linked to a dll\n",
                    i_pvUserData,
                    iLineNumber );
            }
        }
    }
}

#endif // #ifdef _DEBUG

#endif // #ifdef _WINDOWS
