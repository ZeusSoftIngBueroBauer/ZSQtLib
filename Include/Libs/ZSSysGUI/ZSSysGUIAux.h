/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSSysGUI_Aux_h
#define ZSSysGUI_Aux_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qabstractitemview.h>
#else
#include <QtWidgets/qabstractitemview.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"

#ifdef _WINDOWS
// As "min" will be defined as a macro with two arguments and qdatetime uses "min"
// as a function with no arguments "windows.h" must be included after qdatetime
// (which is included by ZSSysTime (which again is included by ZSSysAux)).
#include <windows.h>
#endif

class QMouseEvent;
class QKeyEvent;

namespace ZS
{
namespace System
{
namespace GUI
{
/*******************************************************************************
public auxiliary methods
*******************************************************************************/

ZSSYSGUIDLL_API QString qItemViewDragDropMode2Str( int i_iVal, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );
ZSSYSGUIDLL_API QAbstractItemView::DragDropMode str2QItemViewDragDropMode( const QString& i_str, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName, bool* o_pbConverted = nullptr );

ZSSYSGUIDLL_API QString qDropAction2Str( int i_iVal, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );

ZSSYSGUIDLL_API QString qItemViewCursorAction2Str( int i_iVal, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );

ZSSYSGUIDLL_API QKeyEvent* str2QKeyEvent( const QString& i_str, QEvent::Type i_type ); // type must be either QEvent::KeyPress or QEvent::KeyRelease

ZSSYSGUIDLL_API QString qKeyEvent2Str( QKeyEvent* i_pEv, int i_iDetailLevel = 0 );
ZSSYSGUIDLL_API QString qMouseEvent2Str( QMouseEvent* i_pEv, int i_iDetailLevel = 0 );
ZSSYSGUIDLL_API QString qDragEnterEvent2Str( QDragEnterEvent* i_pEv, int i_iDetailLevel = 0 );
ZSSYSGUIDLL_API QString qDragLeaveEvent2Str( QDragLeaveEvent* i_pEv, int i_iDetailLevel = 0 );
ZSSYSGUIDLL_API QString qDragMoveEvent2Str( QDragMoveEvent* i_pEv, int i_iDetailLevel = 0 );
ZSSYSGUIDLL_API QString qDropEvent2Str( QDropEvent* i_pEv, int i_iDetailLevel = 0 );

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_Aux_h
