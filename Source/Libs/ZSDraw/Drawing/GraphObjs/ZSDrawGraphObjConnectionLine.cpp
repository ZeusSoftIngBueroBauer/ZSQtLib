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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjConnectionLinePropertiesDlg.h"
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
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidgetAction>

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
class CGraphObjConnectionLine : public CGraphObj, public QGraphicsPolygonItem
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

qint64 CGraphObjConnectionLine::s_iInstCount = 0;

/*==============================================================================
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjConnectionLine::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjConnectionLine::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjConnectionLine::createPolygonPointLabelName(int i_idxPt)
//------------------------------------------------------------------------------
{
    return c_strGeometryLabelNameP + QString::number(i_idxPt+1);
}

//------------------------------------------------------------------------------
bool CGraphObjConnectionLine::isPolygonPointLabelName(const QString& i_strLabelName)
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
int CGraphObjConnectionLine::extractIndexFromPolygonPointLabelName(const QString& i_strLabelName)
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
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjConnectionLine::CGraphObjConnectionLine(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameConnections,
        /* type                */ EGraphObjTypeConnectionLine,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeConnectionLine),
        /* strObjName          */ i_strObjName.isEmpty() ? graphObjType2Str(EGraphObjTypeConnectionLine) + QString::number(s_iInstCount) : i_strObjName),
    QGraphicsPolygonItem(),
    m_physValPolygonOrig(*m_pDrawingScene),
    m_physValPolygonScaledAndRotated(*m_pDrawingScene)
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

    // The predefined label names (Name, P1, .., PN) are added (or removed)
    // on adding (or removing) points to the polygon.

    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameCenter);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameWidth);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameHeight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameAngle);

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
    QGraphicsItem_setAcceptHoverEvents(true);

    setStackingOrderValue(c_fStackingOrderOffsetConnectionLines, ERowVersion::Original);
}

//------------------------------------------------------------------------------
CGraphObjConnectionLine::~CGraphObjConnectionLine()
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

    for (ELinePoint linePoint : m_arpCnctPts.keys()) {
        CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts.value(linePoint);
        if (pGraphObjCnctPt != nullptr) {
            pGraphObjCnctPt->removeConnectionLine(this);
        }
        m_arpCnctPts.remove(linePoint);
    }
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjConnectionLine::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeConnectionLine;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjConnectionLine::clone()
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

    Must be overridden to show the context popup menu.
    Usually opened by right clicking on the the object.
*/
void CGraphObjConnectionLine::createContextMenu()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::createContextMenu",
        /* strAddInfo   */ "" );
    if (m_pMenuContext == nullptr) {
        m_pMenuContext = new QMenu();
        // The title only works for sub menus.
        // A widget action must be used to set a title.
        //m_pMenuContext = new QMenu(path());
        //m_pMenuContext->setTitle(path());

        QLabel* pLblTitle = new QLabel(path());
        QFont fntTitle = pLblTitle->font();
        fntTitle.setBold(true);
        pLblTitle->setFont(fntTitle);
        pLblTitle->setContentsMargins(2, 5, 2, 5);
        QWidgetAction* pWdgtAction = new QWidgetAction(this);
        pWdgtAction->setDefaultWidget(pLblTitle);
        m_pMenuContext->addAction(pWdgtAction);
        m_pMenuContext->addSeparator();

        //QIcon iconFormat;
        QPixmap pxmFormat(":/ZS/Draw/FormatGraphObj16x16.png");
        pxmFormat.setMask(pxmFormat.createHeuristicMask());
        //iconFormat.addPixmap(pxmFormat);
        m_pActionMenuContextFormat = new QAction(pxmFormat, "Format", this);
        m_pMenuContext->addAction(m_pActionMenuContextFormat);
        QObject::connect(
            m_pActionMenuContextFormat, &QAction::triggered,
            this, &CGraphObj::onActionFormatTriggered);
        m_pMenuContext->addSeparator();

        //QIcon iconFormatModifyPoints;
        QPixmap pxmFormatModifyPoints(":/ZS/Draw/FormatGraphObjModifyPoints16x16.png");
        pxmFormatModifyPoints.setMask(pxmFormatModifyPoints.createHeuristicMask());
        //iconFormat.addPixmap(pxmFormat);
        m_pActionMenuContextModifyPoints = new QAction(pxmFormatModifyPoints, "Modify Points", this);
        m_pMenuContext->addAction(m_pActionMenuContextModifyPoints);
        m_pMenuContext->addSeparator();
        QObject::connect(
            m_pActionMenuContextModifyPoints, &QAction::triggered,
            this, &CGraphObjConnectionLine::onActionModifyPointsTriggered);
        m_pActionMenuContextModifyPoints->setVisible(false);
        m_pActionMenuContextModifyPoints->setEnabled(false);

        //QIcon iconFormatDeletePoint;
        QPixmap pxmFormatDeletePoint(":/ZS/Draw/FormatGraphObjDeletePoint16x16.png");
        pxmFormatDeletePoint.setMask(pxmFormatDeletePoint.createHeuristicMask());
        //iconFormatDeletePoint.addPixmap(pxmFormat);
        m_pActionMenuContextDeletePoint = new QAction(pxmFormatDeletePoint, "Delete Point", this);
        m_pMenuContext->addAction(m_pActionMenuContextDeletePoint);
        QObject::connect(
            m_pActionMenuContextDeletePoint, &QAction::triggered,
            this, &CGraphObjConnectionLine::onActionDeletePointTriggered);
        m_pActionMenuContextDeletePoint->setVisible(false);
        m_pActionMenuContextDeletePoint->setEnabled(false);
    }
    m_pMenuContext->setTitle(path());
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::showContextMenu(QGraphicsSceneMouseEvent* i_pEv)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pos {" + qGraphicsSceneMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "showContextMenu",
        /* strAddInfo   */ strMthInArgs );

    if (m_pMenuContext == nullptr) {
        createContextMenu();
    }
    m_hitInfoOnShowContextMenu = SGraphObjHitInfo();
    if (m_editMode == EEditMode::CreatingByMouseEvents) {
        m_pActionMenuContextModifyPoints->setVisible(false);
        m_pActionMenuContextModifyPoints->setEnabled(false);
        m_pActionMenuContextDeletePoint->setVisible(false);
        m_pActionMenuContextDeletePoint->setEnabled(false);
    }
    else if (m_editMode == EEditMode::ModifyingPolygonPoints) {
        m_pActionMenuContextModifyPoints->setEnabled(false);
        m_pActionMenuContextModifyPoints->setVisible(false);
        // Check if any line segment has been hit.
        // As this method may have been called by the selection point, "pos" would return
        // the local coordinate of the selection point. We need to use the scene pos.
        QPointF ptEvLocalPos = mapFromScene(i_pEv->scenePos());
        SGraphObjHitInfo hitInfo;
        if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            mthTracer.trace("-+ isPolygonHit([" + QString::number(polygon().size()) + "], .., Pos {" + qPoint2Str(ptEvLocalPos) + ")");
        }
        bool bIsPolygonHit = isPolygonHit(polygon(), m_drawSettings.fillStyle(), ptEvLocalPos, m_pDrawingScene->getHitToleranceInPx(), &hitInfo);
        if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            mthTracer.trace("+- isPolygonHit(HitInfo {" + hitInfo.toString() + "}): " + bool2Str(bIsPolygonHit));
        }
        m_hitInfoOnShowContextMenu = hitInfo;
        if (hitInfo.isPolygonShapePointHit() && polygon().size() > 2) {
            m_pActionMenuContextDeletePoint->setVisible(true);
            m_pActionMenuContextDeletePoint->setEnabled(true);
        }
        else {
            m_pActionMenuContextDeletePoint->setVisible(true);
            m_pActionMenuContextDeletePoint->setEnabled(false);
        }
    }
    else {
        m_pActionMenuContextModifyPoints->setVisible(true);
        m_pActionMenuContextModifyPoints->setEnabled(true);
        m_pActionMenuContextDeletePoint->setVisible(false);
        m_pActionMenuContextDeletePoint->setEnabled(false);
    }
    m_pMenuContext->setTitle(path());
    m_pMenuContext->popup(i_pEv->screenPos());
}

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to create a user defined dialog.
*/
void CGraphObjConnectionLine::openFormatGraphObjsDialog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "openFormatGraphObjsDialog",
        /* strAddInfo   */ "" );

    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Format Connection Line";
    CDlgGraphObjConnectionLineProperties* pDlg = CDlgGraphObjConnectionLineProperties::GetInstance(this);
    if( pDlg == nullptr ) {
        pDlg = CDlgGraphObjConnectionLineProperties::CreateInstance(strDlgTitle, this);
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
void CGraphObjConnectionLine::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
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
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Connects the specified line point with the specified connection point
           by appending the line to the connection point.
*/
void CGraphObjConnectionLine::setConnectionPoint(ELinePoint i_linePoint, CGraphObjConnectionPoint* i_pGraphObjCnctPt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumLinePoint(i_linePoint).toString() +
            ", CnctPoint: " + QString(i_pGraphObjCnctPt == nullptr ? "nullptr" : i_pGraphObjCnctPt->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setConnectionPoint",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceConnectionPoints(mthTracer, EMethodDir::Enter);
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
    CRefCountGuard refCountGuardTrace(&m_iTraceBlockedCounter);

    if (i_pGraphObjCnctPt != nullptr && (i_linePoint == ELinePoint::Start || i_linePoint == ELinePoint::End)) {
        CGraphObj* pGraphObjCnctPt = i_pGraphObjCnctPt;
        QGraphicsEllipseItem* pGraphicsItemCnctPt = dynamic_cast<QGraphicsEllipseItem*>(i_pGraphObjCnctPt);
        CPhysValPoint physValPoint = i_pGraphObjCnctPt->getCenter();
        physValPoint = pGraphObjCnctPt->mapToScene(physValPoint);
        CGraphObjConnectionPoint* pGraphObjCnctPtPrev = m_arpCnctPts.value(i_linePoint, nullptr);
        if (pGraphObjCnctPtPrev != nullptr && pGraphObjCnctPtPrev != i_pGraphObjCnctPt ) {
            pGraphObjCnctPtPrev->removeConnectionLine(this);
            m_arpCnctPts.remove(i_linePoint);
            QObject::disconnect(
                i_pGraphObjCnctPt, &CGraphObj::geometryOnSceneChanged,
                this, &CGraphObjConnectionLine::onGraphObjConnectionPointGeometryOnSceneChanged);
            //QObject::disconnect(
            //    i_pGraphObjCnctPt, &CGraphObj::zValueChanged,
            //    this, &CGraphObjConnectionLine::onGraphObjConnectionPointZValueChanged);
        }
        if (m_arpCnctPts.value(i_linePoint, nullptr) != i_pGraphObjCnctPt) {
            m_arpCnctPts[i_linePoint] = i_pGraphObjCnctPt;
            i_pGraphObjCnctPt->appendConnectionLine(this);
            QObject::connect(
                i_pGraphObjCnctPt, &CGraphObj::geometryOnSceneChanged,
                this, &CGraphObjConnectionLine::onGraphObjConnectionPointGeometryOnSceneChanged);
            //QObject::connect(
            //    i_pGraphObjCnctPt, &CGraphObj::zValueChanged,
            //    this, &CGraphObjConnectionLine::onGraphObjConnectionPointZValueChanged);
        }
        if (i_linePoint == ELinePoint::Start) {
            if (count() == 0) {
                append(physValPoint);
            }
            if (m_editMode == EEditMode::CreatingByMouseEvents) {
                replace(0, physValPoint);
                if (count() == 1) {
                    append(physValPoint);
                }
            }
            else if (first() != physValPoint) {
                if (pGraphObjCnctPtPrev == nullptr) {
                    if (pGraphicsItemCnctPt->contains(first().toQPointF())) {
                        replace(0, physValPoint);
                    }
                    else {
                        insert(0, physValPoint);
                    }
                }
                else {
                    replace(0, physValPoint);
                }
            }
        }
        else if (i_linePoint == ELinePoint::End) {
            if (count() == 0) {
                append(physValPoint);
            }
            if (m_editMode == EEditMode::CreatingByMouseEvents) {
                replace(count()-1, physValPoint);
                if (count() == 1) {
                    append(physValPoint);
                }
            }
            else if (last() != physValPoint) {
                if (pGraphObjCnctPtPrev == nullptr) {
                    if (pGraphicsItemCnctPt->contains(last().toQPointF())) {
                        replace(count()-1, physValPoint);
                    }
                    else {
                        append(physValPoint);
                    }
                }
                else {
                    replace(count()-1, physValPoint);
                }
            }
        }
    }

    refCountGuardTrace.decrementAndReleaseCounter();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceConnectionPoints(mthTracer, EMethodDir::Leave);
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
        traceThisPositionInfo(mthTracer, EMethodDir::Leave);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the line point (start or end) which is connected to the specified connection point.
*/
ELinePoint CGraphObjConnectionLine::getConnectionLinePoint(CGraphObjConnectionPoint* i_pGraphObjCnctPt) const
//------------------------------------------------------------------------------
{
    ELinePoint linePoint = ELinePoint::None;
    for (ELinePoint linePointTmp : m_arpCnctPts.keys()) {
        CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts[linePointTmp];
        if (pGraphObjCnctPt != nullptr && pGraphObjCnctPt == i_pGraphObjCnctPt) {
            linePoint = linePointTmp;
            break;
        }
    }
    return linePoint;
}

//------------------------------------------------------------------------------
CGraphObjConnectionPoint* CGraphObjConnectionLine::getConnectionPoint(ELinePoint i_linePoint) const
//------------------------------------------------------------------------------
{
    return m_arpCnctPts.value(i_linePoint, nullptr);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::setPolygon(const CPhysValPolygon& i_physValPolygon)
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
    }

    bool bGeometryOnSceneChanged = false;
    if (m_physValPolygonScaledAndRotated != i_physValPolygon) {
        // Depending on the Y scale orientation of the drawing scene the polygon coordinates
        // have been passed either relative to the top left or bottom right corner of the
        // parent item's bounding rectangle.
        // The coordinates need to be transformed into the local coordinate system of the graphical
        // object whose origin point is the center of the objects bounding rectangle.
        QPointF ptPosPrev = pos();
        QPolygonF polygon;
        CPhysVal physValAngle;
        QPointF ptPos = getItemPosAndLocalCoors(i_physValPolygon, polygon, physValAngle);

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
            // If selected while creating the object or while modifying
            // (adding/removing/changing) polygon points ..
            if (m_editMode == EEditMode::CreatingByMouseEvents || m_editMode == EEditMode::ModifyingPolygonPoints) {
                showSelectionPoints(c_uSelectionPointsPolygonPoints);
            }
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer, EMethodDir::Leave);
    }
    // Emit signal after updated position info has been traced.
    if (bGeometryOnSceneChanged) {
        emit_geometryOnSceneChanged();
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::setPolygon(const QPolygonF& i_polygon, const ZS::PhysVal::CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPolygon2Str(i_polygon) + " " + i_unit.symbol() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPolygon",
        /* strAddInfo   */ strMthInArgs );

    setPolygon(CPhysValPolygon(*m_pDrawingScene, i_polygon, i_unit));
}

//------------------------------------------------------------------------------
CPhysValPolygon CGraphObjConnectionLine::getPolygon() const
//------------------------------------------------------------------------------
{
    return getPolygon(m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
CPhysValPolygon CGraphObjConnectionLine::getPolygon(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (parentGroup() != nullptr) {
        return parentGroup()->convert(m_physValPolygonScaledAndRotated, i_unit);
    }
    else {
        return m_pDrawingScene->convert(m_physValPolygonScaledAndRotated, i_unit);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjConnectionLine::isEmpty() const
//------------------------------------------------------------------------------
{
    return m_physValPolygonScaledAndRotated.isEmpty();
}

//------------------------------------------------------------------------------
int CGraphObjConnectionLine::count() const
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
CPhysValPoint CGraphObjConnectionLine::at(int i_idx) const
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
CPhysValPoint CGraphObjConnectionLine::first() const
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
CPhysValPoint CGraphObjConnectionLine::last() const
//------------------------------------------------------------------------------
{
    return m_physValPolygonScaledAndRotated.last();
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::replace(int i_idx, const CPhysValPoint& i_physValPoint)
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

    CPhysValPoint physValPoint = i_physValPoint;
    if (parentGroup() != nullptr) {
        physValPoint = parentGroup()->convert(physValPoint);
    }
    else {
        physValPoint = m_pDrawingScene->convert(physValPoint);
    }
    CPhysValPolygon physValPolygon = getPolygon();
    physValPolygon.replace(i_idx, physValPoint);
    setPolygon(physValPolygon);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::append(const CPhysValPoint& i_physValPoint)
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

    CPhysValPoint physValPoint = i_physValPoint;
    if (parentGroup() != nullptr) {
        physValPoint = parentGroup()->convert(physValPoint);
    }
    else {
        physValPoint = m_pDrawingScene->convert(physValPoint);
    }
    CPhysValPolygon physValPolygon = getPolygon();
    m_idxsAdded = qMakePair(physValPolygon.count(), 1);
    physValPolygon.append(physValPoint);
    setPolygon(physValPolygon);
    m_idxsAdded = qMakePair(-1, 0);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::insert(int i_idx, const CPhysValPoint& i_physValPoint)
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

    CPhysValPoint physValPoint = i_physValPoint;
    if (parentGroup() != nullptr) {
        physValPoint = parentGroup()->convert(physValPoint);
    }
    else {
        physValPoint = m_pDrawingScene->convert(physValPoint);
    }
    CPhysValPolygon physValPolygon = getPolygon();
    m_idxsAdded = qMakePair(i_idx, 1);
    physValPolygon.insert(i_idx, physValPoint);
    setPolygon(physValPolygon);
    m_idxsAdded = qMakePair(-1, 0);
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::remove(int i_idx, int i_iCount)
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
void CGraphObjConnectionLine::removeAt(int i_idx)
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
void CGraphObjConnectionLine::removeFirst()
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
void CGraphObjConnectionLine::removeLast()
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
CPhysValPoint CGraphObjConnectionLine::takeAt(int i_idx)
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
CPhysValPoint CGraphObjConnectionLine::takeFirst()
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
CPhysValPoint CGraphObjConnectionLine::takeLast()
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
QRectF CGraphObjConnectionLine::getBoundingRect() const
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
/*! @brief Returns the rotated, physical bounding rectangle.
*/
CPhysValRect CGraphObjConnectionLine::getPhysValBoundingRect(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal)) {
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
QCursor CGraphObjConnectionLine::getProposedCursor(const QPointF& i_pt) const
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
            if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                mthTracer.trace("-+ isPolygonHit(" + qPoint2Str(i_pt) + ")");
            }
            bool bIsPolygonHit = isPolygonHit(polygon(), m_drawSettings.fillStyle(), i_pt, m_pDrawingScene->getHitToleranceInPx(), &hitInfo);
            if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
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
CPhysValPoint CGraphObjConnectionLine::getPositionOfSelectionPoint(
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
QPointF CGraphObjConnectionLine::getPositionOfSelectionPointInSceneCoors(
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
SPolarCoors CGraphObjConnectionLine::getPolarCoorsToSelectionPointFromSceneCoors(
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
QLineF CGraphObjConnectionLine::getAnchorLineToSelectionPointFromPolarInSceneCoors(
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
void CGraphObjConnectionLine::showSelectionPoints(TSelectionPointTypes i_selPts)
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
protected: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplement the base implementation of CGraphObj as for connection lines
           for the first and last polygon points no selection points should be created.
           The first and last polygon points are connection points.
*/
void CGraphObjConnectionLine::showSelectionPointsOfPolygon(const QPolygonF& i_plg)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Polygon {" + qPolygon2Str(i_plg) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "showSelectionPointsOfPolygon",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr && pGraphicsItem->parentItem() == nullptr) {
        //QGraphicsItem_prepareGeometryChange(); // as the boundingRect is changed
        if (m_arpSelPtsPolygon.size() > i_plg.size()) {
            for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= i_plg.size(); --idxSelPt) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                m_arpSelPtsPolygon[idxSelPt] = nullptr;
                // The dtor of the selection point (dtor of CGraphObj) removes itself from the drawing scene.
                //m_pDrawingScene->removeGraphObj(pGraphObjSelPt);
                delete pGraphObjSelPt;
                pGraphObjSelPt = nullptr;
            }
            for (int idxSelPt = m_arpSelPtsPolygon.size()-1; idxSelPt >= i_plg.size(); --idxSelPt) {
                m_arpSelPtsPolygon.removeAt(idxSelPt);
            }
        }
        else if (m_arpSelPtsPolygon.size() < i_plg.size()) {
            for (int idxSelPt = m_arpSelPtsPolygon.size(); idxSelPt < i_plg.size(); ++idxSelPt) {
                m_arpSelPtsPolygon.append(nullptr);
            }
        }
        // For connection lines the first and last polygon point are connection points and
        // for those no selection points should be created.
        // But if the connection line is under construction, there is no connection point at
        // the end of the connection line. In this case we need the selection points (at least
        // the selection point at the end of the last polygon point) to connect the line with
        // the connction points.
        int idxSelPtFirst = 1;
        int idxSelPtLast = i_plg.size()-2;
        if (m_editMode == EEditMode::CreatingByMouseEvents) {
            idxSelPtFirst = 0;
            idxSelPtLast = i_plg.size()-1;
        }
        for (int idxSelPt = idxSelPtFirst; idxSelPt <= idxSelPtLast; idxSelPt++) {
            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
            if (pGraphObjSelPt == nullptr) {
                pGraphObjSelPt = new CGraphObjSelectionPoint(
                    m_pDrawingScene, SGraphObjSelectionPoint(this, ESelectionPointType::PolygonPoint, idxSelPt));
                m_arpSelPtsPolygon[idxSelPt] = pGraphObjSelPt;
                m_pDrawingScene->addItem(pGraphObjSelPt);
                QObject::connect(
                    pGraphObjSelPt, &CGraphObj::aboutToBeDestroyed,
                    this, &CGraphObjConnectionLine::onSelectionPointAboutToBeDestroyed);
                QObject::connect(
                    pGraphObjSelPt, &CGraphObj::geometryOnSceneChanged,
                    this, &CGraphObjConnectionLine::onSelectionPointGeometryOnSceneChanged);
            }
        }
    }
} // showSelectionPointsOfPolygon

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
QList<SGraphObjSelectionPoint> CGraphObjConnectionLine::getPossibleLabelAnchorPoints(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    QList<SGraphObjSelectionPoint> arSelPts;
    if (i_strName == c_strLabelName) {
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjConnectionLine*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::Center));
        for (int idxPt = 0; idxPt < polygon().size(); ++idxPt) {
            arSelPts.append(SGraphObjSelectionPoint(
                const_cast<CGraphObjConnectionLine*>(this), ESelectionPointType::LineCenterPoint, idxPt));
        }
    }
    else if (isPolygonPointLabelName(i_strName)) {
        int idxPt = extractIndexFromPolygonPointLabelName(i_strName);
        if (idxPt >= 0 && idxPt < polygon().size()) {
            arSelPts.append(SGraphObjSelectionPoint(
                const_cast<CGraphObjConnectionLine*>(this), ESelectionPointType::PolygonPoint, idxPt));
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
bool CGraphObjConnectionLine::labelHasDefaultValues(const QString& i_strName) const
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
bool CGraphObjConnectionLine::geometryLabelHasDefaultValues(const QString& i_strName) const
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
QRectF CGraphObjConnectionLine::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = QGraphicsPolygonItem::boundingRect();
    // The QGraphicsPolygonItem::boundingRect call above returns strange, odd values.
    QPolygonF polygon = this->polygon();
    if (!polygon.isEmpty()) {
        rctBounding = polygon.boundingRect();
    }
    int iPenWidth = m_drawSettings.penWidth();
    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHighlighted || isSelected())) {
        iPenWidth += 3; // see paint method
    }
    rctBounding = QRectF(
        rctBounding.left() - static_cast<double>(iPenWidth)/2.0,
        rctBounding.top() - static_cast<double>(iPenWidth)/2.0,
        rctBounding.width() + static_cast<double>(iPenWidth),
        rctBounding.height() + static_cast<double>(iPenWidth));
    if (!m_plgLineStartArrowHead.isEmpty()) {
        rctBounding |= m_plgLineStartArrowHead.boundingRect();
    }
    if (!m_plgLineEndArrowHead.isEmpty()) {
        rctBounding |= m_plgLineEndArrowHead.boundingRect();
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Returns the shape of this item as a QPainterPath in local coordinates.
*/
QPainterPath CGraphObjConnectionLine::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );

    QPainterPath painterPath = QGraphicsPolygonItem::shape();
    if (!m_plgLineStartArrowHead.empty()) {
        QPolygonF plgArrowHead = m_plgLineStartArrowHead;
        // Add a closed polygon if a base line should be drawn.
        if (m_drawSettings.arrowHeadBaseLineType(ELinePoint::Start) != EArrowHeadBaseLineType::NoLine) {
            plgArrowHead.append(plgArrowHead.first());
        }
        painterPath.closeSubpath();
        painterPath.moveTo(0.0, 0.0);
        painterPath.addPolygon(plgArrowHead);
    }
    if (!m_plgLineEndArrowHead.empty()) {
        QPolygonF plgArrowHead = m_plgLineEndArrowHead;
        // Add a closed polygon if a base line should be drawn.
        if (m_drawSettings.arrowHeadBaseLineType(ELinePoint::End) != EArrowHeadBaseLineType::NoLine) {
            plgArrowHead.append(plgArrowHead.first());
        }
        painterPath.closeSubpath();
        painterPath.moveTo(0.0, 0.0);
        painterPath.addPolygon(plgArrowHead);
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
void CGraphObjConnectionLine::paint(
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
            pn.setWidth(3 + m_drawSettings.penWidth());
        }
        else {
            pn.setColor(s_highlightColor);
            pn.setWidth(3 + m_drawSettings.penWidth());
        }
        pn.setStyle(Qt::SolidLine);
        QPainterPath outline;
        outline.moveTo(polygon[0]);
        for (int idxPt = 1; idxPt < polygon.size(); ++idxPt) {
            outline.lineTo(polygon[idxPt]);
        }
        i_pPainter->strokePath(outline, pn);
    }
    i_pPainter->setRenderHints(s_painterRenderHints);
    pn.setColor(m_drawSettings.penColor());
    pn.setWidth(m_drawSettings.penWidth());
    pn.setStyle(lineStyle2QtPenStyle(m_drawSettings.lineStyle().enumerator()));
    i_pPainter->setPen(pn);
    i_pPainter->drawPolyline(polygon);

    CEnumLineEndStyle lineEndStyleLineStart = m_drawSettings.lineEndStyle(ELinePoint::Start);
    CEnumLineEndStyle lineEndStyleLineEnd = m_drawSettings.lineEndStyle(ELinePoint::End);
    if (lineEndStyleLineStart != ELineEndStyle::Normal || lineEndStyleLineEnd != ELineEndStyle::Normal) {
        CEnumArrowHeadBaseLineType baseLineTypeLineStart = m_drawSettings.arrowHeadBaseLineType(ELinePoint::Start);
        CEnumArrowHeadBaseLineType baseLineTypeLineEnd   = m_drawSettings.arrowHeadBaseLineType(ELinePoint::End);
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        i_pPainter->setPen(pn);
        if (lineEndStyleLineStart != ELineEndStyle::Normal) {
            brsh.setStyle(arrowHeadFillStyle2QtBrushStyle(m_drawSettings.arrowHeadFillStyle(ELinePoint::Start)));
            i_pPainter->setBrush(brsh);
            if (baseLineTypeLineStart == EArrowHeadBaseLineType::NoLine) {
                i_pPainter->drawPolyline(m_plgLineStartArrowHead);
            }
            else {
                if (m_drawSettings.arrowHeadFillStyle(ELinePoint::Start) == EArrowHeadFillStyle::NoFill) {
                    i_pPainter->setBrush(Qt::white);
                }
                else {
                    i_pPainter->setBrush(Qt::black);
                }
                i_pPainter->drawPolygon(m_plgLineStartArrowHead);
            }
        }
        if (lineEndStyleLineEnd != ELineEndStyle::Normal) {
            brsh.setStyle( arrowHeadFillStyle2QtBrushStyle(m_drawSettings.arrowHeadFillStyle(ELinePoint::End)) );
            i_pPainter->setBrush(brsh);
            if (baseLineTypeLineEnd == EArrowHeadBaseLineType::NoLine) {
                i_pPainter->drawPolyline(m_plgLineEndArrowHead);
            }
            else {
                if (m_drawSettings.arrowHeadFillStyle(ELinePoint::End) == EArrowHeadFillStyle::NoFill) {
                    i_pPainter->setBrush(Qt::white);
                }
                else {
                    i_pPainter->setBrush(Qt::black);
                }
                i_pPainter->drawPolygon(m_plgLineEndArrowHead);
            }
        }
    }
    i_pPainter->restore();
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
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
        QCursor cursor = Qt::SizeAllCursor;
        if (isSelected()) {
            if (m_editMode == EEditMode::ModifyingPolygonPoints) {
                if (i_pEv->modifiers() & Qt::ControlModifier) {
                    SGraphObjHitInfo hitInfo;
                    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                        mthTracer.trace("-+ isPolygonHit([" + QString::number(polygon().size()) + "], .., Pos {" + qPoint2Str(i_pEv->pos()) + ")");
                    }
                    bool bIsPolygonHit = isPolygonHit(polygon(), m_drawSettings.fillStyle(), i_pEv->pos(), m_pDrawingScene->getHitToleranceInPx(), &hitInfo);
                    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                        mthTracer.trace("+- isPolygonHit(HitInfo {" + hitInfo.toString() + "}): " + bool2Str(bIsPolygonHit));
                    }
                    if (bIsPolygonHit) {
                        cursor = hitInfo.m_cursor;
                    }
                }
            }
        }
        QGraphicsItem_setCursor(cursor);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
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
        QCursor cursor = Qt::SizeAllCursor;
        if (isSelected()) {
            if (m_editMode == EEditMode::ModifyingPolygonPoints) {
                if (i_pEv->modifiers() & Qt::ControlModifier) {
                    SGraphObjHitInfo hitInfo;
                    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                        mthTracer.trace("-+ isPolygonHit([" + QString::number(polygon().size()) + "], .., Pos {" + qPoint2Str(i_pEv->pos()) + ")");
                    }
                    bool bIsPolygonHit = isPolygonHit(polygon(), m_drawSettings.fillStyle(), i_pEv->pos(), m_pDrawingScene->getHitToleranceInPx(), &hitInfo);
                    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                        mthTracer.trace("+- isPolygonHit(HitInfo {" + hitInfo.toString() + "}): " + bool2Str(bIsPolygonHit));
                    }
                    if (bIsPolygonHit) {
                        cursor = hitInfo.m_cursor;
                    }
                }
            }
        }
        QGraphicsItem_setCursor(cursor);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
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

    // Ignore hover events if any object should be or is currently being created.
    if (m_pDrawingScene->getCurrentDrawingTool() == nullptr) {
        bool bSetCurser = false;
        QCursor cursor = Qt::SizeAllCursor;
        if (isSelected()) {
            if (m_editMode == EEditMode::ModifyingPolygonPoints) {
                if (i_pEv->modifiers() & Qt::ControlModifier) {
                    SGraphObjHitInfo hitInfo;
                    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                        mthTracer.trace("-+ isPolygonHit([" + QString::number(polygon().size()) + "], .., Pos {" + qPoint2Str(i_pEv->pos()) + ")");
                    }
                    bool bIsPolygonHit = isPolygonHit(polygon(), m_drawSettings.fillStyle(), i_pEv->pos(), m_pDrawingScene->getHitToleranceInPx(), &hitInfo);
                    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                        mthTracer.trace("+- isPolygonHit(HitInfo {" + hitInfo.toString() + "}): " + bool2Str(bIsPolygonHit));
                    }
                    if (bIsPolygonHit) {
                        cursor = hitInfo.m_cursor;
                        bSetCurser = true;
                    }
                }
            }
        }
        if (bSetCurser) {
            QGraphicsItem_setCursor(cursor);
        }
        else {
            QGraphicsItem_unsetCursor();
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
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
void CGraphObjConnectionLine::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
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
        traceConnectionPoints(mthTracer, EMethodDir::Enter);
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Enter, "Common");
    }

    bool bCallBaseMouseEventHandler = true;
    if (i_pEv->button() == Qt::LeftButton) {
        if (m_editMode == EEditMode::None) {
            if (i_pEv->modifiers() == Qt::NoModifier || i_pEv->modifiers() & Qt::ControlModifier) {
                setEditMode(EEditMode::ModifyingPolygonPoints);
            }
        }
        else if (m_editMode == EEditMode::CreatingByMouseEvents) {
            // The first mouse press right after creating the object will be ignored.
            //bool bIgnoreMouseEvent = false;
            //CPhysValPoint physValPointLast = m_physValPolygonScaledAndRotated.last();
            //if (m_physValPolygonScaledAndRotated.count() == 2) {
            //    CPhysValPoint physValPointFirst = m_physValPolygonScaledAndRotated.first();
            //    if (physValPointFirst == physValPointLast) {
            //        bIgnoreMouseEvent = true;
            //    }
            //}
            //if (!bIgnoreMouseEvent) {
            //    append(physValPointLast);
            //}
        }
        if (m_editMode == EEditMode::ModifyingPolygonPoints) {
            // Check if any line segment has been hit.
            // As this method may have been called by the selection point, "pos" would return
            // the local coordinate of the selection point. We need to use the scene pos.
            QPointF ptEvLocalPos = mapFromScene(i_pEv->scenePos());
            SGraphObjHitInfo hitInfo;
            if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                mthTracer.trace("-+ isPolygonHit([" + QString::number(polygon().size()) + "], .., Pos {" + qPoint2Str(ptEvLocalPos) + ")");
            }
            bool bIsPolygonHit = isPolygonHit(polygon(), m_drawSettings.fillStyle(), ptEvLocalPos, m_pDrawingScene->getHitToleranceInPx(), &hitInfo);
            if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
                mthTracer.trace("+- isPolygonHit(HitInfo {" + hitInfo.toString() + "}): " + bool2Str(bIsPolygonHit));
            }
            if (hitInfo.isNull()) {
                bCallBaseMouseEventHandler = false;
            }
            // If a line segment has been hit (but not at polygon shape points as those are selection points) ..
            else if (hitInfo.isLineSegmentHit()) {
                if (i_pEv->modifiers() == Qt::NoModifier || i_pEv->modifiers() & Qt::ControlModifier) {
                    // .. create a new point there.
                    insert(hitInfo.m_idxLineSegment+1, CPhysValPoint(*m_pDrawingScene, mapToParent(hitInfo.m_ptHit), Units.Length.px));
                    // The newly added selection point will become the new grabber
                    // so that newly created polygon point can be moved.
                    QGraphicsItem* pGraphicsItemMouseGrabber = m_pDrawingScene->mouseGrabberItem();
                    if (pGraphicsItemMouseGrabber != nullptr) {
                        pGraphicsItemMouseGrabber->ungrabMouse();
                    }
                    CGraphObjSelectionPoint* pGraphObjSelPtMouseGrabber = m_arpSelPtsPolygon[hitInfo.m_idxLineSegment+1];
                    if (pGraphObjSelPtMouseGrabber != nullptr) {
                        m_pDrawingScene->setMouseGrabber(pGraphObjSelPtMouseGrabber);
                    }
                }
            }
        }
    }
    else if (i_pEv->button() == Qt::RightButton) {
        if (m_editMode != EEditMode::CreatingByMouseEvents) {
            showContextMenu(i_pEv);
            bCallBaseMouseEventHandler = false;
        }
    }
    if (bCallBaseMouseEventHandler) {
        // Forward the mouse event to the base implementation.
        // This will select the item, creating selection points if not yet created.
        QGraphicsPolygonItem::mousePressEvent(i_pEv);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceConnectionPoints(mthTracer, EMethodDir::Leave);
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
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
        traceConnectionPoints(mthTracer, EMethodDir::Enter);
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Enter, "Common");
    }

    bool bCallBaseMouseEventHandler = true;
    if (m_editMode == EEditMode::None) {
    }
    else if (m_editMode == EEditMode::CreatingByMouseEvents) {
        // When releasing the mouse somewhere but not on a connection point, the last
        // polygon point will be duplicated and appended to become the new polygon point
        // to be moved while creating the connection line.
        // Exception to this rule is the first mouse release right after creating the
        // object which has to be ignored as already two polygon points at the same
        // position are existing.
        // Clicking on a connection point finishes creating the connection line.
        // Exception to this rule is if there are only two polygon points and the
        // connection point hit is the start point of the line. In this case the polygon
        // contains two points and those two points are equal (or almost equal).
        // When not clicking on a connection point finishing creating the line,
        // the mouse grabber item, which is the newly appended selection point,
        // should become (or remain) the mouse grabber.
        bool bChangeEditMode = false;
        CGraphObjConnectionPoint* pGraphObjCnctPtHit = m_pDrawingScene->getConnectionPoint(i_pEv->scenePos());
        if (pGraphObjCnctPtHit != nullptr) {
            if (m_arpCnctPts.value(ELinePoint::Start, nullptr) != pGraphObjCnctPtHit) {
                setConnectionPoint(ELinePoint::End, pGraphObjCnctPtHit);
                bChangeEditMode = true;
            }
            else if (m_physValPolygonScaledAndRotated.count() > 2) {
                setConnectionPoint(ELinePoint::End, pGraphObjCnctPtHit);
                bChangeEditMode = true;
            }
        }
        // Remove unnecessary polygon points.
        if (m_physValPolygonScaledAndRotated.count() > 2) {
            normalize();
        }
        if (bChangeEditMode) {
            // The editMode changed signal will be emitted and received by the drawing scene.
            // The drawing scene is informed this way that creation of the object is finished
            // and will unselect the current drawing tool and will select the object under
            // construction showing the selection points at the bounding rectangle.
            setEditMode(EEditMode::None);
        }
        else {
            CPhysValPoint physValPointLast = m_physValPolygonScaledAndRotated.last();
            if (m_physValPolygonScaledAndRotated.count() == 2) {
                CPhysValPoint physValPointFirst = m_physValPolygonScaledAndRotated.first();
                if (physValPointFirst != physValPointLast) {
                    // Append new polygon point which further on can be moved.
                    append(physValPointLast);
                }
            }
            m_pDrawingScene->setMouseGrabber(m_arpSelPtsPolygon.last());
        }
    }
    else if (m_editMode == EEditMode::ModifyingBoundingRect) {
    }
    else if (m_editMode == EEditMode::ModifyingPolygonPoints) {
        // If a polygon point has been newly created, the corresponding selection point
        // has been temporarily set as the mouse grabber to move the polygon point.
        // Creating the additional polygon point is finished and the selection point
        // must no longer be the mouse grabber.
        QGraphicsItem* pGraphicsItemMouseGrabber = m_pDrawingScene->mouseGrabberItem();
        CGraphObjSelectionPoint* pGraphObjSelPtMouseGrabber = dynamic_cast<CGraphObjSelectionPoint*>(pGraphicsItemMouseGrabber);
        if (pGraphObjSelPtMouseGrabber != nullptr) {
            pGraphObjSelPtMouseGrabber->ungrabMouse();
        }
        bCallBaseMouseEventHandler = false;
    }
    if (bCallBaseMouseEventHandler) {
        // Forward the mouse event to the base implementation.
        QGraphicsPolygonItem::mouseReleaseEvent(i_pEv);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceConnectionPoints(mthTracer, EMethodDir::Leave);
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
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
        traceConnectionPoints(mthTracer, EMethodDir::Enter);
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Enter, "Common");
    }

    if (m_editMode == EEditMode::CreatingByMouseEvents) {
        // When doubleclicking an item, the item will first receive a mouse press event, followed by
        // a release event (i.e., a click), then a double-click event, and finally a release event.
        // If the mouse double click event appeared on the same position as the prior mouse press and
        // release events, the last polygon points will be removed again.
        const QPolygonF& polygon = this->polygon();
        if (polygon.size() >= 3 && polygon[polygon.size()-1].toPoint() == polygon[polygon.size()-2].toPoint()) {
            removeLast();
        }
        // Double clicking finishes creating the connection line.
        // When not clicking on a connection point, the connection line will be discarded.
        CGraphObjConnectionPoint* pGraphObjCnctPtHit = m_pDrawingScene->getConnectionPoint(i_pEv->scenePos());
        if (pGraphObjCnctPtHit == nullptr) {
            // If no connection point has been hit, the connection line got to be deleted.
            // This must be done by the drawing scene through a queued deleteGraphObj call.
            m_pDrawingScene->deleteGraphObj(this);
        }
        else {
            setConnectionPoint(ELinePoint::End, pGraphObjCnctPtHit);
        }
        // The editMode changed signal will be emitted and received by the drawing scene.
        // The drawing scene is informed this way that creation of the object is finished
        // and will unselect the current drawing tool and will select the object under
        // construction showing the selection points at the bounding rectangle.
        setEditMode(EEditMode::None);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceConnectionPoints(mthTracer, EMethodDir::Leave);
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
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
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Enter, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Enter, "Common");
    }

    // Forward the mouse event to the base implementation.
    QGraphicsPolygonItem::mouseMoveEvent(i_pEv);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave, "Common");
        traceGraphObjStates(mthTracer, EMethodDir::Leave, "Common");
        traceThisPositionInfo(mthTracer, EMethodDir::Leave, "Common");
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted()) + "}");
    }
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjConnectionLine::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
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
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsItemChange2Str(i_change, i_value);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
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
            updateLineEndArrowHeadPolygons();
            bGeometryChanged = true;
            bTreeEntryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged) {
        if (m_iItemChangeUpdatePhysValCoorsBlockedCounter == 0) {
        }
        updateLineEndArrowHeadPolygons();
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
        updateLineEndArrowHeadPolygons();
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemRotationHasChanged) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
        bGeometryChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemSelectedHasChanged) {
        //QGraphicsItem_prepareGeometryChange();
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            bringToFront();
            if (m_editMode == EEditMode::CreatingByMouseEvents || m_editMode == EEditMode::ModifyingPolygonPoints) {
                showSelectionPoints(c_uSelectionPointsPolygonPoints);
                hideSelectionPoints(c_uSelectionPointsBoundingRectAll);
            }
            else if (m_editMode == EEditMode::ModifyingBoundingRect) {
                //hideSelectionPoints(c_uSelectionPointsPolygonPoints);
                //showSelectionPoints(c_uSelectionPointsBoundingRectAll);
            }
            else /*if (m_editMode == EEditMode::None)*/ {
                hideSelectionPoints();
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

    valChanged = QGraphicsPolygonItem::itemChange(i_change, i_value);

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
protected slots: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Called by the parent graphic item to inform the child about geometry changes.

    Connection lines don't belong to groups. But their connection points do.
    If a group is moved also the connection points are moved by Qt's graphics scene.
    But not the connection lines which are linked to the connection points. The
    connection points can be considered as parents of connection lines and
    "onGraphObjParentGeometryOnSceneChanged" is called by the connection points if their
    position changes.
*/
void CGraphObjConnectionLine::onGraphObjConnectionPointGeometryOnSceneChanged(
    CGraphObj* i_pGraphObjCnctPt, bool )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjCnctPt->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "onGraphObjConnectionPointGeometryOnSceneChanged",
        /* strAddInfo   */ strMthInArgs );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
    CRefCountGuard refCountGuardTrace(&m_iTraceBlockedCounter);

    CGraphObjConnectionPoint* pGraphObjCnctPt = dynamic_cast<CGraphObjConnectionPoint*>(i_pGraphObjCnctPt);
    if (pGraphObjCnctPt != nullptr) {
        CPhysValPoint physValPoint = pGraphObjCnctPt->getCenter();
        physValPoint = static_cast<CGraphObj*>(pGraphObjCnctPt)->mapToScene(physValPoint);
        ELinePoint linePoint = getConnectionLinePoint(pGraphObjCnctPt);
        if (linePoint == ELinePoint::Start) {
            replace(0, physValPoint);
        }
        else if (linePoint == ELinePoint::End) {
            replace(count()-1, physValPoint);
        }
        normalize();
        updateLineEndArrowHeadPolygons();
    }

    refCountGuardTrace.decrementAndReleaseCounter();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceThisPositionInfo(mthTracer, EMethodDir::Enter);
    }
}

////------------------------------------------------------------------------------
//void CGraphObjConnectionLine::onGraphObjConnectionPointZValueChanged(CGraphObj* i_pGraphObjCnctPt)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_pGraphObjCnctPt->keyInTree();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "onGraphObjConnectionPointZValueChanged",
//        /* strAddInfo   */ strMthInArgs );
//
//    // The connection lines should be drawn above the connection points.
//    double fZValueParent = i_pGraphObjCnctPt->getStackingOrderValue();
//    setStackingOrderValue(fZValueParent + 0.1);
//}

/*==============================================================================
protected: // overridable slots of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplements the method of base class CGraphObj.
*/
void CGraphObjConnectionLine::onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint)
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

    connectGeometryOnSceneChangedSlotWithSelectionPoints();
}

/*==============================================================================
protected: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::onActionModifyPointsTriggered()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = m_hitInfoOnShowContextMenu.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onActionModifyPointsTriggered",
        /* strAddInfo   */ strMthInArgs );

    if (m_editMode != EEditMode::CreatingByMouseEvents) {
        setEditMode(EEditMode::ModifyingPolygonPoints);
    }
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::onActionDeletePointTriggered()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = m_hitInfoOnShowContextMenu.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "CGraphObj::onActionDeletePointTriggered",
        /* strAddInfo   */ strMthInArgs );

    if (m_hitInfoOnShowContextMenu.isPolygonShapePointHit() && polygon().size() > 2) {
        if (m_hitInfoOnShowContextMenu.m_idxPolygonShapePoint >= 0 && m_hitInfoOnShowContextMenu.m_idxPolygonShapePoint < polygon().size()) {
            removeAt(m_hitInfoOnShowContextMenu.m_idxPolygonShapePoint);
        }
    }
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the pure virtual method of base class CGraphObj.
*/
void CGraphObjConnectionLine::updateTransformedCoorsOnParentChanged(
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
void CGraphObjConnectionLine::updateTransformedCoorsOnParentGeometryChanged()
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
void CGraphObjConnectionLine::updateTransformedCoorsOnItemPositionChanged()
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
bool CGraphObjConnectionLine::lineEndArrowHeadPolygonsNeedUpdate(
    const CEnumLinePoint& i_linePoint, const CDrawSettings& i_drawSettingsOld) const
//------------------------------------------------------------------------------
{
    bool bNeedUpdate = false;

    if (i_drawSettingsOld.lineRecordType() != m_drawSettings.lineRecordType()) {
        bNeedUpdate = true;
    }
    else if (i_drawSettingsOld.lineExtent() != m_drawSettings.lineExtent()) {
        bNeedUpdate = true;
    }
    else if (i_drawSettingsOld.lineEndStyle(i_linePoint) != m_drawSettings.lineEndStyle(i_linePoint)) {
        bNeedUpdate = true;
    }
    else if (i_drawSettingsOld.arrowHeadBaseLineType(i_linePoint) != m_drawSettings.arrowHeadBaseLineType(i_linePoint)) {
        bNeedUpdate = true;
    }
    else if (i_drawSettingsOld.arrowHeadWidth(i_linePoint) != m_drawSettings.arrowHeadWidth(i_linePoint)) {
        bNeedUpdate = true;
    }
    else if (i_drawSettingsOld.arrowHeadLength(i_linePoint) != m_drawSettings.arrowHeadLength(i_linePoint)) {
        bNeedUpdate = true;
    }
    return bNeedUpdate;
}

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::updateLineEndArrowHeadPolygons(const CEnumLinePoint& i_linePoint)
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
            CEnumLineEndStyle lineEndStyle = m_drawSettings.lineEndStyle(ELinePoint::Start);
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
            CEnumLineEndStyle lineEndStyle = m_drawSettings.lineEndStyle(ELinePoint::End);
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
QPolygonF CGraphObjConnectionLine::getPolygonScaled(const QPolygonF& i_polygonOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ItemCoors {" + qPolygon2Str(i_polygonOrig) + "}";
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
CPhysValPolygon CGraphObjConnectionLine::getPhysValPolygonOrig(const QPolygonF& i_polygonOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ItemCoors {" + qPolygon2Str(i_polygonOrig) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValPolygonOrig",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    CGraphObjConnectionLine* pVThis = const_cast<CGraphObjConnectionLine*>(this);
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
CPhysValPolygon CGraphObjConnectionLine::getPhysValPolygonScaled(const CPhysValPolygon& i_physValPolygonOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "PhysValCoors {" + i_physValPolygonOrig.toString() + "} " + i_physValPolygonOrig.unit().symbol();
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
void CGraphObjConnectionLine::normalize()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "normalize",
        /* strAddInfo   */ "" );

    QPolygonF plgOld = getPolygon(m_pDrawingScene->drawingSize().unit()).toQPolygonF();
    if (plgOld.size() > 2) {
        QPolygonF plgNew = normalizePolygon(plgOld, m_pDrawingScene->getHitToleranceInPx());
        if (plgOld.size() != plgNew.size()) {
            setPolygon(plgNew, m_pDrawingScene->drawingSize().unit());
            hideSelectionPoints();
            if (isSelected()) {
                showSelectionPoints();
            }
        }
    }
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CGraphObjConnectionLine::getNumberOfPredefinedPolygonPointLabelNames() const
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
int CGraphObjConnectionLine::getNumberOfPolygonPointGeometryLabelNames() const
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
void CGraphObjConnectionLine::updateLabelsOnPolygonPointsAdded()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateLabelsOnPolygonPointsAdded",
        /* strAddInfo   */ "" );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
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
void CGraphObjConnectionLine::updateLabelsOnPolygonPointsRemoved()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateLabelsOnPolygonPointsRemoved",
        /* strAddInfo   */ "" );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) && mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
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
QPolygonF CGraphObjConnectionLine::setPolygonOrig(const QPolygonF& i_polygon)
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
QPolygonF CGraphObjConnectionLine::QGraphicsPolygonItem_setPolygon(const QPolygonF& i_polygon)
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
CPhysValPolygon CGraphObjConnectionLine::setPhysValPolygonOrig(const CPhysValPolygon& i_physValPolygon)
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
CPhysValPolygon CGraphObjConnectionLine::setPhysValPolygonScaledAndRotated(const CPhysValPolygon& i_physValPolygon)
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
void CGraphObjConnectionLine::QGraphicsItem_prepareGeometryChange()
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
void CGraphObjConnectionLine::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, const QString& i_strFilter) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }

    CGraphObj::traceThisPositionInfo(i_mthTracer, i_mthDir, i_strFilter);

    QPolygonF polygon = this->polygon();
    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "GraphObjPolygon {";
    i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = " . ";
        strRuntimeInfo += ". PhysValPolygon Orig {" + m_physValPolygonOrig.toString() + "} " + m_physValPolygonOrig.unit().symbol()
            + ", ScaledRotated {" + m_physValPolygonScaledAndRotated.toString() + "} " + m_physValPolygonScaledAndRotated.unit().symbol();
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = " . ";
        strRuntimeInfo += ". ItemPolygon Orig {" + qPolygon2Str(m_polygonOrig) + "}, Scaled {" + qPolygon2Str(polygon) + "}";
        i_mthTracer.trace(strRuntimeInfo);
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "}";
    i_mthTracer.trace(strRuntimeInfo);
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjConnectionLine::traceConnectionPoints(CMethodTracer& i_mthTracer, EMethodDir i_mthDir) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTraceConnectionPointsBlockedCounter > 0) {
        return;
    }

    QString strRuntimeInfo;
    if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
    else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
    else strRuntimeInfo = " . ";
    strRuntimeInfo += "ConnectionPoints [" + QString::number(m_arpCnctPts.size()) + "]";
    if (!m_arpCnctPts.isEmpty()) {
        strRuntimeInfo += "(";
        for (ELinePoint linePoint : m_arpCnctPts.keys()) {
            const CGraphObjConnectionPoint* pGraphObjCnctPt = m_arpCnctPts[linePoint];
            if (!strRuntimeInfo.endsWith("(")) strRuntimeInfo += ", ";
            if (pGraphObjCnctPt == nullptr) {
                strRuntimeInfo += CEnumLinePoint(linePoint).toString() + ": null";
            }
            else {
                strRuntimeInfo += CEnumLinePoint(linePoint).toString() + ": " + pGraphObjCnctPt->path();
            }
        }
        strRuntimeInfo += ")";
    }
    i_mthTracer.trace(strRuntimeInfo);
}
