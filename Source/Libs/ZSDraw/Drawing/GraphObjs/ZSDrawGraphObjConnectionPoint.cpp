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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtGui/QBitmap>
#include <QtGui/qevent.h>
#include <QtGui/QPainter>

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsSceneEvent>
#include <QtGui/QStyleOption>
#else
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtWidgets/QStyleOption>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CGraphObjConnectionPoint : public CGraphObj, public QGraphicsEllipseItem
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

qint64 CGraphObjConnectionPoint::s_iInstCount = 0;

/*==============================================================================
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjConnectionPoint::s_painterRenderHints = QPainter::Antialiasing;

double CGraphObjConnectionPoint::s_fRadius_px = 5.0;
double CGraphObjConnectionPoint::s_fInnerCircleRadius_px = 2.0;
int CGraphObjConnectionPoint::s_iPenWidth_px = 2;
QColor CGraphObjConnectionPoint::s_colPen = Qt::black;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjConnectionPoint::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
double CGraphObjConnectionPoint::defaultRadiusInPx()
//------------------------------------------------------------------------------
{
    return s_fRadius_px;
}

//------------------------------------------------------------------------------
double CGraphObjConnectionPoint::defaultInnerCircleRadiusInPx()
//------------------------------------------------------------------------------
{
    return s_fInnerCircleRadius_px;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjConnectionPoint::CGraphObjConnectionPoint(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObjEllipse(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameConnections,
        /* type                */ EGraphObjTypeConnectionPoint,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeConnectionPoint),
        /* strObjName          */ i_strObjName.isEmpty() ? "ConnectionPoint" + QString::number(s_iInstCount) : i_strObjName),
    m_lstConnectionLines(),
    m_fInnerCircleRadius_perCent(100.0*(s_fInnerCircleRadius_px/s_fRadius_px)),
    m_fRadius_px(s_fRadius_px),
    m_selPt()
{
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    createTraceAdminObjs("Connections::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ObjName: " + i_strObjName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_strlstPredefinedLabelNames.append(c_strLabelName);
    addLabel(c_strLabelName, i_strObjName, ESelectionPointType::BoundingRectangle, ESelectionPoint::Center);

    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameCenter);
    addGeometryLabel(c_strGeometryLabelNameCenter, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::Center);

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable
            |QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
    setAcceptHoverEvents(true);
}

//------------------------------------------------------------------------------
CGraphObjConnectionPoint::~CGraphObjConnectionPoint()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_bDtorInProgress = true;
    emit_aboutToBeDestroyed();

    if (m_lstConnectionLines.size() > 0) {
        for (int idxLine = m_lstConnectionLines.count()-1; idxLine >= 0; idxLine--) {
            CGraphObjConnectionLine* pGraphObjCnctLine = m_lstConnectionLines[idxLine];
            m_lstConnectionLines[idxLine] = nullptr;
            try {
                // The dtor of the connection line calls "removeConnectionLine" as a reentry.
                delete pGraphObjCnctLine;
            }
            catch(...) {
            }
            pGraphObjCnctLine = nullptr;
        }
    }
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjConnectionPoint::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeConnectionPoint;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjConnectionPoint::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );
    return nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjConnectionPoint::pathNameOfLinkedObject() const
//------------------------------------------------------------------------------
{
    QString strPath;
    if (m_selPt.m_pGraphObj != nullptr) {
        strPath = m_selPt.m_pGraphObj->path();
    }
    return strPath;
}

//------------------------------------------------------------------------------
QString CGraphObjConnectionPoint::myPathName() const
//------------------------------------------------------------------------------
{
    QString strPath;
    if (m_selPt.m_pGraphObj != nullptr) {
        strPath = pathNameOfLinkedObject();
        strPath = m_selPt.m_pGraphObj->tree()->buildPathStr(strPath, m_strName);
    }
    return strPath;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the type of the selection point, the linked object and the
           position at the linked object the selection point is linked to.

    Selection points are differentiated into selection points on the bounding
    rectangle around the graphical object or into polygon shape points.
*/
SGraphObjSelectionPoint CGraphObjConnectionPoint::getSelectionPoint() const
//------------------------------------------------------------------------------
{
    return m_selPt;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjConnectionPoint::appendConnectionLine(CGraphObjConnectionLine* i_pGraphObjCnctLine)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "CnctLine: " + QString(i_pGraphObjCnctLine == nullptr ? "null" : i_pGraphObjCnctLine->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "appendConnectionLine",
        /* strAddInfo   */ strMthInArgs );

    bool bConnected = false;
    if (i_pGraphObjCnctLine != nullptr) {
        int idxLineTmp = findConnectionLineIdx(i_pGraphObjCnctLine);
        // If the connection line is not yet connected with me ...
        if (idxLineTmp < 0) {
            m_lstConnectionLines.append(i_pGraphObjCnctLine);
            bConnected = true;
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bool2Str(bConnected));
    }
    return bConnected;
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::removeConnectionLine(CGraphObjConnectionLine* i_pGraphObjCnctLine)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "CnctLine: " + QString(i_pGraphObjCnctLine == nullptr ? "null" : i_pGraphObjCnctLine->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "removeConnectionLine",
        /* strAddInfo   */ strMthInArgs );

    if (i_pGraphObjCnctLine != nullptr) {
        int iLineIdx = findConnectionLineIdx(i_pGraphObjCnctLine);
        if (iLineIdx < 0 || iLineIdx >= m_lstConnectionLines.count()) {
            // If a connection line is destroyed by the dtor of the connection point
            // "removeConnectionLine" is called as a reentry by the dtor of the connection line.
        }
        else {
            m_lstConnectionLines.removeAt(iLineIdx);
        }
    }
}

//------------------------------------------------------------------------------
int CGraphObjConnectionPoint::findConnectionLineIdx(CGraphObjConnectionLine* i_pGraphObjCnctLine)
//------------------------------------------------------------------------------
{
    int idxLine = -1;
    if (i_pGraphObjCnctLine != nullptr) {
        for (int idxLineTmp = 0; idxLineTmp < m_lstConnectionLines.count(); idxLineTmp++) {
            if (m_lstConnectionLines[idxLineTmp] == i_pGraphObjCnctLine) {
                idxLine = idxLineTmp;
                break;
            }
        }
    }
    return idxLine;
}

//------------------------------------------------------------------------------
int CGraphObjConnectionPoint::getConnectionLinesCount() const
//------------------------------------------------------------------------------
{
    return m_lstConnectionLines.count();
}

//------------------------------------------------------------------------------
CGraphObjConnectionLine* CGraphObjConnectionPoint::getConnectionLine(int i_iLineIdx)
//------------------------------------------------------------------------------
{
    CGraphObjConnectionLine* pGraphObjCnctLine = nullptr;
    if (i_iLineIdx >= 0 && i_iLineIdx < m_lstConnectionLines.count()) {
        pGraphObjCnctLine = m_lstConnectionLines[i_iLineIdx];
    }
    return pGraphObjCnctLine;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setInnerCircleRadiusInPerCent(double i_fRadius_perCent)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Radius: " + QString::number(i_fRadius_perCent) + " %";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setInnerCircleRadiusInPerCent",
        /* strAddInfo   */ strMthInArgs );

    if (m_fInnerCircleRadius_perCent != i_fRadius_perCent) {
        m_fInnerCircleRadius_perCent = i_fRadius_perCent;
        update();
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setInnerCircleRadiusInPx(double i_fRadius_px)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Radius: " + QString::number(i_fRadius_px) + " px";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setInnerCircleRadiusInPx",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        QRectF rctBounding = pGraphicsItem->boundingRect();
        if (rctBounding.width() != 0.0) {
            m_fInnerCircleRadius_perCent = 100.0 * fabs(i_fRadius_px / (rctBounding.width()/2.0));
        }
        else {
            m_fInnerCircleRadius_perCent = 100.0;
        }
        update();
    }
}

//------------------------------------------------------------------------------
double CGraphObjConnectionPoint::getInnerCircleRadiusInPerCent()
//------------------------------------------------------------------------------
{
    return m_fInnerCircleRadius_perCent;
}

//------------------------------------------------------------------------------
double CGraphObjConnectionPoint::getInnerCircleRadiusInPx()
//------------------------------------------------------------------------------
{
    double fWidth = 0.0;
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        QRectF rctBounding = pGraphicsItem->boundingRect();
        if (rctBounding.width() != 0.0) {
            fWidth = (m_fInnerCircleRadius_perCent/100.0) * (rctBounding.width()/2.0);
        }
    }
    return fWidth;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "OldSettings {" + i_drawSettingsOld.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjConnectionPoint::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = getBoundingRect();
    if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
        // Half pen width of the selection rectangle would be enough.
        // But the whole pen width is also not a bad choice.
        rctBounding.adjust(-2.0, -2.0, 2.0, 2.0);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Reimplements QGraphicsItem::shape.
*/
QPainterPath CGraphObjConnectionPoint::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ myPathName(),
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );
    QPainterPath painterPath = QGraphicsEllipseItem::shape();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        const QGraphicsItem* pCThis = static_cast<const QGraphicsItem*>(this);
        QGraphicsItem* pVThis = const_cast<QGraphicsItem*>(pCThis);
        QString strMthRet = qPainterPath2Str(pVThis, painterPath);
        mthTracer.setMethodReturn(strMthRet);
    }
    return painterPath;
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::paint(
    QPainter* i_pPainter,
    const QStyleOptionGraphicsItem* /*i_pStyleOption*/,
    QWidget* /*i_pWdgt*/ )
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

    i_pPainter->save();
    i_pPainter->setRenderHints(s_painterRenderHints);

    QPen pn;
    QBrush brush;
    QRectF rctBounding = getBoundingRect();

    if (m_fInnerCircleRadius_perCent < 100.0) {
        if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHighlighted || isSelected())) {
            if (isSelected()) {
                pn.setColor(s_selectionColor);
                pn.setWidth(3 + m_drawSettings.penWidth());
            }
            else {
                pn.setColor(s_highlightColor);
                pn.setWidth(3 + m_drawSettings.penWidth());
            }
            pn.setStyle(Qt::SolidLine);
            QPainterPath outline;
            outline.moveTo(rctBounding.topLeft());
            outline.lineTo(rctBounding.topRight());
            outline.lineTo(rctBounding.bottomRight());
            outline.lineTo(rctBounding.bottomLeft());
            outline.lineTo(rctBounding.topLeft());
            i_pPainter->strokePath(outline, pn);
        }
    }

    double fInnerCircleRadius_px = getInnerCircleRadiusInPx();
    if (fInnerCircleRadius_px >= 1.0) {
        if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHighlighted || isSelected())) {
            if (isSelected()) {
                pn.setColor(s_selectionColor);
                pn.setWidth(3 + m_drawSettings.penWidth());
            }
            else {
                pn.setColor(s_highlightColor);
                pn.setWidth(3 + m_drawSettings.penWidth());
            }
            pn.setStyle(Qt::SolidLine);
        }
        i_pPainter->setPen(pn);
        i_pPainter->setBrush(brush);

        QRectF rctInnerCircle;
        rctInnerCircle.setLeft(rctBounding.center().x() - fInnerCircleRadius_px);
        rctInnerCircle.setTop(rctBounding.center().y() - fInnerCircleRadius_px);
        rctInnerCircle.setWidth(2.0 * fInnerCircleRadius_px);
        rctInnerCircle.setHeight(2.0 * fInnerCircleRadius_px);
        i_pPainter->drawEllipse(rctInnerCircle);
    }

    i_pPainter->restore();
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    // Ignore hover events if any object should be or is currently being created.
    if (m_pDrawingScene->getCurrentDrawingTool() == nullptr) {
        QGraphicsItem_setCursor(Qt::SizeAllCursor);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
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
    if (m_pDrawingScene->getCurrentDrawingTool() == nullptr) {
        QGraphicsItem_setCursor(Qt::SizeAllCursor);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
} 

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    QGraphicsItem_unsetCursor();

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
        traceThisPositionInfo(mthTracer, EMethodDir::Enter, "Common");
    }

    bool bCallBaseMouseEventHandler = true;
    if (i_pEv->button() == Qt::LeftButton) {
    }
    else if (i_pEv->button() == Qt::RightButton) {
        showContextMenu(i_pEv);
        bCallBaseMouseEventHandler = false;
    }
    if (bCallBaseMouseEventHandler) {
        // Forward the mouse event to the base implementation.
        // This will select the item.
        QGraphicsEllipseItem::mousePressEvent(i_pEv);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
        traceThisPositionInfo(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Enter, "Common");
    }

    // Forward the mouse event to the items base implementation.
    QGraphicsEllipseItem::mouseReleaseEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    // When double clicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".
    //QGraphicsRectItem::mouseDoubleClickEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    // Forward the mouse event to the graphics item base implementation.
    // This will move the item resulting in an itemChange call with PositionHasChanged.
    QGraphicsEllipseItem::mouseMoveEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjConnectionPoint::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
//------------------------------------------------------------------------------
{
    if (m_bDtorInProgress) {
        return i_value;
    }
    if (m_iItemChangeBlockedCounter > 0) {
        return i_value;
    }

    CTrcAdminObj* pTrcAdminObj = selectTraceAdminObj(i_change);
    QString strMthInArgs;
    if (areMethodCallsActive(pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsItemChange2Str(i_change, i_value);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    CGraphObj* pGraphObjThis = dynamic_cast<CGraphObj*>(this);
    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);

    QVariant valChanged = i_value;

    bool bGeometryChanged = false;
    bool bSelectedChanged = false;
    bool bZValueChanged = false;
    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
            if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                tracePositionInfo(mthTracer, EMethodDir::Enter);
            }
            bGeometryChanged = true;
            bTreeEntryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged) {
        if (m_iItemChangeUpdatePhysValCoorsBlockedCounter == 0) {
        }
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemPositionHasChanged) {
        if (m_iItemChangeUpdatePhysValCoorsBlockedCounter == 0) {
            if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                tracePositionInfo(mthTracer, EMethodDir::Enter);
            }
            bGeometryChanged = true;
        }
        bTreeEntryChanged = true;
    }
    else if(i_change == ItemSelectedHasChanged) {
        //QGraphicsItem_prepareGeometryChange();
        // The connection point does not have selection points but is a selection point itself.
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            bringToFront();
        }
        else {
            setEditMode(EEditMode::None);
            resetStackingOrderValueToOriginalValue(); // restore ZValue as before selecting the object
        }
        bSelectedChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemZValueHasChanged) {
        bZValueChanged = true;
        bTreeEntryChanged = true;
    }

    if (bGeometryChanged) {
        if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            tracePositionInfo(mthTracer, EMethodDir::Leave);
        }
        emit_geometryOnSceneChanged();
    }
    if (bSelectedChanged) {
        emit_selectedChanged(isSelected());
    }
    if (bZValueChanged) {
        emit_zValueChanged(zValue());
    }
    if (bTreeEntryChanged && m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }

    valChanged = QGraphicsItem::itemChange(i_change, i_value);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qGraphicsItemChange2Str(i_change, valChanged, false);
        mthTracer.setMethodReturn(strMthRet);
    }
    return valChanged;
}
