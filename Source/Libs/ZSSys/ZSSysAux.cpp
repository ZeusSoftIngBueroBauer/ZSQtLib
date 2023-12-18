/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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
#include <tchar.h>
#ifdef INC_OLE1
#include <ole.h>
#else
#include <ole2.h>
#endif /* !INC_OLE1 */
#endif

#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysRequest.h"

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qcoreevent.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdir.h>
#include <QtCore/qline.h>
#include <QtCore/qmimedata.h>
#include <QtCore/qpoint.h>
#include <QtCore/qsettings.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qrect.h>
#include <QtCore/qthread.h>
#include <QtCore/quuid.h>
#include <QtCore/qurl.h>
#include <QtCore/qvariant.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
public auxiliary methods
*******************************************************************************/

//------------------------------------------------------------------------------
QString ZS::System::getCurrentThreadName()
//------------------------------------------------------------------------------
{
    QString str = QThread::currentThread()->objectName();

    if( str.isEmpty() )
    {
        str = "0x" + threadId2Str(QThread::currentThreadId());
    }
    return str;

} // getCurrentThreadName

//------------------------------------------------------------------------------
QString ZS::System::createGUID()
//------------------------------------------------------------------------------
{
    QString strGUID;

    QUuid uuid = QUuid::createUuid();

    strGUID = uuid.toString();

    return strGUID;

} // createGUID

//------------------------------------------------------------------------------
/*! @brief Converts the given string with unicode characters into a byte array.

    Unicode characters have two bytes. Please note that for UTF8 characters
    the second byte will be encoded as '\0' and the resulting byte array is no
    longer a human readable character string. To convert the byte array back to
    a unicode character string use 'byteArr2Str'.

    @param i_str [in] String to be converted.

    @return Byte array containing two bytes for each character of the input string.
*/
QByteArray ZS::System::str2ByteArr( const QString& i_str )
//------------------------------------------------------------------------------
{
    QByteArray byteArr;
    byteArr.resize(i_str.length()*sizeof(QChar));
    memcpy( byteArr.data(), i_str.data(), byteArr.size() );
    return byteArr;

} // str2ByteArr

//------------------------------------------------------------------------------
/*! @brief Converts the given byte array containg unicode characters into a
           unicode string.

    Unicode characters have two bytes. Please note that for UTF8 characters
    the second byte will be encoded as '\0'. This method reverts the conversion
    done with 'str2ByteArr'.

    @param i_byteArr [in] Byte array to be converted.

    @return String with unicode characters.
*/
QString ZS::System::byteArr2Str( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    QString str;
    str.resize(i_byteArr.size()/sizeof(QChar));
    memcpy( str.data(), i_byteArr.data(), i_byteArr.size() );
    return str;

} // byteArr2Str

//------------------------------------------------------------------------------
QList<QByteArray> ZS::System::strlst2ByteArrLst( const QStringList& i_strlst )
//------------------------------------------------------------------------------
{
    QList<QByteArray> arByteArr;

    #if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
    arByteArr.reserve(i_strlst.size());
    #endif

    for( int idx = 0; idx < i_strlst.size(); idx++ )
    {
        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        arByteArr.append(i_strlst[idx].toAscii());
        #else
        arByteArr.append(i_strlst[idx].toUtf8());
        #endif
    }
    return arByteArr;

} // strlst2ByteArrLst

//------------------------------------------------------------------------------
QStringList ZS::System::byteArrLst2StrLst( const QList<QByteArray>& i_arByteArr )
//------------------------------------------------------------------------------
{
    QStringList strlst;

    #if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
    strlst.reserve(i_arByteArr.size());
    #endif

    for( int idx = 0; idx < i_arByteArr.size(); idx++ )
    {
        strlst.append(i_arByteArr[idx]);
    }
    return strlst;

} // byteArrLst2StrLst

//------------------------------------------------------------------------------
QByteArray ZS::System::joinByteArrLst( const QList<QByteArray>& i_arByteArr, char i_chSeparator )
//------------------------------------------------------------------------------
{
    QByteArray byteArr;

    for( int idx = 0; idx < i_arByteArr.size(); idx++ )
    {
        byteArr += i_arByteArr[idx];

        if( idx < i_arByteArr.size()-1 )
        {
            byteArr += i_chSeparator;
        }
    }
    return byteArr;

} // joinByteArrLst

//------------------------------------------------------------------------------
QByteArray ZS::System::joinByteArrLst( const QList<QByteArray>& i_arByteArr, const QByteArray& i_strSeparator )
//------------------------------------------------------------------------------
{
    QByteArray byteArr;

    for( int idx = 0; idx < i_arByteArr.size(); idx++ )
    {
        byteArr += i_arByteArr[idx];

        if( idx < i_arByteArr.size()-1 )
        {
            byteArr += i_strSeparator;
        }
    }
    return byteArr;

} // joinByteArrLst

//------------------------------------------------------------------------------
QString ZS::System::join(
    const QString& i_strSeparator,
    const QString& i_str1,
    const QString& i_str2,
    const QString& i_str3,
    const QString& i_str4,
    const QString& i_str5 )
//------------------------------------------------------------------------------
{
    QString str = i_str1;

    if( !str.isEmpty() && !i_str2.isEmpty() )
    {
        str += i_strSeparator;
    }
    if( !i_str2.isEmpty() )
    {
        str += i_str2;
    }
    if( !str.isEmpty() && !i_str3.isEmpty() )
    {
        str += i_strSeparator;
    }
    if( !i_str3.isEmpty() )
    {
        str += i_str3;
    }
    if( !str.isEmpty() && !i_str4.isEmpty() )
    {
        str += i_strSeparator;
    }
    if( !i_str4.isEmpty() )
    {
        str += i_str4;
    }
    if( !str.isEmpty() && !i_str5.isEmpty() )
    {
        str += i_strSeparator;
    }
    if( !i_str5.isEmpty() )
    {
        str += i_str5;
    }
    return str;

} // join

//------------------------------------------------------------------------------
QByteArray ZS::System::join(
    const QList<QByteArray>& i_arByteArrs,
    const QByteArray&        i_byteArrSeparator )
//------------------------------------------------------------------------------
{
    QByteArray byteArr;

    for( int idx = 0; idx < i_arByteArrs.size(); idx++ )
    {
        if( idx > 0 )
        {
            byteArr += i_byteArrSeparator;
        }
        byteArr += i_arByteArrs[idx];
    }
    return byteArr;

} // join

//------------------------------------------------------------------------------
QList<QByteArray> ZS::System::split( const QByteArray& i_byteArr, const QByteArray& i_byteArrSeparator )
//------------------------------------------------------------------------------
{
    QList<QByteArray> arByteArrs;

    QString strByteArr   = i_byteArr;
    QString strSeparator = i_byteArrSeparator;

    QStringList strlst = strByteArr.split(strSeparator);

    arByteArrs = strlst2ByteArrLst(strlst);

    return arByteArrs;

} // split

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
//------------------------------------------------------------------------------
bool ZS::System::dir_removeRecursively( const QString& i_strDirPath )
//------------------------------------------------------------------------------
{
    bool bRemoved = true;

    QDir dir(i_strDirPath);

    if( dir.exists(i_strDirPath) )
    {
        QFileInfoList arFileInfos = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::System|QDir::Hidden|QDir::AllDirs|QDir::Files,QDir::DirsFirst);
        QFileInfo     fileInfo;
        int           idxFile;

        for( idxFile = 0; idxFile < arFileInfos.size(); idxFile++ )
        {
            fileInfo = arFileInfos[idxFile];

            if( fileInfo.isDir() )
            {
                bRemoved = dir_removeRecursively( fileInfo.absoluteFilePath() );
            }
            else
            {
                bRemoved = QFile::remove( fileInfo.absoluteFilePath() );
            }

            if( !bRemoved )
            {
                break;
            }
        } // for( idxFile = 0; idxFile < arFileInfos.size(); idxFile++ )

        if( bRemoved )
        {
            bRemoved = dir.rmdir(i_strDirPath);
        }
    } // if( dir.exists(i_strDirPath) )

    return bRemoved;

} // dir_removeRecursively
#endif

#ifdef _WINDOWS
////------------------------------------------------------------------------------
//BOOL ZS::System::SafeTerminateProcess( HANDLE i_hndProcess, UINT i_uExitCode )
////------------------------------------------------------------------------------
//{
//    /*
//    Safely terminate a process by creating a remote thread
//    in the process that calls ExitProcess
//    */
//
//    DWORD     dwTID;
//    DWORD     dwCode;
//    DWORD     dwErr = 0;
//    HANDLE    hndProcessDup = INVALID_HANDLE_VALUE;
//    HANDLE    hndProcess = i_hndProcess;
//    HANDLE    hndRT = nullptr;
//    HINSTANCE hinstKernel = GetModuleHandle(_T("Kernel32"));
//    BOOL      bSuccess = FALSE;
//
//    BOOL bResDupHandle = DuplicateHandle(
//        /* hndSourceProcessHandle */ GetCurrentProcess(),
//        /* hndSourceHandle        */ i_hndProcess,
//        /* hndTargetProcessHandle */ GetCurrentProcess(),
//        /* lpTargetHandle         */ &hndProcessDup,
//        /* dwDesiredAccess        */ PROCESS_ALL_ACCESS,
//        /* bInheritHandle         */ FALSE,
//        /* dwOption               */ 0 );
//
//    // Detect the special case where the process is already dead...
//    if( bResDupHandle )
//    {
//        hndProcess = hndProcessDup;
//    }
//
//    BOOL bResExitCodeProcess = GetExitCodeProcess( hndProcess, &dwCode );
//
//    if( bResExitCodeProcess && (dwCode == STILL_ACTIVE) )
//    {
//        FARPROC pfnExitProc;
//
//        pfnExitProc = GetProcAddress(hinstKernel, "ExitProcess");
//
//        hndRT = CreateRemoteThread(
//            /* hProcess           */ hndProcess,
//            /* lpThreadAttributes */ nullptr,
//            /* dwStackSize        */ 0,
//            /* lpStartAddress     */ (LPTHREAD_START_ROUTINE)pfnExitProc,
//            /* lpParameter        */ (PVOID)&i_uExitCode,
//            /* dwCreationFlags    */ 0,
//            /* lpThreadI          */ &dwTID );
//
//        if( hndRT == nullptr )
//        {
//            dwErr = GetLastError();
//        }
//    }
//    else
//    {
//        dwErr = ERROR_PROCESS_ABORTED;
//    }
//
//    if( hndRT )
//    {
//        // Must wait process to terminate to guarantee that it has exited...
//        WaitForSingleObject(hndProcess, INFINITE);
//        CloseHandle(hndRT);
//        bSuccess = TRUE;
//    }
//
//    if( bResDupHandle )
//    {
//        CloseHandle(hndProcessDup);
//    }
//
//    if( !bSuccess )
//    {
//        SetLastError(dwErr);
//    }
//
//    return bSuccess;
//
//} // SafeTerminateProcess
#endif // #ifdef _WINDOWS


/*******************************************************************************
controlling debug output for base modules
*******************************************************************************/

/*==============================================================================
namespace ZS
==============================================================================*/

static bool s_bDebugIsActive = false;

//------------------------------------------------------------------------------
void ZS::System::setDebugActive( bool i_bDebug )
//------------------------------------------------------------------------------
{
    s_bDebugIsActive = i_bDebug;
}

//------------------------------------------------------------------------------
bool ZS::System::isDebugActive()
//------------------------------------------------------------------------------
{
    return s_bDebugIsActive;
}


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
ThreadId
==============================================================================*/

//------------------------------------------------------------------------------
int ZS::System::threadId2Int( Qt::HANDLE i_hndThreadId )
//------------------------------------------------------------------------------
{
    int iThreadId = 0;

    /* from qnamespace.h:
    #if defined(Q_WS_MAC)
        typedef void* HANDLE;
    #elif defined(Q_WS_WIN)
        typedef void* HANDLE;
    #elif defined(Q_WS_X11)
        typedef unsigned long HANDLE;
    #elif defined(Q_WS_QWS)
        typedef void* HANDLE;
    #elif defined(Q_OS_SYMBIAN)
        typedef unsigned long int HANDLE; // equivalent to TUint32
    #endif */

    if( sizeof(i_hndThreadId) == sizeof(quint32) ) // 32 bit system
    {
        quint32 ulThreadId;
        quint32* pulThreadId = &ulThreadId;
        memcpy( pulThreadId, reinterpret_cast<void*>(&i_hndThreadId), sizeof(ulThreadId) );
    }
    else if( sizeof(i_hndThreadId) == sizeof(quint64) ) // 64 bit system
    {
        quint64 ulThreadId;
        quint64* pulThreadId = &ulThreadId;
        memcpy( pulThreadId, reinterpret_cast<void*>(&i_hndThreadId), sizeof(ulThreadId) );
        iThreadId = static_cast<int>(ulThreadId);
    }

    return iThreadId;

} // threadId2Int

//------------------------------------------------------------------------------
QString ZS::System::threadId2Str( Qt::HANDLE i_hndThreadId )
//------------------------------------------------------------------------------
{
    QString str;

    /* from qnamespace.h:
    #if defined(Q_WS_MAC)
        typedef void* HANDLE;
    #elif defined(Q_WS_WIN)
        typedef void* HANDLE;
    #elif defined(Q_WS_X11)
        typedef unsigned long HANDLE;
    #elif defined(Q_WS_QWS)
        typedef void* HANDLE;
    #elif defined(Q_OS_SYMBIAN)
        typedef unsigned long int HANDLE; // equivalent to TUint32
    #endif */

    if( sizeof(i_hndThreadId) == sizeof(quint32) ) // 32 bit system
    {
        quint32 ulThreadId;
        quint32* pulThreadId = &ulThreadId;
        memcpy( pulThreadId, reinterpret_cast<void*>(&i_hndThreadId), sizeof(ulThreadId) );
        str = QString::number(ulThreadId,16);
    }
    else if( sizeof(i_hndThreadId) == sizeof(quint64) ) // 64 bit system
    {
        quint64 ulThreadId;
        quint64* pulThreadId = &ulThreadId;
        memcpy( pulThreadId, reinterpret_cast<void*>(&i_hndThreadId), sizeof(ulThreadId) );
        str = QString::number(ulThreadId,16);
    }

    return str;

} // threadId2Str

/*==============================================================================
Pointer
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::pointer2Str( void* i_pPointer )
//------------------------------------------------------------------------------
{
    QString str;

    qint64 iPointerAddr = reinterpret_cast<qint64>(i_pPointer);

    str = "0x" + QString::number(iPointerAddr,16);

    return str;

} // pointer2Str

/*==============================================================================
Bool
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::bool2Str( bool i_bValue )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_bValue )
    {
        str = "true";
    }
    else
    {
        str = "false";
    }
    return str;

} // bool2Str

//------------------------------------------------------------------------------
bool ZS::System::str2Bool( const QString& i_str, bool* o_pbConverted )
//------------------------------------------------------------------------------
{
    QString str;
    bool    bValue = false;
    bool    bConverted = true;

    str = i_str.toLower();

    if( str == "false" )
    {
        bValue = false;
    }
    else if( str == "true" )
    {
        bValue = true;
    }
    else if( str == "0" )
    {
        bValue = false;
    }
    else if( str == "1" )
    {
        bValue = true;
    }
    else if( str == "off" )
    {
        bValue = false;
    }
    else if( str == "on" )
    {
        bValue = true;
    }
    else if( str == "no" )
    {
        bValue = false;
    }
    else if( str == "yes" )
    {
        bValue = true;
    }
    else
    {
        bConverted = false;
    }

    if( o_pbConverted != nullptr )
    {
        *o_pbConverted = bConverted;
    }

    return bValue;

} // str2Bool

//------------------------------------------------------------------------------
QByteArray ZS::System::bool2ByteArr( bool i_bValue )
//------------------------------------------------------------------------------
{
    QByteArray str;

    if( i_bValue )
    {
        str = "true";
    }
    else
    {
        str = "false";
    }
    return str;

} // bool2ByteArr

//------------------------------------------------------------------------------
bool ZS::System::byteArr2Bool( const QByteArray& i_str, bool* o_pbConverted )
//------------------------------------------------------------------------------
{
    QByteArray str;
    bool       bValue = false;
    bool       bConverted = true;

    str = i_str.toLower();

    if( str == "false" )
    {
        bValue = false;
    }
    else if( str == "true" )
    {
        bValue = true;
    }
    else if( str == "0" )
    {
        bValue = false;
    }
    else if( str == "1" )
    {
        bValue = true;
    }
    else if( str == "off" )
    {
        bValue = false;
    }
    else if( str == "on" )
    {
        bValue = true;
    }
    else if( str == "no" )
    {
        bValue = false;
    }
    else if( str == "yes" )
    {
        bValue = true;
    }
    else
    {
        bConverted = false;
    }

    if( o_pbConverted != nullptr )
    {
        *o_pbConverted = bConverted;
    }

    return bValue;

} // byteArr2Bool

/*==============================================================================
WINDOWS specific
==============================================================================*/

#ifdef _WINDOWS
////------------------------------------------------------------------------------
//QString ZS::System::hResult2Str( HRESULT i_hRes )
////------------------------------------------------------------------------------
//{
//    QString strError;
//    LPTSTR  lpMsgBuf = nullptr;
//    LPTSTR  lpDisplayBuf = nullptr;
//    SIZE_T  uDisplayBufSize;
//
//    FormatMessage(
//        /* dwFlags      */ FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
//        /* lpSource     */ nullptr,
//        /* dwMessageId  */ i_hRes,
//        /* dwLanguageId */ MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
//        /* lpBuffer     */ (LPTSTR)&lpMsgBuf,
//        /* nSize        */ 0,
//        /* pArguments   */ nullptr );
//
//    uDisplayBufSize = ( lstrlen((LPCTSTR)lpMsgBuf) + 60 ) * sizeof(TCHAR);
//    lpDisplayBuf = (LPTSTR)LocalAlloc( LMEM_ZEROINIT, uDisplayBufSize );
//
//    #pragma warning( disable : 4995 )
//
//    wsprintf( lpDisplayBuf, _TEXT("%s"), lpMsgBuf );
//
//    #pragma warning( default : 4995 )
//
//    #if QT_VERSION >= 0x040704
//        char* ascii = new char[wcslen(lpDisplayBuf)+1];
//        memset( ascii, 0x00, wcslen(lpDisplayBuf)+1 );
//        #pragma warning( disable : 4996 )
//        wcstombs( ascii, lpDisplayBuf, wcslen(lpDisplayBuf) );
//        #pragma warning( default : 4996 )
//        //std::string ststr = ascii;
//        strError = QString(ascii); // ::fromStdString(ststr);
//        // strError = QString::fromWCharArray(lpDisplayBuf);
//        delete [] ascii;
//    #else
//        strError = QString::fromUtf16(reinterpret_cast<const ushort*>(lpDisplayBuf));
//    #endif
//
//    LocalFree(lpMsgBuf);
//    LocalFree(lpDisplayBuf);
//
//    return strError;
//
//} // hResult2Str
#endif // #ifdef _WINDOWS

#ifdef _WINDOWS
////------------------------------------------------------------------------------
//QString ZS::System::winErrorCode2MessageStr( DWORD i_dwErrCode )
////------------------------------------------------------------------------------
//{
//    QString strError;
//    LPTSTR  lpMsgBuf = nullptr;
//    LPTSTR  lpDisplayBuf = nullptr;
//    SIZE_T  uDisplayBufSize;
//
//    FormatMessage(
//        /* dwFlags      */ FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
//        /* lpSource     */ nullptr,
//        /* dwMessageId  */ i_dwErrCode,
//        /* dwLanguageId */ MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
//        /* lpBuffer     */ (LPTSTR)&lpMsgBuf,
//        /* nSize        */ 0,
//        /* pArguments   */ nullptr );
//
//    uDisplayBufSize = ( lstrlen((LPCTSTR)lpMsgBuf) + 60 ) * sizeof(TCHAR);
//    lpDisplayBuf = (LPTSTR)LocalAlloc( LMEM_ZEROINIT, uDisplayBufSize );
//
//    #pragma warning( disable : 4995 )
//
//    wsprintf( lpDisplayBuf, _TEXT("%s"), lpMsgBuf );
//
//    #pragma warning( default : 4995 )
//
//    #if QT_VERSION >= 0x040704
//        size_t iDisplayBufLen = wcslen(lpDisplayBuf);
//        char* ascii = new char[iDisplayBufLen + 1];
//        memset(ascii, 0x00, iDisplayBufLen + 1);
//        #pragma warning( disable : 4996 )
//        wcstombs( ascii, lpDisplayBuf, iDisplayBufLen );
//        #pragma warning( default : 4996 )
//        //std::string ststr = ascii;
//        strError = QString(ascii); // ::fromStdString(ststr);
//        // strError = QString::fromWCharArray(lpDisplayBuf);
//        delete [] ascii;
//    #else
//        strError = QString::fromUtf16(reinterpret_cast<const ushort*>(lpDisplayBuf));
//    #endif
//
//    LocalFree(lpMsgBuf);
//    LocalFree(lpDisplayBuf);
//
//    return strError;
//
//} // winErrorCode2MessageStr
#endif // #ifdef _WINDOWS

//#ifdef _WINDOWS
////------------------------------------------------------------------------------
//QString ZS::System::lastWinErrorCode2MessageStr()
////------------------------------------------------------------------------------
//{
//    return winErrorCode2MessageStr( GetLastError() );
//}
//#endif // #ifdef _WINDOWS

#ifdef _WINDOWS
//------------------------------------------------------------------------------
QString ZS::System::oleVariant2Str( VARIANT* i_pVariant )
//------------------------------------------------------------------------------
{
    QString strVal;

    if( i_pVariant != nullptr )
    {
        //bool    bIsVector = i_pVariant->vt & VT_VECTOR;
        //bool    bIsArray  = i_pVariant->vt & VT_ARRAY;
        //bool    bByRef    = i_pVariant->vt & VT_BYREF;
        quint16 uType     = i_pVariant->vt & VT_TYPEMASK;

        switch( uType )
        {
            case VT_NULL:
            {
                strVal = "VT_NULL";
                break;
            }
            case VT_I2: // SHORT
            {
                strVal = QString::number(i_pVariant->iVal);
                break;
            }
            case VT_I4: // LONG
            {
                strVal = QString::number(i_pVariant->iVal);
                break;
            }
            case VT_R4: // FLOAT
            {
                strVal = "VT_R4";
                break;
            }
            case VT_R8: // DOUBLE
            {
                strVal = "VT_R8";
                break;
            }
            case VT_CY: // CY
            {
                strVal = "VT_CY";
                break;
            }
            case VT_DATE: // DATE
            {
                strVal = "VT_DATE";
                break;
            }
            case VT_BSTR: // BSTR
            {
                strVal = QString( (QChar*)i_pVariant->bstrVal, static_cast<int>(wcslen(i_pVariant->bstrVal)) );
                break;
            }
            case VT_DISPATCH: // IDispatch*
            {
                strVal = "VT_DISPATCH";
                break;
            }
            case VT_ERROR: // SCODE
            {
                strVal = "VT_ERROR";
                break;
            }
            case VT_BOOL: // VARIANT_BOOL
            {
                strVal = "VT_BOOL";
                break;
            }
            case VT_VARIANT:
            {
                strVal = "VT_VARIANT";
                break;
            }
            case VT_UNKNOWN: // IUnknown*
            {
                strVal = "VT_UNKNOWN";
                break;
            }
            case VT_DECIMAL:
            {
                strVal = "VT_DECIMAL";
                break;
            }
            case VT_I1: // CHAR
            {
                strVal = "VT_I1";
                break;
            }
            case VT_UI1: // BYTE
            {
                strVal = "VT_UI1";
                break;
            }
            case VT_UI2: // USHORT
            {
                strVal = "VT_UI2";
                break;
            }
            case VT_UI4: // ULONG
            {
                strVal = "VT_UI4";
                break;
            }
            case VT_I8: // LONGLONG
            {
                strVal = "VT_I8";
                break;
            }
            case VT_UI8: // ULONGLONG
            {
                strVal = "VT_UI8";
                break;
            }
            case VT_INT: // INT
            {
                strVal = "VT_INT";
                break;
            }
            case VT_UINT: // UINT
            {
                strVal = "VT_UINT";
                break;
            }
            case VT_VOID:
            {
                strVal = "VT_VOID";
                break;
            }
            case VT_HRESULT:
            {
                strVal = "VT_HRESULT";
                break;
            }
            case VT_PTR:
            {
                strVal = "VT_PTR";
                break;
            }
            case VT_SAFEARRAY:
            {
                strVal = "VT_SAFEARRAY";
                break;
            }
            case VT_CARRAY:
            {
                strVal = "VT_CARRAY";
                break;
            }
            case VT_USERDEFINED:
            {
                strVal = "VT_USERDEFINED";
                break;
            }
            case VT_LPSTR:
            {
                strVal = "VT_LPSTR";
                break;
            }
            case VT_LPWSTR:
            {
                strVal = "VT_LPWSTR";
                break;
            }
            case VT_RECORD:
            {
                strVal = "VT_RECORD";
                break;
            }
            case VT_INT_PTR:
            {
                strVal = "VT_INT_PTR";
                break;
            }
            case VT_UINT_PTR:
            {
                strVal = "VT_UINT_PTR";
                break;
            }
            case VT_FILETIME:
            {
                strVal = "VT_FILETIME";
                break;
            }
            case VT_BLOB:
            {
                strVal = "VT_BLOB";
                break;
            }
            case VT_STREAM:
            {
                strVal = "VT_STREAM";
                break;
            }
            case VT_STORAGE:
            {
                strVal = "VT_STORAGE";
                break;
            }
            case VT_STREAMED_OBJECT:
            {
                strVal = "VT_STREAMED_OBJECT";
                break;
            }
            case VT_STORED_OBJECT:
            {
                strVal = "VT_STORED_OBJECT";
                break;
            }
            case VT_BLOB_OBJECT:
            {
                strVal = "VT_BLOB_OBJECT";
                break;
            }
            case VT_CF:
            {
                strVal = "VT_CF";
                break;
            }
            case VT_CLSID:
            {
                strVal = "VT_CLSID";
                break;
            }
            case VT_VERSIONED_STREAM:
            {
                strVal = "VT_VERSIONED_STREAM";
                break;
            }
            default:
            {
                strVal = "VT_UNKNOWN";
                break;
            }
        }
    }
    return strVal;

} // oleVariant2Str
#endif

//------------------------------------------------------------------------------
bool ZS::System::isNumeric( const QVariant& i_var )
//------------------------------------------------------------------------------
{
    bool bIsNumeric = false;

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    switch (i_var.type()) {
        case QVariant::Int: // = 2,
        case QVariant::UInt: // = 3,
        case QVariant::LongLong: // = 4,
        case QVariant::ULongLong: // = 5,
        case QVariant::Double: // = 6,
        {
            bIsNumeric = true;
            break;
        }
        default:
        {
            break;
        }
    }
    #else
    switch (i_var.typeId()) {
        case QMetaType::Int: // = 2,
        case QMetaType::UInt: // = 3,
        case QMetaType::LongLong: // = 4,
        case QMetaType::ULongLong: // = 5,
        case QMetaType::Double: // = 6,
        {
            bIsNumeric = true;
            break;
        }
    default:
        {
            break;
        }
    }
    #endif
    return bIsNumeric;

} // isNumeric

/*==============================================================================
Enum Qt::CaseSensitivity
==============================================================================*/

static const SEnumEntry s_arEnumStrQtCaseSensitivity[] = {
    /* 0 */ SEnumEntry( Qt::CaseInsensitive, "CaseInsensitive", "I" ),
    /* 1 */ SEnumEntry( Qt::CaseSensitive,   "CaseSensitive",   "S" )
};

//------------------------------------------------------------------------------
QString ZS::System::qCaseSensitivity2Str( int i_caseSensitivity, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrQtCaseSensitivity, _ZSArrLen(s_arEnumStrQtCaseSensitivity), i_caseSensitivity, i_alias );
}

//------------------------------------------------------------------------------
Qt::CaseSensitivity ZS::System::str2QCaseSensitivity( const QString& i_str, EEnumEntryAliasStr i_alias, bool* o_pbConverted )
//------------------------------------------------------------------------------
{
    Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive;

    bool bConverted = false;

    int iVal = SEnumEntry::str2Enumerator( s_arEnumStrQtCaseSensitivity, _ZSArrLen(s_arEnumStrQtCaseSensitivity), i_str, i_alias, Qt::CaseInsensitive );

    if( iVal >= 0 && iVal < _ZSArrLen(s_arEnumStrQtCaseSensitivity) )
    {
        caseSensitivity = static_cast<Qt::CaseSensitivity>(iVal);
        bConverted = true;
    }

    if( o_pbConverted != nullptr )
    {
        *o_pbConverted = bConverted;
    }

    return caseSensitivity;

} // str2QCaseSensitivity


/*==============================================================================
Enum Qt::Orientation
==============================================================================*/

static const SEnumEntry s_arEnumStrQtQrientation[] = {
    /* 0 */ SEnumEntry( 0,              "0",          "-" ),
    /* 1 */ SEnumEntry( Qt::Horizontal, "Horizontal", "H" ),
    /* 2 */ SEnumEntry( Qt::Vertical,   "Vertical",   "V" )
};

//------------------------------------------------------------------------------
QString ZS::System::qOrientation2Str( int i_iVal, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrQtQrientation, _ZSArrLen(s_arEnumStrQtQrientation), i_iVal, i_alias);
}

//------------------------------------------------------------------------------
Qt::Orientation ZS::System::str2QOrientation( const QString& i_str, EEnumEntryAliasStr i_alias, bool* o_pbConverted )
//------------------------------------------------------------------------------
{
    Qt::Orientation orientation = Qt::Horizontal;

    bool bConverted = false;

    int iVal = SEnumEntry::str2Enumerator(s_arEnumStrQtQrientation, _ZSArrLen(s_arEnumStrQtQrientation), i_str, i_alias, Qt::CaseInsensitive);

    if( iVal >= 0 && iVal < _ZSArrLen(s_arEnumStrQtQrientation) )
    {
        orientation = static_cast<Qt::Orientation>(iVal);
        bConverted = true;
    }

    if( o_pbConverted != nullptr )
    {
        *o_pbConverted = bConverted;
    }

    return orientation;

} // str2QOrientation


/*==============================================================================
Enum Qt::SortOrder
==============================================================================*/

static const SEnumEntry s_arEnumStrQtSortOrder[] = {
    /* 0 */ SEnumEntry( Qt::AscendingOrder,  "Ascending",  "A" ),
    /* 1 */ SEnumEntry( Qt::DescendingOrder, "Descending", "D" )
};

//------------------------------------------------------------------------------
QString ZS::System::qSortOrder2Str( int i_iVal, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrQtSortOrder, _ZSArrLen(s_arEnumStrQtSortOrder), i_iVal, i_alias);
}

//------------------------------------------------------------------------------
Qt::SortOrder ZS::System::str2QSortOrder( const QString& i_str, EEnumEntryAliasStr i_alias, bool* o_pbConverted )
//------------------------------------------------------------------------------
{
    Qt::SortOrder sortOrder = Qt::AscendingOrder;

    bool bConverted = false;

    int iVal = SEnumEntry::str2Enumerator(s_arEnumStrQtSortOrder, _ZSArrLen(s_arEnumStrQtSortOrder), i_str, i_alias, Qt::CaseInsensitive);

    if( iVal >= 0 && iVal < _ZSArrLen(s_arEnumStrQtSortOrder) )
    {
        sortOrder = static_cast<Qt::SortOrder>(iVal);
        bConverted = true;
    }
    if( o_pbConverted != nullptr )
    {
        *o_pbConverted = bConverted;
    }
    return sortOrder;
}


/*==============================================================================
Enum QProcess::ExitStatus
==============================================================================*/

static const SEnumEntry s_arEnumStrQProcessExitStatus[] = {
    /* 0 */ SEnumEntry( QProcess::NormalExit, "NormalExit" ),
    /* 1 */ SEnumEntry( QProcess::CrashExit,  "CrashExit"  )
};

//------------------------------------------------------------------------------
QString ZS::System::qProcessExitStatus2Str( int i_iVal, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrQProcessExitStatus, _ZSArrLen(s_arEnumStrQProcessExitStatus), i_iVal, i_alias);
}

//------------------------------------------------------------------------------
QProcess::ExitStatus ZS::System::str2QProcessExitStatus( const QString& i_str, EEnumEntryAliasStr i_alias, bool* o_pbConverted )
//------------------------------------------------------------------------------
{
    QProcess::ExitStatus eVal = QProcess::NormalExit;

    bool bConverted = false;

    int iVal = SEnumEntry::str2Enumerator( s_arEnumStrQProcessExitStatus, _ZSArrLen(s_arEnumStrQProcessExitStatus), i_str, i_alias, Qt::CaseInsensitive );

    if( iVal >= 0 && iVal < _ZSArrLen(s_arEnumStrQProcessExitStatus) )
    {
        eVal = static_cast<QProcess::ExitStatus>(iVal);
        bConverted = true;
    }

    if( o_pbConverted != nullptr )
    {
        *o_pbConverted = bConverted;
    }

    return eVal;

} // str2QProcessExitStatus


/*==============================================================================
Enum QProcess::Error
==============================================================================*/

static const SEnumEntry s_arEnumStrQProcessError[] = {
    /* 0 */ SEnumEntry( QProcess::FailedToStart, "FailedToStart" ),
    /* 1 */ SEnumEntry( QProcess::Crashed,       "Crashed"       ),
    /* 2 */ SEnumEntry( QProcess::Timedout,      "Timedout"      ),
    /* 3 */ SEnumEntry( QProcess::ReadError,     "ReadError"     ),
    /* 4 */ SEnumEntry( QProcess::WriteError,    "WriteError"    ),
    /* 5 */ SEnumEntry( QProcess::UnknownError,  "UnknownError"  )
};

//------------------------------------------------------------------------------
QString ZS::System::qProcessError2Str( int i_iVal, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrQProcessError, _ZSArrLen(s_arEnumStrQProcessError), i_iVal, i_alias);
}

//------------------------------------------------------------------------------
QProcess::ProcessError ZS::System::str2QProcessError( const QString& i_str, EEnumEntryAliasStr i_alias, bool* o_pbConverted )
//------------------------------------------------------------------------------
{
    QProcess::ProcessError eVal = QProcess::UnknownError;

    bool bConverted = false;

    int iVal = SEnumEntry::str2Enumerator( s_arEnumStrQProcessError, _ZSArrLen(s_arEnumStrQProcessError), i_str, i_alias, Qt::CaseInsensitive );

    if( iVal >= 0 && iVal < _ZSArrLen(s_arEnumStrQProcessError) )
    {
        eVal = static_cast<QProcess::ProcessError>(iVal);
        bConverted = true;
    }

    if( o_pbConverted != nullptr )
    {
        *o_pbConverted = bConverted;
    }

    return eVal;

} // str2QProcessError


/*==============================================================================
Enum QThread::Priority
==============================================================================*/

static const SEnumEntry s_arEnumStrQThreadPriorities[] = {
    /*  0 */ SEnumEntry( QThread::IdlePriority,         "IdlePriority"         ),
    /*  0 */ SEnumEntry( QThread::LowestPriority,       "LowestPriority"       ),
    /*  0 */ SEnumEntry( QThread::LowPriority,          "LowPriority"          ),
    /*  0 */ SEnumEntry( QThread::NormalPriority,       "NormalPriority"       ),
    /*  0 */ SEnumEntry( QThread::HighPriority,         "HighPriority"         ),
    /*  0 */ SEnumEntry( QThread::HighestPriority,      "HighestPriority"      ),
    /*  0 */ SEnumEntry( QThread::TimeCriticalPriority, "TimeCriticalPriority" ),
    /*  0 */ SEnumEntry( QThread::InheritPriority,      "InheritPriority"      )
};

//------------------------------------------------------------------------------
QString ZS::System::qThreadPriority2Str( QThread::Priority i_priority, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrQThreadPriorities, _ZSArrLen(s_arEnumStrQThreadPriorities), i_priority, i_alias);
}

/*==============================================================================
Enum QXmlStreamReader::TokenType
==============================================================================*/

static const SEnumEntry s_arEnumStrQXmlStreamTokenTypes[] = {
    /*  0 */ SEnumEntry( QXmlStreamReader::NoToken,               "NoToken"               ),
    /*  1 */ SEnumEntry( QXmlStreamReader::Invalid,               "Invalid"               ),
    /*  2 */ SEnumEntry( QXmlStreamReader::StartDocument,         "StartDocument"         ),
    /*  3 */ SEnumEntry( QXmlStreamReader::EndDocument,           "EndDocument"           ),
    /*  4 */ SEnumEntry( QXmlStreamReader::StartElement,          "StartElement"          ),
    /*  5 */ SEnumEntry( QXmlStreamReader::EndElement,            "EndElement"            ),
    /*  6 */ SEnumEntry( QXmlStreamReader::Characters,            "Characters"            ),
    /*  7 */ SEnumEntry( QXmlStreamReader::Comment,               "Comment"               ),
    /*  8 */ SEnumEntry( QXmlStreamReader::DTD,                   "DTD"                   ),
    /*  9 */ SEnumEntry( QXmlStreamReader::EntityReference,       "EntityReference"       ),
    /* 10 */ SEnumEntry( QXmlStreamReader::ProcessingInstruction, "ProcessingInstruction" )
};

//------------------------------------------------------------------------------
QString ZS::System::qXmlStreamTokenType2Str( int i_iVal, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrQXmlStreamTokenTypes, _ZSArrLen(s_arEnumStrQXmlStreamTokenTypes), i_iVal, i_alias);
}


/*==============================================================================
Enum Qt::BrushStyle
==============================================================================*/

static const SEnumEntry s_arEnumStrQtBrushStyle[] = {
    /*  0 */ SEnumEntry( Qt::NoBrush, "NoBrush" ),
    /*  1 */ SEnumEntry( Qt::SolidPattern, "SolidPattern" ),
    /*  2 */ SEnumEntry( Qt::Dense1Pattern, "Dense1Pattern" ),
    /*  3 */ SEnumEntry( Qt::Dense2Pattern, "Dense2Pattern" ),
    /*  4 */ SEnumEntry( Qt::Dense3Pattern, "Dense3Pattern" ),
    /*  5 */ SEnumEntry( Qt::Dense4Pattern, "Dense4Pattern" ),
    /*  6 */ SEnumEntry( Qt::Dense5Pattern, "Dense5Pattern" ),
    /*  7 */ SEnumEntry( Qt::Dense6Pattern, "Dense6Pattern" ),
    /*  8 */ SEnumEntry( Qt::Dense7Pattern, "Dense7Pattern" ),
    /*  9 */ SEnumEntry( Qt::HorPattern, "HorPattern" ),
    /* 10 */ SEnumEntry( Qt::VerPattern, "VerPattern" ),
    /* 11 */ SEnumEntry( Qt::CrossPattern, "CrossPattern" ),
    /* 12 */ SEnumEntry( Qt::BDiagPattern, "BDiagPattern" ),
    /* 13 */ SEnumEntry( Qt::FDiagPattern, "FDiagPattern" ),
    /* 14 */ SEnumEntry( Qt::DiagCrossPattern, "DiagCrossPattern" ),
    /* 15 */ SEnumEntry( Qt::LinearGradientPattern, "LinearGradientPattern" ),
    /* 16 */ SEnumEntry( Qt::RadialGradientPattern, "RadialGradientPattern" ),
    /* 17 */ SEnumEntry( Qt::ConicalGradientPattern, "ConicalGradientPattern" )
};

//------------------------------------------------------------------------------
QString ZS::System::qBrushStyle2Str( const Qt::BrushStyle& i_brushStyle )
//------------------------------------------------------------------------------
{
    if( i_brushStyle >= 0 && i_brushStyle < _ZSArrLen(s_arEnumStrQtBrushStyle)) {
        return SEnumEntry::enumerator2Str(s_arEnumStrQtBrushStyle, _ZSArrLen(s_arEnumStrQtBrushStyle), i_brushStyle);
    }
    else if( i_brushStyle == Qt::TexturePattern) {
        return "TexturePattern";
    }
    else {
        return "? (" + QString::number(i_brushStyle) + ")";
    }
}


/*==============================================================================
Enum Qt::CheckState
==============================================================================*/

static const SEnumEntry s_arEnumStrQtCheckState[] = {
    /*  0 */ SEnumEntry( Qt::Unchecked,        "Unchecked"        ),
    /*  1 */ SEnumEntry( Qt::PartiallyChecked, "PartiallyChecked" ),
    /*  2 */ SEnumEntry( Qt::Checked,          "Checked"          )
};

//------------------------------------------------------------------------------
QString ZS::System::qCheckState2Str( int i_state )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrQtCheckState, _ZSArrLen(s_arEnumStrQtCheckState), i_state );
}


/*==============================================================================
Enum Qt::CursorShape
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumStrCursorShape[] =
//------------------------------------------------------------------------------
{
    /*  0 */ SEnumEntry( static_cast<int>(Qt::ArrowCursor), "Arrow" ),
    /*  1 */ SEnumEntry( static_cast<int>(Qt::UpArrowCursor), "UpArrow" ),
    /*  2 */ SEnumEntry( static_cast<int>(Qt::CrossCursor), "Cross" ),
    /*  3 */ SEnumEntry( static_cast<int>(Qt::WaitCursor), "Wait" ),
    /*  4 */ SEnumEntry( static_cast<int>(Qt::IBeamCursor), "IBeam" ),
    /*  5 */ SEnumEntry( static_cast<int>(Qt::SizeVerCursor), "SizeVer" ),
    /*  6 */ SEnumEntry( static_cast<int>(Qt::SizeHorCursor), "SizeHor" ),
    /*  7 */ SEnumEntry( static_cast<int>(Qt::SizeBDiagCursor), "SizeBDiag" ),
    /*  8 */ SEnumEntry( static_cast<int>(Qt::SizeFDiagCursor), "SizeFDiag" ),
    /*  9 */ SEnumEntry( static_cast<int>(Qt::SizeAllCursor), "SizeAll" ),
    /* 10 */ SEnumEntry( static_cast<int>(Qt::BlankCursor), "Blank" ),
    /* 11 */ SEnumEntry( static_cast<int>(Qt::SplitVCursor), "SplitV" ),
    /* 12 */ SEnumEntry( static_cast<int>(Qt::SplitHCursor), "SplitH" ),
    /* 13 */ SEnumEntry( static_cast<int>(Qt::PointingHandCursor), "PointingHand" ),
    /* 14 */ SEnumEntry( static_cast<int>(Qt::ForbiddenCursor), "Forbidden" ),
    /* 15 */ SEnumEntry( static_cast<int>(Qt::WhatsThisCursor), "WhatsThis" ),
    /* 16 */ SEnumEntry( static_cast<int>(Qt::BusyCursor), "Busy" ),
    /* 17 */ SEnumEntry( static_cast<int>(Qt::OpenHandCursor), "OpenHand" ),
    /* 18 */ SEnumEntry( static_cast<int>(Qt::ClosedHandCursor), "ClosedHand" ),
    #if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
    /* 19 */ SEnumEntry( static_cast<int>(Qt::DragCopyCursor), "DragCopy" ),
    /* 20 */ SEnumEntry( static_cast<int>(Qt::DragMoveCursor), "DragMove" ),
    /* 21 */ SEnumEntry( static_cast<int>(Qt::DragLinkCursor), "DragLink" ),
    #else
    /* 19 */ SEnumEntry( 19, "Undefined (19)" ),
    /* 20 */ SEnumEntry( 20, "Undefined (20)" ),
    /* 21 */ SEnumEntry( 21, "Undefined (21)" ),
    #endif
    /* 22 */ SEnumEntry( 22, "Undefined (22)" ),
    /* 23 */ SEnumEntry( 23, "Undefined (23)" ),
    /* 24 */ SEnumEntry( static_cast<int>(Qt::BitmapCursor), "Bitmap" ),
    /* 25 */ SEnumEntry( static_cast<int>(Qt::CustomCursor), "Custom" )
};

//------------------------------------------------------------------------------
QString ZS::System::qCursorShape2Str( int i_shape )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrCursorShape, _ZSArrLen(s_arEnumStrCursorShape), i_shape);
}


/*==============================================================================
Enum Qt::DropAction
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::qDropAction2Str( Qt::DropAction i_action )
//------------------------------------------------------------------------------
{
    QString strAction;
    if (i_action == Qt::IgnoreAction) {
        strAction = "Ignore";
    }
    else if (i_action == Qt::ActionMask) {
        strAction += "Mask";
    }
    else if (i_action == Qt::CopyAction) {
       strAction = "Copy";
    }
    else if (i_action == Qt::MoveAction) {
        strAction = "Move";
    }
    else if (i_action == Qt::LinkAction) {
        strAction += "Link";
    }
    else if (i_action == Qt::TargetMoveAction) {
        strAction += "TargetMove";
    }
    if (strAction.isEmpty()) {
        strAction = "? (" + QString::number(i_action, 16) + ")";
    }
    return strAction;
}


//------------------------------------------------------------------------------
QString ZS::System::qDropActions2Str( int i_actions )
//------------------------------------------------------------------------------
{
    QString strAction;
    if (i_actions == 0x00) {
        strAction = "Ignore";
    }
    else {
        if (i_actions & Qt::CopyAction) {
            if (!strAction.isEmpty()) strAction += "|";
            strAction += "Copy";
        }
        if (i_actions & Qt::MoveAction) {
            if (!strAction.isEmpty()) strAction += "|";
            strAction += "Move";
        }
        if (i_actions & Qt::LinkAction) {
            if (!strAction.isEmpty()) strAction += "|";
            strAction += "Link";
        }
    }
    if (strAction.isEmpty()) {
        strAction = "? (" + QString::number(i_actions, 16) + ")";
    }
    return strAction;
}

//------------------------------------------------------------------------------
QString ZS::System::qEvent2Str( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    return qEventType2Str(i_pEv->type());
}

/*==============================================================================
Enum QEvent:Type
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumStrQEventType[] =
//------------------------------------------------------------------------------
{
    /*     0 */ SEnumEntry( QEvent::None, "None", "invalid event" ),
    /*     1 */ SEnumEntry( QEvent::Timer, "Timer", "timer event" ),
    /*     2 */ SEnumEntry( QEvent::MouseButtonPress, "MouseButtonPress", "mouse button pressed" ),
    /*     3 */ SEnumEntry( QEvent::MouseButtonRelease, "MouseButtonRelease", "mouse button released" ),
    /*     4 */ SEnumEntry( QEvent::MouseButtonDblClick, "MouseButtonDblClick", "mouse button double click" ),
    /*     5 */ SEnumEntry( QEvent::MouseMove, "MouseMove", "mouse move" ),
    /*     6 */ SEnumEntry( QEvent::KeyPress, "KeyPress", "key pressed" ),
    /*     7 */ SEnumEntry( QEvent::KeyRelease, "KeyRelease", "key released" ),
    /*     8 */ SEnumEntry( QEvent::FocusIn, "FocusIn", "keyboard focus received" ),
    /*     9 */ SEnumEntry( QEvent::FocusOut, "FocusOut", "keyboard focus lost" ),
    /*    10 */ SEnumEntry( QEvent::Enter, "Enter", "mouse enters widget" ),
    /*    11 */ SEnumEntry( QEvent::Leave, "Leave", "mouse leaves widget" ),
    /*    12 */ SEnumEntry( QEvent::Paint, "Paint", "paint widget" ),
    /*    13 */ SEnumEntry( QEvent::Move, "Move", "move widget" ),
    /*    14 */ SEnumEntry( QEvent::Resize, "Resize", "resize widget" ),
    /*    15 */ SEnumEntry( QEvent::Create, "Create", "after widget creation" ),
    /*    16 */ SEnumEntry( QEvent::Destroy, "Destroy", "during widget destruction" ),
    /*    17 */ SEnumEntry( QEvent::Show, "Show", "widget is shown" ),
    /*    18 */ SEnumEntry( QEvent::Hide, "Hide", "widget is hidden" ),
    /*    19 */ SEnumEntry( QEvent::Close, "Close", "request to close widget" ),
    /*    20 */ SEnumEntry( QEvent::Quit, "Quit", "request to quit application" ),
    /*    21 */ SEnumEntry( QEvent::ParentChange, "ParentChange", "widget has been reparented" ),
    /*    22 */ SEnumEntry( QEvent::ThreadChange, "ThreadChange", "object has changed threads" ),
    /*    23 */ SEnumEntry( 23, "23 (Unused)", "23 (Unused)" ),
    /*    24 */ SEnumEntry( QEvent::WindowActivate, "WindowActivate", "window was activated" ),
    /*    25 */ SEnumEntry( QEvent::WindowDeactivate, "WindowDeactivate", "window was deactivated" ),
    /*    26 */ SEnumEntry( QEvent::ShowToParent, "ShowToParent", "widget is shown to parent" ),
    /*    27 */ SEnumEntry( QEvent::HideToParent, "HideToParent", "widget is hidden to parent" ),
    /*    28 */ SEnumEntry( 28, "28 (Unused)", "28 (Unused)" ),
    /*    29 */ SEnumEntry( 29, "29 (Unused)", "29 (Unused)" ),
    #ifdef QT3_SUPPORT
    /*    30 */ SEnumEntry( QEvent::Accel, "Accel", "accelerator event" ),
    #else
    /*    30 */ SEnumEntry( 30, "Accel", "accelerator event" ),
    #endif
    /*    31 */ SEnumEntry( QEvent::Wheel, "Wheel", "wheel event" ),
    #ifdef QT3_SUPPORT
    /*    32 */ SEnumEntry( QEvent::AccelAvailable, "AccelAvailable", "accelerator available event" ),
    #else
    /*    32 */ SEnumEntry( 32, "AccelAvailable", "accelerator available event" ),
    #endif
    /*    33 */ SEnumEntry( QEvent::WindowTitleChange, "WindowTitleChange", "window title changed" ),
    /*    34 */ SEnumEntry( QEvent::WindowIconChange, "WindowIconChange", "icon changed" ),
    /*    35 */ SEnumEntry( QEvent::ApplicationWindowIconChange, "ApplicationWindowIconChange", "application icon changed" ),
    /*    36 */ SEnumEntry( QEvent::ApplicationFontChange, "ApplicationFontChange", "application font changed" ),
    /*    37 */ SEnumEntry( QEvent::ApplicationLayoutDirectionChange, "ApplicationLayoutDirectionChange", "application layout direction changed" ),
    /*    38 */ SEnumEntry( QEvent::ApplicationPaletteChange, "ApplicationPaletteChange", "application palette changed" ),
    /*    39 */ SEnumEntry( QEvent::PaletteChange, "PaletteChange", "widget palette changed" ),
    /*    40 */ SEnumEntry( QEvent::Clipboard, "Clipboard", "internal clipboard event" ),
    /*    41 */ SEnumEntry( 41, "41 (Unused)", "41 (Unused)" ),
    /*    42 */ SEnumEntry( QEvent::Speech, "Speech", "reserved for speech input" ),
    /*    43 */ SEnumEntry( QEvent::MetaCall, "MetaCall", "meta call event" ),
    /*    44 */ SEnumEntry( 44, "44 (Unused)", "44 (Unused)" ),
    /*    45 */ SEnumEntry( 45, "45 (Unused)", "45 (Unused)" ),
    /*    46 */ SEnumEntry( 46, "46 (Unused)", "46 (Unused)" ),
    /*    47 */ SEnumEntry( 47, "47 (Unused)", "47 (Unused)" ),
    /*    48 */ SEnumEntry( 48, "48 (Unused)", "48 (Unused)" ),
    /*    49 */ SEnumEntry( 49, "49 (Unused)", "49 (Unused)" ),
    /*    50 */ SEnumEntry( QEvent::SockAct, "SockAct", "socket activation" ),
    /*    51 */ SEnumEntry( QEvent::ShortcutOverride, "ShortcutOverride", "shortcut override request" ),
    /*    52 */ SEnumEntry( QEvent::DeferredDelete, "DeferredDelete", "deferred delete event" ),
    /*    53 */ SEnumEntry( 53, "53 (Unused)", "53 (Unused)" ),
    /*    54 */ SEnumEntry( 54, "54 (Unused)", "54 (Unused)" ),
    /*    55 */ SEnumEntry( 55, "55 (Unused)", "55 (Unused)" ),
    /*    56 */ SEnumEntry( 56, "56 (Unused)", "56 (Unused)" ),
    /*    57 */ SEnumEntry( 57, "57 (Unused)", "57 (Unused)" ),
    /*    58 */ SEnumEntry( 58, "58 (Unused)", "58 (Unused)" ),
    /*    59 */ SEnumEntry( 59, "59 (Unused)", "59 (Unused)" ),
    /*    60 */ SEnumEntry( QEvent::DragEnter, "DragEnter", "drag moves into widget" ),
    /*    61 */ SEnumEntry( QEvent::DragMove, "DragMove", "drag moves in widget" ),
    /*    62 */ SEnumEntry( QEvent::DragLeave, "DragLeave", "drag leaves or is cancelled" ),
    /*    63 */ SEnumEntry( QEvent::Drop, "Drop", "actual drop" ),
    /*    64 */ SEnumEntry( QEvent::DragResponse, "DragResponse", "drag accepted/rejected" ),
    /*    65 */ SEnumEntry( 65, "65 (Unused)", "65 (Unused)" ),
    /*    66 */ SEnumEntry( 66, "66 (Unused)", "66 (Unused)" ),
    #ifdef QT3_SUPPORT
    /*    67 */ SEnumEntry( QEvent::ChildInsertedRequest, "ChildInsertedRequest", "send ChildInserted compatibility events to receiver" ),
    #else
    /*    67 */ SEnumEntry( 67, "ChildInsertedRequest", "send ChildInserted compatibility events to receiver" ),
    #endif
    /*    68 */ SEnumEntry( QEvent::ChildAdded, "ChildAdded", "new child widget" ),
    /*    69 */ SEnumEntry( QEvent::ChildPolished, "ChildPolished", "polished child widget" ),
    #ifdef QT3_SUPPORT
    /*    70 */ SEnumEntry( QEvent::ChildInserted, "ChildInserted", "compatibility child inserted" ),
    #else
    /*    70 */ SEnumEntry( 70, "ChildInserted", "compatibility child inserted" ),
    #endif
    /*    71 */ SEnumEntry( QEvent::ChildRemoved, "ChildRemoved", "deleted child widget" ),
    #ifdef QT3_SUPPORT
    /*    72 */ SEnumEntry( QEvent::LayoutHint, "LayoutHint", "compatibility relayout request" ),
    #else
    /*    72 */ SEnumEntry( 72, "LayoutHint", "compatibility relayout request" ),
    #endif
    /*    73 */ SEnumEntry( QEvent::ShowWindowRequest, "ShowWindowRequest", "widget's window should be mapped" ),
    /*    74 */ SEnumEntry( QEvent::PolishRequest, "PolishRequest", "widget should be polished" ),
    /*    75 */ SEnumEntry( QEvent::Polish, "Polish", "widget is polished" ),
    /*    76 */ SEnumEntry( QEvent::LayoutRequest, "LayoutRequest", "widget should be relayouted" ),
    /*    77 */ SEnumEntry( QEvent::UpdateRequest, "UpdateRequest", "widget should be repainted" ),
    /*    78 */ SEnumEntry( QEvent::UpdateLater, "UpdateLater", "request update() later" ),
    /*    79 */ SEnumEntry( QEvent::EmbeddingControl, "EmbeddingControl", "ActiveX embedding" ),
    /*    80 */ SEnumEntry( QEvent::ActivateControl, "ActivateControl", "ActiveX activation" ),
    /*    81 */ SEnumEntry( QEvent::DeactivateControl, "DeactivateControl", "ActiveX deactivation" ),
    /*    82 */ SEnumEntry( QEvent::ContextMenu, "ContextMenu", "context popup menu" ),
    /*    83 */ SEnumEntry( QEvent::InputMethod, "InputMethod", "input method" ),
    /*    84 */ SEnumEntry( 84, "84 (Unused)", "84 (Unused)" ),
    /*    85 */ SEnumEntry( 85, "85 (Unused)", "85 (Unused)" ),
    /*    86 */ SEnumEntry( 86, "86 (Unused)", "86 (Unused)" ),
    /*    87 */ SEnumEntry( QEvent::TabletMove, "TabletMove", "Wacom tablet event" ),
    /*    88 */ SEnumEntry( QEvent::LocaleChange, "LocaleChange", "the system locale changed" ),
    /*    89 */ SEnumEntry( QEvent::LanguageChange, "LanguageChange", "the application language changed" ),
    /*    90 */ SEnumEntry( QEvent::LayoutDirectionChange, "LayoutDirectionChange", "the layout direction changed" ),
    /*    91 */ SEnumEntry( QEvent::Style, "Style", "internal style event" ),
    /*    92 */ SEnumEntry( QEvent::TabletPress, "TabletPress", "tablet press" ),
    /*    93 */ SEnumEntry( QEvent::TabletRelease, "TabletRelease", "tablet release" ),
    /*    94 */ SEnumEntry( QEvent::OkRequest, "OkRequest", "CE (Ok) button pressed" ),
    /*    95 */ SEnumEntry( QEvent::HelpRequest, "HelpRequest", "CE (?)  button pressed" ),
    /*    96 */ SEnumEntry( QEvent::IconDrag, "IconDrag", "proxy icon dragged" ),
    /*    97 */ SEnumEntry( QEvent::FontChange, "FontChange", "font has changed" ),
    /*    98 */ SEnumEntry( QEvent::EnabledChange, "EnabledChange", "enabled state has changed" ),
    /*    99 */ SEnumEntry( QEvent::ActivationChange, "ActivationChange", "window activation has changed" ),
    /*   100 */ SEnumEntry( QEvent::StyleChange, "StyleChange", "style has changed" ),
    /*   101 */ SEnumEntry( QEvent::IconTextChange, "IconTextChange", "icon text has changed" ),
    /*   102 */ SEnumEntry( QEvent::ModifiedChange, "ModifiedChange", "modified state has changed" ),
    /*   103 */ SEnumEntry( QEvent::WindowBlocked, "WindowBlocked", "window is about to be blocked modally" ),
    /*   104 */ SEnumEntry( QEvent::WindowUnblocked, "WindowUnblocked", "windows modal blocking has ended" ),
    /*   105 */ SEnumEntry( QEvent::WindowStateChange, "WindowStateChange", "windows state has changed" ),
    /*   106 */ SEnumEntry( 106, "106 (Unused)", "106 (Unused)" ),
    /*   107 */ SEnumEntry( 107, "107 (Unused)", "107 (Unused)" ),
    /*   108 */ SEnumEntry( 108, "108 (Unused)", "108 (Unused)" ),
    /*   109 */ SEnumEntry( QEvent::MouseTrackingChange, "MouseTrackingChange", "mouse tracking state has changed" ),
    /*   110 */ SEnumEntry( QEvent::ToolTip, "ToolTip", "ToolTip" ),
    /*   111 */ SEnumEntry( QEvent::WhatsThis, "WhatsThis", "WhatsThis" ),
    /*   112 */ SEnumEntry( QEvent::StatusTip, "StatusTip", "StatusTip" ),
    /*   113 */ SEnumEntry( QEvent::ActionChanged, "ActionChanged", "ActionChanged" ),
    /*   114 */ SEnumEntry( QEvent::ActionAdded, "ActionAdded", "ActionAdded" ),
    /*   115 */ SEnumEntry( QEvent::ActionRemoved, "ActionRemoved", "ActionRemoved" ),
    /*   116 */ SEnumEntry( QEvent::FileOpen, "FileOpen", "file open request" ),
    /*   117 */ SEnumEntry( QEvent::Shortcut, "Shortcut", "shortcut triggered" ),
    /*   118 */ SEnumEntry( QEvent::WhatsThisClicked, "WhatsThisClicked", "WhatsThisClicked" ),
    /*   119 */ SEnumEntry( 119, "119 (Unused)", "119 (Unused)" ),
    /*   120 */ SEnumEntry( QEvent::ToolBarChange, "ToolBarChange", "toolbar visibility toggled" ),
    /*   121 */ SEnumEntry( QEvent::ApplicationActivate, "ApplicationActivate", "application has been changed to active" ),
    /*   122 */ SEnumEntry( QEvent::ApplicationDeactivate, "ApplicationDeactivate", "application has been changed to inactive" ),
    /*   123 */ SEnumEntry( QEvent::QueryWhatsThis, "QueryWhatsThis", "query what's this widget help" ),
    /*   124 */ SEnumEntry( QEvent::EnterWhatsThisMode, "EnterWhatsThisMode", "EnterWhatsThisMode" ),
    /*   125 */ SEnumEntry( QEvent::LeaveWhatsThisMode, "LeaveWhatsThisMode", "LeaveWhatsThisMode" ),
    /*   126 */ SEnumEntry( QEvent::ZOrderChange, "ZOrderChange", "child widget has had its z-order changed" ),
    /*   127 */ SEnumEntry( QEvent::HoverEnter, "HoverEnter", "mouse cursor enters a hover widget" ),
    /*   128 */ SEnumEntry( QEvent::HoverLeave, "HoverLeave", "mouse cursor leaves a hover widget" ),
    /*   129 */ SEnumEntry( QEvent::HoverMove, "HoverMove", "mouse cursor move inside a hover widget" ),
    /*   130 */ SEnumEntry( 130, "130 (Unused)", "130 (Unused)" ),
    /*   131 */ SEnumEntry( QEvent::ParentAboutToChange, "ParentAboutToChange", "sent just before the parent change is done" ),
    /*   132 */ SEnumEntry( QEvent::WinEventAct, "WinEventAct", "win event activation" ),
    /*   133 */ SEnumEntry( 133, "133 (Unused)", "133 (Unused)" ),
    /*   134 */ SEnumEntry( 134, "134 (Unused)", "134 (Unused)" ),
    /*   135 */ SEnumEntry( 135, "135 (Unused)", "135 (Unused)" ),
    /*   136 */ SEnumEntry( 136, "136 (Unused)", "136 (Unused)" ),
    /*   137 */ SEnumEntry( 137, "137 (Unused)", "137 (Unused)" ),
    /*   138 */ SEnumEntry( 138, "138 (Unused)", "138 (Unused)" ),
    /*   139 */ SEnumEntry( 139, "139 (Unused)", "139 (Unused)" ),
    /*   140 */ SEnumEntry( 140, "140 (Unused)", "140 (Unused)" ),
    /*   141 */ SEnumEntry( 141, "141 (Unused)", "141 (Unused)" ),
    /*   142 */ SEnumEntry( 142, "142 (Unused)", "142 (Unused)" ),
    /*   143 */ SEnumEntry( 143, "143 (Unused)", "143 (Unused)" ),
    /*   144 */ SEnumEntry( 144, "144 (Unused)", "144 (Unused)" ),
    /*   145 */ SEnumEntry( 145, "145 (Unused)", "145 (Unused)" ),
    /*   146 */ SEnumEntry( 146, "146 (Unused)", "146 (Unused)" ),
    /*   147 */ SEnumEntry( 147, "147 (Unused)", "147 (Unused)" ),
    /*   148 */ SEnumEntry( 148, "148 (Unused)", "148 (Unused)" ),
    /*   149 */ SEnumEntry( 149, "149 (Unused)", "149 (Unused)" ),
    #ifdef QT_KEYPAD_NAVIGATION
    /*   150 */ SEnumEntry( QEvent::EnterEditFocus, "EnterEditFocus", "enter edit mode in keypad navigation (Defined only with QT_KEYPAD_NAVIGATION)" ),
    /*   151 */ SEnumEntry( QEvent::LeaveEditFocus, "LeaveEditFocus", "leave edit mode in keypad navigation (Defined only with QT_KEYPAD_NAVIGATION)" ),
    #else
    /*   150 */ SEnumEntry( 150, "EnterEditFocus", "enter edit mode in keypad navigation (Defined only with QT_KEYPAD_NAVIGATION)" ),
    /*   151 */ SEnumEntry( 151, "LeaveEditFocus", "leave edit mode in keypad navigation (Defined only with QT_KEYPAD_NAVIGATION)" ),
    #endif
    /*   152 */ SEnumEntry( QEvent::AcceptDropsChange, "AcceptDropsChange", "AcceptDropsChange" ),
    /*   153 */ SEnumEntry( 153, "153 (Unused)", "153 (Unused)" ),
    /*   154 */ SEnumEntry( QEvent::ZeroTimerEvent, "ZeroTimerEvent", "Used for Windows Zero timer events" ),
    /*   155 */ SEnumEntry( QEvent::GraphicsSceneMouseMove, "GraphicsSceneMouseMove", "GraphicsView" ),
    /*   156 */ SEnumEntry( QEvent::GraphicsSceneMousePress, "GraphicsSceneMousePress", "GraphicsSceneMousePress" ),
    /*   157 */ SEnumEntry( QEvent::GraphicsSceneMouseRelease, "GraphicsSceneMouseRelease", "GraphicsSceneMouseRelease" ),
    /*   158 */ SEnumEntry( QEvent::GraphicsSceneMouseDoubleClick, "GraphicsSceneMouseDoubleClick", "GraphicsSceneMouseDoubleClick" ),
    /*   159 */ SEnumEntry( QEvent::GraphicsSceneContextMenu, "GraphicsSceneContextMenu", "GraphicsSceneContextMenu" ),
    /*   160 */ SEnumEntry( QEvent::GraphicsSceneHoverEnter, "GraphicsSceneHoverEnter", "GraphicsSceneHoverEnter" ),
    /*   161 */ SEnumEntry( QEvent::GraphicsSceneHoverMove, "GraphicsSceneHoverMove", "GraphicsSceneHoverMove" ),
    /*   162 */ SEnumEntry( QEvent::GraphicsSceneHoverLeave, "GraphicsSceneHoverLeave", "GraphicsSceneHoverLeave" ),
    /*   163 */ SEnumEntry( QEvent::GraphicsSceneHelp, "GraphicsSceneHelp", "GraphicsSceneHelp" ),
    /*   164 */ SEnumEntry( QEvent::GraphicsSceneDragEnter, "GraphicsSceneDragEnter", "GraphicsSceneDragEnter" ),
    /*   165 */ SEnumEntry( QEvent::GraphicsSceneDragMove, "GraphicsSceneDragMove", "GraphicsSceneDragMove" ),
    /*   166 */ SEnumEntry( QEvent::GraphicsSceneDragLeave, "GraphicsSceneDragLeave", "GraphicsSceneDragLeave" ),
    /*   167 */ SEnumEntry( QEvent::GraphicsSceneDrop, "GraphicsSceneDrop", "GraphicsSceneDrop" ),
    /*   168 */ SEnumEntry( QEvent::GraphicsSceneWheel, "GraphicsSceneWheel", "GraphicsSceneWheel" ),
    /*   169 */ SEnumEntry( QEvent::KeyboardLayoutChange, "KeyboardLayoutChange", "keyboard layout changed" ),
    /*   170 */ SEnumEntry( QEvent::DynamicPropertyChange, "DynamicPropertyChange", "A dynamic property was changed through setProperty/property" ),
    /*   171 */ SEnumEntry( QEvent::TabletEnterProximity, "TabletEnterProximity", "TabletEnterProximity" ),
    /*   172 */ SEnumEntry( QEvent::TabletLeaveProximity, "TabletLeaveProximity", "TabletLeaveProximity" ),
    /*   173 */ SEnumEntry( QEvent::NonClientAreaMouseMove, "NonClientAreaMouseMove", "NonClientAreaMouseMove" ),
    /*   174 */ SEnumEntry( QEvent::NonClientAreaMouseButtonPress, "NonClientAreaMouseButtonPress", "NonClientAreaMouseButtonPress" ),
    /*   175 */ SEnumEntry( QEvent::NonClientAreaMouseButtonRelease, "NonClientAreaMouseButtonRelease", "NonClientAreaMouseButtonRelease" ),
    /*   176 */ SEnumEntry( QEvent::NonClientAreaMouseButtonDblClick, "NonClientAreaMouseButtonDblClick", "NonClientAreaMouseButtonDblClick" ),
    /*   177 */ SEnumEntry( QEvent::MacSizeChange, "MacSizeChange", "when the Qt::WA_Mac{Normal,Small,Mini}Size changes" ),
    /*   178 */ SEnumEntry( QEvent::ContentsRectChange, "ContentsRectChange", "sent by QWidget::setContentsMargins (internal)" ),
    /*   179 */ SEnumEntry( QEvent::MacGLWindowChange, "MacGLWindowChange", "Internal! the window of the GLWidget has changed" ),
    /*   180 */ SEnumEntry( QEvent::FutureCallOut, "FutureCallOut", "FutureCallOut" ),
    /*   181 */ SEnumEntry( QEvent::GraphicsSceneResize, "GraphicsSceneResize", "GraphicsSceneResize" ),
    /*   182 */ SEnumEntry( QEvent::GraphicsSceneMove, "GraphicsSceneMove", "GraphicsSceneMove" ),
    /*   183 */ SEnumEntry( QEvent::CursorChange, "CursorChange", "CursorChange" ),
    /*   184 */ SEnumEntry( QEvent::ToolTipChange, "ToolTipChange", "ToolTipChange" ),
    /*   185 */ SEnumEntry( QEvent::NetworkReplyUpdated, "NetworkReplyUpdated", "Internal for QNetworkReply" ),
    /*   186 */ SEnumEntry( QEvent::GrabMouse, "GrabMouse", "GrabMouse" ),
    /*   187 */ SEnumEntry( QEvent::UngrabMouse, "UngrabMouse", "UngrabMouse" ),
    /*   188 */ SEnumEntry( QEvent::GrabKeyboard, "GrabKeyboard", "GrabKeyboard" ),
    /*   189 */ SEnumEntry( QEvent::UngrabKeyboard, "UngrabKeyboard", "UngrabKeyboard" ),
    /*   190 */ SEnumEntry( 190, "190 (Unused)", "190 (Unused)" ),
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    /*   191 */ SEnumEntry( QEvent::MacGLClearDrawable, "MacGLClearDrawable", "Internal Cocoa, the window has changed, so we must clear" ),
    #else
    /*   191 */ SEnumEntry( 191, "191 (Unused)", "191 (Unused)" ),
    #endif
    /*   192 */ SEnumEntry( QEvent::StateMachineSignal, "StateMachineSignal", "StateMachineSignal" ),
    /*   193 */ SEnumEntry( QEvent::StateMachineWrapped, "StateMachineWrapped", "StateMachineWrapped" ),
    /*   194 */ SEnumEntry( QEvent::TouchBegin, "TouchBegin", "TouchBegin" ),
    /*   195 */ SEnumEntry( QEvent::TouchUpdate, "TouchUpdate", "TouchUpdate" ),
    /*   196 */ SEnumEntry( QEvent::TouchEnd, "TouchEnd", "TouchEnd" ),
    #ifndef QT_NO_GESTURES
    /*   197 */ SEnumEntry( QEvent::NativeGesture, "NativeGesture", "Internal for platform gesture support" ),
    /*   198 */ SEnumEntry( QEvent::Gesture, "Gesture", "Gesture" ),
    #else
    /*   197 */ SEnumEntry( 197, "NativeGesture", "Internal for platform gesture support" ),
    /*   198 */ SEnumEntry( 198, "Gesture", "Gesture" ),
    #endif
    /*   199 */ SEnumEntry( QEvent::RequestSoftwareInputPanel, "RequestSoftwareInputPanel", "RequestSoftwareInputPanel" ),
    /*   200 */ SEnumEntry( QEvent::CloseSoftwareInputPanel, "CloseSoftwareInputPanel", "CloseSoftwareInputPanel" ),
    /*   201 */ SEnumEntry( 201, "201 (Unused)", "201 (Unused)" ),
    #ifndef QT_NO_GESTURES
    /*   202 */ SEnumEntry( QEvent::GestureOverride, "GestureOverride", "GestureOverride" ),
    #else
    /*   202 */ SEnumEntry( 202, "GestureOverride", "GestureOverride" ),
    #endif
    /*   203 */ SEnumEntry( QEvent::WinIdChange, "WinIdChange", "WinIdChange" ),
    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    /*   204 */ SEnumEntry( 204, "204 (Unused)", "204 (Unused)" ),
    /*   205 */ SEnumEntry( 205, "205 (Unused)", "205 (Unused)" ),
    /*   206 */ SEnumEntry( 206, "206 (Unused)", "206 (Unused)" ),
    /*   207 */ SEnumEntry( 207, "207 (Unused)", "207 (Unused)" ),
    /*   208 */ SEnumEntry( 208, "208 (Unused)", "208 (Unused)" ),
    /*   209 */ SEnumEntry( 209, "209 (Unused)", "209 (Unused)" ),
    /*   210 */ SEnumEntry( 210, "210 (Unused)", "210 (Unused)" ),
    /*   211 */ SEnumEntry( 211, "211 (Unused)", "211 (Unused)" ),
    /*   212 */ SEnumEntry( 212, "212 (Unused)", "212 (Unused)" ),
    /*   212 */ SEnumEntry( 213, "213 (Unused)", "213 (Unused)" ),
    /*   212 */ SEnumEntry( 214, "214 (Unused)", "214 (Unused)" ),
    /*   212 */ SEnumEntry( 215, "215 (Unused)", "215 (Unused)" ),
    /*   212 */ SEnumEntry( 216, "216 (Unused)", "216 (Unused)" ),
    /*   212 */ SEnumEntry( 217, "217 (Unused)", "217 (Unused)" ),
    /*   212 */ SEnumEntry( 218, "218 (Unused)", "218 (Unused)" ),
    /*   212 */ SEnumEntry( 219, "219 (Unused)", "219 (Unused)" ),
    #else
    /*   204 */ SEnumEntry( 204, "ScrollPrepare", "ScrollPrepare" ),
    /*   205 */ SEnumEntry( 205, "Scroll", "Scroll" ),
    /*   206 */ SEnumEntry( 206, "Expose", "Expose" ),
    /*   207 */ SEnumEntry( 207, "InputMethodQuery", "InputMethodQuery" ),
    /*   208 */ SEnumEntry( 208, "OrientationChange", "OrientationChange" ),
    /*   209 */ SEnumEntry( 209, "TouchCancel", "TouchCancel" ),
    /*   210 */ SEnumEntry( 210, "ThemeChange", "ThemeChange" ),
    /*   211 */ SEnumEntry( 211, "SockClose", "SockClose" ),
    /*   212 */ SEnumEntry( 212, "PlatformPanel", "PlatformPanel" ),
    /*   212 */ SEnumEntry( 213, "StyleAnimationUpdate", "StyleAnimationUpdate" ),
    /*   212 */ SEnumEntry( 214, "ApplicationStateChange", "ApplicationStateChange" ),
    /*   212 */ SEnumEntry( 215, "WindowChangeInternal", "WindowChangeInternal" ),
    /*   212 */ SEnumEntry( 216, "ScreenChangeInternal", "ScreenChangeInternal" ),
    /*   212 */ SEnumEntry( 217, "PlatformSurface", "PlatformSurface" ),
    /*   212 */ SEnumEntry( 218, "Pointer", "Pointer" ),
    /*   212 */ SEnumEntry( 219, "TabletTrackingChange", "TabletTrackingChange" ),
    #endif
    /*   213 */ SEnumEntry( 220, "220 (Unused)", "220 (Unused)" )
};

//------------------------------------------------------------------------------
QString ZS::System::qEventType2Str( int i_type, bool i_bShort )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_type == 512 )
    {
        str = "ReservedQtJambisMetaCall";
    }
    else if( i_type == 513 )
    {
        str = "ReservedQtJambisDeleteOnMainThread";
    }
    else if( i_type == 1000 )
    {
        if( i_bShort ) str = "User";
        else str = "First user event id";
    }
    else if( i_type == 65535 )
    {
        if( i_bShort ) str = "MaxUser";
        else str = "Max user event id";
    }
    else if( i_type < _ZSArrLen(s_arEnumStrQEventType) )
    {
        if( i_bShort )
        {
            str = SEnumEntry::enumerator2Str(s_arEnumStrQEventType, _ZSArrLen(s_arEnumStrQEventType), i_type);
        }
        else
        {
            str = SEnumEntry::enumerator2Str(s_arEnumStrQEventType, _ZSArrLen(s_arEnumStrQEventType), i_type, 1);
        }
    }
    else
    {
        str = "? (=" + QString::number(i_type) + ")";
    }
    return str;

} // qEventType2Str

/*==============================================================================
Enum Qt::FocusReasong
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumStrFocusReasons[] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( static_cast<int>(Qt::MouseFocusReason), "MouseFocusReason" ),
    /* 1 */ SEnumEntry( static_cast<int>(Qt::TabFocusReason), "TabFocusReason" ),
    /* 2 */ SEnumEntry( static_cast<int>(Qt::BacktabFocusReason), "BacktabFocusReason" ),
    /* 3 */ SEnumEntry( static_cast<int>(Qt::ActiveWindowFocusReason), "ActiveWindowFocusReason" ),
    /* 4 */ SEnumEntry( static_cast<int>(Qt::PopupFocusReason), "PopupFocusReason" ),
    /* 5 */ SEnumEntry( static_cast<int>(Qt::ShortcutFocusReason), "ShortcutFocusReason" ),
    /* 6 */ SEnumEntry( static_cast<int>(Qt::MenuBarFocusReason), "MenuBarFocusReason" ),
    /* 7 */ SEnumEntry( static_cast<int>(Qt::OtherFocusReason), "OtherFocusReason" ),
    /* 8 */ SEnumEntry( static_cast<int>(Qt::NoFocusReason), "NoFocusReason" )
};

//------------------------------------------------------------------------------
QString ZS::System::qFocusReason2Str( int i_iVal )
//------------------------------------------------------------------------------
{
    if( i_iVal >= 0 && i_iVal < _ZSArrLen(s_arEnumStrFocusReasons) )
    {
        return SEnumEntry::enumerator2Str( s_arEnumStrFocusReasons, _ZSArrLen(s_arEnumStrFocusReasons), i_iVal );
    }
    return "? (" + QString::number(i_iVal) + ")";
}


/*==============================================================================
Enum Qt::ItemDataRole
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumStrQItemDataRole[] =
//------------------------------------------------------------------------------
{
    /*  0 */ SEnumEntry( Qt::DisplayRole,               "DisplayRole" ),
    /*  1 */ SEnumEntry( Qt::DecorationRole,            "DecorationRole" ),
    /*  2 */ SEnumEntry( Qt::EditRole,                  "EditRole" ),
    /*  3 */ SEnumEntry( Qt::ToolTipRole,               "ToolTipRole" ),
    /*  4 */ SEnumEntry( Qt::StatusTipRole,             "StatusTipRole" ),
    /*  5 */ SEnumEntry( Qt::WhatsThisRole,             "WhatsThisRole" ),
    /*  6 */ SEnumEntry( Qt::FontRole,                  "FontRole" ),
    /*  7 */ SEnumEntry( Qt::TextAlignmentRole,         "TextAlignmentRole" ),
    /*  8 */ SEnumEntry( Qt::BackgroundRole,            "BackgroundRole" ),
    /*  9 */ SEnumEntry( Qt::ForegroundRole,            "ForegroundRole" ),
    /* 10 */ SEnumEntry( Qt::CheckStateRole,            "CheckStateRole" ),
    /* 11 */ SEnumEntry( Qt::AccessibleTextRole,        "AccessibleTextRole" ),
    /* 12 */ SEnumEntry( Qt::AccessibleDescriptionRole, "AccessibleDescriptionRole" ),
    /* 13 */ SEnumEntry( Qt::SizeHintRole,              "SizeHintRole" ),
    #if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
    /* 14 */ SEnumEntry( Qt::InitialSortOrderRole,      "InitialSortOrderRole" ),
    #else
    /* 14 */ SEnumEntry( 14,                            "? (14)" ),
    #endif
    /* 15 */ SEnumEntry( 15,                            "? (15)" ),
    /* 16 */ SEnumEntry( 16,                            "? (16)" ),
    /* 17 */ SEnumEntry( 17,                            "? (17)" ),
    /* 18 */ SEnumEntry( 18,                            "? (18)" ),
    /* 19 */ SEnumEntry( 19,                            "? (19)" ),
    /* 20 */ SEnumEntry( 20,                            "? (20)" ),
    /* 21 */ SEnumEntry( 21,                            "? (21)" ),
    /* 22 */ SEnumEntry( 22,                            "? (22)" ),
    /* 23 */ SEnumEntry( 23,                            "? (23)" ),
    /* 24 */ SEnumEntry( 24,                            "? (24)" ),
    /* 25 */ SEnumEntry( 25,                            "? (25)" ),
    /* 26 */ SEnumEntry( 26,                            "? (26)" ),
    /* 27 */ SEnumEntry( Qt::DisplayPropertyRole,       "DisplayPropertyRole" ),
    /* 28 */ SEnumEntry( Qt::DecorationPropertyRole,    "DecorationPropertyRole" ),
    /* 29 */ SEnumEntry( Qt::ToolTipPropertyRole,       "ToolTipPropertyRole" ),
    /* 30 */ SEnumEntry( Qt::StatusTipPropertyRole,     "StatusTipPropertyRole," ),
    /* 31 */ SEnumEntry( Qt::WhatsThisPropertyRole,     "WhatsThisPropertyRole," ),
    /* 32 */ SEnumEntry( Qt::UserRole,                  "UserRole" ),
};

//------------------------------------------------------------------------------
QString ZS::System::qItemDataRole2Str( int i_role )
//------------------------------------------------------------------------------
{
    QString str;
    if (i_role >= 0 && i_role < _ZSArrLen(s_arEnumStrQItemDataRole)) {
        str = SEnumEntry::enumerator2Str( s_arEnumStrQItemDataRole, _ZSArrLen(s_arEnumStrQItemDataRole), i_role );
    }
    else if (i_role < Qt::UserRole) {
        str = "? (" + QString::number(i_role) + ")";
    }
    else if (i_role >= Qt::UserRole) {
        str = "UserRole (" + QString::number(i_role) + ")";
    }
    return str;
}

/*==============================================================================
Enum Qt::ItemFlag
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::qItemFlags2Str( quint32 i_flags )
//------------------------------------------------------------------------------
{
    QString str;

    /*
    enum ItemFlag {
        NoItemFlags = 0,
        ItemIsSelectable = 1,
        ItemIsEditable = 2,
        ItemIsDragEnabled = 4,
        ItemIsDropEnabled = 8,
        ItemIsUserCheckable = 16,
        ItemIsEnabled = 32,
        ItemIsAutoTristate = 64,
        ItemNeverHasChildren = 128,
        ItemIsUserTristate = 256
    }; */

    if( i_flags == Qt::NoItemFlags )
    {
        str = "---";
    }
    else // if( i_flags != Qt::NoItemFlags )
    {
        if( i_flags & Qt::ItemIsSelectable )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "Sel";
        }
        if( i_flags & Qt::ItemIsEditable )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "Edi";
        }
        if( i_flags & Qt::ItemIsDragEnabled )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "DgE";
        }
        if( i_flags & Qt::ItemIsDropEnabled )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "DpE";
        }
        if( i_flags & Qt::ItemIsUserCheckable )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "UCh";
        }
        if( i_flags & Qt::ItemIsEnabled )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "Ena";
        }
        if( i_flags & Qt::ItemIsAutoTristate )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "AuT";
        }
        if( i_flags & Qt::ItemNeverHasChildren )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "NHC";
        }
        if( i_flags & Qt::ItemIsUserTristate )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "UsT";
        }
    } // if( i_flags != Qt::NoItemFlags )

    return str;

} // qItemFlags2Str

/*==============================================================================
Enum Qt::Key
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumStrQKeyCode[] =
//------------------------------------------------------------------------------
{
    /* 000: 0x00000020 (     032) */ SEnumEntry( Qt::Key_Space, "Key_Space", " ", " " ),
    /* 001: 0x00000021 (     033) */ SEnumEntry( Qt::Key_Exclam, "Key_Exclam", "?", "?" ),
    /* 002: 0x00000022 (     034) */ SEnumEntry( Qt::Key_QuoteDbl, "Key_QuoteDbl", "\"", "\"" ),
    /* 003: 0x00000023 (     035) */ SEnumEntry( Qt::Key_NumberSign, "Key_NumberSign", "#", "#" ),
    /* 004: 0x00000024 (     036) */ SEnumEntry( Qt::Key_Dollar, "Key_Dollar", "$", "$"  ),
    /* 005: 0x00000025 (     037) */ SEnumEntry( Qt::Key_Percent, "Key_Percent", "%", "%"  ),
    /* 006: 0x00000026 (     038) */ SEnumEntry( Qt::Key_Ampersand, "Key_Ampersand", "&", "&" ),
    /* 007: 0x00000027 (     039) */ SEnumEntry( Qt::Key_Apostrophe, "Key_Apostrophe", "'", "'" ),
    /* 008: 0x00000028 (     040) */ SEnumEntry( Qt::Key_ParenLeft, "Key_ParenLeft", "", "" ),
    /* 009: 0x00000029 (     041) */ SEnumEntry( Qt::Key_ParenRight, "Key_ParenRight", "", "" ),
    /* 010: 0x0000002a (     042) */ SEnumEntry( Qt::Key_Asterisk, "Key_Asterisk", "*", "*" ),
    /* 011: 0x0000002b (     043) */ SEnumEntry( Qt::Key_Plus, "Key_Plus", "+", "+" ),
    /* 012: 0x0000002c (     044) */ SEnumEntry( Qt::Key_Comma, "Key_Comma", ",", "," ),
    /* 013: 0x0000002d (     045) */ SEnumEntry( Qt::Key_Minus, "Key_Minus", "-", "-" ),
    /* 014: 0x0000002e (     046) */ SEnumEntry( Qt::Key_Period, "Key_Period", ".", "." ),
    /* 015: 0x0000002f (     047) */ SEnumEntry( Qt::Key_Slash, "Key_Slash", "/", "/" ),
    /* 016: 0x00000030 (     048) */ SEnumEntry( Qt::Key_0, "Key_0", "0", "0" ),
    /* 017: 0x00000031 (     049) */ SEnumEntry( Qt::Key_1, "Key_1", "1", "1" ),
    /* 018: 0x00000032 (     050) */ SEnumEntry( Qt::Key_2, "Key_2", "2", "2" ),
    /* 019: 0x00000033 (     051) */ SEnumEntry( Qt::Key_3, "Key_3", "3", "3" ),
    /* 020: 0x00000034 (     052) */ SEnumEntry( Qt::Key_4, "Key_4", "4", "4" ),
    /* 021: 0x00000035 (     053) */ SEnumEntry( Qt::Key_5, "Key_5", "5", "5" ),
    /* 022: 0x00000036 (     054) */ SEnumEntry( Qt::Key_6, "Key_6", "6", "6" ),
    /* 023: 0x00000037 (     055) */ SEnumEntry( Qt::Key_7, "Key_7", "7", "7" ),
    /* 024: 0x00000038 (     056) */ SEnumEntry( Qt::Key_8, "Key_8", "8", "8" ),
    /* 025: 0x00000039 (     057) */ SEnumEntry( Qt::Key_9, "Key_9", "9", "9" ),
    /* 026: 0x0000003a (     058) */ SEnumEntry( Qt::Key_Colon, "Key_Colon", ":", ":" ),
    /* 027: 0x0000003b (     059) */ SEnumEntry( Qt::Key_Semicolon, "Key_Semicolon", ";", ";" ),
    /* 028: 0x0000003c (     060) */ SEnumEntry( Qt::Key_Less, "Key_Less", "<", "<" ),
    /* 029: 0x0000003d (     061) */ SEnumEntry( Qt::Key_Equal, "Key_Equal", "=", "=" ),
    /* 030: 0x0000003e (     062) */ SEnumEntry( Qt::Key_Greater, "Key_Greater", ">", ">" ),
    /* 031: 0x0000003f (     063) */ SEnumEntry( Qt::Key_Question, "Key_Question", "?", "?" ),
    /* 032: 0x00000040 (     064) */ SEnumEntry( Qt::Key_At, "Key_At", "@", "@" ),
    /* 033: 0x00000041 (     065) */ SEnumEntry( Qt::Key_A, "Key_A", "a", "a" ),
    /* 034: 0x00000042 (     066) */ SEnumEntry( Qt::Key_B, "Key_B", "b", "b" ),
    /* 035: 0x00000043 (     067) */ SEnumEntry( Qt::Key_C, "Key_C", "c", "c" ),
    /* 036: 0x00000044 (     068) */ SEnumEntry( Qt::Key_D, "Key_D", "d", "d" ),
    /* 037: 0x00000045 (     069) */ SEnumEntry( Qt::Key_E, "Key_E", "e", "e" ),
    /* 038: 0x00000046 (     070) */ SEnumEntry( Qt::Key_F, "Key_F", "f", "f" ),
    /* 039: 0x00000047 (     071) */ SEnumEntry( Qt::Key_G, "Key_G", "g", "g" ),
    /* 040: 0x00000048 (     072) */ SEnumEntry( Qt::Key_H, "Key_H", "h", "h" ),
    /* 041: 0x00000049 (     073) */ SEnumEntry( Qt::Key_I, "Key_I", "i", "i" ),
    /* 042: 0x0000004a (     074) */ SEnumEntry( Qt::Key_J, "Key_J", "j", "j" ),
    /* 043: 0x0000004b (     075) */ SEnumEntry( Qt::Key_K, "Key_K", "k", "k" ),
    /* 044: 0x0000004c (     076) */ SEnumEntry( Qt::Key_L, "Key_L", "l", "l" ),
    /* 045: 0x0000004d (     077) */ SEnumEntry( Qt::Key_M, "Key_M", "m", "m" ),
    /* 046: 0x0000004e (     078) */ SEnumEntry( Qt::Key_N, "Key_N", "n", "n" ),
    /* 047: 0x0000004f (     079) */ SEnumEntry( Qt::Key_O, "Key_O", "o", "o" ),
    /* 048: 0x00000050 (     080) */ SEnumEntry( Qt::Key_P, "Key_P", "p", "p" ),
    /* 049: 0x00000051 (     081) */ SEnumEntry( Qt::Key_Q, "Key_Q", "q", "q" ),
    /* 050: 0x00000052 (     082) */ SEnumEntry( Qt::Key_R, "Key_R", "r", "r" ),
    /* 051: 0x00000053 (     083) */ SEnumEntry( Qt::Key_S, "Key_S", "s", "s" ),
    /* 052: 0x00000054 (     084) */ SEnumEntry( Qt::Key_T, "Key_T", "t", "t" ),
    /* 053: 0x00000055 (     085) */ SEnumEntry( Qt::Key_U, "Key_U", "u", "u" ),
    /* 054: 0x00000056 (     086) */ SEnumEntry( Qt::Key_V, "Key_V", "v", "v" ),
    /* 055: 0x00000057 (     087) */ SEnumEntry( Qt::Key_W, "Key_W", "w", "w" ),
    /* 056: 0x00000058 (     088) */ SEnumEntry( Qt::Key_X, "Key_X", "x", "x" ),
    /* 057: 0x00000059 (     089) */ SEnumEntry( Qt::Key_Y, "Key_Y", "y", "y" ),
    /* 058: 0x0000005a (     090) */ SEnumEntry( Qt::Key_Z, "Key_Z", "z", "z" ),
    /* 059: 0x0000005b (     091) */ SEnumEntry( Qt::Key_BracketLeft, "Key_BracketLeft", "{", "{" ),
    /* 060: 0x0000005c (     092) */ SEnumEntry( Qt::Key_Backslash, "Key_Backslash", "\\", "\\" ),
    /* 061: 0x0000005d (     093) */ SEnumEntry( Qt::Key_BracketRight, "Key_BracketRight", "}", "}" ),
    /* 062: 0x0000005e (     094) */ SEnumEntry( Qt::Key_AsciiCircum, "Key_AsciiCircum", "^", "^" ),
    /* 063: 0x0000005f (     095) */ SEnumEntry( Qt::Key_Underscore, "Key_Underscore", "_", "_" ),
    /* 064: 0x00000060 (     096) */ SEnumEntry( Qt::Key_QuoteLeft, "Key_QuoteLeft", "`", "`" ),
    /* 065: 0x0000007b (     123) */ SEnumEntry( Qt::Key_BraceLeft, "Key_BraceLeft", "[", "[" ),
    /* 066: 0x0000007c (     124) */ SEnumEntry( Qt::Key_Bar, "Key_Bar", "", "" ),
    /* 067: 0x0000007d (     125) */ SEnumEntry( Qt::Key_BraceRight, "Key_BraceRight", "]", "]" ),
    /* 068: 0x0000007e (     126) */ SEnumEntry( Qt::Key_AsciiTilde, "Key_AsciiTilde", "~", "~" ),
    /* 069: 0x000000a0 (     160) */ SEnumEntry( Qt::Key_nobreakspace, "Key_nobreakspace", "", "" ),
    /* 070: 0x000000a1 (     161) */ SEnumEntry( Qt::Key_exclamdown, "Key_exclamdown", "", "" ),
    /* 071: 0x000000a2 (     162) */ SEnumEntry( Qt::Key_cent, "Key_cent", "", "" ),
    /* 072: 0x000000a3 (     163) */ SEnumEntry( Qt::Key_sterling, "Key_sterling", "", "" ),
    /* 073: 0x000000a4 (     164) */ SEnumEntry( Qt::Key_currency, "Key_currency", "", "" ),
    /* 074: 0x000000a5 (     165) */ SEnumEntry( Qt::Key_yen, "Key_yen", "", "" ),
    /* 075: 0x000000a6 (     166) */ SEnumEntry( Qt::Key_brokenbar, "Key_brokenbar", "", "" ),
    /* 076: 0x000000a7 (     167) */ SEnumEntry( Qt::Key_section, "Key_section", "", "" ),
    /* 077: 0x000000a8 (     168) */ SEnumEntry( Qt::Key_diaeresis, "Key_diaeresis", "", "" ),
    /* 078: 0x000000a9 (     169) */ SEnumEntry( Qt::Key_copyright, "Key_copyright", "", "" ),
    /* 079: 0x000000aa (     170) */ SEnumEntry( Qt::Key_ordfeminine, "Key_ordfeminine", "", "" ),
    /* 080: 0x000000ab (     171) */ SEnumEntry( Qt::Key_guillemotleft, "Key_guillemotleft", "", "" ),
    /* 081: 0x000000ac (     172) */ SEnumEntry( Qt::Key_notsign, "Key_notsign", "", "" ),
    /* 082: 0x000000ad (     173) */ SEnumEntry( Qt::Key_hyphen, "Key_hyphen", "", "" ),
    /* 083: 0x000000ae (     174) */ SEnumEntry( Qt::Key_registered, "Key_registered", "", "" ),
    /* 084: 0x000000af (     175) */ SEnumEntry( Qt::Key_macron, "Key_macron", "", "" ),
    /* 085: 0x000000b0 (     176) */ SEnumEntry( Qt::Key_degree, "Key_degree", "", "" ),
    /* 086: 0x000000b1 (     177) */ SEnumEntry( Qt::Key_plusminus, "Key_plusminus", "", "" ),
    /* 087: 0x000000b2 (     178) */ SEnumEntry( Qt::Key_twosuperior, "Key_twosuperior", "", "" ),
    /* 088: 0x000000b3 (     179) */ SEnumEntry( Qt::Key_threesuperior, "Key_threesuperior", "", "" ),
    /* 089: 0x000000b4 (     180) */ SEnumEntry( Qt::Key_acute, "Key_acute", "", "" ),
    /* 090: 0x000000b5 (     181) */ SEnumEntry( Qt::Key_mu, "Key_mu", "", "" ),
    /* 091: 0x000000b6 (     182) */ SEnumEntry( Qt::Key_paragraph, "Key_paragraph", "", "" ),
    /* 092: 0x000000b7 (     183) */ SEnumEntry( Qt::Key_periodcentered, "Key_periodcentered", "", "" ),
    /* 093: 0x000000b8 (     184) */ SEnumEntry( Qt::Key_cedilla, "Key_cedilla", "", "" ),
    /* 094: 0x000000b9 (     185) */ SEnumEntry( Qt::Key_onesuperior, "Key_onesuperior", "", "" ),
    /* 095: 0x000000ba (     186) */ SEnumEntry( Qt::Key_masculine, "Key_masculine", "", "" ),
    /* 096: 0x000000bb (     187) */ SEnumEntry( Qt::Key_guillemotright, "Key_guillemotright", "", "" ),
    /* 097: 0x000000bc (     188) */ SEnumEntry( Qt::Key_onequarter, "Key_onequarter", "", "" ),
    /* 098: 0x000000bd (     189) */ SEnumEntry( Qt::Key_onehalf, "Key_onehalf", "", "" ),
    /* 099: 0x000000be (     190) */ SEnumEntry( Qt::Key_threequarters, "Key_threequarters", "", "" ),
    /* 100: 0x000000bf (     191) */ SEnumEntry( Qt::Key_questiondown, "Key_questiondown", "", "" ),
    /* 101: 0x000000c0 (     192) */ SEnumEntry( Qt::Key_Agrave, "Key_Agrave", "", "" ),
    /* 102: 0x000000c1 (     193) */ SEnumEntry( Qt::Key_Aacute, "Key_Aacute", "", "" ),
    /* 103: 0x000000c2 (     194) */ SEnumEntry( Qt::Key_Acircumflex, "Key_Acircumflex", "", "" ),
    /* 104: 0x000000c3 (     195) */ SEnumEntry( Qt::Key_Atilde, "Key_Atilde", "", "" ),
    /* 105: 0x000000c4 (     196) */ SEnumEntry( Qt::Key_Adiaeresis, "Key_Adiaeresis", "", "" ),
    /* 106: 0x000000c5 (     197) */ SEnumEntry( Qt::Key_Aring, "Key_Aring", "", "" ),
    /* 107: 0x000000c6 (     198) */ SEnumEntry( Qt::Key_AE, "Key_AE", "", "" ),
    /* 108: 0x000000c7 (     199) */ SEnumEntry( Qt::Key_Ccedilla, "Key_Ccedilla", "", "" ),
    /* 109: 0x000000c8 (     200) */ SEnumEntry( Qt::Key_Egrave, "Key_Egrave", "", "" ),
    /* 110: 0x000000c9 (     201) */ SEnumEntry( Qt::Key_Eacute, "Key_Eacute", "", "" ),
    /* 111: 0x000000ca (     202) */ SEnumEntry( Qt::Key_Ecircumflex, "Key_Ecircumflex", "", "" ),
    /* 112: 0x000000cb (     203) */ SEnumEntry( Qt::Key_Ediaeresis, "Key_Ediaeresis", "", "" ),
    /* 113: 0x000000cc (     204) */ SEnumEntry( Qt::Key_Igrave, "Key_Igrave", "", "" ),
    /* 114: 0x000000cd (     205) */ SEnumEntry( Qt::Key_Iacute, "Key_Iacute", "", "" ),
    /* 115: 0x000000ce (     206) */ SEnumEntry( Qt::Key_Icircumflex, "Key_Icircumflex", "", "" ),
    /* 116: 0x000000cf (     207) */ SEnumEntry( Qt::Key_Idiaeresis, "Key_Idiaeresis", "", "" ),
    /* 117: 0x000000d0 (     208) */ SEnumEntry( Qt::Key_ETH, "Key_ETH", "", "" ),
    /* 118: 0x000000d1 (     209) */ SEnumEntry( Qt::Key_Ntilde, "Key_Ntilde", "", "" ),
    /* 119: 0x000000d2 (     210) */ SEnumEntry( Qt::Key_Ograve, "Key_Ograve", "", "" ),
    /* 120: 0x000000d3 (     211) */ SEnumEntry( Qt::Key_Oacute, "Key_Oacute", "", "" ),
    /* 121: 0x000000d4 (     212) */ SEnumEntry( Qt::Key_Ocircumflex, "Key_Ocircumflex", "", "" ),
    /* 122: 0x000000d5 (     213) */ SEnumEntry( Qt::Key_Otilde, "Key_Otilde", "", "" ),
    /* 123: 0x000000d6 (     214) */ SEnumEntry( Qt::Key_Odiaeresis, "Key_Odiaeresis", "", "" ),
    /* 124: 0x000000d7 (     215) */ SEnumEntry( Qt::Key_multiply, "Key_multiply", "", "" ),
    /* 125: 0x000000d8 (     216) */ SEnumEntry( Qt::Key_Ooblique, "Key_Ooblique", "", "" ),
    /* 126: 0x000000d9 (     217) */ SEnumEntry( Qt::Key_Ugrave, "Key_Ugrave", "", "" ),
    /* 127: 0x000000da (     218) */ SEnumEntry( Qt::Key_Uacute, "Key_Uacute", "", "" ),
    /* 128: 0x000000db (     219) */ SEnumEntry( Qt::Key_Ucircumflex, "Key_Ucircumflex", "", "" ),
    /* 129: 0x000000dc (     220) */ SEnumEntry( Qt::Key_Udiaeresis, "Key_Udiaeresis", "", "" ),
    /* 130: 0x000000dd (     221) */ SEnumEntry( Qt::Key_Yacute, "Key_Yacute", "", "" ),
    /* 131: 0x000000de (     222) */ SEnumEntry( Qt::Key_THORN, "Key_THORN", "", "" ),
    /* 132: 0x000000df (     223) */ SEnumEntry( Qt::Key_ssharp, "Key_ssharp", "", "" ),
    /* 133: 0x000000f7 (     224) */ SEnumEntry( Qt::Key_division, "Key_division", "", "" ),
    /* 134: 0x000000ff (     225) */ SEnumEntry( Qt::Key_ydiaeresis, "Key_ydiaeresis", "", "" ),
    /* 135: 0x01000000 (16777216) */ SEnumEntry( Qt::Key_Escape, "Key_Escape", "", "Escape" ),
    /* 136: 0x01000001 (16777217) */ SEnumEntry( Qt::Key_Tab, "Key_Tab", "", "Tab" ),
    /* 137: 0x01000002 (16777218) */ SEnumEntry( Qt::Key_Backtab, "Key_Backtab", "", "Backtab" ),
    /* 138: 0x01000003 (16777219) */ SEnumEntry( Qt::Key_Backspace, "Key_Backspace", "", "Backspace" ),
    /* 139: 0x01000004 (16777220) */ SEnumEntry( Qt::Key_Return, "Key_Return", "", "Return" ),
    /* 140: 0x01000005 (16777221) */ SEnumEntry( Qt::Key_Enter, "Key_Enter", "", "Enter" ),
    /* 141: 0x01000006 (16777222) */ SEnumEntry( Qt::Key_Insert, "Key_Insert", "", "Insert" ),
    /* 142: 0x01000007 (16777223) */ SEnumEntry( Qt::Key_Delete, "Key_Delete", "", "Delete" ),
    /* 143: 0x01000008 (16777224) */ SEnumEntry( Qt::Key_Pause, "Key_Pause", "", "Pause" ),
    /* 144: 0x01000009 (16777225) */ SEnumEntry( Qt::Key_Print, "Key_Print", "", "Print" ),
    /* 145: 0x0100000a (16777226) */ SEnumEntry( Qt::Key_SysReq, "Key_SysReq", "", "SysReq" ),
    /* 146: 0x0100000b (16777227) */ SEnumEntry( Qt::Key_Clear, "Key_Clear", "", "Clear" ),
    /* 147: 0x01000010 (16777232) */ SEnumEntry( Qt::Key_Home, "Key_Home", "", "Home" ),
    /* 148: 0x01000011 (16777233) */ SEnumEntry( Qt::Key_End, "Key_End", "", "End" ),
    /* 149: 0x01000012 (16777234) */ SEnumEntry( Qt::Key_Left, "Key_Left", "", "Left" ),
    /* 150: 0x01000013 (16777235) */ SEnumEntry( Qt::Key_Up, "Key_Up", "", "Up" ),
    /* 151: 0x01000014 (16777236) */ SEnumEntry( Qt::Key_Right, "Key_Right", "", "Right" ),
    /* 152: 0x01000015 (16777237) */ SEnumEntry( Qt::Key_Down, "Key_Down", "", "Down" ),
    /* 153: 0x01000016 (16777238) */ SEnumEntry( Qt::Key_PageUp, "Key_PageUp", "", "PageUp" ),
    /* 154: 0x01000017 (16777239) */ SEnumEntry( Qt::Key_PageDown, "Key_PageDown", "", "PageDown" ),
    /* 155: 0x01000020 (16777248) */ SEnumEntry( Qt::Key_Shift, "Key_Shift", "", "Shift" ),
    /* 156: 0x01000021 (16777249) */ SEnumEntry( Qt::Key_Control, "Key_Control", "", "Control" ),
    /* 157: 0x01000022 (16777250) */ SEnumEntry( Qt::Key_Meta, "Key_Meta", "", "Meta" ),
    /* 158: 0x01000023 (16777251) */ SEnumEntry( Qt::Key_Alt, "Key_Alt", "", "Alt" ),
    /* 159: 0x01000024 (16777252) */ SEnumEntry( Qt::Key_CapsLock, "Key_CapsLock", "", "CapsLock" ),
    /* 160: 0x01000025 (16777253) */ SEnumEntry( Qt::Key_NumLock, "Key_NumLock", "", "Numlock" ),
    /* 161: 0x01000026 (16777254) */ SEnumEntry( Qt::Key_ScrollLock, "Key_ScrollLock", "", "ScrollLock" ),
    /* 162: 0x01000030 (16777264) */ SEnumEntry( Qt::Key_F1, "Key_F1", "", "F1" ),
    /* 163: 0x01000031 (16777265) */ SEnumEntry( Qt::Key_F2, "Key_F2", "", "F2" ),
    /* 164: 0x01000032 (16777266) */ SEnumEntry( Qt::Key_F3, "Key_F3", "", "F3" ),
    /* 165: 0x01000033 (16777267) */ SEnumEntry( Qt::Key_F4, "Key_F4", "", "F4" ),
    /* 166: 0x01000034 (16777268) */ SEnumEntry( Qt::Key_F5, "Key_F5", "", "F5" ),
    /* 167: 0x01000035 (16777269) */ SEnumEntry( Qt::Key_F6, "Key_F6", "", "F6" ),
    /* 168: 0x01000036 (16777270) */ SEnumEntry( Qt::Key_F7, "Key_F7", "", "F7" ),
    /* 169: 0x01000037 (16777271) */ SEnumEntry( Qt::Key_F8, "Key_F8", "", "F8" ),
    /* 170: 0x01000038 (16777272) */ SEnumEntry( Qt::Key_F9, "Key_F9", "", "F9" ),
    /* 171: 0x01000039 (16777273) */ SEnumEntry( Qt::Key_F10, "Key_F10", "", "F10" ),
    /* 172: 0x0100003a (16777274) */ SEnumEntry( Qt::Key_F11, "Key_F11", "", "F11" ),
    /* 173: 0x0100003b (16777275) */ SEnumEntry( Qt::Key_F12, "Key_F12", "", "F12" ),
    /* 174: 0x0100003c (16777276) */ SEnumEntry( Qt::Key_F13, "Key_F13", "", "F13" ),
    /* 175: 0x0100003d (16777277) */ SEnumEntry( Qt::Key_F14, "Key_F14", "", "F14" ),
    /* 176: 0x0100003e (16777278) */ SEnumEntry( Qt::Key_F15, "Key_F15", "", "F15" ),
    /* 177: 0x0100003f (16777279) */ SEnumEntry( Qt::Key_F16, "Key_F16", "", "F16" ),
    /* 178: 0x01000040 (16777280) */ SEnumEntry( Qt::Key_F17, "Key_F17", "", "F17" ),
    /* 179: 0x01000041 (16777281) */ SEnumEntry( Qt::Key_F18, "Key_F18", "", "F18" ),
    /* 180: 0x01000042 (16777282) */ SEnumEntry( Qt::Key_F19, "Key_F19", "", "F19" ),
    /* 181: 0x01000043 (16777283) */ SEnumEntry( Qt::Key_F20, "Key_F20", "", "F20" ),
    /* 182: 0x01000044 (16777284) */ SEnumEntry( Qt::Key_F21, "Key_F21", "", "F21" ),
    /* 183: 0x01000045 (16777285) */ SEnumEntry( Qt::Key_F22, "Key_F22", "", "F22" ),
    /* 184: 0x01000046 (16777286) */ SEnumEntry( Qt::Key_F23, "Key_F23", "", "F23" ),
    /* 185: 0x01000047 (16777287) */ SEnumEntry( Qt::Key_F24, "Key_F24", "", "F24" ),
    /* 186: 0x01000048 (16777288) */ SEnumEntry( Qt::Key_F25, "Key_F25", "", "F25" ),
    /* 187: 0x01000049 (16777289) */ SEnumEntry( Qt::Key_F26, "Key_F26", "", "F26" ),
    /* 188: 0x0100004a (16777290) */ SEnumEntry( Qt::Key_F27, "Key_F27", "", "F27" ),
    /* 189: 0x0100004b (16777291) */ SEnumEntry( Qt::Key_F28, "Key_F28", "", "F28" ),
    /* 190: 0x0100004c (16777292) */ SEnumEntry( Qt::Key_F29, "Key_F29", "", "F29" ),
    /* 191: 0x0100004d (16777293) */ SEnumEntry( Qt::Key_F30, "Key_F30", "", "F30" ),
    /* 192: 0x0100004e (16777294) */ SEnumEntry( Qt::Key_F31, "Key_F31", "", "F31" ),
    /* 193: 0x0100004f (16777295) */ SEnumEntry( Qt::Key_F32, "Key_F32", "", "F32" ),
    /* 194: 0x01000050 (16777296) */ SEnumEntry( Qt::Key_F33, "Key_F33", "", "F33" ),
    /* 195: 0x01000051 (16777297) */ SEnumEntry( Qt::Key_F34, "Key_F34", "", "F34" ),
    /* 196: 0x01000052 (16777298) */ SEnumEntry( Qt::Key_F35, "Key_F35", "", "F35" ),
    /* 197: 0x01000053 (16777299) */ SEnumEntry( Qt::Key_Super_L, "Key_Super_L", "", "" ),
    /* 198: 0x01000054 (16777300) */ SEnumEntry( Qt::Key_Super_R, "Key_Super_R", "", "" ),
    /* 199: 0x01000055 (16777301) */ SEnumEntry( Qt::Key_Menu, "Key_Menu", "", "" ),
    /* 200: 0x01000056 (16777302) */ SEnumEntry( Qt::Key_Hyper_L, "Key_Hyper_L", "", "" ),
    /* 201: 0x01000057 (16777303) */ SEnumEntry( Qt::Key_Hyper_R, "Key_Hyper_R", "", "" ),
    /* 202: 0x01000058 (16777304) */ SEnumEntry( Qt::Key_Help, "Key_Help", "", "" ),
    /* 203: 0x01000059 (16777305) */ SEnumEntry( Qt::Key_Direction_L, "Key_Direction_L", "", "" ),
    /* 204: 0x01000060 (16777312) */ SEnumEntry( Qt::Key_Direction_R, "Key_Direction_R", "", "" ),
    /* 205: 0x01001103 (16781571) */ SEnumEntry( Qt::Key_AltGr, "Key_AltGr", "", "" ),
    /* 206: 0x01001120 (16781600) */ SEnumEntry( Qt::Key_Multi_key, "Key_Multi_key", "", "" ),
    /* 207: 0x01001137 (16781623) */ SEnumEntry( Qt::Key_Codeinput, "Key_Codeinput", "", "" ),
    /* 208: 0x0100113c (16781628) */ SEnumEntry( Qt::Key_SingleCandidate, "Key_SingleCandidate", "", "" ),
    /* 209: 0x0100113d (16781629) */ SEnumEntry( Qt::Key_MultipleCandidate, "Key_MultipleCandidate", "", "" ),
    /* 210: 0x0100113e (16781630) */ SEnumEntry( Qt::Key_PreviousCandidate, "Key_PreviousCandidate", "", "" ),
    /* 211: 0x0100117e (16781649) */ SEnumEntry( Qt::Key_Mode_switch, "Key_Mode_switch", "", "" ),
    /* 212: 0x01001121 (16781601) */ SEnumEntry( Qt::Key_Kanji, "Key_Kanji", "", "" ),
    /* 213: 0x01001122 (16781602) */ SEnumEntry( Qt::Key_Muhenkan, "Key_Muhenkan", "", "" ),
    /* 214: 0x01001123 (16781603) */ SEnumEntry( Qt::Key_Henkan, "Key_Henkan", "", "" ),
    /* 215: 0x01001124 (16781604) */ SEnumEntry( Qt::Key_Romaji, "Key_Romaji", "", "" ),
    /* 216: 0x01001125 (16781605) */ SEnumEntry( Qt::Key_Hiragana, "Key_Hiragana", "", "" ),
    /* 217: 0x01001126 (16781606) */ SEnumEntry( Qt::Key_Katakana, "Key_Katakana", "", "" ),
    /* 218: 0x01001127 (16781607) */ SEnumEntry( Qt::Key_Hiragana_Katakana, "Key_Hiragana_Katakana", "", "" ),
    /* 219: 0x01001128 (16781608) */ SEnumEntry( Qt::Key_Zenkaku, "Key_Zenkaku", "", "" ),
    /* 220: 0x01001129 (16781609) */ SEnumEntry( Qt::Key_Hankaku, "Key_Hankaku", "", "" ),
    /* 221: 0x0100112a (16781610) */ SEnumEntry( Qt::Key_Zenkaku_Hankaku, "Key_Zenkaku_Hankaku", "", "" ),
    /* 222: 0x0100112b (16781611) */ SEnumEntry( Qt::Key_Touroku, "Key_Touroku", "", "" ),
    /* 223: 0x0100112c (16781612) */ SEnumEntry( Qt::Key_Massyo, "Key_Massyo", "", "" ),
    /* 224: 0x0100112d (16781613) */ SEnumEntry( Qt::Key_Kana_Lock, "Key_Kana_Lock", "", "" ),
    /* 225: 0x0100112e (16781614) */ SEnumEntry( Qt::Key_Kana_Shift, "Key_Kana_Shift", "", "" ),
    /* 226: 0x0100112f (16781615) */ SEnumEntry( Qt::Key_Eisu_Shift, "Key_Eisu_Shift", "", "" ),
    /* 227: 0x01001130 (16781616) */ SEnumEntry( Qt::Key_Eisu_toggle, "Key_Eisu_toggle", "", "" ),
    /* 228: 0x01001131 (16781617) */ SEnumEntry( Qt::Key_Hangul, "Key_Hangul", "", "" ),
    /* 229: 0x01001132 (16781618) */ SEnumEntry( Qt::Key_Hangul_Start, "Key_Hangul_Start", "", "" ),
    /* 230: 0x01001133 (16781619) */ SEnumEntry( Qt::Key_Hangul_End, "Key_Hangul_End", "", "" ),
    /* 231: 0x01001134 (16781620) */ SEnumEntry( Qt::Key_Hangul_Hanja, "Key_Hangul_Hanja", "", "" ),
    /* 232: 0x01001135 (16781621) */ SEnumEntry( Qt::Key_Hangul_Jamo, "Key_Hangul_Jamo", "", "" ),
    /* 233: 0x01001136 (16781622) */ SEnumEntry( Qt::Key_Hangul_Romaja, "Key_Hangul_Romaja", "", "" ),
    /* 234: 0x01001138 (16781624) */ SEnumEntry( Qt::Key_Hangul_Jeonja, "Key_Hangul_Jeonja", "", "" ),
    /* 235: 0x01001139 (16781625) */ SEnumEntry( Qt::Key_Hangul_Banja, "Key_Hangul_Banja", "", "" ),
    /* 236: 0x0100113a (16781626) */ SEnumEntry( Qt::Key_Hangul_PreHanja, "Key_Hangul_PreHanja", "", "" ),
    /* 237: 0x0100113b (16781627) */ SEnumEntry( Qt::Key_Hangul_PostHanja, "Key_Hangul_PostHanja", "", "" ),
    /* 238: 0x0100113f (16781631) */ SEnumEntry( Qt::Key_Hangul_Special, "Key_Hangul_Special", "", "" ),
    /* 239: 0x01001250 (16781904) */ SEnumEntry( Qt::Key_Dead_Grave, "Key_Dead_Grave", "", "" ),
    /* 240: 0x01001251 (16781905) */ SEnumEntry( Qt::Key_Dead_Acute, "Key_Dead_Acute", "", "" ),
    /* 241: 0x01001252 (16781906) */ SEnumEntry( Qt::Key_Dead_Circumflex, "Key_Dead_Circumflex", "", "" ),
    /* 242: 0x01001253 (16781907) */ SEnumEntry( Qt::Key_Dead_Tilde, "Key_Dead_Tilde", "", "" ),
    /* 243: 0x01001254 (16781908) */ SEnumEntry( Qt::Key_Dead_Macron, "Key_Dead_Macron", "", "" ),
    /* 244: 0x01001255 (16781909) */ SEnumEntry( Qt::Key_Dead_Breve, "Key_Dead_Breve", "", "" ),
    /* 245: 0x01001256 (16781910) */ SEnumEntry( Qt::Key_Dead_Abovedot, "Key_Dead_Abovedot", "", "" ),
    /* 246: 0x01001257 (16781911) */ SEnumEntry( Qt::Key_Dead_Diaeresis, "Key_Dead_Diaeresis", "", "" ),
    /* 247: 0x01001258 (16781912) */ SEnumEntry( Qt::Key_Dead_Abovering, "Key_Dead_Abovering", "", "" ),
    /* 248: 0x01001259 (16781913) */ SEnumEntry( Qt::Key_Dead_Doubleacute, "Key_Dead_Doubleacute", "", "" ),
    /* 249: 0x0100125a (16781914) */ SEnumEntry( Qt::Key_Dead_Caron, "Key_Dead_Caron", "", "" ),
    /* 250: 0x0100125b (16781915) */ SEnumEntry( Qt::Key_Dead_Cedilla, "Key_Dead_Cedilla", "", "" ),
    /* 251: 0x0100125c (16781916) */ SEnumEntry( Qt::Key_Dead_Ogonek, "Key_Dead_Ogonek", "", "" ),
    /* 252: 0x0100125d (16781917) */ SEnumEntry( Qt::Key_Dead_Iota, "Key_Dead_Iota", "", "" ),
    /* 253: 0x0100125e (16781918) */ SEnumEntry( Qt::Key_Dead_Voiced_Sound, "Key_Dead_Voiced_Sound", "", "" ),
    /* 254: 0x0100125f (16781919) */ SEnumEntry( Qt::Key_Dead_Semivoiced_Sound, "Key_Dead_Semivoiced_Sound", "", "" ),
    /* 255: 0x01001260 (16781920) */ SEnumEntry( Qt::Key_Dead_Belowdot, "Key_Dead_Belowdot", "", "" ),
    /* 256: 0x01001261 (16781921) */ SEnumEntry( Qt::Key_Dead_Hook, "Key_Dead_Hook", "", "" ),
    /* 257: 0x01001262 (16781922) */ SEnumEntry( Qt::Key_Dead_Horn, "Key_Dead_Horn", "", "" ),
    /* 258: 0x01000061 (16777313) */ SEnumEntry( Qt::Key_Back, "Key_Back", "", "" ),
    /* 259: 0x01000062 (16777314) */ SEnumEntry( Qt::Key_Forward, "Key_Forward", "", "" ),
    /* 260: 0x01000063 (16777315) */ SEnumEntry( Qt::Key_Stop, "Key_Stop", "", "" ),
    /* 261: 0x01000064 (16777316) */ SEnumEntry( Qt::Key_Refresh, "Key_Refresh", "", "" ),
    /* 262: 0x01000070 (16777328) */ SEnumEntry( Qt::Key_VolumeDown, "Key_VolumeDown", "", "" ),
    /* 263: 0x01000071 (16777329) */ SEnumEntry( Qt::Key_VolumeMute, "Key_VolumeMute", "", "" ),
    /* 264: 0x01000072 (16777330) */ SEnumEntry( Qt::Key_VolumeUp, "Key_VolumeUp", "", "" ),
    /* 265: 0x01000073 (16777331) */ SEnumEntry( Qt::Key_BassBoost, "Key_BassBoost", "", "" ),
    /* 266: 0x01000074 (16777332) */ SEnumEntry( Qt::Key_BassUp, "Key_BassUp", "", "" ),
    /* 267: 0x01000075 (16777333) */ SEnumEntry( Qt::Key_BassDown, "Key_BassDown", "", "" ),
    /* 268: 0x01000076 (16777334) */ SEnumEntry( Qt::Key_TrebleUp, "Key_TrebleUp", "", "" ),
    /* 269: 0x01000077 (16777335) */ SEnumEntry( Qt::Key_TrebleDown, "Key_TrebleDown", "", "" ),
    /* 270: 0x01000080 (16777344) */ SEnumEntry( Qt::Key_MediaPlay, "Key_MediaPlay", "", "" ),
    /* 271: 0x01000081 (16777345) */ SEnumEntry( Qt::Key_MediaStop, "Key_MediaStop", "", "" ),
    /* 272: 0x01000082 (16777346) */ SEnumEntry( Qt::Key_MediaPrevious, "Key_MediaPrevious", "", "" ),
    /* 273: 0x01000083 (16777347) */ SEnumEntry( Qt::Key_MediaNext, "Key_MediaNext", "", "" ),
    /* 274: 0x01000084 (16777348) */ SEnumEntry( Qt::Key_MediaRecord, "Key_MediaRecord", "", "" ),
    #if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
    /* 275: 0x01000085 (16777349) */ SEnumEntry( Qt::Key_MediaPause, "Key_MediaPause", "", "" ),
    /* 276: 0x01000086 (16777350) */ SEnumEntry( Qt::Key_MediaTogglePlayPause, "Key_MediaTogglePlayPause", "", "" ),
    #endif
    /* 277: 0x01000090 (16777360) */ SEnumEntry( Qt::Key_HomePage, "Key_HomePage", "", "" ),
    /* 278: 0x01000091 (16777361) */ SEnumEntry( Qt::Key_Favorites, "Key_Favorites", "", "" ),
    /* 279: 0x01000092 (16777362) */ SEnumEntry( Qt::Key_Search, "Key_Search", "", "" ),
    /* 280: 0x01000093 (16777363) */ SEnumEntry( Qt::Key_Standby, "Key_Standby", "", "" ),
    /* 281: 0x01000094 (16777364) */ SEnumEntry( Qt::Key_OpenUrl, "Key_OpenUrl", "", "" ),
    /* 282: 0x010000a0 (16777376) */ SEnumEntry( Qt::Key_LaunchMail, "Key_LaunchMail", "", "" ),
    /* 283: 0x010000a1 (16777377) */ SEnumEntry( Qt::Key_LaunchMedia, "Key_LaunchMedia", "", "" ),
    /* 284: 0x010000a2 (16777378) */ SEnumEntry( Qt::Key_Launch0, "Key_Launch0", "", "" ),
    /* 285: 0x010000a3 (16777379) */ SEnumEntry( Qt::Key_Launch1, "Key_Launch1", "", "" ),
    /* 286: 0x010000a4 (16777380) */ SEnumEntry( Qt::Key_Launch2, "Key_Launch2", "", "" ),
    /* 287: 0x010000a5 (16777381) */ SEnumEntry( Qt::Key_Launch3, "Key_Launch3", "", "" ),
    /* 288: 0x010000a6 (16777382) */ SEnumEntry( Qt::Key_Launch4, "Key_Launch4", "", "" ),
    /* 289: 0x010000a7 (16777383) */ SEnumEntry( Qt::Key_Launch5, "Key_Launch5", "", "" ),
    /* 290: 0x010000a8 (16777384) */ SEnumEntry( Qt::Key_Launch6, "Key_Launch6", "", "" ),
    /* 291: 0x010000a9 (16777385) */ SEnumEntry( Qt::Key_Launch7, "Key_Launch7", "", "" ),
    /* 292: 0x010000aa (16777386) */ SEnumEntry( Qt::Key_Launch8, "Key_Launch8", "", "" ),
    /* 293: 0x010000ab (16777387) */ SEnumEntry( Qt::Key_Launch9, "Key_Launch9", "", "" ),
    /* 294: 0x010000ac (16777388) */ SEnumEntry( Qt::Key_LaunchA, "Key_LaunchA", "", "" ),
    /* 295: 0x010000ad (16777389) */ SEnumEntry( Qt::Key_LaunchB, "Key_LaunchB", "", "" ),
    /* 296: 0x010000ae (16777390) */ SEnumEntry( Qt::Key_LaunchC, "Key_LaunchC", "", "" ),
    /* 297: 0x010000af (16777391) */ SEnumEntry( Qt::Key_LaunchD, "Key_LaunchD", "", "" ),
    /* 298: 0x010000b0 (16777392) */ SEnumEntry( Qt::Key_LaunchE, "Key_LaunchE", "", "" ),
    /* 299: 0x010000b1 (16777393) */ SEnumEntry( Qt::Key_LaunchF, "Key_LaunchF", "", "" ),
    /* 300: 0x010000b2 (16777394) */ SEnumEntry( Qt::Key_MonBrightnessUp, "Key_MonBrightnessUp", "", "" ),
    /* 301: 0x010000b3 (16777395) */ SEnumEntry( Qt::Key_MonBrightnessDown, "Key_MonBrightnessDown", "", "" ),
    /* 302: 0x010000b4 (16777396) */ SEnumEntry( Qt::Key_KeyboardLightOnOff, "Key_KeyboardLightOnOff", "", "" ),
    /* 303: 0x010000b5 (16777397) */ SEnumEntry( Qt::Key_KeyboardBrightnessUp, "Key_KeyboardBrightnessUp", "", "" ),
    /* 304: 0x010000b6 (16777398) */ SEnumEntry( Qt::Key_KeyboardBrightnessDown, "Key_KeyboardBrightnessDown", "", "" ),
    /* 305: 0x010000b7 (16777399) */ SEnumEntry( Qt::Key_PowerOff, "Key_PowerOff", "", "" ),
    /* 306: 0x010000b8 (16777400) */ SEnumEntry( Qt::Key_WakeUp, "Key_WakeUp", "", "" ),
    /* 307: 0x010000b9 (16777401) */ SEnumEntry( Qt::Key_Eject, "Key_Eject", "", "" ),
    /* 308: 0x010000ba (16777402) */ SEnumEntry( Qt::Key_ScreenSaver, "Key_ScreenSaver", "", "" ),
    /* 309: 0x010000bb (16777403) */ SEnumEntry( Qt::Key_WWW, "Key_WWW", "", "" ),
    /* 310: 0x010000bc (16777404) */ SEnumEntry( Qt::Key_Memo, "Key_Memo", "", "" ),
    /* 311: 0x010000bd (16777405) */ SEnumEntry( Qt::Key_LightBulb, "Key_LightBulb", "", "" ),
    /* 312: 0x010000be (16777406) */ SEnumEntry( Qt::Key_Shop, "Key_Shop", "", "" ),
    /* 313: 0x010000bf (16777407) */ SEnumEntry( Qt::Key_History, "Key_History", "", "" ),
    /* 314: 0x010000c0 (16777408) */ SEnumEntry( Qt::Key_AddFavorite, "Key_AddFavorite", "", "" ),
    /* 315: 0x010000c1 (16777409) */ SEnumEntry( Qt::Key_HotLinks, "Key_HotLinks", "", "" ),
    /* 316: 0x010000c2 (16777410) */ SEnumEntry( Qt::Key_BrightnessAdjust, "Key_BrightnessAdjust", "", "" ),
    /* 317: 0x010000c3 (16777411) */ SEnumEntry( Qt::Key_Finance, "Key_Finance", "", "" ),
    /* 318: 0x010000c4 (16777412) */ SEnumEntry( Qt::Key_Community, "Key_Community", "", "" ),
    /* 319: 0x010000c5 (16777413) */ SEnumEntry( Qt::Key_AudioRewind, "Key_AudioRewind", "", "" ),
    /* 320: 0x010000c6 (16777414) */ SEnumEntry( Qt::Key_BackForward, "Key_BackForward", "", "" ),
    /* 321: 0x010000c7 (16777415) */ SEnumEntry( Qt::Key_ApplicationLeft, "Key_ApplicationLeft", "", "" ),
    /* 322: 0x010000c8 (16777416) */ SEnumEntry( Qt::Key_ApplicationRight, "Key_ApplicationRight", "", "" ),
    /* 323: 0x010000c9 (16777417) */ SEnumEntry( Qt::Key_Book, "Key_Book", "", "" ),
    /* 324: 0x010000ca (16777418) */ SEnumEntry( Qt::Key_CD, "Key_CD", "", "" ),
    /* 325: 0x010000cb (16777419) */ SEnumEntry( Qt::Key_Calculator, "Key_Calculator", "", "" ),
    /* 326: 0x010000cc (16777420) */ SEnumEntry( Qt::Key_ToDoList, "Key_ToDoList", "", "" ),
    /* 327: 0x010000cd (16777421) */ SEnumEntry( Qt::Key_ClearGrab, "Key_ClearGrab", "", "" ),
    /* 328: 0x010000ce (16777422) */ SEnumEntry( Qt::Key_Close, "Key_Close", "", "" ),
    /* 329: 0x010000cf (16777423) */ SEnumEntry( Qt::Key_Copy, "Key_Copy", "", "" ),
    /* 330: 0x010000d0 (16777424) */ SEnumEntry( Qt::Key_Cut, "Key_Cut", "", "" ),
    /* 331: 0x010000d1 (16777425) */ SEnumEntry( Qt::Key_Display, "Key_Display", "", "" ),
    /* 332: 0x010000d2 (16777426) */ SEnumEntry( Qt::Key_DOS, "Key_DOS", "", "" ),
    /* 333: 0x010000d3 (16777427) */ SEnumEntry( Qt::Key_Documents, "Key_Documents", "", "" ),
    /* 334: 0x010000d4 (16777428) */ SEnumEntry( Qt::Key_Excel, "Key_Excel", "", "" ),
    /* 335: 0x010000d5 (16777429) */ SEnumEntry( Qt::Key_Explorer, "Key_Explorer", "", "" ),
    /* 336: 0x010000d6 (16777430) */ SEnumEntry( Qt::Key_Game, "Key_Game", "", "" ),
    /* 337: 0x010000d7 (16777431) */ SEnumEntry( Qt::Key_Go, "Key_Go", "", "" ),
    /* 338: 0x010000d8 (16777432) */ SEnumEntry( Qt::Key_iTouch, "Key_iTouch", "", "" ),
    /* 339: 0x010000d9 (16777433) */ SEnumEntry( Qt::Key_LogOff, "Key_LogOff", "", "" ),
    /* 340: 0x010000da (16777434) */ SEnumEntry( Qt::Key_Market, "Key_Market", "", "" ),
    /* 341: 0x010000db (16777435) */ SEnumEntry( Qt::Key_Meeting, "Key_Meeting", "", "" ),
    /* 342: 0x010000dc (16777436) */ SEnumEntry( Qt::Key_MenuKB, "Key_MenuKB", "", "" ),
    /* 343: 0x010000dd (16777437) */ SEnumEntry( Qt::Key_MenuPB, "Key_MenuPB", "", "" ),
    /* 344: 0x010000de (16777438) */ SEnumEntry( Qt::Key_MySites, "Key_MySites", "", "" ),
    /* 345: 0x010000df (16777439) */ SEnumEntry( Qt::Key_News, "Key_News", "", "" ),
    /* 346: 0x010000e0 (16777440) */ SEnumEntry( Qt::Key_OfficeHome, "Key_OfficeHome", "", "" ),
    /* 347: 0x010000e1 (16777441) */ SEnumEntry( Qt::Key_Option, "Key_Option", "", "" ),
    /* 348: 0x010000e2 (16777442) */ SEnumEntry( Qt::Key_Paste, "Key_Paste", "", "" ),
    /* 349: 0x010000e3 (16777443) */ SEnumEntry( Qt::Key_Phone, "Key_Phone", "", "" ),
    /* 350: 0x010000e4 (16777444) */ SEnumEntry( Qt::Key_Calendar, "Key_Calendar", "", "" ),
    /* 351: 0x010000e5 (16777445) */ SEnumEntry( Qt::Key_Reply, "Key_Reply", "", "" ),
    /* 352: 0x010000e6 (16777446) */ SEnumEntry( Qt::Key_Reload, "Key_Reload", "", "" ),
    /* 353: 0x010000e7 (16777447) */ SEnumEntry( Qt::Key_RotateWindows, "Key_RotateWindows", "", "" ),
    /* 354: 0x010000e8 (16777448) */ SEnumEntry( Qt::Key_RotationPB, "Key_RotationPB", "", "" ),
    /* 355: 0x010000e9 (16777449) */ SEnumEntry( Qt::Key_RotationKB, "Key_RotationKB", "", "" ),
    /* 356: 0x010000ea (16777450) */ SEnumEntry( Qt::Key_Save, "Key_Save", "", "" ),
    /* 357: 0x010000eb (16777451) */ SEnumEntry( Qt::Key_Send, "Key_Send", "", "" ),
    /* 358: 0x010000ec (16777452) */ SEnumEntry( Qt::Key_Spell, "Key_Spell", "", "" ),
    /* 359: 0x010000ed (16777453) */ SEnumEntry( Qt::Key_SplitScreen, "Key_SplitScreen", "", "" ),
    /* 360: 0x010000ee (16777454) */ SEnumEntry( Qt::Key_Support, "Key_Support", "", "" ),
    /* 361: 0x010000ef (16777455) */ SEnumEntry( Qt::Key_TaskPane, "Key_TaskPane", "", "" ),
    /* 362: 0x010000f0 (16777456) */ SEnumEntry( Qt::Key_Terminal, "Key_Terminal", "", "" ),
    /* 363: 0x010000f1 (16777457) */ SEnumEntry( Qt::Key_Tools, "Key_Tools", "", "" ),
    /* 364: 0x010000f2 (16777458) */ SEnumEntry( Qt::Key_Travel, "Key_Travel", "", "" ),
    /* 365: 0x010000f3 (16777459) */ SEnumEntry( Qt::Key_Video, "Key_Video", "", "" ),
    /* 366: 0x010000f4 (16777460) */ SEnumEntry( Qt::Key_Word, "Key_Word", "", "" ),
    /* 367: 0x010000f5 (16777461) */ SEnumEntry( Qt::Key_Xfer, "Key_Xfer", "", "" ),
    /* 368: 0x010000f6 (16777462) */ SEnumEntry( Qt::Key_ZoomIn, "Key_ZoomIn", "", "" ),
    /* 369: 0x010000f7 (16777463) */ SEnumEntry( Qt::Key_ZoomOut, "Key_ZoomOut", "", "" ),
    /* 370: 0x010000f8 (16777464) */ SEnumEntry( Qt::Key_Away, "Key_Away", "", "" ),
    /* 371: 0x010000f9 (16777465) */ SEnumEntry( Qt::Key_Messenger, "Key_Messenger", "", "" ),
    /* 372: 0x010000fa (16777466) */ SEnumEntry( Qt::Key_WebCam, "Key_WebCam", "", "" ),
    /* 373: 0x010000fb (16777467) */ SEnumEntry( Qt::Key_MailForward, "Key_MailForward", "", "" ),
    /* 374: 0x010000fc (16777468) */ SEnumEntry( Qt::Key_Pictures, "Key_Pictures", "", "" ),
    /* 375: 0x010000fd (16777469) */ SEnumEntry( Qt::Key_Music, "Key_Music", "", "" ),
    /* 376: 0x010000fe (16777470) */ SEnumEntry( Qt::Key_Battery, "Key_Battery", "", "" ),
    /* 377: 0x010000ff (16777471) */ SEnumEntry( Qt::Key_Bluetooth, "Key_Bluetooth", "", "" ),
    /* 378: 0x01000100 (16777472) */ SEnumEntry( Qt::Key_WLAN, "Key_WLAN", "", "" ),
    /* 379: 0x01000101 (16777473) */ SEnumEntry( Qt::Key_UWB, "Key_UWB", "", "" ),
    /* 380: 0x01000102 (16777474) */ SEnumEntry( Qt::Key_AudioForward, "Key_AudioForward", "", "" ),
    /* 381: 0x01000103 (16777475) */ SEnumEntry( Qt::Key_AudioRepeat, "Key_AudioRepeat", "", "" ),
    /* 382: 0x01000104 (16777476) */ SEnumEntry( Qt::Key_AudioRandomPlay, "Key_AudioRandomPlay", "", "" ),
    /* 383: 0x01000105 (16777477) */ SEnumEntry( Qt::Key_Subtitle, "Key_Subtitle", "", "" ),
    /* 384: 0x01000106 (16777478) */ SEnumEntry( Qt::Key_AudioCycleTrack, "Key_AudioCycleTrack", "", "" ),
    /* 385: 0x01000107 (16777479) */ SEnumEntry( Qt::Key_Time, "Key_Time", "", "" ),
    /* 386: 0x01000108 (16777480) */ SEnumEntry( Qt::Key_Hibernate, "Key_Hibernate", "", "" ),
    /* 387: 0x01000109 (16777481) */ SEnumEntry( Qt::Key_View, "Key_View", "", "" ),
    /* 388: 0x0100010a (16777482) */ SEnumEntry( Qt::Key_TopMenu, "Key_TopMenu", "", "" ),
    /* 389: 0x0100010b (16777483) */ SEnumEntry( Qt::Key_PowerDown, "Key_PowerDown", "", "" ),
    /* 390: 0x0100010c (16777484) */ SEnumEntry( Qt::Key_Suspend, "Key_Suspend", "", "" ),
    /* 391: 0x0100010d (16777485) */ SEnumEntry( Qt::Key_ContrastAdjust, "Key_ContrastAdjust", "", "" ),
    #if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
    /* 392: 0x0100010e (16777486) */ SEnumEntry( Qt::Key_LaunchG, "Key_LaunchG", "", "" ),
    /* 393: 0x0100010f (16777487) */ SEnumEntry( Qt::Key_LaunchH, "Key_LaunchH", "", "" ),
    #endif
    /* 394: 0x0100ffff (16842751) */ SEnumEntry( Qt::Key_MediaLast, "Key_MediaLast", "", "" ),
    /* 395: 0x01010000 (16842752) */ SEnumEntry( Qt::Key_Select, "Key_Select", "", "" ),
    /* 396: 0x01010001 (16842753) */ SEnumEntry( Qt::Key_Yes, "Key_Yes", "", "" ),
    /* 397: 0x01010002 (16842754) */ SEnumEntry( Qt::Key_No, "Key_No", "", "" ),
    /* 398: 0x01020001 (16908289) */ SEnumEntry( Qt::Key_Cancel, "Key_Cancel", "", "" ),
    /* 399: 0x01020002 (16908290) */ SEnumEntry( Qt::Key_Printer, "Key_Printer", "", "" ),
    /* 401: 0x01020003 (16908291) */ SEnumEntry( Qt::Key_Execute, "Key_Execute", "", "" ),
    /* 402: 0x01020004 (16908292) */ SEnumEntry( Qt::Key_Sleep, "Key_Sleep", "", "" ),
    /* 403: 0x01020005 (16908293) */ SEnumEntry( Qt::Key_Play, "Key_Play", "", "" ),
    /* 404: 0x01020006 (16908294) */ SEnumEntry( Qt::Key_Zoom, "Key_Zoom", "", "" ),
    /* 405: 0x01100000 (17825792) */ SEnumEntry( Qt::Key_Context1, "Key_Context1", "", "" ),
    /* 406: 0x01100001 (17825793) */ SEnumEntry( Qt::Key_Context2, "Key_Context2", "", "" ),
    /* 407: 0x01100002 (17825794) */ SEnumEntry( Qt::Key_Context3, "Key_Context3", "", "" ),
    /* 408: 0x01100003 (17825795) */ SEnumEntry( Qt::Key_Context4, "Key_Context4", "", "" ),
    /* 409: 0x01100004 (17825796) */ SEnumEntry( Qt::Key_Call, "Key_Call", "", "" ),
    /* 410: 0x01100005 (17825797) */ SEnumEntry( Qt::Key_Hangup, "Key_Hangup", "", "" ),
    /* 411: 0x01100006 (17825798) */ SEnumEntry( Qt::Key_Flip, "Key_Flip", "", "" ),
    #if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
    /* 412: 0x01100007 (17825799) */ SEnumEntry( Qt::Key_ToggleCallHangup, "Key_ToggleCallHangup", "", "" ),
    /* 413: 0x01100008 (17825800) */ SEnumEntry( Qt::Key_VoiceDial, "Key_VoiceDial", "", "" ),
    /* 414: 0x01100009 (17825801) */ SEnumEntry( Qt::Key_LastNumberRedial, "Key_LastNumberRedial", "", "" ),
    /* 415: 0x01100020 (17825824) */ SEnumEntry( Qt::Key_Camera, "Key_Camera", "", "" ),
    /* 416: 0x01100021 (17825825) */ SEnumEntry( Qt::Key_CameraFocus, "Key_CameraFocus", "", "" ),
    #endif
    /* 417: 0x01ffffff (33554431) */ SEnumEntry( Qt::Key_unknown, "Key_unknown" )
};

//------------------------------------------------------------------------------
QString ZS::System::qKeyCode2Str( int i_key, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    static bool s_bHshKeysInit = false;
    static QHash<quint32,int> s_hshKeys;

    if( !s_bHshKeysInit )
    {
        int     idx;
        quint32 uHshCode;
        int     iHshVal;

        for( idx = 0; idx < _ZSArrLen(s_arEnumStrQKeyCode); idx++ )
        {
            // Not the variant value has been initialized with the key code but the enum index.
            //uHshCode  = static_cast<quint32>(s_arEnumStrQKeyCode[idx].m_val.toULongLong());
            uHshCode = static_cast<quint32>(s_arEnumStrQKeyCode[idx].m_iEnumerator);
            iHshVal  = idx;

            if( !s_hshKeys.contains(uHshCode) )
            {
                s_hshKeys[uHshCode] = iHshVal;
            }
        }

        s_bHshKeysInit = true;

    } // if( !s_bHshKeysInit )

    QString str;
    int     idx;

    if( s_hshKeys.contains(i_key) )
    {
        idx = s_hshKeys[i_key];

        if( i_alias < s_arEnumStrQKeyCode[idx].m_strlstNames.size() )
        {
            str = s_arEnumStrQKeyCode[idx].m_strlstNames[i_alias];
        }
        else
        {
            str = s_arEnumStrQKeyCode[idx].m_strlstNames[0];
        }
    }
    else
    {
        str = "Undefined (" + QString::number(i_key) + ", 0x" + QString::number(i_key,16) + ")";
    }

    return str;

} // qKeyCode2Str

//------------------------------------------------------------------------------
int ZS::System::char2QKeyCode( const QChar& i_ch )
//------------------------------------------------------------------------------
{
    static bool s_bHshKeysInit = false;
    static QHash<QChar,int> s_hshKeys;

    if( !s_bHshKeysInit )
    {
        int   idx;
        QChar chHshCode;

        for( idx = 0; idx < _ZSArrLen(s_arEnumStrQKeyCode); idx++ )
        {
            // Not the variant value has been initialized with the key code but the enum index.
            //uHshCode  = static_cast<quint32>(s_arEnumStrQKeyCode[idx].m_val.toULongLong());
            if( s_arEnumStrQKeyCode[idx].m_strlstNames.size() > EEnumEntryAliasStrSymbol )
            {
                if( s_arEnumStrQKeyCode[idx].m_strlstNames[EEnumEntryAliasStrSymbol].length() == 1 )
                {
                    chHshCode = s_arEnumStrQKeyCode[idx].m_strlstNames[EEnumEntryAliasStrSymbol][0];

                    if( !s_hshKeys.contains(chHshCode) )
                    {
                        s_hshKeys[chHshCode] = idx;
                    }
                }
            }
        }

        s_bHshKeysInit = true;

    } // if( !s_bHshKeysInit )

    int iKeyCode = Qt::Key_unknown;

    if( s_hshKeys.contains(i_ch) )
    {
        int idx = s_hshKeys[i_ch];
        iKeyCode = s_arEnumStrQKeyCode[idx].m_iEnumerator;
    }

    return iKeyCode;

} // char2QKeyCode

//------------------------------------------------------------------------------
int ZS::System::str2QKeyCode( const QString& i_str )
//------------------------------------------------------------------------------
{
    static bool s_bHshKeysInit = false;
    static QHash<QString,int> s_hshKeys;

    if( !s_bHshKeysInit ) {
        for( int idx = 0; idx < _ZSArrLen(s_arEnumStrQKeyCode); idx++ ) {
            // Not the variant value has been initialized with the key code but the enum index.
            //uHshCode  = static_cast<quint32>(s_arEnumStrQKeyCode[idx].m_val.toULongLong());
            if( s_arEnumStrQKeyCode[idx].m_strlstNames.size() > EEnumEntryAliasStrText ) {
                if( !s_arEnumStrQKeyCode[idx].m_strlstNames[EEnumEntryAliasStrText].isEmpty() ) {
                    QString strHshCode = s_arEnumStrQKeyCode[idx].m_strlstNames[EEnumEntryAliasStrText];
                    if( !s_hshKeys.contains(strHshCode) ) {
                        s_hshKeys[strHshCode] = idx;
                    }
                }
            }
        }
        s_bHshKeysInit = true;
    }

    int iKeyCode = Qt::Key_unknown;
    if( s_hshKeys.contains(i_str) ) {
        int idx = s_hshKeys[i_str];
        iKeyCode = s_arEnumStrQKeyCode[idx].m_iEnumerator;
    }
    return iKeyCode;
}

/*==============================================================================
Bitfield KeyboardModifier
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::qKeyboardModifiers2Str( quint32 i_modifiers )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_modifiers & Qt::ShiftModifier )
    {
        if (!str.isEmpty()) str += "|";
        str = "Shift";
    }
    if( i_modifiers & Qt::ControlModifier )
    {
        if (!str.isEmpty()) str += "|";
        str += "Control";
    }
    if( i_modifiers & Qt::AltModifier )
    {
        if (!str.isEmpty()) str += "|";
        str += "Alt";
    }
    if( i_modifiers & Qt::MetaModifier )
    {
        if (!str.isEmpty()) str += "|";
        str += "Meta";
    }
    if( i_modifiers & Qt::KeypadModifier )
    {
        if (!str.isEmpty()) str += "|";
        str += "Keypad";
    }
    if( i_modifiers & Qt::GroupSwitchModifier )
    {
        if (!str.isEmpty()) str += "|";
        str += "GroupSwitch";
    }
    return str;

} // qKeyboardModifiers2Str

//------------------------------------------------------------------------------
Qt::KeyboardModifiers ZS::System::char2QKeyboardModifiers( const QChar& i_ch )
//------------------------------------------------------------------------------
{
    Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
    QChar ch = i_ch;
    if (ch.toLower() != i_ch) {
        keyboardModifiers |= Qt::ShiftModifier;
    }
    return keyboardModifiers;
}

//------------------------------------------------------------------------------
QString ZS::System::qMargins2Str( const QMargins& i_margins )
//------------------------------------------------------------------------------
{
    QString str = QString::number(i_margins.left()) + ", " + QString::number(i_margins.top())
        + ", " + QString::number(i_margins.right()) + ", " + QString::number(i_margins.bottom());
    return str;
}

//------------------------------------------------------------------------------
QString ZS::System::qMimeData2Str( const QMimeData* i_pMimeData, int i_iDetailLevel )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_pMimeData == nullptr ) {
        str = "nullptr";
    }
    else {
        QStringList strlstFormats = i_pMimeData->formats();
        str = "Formats [" + QString::number(strlstFormats.size()) + "]";
        if( strlstFormats.size() > 0 ) {
            str += "(";
            for( int idx = 0; idx < strlstFormats.size(); idx++ ) {
                if( idx > 0 ) {
                    str += ", " + strlstFormats[idx];
                }
                else {
                    str += strlstFormats[idx];
                }
                if( i_iDetailLevel > 0 ) {
                    str += " {";
                    QByteArray  byteArr = i_pMimeData->data(strlstFormats[idx]);
                    QDataStream stream(&byteArr, QIODevice::ReadOnly);
                    QStringList strlstItems;

                    while( !stream.atEnd() ) {
                        QString strItem;
                        stream >> strItem;
                        strlstItems << strItem;
                    }
                    str += strlstItems.join(", ");
                    str += "}";
                }
            }
            str += ")";
        }
        if( i_pMimeData->hasUrls() ) {
            str += ", Urls [" + QString::number(i_pMimeData->urls().size()) + "]";
            if( i_pMimeData->urls().size() > 0 ) {
                str += "(";
                for( int idx = 0; idx < i_pMimeData->urls().size(); idx++ ) {
                    if( idx > 0 ) {
                        str += ", ";
                    }
                    str += i_pMimeData->urls()[idx].toString();
                }
                str += ")";
            }
        }
        if( i_pMimeData->hasText() ) {
            str += ", Text: " + i_pMimeData->text();
        }
        if( i_pMimeData->hasHtml() ) {
            str += ", Html: " + i_pMimeData->html();
        }
        if( i_pMimeData->hasImage() ) {
            //QVariant i_pMimeData->imageData();
        }
        if( i_pMimeData->hasColor() ) {
            //QVariant i_pMimeData->colorData();
        }
    } // if( i_pMimeData != nullptr )

    return str;

} // qMimeData2Str

//------------------------------------------------------------------------------
QString ZS::System::qModelIndex2Str( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString str;
    if( !i_modelIdx.isValid() )
    {
        str = "Invalid";
    }
    else
    {
        str = "Row: " + QString::number(i_modelIdx.row());
        str += ", Clm: " + QString::number(i_modelIdx.column());

        // Endless recursion if called by ::data method.
        //if( i_modelIdx.data().canConvert(QVariant::String) )
        //{
        //    str += ", Data: " + i_modelIdx.data().toString();
        //}
    }
    return str;

} // qModelIndex2Str

//------------------------------------------------------------------------------
QString ZS::System::qMouseButton2Str( int i_iBtn )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_iBtn == Qt::MouseButton::NoButton )
    {
        str = "NoButton";
    }
    else if( i_iBtn == Qt::MouseButton::LeftButton )
    {
        str = "LeftButton";
    }
    else if( i_iBtn == Qt::MouseButton::RightButton )
    {
        str = "RightButton";
    }
    else if( i_iBtn == Qt::MouseButton::MiddleButton )
    {
        str = "MiddleButton";
    }
    #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    else if( i_iBtn == Qt::MouseButton::BackButton )
    {
        str = "BackButton";
    }
    #endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    else if( i_iBtn == Qt::MouseButton::XButton1 )
    {
        str = "XButton1";
    }
    #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    else if( i_iBtn == Qt::MouseButton::ExtraButton1 )
    {
        str = "ExtraButton1";
    }
    else if( i_iBtn == Qt::MouseButton::ForwardButton )
    {
        str = "ForwardButton";
    }
    #endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    else if( i_iBtn == Qt::MouseButton::XButton2 )
    {
        str = "XButton2";
    }
    #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    else if( i_iBtn == Qt::MouseButton::ExtraButton2 )
    {
        str = "ExtraButton2";
    }
    else if( i_iBtn == Qt::MouseButton::TaskButton )
    {
        str = "TaskButton";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton3 )
    {
        str = "ExtraButton3";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton4 )
    {
        str = "ExtraButton4";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton5 )
    {
        str = "ExtraButton5";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton6 )
    {
        str = "ExtraButton6";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton7 )
    {
        str = "ExtraButton7";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton8 )
    {
        str = "ExtraButton8";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton9 )
    {
        str = "ExtraButton9";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton10 )
    {
        str = "ExtraButton10";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton11 )
    {
        str = "ExtraButton11";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton12 )
    {
        str = "ExtraButton12";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton13 )
    {
        str = "ExtraButton13";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton14 )
    {
        str = "ExtraButton14";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton15 )
    {
        str = "ExtraButton15";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton16 )
    {
        str = "ExtraButton16";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton17 )
    {
        str = "ExtraButton17";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton18 )
    {
        str = "ExtraButton18";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton19 )
    {
        str = "extrabutton19";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton20 )
    {
        str = "ExtraButton20";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton21 )
    {
        str = "ExtraButton21";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton22 )
    {
        str = "ExtraButton22";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton23 )
    {
        str = "ExtraButton23";
    }
    else if( i_iBtn == Qt::MouseButton::ExtraButton24 )
    {
        str = "ExtraButton24";
    }
    else if( i_iBtn == Qt::MouseButton::AllButtons )
    {
        str = "AllButtons";
    }
    #endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)

    return str;

} // qMouseButton2Str

//------------------------------------------------------------------------------
QString ZS::System::qMouseButtons2Str( int i_iBtns )
//------------------------------------------------------------------------------
{
    QString str;

    if (i_iBtns & Qt::MouseButton::NoButton) {
        if (!str.isEmpty()) str += "|";
        str += "NoButton";
    }
    if( i_iBtns & Qt::MouseButton::LeftButton ) {
        if (!str.isEmpty()) str += "|";
        str += "LeftButton";
    }
    if( i_iBtns & Qt::MouseButton::RightButton ) {
        if (!str.isEmpty()) str += "|";
        str += "RightButton";
    }
    if( i_iBtns & Qt::MouseButton::MiddleButton ) {
        if (!str.isEmpty()) str += "|";
        str += "MiddleButton";
    }
    #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    if( i_iBtns & Qt::MouseButton::BackButton ) {
        if (!str.isEmpty()) str += "|";
        str += "BackButton";
    }
    #endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    if( i_iBtns & Qt::MouseButton::XButton1 ) {
        if (!str.isEmpty()) str += "|";
        str += "XButton1";
    }
    #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    if( i_iBtns & Qt::MouseButton::ExtraButton1 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton1";
    }
    if( i_iBtns & Qt::MouseButton::ForwardButton ) {
        if (!str.isEmpty()) str += "|";
        str += "ForwardButton";
    }
    #endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    if( i_iBtns & Qt::MouseButton::XButton2 ) {
        if (!str.isEmpty()) str += "|";
        str += "XButton2";
    }
    #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    if( i_iBtns & Qt::MouseButton::ExtraButton2 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton2";
    }
    if( i_iBtns & Qt::MouseButton::TaskButton ) {
        if (!str.isEmpty()) str += "|";
        str += "TaskButton";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton3 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton3";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton4 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton4";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton5 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton5";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton6 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton6";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton7 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton7";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton8 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton8";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton9 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton9";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton10 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton10";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton11 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton11";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton12 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton12";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton13 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton13";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton14 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton14";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton15 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton15";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton16 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton16";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton17 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton17";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton18 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton18";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton19 ) {
        if (!str.isEmpty()) str += "|";
        str += "extrabutton19";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton20 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton20";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton21 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton21";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton22 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton22";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton23 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton23";
    }
    if( i_iBtns & Qt::MouseButton::ExtraButton24 ) {
        if (!str.isEmpty()) str += "|";
        str += "ExtraButton24";
    }
    if( i_iBtns & Qt::MouseButton::AllButtons ) {
        if (!str.isEmpty()) str += "|";
        str += "AllButtons";
    }
    #endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)

    return str;

} // qMouseButtons2Str

//------------------------------------------------------------------------------
QString ZS::System::qPenStyle2Str( const Qt::PenStyle& i_penStyle )
//------------------------------------------------------------------------------
{
    QString str;
    if( i_penStyle == Qt::NoPen ) {
        str = "NoPen";
    } else if( i_penStyle == Qt::SolidLine ) {
        str = "Solid";
    } else if( i_penStyle == Qt::DashLine ) {
        str = "Dash";
    } else if( i_penStyle == Qt::DotLine ) {
        str = "Dot";
    } else if( i_penStyle == Qt::DashDotLine ) {
        str = "DashDot";
    } else if( i_penStyle == Qt::DashDotDotLine ) {
        str = "DashDotDot";
    } else if( i_penStyle == Qt::CustomDashLine ) {
        str = "CustomDash";
    } else {
        str = "? (" + QString::number(i_penStyle) + ")";
    }
    return str;
}

//------------------------------------------------------------------------------
QString ZS::System::qPoint2Str( const QPoint& i_pt )
//------------------------------------------------------------------------------
{
    return QString::number(i_pt.x()) + ", " + QString::number(i_pt.y());
}

//------------------------------------------------------------------------------
QString ZS::System::qPoint2Str( const QPointF& i_pt )
//------------------------------------------------------------------------------
{
    return QString::number(i_pt.x(),'f',1) + ", " + QString::number(i_pt.y(),'f',1);
}

//------------------------------------------------------------------------------
QString ZS::System::qSize2Str( const QSize& i_size )
//------------------------------------------------------------------------------
{
    return QString::number(i_size.width()) + ", " + QString::number(i_size.height());
}

//------------------------------------------------------------------------------
QString ZS::System::qSize2Str( const QSizeF& i_size )
//------------------------------------------------------------------------------
{
    return QString::number(i_size.width(),'f',1) + ", " + QString::number(i_size.height(),'f',1);
}

//------------------------------------------------------------------------------
QString ZS::System::qLine2Str( const QLine& i_line )
//------------------------------------------------------------------------------
{
    return "P1 {" + qPoint2Str(i_line.p1()) + "}, P2 {" + qPoint2Str(i_line.p2()) + "}";
}

//------------------------------------------------------------------------------
QString ZS::System::qLine2Str( const QLineF& i_line )
//------------------------------------------------------------------------------
{
    return "P1 {" + qPoint2Str(i_line.p1()) + "}, P2 {" + qPoint2Str(i_line.p2()) + "}";
}

//------------------------------------------------------------------------------
QString ZS::System::qRect2Str( const QRect& i_rct )
//------------------------------------------------------------------------------
{
    return QString::number(i_rct.x())
        + ", " + QString::number(i_rct.y())
        + ", " + QString::number(i_rct.width())
        + ", " + QString::number(i_rct.height());
}

//------------------------------------------------------------------------------
QString ZS::System::qRect2Str( const QRectF& i_rct )
//------------------------------------------------------------------------------
{
    return QString::number(i_rct.x(),'f',1)
        + ", " + QString::number(i_rct.y(),'f',1)
        + ", " + QString::number(i_rct.width(),'f',1)
        + ", " + QString::number(i_rct.height(),'f',1);
}

/*==============================================================================
Enum QVariant::Type
==============================================================================*/

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//------------------------------------------------------------------------------
static const QHash<int, QString> s_hshEnumStrVariantTypes =
//------------------------------------------------------------------------------
{
    { QVariant::Invalid, "Invalid" },
    { QVariant::Bool, "Bool" },
    { QVariant::Int, "Int" },
    { QVariant::UInt, "UInt" },
    { QVariant::LongLong, "LongLong" },
    { QVariant::ULongLong, "ULongLong" },
    { QVariant::Double, "Double" },
    { QVariant::Char, "Char" },
    { QVariant::Map, "Map" },
    { QVariant::List, "List" },
    { QVariant::String, "String" },
    { QVariant::StringList, "StringList" },
    { QVariant::ByteArray, "ByteArray" },
    { QVariant::BitArray, "BitArray" },
    { QVariant::Date, "Date" },
    { QVariant::Time, "Time" },
    { QVariant::DateTime, "DateTime" },
    { QVariant::Url, "Url" },
    { QVariant::Locale, "Locale" },
    { QVariant::Rect, "Rect" },
    { QVariant::RectF, "RectF" },
    { QVariant::Size, "Size" },
    { QVariant::SizeF, "SizeF" },
    { QVariant::Line, "Line" },
    { QVariant::LineF, "LineF" },
    { QVariant::Point, "Point" },
    { QVariant::PointF, "PointF" },
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    { QVariant::RegExp, "RegExp" },
    #else
    { QVariant::RegularExpression, "RegExp" },
    #endif
    { QVariant::Hash, "Hash" },
    #if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
    { QVariant::EasingCurve, "EasingCurve" },
    #endif
    #ifdef QT3_SUPPORT
    { QVariant::ColorGroup, "ColorGroup" },
    #endif
    { QVariant::Font, "Font" },
    { QVariant::Pixmap, "Pixmap" },
    { QVariant::Brush, "Brush" },
    { QVariant::Color, "Color" },
    { QVariant::Palette, "Palette" },
    { QVariant::Icon, "Icon" },
    { QVariant::Image, "Image" },
    { QVariant::Polygon, "Polygon" },
    { QVariant::Region, "Region" },
    { QVariant::Bitmap, "Bitmap" },
    { QVariant::Cursor, "Cursor" },
    { QVariant::SizePolicy, "SizePolicy" },
    { QVariant::KeySequence, "KeySequence" },
    { QVariant::Pen, "Pen" },
    { QVariant::TextLength, "TextLength" },
    { QVariant::TextFormat, "TextFormat" },
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    { QVariant::Matrix, "Matrix" },
    #endif
    { QVariant::Transform, "Transform" },
    { QVariant::Matrix4x4, "Matrix4x4" },
    { QVariant::Vector2D, "Vector2D" },
    { QVariant::Vector3D, "Vector3D" },
    { QVariant::Vector4D, "Vector4D" },
    { QVariant::Quaternion, "Quaternion" },
    { QVariant::UserType, "UserType" }
};
#else
//------------------------------------------------------------------------------
static const QHash<int, QString> s_hshEnumStrVariantTypes =
//------------------------------------------------------------------------------
{
    { QMetaType::UnknownType, "UnknownType" },
    { QMetaType::Bool, "Bool" },
    { QMetaType::Int, "Int" },
    { QMetaType::UInt, "UInt" },
    { QMetaType::LongLong, "LongLong" },
    { QMetaType::ULongLong, "ULongLong" },
    { QMetaType::Double, "Double" },
    { QMetaType::Char, "Char" },
    { QMetaType::QString, "String" },
    { QMetaType::QStringList, "StringList" },
    { QMetaType::QByteArray, "ByteArray" },
    { QMetaType::QBitArray, "BitArray" },
    { QMetaType::QDate, "Date" },
    { QMetaType::QTime, "Time" },
    { QMetaType::QDateTime, "DateTime" },
    { QMetaType::QUrl, "Url" },
    { QMetaType::QLocale, "Locale" },
    { QMetaType::QRect, "Rect" },
    { QMetaType::QRectF, "RectF" },
    { QMetaType::QSize, "Size" },
    { QMetaType::QSizeF, "SizeF" },
    { QMetaType::QLine, "Line" },
    { QMetaType::QLineF, "LineF" },
    { QMetaType::QPoint, "Point" },
    { QMetaType::QPointF, "PointF" },
    { QMetaType::QRegularExpression, "RegExp" },
    { QMetaType::QEasingCurve, "EasingCurve" },
    { QMetaType::QFont, "Font" },
    { QMetaType::QPixmap, "Pixmap" },
    { QMetaType::QBrush, "Brush" },
    { QMetaType::QColor, "Color" },
    { QMetaType::QPalette, "Palette" },
    { QMetaType::QIcon, "Icon" },
    { QMetaType::QImage, "Image" },
    { QMetaType::QPolygon, "Polygon" },
    { QMetaType::QRegion, "Region" },
    { QMetaType::QBitmap, "Bitmap" },
    { QMetaType::QCursor, "Cursor" },
    { QMetaType::QSizePolicy, "SizePolicy" },
    { QMetaType::QKeySequence, "KeySequence" },
    { QMetaType::QPen, "Pen" },
    { QMetaType::QTextLength, "TextLength" },
    { QMetaType::QTextFormat, "TextFormat" },
    { QMetaType::QTransform, "Transform" },
    { QMetaType::QMatrix4x4, "Matrix4x4" },
    { QMetaType::QVector2D, "Vector2D" },
    { QMetaType::QVector3D, "Vector3D" },
    { QMetaType::QVector4D, "Vector4D" },
    { QMetaType::QQuaternion, "Quaternion" }
};
#endif

//------------------------------------------------------------------------------
QString ZS::System::qVariantType2Str( int i_type )
//------------------------------------------------------------------------------
{
    QString str = s_hshEnumStrVariantTypes.value(i_type, "");
    if( str.isEmpty() ) str = "? (=" + QString::number(i_type) + ")";
    return str;
}


/*******************************************************************************
Creating strings
*******************************************************************************/

//------------------------------------------------------------------------------
QString ZS::System::buildPathStr(
    const QString& i_strSeparator,
    const QString& i_str1,
    const QString& i_str2 )
//------------------------------------------------------------------------------
{
    return buildPathStr(i_strSeparator, i_str1, i_str2, "", "", "");
}

//------------------------------------------------------------------------------
QString ZS::System::buildPathStr(
    const QString& i_strSeparator,
    const QString& i_str1,
    const QString& i_str2,
    const QString& i_str3 )
//------------------------------------------------------------------------------
{
    return buildPathStr(i_strSeparator, i_str1, i_str2, i_str3, "", "");
}

//------------------------------------------------------------------------------
QString ZS::System::buildPathStr(
    const QString& i_strSeparator,
    const QString& i_str1,
    const QString& i_str2,
    const QString& i_str3,
    const QString& i_str4 )
//------------------------------------------------------------------------------
{
    return buildPathStr(i_strSeparator, i_str1, i_str2, i_str3, i_str4, "");
}

//------------------------------------------------------------------------------
QString ZS::System::buildPathStr(
    const QString& i_strSeparator,
    const QString& i_str1,
    const QString& i_str2,
    const QString& i_str3,
    const QString& i_str4,
    const QString& i_str5 )
//------------------------------------------------------------------------------
{
    QString strPath = i_str1;

    if( !strPath.isEmpty() && !i_str2.isEmpty() )
    {
        strPath += i_strSeparator;
    }
    if( !i_str2.isEmpty() )
    {
        strPath += i_str2;
    }
    if( !strPath.isEmpty() && !i_str3.isEmpty() )
    {
        strPath += i_strSeparator;
    }
    if( !i_str3.isEmpty() )
    {
        strPath += i_str3;
    }
    if( !strPath.isEmpty() && !i_str4.isEmpty() )
    {
        strPath += i_strSeparator;
    }
    if( !i_str4.isEmpty() )
    {
        strPath += i_str4;
    }
    if( !strPath.isEmpty() && !i_str5.isEmpty() )
    {
        strPath += i_strSeparator;
    }
    if( !i_str5.isEmpty() )
    {
        strPath += i_str5;
    }
    return strPath;

} // buildPathStr

//------------------------------------------------------------------------------
QString ZS::System::truncateStringWithEllipsisInTheMiddle(
    const QString& i_str,
    int            i_iMaxLength,
    const QString& i_strEllipsis )
//------------------------------------------------------------------------------
{
    QString str = i_str;

    if( i_iMaxLength > 0 && i_str.length() > i_iMaxLength )
    {
        if( i_strEllipsis.length() >= i_iMaxLength )
        {
            str = i_str.left(i_iMaxLength-1) + i_strEllipsis[0];
        }
        else
        {
            int iStrLen = i_iMaxLength - i_strEllipsis.length();

            int iStrLenRight = iStrLen/2;
            int iStrLenLeft  = iStrLen - iStrLenRight;
            str = i_str.left(iStrLenLeft) + i_strEllipsis + i_str.right(iStrLenRight);
        }
    }

    return str;

} // truncateStringWithEllipsisInTheMiddle


//------------------------------------------------------------------------------
QString ZS::System::encodeForXml( const QString& i_str )
//------------------------------------------------------------------------------
{
    QString str = i_str;
    if( str.contains("&") ) str.replace("&", "&amp;"); // this must be the first replacement
    if( str.contains("<") ) str.replace("<","&lt;");
    if( str.contains(">") ) str.replace(">","&gt;");
    if( str.contains("\"") ) str.replace("\"", "&quot;");
    if( str.contains("'") ) str.replace("'", "&apos;");
    return str;
}

//------------------------------------------------------------------------------
QString ZS::System::encodeForHtml( const QString& i_str )
//------------------------------------------------------------------------------
{
    QString str = i_str;
    if( str.contains("&") ) str.replace("&", "&amp;"); // this must be the first replacement
    if( str.contains("<") ) str.replace("<","&lt;");
    if( str.contains(">") ) str.replace(">","&gt;");
    if( str.contains("\"") ) str.replace("\"", "&quot;");
    if( str.contains("'") ) str.replace("'", "&apos;");
    if( str.contains("  ") ) str.replace("  ", "&nbsp;");
    return str;
}

//------------------------------------------------------------------------------
QString ZS::System::decodeFromHtml( const QString& i_str )
//------------------------------------------------------------------------------
{
    QString str = i_str;

    if( str.contains("&lt;") ) str.replace("&lt;", "<");
    if( str.contains("&gt;") ) str.replace("&gt;", ">");
    if( str.contains("&quot;") ) str.replace("&quot;", "\"");
    if( str.contains("&apos;") ) str.replace("&apos;", "'");
    if( str.contains("&amp;") ) str.replace("&amp;", "&");
    if( str.contains("&nbsp;") ) str.replace("&nbsp;", " ");
    return str;
}

/*******************************************************************************
Parsing strings
*******************************************************************************/

static const QChar s_archWhiteSpace[]    = { ' ', '\t' };
static const QChar s_archDigitChar[]     = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
static const QChar s_archSign[]          = { '-', '+' };
static const QChar s_archDecimalPoint[]  = { '.' };
static const QChar s_archDivisionPoint[] = { ',' };
static const QChar s_archExponentChar[]  = { 'e', 'E' };

//------------------------------------------------------------------------------
bool ZS::System::isWhiteSpace( const QChar& i_ch )
//------------------------------------------------------------------------------
{
    for( int iIdx = 0; iIdx < static_cast<int>(_ZSArrLen(s_archWhiteSpace)); iIdx++ )
    {
        if( i_ch == s_archWhiteSpace[iIdx] )
        {
            return true;
        }
    }
    return false;

} // isWhiteSpace

//------------------------------------------------------------------------------
bool ZS::System::isDigitChar( const QChar& i_ch )
//------------------------------------------------------------------------------
{
    for( int iIdx = 0; iIdx < static_cast<int>(_ZSArrLen(s_archDigitChar)); iIdx++ )
    {
        if( i_ch == s_archDigitChar[iIdx] )
        {
            return true;
        }
    }
    return false;

} // isDigitChar

//------------------------------------------------------------------------------
bool ZS::System::isSign( const QChar& i_ch )
//------------------------------------------------------------------------------
{
    for( int iIdx = 0; iIdx < static_cast<int>(_ZSArrLen(s_archSign)); iIdx++ )
    {
        if( i_ch == s_archSign[iIdx] )
        {
            return true;
        }
    }
    return false;

} // isSign

//------------------------------------------------------------------------------
bool ZS::System::isDecimalPoint( const QChar& i_ch )
//------------------------------------------------------------------------------
{
    for( int iIdx = 0; iIdx < static_cast<int>(_ZSArrLen(s_archDecimalPoint)); iIdx++ )
    {
        if( i_ch == s_archDecimalPoint[iIdx] )
        {
            return true;
        }
    }
    return false;

} // isDecimalPoint

//------------------------------------------------------------------------------
bool ZS::System::isDivisionPoint( const QChar& i_ch )
//------------------------------------------------------------------------------
{
    for( int iIdx = 0; iIdx < static_cast<int>(_ZSArrLen(s_archDivisionPoint)); iIdx++ )
    {
        if( i_ch == s_archDivisionPoint[iIdx] )
        {
            return true;
        }
    }
    return false;

} // isDivisionPoint

//------------------------------------------------------------------------------
bool ZS::System::isExponentChar( const QChar& i_ch )
//------------------------------------------------------------------------------
{
    for( int iIdx = 0; iIdx < static_cast<int>(_ZSArrLen(s_archExponentChar)); iIdx++ )
    {
        if( i_ch == s_archExponentChar[iIdx] )
        {
            return true;
        }
    }
    return false;

} // isExponentChar

//------------------------------------------------------------------------------
bool ZS::System::isNumChar( const QChar& i_ch )
//------------------------------------------------------------------------------
{
    if( isSign(i_ch) )
    {
        return true;
    }
    if( isDecimalPoint(i_ch) )
    {
        return true;
    }
    if( isDigitChar(i_ch) )
    {
        return true;
    }
    if( isExponentChar(i_ch) )
    {
        return true;
    }
    if( isDivisionPoint(i_ch) )
    {
        return true;
    }
    return false;

} // isNumChar

//------------------------------------------------------------------------------
void ZS::System::insertDelimiter(
    int            i_iDigitsPerDigitGroup,
    const QString& i_strDelimiter,
    QString*       io_pstrValue,
    int            i_iDigitsLeading,
    int            i_iDigitsTrailing )
//------------------------------------------------------------------------------
{
    int iDelimiterCount;
    int idxLeadingDelimiterPos;
    int idxTrailingDelimiterPos;
    int idx;

    if( i_iDigitsPerDigitGroup > 0 )
    {
        idxLeadingDelimiterPos  = i_iDigitsLeading-i_iDigitsPerDigitGroup;
        idxTrailingDelimiterPos = i_iDigitsLeading+i_iDigitsPerDigitGroup+1;

        // There might be a "-" or "+" sign at the beginning of the string ...
        if( i_iDigitsLeading > 0 )
        {
            idx = 0;
            while( ((*io_pstrValue)[idx] < '0' || (*io_pstrValue)[idx] > '9') && (idx < static_cast<int>(io_pstrValue->length())) )
            {
                idx++;
                idxLeadingDelimiterPos++;
                idxTrailingDelimiterPos++;
            }
        }

        // Insert delimiters into leading part:
        if( i_iDigitsLeading > i_iDigitsPerDigitGroup )
        {
            iDelimiterCount = (i_iDigitsLeading-1)/i_iDigitsPerDigitGroup;
            for( idx = 0; idx < iDelimiterCount; idx++ )
            {
                io_pstrValue->insert(idxLeadingDelimiterPos,i_strDelimiter);
                idxTrailingDelimiterPos++;
                idxLeadingDelimiterPos -= i_iDigitsPerDigitGroup;
            }
        }

        // Insert delimiters into trailing part:
        if( i_iDigitsTrailing > i_iDigitsPerDigitGroup )
        {
            iDelimiterCount = (i_iDigitsTrailing-1)/i_iDigitsPerDigitGroup;
            for( idx = 0; idx < iDelimiterCount; idx++ )
            {
                io_pstrValue->insert(idxTrailingDelimiterPos,i_strDelimiter);
                idxTrailingDelimiterPos++;
                idxTrailingDelimiterPos += i_iDigitsPerDigitGroup;
            }
        }
    }
} // insertDelimiter

//------------------------------------------------------------------------------
void ZS::System::removeTrailingZeros(
    QString* io_pstrValue,
    int      i_iDigitsTrailingMin,
    QChar    i_charDecPoint )
//------------------------------------------------------------------------------
{
    // String:               "123.45600e-12"
    // idxChar:               0123456789012
    // idxCharExp          =  9
    // idxCharDecPt        =  3
    // uStrLen             = 13
    // iMantissaSubStrLen  =  9
    // iExponentSubStrLen  =  4
    // uTrailingSubStrLen  =  5
    // uLeadingSubStrLen   =  3
    // iTrailingZeros      =  2

    QString strValue = *io_pstrValue;
    int     idxChar;
    int     idxCharDecPt;
    int     iMantissaSubStrLen = 0;
    int     iExponentSubStrLen = 0;
    int     iTrailingZeros = 0;

    if( strValue.length() > 0 )
    {
        idxCharDecPt = strValue.indexOf(i_charDecPoint,0,Qt::CaseInsensitive);

        if( idxCharDecPt >= 0 )
        {
            idxChar = strValue.indexOf('e',-1,Qt::CaseInsensitive);
            if( idxChar >= 0 )
            {
                iExponentSubStrLen = strValue.length()-idxChar;
            }
            iMantissaSubStrLen = strValue.length() - iExponentSubStrLen;

            if( iMantissaSubStrLen > 0 )
            {
                for( idxChar = iMantissaSubStrLen-1; idxChar >= 0; idxChar-- )
                {
                    if( strValue[idxChar] == i_charDecPoint )
                    {
                        if( i_iDigitsTrailingMin > 0 && idxChar + i_iDigitsTrailingMin < iMantissaSubStrLen )
                        {
                            idxChar += i_iDigitsTrailingMin;
                        }
                        break;
                    }
                    #if QT_VERSION < 0x050000
                    else if( strValue.toAscii()[idxChar] != '0' )
                    #else
                    else if( strValue.toLatin1()[idxChar] != '0' )
                    #endif
                    {
                        break;
                    }
                    iTrailingZeros++;
                }
                if( idxChar < static_cast<int>(iMantissaSubStrLen-1) )
                {
                    strValue.remove(idxChar+1,iTrailingZeros);
                }
            }
            *io_pstrValue = strValue;
        }
    }
} // removeTrailingZeros

//------------------------------------------------------------------------------
void ZS::System::removeLeadingZerosFromExponent( QString* io_pstrValue )
//------------------------------------------------------------------------------
{
    // String:               "123.45600e-02"
    // idxChar:               0123456789012
    // idxCharExp          =  9
    // idxCharDecPt        =  3
    // uStrLen             = 13
    // iMantissaSubStrLen  =  9
    // iExponentSubStrLen  =  4
    // uTrailingSubStrLen  =  5
    // uLeadingSubStrLen   =  3
    // iTrailingZeros      =  2

    QString      strValue = *io_pstrValue;
    int          idxChar;
    int          idxCharExp;
    int          idxCharFirstZero = -1;
    int          idxCharFirstDigit = -1;

    if( strValue.length() > 0 )
    {
        idxCharExp = strValue.indexOf("e",0,Qt::CaseInsensitive);

        if( idxCharExp > 0 )
        {
            for( idxChar = idxCharExp+1; idxChar < strValue.length(); idxChar++ )
            {
                if( idxCharFirstZero < 0 && strValue[idxChar] == '0' )
                {
                    idxCharFirstZero = idxChar;
                }
                if( idxCharFirstDigit < 0 && isDigitChar(strValue[idxChar]) && strValue[idxChar] != '0' )
                {
                    idxCharFirstDigit = idxChar;
                }
            }
            if( idxCharFirstZero < idxCharFirstDigit )
            {
                strValue.remove(idxCharFirstZero,idxCharFirstDigit-idxCharFirstZero);
                *io_pstrValue = strValue;
            }
        }
    }

} // removeLeadingZerosFromExponent

//------------------------------------------------------------------------------
void ZS::System::formatString(
    const QString& i_strValue,
    int*           o_piDigitsLeading,
    int*           o_piDigitsTrailing,
    int*           o_piDigitsExponent )
//------------------------------------------------------------------------------
{
    int  iDigitsLeading = 0;
    int  iDigitsTrailing = 0;
    int  iDigitsExponent = 0;
    bool bLeadingSection = true;
    bool bTrailingSection = false;
    bool bExponentSection = false;

    for( int idx = 0; idx < i_strValue.length(); idx++ )
    {
        if( bLeadingSection )
        {
            if( i_strValue.at(idx) == QChar('-') )
            {
            }
            else if( i_strValue.at(idx) == QChar('+') )
            {
            }
            else if( i_strValue.at(idx) >= QChar('0') && i_strValue.at(idx) <= QChar('9') )
            {
                iDigitsLeading++;
            }
            else if( i_strValue.at(idx) == QChar('.') )
            {
                bLeadingSection = false;
                bTrailingSection = true;
            }
        }
        else if( bTrailingSection )
        {
            if( i_strValue.at(idx) >= QChar('0') && i_strValue.at(idx) <= QChar('9') )
            {
                iDigitsTrailing++;
            }
            else if( i_strValue.at(idx) == QChar('e') || i_strValue.at(idx) == QChar('E') )
            {
                bTrailingSection = false;
                bExponentSection = true;
            }
        }
        else if( bExponentSection )
        {
            if( i_strValue.at(idx) == QChar('-') )
            {
            }
            else if( i_strValue.at(idx) == QChar('+') )
            {
            }
            else if( i_strValue.at(idx) >= QChar('0') && i_strValue.at(idx) <= QChar('9') )
            {
                iDigitsExponent++;
            }
        }
    }
    if( o_piDigitsLeading != nullptr )
    {
        *o_piDigitsLeading = iDigitsLeading;
    }
    if( o_piDigitsTrailing != nullptr )
    {
        *o_piDigitsTrailing = iDigitsTrailing;
    }
    if( o_piDigitsExponent != nullptr )
    {
        *o_piDigitsExponent = iDigitsExponent;
    }
}
