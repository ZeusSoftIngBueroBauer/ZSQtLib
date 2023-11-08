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

#include <QtGui/qevent.h>

#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
public auxiliary methods
*******************************************************************************/

//------------------------------------------------------------------------------
QString ZS::System::GUI::qColor2Str( const QColor& i_clr )
//------------------------------------------------------------------------------
{
    return i_clr.name(QColor::HexArgb);
}

/*==============================================================================
Enum QAbstractItemView::ItemDataRole
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumStrQItemViewDragDropMode[] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( QAbstractItemView::NoDragDrop,   "NoDragDrop"   ),
    /* 1 */ SEnumEntry( QAbstractItemView::DragOnly,     "DragOnly"     ),
    /* 2 */ SEnumEntry( QAbstractItemView::DropOnly,     "DropOnly"     ),
    /* 3 */ SEnumEntry( QAbstractItemView::DragDrop,     "DragDrop"     ),
    /* 4 */ SEnumEntry( QAbstractItemView::InternalMove, "InternalMove" )
};

//------------------------------------------------------------------------------
QString ZS::System::GUI::qItemViewDragDropMode2Str( int i_iVal, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrQItemViewDragDropMode, _ZSArrLen(s_arEnumStrQItemViewDragDropMode), i_iVal, i_alias );
}

//------------------------------------------------------------------------------
QAbstractItemView::DragDropMode ZS::System::GUI::str2QItemViewDragDropMode( const QString& i_str, EEnumEntryAliasStr i_alias, bool* o_pbConverted )
//------------------------------------------------------------------------------
{
    QAbstractItemView::DragDropMode mode = QAbstractItemView::NoDragDrop;

    bool bConverted = false;

    int iVal = SEnumEntry::str2Enumerator( s_arEnumStrQItemViewDragDropMode, _ZSArrLen(s_arEnumStrQItemViewDragDropMode), i_str, i_alias, Qt::CaseInsensitive );

    if( iVal >= 0 && iVal < _ZSArrLen(s_arEnumStrQItemViewDragDropMode) )
    {
        mode = static_cast<QAbstractItemView::DragDropMode>(iVal);
        bConverted = true;
    }

    if( o_pbConverted != nullptr )
    {
        *o_pbConverted = bConverted;
    }

    return mode;

} // str2QItemViewDragDropMode


/*==============================================================================
Enum QAbstractItemView::ItemDataRole
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumStrQItemViewCursorAction[] =
//------------------------------------------------------------------------------
{                       // Unfortunately CursorAction is protected.
    /* 0 */ SEnumEntry( /*QAbstractItemView::MoveUp*/       0, "MoveUp"       ),
    /* 1 */ SEnumEntry( /*QAbstractItemView::MoveDown*/     1, "MoveDown"     ),
    /* 2 */ SEnumEntry( /*QAbstractItemView::MoveLeft*/     2, "MoveLeft"     ),
    /* 3 */ SEnumEntry( /*QAbstractItemView::MoveRight*/    3, "MoveRight"    ),
    /* 4 */ SEnumEntry( /*QAbstractItemView::MoveHome*/     4, "MoveHome"     ),
    /* 5 */ SEnumEntry( /*QAbstractItemView::MoveEnd*/      5, "MoveEnd"      ),
    /* 6 */ SEnumEntry( /*QAbstractItemView::MovePageUp*/   6, "MovePageUp"   ),
    /* 7 */ SEnumEntry( /*QAbstractItemView::MovePageDown*/ 7, "MovePageDown" ),
    /* 8 */ SEnumEntry( /*QAbstractItemView::MoveNext*/     8, "MoveNext"     ),
    /* 9 */ SEnumEntry( /*QAbstractItemView::MovePrevious*/ 9, "MovePrevious" )
};

//------------------------------------------------------------------------------
QString ZS::System::GUI::qItemViewCursorAction2Str( int i_iVal, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(
        s_arEnumStrQItemViewCursorAction, _ZSArrLen(s_arEnumStrQItemViewCursorAction), i_iVal, i_alias );
}

/*==============================================================================
Enum Qt::DockWidgetArea
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::GUI::qDockWidgetArea2Str( Qt::DockWidgetAreas i_areas, EEnumEntryAliasStr /*i_alias*/ )
//------------------------------------------------------------------------------
{
    QString str;
    if(i_areas == Qt::NoDockWidgetArea) {
        str = "None";
    }
    else {
        if(i_areas & Qt::LeftDockWidgetArea) {
            str += "Left";
        }
        if(i_areas & Qt::RightDockWidgetArea) {
            if( !str.isEmpty() ) str += "|";
            str += "Right";
        }
        if(i_areas & Qt::TopDockWidgetArea) {
            if( !str.isEmpty() ) str += "|";
            str += "Top";
        }
        if(i_areas & Qt::BottomDockWidgetArea) {
            if( !str.isEmpty() ) str += "|";
            str += "Bottom";
        }
    }
    return str;
}

//------------------------------------------------------------------------------
Qt::DockWidgetArea ZS::System::GUI::str2QDockWidgetArea( const QString& i_str )
//------------------------------------------------------------------------------
{
    Qt::DockWidgetArea area = Qt::NoDockWidgetArea;
    if( i_str.compare("Left", Qt::CaseInsensitive) == 0 ) {
        area = Qt::LeftDockWidgetArea;
    }
    else if( i_str.compare("Right", Qt::CaseInsensitive) == 0 ) {
        area = Qt::LeftDockWidgetArea;
    }
    else if( i_str.compare("Top", Qt::CaseInsensitive) == 0 ) {
        area = Qt::LeftDockWidgetArea;
    }
    else if( i_str.compare("Bottom", Qt::CaseInsensitive) == 0 ) {
        area = Qt::LeftDockWidgetArea;
    }
    return area;
}

/*==============================================================================
Enum Qt::DropAction
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::GUI::qDropAction2Str( int i_iVal, EEnumEntryAliasStr /*i_alias*/ )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_iVal == Qt::IgnoreAction )
    {
        str = "Ignore";
    }
    else
    {
        if( i_iVal & Qt::CopyAction )
        {
            if( !str.isEmpty() ) str += "|";
            str += "Copy";
        }
        if( i_iVal & Qt::MoveAction )
        {
            if( !str.isEmpty() ) str += "|";
            str += "Move";
        }
        if( i_iVal & Qt::LinkAction )
        {
            if( !str.isEmpty() ) str += "|";
            str += "Link";
        }
    }

    return str;

} // qDropAction2Str

/*==============================================================================
QKeyEvent
==============================================================================*/

//------------------------------------------------------------------------------
QKeyEvent* ZS::System::GUI::str2QKeyEvent( const QString& i_str, QEvent::Type i_type )
//------------------------------------------------------------------------------
{
    QKeyEvent* pEv = nullptr;

    int iKey = -1;
    Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;

    if( i_str.length() == 1 )
    {
        iKey = char2QKeyCode(i_str[0]);
        keyboardModifiers = char2QKeyboardModifiers(i_str[0]);

        pEv = new QKeyEvent(
            /* type      */ i_type,
            /* iKey      */ iKey,
            /* modifiers */ keyboardModifiers,
            /* strText   */ i_str,
            /* bAutoRep  */ false,
            /* uCount    */ 1 );
    }
    else if( i_str.compare("ctrl-c",Qt::CaseInsensitive) == 0 )
    {
        iKey = str2QKeyCode("c");
        keyboardModifiers = Qt::ControlModifier;

        pEv = new QKeyEvent(
            /* type      */ i_type,
            /* iKey      */ iKey,
            /* modifiers */ keyboardModifiers,
            /* strText   */ "c",
            /* bAutoRep  */ false,
            /* uCount    */ 1 );
    }
    else if( i_str.compare("ctrl-x",Qt::CaseInsensitive) == 0 )
    {
        iKey = str2QKeyCode("x");
        keyboardModifiers = Qt::ControlModifier;

        pEv = new QKeyEvent(
            /* type      */ i_type,
            /* iKey      */ iKey,
            /* modifiers */ keyboardModifiers,
            /* strText   */ "x",
            /* bAutoRep  */ false,
            /* uCount    */ 1 );
    }
    else if( i_str.compare("ctrl-v",Qt::CaseInsensitive) == 0 )
    {
        iKey = str2QKeyCode("v");
        keyboardModifiers = Qt::ControlModifier;

        pEv = new QKeyEvent(
            /* type      */ i_type,
            /* iKey      */ iKey,
            /* modifiers */ keyboardModifiers,
            /* strText   */ "v",
            /* bAutoRep  */ false,
            /* uCount    */ 1 );
    }
    else if( i_str.length() > 1 )
    {
        iKey = str2QKeyCode(i_str);

        pEv = new QKeyEvent(
            /* type      */ i_type,
            /* iKey      */ iKey,
            /* modifiers */ keyboardModifiers,
            /* strText   */ "",
            /* bAutoRep  */ false,
            /* uCount    */ 1 );
    }

    return pEv;

} // char2QKeyEvent

//------------------------------------------------------------------------------
QString ZS::System::GUI::qKeyEvent2Str( QKeyEvent* i_pEv, int /*i_iDetailLevel*/ )
//------------------------------------------------------------------------------
{
    QString str;

    str  = "Type: " + qEventType2Str(i_pEv->type());
    str += ", Key: " + qKeyCode2Str(i_pEv->key());
    str += ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->modifiers());
    str += ", Text: " + i_pEv->text();
    str += ", AutoRepeat: " + bool2Str(i_pEv->isAutoRepeat());
    if( i_pEv->isAutoRepeat() )
    {
        str += ": Count: " + QString::number(i_pEv->count());
    }

    return str;

} // qKeyEvent2Str

/*==============================================================================
QMouseEvent
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::GUI::qMouseEvent2Str( QMouseEvent* i_pEv, int /*i_iDetailLevel*/ )
//------------------------------------------------------------------------------
{
    return "Type: " + qEventType2Str(i_pEv->type()) +
        ", Button: " + qMouseButton2Str(i_pEv->button()) +
        ", Buttons: " + qMouseButtons2Str(i_pEv->buttons()) +
        ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->modifiers()) +
        ", Pos: " + qPoint2Str(i_pEv->pos()) +
        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        ", GlobalPos: " + qPoint2Str(i_pEv->globalPos());
        #else
           ", GlobalPos: " + qPoint2Str(i_pEv->globalPosition());
        #endif
}

/*==============================================================================
QDrag/Drop-Event
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::GUI::qDragEnterEvent2Str( QDragEnterEvent* i_pEv, int /*i_iDetailLevel*/ )
//------------------------------------------------------------------------------
{
    return "Type: " + qEventType2Str(i_pEv->type()) +
        ", DropAction: " + qDropAction2Str(i_pEv->dropAction()) +
        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        ", MouseButtons: " + qMouseButtons2Str(i_pEv->mouseButtons()) +
        ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->keyboardModifiers()) +
        ", Pos: " + qPoint2Str(i_pEv->pos()) +
        #else
        ", MouseButtons: " + qMouseButtons2Str(i_pEv->buttons()) +
        ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->modifiers()) +
           ", Pos: " + qPoint2Str(i_pEv->position().toPoint()) +
        #endif
        ", MimeData: " + qMimeData2Str(i_pEv->mimeData());
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::qDragLeaveEvent2Str( QDragLeaveEvent* i_pEv, int /*i_iDetailLevel*/ )
//------------------------------------------------------------------------------
{
    return "Type: " + qEventType2Str(i_pEv->type());
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::qDragMoveEvent2Str( QDragMoveEvent* i_pEv, int /*i_iDetailLevel*/ )
//------------------------------------------------------------------------------
{
    return "Type: " + qEventType2Str(i_pEv->type()) +
        ", DropAction: " + qDropAction2Str(i_pEv->dropAction()) +
        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        ", MouseButtons: " + qMouseButtons2Str(i_pEv->mouseButtons()) +
        ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->keyboardModifiers()) +
        ", Pos: " + qPoint2Str(i_pEv->pos()) +
        #else
        ", MouseButtons: " + qMouseButtons2Str(i_pEv->buttons()) +
        ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->modifiers()) +
        ", Pos: " + qPoint2Str(i_pEv->position().toPoint()) +
        #endif
        ", MimeData: " + qMimeData2Str(i_pEv->mimeData());
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::qDropEvent2Str( QDropEvent* i_pEv, int /*i_iDetailLevel*/ )
//------------------------------------------------------------------------------
{
    return "Type: " + qEventType2Str(i_pEv->type()) +
        ", DropAction: " + qDropAction2Str(i_pEv->dropAction()) +
        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        ", MouseButtons: " + qMouseButtons2Str(i_pEv->mouseButtons()) +
        ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->keyboardModifiers()) +
        ", Pos: " + qPoint2Str(i_pEv->pos()) +
        #else
        ", MouseButtons: " + qMouseButtons2Str(i_pEv->buttons()) +
        ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->modifiers()) +
        ", Pos: " + qPoint2Str(i_pEv->position().toPoint()) +
        #endif
        ", MimeData: " + qMimeData2Str(i_pEv->mimeData());
}
