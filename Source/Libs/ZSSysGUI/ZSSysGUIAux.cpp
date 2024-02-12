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

#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"

#include <QtGui/qcursor.h>
#include <QtGui/qevent.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
public auxiliary methods
*******************************************************************************/

/*==============================================================================
QColor
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::GUI::qColor2Str( const QColor& i_clr )
//------------------------------------------------------------------------------
{
    return i_clr.name(QColor::HexArgb);
}

/*==============================================================================
QCursor
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumStrQCursorShape[] =
//------------------------------------------------------------------------------
{
    /*  0 */ SEnumEntry( Qt::ArrowCursor, "Arrow" ),
    /*  1 */ SEnumEntry( Qt::UpArrowCursor, "UpArrowCursor" ),
    /*  2 */ SEnumEntry( Qt::CrossCursor, "CrossCursor" ),
    /*  3 */ SEnumEntry( Qt::WaitCursor, "WaitCursor" ),
    /*  4 */ SEnumEntry( Qt::IBeamCursor, "IBeamCursor" ),
    /*  5 */ SEnumEntry( Qt::SizeVerCursor, "SizeVerCursor" ),
    /*  6 */ SEnumEntry( Qt::SizeHorCursor, "SizeHorCursor" ),
    /*  7 */ SEnumEntry( Qt::SizeBDiagCursor, "SizeBDiagCursor" ),
    /*  8 */ SEnumEntry( Qt::SizeFDiagCursor, "SizeFDiagCursor" ),
    /*  9 */ SEnumEntry( Qt::SizeAllCursor, "SizeAllCursor" ),
    /* 10 */ SEnumEntry( Qt::BlankCursor, "BlankCursor" ),
    /* 11 */ SEnumEntry( Qt::SplitVCursor, "SplitVCursor" ),
    /* 12 */ SEnumEntry( Qt::SplitHCursor, "SplitHCursor" ),
    /* 13 */ SEnumEntry( Qt::PointingHandCursor, "PointingHandCursor" ),
    /* 14 */ SEnumEntry( Qt::ForbiddenCursor, "ForbiddenCursor" ),
    /* 15 */ SEnumEntry( Qt::OpenHandCursor, "OpenHandCursor" ),
    /* 16 */ SEnumEntry( Qt::ClosedHandCursor, "ClosedHandCursor" ),
    /* 17 */ SEnumEntry( Qt::WhatsThisCursor, "WhatsThisCursor" ),
    /* 18 */ SEnumEntry( Qt::BusyCursor, "BusyCursor" ),
    /* 19 */ SEnumEntry( Qt::DragMoveCursor, "DragMoveCursor" ),
    /* 20 */ SEnumEntry( Qt::DragCopyCursor, "DragCopyCursor" ),
    /* 21 */ SEnumEntry( Qt::DragLinkCursor, "DragLinkCursor" ),
    /* 22 */ SEnumEntry( 22, "? (22)" ),
    /* 23 */ SEnumEntry( 23, "? (23)" ),
    /* 24 */ SEnumEntry( Qt::BitmapCursor, "BitmapCursor" )
};

//------------------------------------------------------------------------------
QString ZS::System::GUI::qCursor2Str(const QCursor& i_cursor)
//------------------------------------------------------------------------------
{
    QString str;
    if (i_cursor.shape() >= 0 && i_cursor.shape() < _ZSArrLen(s_arEnumStrQCursorShape)) {
        str = SEnumEntry::enumerator2Str(
            s_arEnumStrQCursorShape, _ZSArrLen(s_arEnumStrQCursorShape), i_cursor.shape());
    }
    else {
        str = "? (" + QString::number(i_cursor.shape()) + ")";
    }
    return str;
}

/*==============================================================================
Enum QAbstractItemDelegate::EndEditHint
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumStrQItemDelegateEndEditHint[] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( QAbstractItemDelegate::NoHint,           "NoHint"           ),
    /* 1 */ SEnumEntry( QAbstractItemDelegate::EditNextItem,     "EditNextItem"     ),
    /* 2 */ SEnumEntry( QAbstractItemDelegate::EditPreviousItem, "EditPreviousItem" ),
    /* 3 */ SEnumEntry( QAbstractItemDelegate::SubmitModelCache, "SubmitModelCache" ),
    /* 4 */ SEnumEntry( QAbstractItemDelegate::RevertModelCache, "RevertModelCache" )
};

//------------------------------------------------------------------------------
QString ZS::System::GUI::qItemDelegateEndEditHint2Str(QAbstractItemDelegate::EndEditHint i_hint)
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(
        s_arEnumStrQItemDelegateEndEditHint, _ZSArrLen(s_arEnumStrQItemDelegateEndEditHint), i_hint);
}

/*==============================================================================
Enum QAbstractItemView::DragDropMode
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
    return SEnumEntry::enumerator2Str(s_arEnumStrQItemViewDragDropMode, _ZSArrLen(s_arEnumStrQItemViewDragDropMode), i_iVal, i_alias);
}

//------------------------------------------------------------------------------
QAbstractItemView::DragDropMode ZS::System::GUI::str2QItemViewDragDropMode( const QString& i_str, EEnumEntryAliasStr i_alias, bool* o_pbConverted )
//------------------------------------------------------------------------------
{
    QAbstractItemView::DragDropMode mode = QAbstractItemView::NoDragDrop;
    bool bConverted = false;
    int iVal = SEnumEntry::str2Enumerator(
        s_arEnumStrQItemViewDragDropMode, _ZSArrLen(s_arEnumStrQItemViewDragDropMode), i_str, i_alias, Qt::CaseInsensitive);
    if (iVal >= 0 && iVal < _ZSArrLen(s_arEnumStrQItemViewDragDropMode)) {
        mode = static_cast<QAbstractItemView::DragDropMode>(iVal);
        bConverted = true;
    }
    if (o_pbConverted != nullptr) {
        *o_pbConverted = bConverted;
    }
    return mode;
}

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
    if (i_iVal == Qt::IgnoreAction) {
        str = "Ignore";
    }
    else {
        if (i_iVal & Qt::CopyAction) {
            if( !str.isEmpty() ) str += "|";
            str += "Copy";
        }
        if (i_iVal & Qt::MoveAction) {
            if( !str.isEmpty() ) str += "|";
            str += "Move";
        }
        if (i_iVal & Qt::LinkAction) {
            if( !str.isEmpty() ) str += "|";
            str += "Link";
        }
    }
    return str;
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::qDragEnterEvent2Str(QDragEnterEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString str =
        "Type: " + qEventType2Str(i_pEv->type()) +
        ", DropAction: " + qDropAction2Str(i_pEv->dropAction()) +
        ", MouseButtons: " + qMouseButtons2Str(i_pEv->mouseButtons()) +
        ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->keyboardModifiers()) +
        ", Pos: " + qPoint2Str(i_pEv->pos()) +
        ", MimeData: " + qMimeData2Str(i_pEv->mimeData());
    return str;
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::qDragLeaveEvent2Str(QDragLeaveEvent* i_pEv)
//------------------------------------------------------------------------------
{
    return qEventType2Str(i_pEv->type());
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::qDragMoveEvent2Str(QDragMoveEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString str =
        "Type: " + qEventType2Str(i_pEv->type()) +
        ", DropAction: " + qDropAction2Str(i_pEv->dropAction()) +
        ", MouseButtons: " + qMouseButtons2Str(i_pEv->mouseButtons()) +
        ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->keyboardModifiers()) +
        ", Pos: " + qPoint2Str(i_pEv->pos()) +
        ", MimeData: " + qMimeData2Str(i_pEv->mimeData());
    return str;
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::qDropEvent2Str(QDropEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString str =
        "Type: " + qEventType2Str(i_pEv->type()) +
        ", DropAction: " + qDropAction2Str(i_pEv->dropAction()) +
        ", MouseButtons: " + qMouseButtons2Str(i_pEv->mouseButtons()) +
        ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->keyboardModifiers()) +
        ", Pos: " + qPoint2Str(i_pEv->pos()) +
        ", MimeData: " + qMimeData2Str(i_pEv->mimeData());
    return str;
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::qKeyEvent2Str(QKeyEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString str =
        "Type: " + qEventType2Str(i_pEv->type()) +
        ", Key: " + qKeyCode2Str(i_pEv->key()) +
        ", Modifiers: " + qKeyboardModifiers2Str(i_pEv->modifiers()) +
        ", Text: " + i_pEv->text() +
        ", AutoRepeat: " + bool2Str(i_pEv->isAutoRepeat());
    if (i_pEv->isAutoRepeat()) {
        str += ": Count: " + QString::number(i_pEv->count());
    }
    return str;
}

//------------------------------------------------------------------------------
/*!
    @param [in] i_type
        Must be either QEvent::KeyPress or QEvent::KeyRelease.
*/
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

} // str2QKeyEvent

//------------------------------------------------------------------------------
QString ZS::System::GUI::qMouseEvent2Str(QMouseEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString str =
        "Type: " + qEventType2Str(i_pEv->type()) +
        ", Button: " + qMouseButton2Str(i_pEv->button()) +
        ", Buttons: " + qMouseButtons2Str(i_pEv->buttons()) +
        ", KeyboardModifiers: " + qKeyboardModifiers2Str(i_pEv->modifiers()) +
        ", Pos: " + qPoint2Str(i_pEv->pos()) +
        ", GlobalPos: " + qPoint2Str(i_pEv->globalPos());
    return str;
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::qPaintEvent2Str(QPaintEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString str =
        "Type: " + qEventType2Str(i_pEv->type()) +
        ", Rect {" + qRect2Str(i_pEv->rect()) + "}";
        //", Region {" + qRegion2Str(i_pEv->region()) + "}";
    return str;
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::qResizeEvent2Str(QResizeEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString str =
        "Type: " + qEventType2Str(i_pEv->type()) +
        ", Size {" + qSize2Str(i_pEv->size()) + "}" +
        ", OldSize {" + qSize2Str(i_pEv->oldSize()) + "}";
    return str;
}

/*==============================================================================
QPolygon
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::GUI::qPolygon2Str(const QPolygon& i_plg)
//------------------------------------------------------------------------------
{
    QString str = "[" + QString::number(i_plg.size()) + "]";
    if (i_plg.size() > 0) {
        str += "(";
        for (int idxPt = 0; idxPt < i_plg.size(); ++idxPt) {
            if (idxPt > 0) str += ", ";
            str += "{" + qPoint2Str(i_plg.at(idxPt)) + "}";
        }
        str += ")";
    }
    return str;
}

//------------------------------------------------------------------------------
QString ZS::System::GUI::qPolygon2Str(const QPolygonF& i_plg)
//------------------------------------------------------------------------------
{
    QString str = "[" + QString::number(i_plg.size()) + "]";
    if (i_plg.size() > 0) {
        str += "(";
        for (int idxPt = 0; idxPt < i_plg.size(); ++idxPt) {
            if (idxPt > 0) str += ", ";
            str += "{" + qPoint2Str(i_plg.at(idxPt)) + "}";
        }
        str += ")";
    }
    return str;
}

/*==============================================================================
QAbstractSpinBox
==============================================================================*/

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
//------------------------------------------------------------------------------
QString ZS::System::GUI::qSpinBoxStepType2Str(QAbstractSpinBox::StepType i_stepType, EEnumEntryAliasStr i_alias)
//------------------------------------------------------------------------------
{
    QString str;
    if (i_stepType == QAbstractSpinBox::DefaultStepType) {
        str = "Default";
    }
    else if (i_stepType == QAbstractSpinBox::AdaptiveDecimalStepType) {
        str = "AdaptiveDecimalStepType";
    }
    else {
        str = "? (" + QString::number(i_stepType) + ")";
    }
    return str;
}
#endif

/*==============================================================================
QValidator
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::System::GUI::qValidatorState2Str(QValidator::State i_state, EEnumEntryAliasStr i_alias)
//------------------------------------------------------------------------------
{
    QString str;
    if (i_state == QValidator::Invalid) {
        str = "Invalid";
    }
    else if (i_state == QValidator::Intermediate) {
        str = "Intermediate";
    }
    else if (i_state == QValidator::Acceptable) {
        str = "Acceptable";
    }
    else {
        str = "? (" + QString::number(i_state) + ")";
    }
    return str;
}
