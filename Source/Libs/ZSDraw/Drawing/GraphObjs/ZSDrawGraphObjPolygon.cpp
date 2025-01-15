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
#include <QtGui/QMenu>
#include <QtGui/QStyleOption>
#else
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtWidgets/QMenu>
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
        /* strObjName          */ i_strObjName.isEmpty() ? graphObjType2Str(EGraphObjTypePolygon) + QString::number(s_iInstCount) : i_strObjName),
    QGraphicsPolygonItem(),
    m_polygonOrig(),
    m_physValPolygonOrig(*m_pDrawingScene),
    m_physValPolygonScaledAndRotated(*m_pDrawingScene),
    m_plgLineStartArrowHead(),
    m_plgLineEndArrowHead(),
    m_idxsAdded(),
    m_idxsRemoved()
{
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    // For polygons the object creation is not finished if the mouse is released but instead a
    // mouse double click is used to add the last polygon point which finishes creation of the object.
    m_bMouseReleaseEventFinishesObjectCreation = false;

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
void CGraphObjPolygon::showContextMenu(const QPointF& i_ptScreenPos)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pos {" + qPoint2Str(i_ptScreenPos) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::showContextMenu",
        /* strAddInfo   */ strMthInArgs );

    if (m_pMenuContext == nullptr) {
        createContextMenu();
    }
    if (m_editMode == EEditMode::None) {
        m_pActionMenuContextModifyPoints->setEnabled(true);
        m_pActionMenuContextModifyPoints->setVisible(true);
        m_pActionMenuContextDeletePoint->setEnabled(false);
        m_pActionMenuContextDeletePoint->setVisible(false);
    }
    else if (m_editMode == EEditMode::ModifyingPolygonPoints) {
        m_pActionMenuContextModifyPoints->setEnabled(false);
        m_pActionMenuContextModifyPoints->setVisible(false);
        m_pActionMenuContextDeletePoint->setEnabled(true);
        m_pActionMenuContextDeletePoint->setVisible(true);
    }
    m_pMenuContext->setTitle(path());
    m_pMenuContext->popup(i_ptScreenPos.toPoint());
}

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to create a user defined dialog.
*/
void CGraphObjPolygon::openFormatGraphObjsDialog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "openFormatGraphObjsDialog",
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
            // When emitting the geometryOnSceneChanged signal the position of the linked objects
            // is updated. To position the selection points and labels correctly the local coordinate
            // system must be up-to-date.
            // Also note that itemChange is called if the position is changed but itemChange must not
            // overwrite the current values (refCountGuard).
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

        // Points may have been added or removed.
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            showSelectionPoints(c_uSelectionPointsPolygonPoints);
            //if (m_editMode == EEditMode::CreatingByMouseEvents) {
            //    showSelectionPoints(c_uSelectionPointsPolygonPoints);
            //}
            //else {
            //    showSelectionPoints();
            //}
        }
        if (m_idxsAdded.second > 0) {
            updateLabelsOnPolygonPointsAdded();
        }
        else if (m_idxsRemoved.second > 0) {
            updateLabelsOnPolygonPointsRemoved();
        }
        m_idxsAdded = qMakePair(-1, 0);
        m_idxsRemoved = qMakePair(-1, 0);
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
/*! @brief Returns the point at the given index of the polygon as a copy relative
           to the top left corner of the parent group or scene, if the object has
           no parent group.

    @note An operator or method returning a modifiable reference is not provided
          as on modifying the point, some further calculations as updating the
          bounding rectangle must be triggered.

    @param [in] i_idx
        The index must be a valid index position in the range 0 <= i_idx < count().
*/
CPhysValPoint CGraphObjPolygon::at(int i_idx) const
//------------------------------------------------------------------------------
{
    return m_physValPolygonScaledAndRotated.at(i_idx);
}

//------------------------------------------------------------------------------
/*! @brief Returns the first point of the polygon as a copy relative to the top
           left corner of the parent group or scene, if the object has no parent group.

    @note An operator or method returning a modifiable reference is not provided
          as on modifying the point, some further calculations as updating the
          bounding rectangle must be triggered.
*/
CPhysValPoint CGraphObjPolygon::first() const
//------------------------------------------------------------------------------
{
    return m_physValPolygonScaledAndRotated.first();
}

//------------------------------------------------------------------------------
/*! @brief Returns the last point of the polygon as a copy relative to the top
           left corner of the parent group or scene, if the object has no parent group.

    @note An operator or method returning a modifiable reference is not provided
          as on modifying the point, some further calculations as updating the
          bounding rectangle must be triggered.
*/
CPhysValPoint CGraphObjPolygon::last() const
//------------------------------------------------------------------------------
{
    return m_physValPolygonScaledAndRotated.last();
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
//                    o_pHitInfo->m_ptHit = i_pt;
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

    QCursor cursor = Qt::SizeAllCursor;
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        CGraphObjSelectionPoint* pGraphObjSelPtHit = getSelectionPointHit(i_pt);
        if (pGraphObjSelPtHit != nullptr) {
            cursor = pGraphObjSelPtHit->getProposedCursor(i_pt);
        }
        else {
            SGraphObjHitInfo hitInfo;
            if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                mthTracer.trace("-+ isPolygonHit(" + qPoint2Str(i_pt) + ")");
            }
            bool bIsPolygonHit = isPolygonHit(polygon(), m_drawSettings.getFillStyle(), i_pt, m_pDrawingScene->getHitToleranceInPx(), &hitInfo);
            if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                mthTracer.trace("+- isPolygonHit(HitInfo {" + hitInfo.toString() + "}): " + bool2Str(bIsPolygonHit));
            }
            if (bIsPolygonHit) {
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
    //mthTracer.trace("QGraphicsPolygonItem::boundingRect(): " + qRect2Str(rctBounding));
    for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsPolygon) {
        if (pGraphObjSelPt != nullptr) {
            QRectF rctSelPt = pGraphObjSelPt->boundingRect();
            QPolygonF plgSelPt = mapFromItem(pGraphObjSelPt, rctSelPt);
            QRectF rctBoundingSelPt = plgSelPt.boundingRect();
            //mthTracer.trace(pGraphObjSelPt->path() + ".boundingRect(): " + qRect2Str(rctBoundingSelPt));
            rctBounding |= rctBoundingSelPt;
            //mthTracer.trace("rctBounding: " + qRect2Str(rctBounding));
        }
    }
    for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsBoundingRect) {
        if (pGraphObjSelPt != nullptr) {
            QRectF rctSelPt = pGraphObjSelPt->boundingRect();
            QPolygonF plgSelPt = mapFromItem(pGraphObjSelPt, rctSelPt);
            QRectF rctBoundingSelPt = plgSelPt.boundingRect();
            //mthTracer.trace(pGraphObjSelPt->path() + ".boundingRect(): " + qRect2Str(rctBoundingSelPt));
            rctBounding |= rctBoundingSelPt;
            //mthTracer.trace("rctBounding: " + qRect2Str(rctBounding));
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
/*! @brief Returns the shape of this item as a QPainterPath in local coordinates.
*/
QPainterPath CGraphObjPolygon::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );

    // QGraphicsPolygonItem::shape() throws an exception if any segment has a very small
    // length (< 0.5 pixels). So we do this on our own ...
    QPainterPath painterPath;
    QPolygonF polygon = this->polygon();
    if (polygon.size() > 0) {
        painterPath.moveTo(polygon.at(0));
        for (int idxPt = 0; idxPt < polygon.size()-1; ++idxPt) {
            const QPointF& pt1 = polygon.at(idxPt);
            const QPointF& pt2 = polygon.at(idxPt+1);
            if (QLineF(pt1, pt2).length() > 1.0) {
                painterPath.lineTo(pt2);
            }
        }
        if (m_type == EGraphObjTypePolygon) {
            const QPointF& pt1 = polygon.last();
            const QPointF& pt2 = polygon.first();
            if (QLineF(pt1, pt2).length() > 1.0) {
                painterPath.lineTo(pt2);
            }
        }
    }
    if (!m_plgLineStartArrowHead.isEmpty()) {
        painterPath.moveTo(mapToScene(m_plgLineStartArrowHead.at(0)));
        painterPath.addPolygon(m_plgLineStartArrowHead);
    }
    if (!m_plgLineEndArrowHead.isEmpty()) {
        painterPath.moveTo(mapToScene(m_plgLineEndArrowHead.at(0)));
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
    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHighlighted || isSelected())) {
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
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }
    if (m_pDrawingScene->getCurrentDrawingTool() == nullptr) {
        if (m_editMode == EEditMode::None) {
            QGraphicsItem_setCursor(getProposedCursor(i_pEv->pos()));
        }
        else if (m_editMode == EEditMode::CreatingByMouseEvents) {
            // Ignore hover events if the object is currently being created.
        }
        else if (m_editMode == EEditMode::ModifyingPolygonPoints) {
            QGraphicsItem_setCursor(getProposedCursor(i_pEv->pos()));
        }
    }
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
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
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }
    if (m_pDrawingScene->getCurrentDrawingTool() == nullptr) {
        if (m_editMode == EEditMode::None) {
            QGraphicsItem_setCursor(getProposedCursor(i_pEv->pos()));
        }
        else if (m_editMode == EEditMode::CreatingByMouseEvents) {
            // Ignore hover events if the object is currently being created.
        }
        else if (m_editMode == EEditMode::ModifyingPolygonPoints) {
            QGraphicsItem_setCursor(getProposedCursor(i_pEv->pos()));
            //if (hoverMoveEventAfterMouseRelease(i_pEv)) {
            //    QGraphicsItem* pGraphicsItemMouseGrabber = m_pDrawingScene->mouseGrabberItem();
            //    if (pGraphicsItemMouseGrabber == nullptr) {
            //        grabMouse();
            //    }
            //}
        }
    }
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
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
        strMthInArgs = "Ev {" + qGraphicsSceneHoverEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }
    if (m_pDrawingScene->getCurrentDrawingTool() == nullptr) {
        if (m_editMode == EEditMode::None) {
        }
        else if (m_editMode == EEditMode::CreatingByMouseEvents) {
            // Ignore hover events if the object is currently being created.
        }
        else if (m_editMode == EEditMode::ModifyingPolygonPoints) {
            SGraphObjHitInfo hitInfo;
            if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                mthTracer.trace("-+ isPolygonHit([" + QString::number(polygon().size()) + "], .., Pos {" + qPoint2Str(i_pEv->pos()) + ")");
            }
            bool bIsPolygonHit = isPolygonHit(polygon(), m_drawSettings.getFillStyle(), i_pEv->pos(), m_pDrawingScene->getHitToleranceInPx(), &hitInfo);
            if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                mthTracer.trace("+- isPolygonHit(HitInfo {" + hitInfo.toString() + "}): " + bool2Str(bIsPolygonHit));
            }
            if (bIsPolygonHit) {
                QGraphicsItem_setCursor(hitInfo.m_cursor);
            }
            else {
                QGraphicsItem_unsetCursor();
            }
        }
    }
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
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
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }
    bool bEventHandled = false;
    if (m_editMode == EEditMode::None) {
        if (i_pEv->button() == Qt::LeftButton) {
            if (i_pEv->modifiers() & Qt::ControlModifier) {
                setEditMode(EEditMode::ModifyingPolygonPoints);
                bEventHandled = true;
            }
        }
        else if (i_pEv->button() == Qt::RightButton) {
            showContextMenu(i_pEv->screenPos());
            bEventHandled = true;
        }
    }
    else if (m_editMode == EEditMode::CreatingByMouseEvents) {
        // The first mouse press right after creating the object will be ignored.
        bool bIgnoreMouseEvent = false;
        CPhysValPoint physValPointLast = m_physValPolygonScaledAndRotated.last();
        if (m_physValPolygonScaledAndRotated.count() == 2) {
            CPhysValPoint physValPointFirst = m_physValPolygonScaledAndRotated.first();
            if (physValPointFirst == physValPointLast) {
                bIgnoreMouseEvent = true;
            }
        }
        if (!bIgnoreMouseEvent) {
            // The mouse grabber item got to be changed.
            //QGraphicsItem* pGraphicsItemMouseGrabber = m_pDrawingScene->mouseGrabberItem();
            //CGraphObjSelectionPoint* pGraphObjSelPtMouseGrabber = dynamic_cast<CGraphObjSelectionPoint*>(pGraphicsItemMouseGrabber);
            //bool bAdjustMouseGrabber = pGraphObjSelPtMouseGrabber != nullptr && pGraphObjSelPtMouseGrabber == m_arpSelPtsPolygon.last();
            //// The previously "last" selection point should no longer be the mouse grabber.
            //if (bAdjustMouseGrabber) {
            //    pGraphObjSelPtMouseGrabber->ungrabMouse();
            //}
            append(physValPointLast);
            //// The newly appended selection point should become the new grabber.
            //if (bAdjustMouseGrabber) {
            //    pGraphObjSelPtMouseGrabber = m_arpSelPtsPolygon.last();
            //    if (pGraphObjSelPtMouseGrabber != nullptr) {
            //        pGraphObjSelPtMouseGrabber->grabMouse();
            //    }
            //}
        }
        bEventHandled = true;
    }
    else if (m_editMode == EEditMode::ModifyingPolygonPoints) {
        if (i_pEv->modifiers() == Qt::NoModifier) {
            setEditMode(EEditMode::None);
            // Forward the mouse event to the base implementation.
            // This will select the item, creating selection points if not yet created.
            QGraphicsPolygonItem::mousePressEvent(i_pEv);
        }
        else if (i_pEv->modifiers() & Qt::ControlModifier) {
            // Check if any line segment has been hit.
            QPolygonF polygon = this->polygon();
            QPointF ptPos = i_pEv->pos();
            for (int idxPt = 0; idxPt < polygon.size(); ++idxPt) {
                QLineF line(polygon[idxPt], (idxPt+1) < polygon.size() ? polygon[idxPt+1] : polygon[0]);
                SGraphObjHitInfo hitInfo;
                if (isLineHit(line, ptPos, m_pDrawingScene->getHitToleranceInPx(), &hitInfo)) {
                    insert(idxPt+1, CPhysValPoint(*m_pDrawingScene, mapToParent(hitInfo.m_ptHit), Units.Length.px));
                    //// The newly added selection point will become the new grabber
                    //// so that newly created polygon point can be moved.
                    //QGraphicsItem* pGraphicsItemMouseGrabber = m_pDrawingScene->mouseGrabberItem();
                    //if (pGraphicsItemMouseGrabber != nullptr) {
                    //    pGraphicsItemMouseGrabber->ungrabMouse();
                    //}
                    //CGraphObjSelectionPoint* pGraphObjSelPtMouseGrabber = m_arpSelPtsPolygon[idxPt+1];
                    //if (pGraphObjSelPtMouseGrabber != nullptr) {
                    //    pGraphObjSelPtMouseGrabber->grabMouse();
                    //}
                    break;
                }
            }
        }
        bEventHandled = true;
    }
    if (bEventHandled) {
        i_pEv->accept();
    }
    else {
        // Forward the mouse event to the base implementation.
        // This will select the item, creating selection points if not yet created.
        QGraphicsPolygonItem::mousePressEvent(i_pEv);
    }
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
/*! @brief

    @note After dispatching the mouse release event to the mouse grabber item, the
          graphics scene will reset the mouse grabber. So it is useless trying to
          keep the mouse grabber within the item's mouseReleaseEvent method.
*/
void CGraphObjPolygon::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
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
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    bool bEventHandled = false;
    if (m_editMode == EEditMode::None) {
    }
    else if (m_editMode == EEditMode::CreatingByMouseEvents) {
        // The mouse grabber item got to be kept.
        // The newly appended selection point should remain the new grabber.
        m_pDrawingScene->setMouseGrabber(m_arpSelPtsPolygon.last());
        //QGraphicsItem* pGraphicsItemMouseGrabber = m_pDrawingScene->mouseGrabberItem();
        //CGraphObjSelectionPoint* pGraphObjSelPtMouseGrabber = dynamic_cast<CGraphObjSelectionPoint*>(pGraphicsItemMouseGrabber);
        //bool bAdjustMouseGrabber = pGraphObjSelPtMouseGrabber == nullptr || pGraphObjSelPtMouseGrabber != m_arpSelPtsPolygon.last();
        //if (bAdjustMouseGrabber) {
        //    pGraphObjSelPtMouseGrabber->ungrabMouse();
        //    pGraphObjSelPtMouseGrabber = m_arpSelPtsPolygon.last();
        //    if (pGraphObjSelPtMouseGrabber != nullptr) {
        //        pGraphObjSelPtMouseGrabber->grabMouse();
        //    }
        //}
    }
    else if (m_editMode == EEditMode::ModifyingPolygonPoints) {
        // If a polygon point has been newly created, the corresponding selection point
        // has been temporarily set as the mouse grabber to move the polygon point.
        // Creating the additional polygon point is finished and the selection point
        // must no longer be the mouse grabber.
        //QGraphicsItem* pGraphicsItemMouseGrabber = m_pDrawingScene->mouseGrabberItem();
        //CGraphObjSelectionPoint* pGraphObjSelPtMouseGrabber = dynamic_cast<CGraphObjSelectionPoint*>(pGraphicsItemMouseGrabber);
        //if (pGraphObjSelPtMouseGrabber != nullptr) {
        //    pGraphObjSelPtMouseGrabber->ungrabMouse();
        //}
    }
    if (bEventHandled) {
        i_pEv->accept();
    }
    else {
        // Forward the mouse event to the base implementation.
        QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
    }

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
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
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    if (m_editMode == EEditMode::CreatingByMouseEvents) {
        // The mouse grabber item got to be removed.
        QGraphicsItem* pGraphicsItemMouseGrabber = m_pDrawingScene->mouseGrabberItem();
        CGraphObjSelectionPoint* pGraphObjSelPtMouseGrabber = dynamic_cast<CGraphObjSelectionPoint*>(pGraphicsItemMouseGrabber);
        bool bAdjustMouseGrabber = pGraphObjSelPtMouseGrabber != nullptr && pGraphObjSelPtMouseGrabber == m_arpSelPtsPolygon.last();
        if (bAdjustMouseGrabber) {
            pGraphObjSelPtMouseGrabber->ungrabMouse();
        }
        // While creating the object, the selection points at the polygon points were visible.
        // Unselect the object to hide (destroy) those selection points.
        // Note: This mouse event method may have been called by a selection points mouse event method.
        //       The selection point calling the method may be destroyed now. This means that after
        //       invoking this mouse event method the selection point MUST NOT access any instance
        //       members anymore.
        setSelected(false);
        // The editMode changed signal will be emitted and received by the drawing scene.
        // The drawing scene is informed this way that creation of the object is finished
        // and will unselect the current drawing tool and will select the object under
        // construction showing the selection points at the bounding rectangle.
        setEditMode(EEditMode::None);
    }

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjPolygon::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Ev {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
    }

    // Forward the mouse event to the base implementation.
    QGraphicsPolygonItem::mouseMoveEvent(i_pEv);

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
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
    if (m_iItemChangeBlockedCounter > 0) {
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
            if (m_editMode == EEditMode::CreatingByMouseEvents) {
                hideSelectionPoints(c_uSelectionPointsBoundingRectAll);
                showSelectionPoints(c_uSelectionPointsPolygonPoints);
            }
            else if (m_editMode == EEditMode::ModifyingPolygonPoints) {
                hideSelectionPoints(c_uSelectionPointsBoundingRectAll);
                showSelectionPoints(c_uSelectionPointsPolygonPoints);
            }
            else {
                hideSelectionPoints(c_uSelectionPointsPolygonPoints);
                showSelectionPoints(c_uSelectionPointsBoundingRectAll);
            }
            // Not necessary as item has been brought to front and "showSelectionPoints"
            // sets zValue of selection points above item.
            //bringSelectionPointsToFront();
        }
        else {
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

    // Moving a selection point will modify the shape of the object and the position
    // of all other selection points got to be updated. If the position of the other
    // selection points will be changed, those selection points are emitting the
    // geometryOnSceneChanged signal whereupon this slot method would be called again
    // for each other selection point. This will not end up in an endless loop but
    // is useless and anything else but performant. So the slot will be temporarily
    // disconnected from the geometryOnSceneChanged signal of the selection points.
    disconnectGeometryOnSceneChangedSlotFromSelectionPoints();

    SGraphObjSelectionPoint selPt = pGraphObjSelPt->getSelectionPoint();
    if (selPt.m_selPtType == ESelectionPointType::PolygonPoint) {
        if (selPt.m_idxPt >= 0 && selPt.m_idxPt < polygon().size()) {
            replace(selPt.m_idxPt, physValPointParentSelPt);
        }
    }
    else if (selPt.m_selPtType == ESelectionPointType::BoundingRectangle) {
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
    }
    connectGeometryOnSceneChangedSlotWithSelectionPoints();
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
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir,
    const QString& i_strFilter, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }
    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        QPolygonF polygon = this->polygon();
        QString strRuntimeInfo;
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "PhysValPolygon Orig {" + m_physValPolygonOrig.toString() + "} " + m_physValPolygonOrig.unit().symbol()
            + ", ScaledRotated {" + m_physValPolygonScaledAndRotated.toString() + "} " + m_physValPolygonScaledAndRotated.unit().symbol();
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "ItemPolygon Orig {" + qPolygon2Str(m_polygonOrig) + "}, Scaled {" + qPolygon2Str(polygon) + "}";
        i_mthTracer.trace(strRuntimeInfo);
        CGraphObj::traceThisPositionInfo(i_mthTracer, i_mthDir, i_strFilter, i_detailLevel);
    }
}
