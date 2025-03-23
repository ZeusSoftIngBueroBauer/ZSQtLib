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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphicsTextItem.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CGraphicsTextItem : public QGraphicsTextItem
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphicsTextItem::CGraphicsTextItem(QGraphicsItem* i_pParentItem) :
//------------------------------------------------------------------------------
    QGraphicsTextItem(i_pParentItem),
    m_strPath(),
    m_iTraceItemStatesBlockedCounter(0),
    m_pTrcAdminObjCtorsAndDtor(nullptr),
    m_pTrcAdminObjItemChange(nullptr),
    m_pTrcAdminObjBoundingRect(nullptr),
    m_pTrcAdminObjCursor(nullptr),
    m_pTrcAdminObjPaint(nullptr),
    m_pTrcAdminObjSceneEvent(nullptr),
    m_pTrcAdminObjHoverEnterLeaveEvents(nullptr),
    m_pTrcAdminObjHoverMoveEvents(nullptr),
    m_pTrcAdminObjMouseClickEvents(nullptr),
    m_pTrcAdminObjMouseMoveEvents(nullptr),
    m_pTrcAdminObjKeyEvents(nullptr),
    m_pTrcAdminObjFocusEvents(nullptr),
    m_pTrcAdminObjDragDropEvents(nullptr),
    m_pTrcAdminObjInputMethodEvents(nullptr)
{
    m_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::CtorsAndDtor");
    m_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::ItemChange");
    m_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::BoundingRect");
    m_pTrcAdminObjCursor = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::Cursor");
    m_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::Paint");
    m_pTrcAdminObjSceneEvent = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::SceneEvent");
    m_pTrcAdminObjHoverEnterLeaveEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::HoverEnterLeaveEvents");
    m_pTrcAdminObjHoverMoveEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::HoverMoveEvents");
    m_pTrcAdminObjMouseClickEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::MouseClickEvents");
    m_pTrcAdminObjMouseMoveEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::MouseMoveEvents");
    m_pTrcAdminObjKeyEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::KeyEvents");
    m_pTrcAdminObjFocusEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::FocusEvents");
    m_pTrcAdminObjDragDropEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::DragDropEvents");
    m_pTrcAdminObjInputMethodEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::InputMethodEvents");

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(i_pParentItem);
        if (pGraphObjParent == nullptr) {
            strMthInArgs = "Parent: " + QString(i_pParentItem == nullptr ? "null" : "GraphicsItem");
        }
        else {
            strMthInArgs = "Parent: " + pGraphObjParent->path();
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable
            |QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);
}

//------------------------------------------------------------------------------
CGraphicsTextItem::CGraphicsTextItem(const QString& i_strText, QGraphicsItem* i_pParentItem) :
//------------------------------------------------------------------------------
    QGraphicsTextItem(i_strText, i_pParentItem),
    m_strPath(),
    m_iTraceItemStatesBlockedCounter(0),
    m_pTrcAdminObjCtorsAndDtor(nullptr),
    m_pTrcAdminObjItemChange(nullptr),
    m_pTrcAdminObjBoundingRect(nullptr),
    m_pTrcAdminObjPaint(nullptr),
    m_pTrcAdminObjSceneEvent(nullptr),
    m_pTrcAdminObjHoverEnterLeaveEvents(nullptr),
    m_pTrcAdminObjHoverMoveEvents(nullptr),
    m_pTrcAdminObjMouseClickEvents(nullptr),
    m_pTrcAdminObjMouseMoveEvents(nullptr),
    m_pTrcAdminObjKeyEvents(nullptr),
    m_pTrcAdminObjFocusEvents(nullptr),
    m_pTrcAdminObjDragDropEvents(nullptr),
    m_pTrcAdminObjInputMethodEvents(nullptr)
{
    m_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::CtorsAndDtor");
    m_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::ItemChange");
    m_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::BoundingRect");
    m_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::Paint");
    m_pTrcAdminObjSceneEvent = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::SceneEvent");
    m_pTrcAdminObjHoverEnterLeaveEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::HoverEnterLeaveEvents");
    m_pTrcAdminObjHoverMoveEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::HoverMoveEvents");
    m_pTrcAdminObjMouseClickEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::MouseClickEvents");
    m_pTrcAdminObjMouseMoveEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::MouseMoveEvents");
    m_pTrcAdminObjKeyEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::KeyEvents");
    m_pTrcAdminObjFocusEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::FocusEvents");
    m_pTrcAdminObjDragDropEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::DragDropEvents");
    m_pTrcAdminObjInputMethodEvents = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing::GraphObjs::QGraphicsItems", ClassName() + "::InputMethodEvents");

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(i_pParentItem);
        if (pGraphObjParent == nullptr) {
            strMthInArgs = "Parent: " + QString(i_pParentItem == nullptr ? "null" : "GraphicsItem");
        }
        else {
            strMthInArgs = "Parent: " + pGraphObjParent->path();
        }
        strMthInArgs += ", Text: " + i_strText;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable
            |QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);
}

//------------------------------------------------------------------------------
CGraphicsTextItem::~CGraphicsTextItem()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCtorsAndDtor);
    m_pTrcAdminObjCtorsAndDtor = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjItemChange);
    m_pTrcAdminObjItemChange = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjBoundingRect);
    m_pTrcAdminObjBoundingRect = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjCursor);
    m_pTrcAdminObjCursor = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjPaint);
    m_pTrcAdminObjPaint = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjSceneEvent);
    m_pTrcAdminObjSceneEvent = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjHoverEnterLeaveEvents);
    m_pTrcAdminObjHoverEnterLeaveEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjHoverMoveEvents);
    m_pTrcAdminObjHoverMoveEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseClickEvents);
    m_pTrcAdminObjMouseClickEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseMoveEvents);
    m_pTrcAdminObjMouseMoveEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjKeyEvents);
    m_pTrcAdminObjKeyEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjFocusEvents);
    m_pTrcAdminObjFocusEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjDragDropEvents);
    m_pTrcAdminObjDragDropEvents = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjInputMethodEvents);
    m_pTrcAdminObjInputMethodEvents = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphicsTextItem::setPath(const QString& i_strPath)
//------------------------------------------------------------------------------
{
    m_strPath = i_strPath;
}

//------------------------------------------------------------------------------
QString CGraphicsTextItem::path() const
//------------------------------------------------------------------------------
{
    return m_strPath;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphicsTextItem::toHtml() const
//------------------------------------------------------------------------------
{
    return QGraphicsTextItem::toHtml();
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::setHtml(const QString& i_strHtml)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strHtml;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHtml",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::setHtml(i_strHtml);
}

//------------------------------------------------------------------------------
QString CGraphicsTextItem::toPlainText() const
//------------------------------------------------------------------------------
{
    return QGraphicsTextItem::toPlainText();
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::setPlainText(const QString& i_strText)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strText;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPlainText",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::setPlainText(i_strText);
}

/*==============================================================================
public // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QFont CGraphicsTextItem::font() const
//------------------------------------------------------------------------------
{
    return QGraphicsTextItem::font();
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::setFont(const QFont& i_font)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_font.family();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setFont",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::setFont(i_font);
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::setDefaultTextColor(const QColor& i_col)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_col.name();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setDefaultTextColor",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::setDefaultTextColor(i_col);
}

//------------------------------------------------------------------------------
QColor CGraphicsTextItem::defaultTextColor() const
//------------------------------------------------------------------------------
{
    return QGraphicsTextItem::defaultTextColor();
}

/*==============================================================================
public // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphicsTextItem::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );
    QRectF rctBounding = QGraphicsTextItem::boundingRect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
QPainterPath CGraphicsTextItem::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );
    QPainterPath painterPath = QGraphicsTextItem::shape();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        const QGraphicsItem* pCThis = static_cast<const QGraphicsItem*>(this);
        QGraphicsItem* pVThis = const_cast<QGraphicsItem*>(pCThis);
        QString strMthRet = qPainterPath2Str(pVThis, painterPath);
        mthTracer.setMethodReturn(strMthRet);
    }
    return painterPath;
}

//------------------------------------------------------------------------------
bool CGraphicsTextItem::contains(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPoint2Str(i_pt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "contains",
        /* strAddInfo   */ strMthInArgs );
    bool bContains = QGraphicsTextItem::contains(i_pt);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bContains);
    }
    return bContains;
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::paint(QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pOption, QWidget* i_pWidget)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjPaint, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ZValue: " + QString::number(zValue());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::paint(i_pPainter, i_pOption, i_pWidget);
}

/*==============================================================================
public // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphicsTextItem::isObscuredBy(const QGraphicsItem* i_pItem) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal)) {
        const CGraphObj* pGraphObj = dynamic_cast<const CGraphObj*>(i_pItem);
        if (pGraphObj == nullptr) {
            strMthInArgs = "ByItem: " + QString(i_pItem == nullptr ? "null" : "GraphicsItem");
        }
        else {
            strMthInArgs = "ByItem: " + pGraphObj->path();
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "isObscuredBy",
        /* strAddInfo   */ strMthInArgs );
    bool bIsObscured = QGraphicsTextItem::isObscuredBy(i_pItem);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bIsObscured);
    }
    return bIsObscured;
}

//------------------------------------------------------------------------------
QPainterPath CGraphicsTextItem::opaqueArea() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "opaqueArea",
        /* strAddInfo   */ "" );
    QPainterPath painterPath = QGraphicsTextItem::opaqueArea();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        const QGraphicsItem* pCThis = static_cast<const QGraphicsItem*>(this);
        QGraphicsItem* pVThis = const_cast<QGraphicsItem*>(pCThis);
        QString strMthRet = qPainterPath2Str(pVThis, painterPath);
        mthTracer.setMethodReturn(strMthRet);
    }
    return painterPath;
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::setTextWidth(qreal i_fWidth_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fWidth_px) + " px";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTextWidth",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::setTextWidth(i_fWidth_px);
}

//------------------------------------------------------------------------------
qreal CGraphicsTextItem::textWidth() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "textWidth",
        /* strAddInfo   */ "" );
    double fWidth_px = QGraphicsTextItem::textWidth();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(QString::number(fWidth_px) + " px");
    }
    return fWidth_px;
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::adjustSize()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "adjustSize",
        /* strAddInfo   */ "" );
    QGraphicsTextItem::adjustSize();
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::setDocument(QTextDocument* i_pDocument)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pDocument == nullptr ? "null" : i_pDocument->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setDocument",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::setDocument(i_pDocument);
}

//------------------------------------------------------------------------------
QTextDocument* CGraphicsTextItem::document() const
//------------------------------------------------------------------------------
{
    return QGraphicsTextItem::document();
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::setTextInteractionFlags(Qt::TextInteractionFlags i_flags)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qTextInteractionFlags2Str(i_flags);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTextInteractionFlags",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::setTextInteractionFlags(i_flags);
}

//------------------------------------------------------------------------------
Qt::TextInteractionFlags CGraphicsTextItem::textInteractionFlags() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "textInteractionFlags",
        /* strAddInfo   */ "" );
    Qt::TextInteractionFlags flags = QGraphicsTextItem::textInteractionFlags();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qTextInteractionFlags2Str(flags));
    }
    return flags;
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::setTabChangesFocus(bool i_bTabChangesFocus)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bTabChangesFocus);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTabChangesFocus",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::setTabChangesFocus(i_bTabChangesFocus);
}

//------------------------------------------------------------------------------
bool CGraphicsTextItem::tabChangesFocus() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "tabChangesFocus",
        /* strAddInfo   */ "");
    bool bChangesFocus = QGraphicsTextItem::tabChangesFocus();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bChangesFocus);
    }
    return bChangesFocus;
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::setOpenExternalLinks(bool i_bOpen)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bOpen);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setOpenExternalLinks",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::setOpenExternalLinks(i_bOpen);
}

//------------------------------------------------------------------------------
bool CGraphicsTextItem::openExternalLinks() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "openExternalLinks",
        /* strAddInfo   */ "" );
    bool bOpen = QGraphicsTextItem::openExternalLinks();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bOpen);
    }
    return bOpen;
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::setTextCursor(const QTextCursor& i_cursor)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCursor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTextCursor",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::setTextCursor(i_cursor);
}

//------------------------------------------------------------------------------
QTextCursor CGraphicsTextItem::textCursor() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "textCursor",
        /* strAddInfo   */ "" );
    QTextCursor cursor = QGraphicsTextItem::textCursor();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        //mthTracer.setMethodReturn(qTextCursor2Str(cursor));
    }
    return cursor;
}

/*==============================================================================
protected // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphicsTextItem::sceneEvent(QEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjSceneEvent, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjSceneEvent,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "sceneEvent",
        /* strAddInfo   */ strMthInArgs );
    bool bHandled = QGraphicsTextItem::sceneEvent(i_pEv);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
        mthTracer.setMethodReturn(bHandled);
    }
    return bHandled;
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::mousePressEvent(QGraphicsSceneMouseEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::mousePressEvent(i_pEv);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::mouseMoveEvent(i_pEv);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::mouseReleaseEvent(i_pEv);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::mouseDoubleClickEvent(i_pEv);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneContextMenuEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "contextMenuEvent",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::contextMenuEvent(i_pEv);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::keyPressEvent(QKeyEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qKeyEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strMthInArgs );

    if (i_pEv->key() == Qt::Key_Escape) {
        clearFocus();
    }
    else {
        QGraphicsTextItem::keyPressEvent(i_pEv);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::keyReleaseEvent(QKeyEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qKeyEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsTextItem::keyReleaseEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::focusInEvent(QFocusEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjFocusEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qFocusEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjFocusEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "focusInEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
    }

    QGraphicsTextItem::focusInEvent(i_pEv);
    emit_focusChanged(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::focusOutEvent(QFocusEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjFocusEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qFocusEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjFocusEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "focusOutEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
    }

    QGraphicsTextItem::focusOutEvent(i_pEv);
    emit_focusChanged(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::dragEnterEvent(QGraphicsSceneDragDropEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDragDropEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneDragDropEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDragDropEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dragEnterEvent",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::dragEnterEvent(i_pEv);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::dragLeaveEvent(QGraphicsSceneDragDropEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDragDropEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneDragDropEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDragDropEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dragLeaveEvent",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::dragLeaveEvent(i_pEv);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::dragMoveEvent(QGraphicsSceneDragDropEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDragDropEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneDragDropEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDragDropEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dragMoveEvent",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::dragMoveEvent(i_pEv);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::dropEvent(QGraphicsSceneDragDropEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjDragDropEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneDragDropEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjDragDropEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dropEvent",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::dropEvent(i_pEv);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::inputMethodEvent(QInputMethodEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjInputMethodEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qInputMethodEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjInputMethodEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "inputMethodEvent",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsTextItem::inputMethodEvent(i_pEv);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEnterLeaveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEnterLeaveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );

    // Ignore hover events if any object should be or is currently being created.
    CDrawingScene* pDrawingScene = dynamic_cast<CDrawingScene*>(scene());
    if (pDrawingScene != nullptr && pDrawingScene->getCurrentDrawingTool() == nullptr) {
        QGraphicsItem_setCursor(Qt::IBeamCursor);
    }
    //QGraphicsTextItem::hoverEnterEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::hoverMoveEvent(QGraphicsSceneHoverEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    // Ignore hover events if any object should be or is currently being created.
    CDrawingScene* pDrawingScene = dynamic_cast<CDrawingScene*>(scene());
    if (pDrawingScene != nullptr && pDrawingScene->getCurrentDrawingTool() == nullptr) {
        QGraphicsItem_setCursor(Qt::IBeamCursor);
    }
    //QGraphicsTextItem::hoverMoveEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEnterLeaveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEnterLeaveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem_unsetCursor();
    //QGraphicsTextItem::hoverLeaveEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

/*==============================================================================
protected // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphicsTextItem::inputMethodQuery(Qt::InputMethodQuery i_query) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjInputMethodEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qInputMethodQuery2Str(i_query) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjInputMethodEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "inputMethodQuery",
        /* strAddInfo   */ strMthInArgs );
    QVariant var = QGraphicsTextItem::inputMethodQuery(i_query);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(var.toString());
    }
    return var;
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphicsTextItem::QGraphicsItem_setCursor(const QCursor& i_cursor)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCursor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCursorShape2Str(i_cursor.shape());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setCursor",
        /* strAddInfo   */ strMthInArgs );

    setCursor(i_cursor);
}

//------------------------------------------------------------------------------
void CGraphicsTextItem::QGraphicsItem_unsetCursor()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "unsetCursor",
        /* strAddInfo   */ "" );

    unsetCursor();
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphicsTextItem::emit_focusChanged(QFocusEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjFocusEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qFocusEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjFocusEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "emit_focusChanged",
        /* strAddInfo   */ strMthInArgs );

    emit focusChanged(i_pEv);
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphicsTextItem::traceGraphicsItemStates(
    ZS::System::CMethodTracer& i_mthTracer,
    ZS::System::EMethodDir i_mthDir,
    const QString& i_strFilter,
    ZS::System::ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (m_iTraceItemStatesBlockedCounter > 0) {
        return;
    }
    if (!i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        return;
    }
    QString strRuntimeInfo;
    if (i_strFilter.isEmpty() || i_strFilter.contains("Common")) {
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = " . ";
        strRuntimeInfo += "IsSelected: " + bool2Str(isSelected()) +
            ", IsVisible: " + bool2Str(isVisible()) +
            ", IsEnabled: " + bool2Str(isEnabled()) +
            ", HasCursor: " + bool2Str(hasCursor()) +
            ", HasFocus: " + bool2Str(hasFocus());
        i_mthTracer.trace(strRuntimeInfo);
    }
    if (i_strFilter.isEmpty() || i_strFilter.contains("HoverEvents")) {
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "AcceptHoverEvents: " + bool2Str(acceptHoverEvents());
        i_mthTracer.trace(strRuntimeInfo);
    }
    if (i_strFilter.isEmpty() || i_strFilter.contains("MouseButtons")) {
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "AcceptedMouseButtons {" + qMouseButtons2Str(acceptedMouseButtons()) + "}";
        i_mthTracer.trace(strRuntimeInfo);
    }
    if (i_strFilter.isEmpty() || i_strFilter.contains("Flags")) {
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "Flags {" + qGraphicsItemFlags2Str(flags()) + "}";
        i_mthTracer.trace(strRuntimeInfo);
    }
}
