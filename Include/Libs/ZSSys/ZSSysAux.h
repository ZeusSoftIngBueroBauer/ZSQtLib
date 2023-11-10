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

#ifndef ZSSys_Aux_h
#define ZSSys_Aux_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysCommon.h"
#include "ZSSys/ZSSysTime.h"

#include <QtCore/qbytearray.h>
#include <QtCore/qmargins.h>
#include <QtCore/qmutex.h>
#include <QtCore/qprocess.h>
#include <QtCore/qstring.h>
#include <QtCore/qthread.h>

#ifdef _WINDOWS
// As "min" will be defined as a macro with two arguments and qdatetime uses "min"
// as a function with no arguments "windows.h" must be included after qdatetime
// (which is included by ZSSysTime (which again is included by ZSSysAux)).
#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#endif

/*******************************************************************************
global type definitions and constants
*******************************************************************************/


class QEvent;
class QMimeData;
class QModelIndex;
class QPoint;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
class QStringList;
#endif
class QVariant;

namespace ZS
{
namespace System
{
/*******************************************************************************
public auxiliary methods
*******************************************************************************/

ZSSYSDLL_API QString getCurrentThreadName();

ZSSYSDLL_API QString createGUID();

ZSSYSDLL_API QByteArray str2ByteArr( const QString& i_str );
ZSSYSDLL_API QString byteArr2Str( const QByteArray& i_byteArr );

ZSSYSDLL_API QList<QByteArray> strlst2ByteArrLst( const QStringList& i_strlst );
ZSSYSDLL_API QStringList byteArrLst2StrLst( const QList<QByteArray>& i_arByteArr );

ZSSYSDLL_API QByteArray joinByteArrLst( const QList<QByteArray>& i_arByteArr, char i_chSeparator );
ZSSYSDLL_API QByteArray joinByteArrLst( const QList<QByteArray>& i_arByteArr, const QByteArray& i_strSeparator );

ZSSYSDLL_API QString join(
    const QString& i_strSeparator,
    const QString& i_str1,
    const QString& i_str2,
    const QString& i_str3 = QString(),
    const QString& i_str4 = QString(),
    const QString& i_str5 = QString() );

ZSSYSDLL_API QByteArray join( const QList<QByteArray>& i_arByteArrs, const QByteArray& i_byteArrSeparator );
ZSSYSDLL_API QList<QByteArray> split( const QByteArray& i_byteArr, const QByteArray& i_byteArrSeparator );

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
ZSSYSDLL_API bool dir_removeRecursively( const QString& i_strDirPath );
#endif

#ifdef _WINDOWS
/* A Safer Alternative to TerminateProcess()
   See http://www.drdobbs.com/a-safer-alternative-to-terminateprocess/184416547
   for more details. */
//ZSSYSDLL_API BOOL SafeTerminateProcess( HANDLE i_hndProcess, UINT i_uExitCode );
#endif


/*******************************************************************************
controlling debug output for base modules
*******************************************************************************/

ZSSYSDLL_API void setDebugActive( bool i_bDebug );
ZSSYSDLL_API bool isDebugActive();


/*******************************************************************************
converting common data types into strings and vice versa
*******************************************************************************/

ZSSYSDLL_API int threadId2Int( Qt::HANDLE i_hndThreadId );
ZSSYSDLL_API QString threadId2Str( Qt::HANDLE i_hndThreadId );

ZSSYSDLL_API QString pointer2Str( void* i_pPointer );

ZSSYSDLL_API QString bool2Str( bool i_bValue );
ZSSYSDLL_API bool str2Bool( const QString& i_str, bool* o_pbConverted = nullptr );

ZSSYSDLL_API QByteArray bool2ByteArr( bool i_bValue );
ZSSYSDLL_API bool byteArr2Bool( const QByteArray& i_str, bool* o_pbConverted = nullptr );

#ifdef _WINDOWS
//ZSSYSDLL_API QString hResult2Str( HRESULT i_hRes );
//ZSSYSDLL_API QString winErrorCode2MessageStr( DWORD i_dwErrCode );
ZSSYSDLL_API QString lastWinErrorCode2MessageStr();
#ifndef __oaidl_h__
typedef /* [wire_marshal] */ struct tagVARIANT VARIANT;
#endif
ZSSYSDLL_API QString oleVariant2Str( VARIANT* i_pVariant );
#endif

ZSSYSDLL_API bool isNumeric( const QVariant& i_var );

ZSSYSDLL_API QString qCaseSensitivity2Str( int i_iVal, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );
ZSSYSDLL_API Qt::CaseSensitivity str2QCaseSensitivity( const QString& i_str, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName, bool* o_pbConverted = nullptr );

ZSSYSDLL_API QString qOrientation2Str( int i_iVal, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );
ZSSYSDLL_API Qt::Orientation str2QOrientation( const QString& i_str, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName, bool* o_pbConverted = nullptr );

ZSSYSDLL_API QString qSortOrder2Str( int i_iVal, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );
ZSSYSDLL_API Qt::SortOrder str2QSortOrder( const QString& i_str, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName, bool* o_pbConverted = nullptr );

ZSSYSDLL_API QString qProcessExitStatus2Str( int i_iVal, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );
ZSSYSDLL_API QProcess::ExitStatus str2QProcessExitStatus( const QString& i_str, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName, bool* o_pbConverted = nullptr );

ZSSYSDLL_API QString qProcessError2Str( int i_iVal, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );
ZSSYSDLL_API QProcess::ProcessError str2QProcessError( const QString& i_str, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName, bool* o_pbConverted = nullptr );

ZSSYSDLL_API QString qThreadPriority2Str( QThread::Priority i_priority, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );

ZSSYSDLL_API QString qXmlStreamTokenType2Str( int i_iVal, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );

ZSSYSDLL_API QString qBrushStyle2Str( const Qt::BrushStyle& i_brushStyle );
ZSSYSDLL_API QString qCheckState2Str( int i_iVal );
ZSSYSDLL_API QString qCursorShape2Str( int i_iVal );
ZSSYSDLL_API QString qDropActions2Str( int i_actions );
ZSSYSDLL_API QString qEvent2Str( QEvent* i_pEv, int i_iDetailLevel = 0 );
ZSSYSDLL_API QString qEventType2Str( int i_iVal, bool i_bShort = true );
ZSSYSDLL_API QString qFocusReason2Str( int i_iVal );
ZSSYSDLL_API QString qItemDataRole2Str( int i_iVal );
ZSSYSDLL_API QString qItemFlags2Str( quint32 i_flags );

ZSSYSDLL_API QString qKeyCode2Str( int i_iVal, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );
ZSSYSDLL_API int char2QKeyCode( const QChar& i_ch );    // e.g. "o", "1", "Z", " ", "?", ..
ZSSYSDLL_API int str2QKeyCode( const QString& i_str );  // e.g. "F1", "Enter", "Escape", ..
ZSSYSDLL_API QString qKeyboardModifiers2Str( quint32 i_modifiers );
ZSSYSDLL_API Qt::KeyboardModifiers char2QKeyboardModifiers( const QChar& i_ch );

ZSSYSDLL_API QString qMargins2Str( const QMargins& i_margins, bool i_bShort = true );
ZSSYSDLL_API QString qMimeData2Str( const QMimeData* i_pMimeData, int i_iDetailLevel = 0 );
ZSSYSDLL_API QString qModelIndex2Str( const QModelIndex& i_modelIdx );
ZSSYSDLL_API QString qMouseButton2Str( int i_iBtn, bool i_bShort = true );
ZSSYSDLL_API QString qMouseButtons2Str( int i_iBtns, bool i_bShort = true );
ZSSYSDLL_API QString qPenStyle2Str( const Qt::PenStyle& i_penStyle );
ZSSYSDLL_API QString qPoint2Str( const QPoint& i_pt, bool i_bShort = true );
ZSSYSDLL_API QString qPoint2Str( const QPointF& i_pt, bool i_bShort = true );
ZSSYSDLL_API QString qRect2Str( const QRect& i_rct, bool i_bShort = true );
ZSSYSDLL_API QString qRect2Str( const QRectF& i_rct, bool i_bShort = true );
ZSSYSDLL_API QString qSize2Str( const QSize& i_size, bool i_bShort = true );
ZSSYSDLL_API QString qSize2Str( const QSizeF& i_size, bool i_bShort = true );
ZSSYSDLL_API QString qVariantType2Str( int i_iVal );


/*******************************************************************************
Creating strings
*******************************************************************************/

ZSSYSDLL_API QString buildPathStr(
    const QString& i_strSeparator,
    const QString& i_str1,
    const QString& i_str2 );

ZSSYSDLL_API QString buildPathStr(
    const QString& i_strSeparator,
    const QString& i_str1,
    const QString& i_str2,
    const QString& i_str3 );

ZSSYSDLL_API QString buildPathStr(
    const QString& i_strSeparator,
    const QString& i_str1,
    const QString& i_str2,
    const QString& i_str3,
    const QString& i_str4 );

ZSSYSDLL_API QString buildPathStr(
    const QString& i_strSeparator,
    const QString& i_str1,
    const QString& i_str2,
    const QString& i_str3,
    const QString& i_str4,
    const QString& i_str5 );

ZSSYSDLL_API QString truncateStringWithEllipsisInTheMiddle(
    const QString& i_str,
    int            i_iMaxLength,
    const QString& i_strEllipsis = ".." );

ZSSYSDLL_API QString encodeForXml( const QString& i_str );
ZSSYSDLL_API QString encodeForHtml( const QString& i_str );
ZSSYSDLL_API QString decodeFromHtml( const QString& i_str );

/*******************************************************************************
Parsing strings
*******************************************************************************/

ZSSYSDLL_API bool isWhiteSpace( const QChar& i_ch );
ZSSYSDLL_API bool isDigitChar( const QChar& i_ch );
ZSSYSDLL_API bool isSign( const QChar& i_ch );
ZSSYSDLL_API bool isDecimalPoint( const QChar& i_ch );
ZSSYSDLL_API bool isDivisionPoint( const QChar& i_ch );
ZSSYSDLL_API bool isExponentChar( const QChar& i_ch );
ZSSYSDLL_API bool isNumChar( const QChar& i_ch );

ZSSYSDLL_API void insertDelimiter(
    int            i_iDigitsPerDigitGroup,
    const QString& i_strDelimiter,
    QString*       io_pstrValue,
    int            i_iDigitsLeading,
    int            i_iDigitsTrailing );
ZSSYSDLL_API void removeTrailingZeros(
    QString* io_pstrValue,
    int      i_iDigitsTrailingMin = 1,
    QChar    i_charDecPoint = '.' );
ZSSYSDLL_API void removeLeadingZerosFromExponent( QString* io_pstrValue );
ZSSYSDLL_API void formatString(
    const QString& i_strValue,
    int*           o_piDigitsLeading,
    int*           o_piDigitsTrailing,
    int*           o_piDigitsExponent );


} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_Aux_h
