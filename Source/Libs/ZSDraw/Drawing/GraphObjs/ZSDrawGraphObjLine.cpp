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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLinePropertiesDlg.h"
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
class CGraphObjLine : public CGraphObj, public QGraphicsLineItem
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

qint64 CGraphObjLine::s_iInstCount = 0;

/*==============================================================================
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjLine::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjLine::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLine::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjLine::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a graphical line object.

    The number of line objects stored in s_iInstCount is increased to create
    a unique line name when creating objects by passing an empty object name.

    @param [in] i_strObjName
        Name of the graphical object.
        Names of graphical objects must be unique below its parent.
        If an empty string is passed a unique name is created by adding the
        current number of line objects taken from s_iInstCount to "Line".
*/
CGraphObjLine::CGraphObjLine(CDrawingScene* i_pDrawingScene, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeLine,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeLine),
        /* strObjName          */ i_strObjName.isEmpty() ? "Line" + QString::number(s_iInstCount) : i_strObjName),
    QGraphicsLineItem(),
    m_lineOrig(),
    m_physValLineOrig(*m_pDrawingScene),
    m_physValLineScaled(*m_pDrawingScene),
    m_physValLineScaledAndRotated(*m_pDrawingScene),
    m_plgP1ArrowHead(),
    m_plgP2ArrowHead()
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

    m_strlstPredefinedLabelNames.append(c_strLabelName);
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameP1);
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameP2);

    for (const QString& strLabelName : m_strlstPredefinedLabelNames) {
        if (!m_hshpLabels.contains(strLabelName)) {
            if (strLabelName == c_strGeometryLabelNameP1) {
                addLabel(strLabelName, strLabelName, ESelectionPointType::PolygonPoint, 0);
            }
            else if (strLabelName == c_strGeometryLabelNameP2) {
                addLabel(strLabelName, strLabelName, ESelectionPointType::PolygonPoint, 1);
            }
            else {
                addLabel(strLabelName, strLabelName, ESelectionPointType::BoundingRectangle, ESelectionPoint::Center);
            }
        }
    }

    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameP1);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameP2);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameCenter);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameDX);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameDY);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameLength);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameAngle);

    const CUnit& unit = m_pDrawingScene->drawingSize().unit();
    for (const QString& strLabelName : m_strlstGeometryLabelNames) {
        if (strLabelName == c_strGeometryLabelNameP1) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, 0);
        }
        else if (strLabelName == c_strGeometryLabelNameP2) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, 1);
        }
        else if (strLabelName == c_strGeometryLabelNameCenter) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::Center);
        }
        else if (strLabelName == c_strGeometryLabelNameDY) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryDY, 0, 1);
        }
        else if (strLabelName == c_strGeometryLabelNameDX) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryDX, 0, 1);
        }
        else if (strLabelName == c_strGeometryLabelNameLength) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryLength, 0, 1);
        }
        else if (strLabelName == c_strGeometryLabelNameAngle) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryAngle, 0, 1);
        }
    }

    // Initialise list with number of selection points.
    // A line may provide two selection points - at start and end of line.
    m_arpSelPtsPolygon.append(nullptr);
    m_arpSelPtsPolygon.append(nullptr);

    setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable
            |QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
    setAcceptHoverEvents(true);

} // ctor

//------------------------------------------------------------------------------
CGraphObjLine::~CGraphObjLine()
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
}

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjLine::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeLine;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjLine::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CGraphObjLine* pGraphObjLine = new CGraphObjLine(m_pDrawingScene, m_strName);
    pGraphObjLine->setLine(getLine(drawingSize.unit()));
    pGraphObjLine->setDrawSettings(m_drawSettings);
    return pGraphObjLine;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to create a user defined dialog.
*/
void CGraphObjLine::openFormatGraphObjsDialog()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "openFormatGraphObjsDialog",
        /* strAddInfo   */ "" );

    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Format Line";
    CDlgGraphObjLineProperties* pDlg = CDlgGraphObjLineProperties::GetInstance(this);
    if( pDlg == nullptr ) {
        pDlg = CDlgGraphObjLineProperties::CreateInstance(strDlgTitle, this);
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
void CGraphObjLine::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
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
/*! @brief Sets the item's line to the given line.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or bottom left corner of the
    parent item's bounding rectangle.

    @param [in] i_physValLine
        Line coordinates to be set.
*/
void CGraphObjLine::setLine( const CPhysValLine& i_physValLine )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValLine.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setLine",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    QPointF ptPosPrev = pos();

    // Depending on the Y scale orientation of the drawing scene the line coordinates
    // have been passed either relative to the top left or bottom right corner of the
    // parent item's bounding rectangle.
    // The coordinates need to be transformed into the local coordinate system of the graphical
    // object whose origin point is the center of the objects bounding rectangle.

    QLineF lineF;
    QPointF ptPos = getItemPosAndLocalCoors(i_physValLine, lineF);

    bool bGeometryOnSceneChanged = false;

    // If the coordinates MUST be updated (e.g. after the drawing size has been changed)
    // or if the coordinates have been changed ...
    if (m_physValLineOrig.isNull() || line() != lineF || m_physValLineOrig != i_physValLine/* || m_bForceConversionToSceneCoors*/)
    {
        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsItem_prepareGeometryChange();

        {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdatePhysValCoorsBlockedCounter);
            CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

            // Store physical coordinates.
            setPhysValLineOrig(i_physValLine);
            setPhysValLineScaled(i_physValLine);
            setPhysValLineScaledAndRotated(i_physValLine);

            // Set the line in local coordinate system.
            // Also note that itemChange must not overwrite the current values (refCountGuard).
            setLineOrig(lineF);
            QGraphicsLineItem_setLine(lineF);

            // Please note that GraphicsLineItem::setLine did not update the position of the
            // item in the parent. This has to be done "manually" afterwards.

            // Move the object to the parent position.
            // This has to be done after resizing the line which updates the local coordinates
            // of the line with origin (0/0) at the lines center point.
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
/*! @brief Sets the item's line to the given line coordinates.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or relative to the bottom right
    corner of the parent item's bounding rectangle.

    @param [in] i_fX1
        X coordinate of point 1 passed by the given unit.
    @param [in] i_fY1
        Y coordinate of point 1 passed by the given unit.
    @param [in] i_fX2
        X coordinate of point 2 passed by the given unit.
    @param [in] i_fY2
        y coordinate of point 2 passed by the given unit.
    @param [in] i_unit
        Unit in which the coordinates are passed.
*/
void CGraphObjLine::setLine(
    double i_fX1, double i_fY1, double i_fX2, double i_fY2, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "X1: " + QString::number(i_fX1) +
                       "Y1: " + QString::number(i_fY1) +
                       "X2: " + QString::number(i_fX2) +
                       "Y2: " + QString::number(i_fY2) +
                       "Unit: " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setLine",
        /* strAddInfo   */ strMthInArgs );

    setLine(CPhysValLine(*m_pDrawingScene, i_fX1, i_fY1, i_fX2, i_fY2, i_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the item's line to the given line coordinates.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or relative to the bottom right
    corner of the parent item's bounding rectangle.

    @param [in] i_p1
        X and Y coordinates of point 1 passed by the given unit.
    @param [in] i_p2
        X and Y coordinates of point 2 passed by the given unit.
    @param [in] i_unit
        Unit in which the coordinates are passed.
*/
void CGraphObjLine::setLine(
    const QPointF& i_p1, const QPointF& i_p2, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "P1 {" + qPoint2Str(i_p1) + "}, P2 {" + qPoint2Str(i_p2) + "} " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setLine",
        /* strAddInfo   */ strMthInArgs );

    setLine(CPhysValLine(*m_pDrawingScene, i_p1, i_p2, i_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the item's line to the given line coordinates.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or relative to the bottom right
    corner of the parent item's bounding rectangle.

    @param [in] i_line
        Line coordinates passed by the given unit.
    @param [in] i_unit
        Unit in which the coordinates are passed.
*/
void CGraphObjLine::setLine(const QLineF& i_line, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Line {" + qLine2Str(i_line) + "} " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setLine",
        /* strAddInfo   */ strMthInArgs );

    setLine(CPhysValLine(*m_pDrawingScene, i_line, i_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the line coordinates by passing P1 and P2.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or relative to the bottom right
    corner of the parent item's bounding rectangle.

    @param [in] i_physValP1
        Start point of the line.
    @param [in] i_physValP2
        End point of the line.
*/
void CGraphObjLine::setLine(
    const CPhysValPoint& i_physValP1, const CPhysValPoint& i_physValP2)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "P1: " + i_physValP1.toString() + ", P2: " + i_physValP2.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setLine",
        /* strAddInfo   */ strMthInArgs );

    setLine(CPhysValLine(i_physValP1, i_physValP2));
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's line coordinates either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the current unit of the drawing scene.
*/
CPhysValLine CGraphObjLine::getLine() const
//------------------------------------------------------------------------------
{
    return getLine(m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's line coordinates either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the given unit.

    @param [in] i_unit
        Unit in which the line coordinates should be returned.
*/
CPhysValLine CGraphObjLine::getLine(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (parentGroup() != nullptr) {
        return parentGroup()->convert(m_physValLineScaledAndRotated, i_unit);
    }
    else {
        return m_pDrawingScene->convert(m_physValLineScaledAndRotated, i_unit);
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the coordinates of P1 by keeping P2 unchanged.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or relative to the bottom right
    corner of the parent item's bounding rectangle.

    @param [in] i_p1
        X and Y coordinate of point 1.
        The coordinates must be passed in the current unit of the drawing scene.
*/
void CGraphObjLine::setP1(const QPointF& i_p1)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_p1) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setP1",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLine = getLine();
    physValLine.setP1(i_p1);
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Sets the coordinates of P1 by keeping P2 unchanged.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or relative to the bottom right
    corner of the parent item's bounding rectangle.

    @param [in] i_physValP1
        X and Y coordinate of point 1. If the unit of the passed point is
        different from the current unit of drawing scene, the point coordinates
        will be converted to the current unit of the drawing scene.
*/
void CGraphObjLine::setP1(const CPhysValPoint& i_physValP1)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValP1.toString(true, ",") + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setP1",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLine = getLine();
    physValLine.setP1(i_physValP1);
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's P1 coordinate either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the current unit of the drawing scene.
*/
CPhysValPoint CGraphObjLine::getP1() const
//------------------------------------------------------------------------------
{
    return getLine().p1();
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's P1 coordinate either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the given unit.

    @param [in] i_unit
        Unit in which the coordinates should be returned.
*/
CPhysValPoint CGraphObjLine::getP1(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getLine(i_unit).p1();
}

//------------------------------------------------------------------------------
/*! @brief Sets the coordinates of P2 by keeping P1 unchanged.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or relative to the bottom right
    corner of the parent item's bounding rectangle.

    @param [in] i_p2
        X and Y coordinate of point 2.
        The coordinates must be passed in the current unit of the drawing scene.
*/
void CGraphObjLine::setP2(const QPointF& i_p2)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_p2) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setP2",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLine = getLine();
    physValLine.setP2(i_p2);
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Sets the coordinates of P2 by keeping P1 unchanged.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or relative to the bottom right
    corner of the parent item's bounding rectangle.

    @param [in] i_physValP2
        X and Y coordinate of point 2. If the unit of the passed point is
        different from the current unit of drawing scene, the point coordinates
        will be converted to the current unit of the drawing scene.
*/
void CGraphObjLine::setP2(const CPhysValPoint& i_physValP2)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValP2.toString(true, ",") + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setP2",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLine = getLine();
    physValLine.setP2(i_physValP2);
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's P2 coordinate either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the current unit of the drawing scene.
*/
CPhysValPoint CGraphObjLine::getP2() const
//------------------------------------------------------------------------------
{
    return getLine().p2();
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's P2 coordinate either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the given unit.

    @param [in] i_unit
        Unit in which the coordinates should be returned.
*/
CPhysValPoint CGraphObjLine::getP2(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getLine(i_unit).p2();
}

//------------------------------------------------------------------------------
/*! @brief Sets the center point of the line by updating both point 1 and point 2
           correspondingly. The length and the angle remain the same.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or relative to the bottom right
    corner of the parent item's bounding rectangle.

    @param [in] i_pCenter
        X and Y coordinate of point 1.
        The coordinates must be passed in the current unit of the drawing scene.
*/
void CGraphObjLine::setCenter(const QPointF& i_pCenter)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pCenter) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLine = getLine();
    physValLine.setCenter(i_pCenter);
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Sets the center point of the line by updating both point 1 and point 2
           correspondingly. The length and the angle remain the same.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or relative to the bottom right
    corner of the parent item's bounding rectangle.

    @param [in] i_physValCenter
        X and Y coordinate of the center point. If the unit of the passed point is
        different from the current unit of the drawing scene, the point coordinates
        will be converted into scene's current unit.
*/
void CGraphObjLine::setCenter(const CPhysValPoint& i_physValCenter)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValCenter.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLine = getLine();
    physValLine.setCenter(i_physValCenter);
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's center coordinate either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the current unit of the drawing scene.
*/
CPhysValPoint CGraphObjLine::getCenter() const
//------------------------------------------------------------------------------
{
    return getLine().center();
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's center coordinate either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the given unit.

    @param [in] i_unit
        Unit in which the coordinates should be returned.
*/
CPhysValPoint CGraphObjLine::getCenter(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getLine(i_unit).center();
}

//------------------------------------------------------------------------------
/*! @brief Sets the length of the line in the given unit by keeping P1 unchanged
           and moving P2 correspondingly.

    The angle remains the same.

    @param [in] i_fLength
        Length to be set.
*/
void CGraphObjLine::setLength(double i_fLength)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fLength);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setLength",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLine = getLine();
    physValLine.setLength(i_fLength);
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Sets the length of the line in the given unit.

    Point 1 remains unchanged. Point 2 is updated correspondingly.
    The angle remains the same.

    @param [in] i_physValLength
        Length to be set.
*/
void CGraphObjLine::setLength(const CPhysVal& i_physValLength)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValLength.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setLength",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLine = getLine();
    physValLine.setLength(i_physValLength);
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's length in the current unit of the drawing scene.
*/
CPhysVal CGraphObjLine::getLength() const
//------------------------------------------------------------------------------
{
    return getLine().length();
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's length in the given unit.

    @param [in] i_unit
        Unit in which the length should be returned.
*/
CPhysVal CGraphObjLine::getLength(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getLine(i_unit).length();
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
void CGraphObjLine::setRotationAngle(double i_fAngle_degree)
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
/*! @brief Sets the clockwise rotation angle around the z axis.

    For lines the z-axis goes through P1.

                         P1        0°        P2
      X P1 ---            X------------------X              X P2
      |     |                                               |
      |    / 90°                                         +->|
      |   /                                             /   |
      |<-                                              |270°|
      |                  P2                  P1         \   |
      X P2                X------------------X           \  X P1 ----
                          ^                  |            \        /
                           \                /              +------+
                            +-----180°-----+

    The center point and the length of the line remains unchanged.
    Points 1 and 2 are moved correspondingly.

    @param [in] i_physValAngle
        Angle to be set.
*/
void CGraphObjLine::setRotationAngle(const CPhysVal& i_physValAngle)
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

    CPhysValLine physValLine = getLine();
    physValLine.setAngle(i_physValAngle);
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's angle in the given unit.

    The return value will be in the range of values from 0.0 up to but not
    including 360.0.
    
    @param [in] i_unit
        Unit in which the angle should be returned.

    @sa setAngle()
*/
CPhysVal CGraphObjLine::rotationAngle(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysValLine physValLine = getLine();
    return physValLine.angle();
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Reimplements the virtual method of base class CGraphObj.
*/
QRectF CGraphObjLine::getBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ "" );

    // Line points in local coordinates.
    QLineF lineF = line();
    QRectF rctBounding(lineF.p1(), lineF.p2());
    // Width and height should never be less than 0.0.
    if (rctBounding.width() < 0.0) {
        rctBounding.moveLeft(rctBounding.left() + rctBounding.width());
        rctBounding.setWidth(-rctBounding.width());
    }
    if (rctBounding.height() < 0.0) {
        rctBounding.moveTop(rctBounding.top() + rctBounding.height());
        rctBounding.setHeight(-rctBounding.height());
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Reimplements the virtual method of base class CGraphObj.
*/
QRectF CGraphObjLine::getEffectiveBoundingRectOnScene() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getEffectiveBoundingRectOnScene",
        /* strAddInfo   */ "" );

    QPolygonF plg({
        mapToScene(line().p1()),
        mapToScene(line().p2())
    });
    QRectF rctBounding = plg.boundingRect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Returns the rotated, physical bounding rectangle.
*/
CPhysValRect CGraphObjLine::getPhysValBoundingRect(const CUnit& i_unit) const
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

    CPhysValRect physValRectBounding(*m_pDrawingScene, getBoundingRect());
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
QCursor CGraphObjLine::getProposedCursor(const QPointF& i_pt) const
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
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItemThis != nullptr) {
        CGraphObjSelectionPoint* pGraphObjSelPtHit = getSelectionPointHit(i_pt);
        if (pGraphObjSelPtHit != nullptr) {
            cursor = pGraphObjSelPtHit->getProposedCursor(i_pt);
        }
        else {
            SGraphObjHitInfo hitInfo;
            if (isLineHit(line(), i_pt, m_pDrawingScene->getHitToleranceInPx(), &hitInfo)) {
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
CPhysValPoint CGraphObjLine::getPositionOfSelectionPoint(
    ESelectionPointType i_selPtType, int i_idxPt, const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysValPoint physValPos(*m_pDrawingScene);
    //CGraphObjLine* pVThis = const_cast<CGraphObjLine*>(this);
    //const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    //CRefCountGuard refCountGuardGeometryChangedSignal(&pVThis->m_iGeometryOnSceneChangedSignalBlockedCounter);
    //QPointF ptPos = QPointF(i_idxPt == 0 ? line().p1() : line().p2());
    //// Before mapping to parent or scene, the rotation will be reset.
    //// Otherwise transformed coordinates will be returned.
    //// And itemChange is called but should not emit the geometryChangds signal ..
    //pVThis->QGraphicsItem_setRotation(0.0);
    //ptPos = pGraphicsItemThis->mapToParent(ptPos);
    //pVThis->QGraphicsItem_setRotation(m_physValRotationAngle.getVal(Units.Angle.Degree));
    CPhysValPoint physValPoint(i_idxPt == 0 ? m_physValLineScaledAndRotated.p1() : m_physValLineScaledAndRotated.p2());
    if (parentGroup() != nullptr) {
        physValPoint = parentGroup()->convert(physValPoint, i_unit);
    }
    else {
        physValPoint = m_pDrawingScene->convert(physValPoint, i_unit);
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
/*! @brief Returns coordinates of selection point in scene coordinates.
*/
QPointF CGraphObjLine::getPositionOfSelectionPointInSceneCoors(
    ESelectionPointType i_selPtType, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    QPointF ptScenePos;
    QPointF ptPos = line().center();
    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        ptScenePos = pGraphicsItem->mapToScene(ptPos);
    }
    return ptScenePos;
}

//------------------------------------------------------------------------------
/*! @brief Returns coordinates of selection point in scene coordinates.
*/
QPointF CGraphObjLine::getPositionOfSelectionPointInSceneCoors(
    ESelectionPointType i_selPtType, int i_idxPt) const
//------------------------------------------------------------------------------
{
    QPointF ptScenePos;
    QPointF ptPos = QPointF(i_idxPt == 0 ? line().p1() : line().p2());
    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        ptScenePos = pGraphicsItem->mapToScene(ptPos);
    }
    return ptScenePos;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates in length and angle of the given point
           to the selection point of the graphical object.

    How the angle of anchor lines to selection points is interpreted depends on
    the graphical object type and the selection point.

    For the graphical object Line only the Center point is allowed for this method.

    Example: Horizontal Line

                        + Pt
                       /
                      / Calculated Angle: 60°
        +------------x------------+
        P1         Center         P2

    @note This method is used to keep the relative position of labels to the
          graphical object they are linked to if the linked object is resized,
          rotated or moved.

          For example if the line would be rotated by 180°:

            P2         Center         P1
            +------------x------------+
             Angle: 60° /
                       /
                   Pt +
*/
SPolarCoors CGraphObjLine::getPolarCoorsToSelectionPointFromSceneCoors(
    const QPointF& i_pt, ESelectionPointType i_selPtType, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QLineF thisLine = line();
    QLineF thisLineSceneCoors(pGraphicsItemThis->mapToScene(thisLine.p1()), pGraphicsItemThis->mapToScene(thisLine.p2()));
    QPointF ptSelPtSceneCoors(pGraphicsItemThis->mapToScene(thisLine.center()));
    QLineF lineFromSelPtSceneCoors(ptSelPtSceneCoors, i_pt);
    double fAngle_degree = thisLineSceneCoors.angleTo(lineFromSelPtSceneCoors);
    return SPolarCoors(lineFromSelPtSceneCoors.length(), fAngle_degree);
}

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates in length and angle of the given point
           to the selection point of the graphical object.

    How the angle of anchor lines to selection points is interpreted depends on
    the graphical object type and the selection point.

    For the graphical object Line only the shape points P1 and P2 are allowed for this method.

    Example: Horizontal Line

           + Pt
          /
         / Calculated Angle: 60°
        +------------x------------+
        P1         Center         P2

    @note This method is used to keep the relative position of labels to the
          graphical object they are linked to if the linked object is resized,
          rotated or moved.

          For example if the line would be rotated by 180°:

            P2         Center         P1
            +------------x------------+
                          Angle: 60° /
                                    /
                                Pt +
*/
SPolarCoors CGraphObjLine::getPolarCoorsToSelectionPointFromSceneCoors(
    const QPointF& i_pt, ESelectionPointType i_selPtType, int i_idxPt) const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QLineF thisLine = line();
    QLineF thisLineSceneCoors;
    if (i_idxPt == 0) {
        thisLineSceneCoors = QLineF(pGraphicsItemThis->mapToScene(thisLine.p1()),
                                    pGraphicsItemThis->mapToScene(thisLine.p2()));
    }
    else {
        thisLineSceneCoors = QLineF(pGraphicsItemThis->mapToScene(thisLine.p2()),
                                    pGraphicsItemThis->mapToScene(thisLine.p1()));
    }
    QPointF ptSelPtSceneCoors = thisLineSceneCoors.p1();
    QLineF lineFromSelPtSceneCoors(ptSelPtSceneCoors, i_pt);
    double fAngle_degree = thisLineSceneCoors.angleTo(lineFromSelPtSceneCoors);
    return SPolarCoors(lineFromSelPtSceneCoors.length(), fAngle_degree);
}

//------------------------------------------------------------------------------
/*! @brief Returns a line with the given length and angle with the start point (P1)
           at the given selection point in scene coordinates.

    For the graphical object Line only the Center point is allowed for this method.

    For more details see base implementation in CGraphObj.
*/
QLineF CGraphObjLine::getAnchorLineToSelectionPointFromPolarInSceneCoors(
    const SPolarCoors& i_polarCoors, ESelectionPointType i_selPtType, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QLineF lineSelPt = line();
    QLineF lineSelPtSceneCoors(pGraphicsItemThis->mapToScene(lineSelPt.center()),
                               pGraphicsItemThis->mapToScene(lineSelPt.p2()));
    return ZS::Draw::getLineFromPolar(
        i_polarCoors.m_fLength_px, i_polarCoors.m_fAngle_degrees, lineSelPtSceneCoors);
}

//------------------------------------------------------------------------------
/*! @brief Returns a line with the given length and angle with the start point (P1)
           at the given selection point in scene coordinates.

    For more details see base implementation in CGraphObj.
*/
QLineF CGraphObjLine::getAnchorLineToSelectionPointFromPolarInSceneCoors(
    const SPolarCoors& i_polarCoors, ESelectionPointType i_selPtType, int i_idxPt) const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QLineF lineSelPt = line();
    QLineF lineSelPtSceneCoors;
    if (i_idxPt == 0) {
        lineSelPtSceneCoors = QLineF(pGraphicsItemThis->mapToScene(lineSelPt.p1()),
                                     pGraphicsItemThis->mapToScene(lineSelPt.p2()));
    }
    else {
        lineSelPtSceneCoors = QLineF(pGraphicsItemThis->mapToScene(lineSelPt.p2()),
                                     pGraphicsItemThis->mapToScene(lineSelPt.p1()));
    }
    return ZS::Draw::getLineFromPolar(
        i_polarCoors.m_fLength_px, i_polarCoors.m_fAngle_degrees, lineSelPtSceneCoors);
}

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::showSelectionPoints(TSelectionPointTypes i_selPts)
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
        if (i_selPts & c_uSelectionPointsPolygonPoints) {
            QLineF lineF = line();
            QPolygonF plg;
            plg.append(lineF.p1());
            plg.append(lineF.p2());
            showSelectionPointsOfPolygon(plg);
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
QList<SGraphObjSelectionPoint> CGraphObjLine::getPossibleLabelAnchorPoints(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    static QList<SGraphObjSelectionPoint> s_arSelPtsUserDefined;
    if (s_arSelPtsUserDefined.isEmpty()) {
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjLine*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::Center));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjLine*>(this), ESelectionPointType::PolygonPoint, 0)); // P1, Start point of the line
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjLine*>(this), ESelectionPointType::PolygonPoint, 1)); // P2, End point of the line
    }
    static QHash<QString, QList<SGraphObjSelectionPoint>> s_hshSelPtsPredefined;
    if (s_hshSelPtsPredefined.isEmpty()) {
        QList<SGraphObjSelectionPoint> arSelPts;
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjLine*>(this), ESelectionPointType::BoundingRectangle, ESelectionPoint::Center));
        s_hshSelPtsPredefined.insert(c_strLabelName, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjLine*>(this), ESelectionPointType::PolygonPoint, 0));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameP1, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(
            const_cast<CGraphObjLine*>(this), ESelectionPointType::PolygonPoint, 1));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameP2, arSelPts);
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
bool CGraphObjLine::labelHasDefaultValues(const QString& i_strName) const
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
        else if (i_strName == c_strGeometryLabelNameP1) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::PolygonPoint) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_idxPt != 0) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameP2) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::PolygonPoint) {
                bHasDefaultValues = false;
            }
            if (labelDscr.m_selPt1.m_idxPt != 1) {
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
bool CGraphObjLine::geometryLabelHasDefaultValues(const QString& i_strName) const
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
        else if (i_strName == c_strGeometryLabelNameP1) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::PolygonPoint) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_idxPt != 0) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameP2) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::PolygonPoint) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_idxPt != 1) {
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
        else {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::PolygonPoint) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_idxPt != 0) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_selPtType != ESelectionPointType::PolygonPoint) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_idxPt != 1) {
                bHasDefaultValues = false;
            }
        }
    }
    return bHasDefaultValues;
}

/*==============================================================================
public: // must overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjLine::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = QGraphicsLineItem::boundingRect();
    for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsPolygon) {
        if (pGraphObjSelPt != nullptr) {
            QRectF rctSelPt = pGraphObjSelPt->boundingRect();
            QPolygonF plgSelPt = mapFromItem(pGraphObjSelPt, rctSelPt);
            rctBounding |= plgSelPt.boundingRect();
        }
    }
    if (!m_plgP1ArrowHead.isEmpty()) {
        rctBounding |= m_plgP1ArrowHead.boundingRect();
    }
    if (!m_plgP2ArrowHead.isEmpty()) {
        rctBounding |= m_plgP2ArrowHead.boundingRect();
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
/*! @brief Reimplements QGraphicsItem::shape.
*/
QPainterPath CGraphObjLine::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );

    QPainterPath painterPath = QGraphicsLineItem::shape();
    if (!m_plgP1ArrowHead.isEmpty()) {
        painterPath.addPolygon(m_plgP1ArrowHead);
    }
    if (!m_plgP2ArrowHead.isEmpty()) {
        painterPath.addPolygon(m_plgP2ArrowHead);
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
void CGraphObjLine::paint(
    QPainter* i_pPainter,
    const QStyleOptionGraphicsItem* i_pStyleOption,
    QWidget* i_pWdgt )
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

    QPen pn = pen();
    QLineF lineF = line();
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
        outline.moveTo(lineF.p1());
        outline.lineTo(lineF.p2());
        i_pPainter->strokePath(outline, pn);
        pn.setWidth(1 + m_drawSettings.getPenWidth());
    }
    pn.setColor(m_drawSettings.getPenColor());
    pn.setWidth(m_drawSettings.getPenWidth());
    pn.setStyle(lineStyle2QtPenStyle(m_drawSettings.getLineStyle().enumerator()));
    i_pPainter->setPen(pn);
    i_pPainter->setRenderHints(s_painterRenderHints);
    i_pPainter->drawLine(lineF);

    ////#pragma message(__TODO__"To be removed")
    ////i_pPainter->setPen(Qt::red);
    ////QLineF lineCenterHor(lineF.center().x()-5, lineF.center().y(), lineF.center().x()+5, lineF.center().y());
    ////QLineF lineCenterVer(lineF.center().x(), lineF.center().y()-5, lineF.center().x(), lineF.center().y()+5);
    ////i_pPainter->drawLine(lineCenterHor);
    ////i_pPainter->drawLine(lineCenterVer);
    ////#pragma message(__TODO__"To be removed")

    CEnumLineEndStyle lineEndStyleP1 = m_drawSettings.getLineEndStyle(ELinePoint::Start);
    CEnumLineEndStyle lineEndStyleP2 = m_drawSettings.getLineEndStyle(ELinePoint::End);
    if (lineEndStyleP1 != ELineEndStyle::Normal || lineEndStyleP2 != ELineEndStyle::Normal) {
        CEnumArrowHeadBaseLineType baseLineTypeP1 = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::Start);
        CEnumArrowHeadBaseLineType baseLineTypeP2 = m_drawSettings.getArrowHeadBaseLineType(ELinePoint::End);
        pn.setWidth(1);
        pn.setStyle(Qt::SolidLine);
        i_pPainter->setPen(pn);
        QBrush brsh(pn.color());
        if (lineEndStyleP1 != ELineEndStyle::Normal) {
            brsh.setStyle(arrowHeadFillStyle2QtBrushStyle(m_drawSettings.getArrowHeadFillStyle(ELinePoint::Start)));
            i_pPainter->setBrush(brsh);
            if (baseLineTypeP1 == EArrowHeadBaseLineType::NoLine) {
                i_pPainter->drawPolyline(m_plgP1ArrowHead);
            }
            else {
                if (m_drawSettings.getArrowHeadFillStyle(ELinePoint::Start) == EArrowHeadFillStyle::NoFill) {
                    i_pPainter->setBrush(Qt::white);
                }
                else {
                    i_pPainter->setBrush(Qt::black);
                }
                i_pPainter->drawPolygon(m_plgP1ArrowHead);
            }
        }
        if (lineEndStyleP2 != ELineEndStyle::Normal) {
            brsh.setStyle(arrowHeadFillStyle2QtBrushStyle(m_drawSettings.getArrowHeadFillStyle(ELinePoint::End)));
            i_pPainter->setBrush(brsh);
            if (baseLineTypeP2 == EArrowHeadBaseLineType::NoLine) {
                i_pPainter->drawPolyline(m_plgP2ArrowHead);
            }
            else {
                if (m_drawSettings.getArrowHeadFillStyle(ELinePoint::End) == EArrowHeadFillStyle::NoFill) {
                    i_pPainter->setBrush(Qt::white);
                }
                else {
                    i_pPainter->setBrush(Qt::black);
                }
                i_pPainter->drawPolygon(m_plgP2ArrowHead);
            }
        }
    }
    i_pPainter->restore();
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

////------------------------------------------------------------------------------
///*! @brief Watches mouse events of the selection points.
//
//    A mouse press event on one of the selection points selects the line.
//
//    If the line is under construction by mouse events a mouse release event
//    in selection point of the line end finishes the creation of the line.
//*/
//bool CGraphObjLine::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjSceneEventFilter, EMethodTraceDetailLevel::ArgsNormal)) {
//        CGraphObj* pGraphObjWatched = dynamic_cast<CGraphObj*>(i_pGraphicsItemWatched);
//        strMthInArgs = "ItemWatched: " + QString(pGraphObjWatched == nullptr ? "null" : pGraphObjWatched->path());
//        if (isGraphicsSceneHoverEvent(i_pEv)) {
//            QGraphicsSceneHoverEvent* pHoverEvent = dynamic_cast<QGraphicsSceneHoverEvent*>(i_pEv);
//            strMthInArgs += ", Ev {" + qEventType2Str(i_pEv->type()) + ", " + qGraphicsSceneHoverEvent2Str(pHoverEvent) + "}";
//        }
//        else if (isGraphicsSceneMouseEvent(i_pEv)) {
//            QGraphicsSceneMouseEvent* pMouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
//            strMthInArgs += ", Ev {" + qEventType2Str(i_pEv->type()) + ", " + qGraphicsSceneMouseEvent2Str(pMouseEvent) + "}";
//        }
//        else {
//            strMthInArgs += ", Ev {" + qEvent2Str(i_pEv) + "}";
//        }
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjSceneEventFilter,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "sceneEventFilter",
//        /* strAddInfo   */ strMthInArgs );
//
//    CGraphObjSelectionPoint* pGraphObjSelPtWatched = dynamic_cast<CGraphObjSelectionPoint*>(i_pGraphicsItemWatched);
//    if (pGraphObjSelPtWatched == nullptr) {
//        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "pGraphObjSelPtWatched == nullptr");
//    }
//
//    SGraphObjSelectionPoint selPt = pGraphObjSelPtWatched->getSelectionPoint();
//
//    bool bEventHandled = false;
//    if (i_pEv->type() == QEvent::GraphicsSceneMousePress) {
//        setSelected(true);
//        bEventHandled = true;
//    }
//    else if (i_pEv->type() == QEvent::GraphicsSceneMouseRelease) {
//        if (m_editMode == EEditMode::CreatingByMouseEvents && selPt.m_idxPt == 1) {
//            setEditMode(EEditMode::None);
//            setSelected(false);
//            bEventHandled = true;
//        }
//    }
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
//        mthTracer.setMethodReturn(bEventHandled);
//    }
//    return bEventHandled;
//
//} // sceneEventFilter

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
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

    // Ignore hover events if the object is currently being created.
    if (m_editMode != EEditMode::CreatingByMouseEvents) {
        //showSelectionPoints();
        //setIsHit(true);
        //QGraphicsItem_setCursor(getProposedCursor(i_pEv->pos()));
        QGraphicsItem_setCursor(Qt::SizeAllCursor);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjLine::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
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

    // Ignore hover events if the object is currently being created.
    if (m_editMode != EEditMode::CreatingByMouseEvents) {
        //showSelectionPoints();
        //setIsHit(true);
        //QGraphicsItem_setCursor(getProposedCursor(i_pEv->pos()));
        QGraphicsItem_setCursor(Qt::SizeAllCursor);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjLine::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
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

    // If the object is selected the selection points should remain visible.
    //if (!isSelected()) {
    //    hideSelectionPoints();
    //}
    //setIsHit(false);
    QGraphicsItem_unsetCursor();

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
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
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }
    bool bEventHandled = false;
    if (m_editMode == EEditMode::None) {
        if (i_pEv->button() == Qt::RightButton) {
            showContextMenu(i_pEv->screenPos());
            bEventHandled = true;
        }
    }
    if (!bEventHandled) {
        // Forward the mouse event to the base implementation.
        // This will select the item, creating selection points if not yet created.
        QGraphicsLineItem::mousePressEvent(i_pEv);
    }
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjLine::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
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
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    //// The mouse release event would select the object.
    //// This is not wanted if the selection tool is not active.
    //bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
    //bool bIsSelectableReset = false;

    //if (bIsSelectable /*&& m_pDrawingScene->getEditTool() != EEditTool::Select*/) {
    //    setFlag(QGraphicsItem::ItemIsSelectable, false);
    //    bIsSelectableReset = true;
    //}

    // Forward the mouse event to the LineItems base implementation.
    QGraphicsLineItem::mouseReleaseEvent(i_pEv);

    //if (bIsSelectableReset) {
    //    setFlag(QGraphicsItem::ItemIsSelectable, bIsSelectable);
    //}

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjLine::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
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
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    // When double clicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".
    //QGraphicsLineItem::mouseDoubleClickEvent(i_pEv);

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
    }
}

//------------------------------------------------------------------------------
void CGraphObjLine::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
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
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Enter);
        traceGraphObjStates(mthTracer, EMethodDir::Enter);
    }

    // Forward the mouse event to the LineItems base implementation.
    // This will move the item resulting in an itemChange call with PositionHasChanged.
    QGraphicsLineItem::mouseMoveEvent(i_pEv);

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceGraphicsItemStates(mthTracer, EMethodDir::Leave);
        traceGraphObjStates(mthTracer, EMethodDir::Leave);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs("Ev {Accepted: " + bool2Str(i_pEv->isAccepted())+ "}");
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
QVariant CGraphObjLine::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
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
            // Not necessary to bring selection points to front as item has been already brought
            // to front and "showSelectionPoints" sets zValue of selection points above item.
            //bringSelectionPointsToFront();
        }
        else {
            hideSelectionPoints();
            resetStackingOrderValueToOriginalValue(); // restore ZValue as before selecting the object
            //m_editMode = EEditMode::None;
            //m_editResizeMode = EEditResizeMode::None;
            //m_selPtSelectedBoundingRect = ESelectionPoint::None;
            //m_idxSelPtSelectedPolygon = -1;
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
//void CGraphObjLine::onDrawingSizeChanged(const CDrawingSize& i_drawingSize)
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
void CGraphObjLine::onGraphObjParentGeometryOnSceneChanged(
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
            CPhysValLine physValLine = getPhysValLineScaled(m_physValLineOrig);
            setPhysValLineScaled(physValLine);
            //physValLine.setAngle(m_physValRotationAngle);
            setPhysValLineScaledAndRotated(physValLine);

            QPointF ptPosPrev = pos();

            QLineF lineF;
            QPointF ptPos = getItemPosAndLocalCoors(physValLine, lineF);

            // Prepare the item for a geometry change. This function must be called before
            // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
            QGraphicsItem_prepareGeometryChange();

            {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdatePhysValCoorsBlockedCounter);
                CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

                // Set the line in local coordinate system.
                // Also note that itemChange must not overwrite the current coordinates (refCountGuard).
                QGraphicsLineItem_setLine(lineF);

                // Please note that GraphicsLineItem::setLine did not update the position of the
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
void CGraphObjLine::onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint)
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
    if (selPt.m_selPtType == ESelectionPointType::PolygonPoint) {
        if (selPt.m_idxPt == 0) {
            setP1(physValParentSelPt);
        }
        else if (selPt.m_idxPt == 1) {
            setP2(physValParentSelPt);
        }
    }
}

/*==============================================================================
protected: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the pure virtual method of base class CGraphObj.
*/
void CGraphObjLine::updateTransformedCoorsOnParentChanged(
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

        setLineOrig(line());
        CPhysValLine physValLine = getPhysValLineOrig(m_lineOrig);
        setPhysValLineOrig(physValLine);
        physValLine = getPhysValLineScaled(m_physValLineOrig);
        setPhysValLineScaled(physValLine);
        setPhysValLineScaledAndRotated(physValLine);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
    emit_geometryOnSceneChanged(true);
}

//------------------------------------------------------------------------------
/*! @brief Overrides the pure virtual method of base class CGraphObj.
*/
void CGraphObjLine::updateTransformedCoorsOnParentGeometryChanged()
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

        setLineOrig(line());
        CPhysValLine physValLine = getPhysValLineOrig(m_lineOrig);
        setPhysValLineOrig(physValLine);
        physValLine = getPhysValLineScaled(m_physValLineOrig);
        setPhysValLineScaled(physValLine);
        //physValLine.setAngle(m_physValRotationAngle);
        setPhysValLineScaledAndRotated(physValLine);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
    emit_geometryOnSceneChanged(true);
}

//------------------------------------------------------------------------------
/*! @brief Overrides the pure virtual method of base class CGraphObj.
*/
void CGraphObjLine::updateTransformedCoorsOnItemPositionChanged()
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

        #if 0
        //QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
        //QLineF lineF = line();
        //double fRotationAngle_degree = m_physValRotationAngle.getVal(Units.Angle.Degree);
        //if (fRotationAngle_degree != 0.0) {
        //    QGraphicsItem_setRotation(0.0);
        //}
        //if (parentGroup() != nullptr) {
        //    QPointF pt1 = pGraphicsItemThis->mapToParent(lineF.p1());
        //    QPointF pt2 = pGraphicsItemThis->mapToParent(lineF.p2());
        //    pt1 = parentGroup()->mapToTopLeftOfBoundingRect(pt1);
        //    pt2 = parentGroup()->mapToTopLeftOfBoundingRect(pt2);
        //    CPhysValPoint physValPointP1 = parentGroup()->convert(pt1);
        //    CPhysValPoint physValPointP2 = parentGroup()->convert(pt2);
        //    CPhysValLine physValLine(physValPointP1, physValPointP2);
        //    setPhysValLineOrig(physValLine);
        //    setPhysValLineScaled(physValLine);
        //    setPhysValLineScaledAndRotated(physValLine);
        //}
        //else {
        //    // Please note that "mapToScene" maps the local coordinates relative to the
        //    // top left corner of the item's bounding rectangle and there is no need to
        //    // call "mapToBoundingRectTopLeft" beforehand.
        //    QPointF pt1 = pGraphicsItemThis->mapToScene(lineF.p1());
        //    QPointF pt2 = pGraphicsItemThis->mapToScene(lineF.p2());
        //    CPhysValPoint physValPointP1 = m_pDrawingScene->convert(pt1);
        //    CPhysValPoint physValPointP2 = m_pDrawingScene->convert(pt2);
        //    CPhysValLine physValLine(physValPointP1, physValPointP2);
        //    setPhysValLineOrig(physValLine);
        //    setPhysValLineScaled(physValLine);
        //    setPhysValLineScaledAndRotated(physValLine);
        //}
        //if (fRotationAngle_degree != 0.0) {
        //    QGraphicsItem_setRotation(fRotationAngle_degree);
        //}
        #else
        CPhysValLine physValLine = getPhysValLineOrig(m_lineOrig);
        setPhysValLineOrig(physValLine);
        physValLine = getPhysValLineScaled(m_physValLineOrig);
        setPhysValLineScaled(physValLine);
        //physValLine.setAngle(m_physValRotationAngle);
        setPhysValLineScaledAndRotated(physValLine);
        #endif
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
    emit_geometryOnSceneChanged();
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjLine::lineEndArrowHeadPolygonsNeedUpdate(
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
void CGraphObjLine::updateLineEndArrowHeadPolygons(const CEnumLinePoint& i_linePoint)
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

    QLineF lineF = line();
    if (!i_linePoint.isValid() || i_linePoint == ELinePoint::Start) {
        CEnumLineEndStyle lineEndStyle = m_drawSettings.getLineEndStyle(ELinePoint::Start);
        if (lineEndStyle != ELineEndStyle::Normal) {
            getLineEndArrowPolygons(
                /* line          */ lineF,
                /* drawSetings   */ m_drawSettings,
                /* pplgLineStart */ &m_plgP1ArrowHead,
                /* pplgLineEnd   */ nullptr);
        }
    }
    if (!i_linePoint.isValid() || i_linePoint == ELinePoint::End) {
        CEnumLineEndStyle lineEndStyle = m_drawSettings.getLineEndStyle(ELinePoint::End);
        if (lineEndStyle != ELineEndStyle::Normal) {
            getLineEndArrowPolygons(
                /* line          */ lineF,
                /* drawSetings   */ m_drawSettings,
                /* pplgLineStart */ nullptr,
                /* pplgLineEnd   */ &m_plgP2ArrowHead);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Calculates the scaled, not rotated line in pixels in item coordinates
           relative to the origin (center point) of the orignal bounding rectangle.

    The relative distance of the center point of the scaled line to the
    origin (center point) of the parent's bounding rectangle remains the same.
    The length is scaled to the scale factors of the parent group.

    @param [in] i_lineOrig
        Unscaled line in the item's local coordinates system whose origin
        is the center point of the item.
    @return Scaled line.
*/
QLineF CGraphObjLine::getLineScaled(const QLineF& i_lineOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qLine2Str(i_lineOrig) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getLineScaled",
        /* strAddInfo   */ strMthInArgs );

    QPointF ptCenter(m_fParentGroupScaleX * i_lineOrig.center().x(), m_fParentGroupScaleY * i_lineOrig.center().y());
    double fDX_px = m_fParentGroupScaleX * i_lineOrig.dx();
    double fDY_px = m_fParentGroupScaleY * i_lineOrig.dy();
    QPointF p1(ptCenter.x() - fDX_px/2.0, ptCenter.y() - fDY_px/2.0);
    QPointF p2(ptCenter.x() + fDX_px/2.0, ptCenter.y() + fDY_px/2.0);
    QLineF lineF(p1, p2);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qLine2Str(lineF) + "}");
    }
    return lineF;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the unscaled and not rotated physical line in the
           current unit of the drawing scene relative to the top left or
           bottom left corner of the parent's bounding rectangle.

    @param [in] i_lineOrig
        Unscaled, not rotated line in the item's local coordinates system
        whose origin is the center point of the item.
    @return Physical line whose origin is either the top left or bottom
            left corner of the parent's bounding rectangle.
*/
CPhysValLine CGraphObjLine::getPhysValLineOrig(const QLineF& i_lineOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qLine2Str(i_lineOrig) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValLineOrig",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    CGraphObjLine* pVThis = const_cast<CGraphObjLine*>(this);
    double fRotationAngle_degree = m_physValRotationAngle.getVal(Units.Angle.Degree);
    if (fRotationAngle_degree != 0.0) {
        pVThis->QGraphicsItem_setRotation(0.0);
    }
    CPhysValPoint physValPointP1(*m_pDrawingScene);
    CPhysValPoint physValPointP2(*m_pDrawingScene);
    if (parentGroup() != nullptr) {
        QPointF p1 = pGraphicsItemThis->mapToParent(i_lineOrig.p1());
        p1 = parentGroup()->mapToTopLeftOfBoundingRect(p1);
        physValPointP1 = parentGroup()->convert(p1);
        QPointF p2 = pGraphicsItemThis->mapToParent(i_lineOrig.p2());
        p2 = parentGroup()->mapToTopLeftOfBoundingRect(p2);
        physValPointP2 = parentGroup()->convert(p2);
    }
    else {
        // Please note that "mapToScene" maps the local coordinates relative to the
        // top left corner of the item's bounding rectangle and there is no need to
        // call "mapToBoundingRectTopLeft" beforehand.
        QPointF p1 = pGraphicsItemThis->mapToScene(i_lineOrig.p1());
        physValPointP1 = m_pDrawingScene->convert(p1);
        QPointF p2 = pGraphicsItemThis->mapToScene(i_lineOrig.p2());
        physValPointP2 = m_pDrawingScene->convert(p2);
    }
    if (fRotationAngle_degree != 0.0) {
        pVThis->QGraphicsItem_setRotation(fRotationAngle_degree);
    }

    CPhysValLine physValLine(physValPointP1, physValPointP2);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValLine.toString() + "} " + physValLine.unit().symbol());
    }
    return physValLine;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the scaled, not rotated physical line in the current
           unit of the drawing scene relative to the top left or bottem left
           corner of the parent's bounding rectangle.

    The relative distance of the center point of the scaled line to the
    top left or bottom left corner of the parent's bounding rectangle remains the same.
    The length is scaled to the scale factors of the parent group.

    @param [in] i_physValLineOrig
        Unscaled line in the parent's, physical coordinates system whose origin
        is the top left or bottom right corner of the parent's bounding rectangle.
    @return Scaled line.
*/
CPhysValLine CGraphObjLine::getPhysValLineScaled(const CPhysValLine& i_physValLineOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValLineOrig.toString() + "} " + i_physValLineOrig.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValLineScaled",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPoint physValPointCenter(*m_pDrawingScene);
    physValPointCenter.setX(m_fParentGroupScaleX * i_physValLineOrig.center().x().getVal());
    physValPointCenter.setY(m_fParentGroupScaleY * i_physValLineOrig.center().y().getVal());
    double fDX = m_fParentGroupScaleX * i_physValLineOrig.dx().getVal();
    double fDY = m_fParentGroupScaleY * i_physValLineOrig.dy().getVal();
    CPhysValPoint physValPointP1(*m_pDrawingScene);
    physValPointP1.setX(physValPointCenter.x().getVal() - fDX/2.0);
    physValPointP1.setY(physValPointCenter.y().getVal() - fDY/2.0);
    CPhysValPoint physValPointP2(*m_pDrawingScene);
    physValPointP2.setX(physValPointCenter.x().getVal() + fDX/2.0);
    physValPointP2.setY(physValPointCenter.y().getVal() + fDY/2.0);

    CPhysValLine physValLine(physValPointP1, physValPointP2);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValLine.toString() + "} " + physValLine.unit().symbol());
    }
    return physValLine;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the item position relative to the parent item or drawing scene
           as well as the item coordinates local coordinates.

    @param [in] i_physValLine
        Line in parent coordinates, depending on the Y scale orientation
        relative to the top left or bottom left corner of parent item's bounding
        rectangle. If the item belongs to a parent group the passed line
        must have been resized and the center must have been moved according to the
        parents scale factors.
    @param [out] o_rectF
        Rectangle coordinates in local coordinates.
    @param [out] o_physValAngle
        The rotatian angle of the passed rectangle.
*/
QPointF CGraphObjLine::getItemPosAndLocalCoors(
    const CPhysValLine& i_physValLine, QLineF& o_line) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getItemPosAndLocalCoors",
        /* strAddInfo   */ strMthInArgs );

    // First determine the position of the item in the parent's (scene or group) coordinate system.
    CPhysValLine physValLineTmp(i_physValLine);
    if (parentGroup() != nullptr) {
        physValLineTmp = parentGroup()->convert(physValLineTmp, Units.Length.px);
    }
    else {
        physValLineTmp = m_pDrawingScene->convert(physValLineTmp, Units.Length.px);
    }
    o_line = QLineF(physValLineTmp.p1().toQPointF(), physValLineTmp.p2().toQPointF());

    // Transform the parent coordinates into local coordinate system.
    // The origin is the center point of the line.
    QPointF ptPos = o_line.center(); // line here still in parent or scene coordinates
    QPointF p1 = o_line.p1() - ptPos;
    QPointF p2 = o_line.p2() - ptPos;
    o_line = QLineF(p1, p2); // line now in local coordinates

    if (parentGroup() != nullptr) {
        ptPos = parentGroup()->mapFromTopLeftOfBoundingRect(ptPos);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "Line {" + qLine2Str(o_line) + "}";
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn("{" + qPoint2Str(ptPos) + "}");
    }
    return ptPos;
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Set the original, untransformed (not scaled, not rotated) line
           coordinates in local coordinates relative to the origin of the
           line's bounding rectangle

    @param [in] i_line
        Coordinates in local coordinates to be set.

    @return Previous original coordinates.
*/
QLineF CGraphObjLine::setLineOrig(const QLineF& i_line)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + qLine2Str(i_line) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setLineOrig",
        /* strAddInfo   */ strMthInArgs );

    QLineF linePrev = m_lineOrig;
    m_lineOrig = i_line;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + qLine2Str(linePrev) + "}");
    }
    return linePrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated line coordinates in local
           coordinates relative to the origin of the item's bounding rectangle.

    @param [in] i_line
        Coordinates to be set.

    @return Previous coordinates.
*/
QLineF CGraphObjLine::QGraphicsLineItem_setLine(const QLineF& i_line)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + qLine2Str(i_line) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsLineItem_setLine",
        /* strAddInfo   */ strMthInArgs );

    QLineF linePrev = QGraphicsLineItem::line();
    QGraphicsLineItem::setLine(i_line);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + qLine2Str(linePrev) + "}");
    }
    return linePrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated line coordinates in local
           coordinates relative to the origin of the item's bounding rectangle.

    @param [in] i_fX1, i_fY1, i_fX2, i_fY2
        Line coordinates to be set.

    @return Previous line coordinates.
*/
QLineF CGraphObjLine::QGraphicsLineItem_setLine(double i_fX1, double i_fY1, double i_fX2, double i_fY2)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + qLine2Str(QLineF(i_fX1, i_fY1, i_fX2, i_fY2)) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsLineItem_setLine",
        /* strAddInfo   */ strMthInArgs );

    QLineF linePrev = QGraphicsLineItem::line();
    QGraphicsLineItem::setLine(i_fX1, i_fY1, i_fX2, i_fY2);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qLine2Str(linePrev));
    }
    return linePrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the original, untransformed (not scaled, not rotated) line
           coordinates with unit in parent coordinates relative to the top left
           or bottom left corner of the parent.

    @param [in] i_physValLine
        Coordinates to be set.

    @return Previous original line coordinates.
*/
CPhysValLine CGraphObjLine::setPhysValLineOrig(const CPhysValLine& i_physValLine)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValLineOrig",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLinePrev = m_physValLineOrig;
    m_physValLineOrig = i_physValLine;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValLinePrev.toString() + "} " + physValLinePrev.unit().symbol());
    }
    return physValLinePrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated line coordinates with unit in
           parent coordinates relative to the top left or bottom left corner of
           the parent.

    @param [in] i_physValLine
        Line coordinates relative to the parent (or scene) to be set.

    @return Previous line coordinates.
*/
CPhysValLine CGraphObjLine::setPhysValLineScaled(const CPhysValLine& i_physValLine)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValLineScaled",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLinePrev = m_physValLineScaled;
    m_physValLineScaled = i_physValLine;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValLinePrev.toString() + "} " + physValLinePrev.unit().symbol());
    }
    return physValLinePrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled and rotated line coordinates with unit in parent
           coordinates relative to the top left or bottom left corner of the parent.

    @param [in] i_physValLine
        Line coordinates relative to the parent (or scene) to be set.

    @return Previous line coordinates.
*/
CPhysValLine CGraphObjLine::setPhysValLineScaledAndRotated(const CPhysValLine& i_physValLine)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValLineScaledAndRotated",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLinePrev = m_physValLineScaledAndRotated;
    m_physValLineScaledAndRotated = i_physValLine;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValLinePrev.toString() + "} " + physValLinePrev.unit().symbol());
    }
    return physValLinePrev;
}

//------------------------------------------------------------------------------
/*! @brief Internal method reimplementing the prepareGeometryChange method of
           graphics item to trace the method call.

    As the prepareGeometryChange method is a protected method of QGraphicsItem
    this method must be reimplemented by the derived classes.
*/
void CGraphObjLine::QGraphicsItem_prepareGeometryChange()
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
void CGraphObjLine::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir,
    const QString& i_strFilter, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }
    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        QLineF lineF = line();
        QString strRuntimeInfo;
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "PhysValLine Orig {" + m_physValLineOrig.toString() + "} " + m_physValLineOrig.unit().symbol()
            + ", Scaled {" + m_physValLineScaled.toString() + "} " + m_physValLineScaled.unit().symbol()
            + ", ScaledRotated {" + m_physValLineScaledAndRotated.toString() + "} " + m_physValLineScaledAndRotated.unit().symbol();
        i_mthTracer.trace(strRuntimeInfo);
        if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
        else strRuntimeInfo = "   ";
        strRuntimeInfo += "ItemLine Orig {" + qLine2Str(m_lineOrig) + "}, Scaled {" + qLine2Str(line()) + "}";
        i_mthTracer.trace(strRuntimeInfo);
        CGraphObj::traceThisPositionInfo(i_mthTracer, i_mthDir, i_strFilter, i_detailLevel);
    }
}
