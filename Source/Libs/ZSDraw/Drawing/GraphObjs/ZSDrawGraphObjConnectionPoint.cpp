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
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjConnectionPointPropertiesDlg.h"
#include "ZSDraw/Common/ZSDrawAux.h"
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

double CGraphObjConnectionPoint::s_fDefaultWidth_px = 7.0;

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
double CGraphObjConnectionPoint::defaultWidthInPx()
//------------------------------------------------------------------------------
{
    return s_fDefaultWidth_px;
}

//------------------------------------------------------------------------------
QSizeF CGraphObjConnectionPoint::defaultSizeInPx()
//------------------------------------------------------------------------------
{
    return QSizeF(s_fDefaultWidth_px, s_fDefaultWidth_px);
}


/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjConnectionPoint::CGraphObjConnectionPoint(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameConnections,
        /* type                */ EGraphObjTypeConnectionPoint,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeConnectionPoint),
        /* strObjName          */ i_strObjName.isEmpty() ? "ConnectionPoint" + QString::number(s_iInstCount) : i_strObjName),
    QGraphicsEllipseItem(),
    m_selPt(),
    m_lstConnectionLines(),
    m_rectOrig(),
    m_physValRectOrig(*m_pDrawingScene),
    m_physValRectScaled(*m_pDrawingScene),
    m_physValRectScaledAndRotated(*m_pDrawingScene)
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
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameWidth);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameHeight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameAngle);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameTopLeft);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameTopRight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameBottomRight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameBottomLeft);

    for (const QString& strLabelName : m_strlstGeometryLabelNames) {
        if (strLabelName == c_strGeometryLabelNameCenter) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::Center);
        }
        else if (strLabelName == c_strGeometryLabelNameWidth) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryLength, ESelectionPoint::LeftCenter, ESelectionPoint::RightCenter);
        }
        else if (strLabelName == c_strGeometryLabelNameHeight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryLength, ESelectionPoint::TopCenter, ESelectionPoint::BottomCenter);
        }
        else if (strLabelName == c_strGeometryLabelNameAngle) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryAngle, ESelectionPoint::LeftCenter, ESelectionPoint::RightCenter);
        }
        else if (strLabelName == c_strGeometryLabelNameTopLeft) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::TopLeft);
        }
        else if (strLabelName == c_strGeometryLabelNameTopRight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::TopRight);
        }
        else if (strLabelName == c_strGeometryLabelNameBottomRight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::BottomRight);
        }
        else if (strLabelName == c_strGeometryLabelNameBottomLeft) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::BottomLeft);
        }
    }

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable
            |QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
    QGraphicsItem_setAcceptHoverEvents(true);

    setStackingOrderValue(c_fStackingOrderOffsetConnectionPoints, ERowVersion::Original);
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
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to create a user defined dialog.
*/
void CGraphObjConnectionPoint::openFormatGraphObjsDialog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "openFormatGraphObjsDialog",
        /* strAddInfo   */ "" );

    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Format Connection Point";
    CDlgGraphObjConnectionPointProperties* pDlg = CDlgGraphObjConnectionPointProperties::GetInstance(this);
    if( pDlg == nullptr ) {
        pDlg = CDlgGraphObjConnectionPointProperties::CreateInstance(strDlgTitle, this);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->setModal(false);
        pDlg->show();
    }
    else {
        if( pDlg->isHidden() ) {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
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
QString CGraphObjConnectionPoint::path() const
//------------------------------------------------------------------------------
{
    QString strPath;
    if (m_selPt.m_pGraphObj != nullptr) {
        strPath = pathNameOfLinkedObject();
        strPath = m_selPt.m_pGraphObj->tree()->buildPathStr(strPath, m_strName);
    }
    else {
        strPath = CIdxTreeEntry::path();
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

    bool bDrawSettingsChanged = (m_drawSettings != i_drawSettingsOld);
    if (bDrawSettingsChanged) {
        update();
        emit_drawSettingsChanged();
    }
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::appendConnectionLine(CGraphObjConnectionLine* i_pGraphObjCnctLine)
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceConnectionLines(mthTracer, EMethodDir::Enter);
    }

    if (i_pGraphObjCnctLine != nullptr) {
        int idxLineTmp = getConnectionLineIdx(i_pGraphObjCnctLine);
        // If the connection line is not yet connected with me ...
        if (idxLineTmp < 0) {
            m_lstConnectionLines.append(i_pGraphObjCnctLine);
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceConnectionLines(mthTracer, EMethodDir::Leave);
    }
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceConnectionLines(mthTracer, EMethodDir::Enter);
    }

    if (i_pGraphObjCnctLine != nullptr) {
        int iLineIdx = getConnectionLineIdx(i_pGraphObjCnctLine);
        if (iLineIdx < 0 || iLineIdx >= m_lstConnectionLines.count()) {
            // If a connection line is destroyed by the dtor of the connection point
            // "removeConnectionLine" is called as a reentry by the dtor of the connection line.
        }
        else {
            m_lstConnectionLines.removeAt(iLineIdx);
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceConnectionLines(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
int CGraphObjConnectionPoint::getConnectionLineIdx(CGraphObjConnectionLine* i_pGraphObjCnctLine)
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
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the item's rectangle to the given rectangle.

    Depending on the Y axis scale orientation of the drawing scene the coordinates
    must be passed either relative to the top left or relative to the bottom left
    corner of the parent item's bounding rectangle.

    @param [in] i_physValRect
        Rectangle to be set in parent coordinates, depending on the Y scale orientation
        relative to the top left or bottom left corner of parent item's bounding rectangle.
        The passed rectangle may be rotated (the angle may be in range 0 .. 360°).
*/
void CGraphObjConnectionPoint::setRect( const CPhysValRect& i_physValRect )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValRect.toString(true);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    bool bGeometryOnSceneChanged = false;
    if (m_physValRectScaledAndRotated != i_physValRect) {
        // Depending on the Y scale orientation of the drawing scene the rectangle coordinates
        // have been passed either relative to the top left or bottom left corner of the
        // parent item's bounding rectangle.
        // The coordinates need to be transformed into the local coordinate system of the graphical
        // object whose origin point is the center of the objects bounding rectangle.
        QPointF ptPosPrev = pos();
        QRectF rectF;
        CPhysVal physValAngle;
        QPointF ptPos = getItemPosAndLocalCoors(i_physValRect, rectF, physValAngle);

        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsItem_prepareGeometryChange();

        {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdatePhysValCoorsBlockedCounter);
            CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

            // Store physical coordinates.
            CPhysValRect physValRect(i_physValRect);
            physValRect.setAngle(0.0);
            setPhysValRectOrig(physValRect);
            setPhysValRectScaled(physValRect);
            setPhysValRectScaledAndRotated(i_physValRect);

            // Set the rectangle in local coordinate system.
            // Also note that itemChange must not overwrite the current values (refCountGuard).
            setRectOrig(rectF);
            QGraphicsEllipseItem_setRect(rectF);

            // Move the object to the parent position.
            // This has to be done after resizing the groups rectangle which updates the local coordinates
            // of the rectangle with origin (0/0) at the rectangle's center point.
            // "setPos" will trigger an itemChange call which will update the position of the
            // selection points and labels. To position the selection points and labels correctly
            // the local coordinate system must be up-to-date.
            // Also note that itemChange must not overwrite the current rectangle coordinates (refCountGuard).
            if (ptPos != ptPosPrev) {
                QGraphicsItem_setPos(ptPos);
            }
            if (physValAngle != m_physValRotationAngle) {
                m_physValRotationAngle = physValAngle;
                QGraphicsItem_setRotation(m_physValRotationAngle.getVal(Units.Angle.Degree));
            }
        }
        // If the geometry of the parent on the scene of this item changes, also the geometry
        // on the scene of this item is changed.
        bGeometryOnSceneChanged = true;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }
    // Emit signal after updated position info has been traced.
    if (bGeometryOnSceneChanged) {
        emit_geometryOnSceneChanged();
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setRect(const QPointF& i_pTL, const QPointF& i_pBR, const ZS::PhysVal::CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "TL {" + qPoint2Str(i_pTL) + "}, BR {" + qPoint2Str(i_pBR) + "} " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );

    setRect(CPhysValRect(*m_pDrawingScene, i_pTL, i_pBR, i_unit));
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setRect(const QPointF& i_pTL, const QSizeF& i_size, const ZS::PhysVal::CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "TL {" + qPoint2Str(i_pTL) + "}, Size {" + qSize2Str(i_size) + "} " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );

    setRect(CPhysValRect(*m_pDrawingScene, i_pTL, i_size, i_unit));
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setRect(const CPhysValPoint& i_physValTL, const CPhysValPoint& i_physValBR)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "TL {" + i_physValTL.toString() + "}, BR {" + i_physValBR.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );

    setRect(CPhysValRect(i_physValTL, i_physValBR));
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setRect(const CPhysValPoint& i_physValTL, const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "TL {" + i_physValTL.toString() + "}, Size {" + i_physValSize.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );

    setRect(CPhysValRect(i_physValTL, i_physValSize));
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene,
           returns the item's coordinates either
           - relative to the top left corner or
           - relative to the bottom right corner
           of the parent's bounding rectangle in the current unit of the drawing scene.
*/
CPhysValRect CGraphObjConnectionPoint::getRect() const
//------------------------------------------------------------------------------
{
    return getRect(m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene,
           returns the item's coordinates either
           - relative to the top left corner or
           - relative to the bottom right corner
           of the parent's bounding rectangle in the current unit of the drawing scene.

    @param [in] i_unit
        Unit in which the line coordinates should be returned.

    @return Physical rectangle (scaled and rotated) in parent or scene coordinates,
            if the object has no parent group.
*/
CPhysValRect CGraphObjConnectionPoint::getRect(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (parentGroup() != nullptr) {
        return parentGroup()->convert(m_physValRectScaledAndRotated, i_unit);
    }
    else {
        return m_pDrawingScene->convert(m_physValRectScaledAndRotated, i_unit);
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString(true) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setCenter(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjConnectionPoint::getCenter() const
//------------------------------------------------------------------------------
{
    return getRect().center();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjConnectionPoint::getCenter(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).center();
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setSize(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qSize2Str(i_size) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setSize(i_size);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setSize(const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValSize.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setSize(i_physValSize);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObjConnectionPoint::getSize() const
//------------------------------------------------------------------------------
{
    return getRect().size();
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObjConnectionPoint::getSize(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).size();
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setWidth(double i_fWidth)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fWidth);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidth(i_fWidth);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setWidth(const CPhysVal& i_physValWidth)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidth(i_physValWidth);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setWidthByMovingLeftCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPoint2Str(i_pt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidthByMovingLeftCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidthByMovingLeftCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValPoint.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidthByMovingLeftCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidthByMovingLeftCenter(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setWidthByMovingRightCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPoint2Str(i_pt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidthByMovingRightCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidthByMovingRightCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValPoint.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setWidthByMovingRightCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidthByMovingRightCenter(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjConnectionPoint::getWidth() const
//------------------------------------------------------------------------------
{
    return getRect().width();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjConnectionPoint::getWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).width();
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setHeight(double i_fHeight)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fHeight);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeight(i_fHeight);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setHeight(const CPhysVal& i_physValHeight)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeight(i_physValHeight);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setHeightByMovingTopCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPoint2Str(i_pt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeightByMovingTopCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeightByMovingTopCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValPoint.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeightByMovingTopCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeightByMovingTopCenter(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setHeightByMovingBottomCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPoint2Str(i_pt);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeightByMovingBottomCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeightByMovingBottomCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValPoint.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setHeightByMovingBottomCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeightByMovingBottomCenter(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjConnectionPoint::getHeight() const
//------------------------------------------------------------------------------
{
    return getRect().height();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjConnectionPoint::getHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).height();
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setTopLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTopLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopLeft(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setTopLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTopLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopLeft(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjConnectionPoint::getTopLeft() const
//------------------------------------------------------------------------------
{
    return getRect().topLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjConnectionPoint::getTopLeft(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).topLeft();
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setTopRight(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTopRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopRight(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setTopRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setTopRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopRight(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjConnectionPoint::getTopRight() const
//------------------------------------------------------------------------------
{
    return getRect().topRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjConnectionPoint::getTopRight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).topRight();
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setBottomRight(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setBottomRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomRight(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setBottomRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setBottomRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomRight(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjConnectionPoint::getBottomRight() const
//------------------------------------------------------------------------------
{
    return getRect().bottomRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjConnectionPoint::getBottomRight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).bottomRight();
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setBottomLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setBottomLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomLeft(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setBottomLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setBottomLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomLeft(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjConnectionPoint::getBottomLeft() const
//------------------------------------------------------------------------------
{
    return getRect().bottomLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjConnectionPoint::getBottomLeft(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).bottomLeft();
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overloaded method to set the position of the ellipse.

    Same as setCenter.

    @param [in] i_physValPos
        New position of the graphical object.
*/
void CGraphObjConnectionPoint::setPosition(const CPhysValPoint& i_physValPos)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPos.toString(true) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPosition",
        /* strAddInfo   */ strMthInArgs );

    setCenter(i_physValPos);
}

//------------------------------------------------------------------------------
/*! @brief Overloaded method to set the clockwise rotation angle, in degrees,
           around the Z axis.

    @note This method must be overwritten. Otherwise because of implicit conversion
          instead of setRotationAngle(double) the overloaded method
          setRotationAngle(const CPhysVal&) would be called. And the physValAngle
          value would not contain the unit.

    @param [in] i_fAngle_degree
        Rotation angle in degree.
*/
void CGraphObjConnectionPoint::setRotationAngle(double i_fAngle_degree)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fAngle_degree, 'f', 3);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRotationAngle",
        /* strAddInfo   */ strMthInArgs );

    setRotationAngle(CPhysVal(i_fAngle_degree, Units.Angle.Degree, 0.1));
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::setRotationAngle(const CPhysVal& i_physValAngle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValAngle.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRotationAngle",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setAngle(i_physValAngle);
    setRect(physValRect);
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplements the virtual method of base class CGraphObj.
*/
QRectF CGraphObjConnectionPoint::getBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = rect(); // in local coordinates
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Returns the rotated, physical bounding rectangle.
*/
CPhysValRect CGraphObjConnectionPoint::getPhysValBoundingRect(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValBoundingRect",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectBounding = m_physValRectScaledAndRotated;
    if (parentGroup() != nullptr) {
        physValRectBounding = parentGroup()->convert(physValRectBounding, i_unit);
    }
    else {
        physValRectBounding = m_pDrawingScene->convert(physValRectBounding, i_unit);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValRectBounding.toString(true) + "}");
    }
    return physValRectBounding;
}

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::showSelectionPoints(TSelectionPointTypes i_selPts)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + selectionPointTypes2Str(i_selPts) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    if( parentItem() == nullptr )
    {
        showSelectionPointsOfBoundingRect( rect(), i_selPts );
    }
} // showSelectionPoints

/*==============================================================================
public: // overridables of base class CGraphObj (text labels)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the list of the possible anchor points for the given label name.

    For the predefined labels of the rectangle the following applies:

    - The "Name" label may be anchored to the center point of the bounding rectangle
      and the center points of the border lines.
    - "TopLeft", "TopRight", "BottomRight" and "BottomLeft" may be anchored to their
       corresponding bounding rectangle corners.

    User defined labels may be anchored to any selection point at the bounding rectangle.

    Please note that the most common used selection points should be at the beginning
    of the list so that combo boxes to select the selection point start with those.

    @return List of possbile selection points.
*/
QList<SGraphObjSelectionPoint> CGraphObjConnectionPoint::getPossibleLabelAnchorPoints(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    static QList<SGraphObjSelectionPoint> s_arSelPtsUserDefined;
    if (s_arSelPtsUserDefined.isEmpty()) {
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::Center));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::TopCenter));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomCenter));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter));
    }
    static QHash<QString, QList<SGraphObjSelectionPoint>> s_hshSelPtsPredefined;
    if (s_hshSelPtsPredefined.isEmpty()) {
        QList<SGraphObjSelectionPoint> arSelPts;
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::Center));
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::TopCenter));
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomCenter));
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter));
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter));
        s_hshSelPtsPredefined.insert(c_strLabelName, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::TopCenter));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameTopCenter, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::RightCenter));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameRightCenter, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::BottomCenter));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameBottomCenter, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionPoint*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::LeftCenter));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameLeftCenter, arSelPts);
    }
    if (s_hshSelPtsPredefined.contains(i_strName)) {
        return s_hshSelPtsPredefined.value(i_strName);
    }
    return s_arSelPtsUserDefined;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the label with the passed name has been modified or still
           has its default values.

    @param [in] i_strName
        Name of the label to be checked.

    @return true if the label still has its default values, false otherwise.
*/
bool CGraphObjConnectionPoint::labelHasDefaultValues(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    bool bHasDefaultValues = false;
    if (isPredefinedLabelName(i_strName)) {
        bHasDefaultValues = true;
        const SLabelDscr& labelDscr = m_hshLabelDscrs[i_strName];
        if (labelDscr.m_bLabelIsVisible) {
            bHasDefaultValues = false;
        }
        else if (labelDscr.m_bShowAnchorLine) {
            bHasDefaultValues = false;
        }
        else if (labelDscr.m_polarCoorsToLinkedSelPt != SPolarCoors()) {
            bHasDefaultValues = false;
        }
        else if (i_strName == c_strLabelName) {
            if (labelDscr.m_strText != m_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::Center) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameTopCenter) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameRightCenter) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::RightCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameBottomCenter) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::BottomCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameLeftCenter) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::LeftCenter) {
                bHasDefaultValues = false;
            }
        }
    }
    return bHasDefaultValues;
}

/*==============================================================================
public: // overridables of base class CGraphObj (geometry labels)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether the label with the passed name has been modified or still
           has its default values.

    @param [in] i_strName
        Name of the label to be checked.

    @return true if the label still has its default values, false otherwise.
*/
bool CGraphObjConnectionPoint::geometryLabelHasDefaultValues(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    if (!m_hshGeometryLabelDscrs.contains(i_strName)) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }

    bool bHasDefaultValues = false;
    if (m_strlstGeometryLabelNames.contains(i_strName)) {
        bHasDefaultValues = true;
        const SLabelDscr& labelDscr = m_hshGeometryLabelDscrs[i_strName];
        if (labelDscr.m_bLabelIsVisible) {
            bHasDefaultValues = false;
        }
        else if (labelDscr.m_bShowAnchorLine) {
            bHasDefaultValues = false;
        }
        else if (!labelDscr.m_strText.isEmpty()) {
            bHasDefaultValues = false;
        }
        else if (labelDscr.m_polarCoorsToLinkedSelPt != SPolarCoors()) {
            bHasDefaultValues = false;
        }
        else if (i_strName == c_strGeometryLabelNameTopCenter) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameRightCenter) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::RightCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameBottomCenter) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::BottomCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameLeftCenter) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::LeftCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameCenter) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::Center) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameWidth) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::LeftCenter) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_selPt != ESelectionPoint::RightCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameHeight) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopCenter) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_selPt != ESelectionPoint::BottomCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameAngle) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::Center) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_selPt != ESelectionPoint::RightCenter) {
                bHasDefaultValues = false;
            }
        }
    }
    return bHasDefaultValues;
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
    int iPenWidth = m_drawSettings.penWidth();
    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHighlighted || isSelected())) {
        iPenWidth += 3; // see paint method
    }
    rctBounding = QRectF(
        rctBounding.left() - static_cast<double>(iPenWidth)/2.0,
        rctBounding.top() - static_cast<double>(iPenWidth)/2.0,
        rctBounding.width() + static_cast<double>(iPenWidth),
        rctBounding.height() + static_cast<double>(iPenWidth));
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
        /* strObjName   */ path(),
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceDrawSettings(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    i_pPainter->save();
    i_pPainter->setRenderHints(s_painterRenderHints);

    QPen pn;
    QBrush brush;
    QRectF rctBounding = getBoundingRect();

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
    pn.setColor(m_drawSettings.penColor());
    pn.setWidth(m_drawSettings.penWidth());
    pn.setStyle(lineStyle2QtPenStyle(m_drawSettings.lineStyle().enumerator()));
    i_pPainter->setPen(pn);
    i_pPainter->setBrush(Qt::NoBrush);
    i_pPainter->drawEllipse(rctBounding);

    double fInnerWidth_px = getWidth(Units.Length.px).getVal() / 2.0;
    if (fInnerWidth_px >= 1.0) {
        pn.setColor(m_drawSettings.fillColor());
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        i_pPainter->setPen(pn);
        brush.setColor(m_drawSettings.fillColor());
        brush.setStyle(Qt::SolidPattern);
        i_pPainter->setBrush(brush);
        QRectF rctInnerCircle;
        rctInnerCircle.setLeft(rctBounding.center().x() - fInnerWidth_px/2.0);
        rctInnerCircle.setTop(rctBounding.center().y() - fInnerWidth_px/2.0);
        rctInnerCircle.setWidth(fInnerWidth_px);
        rctInnerCircle.setHeight(fInnerWidth_px);
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
        if (cursor() != Qt::SizeAllCursor) {
            QGraphicsItem_setCursor(Qt::SizeAllCursor);
        }
    }
    // Indicate that connection line can be started or terminated here.
    else if (m_pDrawingScene->getCurrentDrawingTool()->graphObjType() == EGraphObjTypeConnectionLine) {
        QPixmap pxmCursor(":/ZS/Draw/CursorPin16x16.png");
        QCursor cursor(pxmCursor, 0, pxmCursor.height()-1);
        QGraphicsItem_setCursor(cursor);
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    // Ignore hover events if any object should be or is currently being created.
    if (m_pDrawingScene->getCurrentDrawingTool() == nullptr) {
        if (cursor() != Qt::SizeAllCursor) {
            QGraphicsItem_setCursor(Qt::SizeAllCursor);
        }
    }
    // Indicate that connection line can be started or terminated here.
    else if (m_pDrawingScene->getCurrentDrawingTool()->graphObjType() == EGraphObjTypeConnectionLine) {
        QPixmap pxmCursor(":/ZS/Draw/CursorPin16x16.png");
        QCursor cursor(pxmCursor, 0, pxmCursor.height()-1);
        QGraphicsItem_setCursor(cursor);
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
        if (m_editMode == EEditMode::None) {
            setEditMode(EEditMode::ModifyingBoundingRect);
        }
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

    if (m_editMode == EEditMode::CreatingByMouseEvents) {
        // The editMode changed signal will be emitted and received by the drawing scene.
        // The drawing scene is informed this way that creation of the object is finished
        // and will unselect the current drawing tool and will select the object under
        // construction showing the selection points at the bounding rectangle.
        if (isResizable()) {
            setEditMode(EEditMode::ModifyingBoundingRect);
        }
        else {
            setEditMode(EEditMode::None);
        }
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
            // Update the object shape point in parent coordinates kept in the unit of the drawing scene.
            // If the item is not a group and as long as the item is not added as a child to
            // a group, the current (transformed) and original coordinates are equal.
            // If the item is a child of a group, the current (transformed) coordinates are only
            // taken over as the original coordinates if initially creating the item or when
            // adding the item to or removing the item from a group.
            updateTransformedCoorsOnItemPositionChanged();
            bGeometryChanged = true;
        }
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemSelectedHasChanged) {
        //QGraphicsItem_prepareGeometryChange();
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            bringToFront();
            // If not resizable, the connection point does not have selection points
            // but is a selection point itself.
            if (isResizable()) {
                if (m_editMode == EEditMode::CreatingByMouseEvents || m_editMode == EEditMode::ModifyingPolygonPoints) {
                    hideSelectionPoints(c_uSelectionPointsPolygonPoints);
                    showSelectionPoints(c_uSelectionPointsBoundingRectAll);
                }
                else if (m_editMode == EEditMode::ModifyingBoundingRect) {
                    hideSelectionPoints(c_uSelectionPointsPolygonPoints);
                    showSelectionPoints(c_uSelectionPointsBoundingRectAll);
                }
                else /*if (m_editMode == EEditMode::None)*/ {
                    hideSelectionPoints();
                }
            }
        }
        else {
            setEditMode(EEditMode::None);
            hideSelectionPoints();
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

/*==============================================================================
protected: // overridable slots of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjConnectionPoint::onGraphObjParentGeometryOnSceneChanged(
    CGraphObj* i_pGraphObjParent, bool i_bParentOfParentChanged)
//------------------------------------------------------------------------------
{
    if (m_iIgnoreParentGeometryChange > 0) {
        return;
    }
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree() + ", ParentOfParentChanged: " + bool2Str(i_bParentOfParentChanged);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onGraphObjParentGeometryOnSceneChanged",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    bool bGeometryOnSceneChanged = false;

    {   CRefCountGuard refCountGuardTracePositionInfo(&m_iTracePositionInfoBlockedCounter);

        if (i_pGraphObjParent->isGroup()) {
            CGraphObjGroup* pGraphObjGroupParent = dynamic_cast<CGraphObjGroup*>(i_pGraphObjParent);
            if (i_bParentOfParentChanged) {
                initParentTransform();
                updateTransformedCoorsOnParentGeometryChanged();
            }
            CPhysValRect physValRectGroupParentCurr = pGraphObjGroupParent->getRect(m_physValRectParentGroupOrig.unit());
            if (m_physValRectParentGroupOrig.width().getVal() > 0.0) {
                setParentGroupScaleX(physValRectGroupParentCurr.width().getVal() / m_physValRectParentGroupOrig.width().getVal());
            }
            if (m_physValRectParentGroupOrig.height().getVal() > 0.0) {
                setParentGroupScaleY(physValRectGroupParentCurr.height().getVal() / m_physValRectParentGroupOrig.height().getVal());
            }

            // The relative distance of the center point to the top left or bottom left corner
            // of the parent's bounding rectangle should remain the same.
            CPhysValRect physValRect = getPhysValRectScaled(m_physValRectOrig);
            setPhysValRectScaled(physValRect);
            physValRect.setAngle(m_physValRotationAngle);
            setPhysValRectScaledAndRotated(physValRect);

            QPointF ptPosPrev = pos();

            QRectF rectF;
            CPhysVal physValAngle;
            QPointF ptPos = getItemPosAndLocalCoors(physValRect, rectF, physValAngle);

            // Prepare the item for a geometry change. This function must be called before
            // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
            QGraphicsItem_prepareGeometryChange();

            {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdatePhysValCoorsBlockedCounter);
                CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

                // Set the rectangle in local coordinate system.
                // Also note that itemChange must not overwrite the current coordinates (refCountGuard).
                QGraphicsEllipseItem_setRect(rectF);

                // Please note that QGraphicsRectItem::setRect did not update the position of the
                // item in the parent. This has to be done "manually" afterwards.

                // Move the object to the parent position.
                // This has to be done after resizing the item which updates the local coordinates
                // of the item with origin (0/0) at the center point.
                // "setPos" will trigger an itemChange call which will update the position of the
                // selection points and labels. To position the selection points and labels correctly
                // the local coordinate system must be up-to-date.
                // Also note that itemChange must not overwrite the current coordinates (refCountGuard).
                // If the position is not changed, itemChange is not called with PositionHasChanged and
                // the position of the arrow heads will not be updated. We got to do this here "manually".
                if (ptPos != ptPosPrev) {
                    QGraphicsItem_setPos(ptPos);
                }
            }
            // If the geometry of the parent on the scene of this item changes, also the geometry
            // on the scene of this item is changed.
            bGeometryOnSceneChanged = true;
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }

    // Emit signal after updated position info has been traced.
    if (bGeometryOnSceneChanged) {
        emit_geometryOnSceneChanged();
    }
}

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjConnectionPoint::onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pSelectionPoint->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onSelectionPointGeometryOnSceneChanged",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    QPointF ptPosThis = pos();
    CGraphObjSelectionPoint* pGraphObjSelPt = dynamic_cast<CGraphObjSelectionPoint*>(i_pSelectionPoint);
    QGraphicsItem* pGraphicsItemSelPt = dynamic_cast<QGraphicsItem*>(pGraphObjSelPt);
    QPointF ptScenePosSelPt = pGraphicsItemSelPt->scenePos();
    QPointF ptPosSelPt = mapFromScene(ptScenePosSelPt);
    QPointF ptParentPosSelPt = pGraphicsItemThis->mapToParent(ptPosSelPt);
    CPhysValPoint physValPointParentSelPt(*m_pDrawingScene);
    if (parentGroup() != nullptr) {
        physValPointParentSelPt = parentGroup()->convert(ptParentPosSelPt);
    }
    else {
        physValPointParentSelPt = m_pDrawingScene->convert(ptParentPosSelPt);
    }

    SGraphObjSelectionPoint selPt = pGraphObjSelPt->getSelectionPoint();
    if (selPt.m_selPtType == ESelectionPointType::BoundingRectangle) {
        // Moving a selection point will modify the shape of the object and the position
        // of all other selection points got to be updated. If the position of the other
        // selection points will be changed, those selection points are emitting the
        // geometryOnSceneChanged signal whereupon this slot method would be called again
        // for each other selection point. This will not end up in an endless loop but
        // is useless and anything else but performant. So the slot will be temporarily
        // disconnected from the geometryOnSceneChanged signal of the selection points.
        disconnectGeometryOnSceneChangedSlotFromSelectionPoints();

        switch (selPt.m_selPt) {
            case ESelectionPoint::TopLeft: {
                setTopLeft(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::TopRight: {
                setTopRight(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::BottomRight: {
                setBottomRight(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::BottomLeft: {
                setBottomLeft(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::TopCenter: {
                setHeightByMovingTopCenter(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::RightCenter: {
                setWidthByMovingRightCenter(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::BottomCenter: {
                setHeightByMovingBottomCenter(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::LeftCenter: {
                setWidthByMovingLeftCenter(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::Center: {
                setCenter(physValPointParentSelPt);
                break;
            }
            case ESelectionPoint::RotateTop: {
                // The angle returned by getAngleDegree is counted counterclockwise with 0° at 3 o'clock.
                double fAngle_degree = ZS::Draw::getAngleDegree(ptPosThis, ptParentPosSelPt);
                // setRotationAngle expects the angle counted clockwise with 0° at 3 o'clock.
                fAngle_degree = ZS::System::Math::toClockWiseAngleDegree(fAngle_degree);
                // RotateTop is at 270°.
                fAngle_degree -= 270.0;
                fAngle_degree = ZS::System::Math::normalizeAngleInDegree(fAngle_degree);
                setRotationAngle(fAngle_degree);
                break;
            }
            case ESelectionPoint::RotateBottom: {
                double fAngle_degree = ZS::Draw::getAngleDegree(ptPosThis, ptParentPosSelPt);
                fAngle_degree = ZS::System::Math::toClockWiseAngleDegree(fAngle_degree);
                // RotateBottom is at 90°.
                fAngle_degree -= 90.0;
                fAngle_degree = ZS::System::Math::normalizeAngleInDegree(fAngle_degree);
                setRotationAngle(fAngle_degree);
                break;
            }
            default: {
                break;
            }
        }
        connectGeometryOnSceneChangedSlotWithSelectionPoints();
    }
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjConnectionPoint::updateTransformedCoorsOnParentChanged(
    CGraphObjGroup* i_pGraphObjGroupPrev, CGraphObjGroup* i_pGraphObjGroupNew)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "PrevGroup: " + QString(i_pGraphObjGroupPrev == nullptr ? "null" : i_pGraphObjGroupPrev->path()) +
            ", NewGroup: " + QString(i_pGraphObjGroupNew == nullptr ? "null" : i_pGraphObjGroupNew->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateTransformedCoorsOnParentChanged",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

        setRectOrig(rect());
        CPhysValRect physValRect = getPhysValRectOrig(m_rectOrig);
        setPhysValRectOrig(physValRect);
        physValRect = getPhysValRectScaled(m_physValRectOrig);
        setPhysValRectScaled(physValRect);
        // The relative rotation angle on the screen should remain the same.
        if (i_pGraphObjGroupPrev != nullptr) {
            #pragma message(__TODO__"PhysVal: round2Resolution is different from round2Accuracy.")
            #pragma message(__TODO__"PhysVal: Keep both resolution and accuracy in CPhysVal.")
            #pragma message(__TODO__"PhysVal: Values with resolution may also have an accuracy.")
            #pragma message(__TODO__"PhysVal: Resolutions should not be added, but accuracies should be added.")
            CPhysValRes physValRes = m_physValRotationAngle.getRes();
            CPhysVal physValRotationAngleParentGroup = i_pGraphObjGroupPrev->rotationAngle();
            if (i_pGraphObjGroupNew == nullptr) {
                m_physValRotationAngle += i_pGraphObjGroupPrev->mapRotationAngleToScene(physValRotationAngleParentGroup);
            }
            else {
                m_physValRotationAngle += i_pGraphObjGroupPrev->mapRotationAngleTo(physValRotationAngleParentGroup, i_pGraphObjGroupNew);
            }
            // Keep the resolution.
            m_physValRotationAngle.setRes(physValRes);
        }
        physValRect.setAngle(m_physValRotationAngle);
        setPhysValRectScaledAndRotated(physValRect);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }
    emit_geometryOnSceneChanged(true);
}

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjConnectionPoint::updateTransformedCoorsOnParentGeometryChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateTransformedCoorsOnParentGeometryChanged",
        /* strAddInfo   */ "" );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    // ItemChange will be called but should not emit the geometryOnSceneChanged signal.
    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

        setRectOrig(rect());
        CPhysValRect physValRect = getPhysValRectOrig(m_rectOrig);
        setPhysValRectOrig(physValRect);
        physValRect = getPhysValRectScaled(m_physValRectOrig);
        setPhysValRectScaled(physValRect);
        physValRect.setAngle(m_physValRotationAngle);
        setPhysValRectScaledAndRotated(physValRect);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }
    emit_geometryOnSceneChanged(true);
}

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjConnectionPoint::updateTransformedCoorsOnItemPositionChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateTransformedCoorsOnItemPositionChanged",
        /* strAddInfo   */ "" );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    // ItemChange will be called but should not emit the geometryOnSceneChanged signal.
    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

        CPhysValRect physValRect = getPhysValRectOrig(m_rectOrig);
        setPhysValRectOrig(physValRect);
        physValRect = getPhysValRectScaled(m_physValRectOrig);
        setPhysValRectScaled(physValRect);
        physValRect.setAngle(m_physValRotationAngle);
        setPhysValRectScaledAndRotated(physValRect);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }
    emit_geometryOnSceneChanged();
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Calculates the scaled, not rotated rectangle in pixels in item coordinates
           relative to the origin (center point) of the orignal bounding rectangle.

    The relative distance of the center point of the scaled rectangle to the
    origin (center point) of the parent's bounding rectangle remains the same.
    The width and height are scaled to the scale factors of the parent group.

    @param [in] i_rectOrig
        Unscaled rectangle in the item's local coordinates system whose origin
        is the center point of the item.
    @return Scaled rectangle.
*/
QRectF CGraphObjConnectionPoint::getRectScaled(const QRectF& i_rectOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qRect2Str(i_rectOrig) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getRectScaled",
        /* strAddInfo   */ strMthInArgs );

    QPointF ptCenter(m_fParentGroupScaleX * i_rectOrig.center().x(), m_fParentGroupScaleY * i_rectOrig.center().y());
    QSizeF sizeF(m_fParentGroupScaleX * i_rectOrig.width(), m_fParentGroupScaleY * i_rectOrig.height());
    QRectF rectF;
    rectF.setSize(sizeF);
    rectF.moveCenter(ptCenter);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rectF) + "}");
    }
    return rectF;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the unscaled and not rotated physical rectangle in the
           current unit of the drawing scene relative to the top left or
           bottom left corner of the parent's bounding rectangle.

    @param [in] i_rectOrig
        Unscaled, not rotated rectangle in the item's local coordinates system
        whose origin is the center point of the item.
    @return Physical rectangle whose origin is either the top left or bottom
            left corner of the parent's bounding rectangle.
*/
CPhysValRect CGraphObjConnectionPoint::getPhysValRectOrig(const QRectF& i_rectOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qRect2Str(i_rectOrig) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValRectOrig",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    CGraphObjConnectionPoint* pVThis = const_cast<CGraphObjConnectionPoint*>(this);
    double fRotationAngle_degree = m_physValRotationAngle.getVal(Units.Angle.Degree);
    if (fRotationAngle_degree != 0.0) {
        pVThis->QGraphicsItem_setRotation(0.0);
    }
    CPhysValPoint physValPointCenter(*m_pDrawingScene);
    CPhysValSize physValSize(*m_pDrawingScene);
    if (parentGroup() != nullptr) {
        QPointF ptCenter = pGraphicsItemThis->mapToParent(i_rectOrig.center());
        ptCenter = parentGroup()->mapToTopLeftOfBoundingRect(ptCenter);
        physValPointCenter = parentGroup()->convert(ptCenter);
        physValSize = parentGroup()->convert(i_rectOrig.size());
    }
    else {
        // Please note that "mapToScene" maps the local coordinates relative to the
        // top left corner of the item's bounding rectangle and there is no need to
        // call "mapToBoundingRectTopLeft" beforehand.
        QPointF ptCenter = pGraphicsItemThis->mapToScene(i_rectOrig.center());
        physValPointCenter = m_pDrawingScene->convert(ptCenter);
        physValSize = m_pDrawingScene->convert(i_rectOrig.size());
    }
    if (fRotationAngle_degree != 0.0) {
        pVThis->QGraphicsItem_setRotation(fRotationAngle_degree);
    }

    CPhysValRect physValRect(*m_pDrawingScene);
    physValRect.setSize(physValSize);
    physValRect.setCenter(physValPointCenter);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValRect.toString() + "} " + physValRect.unit().symbol());
    }
    return physValRect;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the scaled, not rotated physical rectangle in the current
           unit of the drawing scene relative to the top left or bottem left
           corner of the parent's bounding rectangle.

    The relative distance of the center point of the scaled rectangle to the
    top left or bottom left corner of the parent's bounding rectangle remains the same.
    The width and height are scaled to the scale factors of the parent group.

    @param [in] i_physValRectOrig
        Unscaled rectangle in the parent's, physical coordinates system whose origin
        is the top left or bottom right corner of the parent's bounding rectangle.
    @return Scaled rectangle.
*/
CPhysValRect CGraphObjConnectionPoint::getPhysValRectScaled(const CPhysValRect& i_physValRectOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValRectOrig.toString() + "} " + i_physValRectOrig.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValRectScaled",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPoint physValPointCenter(*m_pDrawingScene);
    physValPointCenter.setX(m_fParentGroupScaleX * i_physValRectOrig.center().x().getVal());
    physValPointCenter.setY(m_fParentGroupScaleY * i_physValRectOrig.center().y().getVal());
    CPhysValSize physValSize(*m_pDrawingScene);
    physValSize.setWidth(m_fParentGroupScaleX * i_physValRectOrig.width().getVal());
    physValSize.setHeight(m_fParentGroupScaleY * i_physValRectOrig.height().getVal());

    CPhysValRect physValRect(*m_pDrawingScene);
    physValRect.setSize(physValSize);
    physValRect.setCenter(physValPointCenter);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValRect.toString() + "} " + physValRect.unit().symbol());
    }
    return physValRect;
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Set the original, untransformed (not scaled, not rotated) rectangle
           coordinates in local coordinates relative to the origin of the
           groups bounding rectangle

    @param [in] i_rect
        Rectangle coordinates in local coordinates to be set.

    @return Previous original rectangle coordinates.
*/
QRectF CGraphObjConnectionPoint::setRectOrig(const QRectF& i_rect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + qRect2Str(i_rect) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRectOrig",
        /* strAddInfo   */ strMthInArgs );

    QRectF rectPrev = m_rectOrig;
    m_rectOrig = i_rect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + qRect2Str(rectPrev) + "}");
    }
    return rectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated line coordinates in local
           coordinates relative to the origin of the item's bounding rectangle.

    @param [in] i_line
        Coordinates to be set.

    @return Previous coordinates.
*/
QRectF CGraphObjConnectionPoint::QGraphicsEllipseItem_setRect(const QRectF& i_rect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + qRect2Str(i_rect) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsEllipseItem_setRect",
        /* strAddInfo   */ strMthInArgs );

    QRectF rectPrev = QGraphicsEllipseItem::rect();
    QGraphicsEllipseItem::setRect(i_rect);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + qRect2Str(rectPrev) + "}");
    }
    return rectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated line coordinates in local
           coordinates relative to the origin of the item's bounding rectangle.

    @param [in] i_fX1, i_fY1, i_fX2, i_fY2
        Line coordinates to be set.

    @return Previous line coordinates.
*/
QRectF CGraphObjConnectionPoint::QGraphicsEllipseItem_setRect(double i_fX, double i_fY, double i_fWidth, double i_fHeight)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + qRect2Str(QRectF(i_fX, i_fY, i_fWidth, i_fHeight)) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsEllipseItem_setRect",
        /* strAddInfo   */ strMthInArgs );

    QRectF rectPrev = QGraphicsEllipseItem::rect();
    QGraphicsEllipseItem::setRect(QRectF(i_fX, i_fY, i_fWidth, i_fHeight));
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qRect2Str(rectPrev));
    }
    return rectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the original, untransformed (not scaled, not rotated) rectangle
           coordinates with unit in parent coordinates relative to the top left
           or bottom left corner of the parent.

    @param [in] i_physValRect
        Rectangle coordinates relative to the parent (or scene) to be set.

    @return Previous original rectangle coordinates.
*/
CPhysValRect CGraphObjConnectionPoint::setPhysValRectOrig(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValRectOrig",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectPrev = m_physValRectOrig;
    m_physValRectOrig = i_physValRect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValRectPrev.toString() + "} " + physValRectPrev.unit().symbol());
    }
    return physValRectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated rectangle coordinates with unit in
           parent coordinates relative to the top left or bottom left corner of
           the parent.

    @param [in] i_physValRect
        Rectangle coordinates relative to the parent (or scene) to be set.

    @return Previous rectangle coordinates.
*/
CPhysValRect CGraphObjConnectionPoint::setPhysValRectScaled(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValRectScaled",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectPrev = m_physValRectScaled;
    m_physValRectScaled = i_physValRect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValRectPrev.toString() + "} " + physValRectPrev.unit().symbol());
    }
    return physValRectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled and rotated rectangle coordinates with unit in parent
           coordinates relative to the top left or bottom left corner of the parent.

    @param [in] i_physValRect
        Rectangle coordinates relative to the parent (or scene) to be set.

    @return Previous rectangle coordinates.
*/
CPhysValRect CGraphObjConnectionPoint::setPhysValRectScaledAndRotated(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValRectScaledAndRotated",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectPrev = m_physValRectScaledAndRotated;
    m_physValRectScaledAndRotated = i_physValRect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValRectPrev.toString() + "} " + physValRectPrev.unit().symbol());
    }
    return physValRectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Internal method reimplementing the prepareGeometryChange method of
           graphics item to trace the method call.

    As the prepareGeometryChange method is a protected method of QGraphicsItem
    this method must be reimplemented by the derived classes.
*/
void CGraphObjConnectionPoint::QGraphicsItem_prepareGeometryChange()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsItem_prepareGeometryChange",
        /* strAddInfo   */ "" );

    prepareGeometryChange();
}

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, const QString& i_strFilter) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }

    CGraphObj::traceThisPositionInfo(i_mthTracer, i_mthDir, i_strFilter);

    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "GraphObjEllipse {";
    i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = " . ";
        strRuntimeInfo += ". PhysValRect Orig {" + m_physValRectOrig.toString() + "} " + m_physValRectOrig.unit().symbol()
            + ", Scaled {" + m_physValRectScaled.toString() + "} " + m_physValRectScaled.unit().symbol();
            + ", ScaledRotated {" + m_physValRectScaledAndRotated.toString() + "} " + m_physValRectScaledAndRotated.unit().symbol();
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = " . ";
        strRuntimeInfo += ". ItemRect Orig {" + qRect2Str(m_rectOrig) + "}, Scaled {" + qRect2Str(rect()) + "}";
        i_mthTracer.trace(strRuntimeInfo);
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "}";
    i_mthTracer.trace(strRuntimeInfo);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::traceGraphObjStates(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, const QString& i_strFilter) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }

    CGraphObj::traceGraphObjStates(i_mthTracer, i_mthDir, i_strFilter);

    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "LinkedTo: SelPt {" + m_selPt.toString() + "}";
    i_mthTracer.trace(strRuntimeInfo);
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionPoint::traceConnectionLines(CMethodTracer& i_mthTracer, EMethodDir i_mthDir) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTraceConnectionLinesBlockedCounter > 0) {
        return;
    }

    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "ConnectionLines [" + QString::number(m_lstConnectionLines.size()) + "]";
    i_mthTracer.trace(strRuntimeInfo);
    if (!m_lstConnectionLines.isEmpty()) {
        for (const CGraphObjConnectionLine* pGraphObjCnctLine : m_lstConnectionLines) {
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = " . ";
            if (pGraphObjCnctLine == nullptr) {
                strRuntimeInfo += "null";
            }
            else {
                strRuntimeInfo += pGraphObjCnctLine->path() + " {CnctPt1: ";
                CGraphObjConnectionPoint* pCnctPtStart = pGraphObjCnctLine->getConnectionPoint(ELinePoint::Start);
                if (pCnctPtStart == nullptr) {
                    strRuntimeInfo += "null";
                }
                else {
                    strRuntimeInfo += pCnctPtStart->path();
                }
                strRuntimeInfo += ", CnctPt2: ";
                CGraphObjConnectionPoint* pCnctPtEnd = pGraphObjCnctLine->getConnectionPoint(ELinePoint::End);
                if (pCnctPtEnd == nullptr) {
                    strRuntimeInfo += "null";
                }
                else {
                    strRuntimeInfo += pCnctPtEnd->path();
                }
                strRuntimeInfo += "}";
            }
            i_mthTracer.trace(strRuntimeInfo);
        }
    }
}
