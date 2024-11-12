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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolyline.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPolylinePropertiesDlg.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysRefCountGuard.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

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
class CGraphObjPolyline : public CGraphObj, public QGraphicsPolygonItem
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

qint64 CGraphObjPolyline::s_iInstCount = 0;

/*==============================================================================
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjPolyline::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjPolyline::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjPolyline::CGraphObjPolyline(CDrawingScene* i_pDrawingScene, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypePolyline,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypePolyline),
        /* strObjName          */ i_strObjName.isEmpty() ? "Polyline" + QString::number(s_iInstCount) : i_strObjName),
    QGraphicsPolygonItem(),
    m_polylineOrig(),
    m_physValPolylineOrig(*m_pDrawingScene),
    m_physValPolylineScaled(*m_pDrawingScene),
    m_physValPolylineScaledAndRotated(*m_pDrawingScene),
    m_plgLineStartArrowHead(),
    m_plgLineEndArrowHead()
{
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    createTraceAdminObjs("StandardShapes::" + ClassName());

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

    initInstance();
}

/*==============================================================================
protected: // ctor (used by derived classes, e.g. CGraphObjPolygon)
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjPolyline::CGraphObjPolyline(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strFactoryGroupName,
    EGraphObjType i_type,
    const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ i_strFactoryGroupName,
        /* type                */ i_type,
        /* strType             */ ZS::Draw::graphObjType2Str(i_type),
        /* strObjName          */ i_strObjName),
    QGraphicsPolygonItem(),
    m_polylineOrig(),
    m_physValPolylineOrig(*m_pDrawingScene),
    m_physValPolylineScaled(*m_pDrawingScene),
    m_physValPolylineScaledAndRotated(*m_pDrawingScene),
    m_plgLineStartArrowHead(),
    m_plgLineEndArrowHead()
{
    createTraceAdminObjs("StandardShapes::" + ClassName());

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

    initInstance();
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::initInstance()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObjPolyline::initInstance",
        /* strAddInfo   */ "" );

    m_strlstPredefinedLabelNames.append(c_strLabelName);

    for (const QString& strLabelName : m_strlstPredefinedLabelNames) {
        if (!m_hshpLabels.contains(strLabelName)) {
            addLabel(strLabelName, strLabelName, ESelectionPoint::Center);
        }
    }

    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameCenter);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameWidth);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameHeight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameAngle);

    const CUnit& unit = m_pDrawingScene->drawingSize().unit();
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
    }

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable
            |QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
    setAcceptHoverEvents(true);

} // initInstance

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjPolyline::~CGraphObjPolyline()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    emit_aboutToBeDestroyed();
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjPolyline::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypePolyline;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjPolyline::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CGraphObjPolyline* pGraphObj = new CGraphObjPolyline(m_pDrawingScene, m_strName);
    pGraphObj->setPolyline(getPolyline(drawingSize.unit()));
    pGraphObj->setDrawSettings(m_drawSettings);
    return pGraphObj;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to create a user defined dialog.
*/
void CGraphObjPolyline::onCreateAndExecDlgFormatGraphObjs()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onCreateAndExecDlgFormatGraphObjs",
        /* strAddInfo   */ "" );

    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Format Line";
    CDlgGraphObjPolylineProperties* pDlg = CDlgGraphObjPolylineProperties::GetInstance(this);
    if( pDlg == nullptr ) {
        pDlg = CDlgGraphObjPolylineProperties::CreateInstance(strDlgTitle, this);
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
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolyline::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
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
        if (lineEndArrowHeadPolygonsNeedUpdate(ELinePoint::Start, i_drawSettingsOld)) {
            updateLineEndArrowHeadPolygons(ELinePoint::Start);
            bDrawSettingsChanged = true;
        }
        if (lineEndArrowHeadPolygonsNeedUpdate(ELinePoint::End, i_drawSettingsOld)) {
            updateLineEndArrowHeadPolygons(ELinePoint::End);
        }
        update();
        emit_drawSettingsChanged();
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolyline::setPolyline(const CPhysValPolyline& i_physValPolyline)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "[" + QString::number(i_physValPolyline.count()) + "](" + i_physValPolyline.toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPolyline",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    QPointF ptPosPrev = pos();

    // Depending on the Y scale orientation of the drawing scene the polyline coordinates
    // have been passed either relative to the top left or bottom right corner of the
    // parent item's bounding rectangle.
    // The coordinates need to be transformed into the local coordinate system of the graphical
    // object whose origin point is the center of the objects bounding rectangle.

    QPolygonF polyline;
    QPointF ptPos = getItemPosAndLocalCoors(i_physValPolyline, polyline);

    bool bGeometryOnSceneChanged = false;

    // If the coordinates MUST be updated (e.g. after the drawing size has been changed)
    // or if the coordinates have been changed ...
    if (m_physValPolylineOrig.isNull() || polygon() != polyline || m_physValPolylineOrig != i_physValPolyline)
    {
        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsItem_prepareGeometryChange();

        {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdatePhysValCoorsBlockedCounter);
            CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

            // Store physical coordinates.
            CPhysValPolyline physValPolyline(i_physValPolyline);
            physValPolyline.setAngle(0.0);
            setPhysValPolylineOrig(i_physValPolyline);
            setPhysValPolylineScaled(i_physValPolyline);
            setPhysValPolylineScaledAndRotated(i_physValPolyline);

            // Set the polyline in local coordinate system.
            // Also note that itemChange must not overwrite the current values (refCountGuard).
            setPolylineOrig(polyline);
            QGraphicsPolygonItem_setPolygon(polyline);

            // Please note that GraphicsPolygonItem::setPolygon did not update the position of the
            // item in the parent. This has to be done "manually" afterwards.

            // Move the object to the parent position.
            // This has to be done after resizing the polyline which updates the local coordinates
            // of the polyline with origin (0/0) at the polylines center point.
            // "setPos" will trigger an itemChange call which will update the position of the
            // selection points and labels. To position the selection points and labels correctly
            // the local coordinate system must be up-to-date.
            // Also note that itemChange must not overwrite the current values (refCountGuard).
            // If the position is not changed, itemChange is not called with PositionHasChanged and
            // the position of the arrow heads will not be updated. We got to do this here "manually".
            if (ptPos != ptPosPrev) {
                QGraphicsItem_setPos(ptPos);
            }
            else {
                updateLineEndArrowHeadPolygons();
            }
        }
        // If the geometry of the parent on the scene of this item changes, also the geometry
        // on the scene of this item is changed.
        bGeometryOnSceneChanged = true;
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
    // Emit signal after updated position info has been traced.
    if (bGeometryOnSceneChanged) {
        emit_geometryOnSceneChanged();
    }
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setPolyline(const QPolygonF& i_polyline, const ZS::PhysVal::CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "[" + QString::number(i_polyline.count()) + "](" + qPolygon2Str(i_polyline) + ") " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setLine",
        /* strAddInfo   */ strMthInArgs );

    setPolyline(CPhysValPolyline(*m_pDrawingScene, i_polyline, i_unit));
}

//------------------------------------------------------------------------------
CPhysValPolyline CGraphObjPolyline::getPolyline() const
//------------------------------------------------------------------------------
{
    return getPolyline(m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
CPhysValPolyline CGraphObjPolyline::getPolyline(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (parentGroup() != nullptr) {
        return parentGroup()->convert(m_physValPolylineScaledAndRotated, i_unit);
    }
    else {
        return m_pDrawingScene->convert(m_physValPolylineScaledAndRotated, i_unit);
    }
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setCenter(const QPointF& i_pt)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setCenter(i_pt);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setCenter(const CPhysValPoint& i_physValPoint)
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
        /* strMethod    */ "setCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setCenter(i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::getCenter() const
//------------------------------------------------------------------------------
{
    return getPolyline().center();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::getCenter(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolyline(i_unit).center();
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setSize(const QSizeF& i_size)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setSize(i_size);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setSize(const CPhysValSize& i_physValSize)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setSize(i_physValSize);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObjPolyline::getSize() const
//------------------------------------------------------------------------------
{
    return getPolyline().size();
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObjPolyline::getSize(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolyline(i_unit).size();
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setWidth(double i_fWidth)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setWidth(i_fWidth);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setWidth(const CPhysVal& i_physValWidth)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setWidth(i_physValWidth);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setWidthByMovingLeftCenter(const QPointF& i_pt)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setWidthByMovingLeftCenter(i_pt);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setWidthByMovingLeftCenter(i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setWidthByMovingRightCenter(const QPointF& i_pt)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setWidthByMovingRightCenter(i_pt);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setWidthByMovingRightCenter(i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjPolyline::getWidth() const
//------------------------------------------------------------------------------
{
    return getPolyline().width();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjPolyline::getWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolyline(i_unit).width();
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setHeight(double i_fHeight)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setHeight(i_fHeight);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setHeight(const CPhysVal& i_physValHeight)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setHeight(i_physValHeight);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setHeightByMovingTopCenter(const QPointF& i_pt)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setHeightByMovingTopCenter(i_pt);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setHeightByMovingTopCenter(i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setHeightByMovingBottomCenter(const QPointF& i_pt)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setHeightByMovingBottomCenter(i_pt);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setHeightByMovingBottomCenter(i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjPolyline::getHeight() const
//------------------------------------------------------------------------------
{
    return getPolyline().height();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjPolyline::getHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolyline(i_unit).height();
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setTopLeft(const QPointF& i_pt)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setTopLeft(i_pt);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setTopLeft(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setTopLeft(i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::getTopLeft() const
//------------------------------------------------------------------------------
{
    return getPolyline().topLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::getTopLeft(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolyline(i_unit).topLeft();
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setTopRight(const QPointF& i_pt)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setTopRight(i_pt);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setTopRight(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setTopRight(i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::getTopRight() const
//------------------------------------------------------------------------------
{
    return getPolyline().topRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::getTopRight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolyline(i_unit).topRight();
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setBottomRight(const QPointF& i_pt)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setBottomRight(i_pt);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setBottomRight(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setBottomRight(i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::getBottomRight() const
//------------------------------------------------------------------------------
{
    return getPolyline().bottomRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::getBottomRight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolyline(i_unit).bottomRight();
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setBottomLeft(const QPointF& i_pt)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setBottomLeft(i_pt);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::setBottomLeft(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setBottomLeft(i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::getBottomLeft() const
//------------------------------------------------------------------------------
{
    return getPolyline().bottomLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::getBottomLeft(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolyline(i_unit).bottomLeft();
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolyline::setRotationAngle(double i_fAngle_degree)
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
void CGraphObjPolyline::setRotationAngle(const ZS::PhysVal::CPhysVal& i_physValAngle)
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

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.setAngle(i_physValAngle);
    setPolyline(physValPolyline);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjPolyline::isEmpty() const
//------------------------------------------------------------------------------
{
    return m_physValPolylineScaledAndRotated.isEmpty();
}

//------------------------------------------------------------------------------
int CGraphObjPolyline::count() const
//------------------------------------------------------------------------------
{
    return m_physValPolylineScaledAndRotated.count();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::at(int i_idx) const
//------------------------------------------------------------------------------
{
    return m_physValPolylineScaledAndRotated.at(i_idx);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::replace(int i_idx, const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Idx: " + QString::number(i_idx) + ", {" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "replace",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.replace(i_idx, i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::append(const CPhysValPoint& i_physValPoint)
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
        /* strMethod    */ "append",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.append(i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::insert(int i_idx, const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Idx: " + QString::number(i_idx) + ", {" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "insert",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.insert(i_idx, i_physValPoint);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::remove(int i_idx, int i_iCount)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Idx: " + QString::number(i_idx) + ", Count: " + QString::number(i_iCount);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "remove",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.remove(i_idx, i_iCount);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::removeAt(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Idx: " + QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "removeAt",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.removeAt(i_idx);
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::removeFirst()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "removeFirst",
        /* strAddInfo   */ "" );

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.removeFirst();
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::removeLast()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "removeLast",
        /* strAddInfo   */ "" );

    CPhysValPolyline physValPolyline = getPolyline();
    physValPolyline.removeLast();
    setPolyline(physValPolyline);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::takeAt(int i_idx)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Idx: " + QString::number(i_idx);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "takeAt",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolyline physValPolyline = getPolyline();
    CPhysValPoint physValPoint = physValPolyline.takeAt(i_idx);
    setPolyline(physValPolyline);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(physValPoint.toString());
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::takeFirst()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "takeFirst",
        /* strAddInfo   */ "" );

    CPhysValPolyline physValPolyline = getPolyline();
    CPhysValPoint physValPoint = physValPolyline.takeFirst();
    setPolyline(physValPolyline);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(physValPoint.toString());
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolyline::takeLast()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "takeLast",
        /* strAddInfo   */ "" );

    CPhysValPolyline physValPolyline = getPolyline();
    CPhysValPoint physValPoint = physValPolyline.takeLast();
    setPolyline(physValPolyline);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(physValPoint.toString());
    }
    return physValPoint;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the scaled but not rotated coordinates in local coordinates
           relative to the origin of the bounding rectangle.

    Please note that the boundingRect call of QGraphicsItem als takes the pen width
    into account. So we cannot call this method to get the real bounding rectangle of
    the object if only the real shape points should be considered.

    @return Bounding rectangle in local coordinates.
*/
QRectF CGraphObjPolyline::getBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ "" );

    // Points in local coordinates.
    QPolygonF polyline = polygon();
    QRectF rctBounding = polyline.boundingRect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Returns the effective (resulting) bounding rectangle of this item
           on the drawing scene.

    To get the effective bounding rectangle the left most, the right most
    as well as the top most and bottom most shape points of the transformed
    (rotated and scaled) object are are taken into account.

    If the object is rotated the effective bounding rectangle is not the
    bounding rectangle (in item coordinates) mapped to the scene.
    Before mapping the points to the scene the TopMost, BottomMost, LeftMost
    and RightMost points of the rotated object have to be calculated and each
    point has to be mapped to the scene.

    E.g. rotated trapez on the scene:

                             + TopMost
                            / \
                           /   \
                          /     \
                LeftMost +       + RightMost
                          \     /
                           \   /
                            \ /
                             + BottomMost
*/
QRectF CGraphObjPolyline::getEffectiveBoundingRectOnScene() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getEffectiveBoundingRectOnScene",
        /* strAddInfo   */ "" );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPolygonF plg({
        pGraphicsItemThis->mapToScene(getTopLeft().toQPointF(Units.Length.px)),
        pGraphicsItemThis->mapToScene(getTopRight().toQPointF(Units.Length.px)),
        pGraphicsItemThis->mapToScene(getBottomRight().toQPointF(Units.Length.px)),
        pGraphicsItemThis->mapToScene(getBottomLeft().toQPointF(Units.Length.px))});
    QRectF rctBounding = plg.boundingRect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//bool CGraphObjPolyline::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Point:" + point2Str(i_pt) +
//            ", HitInfo, " + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo)) +
//            ", Polygon:" + polygon2Str(polygon());
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjIsHit,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "isHit",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bIsHit = false;
//
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//
//    if( pGraphicsItem != nullptr )
//    {
//        if( pGraphicsItem->isSelected() )
//        {
//            bIsHit = isPolygonSelectionPointHit(i_pt,o_pHitInfo);
//
//            if( !bIsHit )
//            {
//                bIsHit = isBoundingRectSelectionPointHit(
//                    /* pt               */ i_pt,
//                    /* iSelPtsCount     */ -1,
//                    /* pSelPts          */ nullptr,
//                    /* pGraphObjHitInfo */ o_pHitInfo );
//            }
//        }
//
//        if( !bIsHit )
//        {
//            QPolygonF plg = polygon();
//            bIsHit = isPolygonHit( plg, EFillStyle::NoFill, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );
//        }
//
//        if( !bIsHit )
//        {
//            if( pGraphicsItem->isSelected() )
//            {
//                bIsHit = pGraphicsItem->contains(i_pt);
//
//                if( o_pHitInfo != nullptr )
//                {
//                    //o_pHitInfo->m_editMode = EEditMode::Move;
//                    //o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
//                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
//                    o_pHitInfo->m_idxPolygonShapePoint = -1;
//                    o_pHitInfo->m_idxLineSegment = -1;
//                    o_pHitInfo->m_ptSelected = i_pt;
//                }
//            }
//        }
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        if( bIsHit && o_pHitInfo != nullptr )
//        {
//            o_pHitInfo->setCursor( Math::degree2Rad(m_fRotAngleCurr_deg) );
//        }
//#endif
//
//    } // if( pGraphicsItem != nullptr )
//
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        QString strMthOutArgs;
//        if (o_pHitInfo != nullptr) {
//            strMthOutArgs = "HitInfo {" + o_pHitInfo->toString() + "}";
//            mthTracer.setMethodOutArgs(strMthOutArgs);
//        }
//        mthTracer.setMethodReturn(bIsHit);
//    }
//
//    return bIsHit;
//
//} // isHit

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolyline::showSelectionPoints(TSelectionPointTypes i_selPts)
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

    if (parentItem() == nullptr) {
        // Create bounding rectangle's selection points before shape edge points so that
        // the shape edge points receive mouse events before the bounding rectangle's
        // selection points (as they are "above" them).
        if (i_selPts & c_uSelectionPointsBoundingRectAll) {
            showSelectionPointsOfBoundingRect(getBoundingRect());
        }
        if (i_selPts & c_uSelectionPointsPolygonShapePoints) {
            showSelectionPointsOfPolygon(polygon());
        }
    }
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjPolyline::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = QGraphicsPolygonItem::boundingRect();
    for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsPolygon) {
        if (pGraphObjSelPt != nullptr) {
            QRectF rctSelPt = pGraphObjSelPt->boundingRect();
            QPolygonF plgSelPt = mapFromItem(pGraphObjSelPt, rctSelPt);
            rctBounding |= plgSelPt.boundingRect();
        }
    }
    for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsBoundingRect) {
        if (pGraphObjSelPt != nullptr) {
            QRectF rctSelPt = pGraphObjSelPt->boundingRect();
            QPolygonF plgSelPt = mapFromItem(pGraphObjSelPt, rctSelPt);
            rctBounding |= plgSelPt.boundingRect();
        }
    }
    if (!m_plgLineStartArrowHead.isEmpty()) {
        rctBounding |= m_plgLineStartArrowHead.boundingRect();
    }
    if (!m_plgLineEndArrowHead.isEmpty()) {
        rctBounding |= m_plgLineEndArrowHead.boundingRect();
    }
    rctBounding = QRectF(
        rctBounding.left() - m_drawSettings.getPenWidth()/2,
        rctBounding.top() - m_drawSettings.getPenWidth()/2,
        rctBounding.width() + m_drawSettings.getPenWidth(),
        rctBounding.height() + m_drawSettings.getPenWidth() );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
QPainterPath CGraphObjPolyline::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );

    QPainterPath painterPath = QGraphicsPolygonItem::shape();
    if (!m_plgLineStartArrowHead.isEmpty()) {
        painterPath.addPolygon(m_plgLineStartArrowHead);
    }
    if (!m_plgLineEndArrowHead.isEmpty()) {
        painterPath.addPolygon(m_plgLineEndArrowHead);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        const QGraphicsItem* pCThis = static_cast<const QGraphicsItem*>(this);
        QGraphicsItem* pVThis = const_cast<QGraphicsItem*>(pCThis);
        QString strMthRet = qPainterPath2Str(pVThis, painterPath);
        mthTracer.setMethodReturn(strMthRet);
    }
    return painterPath;
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::paint(
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

    QPolygonF polyline = polygon();
    if (polyline.size() < 2) {
        return;
    }

    i_pPainter->save();

    QPen pn = pen();
    if (m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || m_bIsHighlighted || isSelected())) {
        if (isSelected()) {
            pn.setColor(s_selectionColor);
            pn.setWidth(3 + m_drawSettings.getPenWidth());
        }
        else {
            pn.setColor(s_highlightColor);
            pn.setWidth(3 + m_drawSettings.getPenWidth());
        }
        pn.setStyle(Qt::SolidLine);
        QPainterPath outline;
        outline.moveTo(polyline[0]);
        for (int idxPt = 1; idxPt < polyline.size(); ++idxPt) {
            outline.lineTo(polyline[idxPt]);
        }
        i_pPainter->strokePath(outline, pn);
        pn.setWidth(1 + m_drawSettings.getPenWidth());
    }
    pn.setColor(m_drawSettings.getPenColor());
    pn.setWidth(m_drawSettings.getPenWidth());
    pn.setStyle(lineStyle2QtPenStyle(m_drawSettings.getLineStyle().enumerator()));
    i_pPainter->setPen(pn);
    i_pPainter->setRenderHints(s_painterRenderHints);
    i_pPainter->drawPolyline(polyline);

    CEnumLineEndStyle lineEndStyleLineStart = m_drawSettings.getLineEndStyle(ELinePoint::Start);
    CEnumLineEndStyle lineEndStyleLineEnd = m_drawSettings.getLineEndStyle(ELinePoint::End);
    if (lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal) {
        CEnumArrowHeadBaseLineType baseLineTypeLineStart = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::Start);
        CEnumArrowHeadBaseLineType baseLineTypeLineEnd   = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::End);
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        i_pPainter->setPen(pn);
        QBrush brsh(pn.color());
        if (lineEndStyleLineStart != ELineEndStyle::Normal) {
            brsh.setStyle(arrowHeadFillStyle2QtBrushStyle(m_drawSettings.getArrowHeadFillStyle(ELinePoint::Start)));
            i_pPainter->setBrush(brsh);
            if (baseLineTypeLineStart == EArrowHeadBaseLineType::NoLine) {
                i_pPainter->drawPolyline(m_plgLineStartArrowHead);
            }
            else {
                if (m_drawSettings.getArrowHeadFillStyle(ELinePoint::Start) == EArrowHeadFillStyle::NoFill) {
                    i_pPainter->setBrush(Qt::white);
                }
                else {
                    i_pPainter->setBrush(Qt::black);
                }
                i_pPainter->drawPolygon(m_plgLineStartArrowHead);
            }
        }
        if (lineEndStyleLineEnd != ELineEndStyle::Normal) {
            brsh.setStyle( arrowHeadFillStyle2QtBrushStyle(m_drawSettings.getArrowHeadFillStyle(ELinePoint::End)) );
            i_pPainter->setBrush(brsh);
            if (baseLineTypeLineEnd == EArrowHeadBaseLineType::NoLine) {
                i_pPainter->drawPolyline(m_plgLineEndArrowHead);
            }
            else {
                if (m_drawSettings.getArrowHeadFillStyle(ELinePoint::End) == EArrowHeadFillStyle::NoFill) {
                    i_pPainter->setBrush(Qt::white);
                }
                else {
                    i_pPainter->setBrush(Qt::black);
                }
                i_pPainter->drawPolygon(m_plgLineEndArrowHead);
            }
        }
    }

    if ((m_pDrawingScene->getMode() == EMode::Edit) && isSelected()) {
        if (!polyline.isEmpty()) {
            QRectF rctBounding = getBoundingRect();
            pn.setColor(Qt::blue);
            pn.setStyle(Qt::DotLine);
            pn.setWidth(1);
            i_pPainter->setPen(pn);
            i_pPainter->setBrush(Qt::NoBrush);
            i_pPainter->drawRect(rctBounding);
            if (m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)] != nullptr
             && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)] != nullptr) {
                CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)];
                CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)];
                QPointF ptRct = QPointF(pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y());
                QPointF ptRot = QPointF(pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y());
                QPointF ptRctM = mapFromScene(ptRct);
                QPointF ptRotM = mapFromScene(ptRot);
                i_pPainter->drawLine(ptRctM, ptRotM);
            }
            if (m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)] != nullptr
             && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)] != nullptr) {
                CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)];
                CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)];
                QPointF ptRct = QPointF(pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y());
                QPointF ptRot = QPointF(pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y());
                QPointF ptRctM = mapFromScene(ptRct);
                QPointF ptRotM = mapFromScene(ptRot);
                i_pPainter->drawLine( ptRctM, ptRotM );
            }
        }
    }
    i_pPainter->restore();
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

////------------------------------------------------------------------------------
//bool CGraphObjPolyline::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
////------------------------------------------------------------------------------
//{
//    //--------------------------------------------------------------------
//    // Mouse events of selection points will be "watched" to forward those
//    // events to this item in order to resize and rotate this item.
//    //--------------------------------------------------------------------
//
//    CGraphObjSelectionPoint* pGraphObjSelPtWatched = dynamic_cast<CGraphObjSelectionPoint*>(i_pGraphicsItemWatched);
//    if (pGraphObjSelPtWatched == nullptr) {
//        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObjSelPtWatched == nullptr" );
//    }
//
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjSceneEventFilter, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "ItemWatched: " + pGraphObjSelPtWatched->name();
//        if (isGraphicsSceneHoverEvent(i_pEv)) {
//            QGraphicsSceneHoverEvent* pHoverEvent = dynamic_cast<QGraphicsSceneHoverEvent*>(i_pEv);
//            strMthInArgs += ", " + qEventType2Str(i_pEv->type()) + " {" + qGraphicsSceneHoverEvent2Str(pHoverEvent) + "}";
//        }
//        else if (isGraphicsSceneMouseEvent(i_pEv)) {
//            QGraphicsSceneMouseEvent* pMouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//            strMthInArgs += ", " + qEventType2Str(i_pEv->type()) + " {" + qGraphicsSceneMouseEvent2Str(pMouseEvent) + "}";
//        }
//        else {
//            strMthInArgs += ", Event: " + qEventType2Str(i_pEv->type());
//        }
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjSceneEventFilter,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "sceneEventFilter",
//        /* strAddInfo   */ strMthInArgs );
//
//    bool bEventHandled = false;
//
//    switch( i_pEv->type() )
//    {
//        case QEvent::GraphicsSceneMouseMove:
//        {
//            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//            if( pEv != nullptr )
//            {
//                QPointF ptEvPos;
//                for( int i = 0x1; i <= 0x10; i <<= 1 )
//                {
//                    Qt::MouseButton button = Qt::MouseButton(i);
//                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                    pEv->setButtonDownPos( button, ptEvPos );
//                }
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//                pEv->setPos(ptEvPos);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//                pEv->setLastPos(ptEvPos);
//                mouseMoveEvent(pEv);
//                bEventHandled = true;
//            }
//            break;
//        }
//        case QEvent::GraphicsSceneMousePress:
//        {
//            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//            if( pEv != nullptr )
//            {
//                QPointF ptEvPos;
//                for( int i = 0x1; i <= 0x10; i <<= 1 )
//                {
//                    Qt::MouseButton button = Qt::MouseButton(i);
//                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                    pEv->setButtonDownPos( button, ptEvPos );
//                }
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//                pEv->setPos(ptEvPos);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//                pEv->setLastPos(ptEvPos);
//                mousePressEvent(pEv);
//                bEventHandled = true;
//            }
//            break;
//        }
//        case QEvent::GraphicsSceneMouseRelease:
//        {
//            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//            if( pEv != nullptr )
//            {
//                QPointF ptEvPos;
//                for( int i = 0x1; i <= 0x10; i <<= 1 )
//                {
//                    Qt::MouseButton button = Qt::MouseButton(i);
//                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                    pEv->setButtonDownPos( button, ptEvPos );
//                }
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//                pEv->setPos(ptEvPos);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//                pEv->setLastPos(ptEvPos);
//                mouseReleaseEvent(pEv);
//                bEventHandled = true;
//            }
//            break;
//        }
//        case QEvent::GraphicsSceneMouseDoubleClick:
//        {
//            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//            if( pEv != nullptr )
//            {
//                QPointF ptEvPos;
//                for( int i = 0x1; i <= 0x10; i <<= 1 )
//                {
//                    Qt::MouseButton button = Qt::MouseButton(i);
//                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
//                    pEv->setButtonDownPos( button, ptEvPos );
//                }
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
//                pEv->setPos(ptEvPos);
//                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
//                pEv->setLastPos(ptEvPos);
//                mouseDoubleClickEvent(pEv);
//                bEventHandled = true;
//            }
//            break;
//        }
//        default:
//        {
//            break;
//        }
//
//    } // switch( i_pEv->type() )
//
//    return bEventHandled;
//
//} // sceneEventFilter

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolyline::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );

    // Ignore hover events if the object is currently being created.
    if (m_editMode != EEditMode::CreatingByMouseEvents) {
        showSelectionPoints();
        setIsHit(true);
        setCursor(getProposedCursor(i_pEv->pos()));
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    // Ignore hover events if the object is currently being created.
    if (m_editMode != EEditMode::CreatingByMouseEvents) {
        showSelectionPoints();
        setIsHit(true);
        setCursor(getProposedCursor(i_pEv->pos()));
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strMthInArgs );

    // If the object is selected the selection points should remain visible.
    if (!isSelected()) {
        hideSelectionPoints();
    }
    setIsHit(false);
    unsetCursor();

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolyline::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );

//    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
//    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();
//
//    if( modeDrawing == EMode::Edit )
//    {
//        //Qt::MouseButtons iEvMouseButtonState = i_pEv->buttons() & Qt::MouseButtonMask;
//        Qt::KeyboardModifiers iEvMouseKeyboardModifiers = i_pEv->modifiers() & Qt::KeyboardModifierMask;
//
//        QPointF ptMouseItemPos = i_pEv->pos();
//
//        // The "old" coordinates are used by resize (mouse move) events to calculate
//        // the difference between the original coordinates on before starting the
//        // resize event and the current coordinates during the resize event.
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        m_rctOnMousePressEvent = m_rctCurr;
//        m_ptRotOriginOnMousePressEvent = mapToScene(m_ptRotOriginCurr);
//#endif
//
//        if( editToolDrawing == EEditTool::CreateObjects && m_editMode == EEditMode::Creating )
//        {
//            QGraphicsPolygonItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)
//
//            QPolygonF plg = polygon();
//            QRectF    rctItem;
//
//            if( plg.size() < 2 )
//            {
//                plg.append( QPointF(0.0,0.0) );
//                plg.append( QPointF(1.0,1.0) );
//            }
//
//            // If there was no polygon shape point selected ..
//            if( m_idxSelPtSelectedPolygon < 0 || m_idxSelPtSelectedPolygon >= plg.size() )
//            {
//                // .. the shape has been initially hit right after creation.
//                m_idxSelPtSelectedPolygon = plg.size()-1;
//                plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
//            }
//            // If there was already a polygon shape point selected ..
//            else
//            {
//                // .. the currently selected shape point will be fixed and
//                // another point will be added.
//                plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
//                plg.append(ptMouseItemPos);
//                m_idxSelPtSelectedPolygon = plg.size()-1;
//            }
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            m_rctCurr = plg.boundingRect();
//            m_ptRotOriginCurr = m_rctCurr.center();
//
//            QGraphicsPolygonItem::setPolygon(plg);
//            while( m_arpSelPtsPolygon.size() < plg.size() )
//            {
//                m_arpSelPtsPolygon.append(nullptr);
//            }
//
//            showSelectionPointsOfPolygon(plg);
//            updateSelectionPointsOfBoundingRect(m_rctCurr);
//#endif
//
//            updateTransform();
//
//            //updateEditInfo();
//            //updateToolTip();
//
//        } // if( editToolDrawing == EEditTool::CreateObjects && m_editMode == EEditMode::Creating )
//
//        else if( editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None )
//        {
//            if( contains(ptMouseItemPos) )
//            {
//                QGraphicsPolygonItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)
//            }
//
//            if( isSelected() )
//            {
//                CGraphObjSelectionPoint* pGraphObjSelPt;
//                ESelectionPoint          selPt;
//                int                      idxSelPt;
//
//                // Polygon shape points may be very close or even overlap each other.
//                // On creating the polygon the most likely point the user wants to grab
//                // is the last shape point to move it around. If the ALT keyboard key
//                // is pressed together with the mouse the "alternate" selection point
//                // is desired (if there is any) and we first check whether any selection
//                // point from the bounding rectangle has been hit.
//
//                SGraphObjHitInfo hitInfo;
//
//                bool bIsHit = false;
//
//                if( iEvMouseKeyboardModifiers & Qt::AltModifier )
//                {
//                    bIsHit = isBoundingRectSelectionPointHit(
//                        /* pt               */ ptMouseItemPos,
//                        /* iSelPtsCount     */ -1,
//                        /* pSelPts          */ nullptr,
//                        /* pGraphObjHitInfo */ &hitInfo );
//                }
//
//                if( !bIsHit )
//                {
//                    bIsHit = isHit( ptMouseItemPos, &hitInfo );
//                }
//
//                m_editMode                  = hitInfo.m_editMode;
//                m_editResizeMode            = hitInfo.m_editResizeMode;
//                m_idxSelPtSelectedPolygon   = hitInfo.m_idxPolygonShapePoint;
//                m_selPtSelectedBoundingRect = hitInfo.m_selPtBoundingRect;
//
//                for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
//                {
//                    pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
//
//                    if( pGraphObjSelPt != nullptr )
//                    {
//                        if( idxSelPt == m_idxSelPtSelectedPolygon )
//                        {
//                            pGraphObjSelPt->setSelected(true);
//                        }
//                        else
//                        {
//                            pGraphObjSelPt->setSelected(false);
//                        }
//                    }
//                }
//
//                for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
//                {
//                    selPt = static_cast<ESelectionPoint>(idxSelPt);
//
//                    pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
//
//                    if( pGraphObjSelPt != nullptr )
//                    {
//                        if( m_selPtSelectedBoundingRect == selPt )
//                        {
//                            pGraphObjSelPt->setSelected(true);
//                        }
//                        else
//                        {
//                            pGraphObjSelPt->setSelected(false);
//                        }
//                    }
//                }
//
//                if( m_editMode == EEditMode::Resize || m_editMode == EEditMode::Rotate || m_editMode == EEditMode::MoveShapePoint )
//                {
//                    m_plgOnMousePressEvent = polygon();
//                    m_rctOnMousePressEvent = m_plgOnMousePressEvent.boundingRect();
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//                    m_ptRotOriginOnMousePressEvent = mapToScene(m_ptRotOriginCurr);
//#endif
//                }
//
//                m_pDrawingScene->setMode( EMode::Undefined, EEditTool::Undefined, m_editMode, m_editResizeMode, false );
//
//                updateTransform();
//
//                //updateEditInfo();
//                //updateToolTip();
//
//            } // if( isSelected() )
//
//        } // if( editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None )
//
//    } // if( modeDrawing == EMode::Edit )
//
//    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
//    {
//        strMthInArgs  = "Selected:" + bool2Str(isSelected());
//        strMthInArgs += ", EditMode:" + m_editMode.toString();
//        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
//        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
//        strMthInArgs += ", Polygon:" + polygon2Str(polygon());
//        mthTracer.trace(strMthInArgs);
//    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjPolyline::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

//    CEnumMode modeDrawing = m_pDrawingScene->getMode();
//
//    if( modeDrawing == EMode::Edit )
//    {
//        Qt::MouseButtons iEvMouseButtonState = i_pEv->buttons() & Qt::MouseButtonMask;
//
//        // If no mouse button is pressed ...
//        if( iEvMouseButtonState == Qt::NoButton )
//        {
//            if( m_editMode == EEditMode::Creating /*|| m_editMode == EEditMode::MoveShapePoint*/ )
//            {
//                QPointF ptMouseItemPos = i_pEv->pos();
//
//                QPolygonF plg = polygon();
//
//                if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < plg.size() )
//                {
//                    plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
//                }
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//                m_rctCurr = plg.boundingRect();
//                m_ptRotOriginCurr = m_rctCurr.center();
//#endif
//
//                QGraphicsPolygonItem::setPolygon(plg);
//
//                // Don't change the position of the object as the position of further
//                // mouse events should be received relative to the object's position
//                // on starting the edit process on pressing the mouse.
//                //setPos(ptPosNew); // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//                m_bCoorsDirty = true;
//#endif
//
//                updateLineEndArrowHeadPolygons();
//
//                updateTransform();
//
//                //updateEditInfo();
//                //updateToolTip();
//
//            } // if( m_editMode == EEditMode::Creating || m_editMode == EEditMode::MoveShapePoint )
//
//        } // if( iEvMouseButtonState == Qt::NoButton )
//
//        // If the left mouse button is pressed ...
//        else if( iEvMouseButtonState & Qt::LeftButton )
//        {
//            if( m_editMode == EEditMode::Move )
//            {
//                QGraphicsPolygonItem::mouseMoveEvent(i_pEv);
//            }
//
//            else if( m_editMode == EEditMode::Resize )
//            {
//                QPointF ptMouseItemPos = i_pEv->pos();
//
//                QPolygonF plg;
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//                m_rctCurr = resizeRect( m_rctOnMousePressEvent, m_selPtSelectedBoundingRect.enumerator(), ptMouseItemPos );
//
//                plg = resizePolygon( m_plgOnMousePressEvent, m_rctCurr, m_rctOnMousePressEvent );
//
//                //m_ptRotOriginCurr = m_rctCurr.center();
//
//                QGraphicsPolygonItem::setPolygon(plg);
//
//                updateSelectionPointsOfPolygon(plg);
//                updateSelectionPointsOfBoundingRect(m_rctCurr);
//
//                updateLabelPositionsAndContents();
//
//                m_bCoorsDirty = true;
//#endif
//
//                updateLineEndArrowHeadPolygons();
//
//                //updateTransform();
//
//                //updateEditInfo();
//                //updateToolTip();
//
//            } // if( m_editMode == EEditMode::Resize )
//
//            else if( m_editMode == EEditMode::Rotate )
//            {
//                QPointF ptMouseScenePos = i_pEv->scenePos();
//
//                double fRotAngle_rad = getAngleRad( m_ptRotOriginOnMousePressEvent, ptMouseScenePos );
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//                m_fRotAngleCurr_deg = Math::rad2Degree(fRotAngle_rad);
//
//                switch( m_selPtSelectedBoundingRect.enumerator() )
//                {
//                    case ESelectionPoint::RotateTop:
//                    {
//                        m_fRotAngleCurr_deg -= 90.0;
//                        break;
//                    }
//                    case ESelectionPoint::RotateBottom:
//                    {
//                        m_fRotAngleCurr_deg -= 270.0;
//                        break;
//                    }
//                    default:
//                    {
//                        break;
//                    }
//                }
//
//                m_fRotAngleCurr_deg = Math::round2Resolution( m_fRotAngleCurr_deg, m_pDrawingScene->getRotationAngleResolutionInDegree() );
//
//                while( m_fRotAngleCurr_deg >= 360.0 )
//                {
//                    m_fRotAngleCurr_deg -= 360.0;
//                }
//                while( m_fRotAngleCurr_deg < 0.0 )
//                {
//                    m_fRotAngleCurr_deg += 360.0;
//                }
//
//                m_bCoorsDirty = true;
//#endif
//
//                updateLineEndArrowHeadPolygons();
//
//                updateTransform();
//
//                //updateEditInfo();
//                //updateToolTip();
//
//            } // if( m_editMode == EEditMode::Rotate )
//
//            else if( m_editMode == EEditMode::MoveShapePoint )
//            {
//                QPointF ptMouseItemPos = i_pEv->pos();
//
//                QPolygonF plg = polygon();
//
//                CGraphObjSelectionPoint* pGraphObjSelPt;
//                QPointF                  ptSel;
//
//                if( plg.size() >= 2 ) // anyway ..
//                {
//                    if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < plg.size() )
//                    {
//                        plg[m_idxSelPtSelectedPolygon] = ptMouseItemPos;
//
//                        setPolygon(plg);
//
//                        if( m_idxSelPtSelectedPolygon >= 0 && m_idxSelPtSelectedPolygon < m_arpSelPtsPolygon.size() )
//                        {
//                            pGraphObjSelPt = m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon];
//
//                            if( pGraphObjSelPt != nullptr )
//                            {
//                                ptSel = plg[m_idxSelPtSelectedPolygon];
//                                ptSel = mapToScene(ptSel);
//                                pGraphObjSelPt->setPos(ptSel);
//                            }
//                        }
//                    }
//
//                } // if( plg.size() >= 2 )
//
//                m_bCoorsDirty = true;
//
//                updateLineEndArrowHeadPolygons();
//
//                updateTransform();
//
//                //updateEditInfo();
//                //updateToolTip();
//
//            } // if( m_editMode == EEditMode::MoveShapePoint )
//
//        } // if( iEvMouseButtonState & Qt::LeftButton )
//
//    } // if( modeDrawing == EMode::Edit )

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjPolyline::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

//    CEnumMode modeDrawing = m_pDrawingScene->getMode();
//
//    if( modeDrawing == EMode::Edit )
//    {
//        if( m_editMode == EEditMode::Creating )
//        {
//            QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
//        }
//
//        else if( m_editMode == EEditMode::Move )
//        {
//            QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
//
//            m_editMode = EEditMode::None;
//            m_editResizeMode = EEditResizeMode::None;
//            m_idxSelPtSelectedPolygon = -1;
//            m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            acceptCurrentAsOriginalCoors();
//#endif
//        }
//
//        else if( m_editMode == EEditMode::Resize )
//        {
//            // The mouse release event would select the object.
//            // This is not wanted if the selection tool is not active.
//            bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
//            bool bIsSelectableReset = false;
//
//            if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditTool::Select )
//            {
//                setFlag(QGraphicsItem::ItemIsSelectable,false);
//                bIsSelectableReset = true;
//            }
//
//            QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
//
//            if( bIsSelectableReset )
//            {
//                setFlag(QGraphicsItem::ItemIsSelectable,bIsSelectable);
//            }
//
//            m_editMode = EEditMode::None;
//            m_editResizeMode = EEditResizeMode::None;
//            m_idxSelPtSelectedPolygon = -1;
//            m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//            QPointF ptScenePosOld = scenePos();
//            QPointF ptScenePosNew = ptScenePosOld;
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            QPolygonF plgScene         = mapToScene(m_rctCurr);
//            QRectF    rctBoundingScene = plgScene.boundingRect();
//            QPointF   ptRotOriginScene = rctBoundingScene.center();
//            double    fAngle_rad       = Math::degree2Rad(m_fRotAngleCurr_deg);
//
//            ptScenePosNew = rotatePoint( ptRotOriginScene, ptScenePosOld, -fAngle_rad );
//
//            m_ptRotOriginCurr = m_rctCurr.center();
//
//            setPos(ptScenePosNew);
//
//            m_bCoorsDirty = true;
//
//            updateLineEndArrowHeadPolygons();
//
//            updateTransform();
//
//            acceptCurrentAsOriginalCoors();
//#endif
//
//            //updateEditInfo();
//            //updateToolTip();
//
//        } // if( m_editMode == EEditMode::Resize )
//
//        else if( m_editMode == EEditMode::Rotate )
//        {
//            // The mouse release event would select the object.
//            // This is not wanted if the selection tool is not active.
//            bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
//            bool bIsSelectableReset = false;
//
//            if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditTool::Select )
//            {
//                setFlag(QGraphicsItem::ItemIsSelectable,false);
//                bIsSelectableReset = true;
//            }
//
//            QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
//
//            if( bIsSelectableReset )
//            {
//                setFlag(QGraphicsItem::ItemIsSelectable,bIsSelectable);
//            }
//
//            m_editMode = EEditMode::None;
//            m_editResizeMode = EEditResizeMode::None;
//            m_idxSelPtSelectedPolygon = -1;
//            m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            acceptCurrentAsOriginalCoors();
//#endif
//
//            //updateEditInfo();
//            //updateToolTip();
//
//        } // if( m_editMode == EEditMode::Rotate )
//
//        else if( m_editMode == EEditMode::MoveShapePoint )
//        {
//            // The mouse release event would select the object.
//            // This is not wanted if the selection tool is not active.
//            bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
//            bool bIsSelectableReset = false;
//
//            if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditTool::Select )
//            {
//                setFlag(QGraphicsItem::ItemIsSelectable,false);
//                bIsSelectableReset = true;
//            }
//
//            QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
//
//            if( bIsSelectableReset )
//            {
//                setFlag(QGraphicsItem::ItemIsSelectable,bIsSelectable);
//            }
//
//            m_editMode = EEditMode::None;
//            m_editResizeMode = EEditResizeMode::None;
//            m_idxSelPtSelectedPolygon = -1;
//            m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//            normalize();
//
//            QPolygonF plg              = polygon();
//            QRectF    rctItem          = plg.boundingRect();
//            QPointF   ptScenePosOld    = mapToScene(rctItem.topLeft());
//            QPointF   ptScenePosNew    = ptScenePosOld;
//            QPolygonF polygonScene     = mapToScene(rctItem);
//            QRectF    rctBoundingScene = polygonScene.boundingRect();
//            QPointF   ptRotOriginScene = rctBoundingScene.center();
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            double    fAngle_rad       = Math::degree2Rad(m_fRotAngleCurr_deg);
//#endif
//
//            QSizeF    sizMoveOffs( rctItem.left(), rctItem.top() );
//            QPointF   pt;
//            int       idxPt;
//
//            // The position of the polyline should become the top left corner of it's bounding rectangle.
//            // On moving one shape point all other shape points should keep their scene position.
//            for( idxPt = 0; idxPt < plg.size(); idxPt++ )
//            {
//                pt = plg[idxPt];
//                pt.setX( pt.x() - sizMoveOffs.width() );
//                pt.setY( pt.y() - sizMoveOffs.height() );
//                plg[idxPt] = pt;
//            }
//
//            QGraphicsPolygonItem::setPolygon(plg);
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            m_rctCurr = plg.boundingRect();
//
//            m_ptRotOriginCurr = m_rctCurr.center();
//
//            ptScenePosNew = rotatePoint( ptRotOriginScene, ptScenePosOld, -fAngle_rad );
//
//            setPos(ptScenePosNew);
//
//            m_bCoorsDirty = true;
//#endif
//
//            updateLineEndArrowHeadPolygons();
//
//            updateTransform();
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            acceptCurrentAsOriginalCoors();
//#endif
//
//            //updateEditInfo();
//            //updateToolTip();
//
//        } // if( m_editMode == EEditMode::MoveShapePoint )
//
//    } // if( modeDrawing == EMode::Edit )

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjPolyline::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );

    // When doubleclicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".
    // This is not necessary here.
    //QGraphicsPolygonItem::mouseDoubleClickEvent(i_pEv);

//    CEnumMode modeDrawing = m_pDrawingScene->getMode();
//
//    if( modeDrawing == EMode::Edit )
//    {
//        if( m_editMode == EEditMode::Creating )
//        {
//            // Before the double click event a mouse press event occurs creating an additional
//            // point which is not desired. This point got to be removed again. We are going
//            // to remove all succeeding points overlapping each other which will also remove the
//            // last undesired point.
//
//            QPolygonF plg = polygon();
//
//            if( plg.size() > 2 )
//            {
//                int idxPt;
//
//                for( idxPt = plg.size()-1; idxPt > 0; idxPt-- )
//                {
//                    if( plg[idxPt] == plg[idxPt-1] )
//                    {
//                        plg.remove(idxPt);
//                    }
//                }
//            }
//
//            QRectF  rctItem = plg.boundingRect();
//            QPointF ptPos   = pos();   // current position (coordinates of first shape point)
//            QPointF ptLT    = mapToScene(rctItem.topLeft());
//            QSizeF  sizMoveOffs( ptLT.x() - ptPos.x(), ptLT.y() - ptPos.y() );
//            QPointF pt;
//            int     idxPt;
//
//            // The position of the polyline should become the top left corner of it's bounding rectangle.
//            for( idxPt = 0; idxPt < plg.size(); idxPt++ )
//            {
//                pt = plg[idxPt];
//                pt.setX( pt.x() - sizMoveOffs.width() );
//                pt.setY( pt.y() - sizMoveOffs.height() );
//                plg[idxPt] = pt;
//            }
//
//            QGraphicsPolygonItem::setPolygon(plg);
//
//            setPos(ptLT);
//
//            normalize();
//
//            m_bCoorsDirty = true;
//
//            updateLineEndArrowHeadPolygons();
//
//            plg = polygon();
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            m_rctCurr = plg.boundingRect();
//            m_ptRotOriginCurr = m_rctCurr.center();
//#endif
//
//            m_editMode = EEditMode::None;
//            m_editResizeMode = EEditResizeMode::None;
//            m_idxSelPtSelectedPolygon = -1;
//            m_selPtSelectedBoundingRect = ESelectionPoint::None;
//
//            updateTransform();
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//            acceptCurrentAsOriginalCoors();
//#endif
//
//            // The object has been initially created.
//            //m_pDrawingScene->onGraphObjCreationFinished(this);
//
//            //updateEditInfo();
//            //updateToolTip();
//
//        } // if( m_editMode == EEditMode::Creating )
//
//        else if( isSelected() )
//        {
//            onCreateAndExecDlgFormatGraphObjs();
//        }
//
//    } // if( modeDrawing == EMode::Edit )

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplments the itemChange method.

    The object may be moved or transformed by several methods.
    "itemChange" is a central point to update the coordinates upon those changes.
*/
QVariant CGraphObjPolyline::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
//------------------------------------------------------------------------------
{
    if (m_bDtorInProgress) {
        return i_value;
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsItemChange2Str(i_change, i_value);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strMthInArgs );
    traceGraphObjStates(mthTracer);

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
            tracePositionInfo(mthTracer, EMethodDir::Enter);
            updateLineEndArrowHeadPolygons();
            bGeometryChanged = true;
            bTreeEntryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged) {
        if (m_iItemChangeUpdatePhysValCoorsBlockedCounter == 0) {
            //tracePositionInfo(mthTracer, EMethodDir::Enter);
            // Update the object shape point in parent coordinates kept in the unit of the drawing scene.
            // If the item is not a group and as long as the item is not added as a child to
            // a group, the current (transformed) and original coordinates are equal.
            // If the item is a child of a group, the current (transformed) coordinates are only
            // taken over as the original coordinates if initially creating the item or when
            // adding the item to or removing the item from a group.
            //initParentScaleParameters();
            //bGeometryChanged = true;
        }
        updateLineEndArrowHeadPolygons();
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemPositionHasChanged) {
        if (m_iItemChangeUpdatePhysValCoorsBlockedCounter == 0) {
            tracePositionInfo(mthTracer, EMethodDir::Enter);
            // Update the object shape point in parent coordinates kept in the unit of the drawing scene.
            // If the item is not a group and as long as the item is not added as a child to
            // a group, the current (transformed) and original coordinates are equal.
            // If the item is a child of a group, the current (transformed) coordinates are only
            // taken over as the original coordinates if initially creating the item or when
            // adding the item to or removing the item from a group.
            updateTransformedCoorsOnItemPositionChanged();
            bGeometryChanged = true;
        }
        updateLineEndArrowHeadPolygons();
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemRotationHasChanged) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
        bGeometryChanged = true;
        bTreeEntryChanged = true;
    }

    else if (i_change == ItemSelectedHasChanged) {
        QGraphicsItem_prepareGeometryChange();
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            bringToFront();
            showSelectionPoints();
            // Not necessary as item has been brought to front and "showSelectionPoints"
            // sets zValue of selection points above item.
            //bringSelectionPointsToFront();
            setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
        }
        else {
            hideSelectionPoints();
            resetStackingOrderValueToOriginalValue(); // restore ZValue as before selecting the object
        }
        bSelectedChanged = true;
        bTreeEntryChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemZValueHasChanged) {
        bZValueChanged = true;
        bTreeEntryChanged = true;
    }

    if (bGeometryChanged) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
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

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qGraphicsItemChange2Str(i_change, valChanged, false);
        mthTracer.setMethodReturn(strMthRet);
    }
    return valChanged;
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjPolyline::lineEndArrowHeadPolygonsNeedUpdate(
    const CEnumLinePoint& i_linePoint, const CDrawSettings& i_drawSettingsOld) const
//------------------------------------------------------------------------------
{
    bool bNeedUpdate = false;

    if (i_drawSettingsOld.getLineRecordType() != m_drawSettings.getLineRecordType()) {
        bNeedUpdate = true;
    }
    else if (i_drawSettingsOld.getLineExtent() != m_drawSettings.getLineExtent()) {
        bNeedUpdate = true;
    }
    else if (i_drawSettingsOld.getLineEndStyle(i_linePoint) != m_drawSettings.getLineEndStyle(i_linePoint)) {
        bNeedUpdate = true;
    }
    else if (i_drawSettingsOld.getArrowHeadBaseLineType(i_linePoint) != m_drawSettings.getArrowHeadBaseLineType(i_linePoint)) {
        bNeedUpdate = true;
    }
    else if (i_drawSettingsOld.getArrowHeadWidth(i_linePoint) != m_drawSettings.getArrowHeadWidth(i_linePoint)) {
        bNeedUpdate = true;
    }
    else if (i_drawSettingsOld.getArrowHeadLength(i_linePoint) != m_drawSettings.getArrowHeadLength(i_linePoint)) {
        bNeedUpdate = true;
    }
    return bNeedUpdate;
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::updateLineEndArrowHeadPolygons(const CEnumLinePoint& i_linePoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_linePoint.isValid() ? i_linePoint.toString() : "All");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateLineEndArrowHeadPolygons",
        /* strAddInfo   */ strMthInArgs );

    const QPolygonF polygonF = polygon();
    if (polygonF.size() >= 2) {
        if (!i_linePoint.isValid() || i_linePoint == ELinePoint::Start) {
            QLineF lineF(polygonF[0], polygonF[1]);
            CEnumLineEndStyle lineEndStyle = m_drawSettings.getLineEndStyle(ELinePoint::Start);
            if (lineEndStyle != ELineEndStyle::Normal) {
                getLineEndArrowPolygons(
                    /* line          */ lineF,
                    /* drawSetings   */ m_drawSettings,
                    /* pplgLineStart */ &m_plgLineStartArrowHead,
                    /* pplgLineEnd   */ nullptr );
            }
        }
        if (!i_linePoint.isValid() || i_linePoint == ELinePoint::End) {
            QLineF lineF(polygonF[polygonF.size()-2], polygonF[polygonF.size()-1]);
            CEnumLineEndStyle lineEndStyle = m_drawSettings.getLineEndStyle(ELinePoint::End);
            if (lineEndStyle != ELineEndStyle::Normal) {
                getLineEndArrowPolygons(
                    /* line          */ lineF,
                    /* drawSetings   */ m_drawSettings,
                    /* pplgLineStart */ nullptr,
                    /* pplgLineEnd   */ &m_plgLineEndArrowHead );
            }
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Calculates the item position relative to the parent item or drawing scene
           as well as the item coordinates in local coordinates.

    @param [in] i_physValPolyline
        Polyline in parent coordinates, depending on the Y scale orientation
        relative to the top left or bottom left corner of parent item's bounding
        rectangle. If the item belongs to a parent group the passed polyline
        must have been resized and the center must have been moved according to the
        parents scale factors.
    @param [out] o_rectF
        Rectangle coordinates in local coordinates.
    @param [out] o_physValAngle
        The rotatian angle of the passed rectangle.
*/
QPointF CGraphObjPolyline::getItemPosAndLocalCoors(
    const CPhysValPolyline& i_physValPolyline, QPolygonF& o_polyline) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPolyline.toString() + "} " + i_physValPolyline.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getItemPosAndLocalCoors",
        /* strAddInfo   */ strMthInArgs );

    // First determine the position of the item in the parent's (scene or group) coordinate system.
    CPhysValPolyline physValPolylineTmp(i_physValPolyline);
    if (parentGroup() != nullptr) {
        physValPolylineTmp = parentGroup()->convert(physValPolylineTmp, Units.Length.px);
    }
    else {
        physValPolylineTmp = m_pDrawingScene->convert(physValPolylineTmp, Units.Length.px);
    }
    o_polyline = physValPolylineTmp.toQPolygonF();

    // Transform the parent coordinates into local coordinate system.
    // The origin is the center point of the polyline's bounding rectangle.
    QPointF ptPos = o_polyline.boundingRect().center(); // polyline here still in parent or scene coordinates
    for (int idxPt = 0; idxPt < o_polyline.size(); ++idxPt) {
        o_polyline[idxPt] = o_polyline[idxPt] - ptPos; // polyline points now in local coordinates
    }

    if (parentGroup() != nullptr) {
        ptPos = parentGroup()->mapFromTopLeftOfBoundingRect(ptPos);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "Polyline [" + QString::number(o_polyline.size()) + "](" + qPolygon2Str(o_polyline) + ")";
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn("{" + qPoint2Str(ptPos) + "}");
    }
    return ptPos;
}

//------------------------------------------------------------------------------
void CGraphObjPolyline::normalize()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "normalize",
        /* strAddInfo   */ "" );

    QPolygonF plgOld = polygon();
    QPolygonF plgNew = normalizePolygon( plgOld, m_pDrawingScene->getHitToleranceInPx() );
    if (plgOld.size() != plgNew.size()) {
        setPolygon(plgNew);
        hideSelectionPoints();
        if (isSelected()) {
            showSelectionPoints();
        }
    }
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Set the original, untransformed (not scaled, not rotated) polyline
           coordinates in local coordinates relative to the origin of the
           polyline's bounding rectangle

    @param [in] i_polyline
        Coordinates in local coordinates to be set.

    @return Previous original coordinates.
*/
QPolygonF CGraphObjPolyline::setPolylineOrig(const QPolygonF& i_polyline)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New [" + QString::number(i_polyline.size()) + "](" + qPolygon2Str(i_polyline) + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPolylineOrig",
        /* strAddInfo   */ strMthInArgs );

    QPolygonF polylinePrev = m_polylineOrig;
    m_polylineOrig = i_polyline;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev [" + QString::number(polylinePrev.size()) + "](" + qPolygon2Str(polylinePrev) + ")");
    }
    return polylinePrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated coordinates in local coordinates
           relative to the origin of the item's bounding rectangle.

    @param [in] i_polyline
        Coordinates to be set.

    @return Previous coordinates.
*/
QPolygonF CGraphObjPolyline::QGraphicsPolygonItem_setPolygon(const QPolygonF& i_polyline)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New [" + QString::number(i_polyline.size()) + "](" + qPolygon2Str(i_polyline) + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsPolygonItem_setPolygon",
        /* strAddInfo   */ strMthInArgs );

    QPolygonF polylinePrev = QGraphicsPolygonItem::polygon();
    QGraphicsPolygonItem::setPolygon(i_polyline);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev [" + QString::number(polylinePrev.size()) + "](" + qPolygon2Str(polylinePrev) + ")");
    }
    return polylinePrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the original, untransformed (not scaled, not rotated) line
           coordinates with unit in parent coordinates relative to the top left
           or bottom left corner of the parent.

    @param [in] i_physValPolyline
        Coordinates to be set.

    @return Previous original line coordinates.
*/
CPhysValPolyline CGraphObjPolyline::setPhysValPolylineOrig(const CPhysValPolyline& i_physValPolyline)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New [" + QString::number(i_physValPolyline.count()) + "](" + i_physValPolyline.toString() + ") " + i_physValPolyline.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValPolylineOrig",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolyline physValPolylinePrev = m_physValPolylineOrig;
    m_physValPolylineOrig = i_physValPolyline;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev [" + QString::number(physValPolylinePrev.count()) + "](" + physValPolylinePrev.toString() + ") " + physValPolylinePrev.unit().symbol());
    }
    return physValPolylinePrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated line coordinates with unit in
           parent coordinates relative to the top left or bottom left corner of
           the parent.

    @param [in] i_physValPolyline
        Line coordinates relative to the parent (or scene) to be set.

    @return Previous line coordinates.
*/
CPhysValPolyline CGraphObjPolyline::setPhysValPolylineScaled(const CPhysValPolyline& i_physValPolyline)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New [" + QString::number(i_physValPolyline.count()) + "](" + i_physValPolyline.toString() + ") " + i_physValPolyline.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValPolylineScaled",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolyline physValPolylinePrev = m_physValPolylineScaled;
    m_physValPolylineScaled = i_physValPolyline;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev [" + QString::number(physValPolylinePrev.count()) + "](" + physValPolylinePrev.toString() + ") " + physValPolylinePrev.unit().symbol());
    }
    return physValPolylinePrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled and rotated line coordinates with unit in parent
           coordinates relative to the top left or bottom left corner of the parent.

    @param [in] i_physValPolyline
        Line coordinates relative to the parent (or scene) to be set.

    @return Previous line coordinates.
*/
CPhysValPolyline CGraphObjPolyline::setPhysValPolylineScaledAndRotated(const CPhysValPolyline& i_physValPolyline)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New [" + QString::number(i_physValPolyline.count()) + "](" + i_physValPolyline.toString() + ") " + i_physValPolyline.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValPolylineScaledAndRotated",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolyline physValPolylinePrev = m_physValPolylineScaledAndRotated;
    m_physValPolylineScaledAndRotated = i_physValPolyline;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev [" + QString::number(physValPolylinePrev.count()) + "](" + physValPolylinePrev.toString() + ") " + physValPolylinePrev.unit().symbol());
    }
    return physValPolylinePrev;
}

//------------------------------------------------------------------------------
/*! @brief Internal method reimplementing the prepareGeometryChange method of
           graphics item to trace the method call.

    As the prepareGeometryChange method is a protected method of QGraphicsItem
    this method must be reimplemented by the derived classes.
*/
void CGraphObjPolyline::QGraphicsItem_prepareGeometryChange()
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
void CGraphObjPolyline::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItem != nullptr) {
            QPolygonF polyline = polygon();
            QString strRuntimeInfo;
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "PhysValPolyline Orig [" + QString::number(m_physValPolylineOrig.count()) + "](" + m_physValPolylineOrig.toString() + ") " + m_physValPolylineOrig.unit().symbol()
                + ", Scaled [" + QString::number(m_physValPolylineScaled.count()) + "](" + m_physValPolylineScaled.toString() + ") " + m_physValPolylineScaled.unit().symbol()
                + ", ScaledRotated [" + QString::number(m_physValPolylineScaledAndRotated.count()) + "](" + m_physValPolylineScaledAndRotated.toString() + ") " + m_physValPolylineScaledAndRotated.unit().symbol();
            i_mthTracer.trace(strRuntimeInfo);
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "ItemLine Orig [" + QString::number(m_polylineOrig.size()) + "](" + qPolygon2Str(m_polylineOrig) + ")"
                + ", Scaled [" + QString::number(polygon().size()) + "](" + qPolygon2Str(polygon()) + ")";
            i_mthTracer.trace(strRuntimeInfo);
            CGraphObj::traceThisPositionInfo(i_mthTracer, i_mthDir, i_detailLevel);
        }
    }
}
