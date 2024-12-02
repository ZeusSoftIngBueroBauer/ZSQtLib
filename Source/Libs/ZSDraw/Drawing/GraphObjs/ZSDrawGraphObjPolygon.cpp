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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjPolygon.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjPolygonPropertiesDlg.h"
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
class CGraphObjPolygon : public CGraphObj, public QGraphicsPolygonItem
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

qint64 CGraphObjPolygon::s_iInstCount = 0;

/*==============================================================================
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjPolygon::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjPolygon::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjPolygon::createPolygonPointLabelName(int i_idxPt)
//------------------------------------------------------------------------------
{
    return c_strGeometryLabelNameP + QString::number(i_idxPt+1);
}

//------------------------------------------------------------------------------
bool CGraphObjPolygon::isPolygonPointLabelName(const QString& i_strLabelName)
//------------------------------------------------------------------------------
{
    bool bIs = false;
    if (i_strLabelName.startsWith(c_strGeometryLabelNameP)) {
        QString strIndex = i_strLabelName.mid(1);
        bool bIsValidIndex = false;
        strIndex.toInt(&bIsValidIndex);
        if (bIsValidIndex) {
            bIs = true;
        }
    }
    return bIs;
}

//------------------------------------------------------------------------------
int CGraphObjPolygon::extractIndexFromPolygonPointLabelName(const QString& i_strLabelName)
//------------------------------------------------------------------------------
{
    int idxPt = -1;
    if (i_strLabelName.startsWith(c_strGeometryLabelNameP)) {
        QString strIndex = i_strLabelName.mid(1);
        bool bIsValidIndex = false;
        int idxPtTmp = strIndex.toInt(&bIsValidIndex);
        // The label name for the polygon point at index 0 is P1.
        if (bIsValidIndex && idxPtTmp > 0) {
            idxPt = idxPtTmp - 1;
        }
    }
    return idxPt;
}

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjPolygon::CGraphObjPolygon(CDrawingScene* i_pDrawingScene, EGraphObjType i_graphObjType, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ i_graphObjType,
        /* strType             */ ZS::Draw::graphObjType2Str(i_graphObjType),
        /* strObjName          */ i_strObjName.isEmpty() ? "Polygon" + QString::number(s_iInstCount) : i_strObjName),
    QGraphicsPolygonItem(),
    m_polygonOrig(),
    m_physValPolygonOrig(*m_pDrawingScene),
    m_physValPolygonScaled(*m_pDrawingScene),
    m_physValPolygonScaledAndRotated(*m_pDrawingScene),
    m_plgLineStartArrowHead(),
    m_plgLineEndArrowHead(),
    m_idxsAdded(),
    m_idxsRemoved()
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
}

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjPolygon::~CGraphObjPolygon()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObjPolygon::dtor",
        /* strAddInfo   */ "" );

    m_bDtorInProgress = true;
    emit_aboutToBeDestroyed();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Changes the type of the graphical object to be either a polygon
           or a polyline.

    For polygons the last and first end points are connected.

    @param [in] i_graphObjType
        Range [EGraphObjTypePolyline, EGraphObjTypePolygon]
*/
void CGraphObjPolygon::setType(EGraphObjType i_graphObjType)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = graphObjType2Str(i_graphObjType);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setType",
        /* strAddInfo   */ strMthInArgs );

    if (i_graphObjType != EGraphObjTypePolygon && i_graphObjType != EGraphObjTypePolyline) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, graphObjType2Str(i_graphObjType));
    }
    if (m_type != i_graphObjType) {
        m_type = i_graphObjType;
        m_strType = graphObjType2Str(m_type);
        update();
        if (m_pTree != nullptr) {
            m_pTree->onTreeEntryChanged(this);
        }
        emit_typeChanged(m_type);
    }
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjPolygon::type() const
//------------------------------------------------------------------------------
{
    return m_type;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjPolygon::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CGraphObjPolygon* pGraphObj = new CGraphObjPolygon(m_pDrawingScene, m_type, m_strName);
    pGraphObj->setPolygon(getPolygon(drawingSize.unit()));
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
void CGraphObjPolygon::onCreateAndExecDlgFormatGraphObjs()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onCreateAndExecDlgFormatGraphObjs",
        /* strAddInfo   */ "" );

    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Format Line";
    CDlgGraphObjPolygonProperties* pDlg = CDlgGraphObjPolygonProperties::GetInstance(this);
    if( pDlg == nullptr ) {
        pDlg = CDlgGraphObjPolygonProperties::CreateInstance(strDlgTitle, this);
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
void CGraphObjPolygon::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
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
void CGraphObjPolygon::setPolygon(const CPhysValPolygon& i_physValPolygon)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPolygon.toString(true) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPolygon",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    if (i_physValPolygon.count() < 2) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange,
            "Polygon size " + QString::number(i_physValPolygon.count()) + " is less than 2");
    }

    QPointF ptPosPrev = pos();

    // Depending on the Y scale orientation of the drawing scene the polygon coordinates
    // have been passed either relative to the top left or bottom right corner of the
    // parent item's bounding rectangle.
    // The coordinates need to be transformed into the local coordinate system of the graphical
    // object whose origin point is the center of the objects bounding rectangle.

    QPolygonF polygon;
    CPhysVal physValAngle;
    QPointF ptPos = getItemPosAndLocalCoors(i_physValPolygon, polygon, physValAngle);

    bool bGeometryOnSceneChanged = false;

    // If the coordinates MUST be updated (e.g. after the drawing size has been changed)
    // or if the coordinates have been changed ...
    if (m_physValPolygonOrig.isNull() || this->polygon() != polygon || m_physValPolygonOrig != i_physValPolygon)
    {
        // If the added or removed indices are unknown, we must assume something.
        if ((i_physValPolygon.count() > m_physValPolygonOrig.count()) && (m_idxsAdded.second <= 0)) {
            // In case the number of points have been increased, labels will be added at the end.
            m_idxsAdded.first = m_physValPolygonOrig.count();
            m_idxsAdded.second = i_physValPolygon.count() - m_physValPolygonOrig.count();
        }
        else if ((i_physValPolygon.count() < m_physValPolygonOrig.count()) && (m_idxsRemoved.second <= 0)) {
            // In case the number of points have been decreased, labels will be removed at the end.
            m_idxsRemoved.first = i_physValPolygon.count();
            m_idxsRemoved.second = m_physValPolygonOrig.count() - i_physValPolygon.count();
        }

        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsItem_prepareGeometryChange();

        {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdatePhysValCoorsBlockedCounter);
            CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

            // Store physical coordinates.
            CPhysValPolygon physValPolygon(i_physValPolygon);
            physValPolygon.setAngle(0.0);
            setPhysValPolygonOrig(physValPolygon);
            setPhysValPolygonScaled(physValPolygon);
            setPhysValPolygonScaledAndRotated(i_physValPolygon);

            // Set the polygon in local coordinate system.
            // Also note that itemChange must not overwrite the current values (refCountGuard).
            setPolygonOrig(polygon);
            QGraphicsPolygonItem_setPolygon(polygon);

            // Please note that GraphicsPolygonItem::setPolygon did not update the position of the
            // item in the parent. This has to be done "manually" afterwards.

            // Move the object to the parent position.
            // This has to be done after resizing the polygon which updates the local coordinates
            // of the polygon with origin (0/0) at the polygons center point.
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
            if (physValAngle != m_physValRotationAngle) {
                m_physValRotationAngle = physValAngle;
                QGraphicsItem_setRotation(m_physValRotationAngle.getVal(Units.Angle.Degree));
            }
        }
        // If the geometry of the parent on the scene of this item changes, also the geometry
        // on the scene of this item is changed.
        bGeometryOnSceneChanged = true;

        if (m_idxsAdded.second > 0) {
            updateLabelsOnPolygonPointsAdded();
        }
        else if (m_idxsRemoved.second > 0) {
            updateLabelsOnPolygonPointsRemoved();
        }
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
    // Emit signal after updated position info has been traced.
    if (bGeometryOnSceneChanged) {
        emit_geometryOnSceneChanged();
    }
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setPolygon(const QPolygonF& i_polygon, const ZS::PhysVal::CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "[" + QString::number(i_polygon.count()) + "](" + qPolygon2Str(i_polygon) + ") " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setLine",
        /* strAddInfo   */ strMthInArgs );

    setPolygon(CPhysValPolygon(*m_pDrawingScene, i_polygon, i_unit));
}

//------------------------------------------------------------------------------
CPhysValPolygon CGraphObjPolygon::getPolygon() const
//------------------------------------------------------------------------------
{
    return getPolygon(m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
CPhysValPolygon CGraphObjPolygon::getPolygon(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (parentGroup() != nullptr) {
        return parentGroup()->convert(m_physValPolygonScaledAndRotated, i_unit);
    }
    else {
        return m_pDrawingScene->convert(m_physValPolygonScaledAndRotated, i_unit);
    }
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setCenter(const QPointF& i_pt)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setCenter(i_pt);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setCenter(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setCenter(i_physValPoint);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::getCenter() const
//------------------------------------------------------------------------------
{
    return getPolygon().center();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::getCenter(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolygon(i_unit).center();
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setSize(const QSizeF& i_size)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setSize(i_size);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setSize(const CPhysValSize& i_physValSize)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setSize(i_physValSize);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObjPolygon::getSize() const
//------------------------------------------------------------------------------
{
    return getPolygon().size();
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObjPolygon::getSize(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolygon(i_unit).size();
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setWidth(double i_fWidth)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setWidth(i_fWidth);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setWidth(const CPhysVal& i_physValWidth)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setWidth(i_physValWidth);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setWidthByMovingLeftCenter(const QPointF& i_pt)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setWidthByMovingLeftCenter(i_pt);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setWidthByMovingLeftCenter(i_physValPoint);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setWidthByMovingRightCenter(const QPointF& i_pt)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setWidthByMovingRightCenter(i_pt);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setWidthByMovingRightCenter(i_physValPoint);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjPolygon::getWidth() const
//------------------------------------------------------------------------------
{
    return getPolygon().width();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjPolygon::getWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolygon(i_unit).width();
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setHeight(double i_fHeight)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setHeight(i_fHeight);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setHeight(const CPhysVal& i_physValHeight)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setHeight(i_physValHeight);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setHeightByMovingTopCenter(const QPointF& i_pt)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setHeightByMovingTopCenter(i_pt);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setHeightByMovingTopCenter(i_physValPoint);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setHeightByMovingBottomCenter(const QPointF& i_pt)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setHeightByMovingBottomCenter(i_pt);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setHeightByMovingBottomCenter(i_physValPoint);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjPolygon::getHeight() const
//------------------------------------------------------------------------------
{
    return getPolygon().height();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjPolygon::getHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolygon(i_unit).height();
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setTopLeft(const QPointF& i_pt)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setTopLeft(i_pt);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setTopLeft(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setTopLeft(i_physValPoint);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::getTopLeft() const
//------------------------------------------------------------------------------
{
    return getPolygon().topLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::getTopLeft(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolygon(i_unit).topLeft();
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setTopRight(const QPointF& i_pt)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setTopRight(i_pt);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setTopRight(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setTopRight(i_physValPoint);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::getTopRight() const
//------------------------------------------------------------------------------
{
    return getPolygon().topRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::getTopRight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolygon(i_unit).topRight();
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setBottomRight(const QPointF& i_pt)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setBottomRight(i_pt);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setBottomRight(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setBottomRight(i_physValPoint);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::getBottomRight() const
//------------------------------------------------------------------------------
{
    return getPolygon().bottomRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::getBottomRight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolygon(i_unit).bottomRight();
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setBottomLeft(const QPointF& i_pt)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setBottomLeft(i_pt);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::setBottomLeft(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setBottomLeft(i_physValPoint);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::getBottomLeft() const
//------------------------------------------------------------------------------
{
    return getPolygon().bottomLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::getBottomLeft(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getPolygon(i_unit).bottomLeft();
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

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
void CGraphObjPolygon::setRotationAngle(double i_fAngle_degree)
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
void CGraphObjPolygon::setRotationAngle(const ZS::PhysVal::CPhysVal& i_physValAngle)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.setAngle(i_physValAngle);
    setPolygon(physValPolygon);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjPolygon::isEmpty() const
//------------------------------------------------------------------------------
{
    return m_physValPolygonScaledAndRotated.isEmpty();
}

//------------------------------------------------------------------------------
int CGraphObjPolygon::count() const
//------------------------------------------------------------------------------
{
    return m_physValPolygonScaledAndRotated.count();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::at(int i_idx) const
//------------------------------------------------------------------------------
{
    return m_physValPolygonScaledAndRotated.at(i_idx);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::replace(int i_idx, const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.replace(i_idx, i_physValPoint);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::append(const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    m_idxsAdded = qMakePair(physValPolygon.count(), 1);
    physValPolygon.append(i_physValPoint);
    setPolygon(physValPolygon);
    m_idxsAdded = qMakePair(-1, 0);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::insert(int i_idx, const CPhysValPoint& i_physValPoint)
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

    CPhysValPolygon physValPolygon = getPolygon();
    m_idxsAdded = qMakePair(i_idx, 1);
    physValPolygon.insert(i_idx, i_physValPoint);
    setPolygon(physValPolygon);
    m_idxsAdded = qMakePair(-1, 0);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::remove(int i_idx, int i_iCount)
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

    CPhysValPolygon physValPolygon = getPolygon();
    m_idxsRemoved = qMakePair(i_idx, i_iCount);
    physValPolygon.remove(i_idx, i_iCount);
    setPolygon(physValPolygon);
    m_idxsRemoved = qMakePair(-1, 0);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::removeAt(int i_idx)
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

    CPhysValPolygon physValPolygon = getPolygon();
    m_idxsRemoved = qMakePair(i_idx, 1);
    physValPolygon.removeAt(i_idx);
    setPolygon(physValPolygon);
    m_idxsRemoved = qMakePair(-1, 0);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::removeFirst()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "removeFirst",
        /* strAddInfo   */ "" );

    CPhysValPolygon physValPolygon = getPolygon();
    m_idxsRemoved = qMakePair(0, 1);
    physValPolygon.removeFirst();
    setPolygon(physValPolygon);
    m_idxsRemoved = qMakePair(-1, 0);
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::removeLast()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "removeLast",
        /* strAddInfo   */ "" );

    CPhysValPolygon physValPolygon = getPolygon();
    m_idxsRemoved = qMakePair(physValPolygon.count()-1, 1);
    physValPolygon.removeLast();
    setPolygon(physValPolygon);
    m_idxsRemoved = qMakePair(-1, 0);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::takeAt(int i_idx)
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

    CPhysValPolygon physValPolygon = getPolygon();
    m_idxsRemoved = qMakePair(i_idx, 1);
    CPhysValPoint physValPoint = physValPolygon.takeAt(i_idx);
    setPolygon(physValPolygon);
    m_idxsRemoved = qMakePair(-1, 0);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(physValPoint.toString());
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::takeFirst()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "takeFirst",
        /* strAddInfo   */ "" );

    CPhysValPolygon physValPolygon = getPolygon();
    m_idxsRemoved = qMakePair(0, 1);
    CPhysValPoint physValPoint = physValPolygon.takeFirst();
    setPolygon(physValPolygon);
    m_idxsRemoved = qMakePair(-1, 0);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(physValPoint.toString());
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::takeLast()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "takeLast",
        /* strAddInfo   */ "" );

    CPhysValPolygon physValPolygon = getPolygon();
    m_idxsRemoved = qMakePair(physValPolygon.count()-1, 1);
    CPhysValPoint physValPoint = physValPolygon.takeLast();
    setPolygon(physValPolygon);
    m_idxsRemoved = qMakePair(-1, 0);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(physValPoint.toString());
    }
    return physValPoint;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplements the virtual method of base class CGraphObj.
*/
QRectF CGraphObjPolygon::getBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ "" );

    // Points in local coordinates.
    QPolygonF polygon = this->polygon();
    QRectF rctBounding = polygon.boundingRect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Reimplements the virtual method of base class CGraphObj.
*/
QRectF CGraphObjPolygon::getEffectiveBoundingRectOnScene() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getEffectiveBoundingRectOnScene",
        /* strAddInfo   */ "" );

    const QGraphicsPolygonItem* pGraphicsItemThis = dynamic_cast<const QGraphicsPolygonItem*>(this);
    QPolygonF polygon = pGraphicsItemThis->polygon();
    polygon = pGraphicsItemThis->mapToScene(polygon);
    QRectF rctBounding = polygon.boundingRect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Returns the rotated, physical bounding rectangle.
*/
CPhysValRect CGraphObjPolygon::getPhysValBoundingRect(const CUnit& i_unit) const
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

    CPhysValRect physValRectBounding = m_physValPolygonScaledAndRotated.physValBoundingRect();
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
public: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//bool CGraphObjPolygon::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
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
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the proposed cursor shape for the given point.

    The cursor shape depends which shape point of the object has been hit.
    If a selection point has been hit, the position and type of selection point
    defines the cursor shape.

    If no selection point is hit, the cursor shape is defined which position
    of the line has been hit. If the start or end point of the line is hit
    (and if there is no selection point over those end points), the cursor
    shape is a cross cursor. On any other point of the line the proposed cursor
    shape is a SizeAllCursor to indicate that the line may be moved when selecting
    the line and moving the mouse.

    @param i_pt [in] Point to be check in local coordinates.
*/
QCursor CGraphObjPolygon::getProposedCursor(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCursor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Point {" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCursor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getProposedCursor",
        /* strAddInfo   */ strMthInArgs );

    QCursor cursor = Qt::ArrowCursor;
    if (m_bIsHit) {
        cursor = Qt::SizeAllCursor;
    }
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        CGraphObjSelectionPoint* pGraphObjSelPtHit = getSelectionPointHit(i_pt);
        if (pGraphObjSelPtHit != nullptr) {
            cursor = pGraphObjSelPtHit->getProposedCursor(i_pt);
        }
        else {
            SGraphObjHitInfo hitInfo;
            if (isPolygonHit(polygon(), m_drawSettings.getFillStyle(), i_pt, m_pDrawingScene->getHitToleranceInPx(), &hitInfo)) {
                cursor = hitInfo.m_cursor;
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qCursorShape2Str(cursor.shape()));
    }
    return cursor;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjPolygon::getPositionOfSelectionPoint(
    ESelectionPointType i_selPtType, int i_idxPt, const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (i_selPtType != ESelectionPointType::PolygonPoint && i_selPtType != ESelectionPointType::LineCenterPoint) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
    CPhysValPoint physValPoint(*m_pDrawingScene);
    if (m_physValPolygonScaledAndRotated.count() >= 2) {
        if (i_selPtType == ESelectionPointType::PolygonPoint) {
            physValPoint = m_physValPolygonScaledAndRotated.at(i_idxPt);
        }
        else if (i_selPtType == ESelectionPointType::LineCenterPoint) {
            if (i_idxPt == (m_physValPolygonScaledAndRotated.count() - 1)) {
                CPhysValPoint physValPointP1 = m_physValPolygonScaledAndRotated.at(i_idxPt);
                CPhysValPoint physValPointP2 = m_physValPolygonScaledAndRotated.at(0);
                physValPoint = CPhysValLine(physValPointP1, physValPointP2).center();
            }
            else {
                CPhysValPoint physValPointP1 = m_physValPolygonScaledAndRotated.at(i_idxPt);
                CPhysValPoint physValPointP2 = m_physValPolygonScaledAndRotated.at(i_idxPt+1);
                physValPoint = CPhysValLine(physValPointP1, physValPointP2).center();
            }
        }
        if (parentGroup() != nullptr) {
            physValPoint = parentGroup()->convert(physValPoint, i_unit);
        }
        else {
            physValPoint = m_pDrawingScene->convert(physValPoint, i_unit);
        }
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
/*! @brief Returns coordinates of selection point in scene coordinates.
*/
QPointF CGraphObjPolygon::getPositionOfSelectionPointInSceneCoors(
    ESelectionPointType i_selPtType, int i_idxPt) const
//------------------------------------------------------------------------------
{
    CPhysValPoint physValPoint(*m_pDrawingScene);
    if (m_physValPolygonScaledAndRotated.count() >= 2) {
        if (i_selPtType == ESelectionPointType::PolygonPoint) {
            physValPoint = m_physValPolygonScaledAndRotated.at(i_idxPt);
        }
        else if (i_selPtType == ESelectionPointType::LineCenterPoint) {
            if (i_idxPt == (m_physValPolygonScaledAndRotated.count() - 1)) {
                CPhysValPoint physValPointP1 = m_physValPolygonScaledAndRotated.at(i_idxPt);
                CPhysValPoint physValPointP2 = m_physValPolygonScaledAndRotated.at(0);
                physValPoint = CPhysValLine(physValPointP1, physValPointP2).center();
            }
            else {
                CPhysValPoint physValPointP1 = m_physValPolygonScaledAndRotated.at(i_idxPt);
                CPhysValPoint physValPointP2 = m_physValPolygonScaledAndRotated.at(i_idxPt+1);
                physValPoint = CPhysValLine(physValPointP1, physValPointP2).center();
            }
        }
        if (parentGroup() != nullptr) {
            physValPoint = parentGroup()->mapToScene(physValPoint);
        }
        physValPoint = m_pDrawingScene->convert(physValPoint, Units.Length.px);
    }
    return physValPoint.toQPointF();
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates in length and angle of the given point
           to the selection point of the graphical object.

    How the angle of anchor lines to selection points is interpreted depends on
    the graphical object type and the selection point.

    For more details see base implementation in CGraphObj.
*/
SPolarCoors CGraphObjPolygon::getPolarCoorsToSelectionPointFromSceneCoors(
    const QPointF& i_pt, ESelectionPointType i_selPtType, int i_idxPt) const
//------------------------------------------------------------------------------
{
    if (i_selPtType != ESelectionPointType::PolygonPoint && i_selPtType != ESelectionPointType::LineCenterPoint) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPolygonF thisPolygon = polygon();
    QPointF ptThisLineSceneCoorsP1;
    QPointF ptThisLineSceneCoorsP2;
    if (thisPolygon.size() >= 2) {
        if (i_selPtType == ESelectionPointType::PolygonPoint) {
            if (i_idxPt == (thisPolygon.size() - 1)) {
                ptThisLineSceneCoorsP1 = thisPolygon[i_idxPt];
                ptThisLineSceneCoorsP2 = thisPolygon[0];
            }
            else {
                ptThisLineSceneCoorsP1 = thisPolygon[i_idxPt];
                ptThisLineSceneCoorsP2 = thisPolygon[i_idxPt+1];
            }
        }
        else if (i_selPtType == ESelectionPointType::LineCenterPoint) {
            if (i_idxPt == (thisPolygon.size() - 1)) {
                ptThisLineSceneCoorsP1 = thisPolygon[i_idxPt];
                ptThisLineSceneCoorsP2 = thisPolygon[0];
            }
            else {
                ptThisLineSceneCoorsP1 = thisPolygon[i_idxPt];
                ptThisLineSceneCoorsP2 = thisPolygon[i_idxPt+1];
            }
            ptThisLineSceneCoorsP1 = QLineF(ptThisLineSceneCoorsP1, ptThisLineSceneCoorsP2).center();
        }
    }
    ptThisLineSceneCoorsP1 = pGraphicsItemThis->mapToScene(ptThisLineSceneCoorsP1);
    ptThisLineSceneCoorsP2 = pGraphicsItemThis->mapToScene(ptThisLineSceneCoorsP2);
    QLineF thisLineSceneCoors(ptThisLineSceneCoorsP1, ptThisLineSceneCoorsP2);
    return ZS::Draw::getPolarCoors(thisLineSceneCoors, i_pt);
}

//------------------------------------------------------------------------------
/*! @brief Returns a line with the given length and angle with the start point (P1)
           at the given selection point in scene coordinates.

    For more details see base implementation in CGraphObj.
*/
QLineF CGraphObjPolygon::getAnchorLineToSelectionPointFromPolarInSceneCoors(
    const SPolarCoors& i_polarCoors, ESelectionPointType i_selPtType, int i_idxPt) const
//------------------------------------------------------------------------------
{
    if (i_selPtType != ESelectionPointType::PolygonPoint && i_selPtType != ESelectionPointType::LineCenterPoint) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, CEnumSelectionPointType(i_selPtType).toString());
    }
    QLineF anchorLine;
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPolygonF plgSelPt = polygon();
    QLineF lineSelPtSceneCoors;
    if (plgSelPt.size() >= 2) {
        if (i_selPtType == ESelectionPointType::PolygonPoint) {
            if (i_idxPt == (plgSelPt.size() - 1)) {
                lineSelPtSceneCoors = QLineF(pGraphicsItemThis->mapToScene(plgSelPt[i_idxPt]),
                                             pGraphicsItemThis->mapToScene(plgSelPt[0]));
            }
            else {
                lineSelPtSceneCoors = QLineF(pGraphicsItemThis->mapToScene(plgSelPt[i_idxPt]),
                                             pGraphicsItemThis->mapToScene(plgSelPt[i_idxPt+1]));
            }
            anchorLine = ZS::Draw::getLineFromPolar(
                i_polarCoors.m_fLength_px, i_polarCoors.m_fAngle_degrees, lineSelPtSceneCoors);
        }
        else if (i_selPtType == ESelectionPointType::LineCenterPoint) {
            if (i_idxPt == (plgSelPt.size() - 1)) {
                lineSelPtSceneCoors = QLineF(pGraphicsItemThis->mapToScene(plgSelPt[i_idxPt]),
                                             pGraphicsItemThis->mapToScene(plgSelPt[0]));
                lineSelPtSceneCoors.setP1(lineSelPtSceneCoors.center());
            }
            else {
                lineSelPtSceneCoors = QLineF(pGraphicsItemThis->mapToScene(plgSelPt[i_idxPt]),
                                             pGraphicsItemThis->mapToScene(plgSelPt[i_idxPt+1]));
                lineSelPtSceneCoors.setP1(lineSelPtSceneCoors.center());
            }
            anchorLine = ZS::Draw::getLineFromPolar(
                i_polarCoors.m_fLength_px, i_polarCoors.m_fAngle_degrees, lineSelPtSceneCoors);
        }
    }
    return anchorLine;
}

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjPolygon::showSelectionPoints(TSelectionPointTypes i_selPts)
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
        if (i_selPts & c_uSelectionPointsPolygonPoints) {
            showSelectionPointsOfPolygon(polygon());
        }
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj (text labels)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the list of the possible anchor points for the given label name.

    For the predefined labels "Name", "P1" and "P2" the following applies:

    - The "Name" label may be anchored to the center point of the line.
    - "P1" may be anchored to the start of the line (SelectionPoint = TopLeft).
    - "P2" may be anchored to the end of the line (SelectionPoint = BottomRight).

    User defined labels may be anchored to either Center, Start or End of the line.

    Please note that the most common used selection points should be at the beginning
    of the list so that combo boxes to select the selection point start with those.

    @return List of possbile selection points.
*/
QList<SGraphObjSelectionPoint> CGraphObjPolygon::getPossibleLabelAnchorPoints(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    QList<SGraphObjSelectionPoint> arSelPts;
    if (i_strName == c_strLabelName) {
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjPolygon*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::Center));
        for (int idxPt = 0; idxPt < polygon().size(); ++idxPt) {
            arSelPts.append(SGraphObjSelectionPoint(
                const_cast<CGraphObjPolygon*>(this), ESelectionPointType::LineCenterPoint, idxPt));
        }
    }
    else if (isPolygonPointLabelName(i_strName)) {
        int idxPt = extractIndexFromPolygonPointLabelName(i_strName);
        if (idxPt >= 0 && idxPt < polygon().size()) {
            arSelPts.append(SGraphObjSelectionPoint(
                const_cast<CGraphObjPolygon*>(this), ESelectionPointType::PolygonPoint, idxPt));
        }
    }
    return arSelPts;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the label with the passed name has been modified or still
           has its default values.

    @param [in] i_strName
        Name of the label to be checked.

    @return true if the label still has its default values, false otherwise.
*/
bool CGraphObjPolygon::labelHasDefaultValues(const QString& i_strName) const
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
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::LineCenterPoint) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_idxPt != 0) {
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
bool CGraphObjPolygon::geometryLabelHasDefaultValues(const QString& i_strName) const
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
        else if (!labelDscr.m_strText.isEmpty() && labelDscr.m_strText != labelDscr.m_strKey) {
            bHasDefaultValues = false;
        }
        else if (labelDscr.m_polarCoorsToLinkedSelPt != SPolarCoors()) {
            bHasDefaultValues = false;
        }
        else if (i_strName == c_strGeometryLabelNameCenter) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::Center) {
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
QRectF CGraphObjPolygon::boundingRect() const
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
QPainterPath CGraphObjPolygon::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );

    // QGraphicsPolygonItem::shape() throws an exception if any segment has a zero length.
    // Before invoking QGraphicsPolygonItem::shape() we check whether the polygons bounding
    // rectangle has at least 10 pixels height and width.
    QPolygonF polygon = this->polygon();
    QRectF rctBounding = polygon.boundingRect();
    QPainterPath painterPath;
    if (rctBounding.width() > 10.0 && rctBounding.height() > 10.0) {
        painterPath = QGraphicsPolygonItem::shape();
        if (!m_plgLineStartArrowHead.isEmpty()) {
            painterPath.addPolygon(m_plgLineStartArrowHead);
        }
        if (!m_plgLineEndArrowHead.isEmpty()) {
            painterPath.addPolygon(m_plgLineEndArrowHead);
        }
    }
    else {
        painterPath.addRect(getBoundingRect());
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
void CGraphObjPolygon::paint(
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

    QPolygonF polygon = this->polygon();
    if (polygon.size() < 2) {
        return;
    }

    i_pPainter->save();

    QPen pn = pen();
    QBrush brsh;
    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHit || m_bIsHighlighted || isSelected())) {
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
        outline.moveTo(polygon[0]);
        for (int idxPt = 1; idxPt < polygon.size(); ++idxPt) {
            outline.lineTo(polygon[idxPt]);
        }
        if (m_type == EGraphObjTypePolygon) {
            outline.lineTo(polygon[0]);
        }
        i_pPainter->strokePath(outline, pn);
        pn.setWidth(1 + m_drawSettings.getPenWidth());
    }
    i_pPainter->setRenderHints(s_painterRenderHints);
    pn.setColor(m_drawSettings.getPenColor());
    pn.setWidth(m_drawSettings.getPenWidth());
    pn.setStyle(lineStyle2QtPenStyle(m_drawSettings.getLineStyle().enumerator()));
    i_pPainter->setPen(pn);
    if (m_type == EGraphObjTypePolygon) {
        brsh.setColor(m_drawSettings.getFillColor());
        brsh.setStyle(fillStyle2QtBrushStyle(m_drawSettings.getFillStyle()));
        i_pPainter->setBrush(brsh);
        i_pPainter->drawPolygon(polygon);
    }
    else if (m_type == EGraphObjTypePolyline) {
        i_pPainter->drawPolyline(polygon);
    }

    CEnumLineEndStyle lineEndStyleLineStart = m_drawSettings.getLineEndStyle(ELinePoint::Start);
    CEnumLineEndStyle lineEndStyleLineEnd = m_drawSettings.getLineEndStyle(ELinePoint::End);
    if (lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal) {
        CEnumArrowHeadBaseLineType baseLineTypeLineStart = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::Start);
        CEnumArrowHeadBaseLineType baseLineTypeLineEnd   = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::End);
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        i_pPainter->setPen(pn);
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
        if (!polygon.isEmpty()) {
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
//bool CGraphObjPolygon::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
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
void CGraphObjPolygon::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
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
void CGraphObjPolygon::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
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
void CGraphObjPolygon::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
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
void CGraphObjPolygon::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
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

    // Forward the mouse event to the base implementation.
    // This will select the item, creating selection points if not yet created.
    QGraphicsPolygonItem::mousePressEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
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
void CGraphObjPolygon::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
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
void CGraphObjPolygon::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
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
//            // The position of the polygon should become the top left corner of it's bounding rectangle.
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
void CGraphObjPolygon::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
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
//            // The position of the polygon should become the top left corner of it's bounding rectangle.
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
QVariant CGraphObjPolygon::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
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
        todo: traceInternalStates("States");
        showSelectionPoints of boundingRect not if EditMode = CreatingByMouseEvents
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
protected: // overridable slots of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjPolygon::onDrawingSizeChanged(const CDrawingSize& i_drawingSize)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_drawingSize.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "onDrawingSizeChanged",
//        /* strAddInfo   */ strMthInArgs );
//
//    if (m_physValLineCurr.unit() != i_drawingSize.unit()) {
//        m_bForceConversionToSceneCoors = true;
//        setLine(m_pDrawingScene->convert(m_physValLineCurr, i_drawingSize.unit()));
//        m_bForceConversionToSceneCoors = false;
//        emit_geometryValuesUnitChanged();
//    }
//}

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjPolygon::onGraphObjParentGeometryOnSceneChanged(
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
    tracePositionInfo(mthTracer, EMethodDir::Enter);

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
            CPhysValPolygon physValPolygon = getPhysValPolygonScaled(m_physValPolygonOrig);
            setPhysValPolygonScaled(physValPolygon);
            physValPolygon.setAngle(m_physValRotationAngle);
            setPhysValPolygonScaledAndRotated(physValPolygon);

            QPointF ptPosPrev = pos();

            QPolygonF polygon;
            CPhysVal physValAngle;
            QPointF ptPos = getItemPosAndLocalCoors(physValPolygon, polygon, physValAngle);

            // Prepare the item for a geometry change. This function must be called before
            // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
            QGraphicsItem_prepareGeometryChange();

            {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdatePhysValCoorsBlockedCounter);
                CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

                // Set the polygon in local coordinate system.
                // Also note that itemChange must not overwrite the current coordinates (refCountGuard).
                QGraphicsPolygonItem_setPolygon(polygon);

                // Please note that GraphicsPolygonItem::setPolygon did not update the position of the
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
                else {
                    updateLineEndArrowHeadPolygons();
                }
            }
            // If the geometry of the parent on the scene of this item changes, also the geometry
            // on the scene of this item is changed.
            bGeometryOnSceneChanged = true;
        }
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);

    // Emit signal after updated position info has been traced.
    if (bGeometryOnSceneChanged) {
        emit_geometryOnSceneChanged();
    }
}

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjPolygon::onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint)
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
    CGraphObjSelectionPoint* pGraphObjSelPt = dynamic_cast<CGraphObjSelectionPoint*>(i_pSelectionPoint);
    QGraphicsItem* pGraphicsItemSelPt = dynamic_cast<QGraphicsItem*>(pGraphObjSelPt);
    QPointF ptScenePosSelPt = pGraphicsItemSelPt->scenePos();
    QPointF ptPosSelPt = mapFromScene(ptScenePosSelPt);
    QPointF ptParentPosSelPt = pGraphicsItemThis->mapToParent(ptPosSelPt);
    CPhysValPoint physValParentSelPt(*m_pDrawingScene);
    if (parentGroup() != nullptr) {
        physValParentSelPt = parentGroup()->convert(ptParentPosSelPt);
    }
    else {
        physValParentSelPt = m_pDrawingScene->convert(ptParentPosSelPt);
    }
    SGraphObjSelectionPoint selPt = pGraphObjSelPt->getSelectionPoint();

    //if (selPt.m_selPtType == ESelectionPointType::PolygonShapePoint) {
    //    if (selPt.m_idxPt == 0) {
    //        setP1(physValParentSelPt);
    //    }
    //    else if (selPt.m_idxPt == 1) {
    //        setP2(physValParentSelPt);
    //    }
    //}
}

/*==============================================================================
protected: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the pure virtual method of base class CGraphObj.
*/
void CGraphObjPolygon::updateTransformedCoorsOnParentChanged(
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
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

        setPolygonOrig(polygon());
        CPhysValPolygon physValPolygon = getPhysValPolygonOrig(m_polygonOrig);
        setPhysValPolygonOrig(physValPolygon);
        physValPolygon = getPhysValPolygonScaled(m_physValPolygonOrig);
        setPhysValPolygonScaled(physValPolygon);
        setPhysValPolygonScaledAndRotated(physValPolygon);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
    emit_geometryOnSceneChanged(true);
}

//------------------------------------------------------------------------------
/*! @brief Overrides the pure virtual method of base class CGraphObj.
*/
void CGraphObjPolygon::updateTransformedCoorsOnParentGeometryChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateTransformedCoorsOnParentGeometryChanged",
        /* strAddInfo   */ "" );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

        setPolygonOrig(polygon());
        CPhysValPolygon physValPolygon = getPhysValPolygonOrig(m_polygonOrig);
        setPhysValPolygonOrig(physValPolygon);
        physValPolygon = getPhysValPolygonScaled(m_physValPolygonOrig);
        setPhysValPolygonScaled(physValPolygon);
        physValPolygon.setAngle(m_physValRotationAngle);
        setPhysValPolygonScaledAndRotated(physValPolygon);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
    emit_geometryOnSceneChanged(true);
}

//------------------------------------------------------------------------------
/*! @brief Overrides the pure virtual method of base class CGraphObj.
*/
void CGraphObjPolygon::updateTransformedCoorsOnItemPositionChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateTransformedCoorsOnItemPositionChanged",
        /* strAddInfo   */ "" );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    // ItemChange is called but should not emit the geometryOnSceneChanged signal.
    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

        CPhysValPolygon physValPolygon = getPhysValPolygonOrig(m_polygonOrig);
        setPhysValPolygonOrig(physValPolygon);
        physValPolygon = getPhysValPolygonScaled(m_physValPolygonOrig);
        setPhysValPolygonScaled(physValPolygon);
        physValPolygon.setAngle(m_physValRotationAngle);
        setPhysValPolygonScaledAndRotated(physValPolygon);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
    emit_geometryOnSceneChanged();
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjPolygon::lineEndArrowHeadPolygonsNeedUpdate(
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
void CGraphObjPolygon::updateLineEndArrowHeadPolygons(const CEnumLinePoint& i_linePoint)
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
/*! @brief Calculates the scaled, not rotated polygon in pixels in item coordinates
           relative to the origin (center point) of the orignal bounding rectangle.

    The relative distance of the center point of the scaled polygon to the
    origin (center point) of the parent's bounding rectangle remains the same.
    The size is scaled to the scale factors of the parent group.

    @param [in] i_polygonOrig
        Unscaled polygon in the item's local coordinates system whose origin
        is the center point of the item.
    @return Scaled polygon.
*/
QPolygonF CGraphObjPolygon::getPolygonScaled(const QPolygonF& i_polygonOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPolygon2Str(i_polygonOrig) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPolygonScaled",
        /* strAddInfo   */ strMthInArgs );

    QRectF rctBounding = i_polygonOrig.boundingRect();
    QPointF ptCenter(m_fParentGroupScaleX * rctBounding.center().x(), m_fParentGroupScaleY * rctBounding.center().y());
    QPolygonF polygonScaled;
    for (int idxPt = 0; idxPt < i_polygonOrig.size(); ++idxPt) {
        double fDX_px = m_fParentGroupScaleX * i_polygonOrig[idxPt].x();
        double fDY_px = m_fParentGroupScaleY * i_polygonOrig[idxPt].y();
        QPointF pt(ptCenter.x() - fDX_px/2.0, ptCenter.y() - fDY_px/2.0);
        polygonScaled[idxPt] = pt;
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qPolygon2Str(polygonScaled) + "}");
    }
    return polygonScaled;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the unscaled and not rotated physical polygon in the
           current unit of the drawing scene relative to the top left or
           bottom left corner of the parent's bounding rectangle.

    @param [in] i_polygonOrig
        Unscaled, not rotated polygon in the item's local coordinates system
        whose origin is the center point of the item.
    @return Physical polygon whose origin is either the top left or bottom
            left corner of the parent's bounding rectangle.
*/
CPhysValPolygon CGraphObjPolygon::getPhysValPolygonOrig(const QPolygonF& i_polygonOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPolygon2Str(i_polygonOrig) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValPolygonOrig",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    CGraphObjPolygon* pVThis = const_cast<CGraphObjPolygon*>(this);
    double fRotationAngle_degree = m_physValRotationAngle.getVal(Units.Angle.Degree);
    if (fRotationAngle_degree != 0.0) {
        pVThis->QGraphicsItem_setRotation(0.0);
    }
    CPhysValPolygon physValPolygon(*m_pDrawingScene, i_polygonOrig);
    if (parentGroup() != nullptr) {
        for (int idxPt = 0; idxPt < i_polygonOrig.size(); ++idxPt) {
            QPointF pt = pGraphicsItemThis->mapToParent(i_polygonOrig[idxPt]);
            pt = parentGroup()->mapToTopLeftOfBoundingRect(pt);
            CPhysValPoint physValPoint = parentGroup()->convert(pt);
            physValPolygon.replace(idxPt, physValPoint);
        }
    }
    else {
        // Please note that "mapToScene" maps the local coordinates relative to the
        // top left corner of the item's bounding rectangle and there is no need to
        // call "mapToBoundingRectTopLeft" beforehand.
        for (int idxPt = 0; idxPt < i_polygonOrig.size(); ++idxPt) {
            QPointF pt = pGraphicsItemThis->mapToScene(i_polygonOrig[idxPt]);
            CPhysValPoint physValPoint = m_pDrawingScene->convert(pt);
            physValPolygon.replace(idxPt, physValPoint);
        }
    }
    if (fRotationAngle_degree != 0.0) {
        pVThis->QGraphicsItem_setRotation(fRotationAngle_degree);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValPolygon.toString() + "} " + physValPolygon.unit().symbol());
    }
    return physValPolygon;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the scaled, not rotated physical polygon in the current
           unit of the drawing scene relative to the top left or bottem left
           corner of the parent's bounding rectangle.

    The relative distance of the center point of the scaled polygon to the
    top left or bottom left corner of the parent's bounding rectangle remains the same.
    The size is scaled to the scale factors of the parent group.

    @param [in] i_physValPolygonOrig
        Unscaled polygon in the parent's, physical coordinates system whose origin
        is the top left or bottom right corner of the parent's bounding rectangle.
    @return Scaled polygon.
*/
CPhysValPolygon CGraphObjPolygon::getPhysValPolygonScaled(const CPhysValPolygon& i_physValPolygonOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPolygonOrig.toString() + "} " + i_physValPolygonOrig.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValPolygonScaled",
        /* strAddInfo   */ strMthInArgs );

    QPointF ptCenterOrig = i_physValPolygonOrig.center().toQPointF(m_pDrawingScene->drawingSize().unit());
    QPointF ptCenterScaled = ptCenterOrig;
    ptCenterScaled.setX(m_fParentGroupScaleX * ptCenterOrig.x());
    ptCenterScaled.setY(m_fParentGroupScaleY * ptCenterOrig.y());

    CPhysValPolygon physValPolygon(i_physValPolygonOrig);
    for (int idxPt = 0; idxPt < i_physValPolygonOrig.count(); ++idxPt) {
        double fDXOrig = ptCenterOrig.x() - i_physValPolygonOrig.at(idxPt).x().getVal();
        double fDYOrig = ptCenterOrig.y() - i_physValPolygonOrig.at(idxPt).y().getVal();
        double fDXScaled = m_fParentGroupScaleX * fDXOrig;
        double fDYScaled = m_fParentGroupScaleY * fDYOrig;
        CPhysValPoint physValPoint(*m_pDrawingScene);
        physValPoint.setX(ptCenterScaled.x() - fDXScaled);
        physValPoint.setY(ptCenterScaled.y() - fDYScaled);
        physValPolygon.replace(idxPt, physValPoint);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValPolygon.toString() + "} " + physValPolygon.unit().symbol());
    }
    return physValPolygon;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the item position relative to the parent item or drawing scene
           as well as the item coordinates in local coordinates.

    @param [in] i_physValPolygon
        Polyline in parent coordinates, depending on the Y scale orientation
        relative to the top left or bottom left corner of parent item's bounding
        rectangle. If the item belongs to a parent group the passed polygon
        must have been resized and the center must have been moved according to the
        parents scale factors.
    @param [out] o_polygon
        Polygon in local coordinates.
    @param [out] o_physValAngle
        The rotatian angle of the passed rectangle.
*/
QPointF CGraphObjPolygon::getItemPosAndLocalCoors(
    const CPhysValPolygon& i_physValPolygon, QPolygonF& o_polygon, CPhysVal& o_physValAngle) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPolygon.toString() + "} " + i_physValPolygon.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getItemPosAndLocalCoors",
        /* strAddInfo   */ strMthInArgs );

    // First determine the position of the item in the parent's (scene or group) coordinate system.
    CPhysValPolygon physValPolygonTmp(i_physValPolygon);
    // For the graphics item the rotation angle is set explicitly applied to the unrotated coordinates.
    o_physValAngle = physValPolygonTmp.angle();
    physValPolygonTmp.setAngle(0.0);
    if (parentGroup() != nullptr) {
        physValPolygonTmp = parentGroup()->convert(physValPolygonTmp, Units.Length.px);
    }
    else {
        physValPolygonTmp = m_pDrawingScene->convert(physValPolygonTmp, Units.Length.px);
    }
    o_polygon = physValPolygonTmp.toQPolygonF();

    // Transform the parent coordinates into local coordinate system.
    // The origin is the center point of the polygon's bounding rectangle.
    QPointF ptPos = physValPolygonTmp.center().toQPointF(); // polygon here still in parent or scene coordinates
    for (int idxPt = 0; idxPt < o_polygon.size(); ++idxPt) {
        o_polygon[idxPt] = o_polygon[idxPt] - ptPos; // polygon points now in local coordinates
    }

    if (parentGroup() != nullptr) {
        ptPos = parentGroup()->mapFromTopLeftOfBoundingRect(ptPos);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "Polygon {" + qPolygon2Str(o_polygon) + "}, Angle: " + o_physValAngle.toString();
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn("{" + qPoint2Str(ptPos) + "}");
    }
    return ptPos;
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::normalize()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "normalize",
        /* strAddInfo   */ "" );

    QPolygonF plgOld = getPolygon(m_pDrawingScene->drawingSize().unit()).toQPolygonF();
    QPolygonF plgNew = normalizePolygon(plgOld, m_pDrawingScene->getHitToleranceInPx());
    if (plgOld.size() != plgNew.size()) {
        setPolygon(plgNew, m_pDrawingScene->drawingSize().unit());
        hideSelectionPoints();
        if (isSelected()) {
            showSelectionPoints();
        }
    }
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CGraphObjPolygon::getNumberOfPredefinedPolygonPointLabelNames() const
//------------------------------------------------------------------------------
{
    int iNumberOfLabels = 0;
    for (const QString& strLabelName : m_strlstPredefinedLabelNames) {
        if (isPolygonPointLabelName(strLabelName)) {
            ++iNumberOfLabels;
        }
    }
    return iNumberOfLabels;
}

//------------------------------------------------------------------------------
int CGraphObjPolygon::getNumberOfPolygonPointGeometryLabelNames() const
//------------------------------------------------------------------------------
{
    int iNumberOfLabels = 0;
    for (const QString& strLabelName : m_strlstGeometryLabelNames) {
        if (isPolygonPointLabelName(strLabelName)) {
            ++iNumberOfLabels;
        }
    }
    return iNumberOfLabels;
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliaray method to update text, position, user defined labels
           as well as geometry labels if polygon points have been added.
*/
void CGraphObjPolygon::updateLabelsOnPolygonPointsAdded()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateLabelsOnPolygonPointsAdded",
        /* strAddInfo   */ "" );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
    if (m_idxsAdded.second > 0) {
        if (!m_strlstPredefinedLabelNames.contains(c_strLabelName)) {
            m_strlstPredefinedLabelNames.append(c_strLabelName);
            addLabel(c_strLabelName, c_strLabelName, ESelectionPointType::LineCenterPoint, 0);
        }
        int iCountPrev = getNumberOfPredefinedPolygonPointLabelNames();
        for (int idxPt = iCountPrev; idxPt < iCountPrev + m_idxsAdded.second; ++idxPt) {
            const QString strLabelName = createPolygonPointLabelName(idxPt);
            m_strlstPredefinedLabelNames.append(strLabelName);
            addLabel(strLabelName, strLabelName, ESelectionPointType::PolygonPoint, idxPt);
        }
        int iCountNew = getNumberOfPredefinedPolygonPointLabelNames();
        // Update the indicated point labels. E.g. if inserting a point at the beginning,
        // previously the label P1 pointed to index 0. After inserting a point at index 0,
        // the label P1 must be renamed to P2 pointing to index 1.
        for (int idxPt = (iCountNew - 1); idxPt >= (m_idxsAdded.first + m_idxsAdded.second); --idxPt) {
            const QString strLabelNameOld = createPolygonPointLabelName(idxPt - m_idxsAdded.second);
            const QString strLabelNameNew = createPolygonPointLabelName(idxPt);
            SLabelDscr& labelDscrOld = m_hshLabelDscrs[strLabelNameOld];
            SLabelDscr& labelDscrNew = m_hshLabelDscrs[strLabelNameNew];
            labelDscrNew.m_bLabelIsVisible = labelDscrOld.m_bLabelIsVisible;
            labelDscrNew.m_bShowAnchorLine = labelDscrOld.m_bShowAnchorLine;
            labelDscrNew.m_polarCoorsToLinkedSelPt = labelDscrOld.m_polarCoorsToLinkedSelPt;
            CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(strLabelNameOld, nullptr);
            if (pGraphObjLabel != nullptr) {
                m_hshpLabels.remove(strLabelNameOld);
                m_hshpLabels[strLabelNameNew] = pGraphObjLabel;
                pGraphObjLabel->setKey(strLabelNameNew);
                pGraphObjLabel->setText(strLabelNameNew);
                pGraphObjLabel->setSelectionPoint1(labelDscrNew.m_selPt1);
                pGraphObjLabel->setSelectionPoint2(labelDscrNew.m_selPt2);
            }
            labelDscrOld.m_bLabelIsVisible = false;
            labelDscrOld.m_bShowAnchorLine = false;
            labelDscrOld.m_polarCoorsToLinkedSelPt = SPolarCoors();
        }
        // Update the position of the indicated name and user defined labels.
        // But only if previously a polygon was set ..
        if (iCountPrev > 1) {
            for (const QString& strLabelName : m_hshLabelDscrs.keys()) {
                if (!isPolygonPointLabelName(strLabelName)) {
                    CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(strLabelName, nullptr);
                    SLabelDscr& labelDscr = m_hshLabelDscrs[strLabelName];
                    if ((labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonPoint)
                        || (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::LineCenterPoint))
                    {
                        if (labelDscr.m_selPt1.m_idxPt >= m_idxsAdded.first) {
                            labelDscr.m_selPt1.m_idxPt += m_idxsAdded.second;
                        }
                    }
                    if (pGraphObjLabel != nullptr) {
                        pGraphObjLabel->setSelectionPoint1(labelDscr.m_selPt1);
                    }
                    if ((labelDscr.m_selPt2.m_selPtType == ESelectionPointType::PolygonPoint)
                        || (labelDscr.m_selPt2.m_selPtType == ESelectionPointType::LineCenterPoint))
                    {
                        if (labelDscr.m_selPt2.m_idxPt >= m_idxsAdded.first) {
                            labelDscr.m_selPt2.m_idxPt += m_idxsAdded.second;
                        }
                    }
                    if (pGraphObjLabel != nullptr) {
                        pGraphObjLabel->setSelectionPoint2(labelDscr.m_selPt2);
                    }
                }
            }
        }
        // Update geometry labels.
        iCountPrev = getNumberOfPolygonPointGeometryLabelNames();
        for (int idxPt = iCountPrev; idxPt < iCountPrev + m_idxsAdded.second; ++idxPt) {
            const QString strLabelName = createPolygonPointLabelName(idxPt);
            m_strlstGeometryLabelNames.append(strLabelName);
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, idxPt);
        }
        iCountNew = getNumberOfPolygonPointGeometryLabelNames();
        // Update the indicated position labels. E.g. if inserting a point at the beginning,
        // previously the label P1 pointed to index 0. After inserting a point at index 0,
        // the label P1 must be renamed to P2 pointing to index 1.
        for (int idxPt = (iCountNew - 1); idxPt >= (m_idxsAdded.first + m_idxsAdded.second); --idxPt) {
            const QString strLabelNameOld = createPolygonPointLabelName(idxPt - m_idxsAdded.second);
            const QString strLabelNameNew = createPolygonPointLabelName(idxPt);
            SLabelDscr& labelDscrOld = m_hshGeometryLabelDscrs[strLabelNameOld];
            SLabelDscr& labelDscrNew = m_hshGeometryLabelDscrs[strLabelNameNew];
            labelDscrNew.m_bLabelIsVisible = labelDscrOld.m_bLabelIsVisible;
            labelDscrNew.m_bShowAnchorLine = labelDscrOld.m_bShowAnchorLine;
            labelDscrNew.m_polarCoorsToLinkedSelPt = labelDscrOld.m_polarCoorsToLinkedSelPt;
            CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(strLabelNameOld, nullptr);
            if (pGraphObjLabel != nullptr) {
                m_hshpGeometryLabels.remove(strLabelNameOld);
                m_hshpGeometryLabels[strLabelNameNew] = pGraphObjLabel;
                pGraphObjLabel->setKey(strLabelNameNew);
                pGraphObjLabel->setText(strLabelNameNew);
                pGraphObjLabel->setSelectionPoint1(labelDscrNew.m_selPt1);
                pGraphObjLabel->setSelectionPoint2(labelDscrNew.m_selPt2);
            }
            labelDscrOld.m_bLabelIsVisible = false;
            labelDscrOld.m_bShowAnchorLine = false;
            labelDscrOld.m_polarCoorsToLinkedSelPt = SPolarCoors();
        }
        m_idxsAdded = qMakePair(-1, 0);
    }
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliaray method to update text, position, user defined labels
           as well as geometry labels if polygon points have been removed.
*/
void CGraphObjPolygon::updateLabelsOnPolygonPointsRemoved()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateLabelsOnPolygonPointsRemoved",
        /* strAddInfo   */ "" );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
    if (m_idxsRemoved.second > 0) {
        // Update text, position and user defined labels.
        int iCountPrev = m_idxsRemoved.first + m_idxsRemoved.second;
        for (int idxPt = m_idxsRemoved.first; idxPt < iCountPrev; ++idxPt) {
            const QString strLabelName = createPolygonPointLabelName(idxPt);
            m_strlstPredefinedLabelNames.removeOne(strLabelName);
            removeLabel(strLabelName);
        }
        int iCountNew = getNumberOfPredefinedPolygonPointLabelNames();
        // Update the indicated point labels. E.g. if removing the first point, previously
        // the label P2 pointed to index 1. After removing the point at index 0, the label
        // P2 must be renamed to P1 pointing to index 0.
        // But only if previously a polygon was set ..
        for (int idxPt = m_idxsRemoved.first; idxPt < iCountNew; ++idxPt) {
            const QString strLabelNameNew = createPolygonPointLabelName(idxPt);
            const QString strLabelNameOld = createPolygonPointLabelName(idxPt + m_idxsRemoved.second);
            SLabelDscr labelDscr = m_hshLabelDscrs[strLabelNameOld];
            labelDscr.m_strKey = strLabelNameNew;
            labelDscr.m_strText = strLabelNameNew;
            labelDscr.m_selPt1.m_idxPt = idxPt;
            m_hshLabelDscrs[strLabelNameNew] = labelDscr;
            m_hshLabelDscrs.remove(strLabelNameOld);
            int idxLabelName = m_strlstPredefinedLabelNames.indexOf(strLabelNameOld);
            m_strlstPredefinedLabelNames[idxLabelName] = strLabelNameNew;
            CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(strLabelNameOld, nullptr);
            if (pGraphObjLabel != nullptr) {
                m_hshpLabels.remove(strLabelNameOld);
                m_hshpLabels[strLabelNameNew] = pGraphObjLabel;
                pGraphObjLabel->setKey(strLabelNameNew);
                pGraphObjLabel->setText(strLabelNameNew);
                pGraphObjLabel->setSelectionPoint1(labelDscr.m_selPt1);
                pGraphObjLabel->setSelectionPoint2(labelDscr.m_selPt2);
            }
        }
        // Update the position of the indicated name and user defined labels.
        // But only if previously a polygon was set ..
        if (iCountPrev > 1) {
            for (const QString& strLabelName : m_hshLabelDscrs.keys()) {
                if (!isPolygonPointLabelName(strLabelName)) {
                    CGraphObjLabel* pGraphObjLabel = m_hshpLabels.value(strLabelName, nullptr);
                    SLabelDscr& labelDscr = m_hshLabelDscrs[strLabelName];
                    if ((labelDscr.m_selPt1.m_selPtType == ESelectionPointType::PolygonPoint)
                        || (labelDscr.m_selPt1.m_selPtType == ESelectionPointType::LineCenterPoint))
                    {
                        if (labelDscr.m_selPt1.m_idxPt >= m_idxsRemoved.first) {
                            labelDscr.m_selPt1.m_idxPt -= m_idxsRemoved.second;
                        }
                    }
                    if (pGraphObjLabel != nullptr) {
                        pGraphObjLabel->setSelectionPoint1(labelDscr.m_selPt1);
                    }
                    if ((labelDscr.m_selPt2.m_selPtType == ESelectionPointType::PolygonPoint)
                        || (labelDscr.m_selPt2.m_selPtType == ESelectionPointType::LineCenterPoint))
                    {
                        if (labelDscr.m_selPt2.m_idxPt >= m_idxsRemoved.first) {
                            labelDscr.m_selPt2.m_idxPt -= m_idxsRemoved.second;
                        }
                    }
                    if (pGraphObjLabel != nullptr) {
                        pGraphObjLabel->setSelectionPoint2(labelDscr.m_selPt2);
                    }
                }
            }
        }
        // Update geometry labels.
        iCountPrev = m_idxsRemoved.first + m_idxsRemoved.second;
        for (int idxPt = m_idxsRemoved.first; idxPt < iCountPrev; ++idxPt) {
            const QString strLabelName = createPolygonPointLabelName(idxPt);
            m_strlstGeometryLabelNames.removeOne(strLabelName);
            removeGeometryLabel(strLabelName);
        }
        iCountNew = getNumberOfPolygonPointGeometryLabelNames();
        // Update the indicated point labels. E.g. if removing the first point, previously
        // the label P2 pointed to index 1. After removing the point at index 0, the label
        // P2 must be renamed to P1 pointing to index 0.
        for (int idxPt = m_idxsRemoved.first; idxPt < iCountNew; ++idxPt) {
            const QString strLabelNameNew = createPolygonPointLabelName(idxPt);
            const QString strLabelNameOld = createPolygonPointLabelName(idxPt + m_idxsRemoved.second);
            SLabelDscr labelDscr = m_hshGeometryLabelDscrs[strLabelNameOld];
            labelDscr.m_strKey = strLabelNameNew;
            labelDscr.m_strText = strLabelNameNew;
            labelDscr.m_selPt1.m_idxPt = idxPt;
            m_hshGeometryLabelDscrs[strLabelNameNew] = labelDscr;
            m_hshGeometryLabelDscrs.remove(strLabelNameOld);
            int idxLabelName = m_strlstGeometryLabelNames.indexOf(strLabelNameOld);
            m_strlstGeometryLabelNames[idxLabelName] = strLabelNameNew;
            CGraphObjLabel* pGraphObjLabel = m_hshpGeometryLabels.value(strLabelNameOld, nullptr);
            if (pGraphObjLabel != nullptr) {
                m_hshpGeometryLabels.remove(strLabelNameOld);
                m_hshpGeometryLabels[strLabelNameNew] = pGraphObjLabel;
                pGraphObjLabel->setKey(strLabelNameNew);
                pGraphObjLabel->setText(strLabelNameNew);
                pGraphObjLabel->setSelectionPoint1(labelDscr.m_selPt1);
                pGraphObjLabel->setSelectionPoint2(labelDscr.m_selPt2);
            }
        }
        m_idxsRemoved = qMakePair(-1, 0);
    }
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Set the original, untransformed (not scaled, not rotated) polygon
           coordinates in local coordinates relative to the origin of the
           polygon's bounding rectangle

    @param [in] i_polygon
        Coordinates in local coordinates to be set.

    @return Previous original coordinates.
*/
QPolygonF CGraphObjPolygon::setPolygonOrig(const QPolygonF& i_polygon)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New [" + QString::number(i_polygon.size()) + "](" + qPolygon2Str(i_polygon) + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPolygonOrig",
        /* strAddInfo   */ strMthInArgs );

    QPolygonF polygonPrev = m_polygonOrig;
    m_polygonOrig = i_polygon;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev [" + QString::number(polygonPrev.size()) + "](" + qPolygon2Str(polygonPrev) + ")");
    }
    return polygonPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated coordinates in local coordinates
           relative to the origin of the item's bounding rectangle.

    @param [in] i_polygon
        Coordinates to be set.

    @return Previous coordinates.
*/
QPolygonF CGraphObjPolygon::QGraphicsPolygonItem_setPolygon(const QPolygonF& i_polygon)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New [" + QString::number(i_polygon.size()) + "](" + qPolygon2Str(i_polygon) + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsPolygonItem_setPolygon",
        /* strAddInfo   */ strMthInArgs );

    QPolygonF polygonPrev = QGraphicsPolygonItem::polygon();
    QGraphicsPolygonItem::setPolygon(i_polygon);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev [" + QString::number(polygonPrev.size()) + "](" + qPolygon2Str(polygonPrev) + ")");
    }
    return polygonPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the original, untransformed (not scaled, not rotated) line
           coordinates with unit in parent coordinates relative to the top left
           or bottom left corner of the parent.

    @param [in] i_physValPolygon
        Coordinates to be set.

    @return Previous original line coordinates.
*/
CPhysValPolygon CGraphObjPolygon::setPhysValPolygonOrig(const CPhysValPolygon& i_physValPolygon)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New [" + QString::number(i_physValPolygon.count()) + "](" + i_physValPolygon.toString() + ") " + i_physValPolygon.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValPolygonOrig",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolygon physValPolygonPrev = m_physValPolygonOrig;
    m_physValPolygonOrig = i_physValPolygon;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev [" + QString::number(physValPolygonPrev.count()) + "](" + physValPolygonPrev.toString() + ") " + physValPolygonPrev.unit().symbol());
    }
    return physValPolygonPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated line coordinates with unit in
           parent coordinates relative to the top left or bottom left corner of
           the parent.

    @param [in] i_physValPolygon
        Line coordinates relative to the parent (or scene) to be set.

    @return Previous line coordinates.
*/
CPhysValPolygon CGraphObjPolygon::setPhysValPolygonScaled(const CPhysValPolygon& i_physValPolygon)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New [" + QString::number(i_physValPolygon.count()) + "](" + i_physValPolygon.toString() + ") " + i_physValPolygon.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValPolygonScaled",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolygon physValPolygonPrev = m_physValPolygonScaled;
    m_physValPolygonScaled = i_physValPolygon;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev [" + QString::number(physValPolygonPrev.count()) + "](" + physValPolygonPrev.toString() + ") " + physValPolygonPrev.unit().symbol());
    }
    return physValPolygonPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled and rotated line coordinates with unit in parent
           coordinates relative to the top left or bottom left corner of the parent.

    @param [in] i_physValPolygon
        Line coordinates relative to the parent (or scene) to be set.

    @return Previous line coordinates.
*/
CPhysValPolygon CGraphObjPolygon::setPhysValPolygonScaledAndRotated(const CPhysValPolygon& i_physValPolygon)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New [" + QString::number(i_physValPolygon.count()) + "](" + i_physValPolygon.toString() + ") " + i_physValPolygon.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValPolygonScaledAndRotated",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPolygon physValPolygonPrev = m_physValPolygonScaledAndRotated;
    m_physValPolygonScaledAndRotated = i_physValPolygon;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev [" + QString::number(physValPolygonPrev.count()) + "](" + physValPolygonPrev.toString() + ") " + physValPolygonPrev.unit().symbol());
    }
    return physValPolygonPrev;
}

//------------------------------------------------------------------------------
/*! @brief Internal method reimplementing the prepareGeometryChange method of
           graphics item to trace the method call.

    As the prepareGeometryChange method is a protected method of QGraphicsItem
    this method must be reimplemented by the derived classes.
*/
void CGraphObjPolygon::QGraphicsItem_prepareGeometryChange()
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
void CGraphObjPolygon::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItem != nullptr) {
            QPolygonF polygon = this->polygon();
            QString strRuntimeInfo;
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "PhysValPolygon Orig [" + QString::number(m_physValPolygonOrig.count()) + "](" + m_physValPolygonOrig.toString() + ") " + m_physValPolygonOrig.unit().symbol()
                + ", Scaled [" + QString::number(m_physValPolygonScaled.count()) + "](" + m_physValPolygonScaled.toString() + ") " + m_physValPolygonScaled.unit().symbol()
                + ", ScaledRotated [" + QString::number(m_physValPolygonScaledAndRotated.count()) + "](" + m_physValPolygonScaledAndRotated.toString() + ") " + m_physValPolygonScaledAndRotated.unit().symbol();
            i_mthTracer.trace(strRuntimeInfo);
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "ItemLine Orig [" + QString::number(m_polygonOrig.size()) + "](" + qPolygon2Str(m_polygonOrig) + ")"
                + ", Scaled [" + QString::number(polygon.size()) + "](" + qPolygon2Str(polygon) + ")";
            i_mthTracer.trace(strRuntimeInfo);
            CGraphObj::traceThisPositionInfo(i_mthTracer, i_mthDir, i_detailLevel);
        }
    }
}
