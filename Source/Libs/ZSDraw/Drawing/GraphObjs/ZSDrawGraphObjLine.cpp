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
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLinePropertiesDlg.h"
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
class CGraphObjLine : public CGraphObj, public QGraphicsLineItem
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QString CGraphObjLine::c_strGeometryLabelNameP1 = "P1";
const QString CGraphObjLine::c_strGeometryLabelNameP2 = "P2";
const QString CGraphObjLine::c_strGeometryLabelNameCenter = "Center";
const QString CGraphObjLine::c_strGeometryLabelNameDX = "dX";
const QString CGraphObjLine::c_strGeometryLabelNameDY = "dY";
const QString CGraphObjLine::c_strGeometryLabelNameLength = "Length";
const QString CGraphObjLine::c_strGeometryLabelNameAngle = "Angle";

/*==============================================================================
public: // class members
==============================================================================*/

qint64 CGraphObjLine::s_iInstCount = 0;

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
    m_physValLineOrig(*m_pDrawingScene),
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_strlstPredefinedLabelNames.append(c_strLabelName);
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameP1);
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameP2);

    for (const QString& strLabelName : m_strlstPredefinedLabelNames) {
        if (!m_hshpLabels.contains(strLabelName)) {
            if (strLabelName == c_strGeometryLabelNameP1) {
                addLabel(strLabelName, strLabelName, 0);
            }
            else if (strLabelName == c_strGeometryLabelNameP2) {
                addLabel(strLabelName, strLabelName, 1);
            }
            else {
                addLabel(strLabelName, strLabelName, ESelectionPoint::Center);
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

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges
           | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton | Qt::XButton1 | Qt::XButton2);
    setAcceptHoverEvents(true);

} // ctor

//------------------------------------------------------------------------------
CGraphObjLine::~CGraphObjLine()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    emit_aboutToBeDestroyed();

    //m_physValLineOrig;
    //m_plgP1ArrowHead;
    //m_plgP2ArrowHead;

} // dtor

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
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CGraphObjLine* pGraphObjLine = new CGraphObjLine(m_pDrawingScene, m_strName);
    pGraphObjLine->setLine(getLine(drawingSize.unit()));
    pGraphObjLine->setDrawSettings(m_drawSettings);
    return pGraphObjLine;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjLine::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QLineF lineF = line();
    QPolygonF plgScene;
    plgScene.append(pGraphicsItemThis->mapToScene(lineF.p1()));
    plgScene.append(pGraphicsItemThis->mapToScene(lineF.p2()));
    return qPolygon2Str(plgScene);
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/* @brief

    Must be overridden to create a user defined dialog.
*/
void CGraphObjLine::onCreateAndExecDlgFormatGraphObjs()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onCreateAndExecDlgFormatGraphObjs",
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
        /* strObjName   */ m_strName,
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLine",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    QPointF ptPosPrev = pos();

    // Depending on the Y scale orientation of the drawing scene the line coordinates
    // have been passed either relative to the top left or bottom right corner of the
    // parent item's bounding rectangle.
    // The coordinates need to be transformed into the local coordinate system of the graphical
    // object whose origin point is the center of the objects bounding rectangle.
    // First determine the position of the line in the parent's (scene or group) coordinate system.
    QLineF lineF;
    if (parentGroup() != nullptr) {
        lineF = parentGroup()->convert(i_physValLine, Units.Length.px).toQLineF();
    }
    else {
        lineF = m_pDrawingScene->convert(i_physValLine, Units.Length.px).toQLineF();
    }

    // Transform the parent coordinates into local coordinate system.
    // The origin is the center point of the line.
    QPointF ptPos = lineF.center();
    QPointF pt1 = lineF.p1() - ptPos;
    QPointF pt2 = lineF.p2() - ptPos;
    lineF = QLineF(pt1, pt2);

    if (parentGroup() != nullptr) {
        ptPos = parentGroup()->mapFromTopLeftOfBoundingRect(ptPos);
    }

    // If the coordinates MUST be updated (e.g. after the drawing size has been changed)
    // or if the coordinates have been changed ...
    if (m_physValLineOrig.isNull() || m_physValLineOrig != i_physValLine/* || m_bForceConversionToSceneCoors*/)
    {
        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsLineItem::prepareGeometryChange();

        {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdateOriginalCoorsBlockedCounter);
            CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryChangedSignalBlockedCounter);

            // Store original line coordinates.
            setLineOrig(i_physValLine);

            // Set the line in local coordinate system.
            // Also note that itemChange must not overwrite the current line value (refCountGuard).
            QGraphicsLineItem_setLine(lineF);

            // Please note that GraphicsLineItem::setLine did not update the position of the
            // item in the parent. This has to be done "manually" afterwards.

            // Move the object to the parent position.
            // This has to be done after resizing the line which updates the local coordinates
            // of the line with origin (0/0) at the lines center point.
            // "setPos" will trigger an itemChange call which will update the position of the
            // selection points and labels. To position the selection points and labels correctly
            // the local coordinate system must be up-to-date.
            // Also note that itemChange must not overwrite the current line value (refCountGuard).
            // If the position is not changed, itemChange is not called with PositionHasChanged and
            // the position of the arrow heads will not be updated. We got to do this here "manually".
            if (ptPos != ptPosPrev) {
                QGraphicsItem_setPos(ptPos);
            }
            else {
                updateLineEndArrowHeadPolygons();
            }
        }
        emit_geometryChanged();
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
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
    @param [in] i_fRes
        Resolution of the line coordinates.
    @param [in] i_unit
        Unit in which the coordinates are passed.
*/
void CGraphObjLine::setLine(
    double i_fX1, double i_fY1, double i_fX2, double i_fY2, double i_fRes, const CUnit& i_unit)
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
        /* strObjName   */ m_strName,
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
    @param [in] i_fRes
        Resolution of the line coordinates.
    @param [in] i_unit
        Unit in which the coordinates are passed.
*/
void CGraphObjLine::setLine(
    const QPointF& i_p1, const QPointF& i_p2, double i_fRes, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "P1 {" + qPoint2Str(i_p1) + "}, P2 {" + qPoint2Str(i_p2) + "} " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
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
    @param [in] i_fRes
        Resolution of the line coordinates.
    @param [in] i_unit
        Unit in which the coordinates are passed.
*/
void CGraphObjLine::setLine(const QLineF& i_line, double i_fRes, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Line {" + qLine2Str(i_line) + "}, Res: " + QString::number(i_fRes) + " " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
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
        /* strObjName   */ m_strName,
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
    CPhysValLine physValLine = m_physValLineOrig;
    #pragma message(__TODO__"Take transformation into account")
    return physValLine;
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
        /* strObjName   */ m_strName,
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
        /* strObjName   */ m_strName,
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
        /* strObjName   */ m_strName,
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
        /* strObjName   */ m_strName,
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
        /* strObjName   */ m_strName,
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
        /* strObjName   */ m_strName,
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
        /* strObjName   */ m_strName,
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
        /* strObjName   */ m_strName,
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

//------------------------------------------------------------------------------
/*! @brief Sets the clockwise rotation angle, in degrees, around the z axis.

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
        /* strObjName   */ m_strName,
        /* strMethod    */ "setAngle",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLine = getLine();
    physValLine.setAngle(i_physValAngle);
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's angle in degrees.

    The return value will be in the range of values from 0.0 up to but not
    including 360.0.

    @sa setAngle()
*/
CPhysVal CGraphObjLine::rotationAngle() const
//------------------------------------------------------------------------------
{
    return rotationAngle(Units.Angle.Degree);
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
/*! @brief Returns the current bounding rectangle of the object in local coordinates.

    @return Bounding rectangle in local coordinates.
*/
QRectF CGraphObjLine::getBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ "" );

    // Line points in local coordinates.
    QLineF lineF = line();
    QRectF rctBounding(lineF.p1(), lineF.p2());
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Called by the parent group if it's bounding rectangle has been changed and
           the groups physical origin point (top left or bottom left corner) may have
           been changed.

    The childs must update their original shape point coordinates. This is also necessary
    if the groups position and also the childs position (in graphics item pixell coordinates)
    have not been changed.

    @param [in] i_rctBoundingNew
    @param [in] i_rctBoundingPrev
*/
void CGraphObjLine::updateOriginalPhysValCoors()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateOriginalPhysValCoors",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    QLineF lineF = line();
    // Before mapping to parent or scene, the rotation will be reset.
    // Otherwise transformed coordinates will be returned.
    // And itemChange is called but should not emit the geometryChanged signal ..
    CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryChangedSignalBlockedCounter);
    QGraphicsItem_setRotation(0.0);
    if (parentGroup() != nullptr) {
        QPointF pt1 = pGraphicsItemThis->mapToParent(lineF.p1());
        QPointF pt2 = pGraphicsItemThis->mapToParent(lineF.p2());
        pt1 = parentGroup()->mapToTopLeftOfBoundingRect(pt1);
        pt2 = parentGroup()->mapToTopLeftOfBoundingRect(pt2);
        CPhysValPoint physValPointP1 = parentGroup()->convert(pt1);
        CPhysValPoint physValPointP2 = parentGroup()->convert(pt2);
        setLineOrig(CPhysValLine(physValPointP1, physValPointP2));
    }
    else {
        // Please note that "mapToScene" maps the local coordinates relative to the
        // top left corner of the item's bounding rectangle and there is no need to
        // call "mapToBoundingRectTopLeft" beforehand.
        QPointF pt1 = pGraphicsItemThis->mapToScene(lineF.p1());
        QPointF pt2 = pGraphicsItemThis->mapToScene(lineF.p2());
        CPhysValPoint physValPointP1 = m_pDrawingScene->convert(pt1);
        CPhysValPoint physValPointP2 = m_pDrawingScene->convert(pt2);
        setLineOrig(CPhysValLine(physValPointP1, physValPointP2));
    }
    QGraphicsItem_setRotation(m_physValRotationAngle.getVal(Units.Angle.Degree));
}

////------------------------------------------------------------------------------
///*! @brief Returns the bounding rectangle of the object in local coordinates.
//
//    This method is used by a group to resize its children.
//
//    Please refer to documentation about the difference between current (transformed)
//    and original coordinates at base class CGraphObj.
//
//    @return Original bounding rectangle in local coordinates.
//*/
//QRectF CGraphObjLine::getOriginalBoundingRectInParent() const
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "getOriginalBoundingRectInParent",
//        /* strAddInfo   */ "" );
//
//    // Line points in local coordinates.
//    QLineF lineF = line();
//    QGraphicsItem* pGraphicsItemParent = parentItem();
//    CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(pGraphicsItemParent);
//    // If the line belongs to a group ...
//    if (pGraphObjParent != nullptr) {
//        lineF = m_physValLineOrig.toQLineF();
//        // The original coordinates in m_physValLineOrig are relative to the
//        // top left corner of the parent's bounding rectangle.
//        QPointF pt1 = lineF.p1();
//        QPointF pt2 = lineF.p2();
//        // The coordinates must be mapped to local coordinates.
//        QRectF rctBoundingParent = pGraphObjParent->getOriginalBoundingRectInParent();
//        QPointF ptOriginParent = rctBoundingParent.topLeft();
//        pt1 += ptOriginParent;
//        pt2 += ptOriginParent;
//        lineF = QLineF(pt1, pt2);
//    }
//    // If the item does not belong to a group ...
//    else {
//        // .. the current (transformed) and original coordinates must be the same.
//        // lineF already contains the local coordinates.
//    }
//    QRectF rctBounding = ZS::Draw::boundingRect(lineF);
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
//    }
//    return rctBounding;
//}

////------------------------------------------------------------------------------
///*! @brief Pure virtual method which must be overridden by derived classes to
//           set the bounding rectangle of the object in local coordinates.
//
//    This method is used by a group to resize its children.
//
//    P1 and P2 should not be swapped when invoking this method.
//
//    If P1 is right of P2, P1 should stay right of P1.
//    If P1 is below of P2, P1 should stay below of P1.
//
//    @param [in] i_rectBounding
//        New bounding rectangle of the object in local coordinates.
//*/
//void CGraphObjLine::setCurrentBoundingRectInParent(const QRectF& i_rectBounding)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = qRect2Str(i_rectBounding);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setCurrentBoundingRectInParent",
//        /* strAddInfo   */ strMthInArgs );
//
//    double fXL = i_rectBounding.left();
//    double fYT = i_rectBounding.top();
//    double fXR = i_rectBounding.right();
//    double fYB = i_rectBounding.bottom();
//
//    QLineF lineF = line();
//    QPointF pt1 = lineF.p1();
//    QPointF pt2 = lineF.p2();
//
//    if (pt1.x() < pt2.x()) {
//        pt1.setX(i_rectBounding.left());
//        pt2.setX(i_rectBounding.right());
//    }
//    else {
//        pt1.setX(i_rectBounding.right());
//        pt2.setX(i_rectBounding.left());
//    }
//    if (lineF.y1() < lineF.y2()) {
//        pt1.setY(i_rectBounding.top());
//        pt2.setY(i_rectBounding.bottom());
//    }
//    else {
//        pt1.setY(i_rectBounding.bottom());
//        pt2.setY(i_rectBounding.top());
//    }
//
//    lineF = QLineF(pt1, pt2);
//
//    // The points are in local coordinates whose origin is at the center point of the line.
//    // The physical line values as provided to the user must be relative to the top left
//    // corner of the parent's bounding rectangle or to the scene, if the item does not
//    // have another item as parent.
//    QGraphicsItem* pGraphicsItemParent = parentItem();
//    CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(pGraphicsItemParent);
//    if (pGraphObjParent != nullptr) {
//        // .. the coordinates were passed relative to the top left
//        // corner of the parent item's bounding rectangle.
//        QRectF rectBoundingParent = pGraphObjParent->getBoundingRect();
//        QPointF ptOriginParent = rectBoundingParent.topLeft();
//        pt1 -= ptOriginParent;
//        pt2 -= ptOriginParent;
//    }
//    // If the item does not belong to a group but has already been added to the scene ...
//    else if (scene() != nullptr) {
//        // .. the coordinates were passed relative to the top left corner of the scene.
//        // The origin of the local coordinates is the center point of the line. We need
//        // to move P1 and P2 so that the center line will get the local coordinates (0/0).
//        pt1 = mapToScene(pt1);
//        pt2 = mapToScene(pt2);
//    }
//
//    // The current coordinates need to be updated (if changed)
//    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
//    CPhysValLine physValLine(pt1, pt2, drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
//    if (m_pDrawingScene->drawingSize().unit() != physValLine.unit()) {
//        physValLine = m_pDrawingScene->convert(physValLine);
//    }
//
//    if (m_physValLineCurr != physValLine) {
//        setPhysValLine(physValLine, ECoordinatesVersion::Transformed);
//        // The coordinates of the bounding rectangle are passed in local coordinates.
//        QGraphicsLineItem_setLine(lineF);
//        emit_geometryChanged();
//    }
//
//    // If the scene position has been changed, the position of the labels got to be updated.
//    updateInternalScenePos();
//}

//------------------------------------------------------------------------------
/*! @brief Sets the group scale for the line.

    The current lines position is moved to keep the relative position in the
    parent group.
    The current lines horizontal (dx) and vertical (dy) components are resized.

         +----+----+----+----+----+----+----+----+
         0    5    10   15   20   25   30   35   40
       0 +-Group---------------------------------+ -30
         |                                       |
      10 |     P1 x (9/10) (-11/-20)             | -20
         |         x                             |
      20 |          x                            | -10
         |       Pos x (12/25) (-8/-5)           |
      30 +------------x------X-------------------+   0
         |             x   (0/0)                 |
      40 |           P1 x (15/40) (-5/10)        |  10
         |                                       |
      50 |                                       |  20
         |                                       |
      60 +---------------------------------------+  30
        -20  -15  -10  -5    0    5    10   15   20
         +----+----+----+----+----+----+----+----+

     ScaleX = 1.5; ScaleY = 1.5

        +----+----+----+----+----+----+----+----+----+----+----+----+
        0    5    10   15   20   25   30   35   40   45   50   55   60
      0 +-Group'----------------------------------------------------+
        |                                                           | -40
     10 |                                                           |
        |        P1' x (13.5/15) (-16.5/-30)                        | -30
     20 |             x                                             |
        |              x                                            | -20
     30 |               x                                           |
        |                x (18/37.5) (-12/-7.5)                     | -10
     40 |            Pos' x                                         |
        +------------------x----------X'----------------------------+   0
     50 |                   x       (0/0)                           |
        |                    x                                      |  10
     60 |                 P2' x (22.5/60) (-7.5/15)                 |
        |                                                           |  20
     70 |                                                           |
        |                                                           |  30
     80 |                                                           |
        |                                                           |  40
     90 +-----------------------------------------------------------+
       -30  -25  -20  -15  -10  -5    0    5    10   15   20   25   30
        +----+----+----+----+----+----+----+----+----+----+----+----+
*/
//void CGraphObjLine::setGroupScale(double i_fXScale, double i_fYScale)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = QString::number(i_fXScale) + ", " + QString::number(i_fYScale);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setGroupScale",
//        /* strAddInfo   */ strMthInArgs );
//
//    //QPointF posOrig = m_physValLineOrig.center().toQPointF();
//    //QPointF posCurr = posOrig;
//    //posCurr.setX(posCurr.x() * i_fXScale);
//    //posCurr.setY(posCurr.y() * i_fYScale);
//    //double fTranslateDX = posCurr.x() - posOrig.x();
//    //double fTranslateDY = posCurr.y() - posOrig.y();
//    //m_transformationsByGroup.translate(fTranslateDX, fTranslateDY);
//    //m_transformationsByGroup.scale(i_fXScale, i_fYScale);
//    //QLineF lineOrig = m_physValLineOrig.toQLineF();
//    //QLineF lineCurr = m_transformationsByGroup.map(lineOrig);
//    //CPhysValLine physValLineCurr = m_physValLineCurr;
//    //physValLineCurr = lineCurr;
//    //setLine(physValLineCurr);
//}

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
        /* strObjName   */ m_strName,
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
            if (isLineHit(line(), i_pt, m_pDrawingScene->getHitToleranceInPx(), &hitInfo)) {
                cursor = hitInfo.m_cursor;
            }
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodOutArgs(qCursorShape2Str(cursor.shape()));
    }
    return cursor;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjLine::getPositionOfSelectionPoint(int i_idxPt, const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPointF ptPos = QPointF(i_idxPt == 0 ? line().p1() : line().p2());
    // Before mapping to parent or scene, the rotation will be reset.
    // Otherwise transformed coordinates will be returned.
    // And itemChange is called but should not emit the geometryChangds signal ..
    CGraphObjLine* pVThis = const_cast<CGraphObjLine*>(this);
    CRefCountGuard refCountGuardGeometryChangedSignal(&pVThis->m_iGeometryChangedSignalBlockedCounter);
    pVThis->QGraphicsItem_setRotation(0.0);
    ptPos = pGraphicsItemThis->mapToParent(ptPos);
    pVThis->QGraphicsItem_setRotation(m_physValRotationAngle.getVal(Units.Angle.Degree));
    CPhysValPoint physValPos(*m_pDrawingScene);
    if (parentGroup() != nullptr) {
        ptPos = parentGroup()->mapToTopLeftOfBoundingRect(ptPos);
        physValPos = parentGroup()->convert(ptPos, i_unit);
    }
    else {
        physValPos = m_pDrawingScene->convert(ptPos, i_unit);
    }
    return physValPos;
}

//------------------------------------------------------------------------------
/*! @brief Returns coordinates of selection point in scene coordinates.
*/
QPointF CGraphObjLine::getPositionOfSelectionPointInSceneCoors( ESelectionPoint i_selPt ) const
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
QPointF CGraphObjLine::getPositionOfSelectionPointInSceneCoors( int i_idxPt ) const
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
SPolarCoors CGraphObjLine::getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, ESelectionPoint i_selPt) const
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
SPolarCoors CGraphObjLine::getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, int i_idxPt) const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QLineF thisLine = line();
    QLineF thisLineSceneCoors;
    QPointF ptSelPtSceneCoors;
    if (i_idxPt == 0) {
        thisLineSceneCoors = QLineF(pGraphicsItemThis->mapToScene(thisLine.p1()),
                                    pGraphicsItemThis->mapToScene(thisLine.p2()));
        ptSelPtSceneCoors = QPointF(pGraphicsItemThis->mapToScene(thisLine.p1()));
    }
    else {
        thisLineSceneCoors = QLineF(pGraphicsItemThis->mapToScene(thisLine.p2()),
                                    pGraphicsItemThis->mapToScene(thisLine.p1()));
        ptSelPtSceneCoors = QPointF(pGraphicsItemThis->mapToScene(thisLine.p2()));
    }
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
    const SPolarCoors& i_polarCoors, ESelectionPoint i_selPt) const
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
    const SPolarCoors& i_polarCoors, int i_idxPt) const
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    if (parentItem() == nullptr) {
        if (i_selPts & c_uSelectionPointsPolygonShapePoints) {
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
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjLine*>(this), ESelectionPoint::Center));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjLine*>(this), 0)); // P1, Start point of the line
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjLine*>(this), 1)); // P2, End point of the line
    }
    static QHash<QString, QList<SGraphObjSelectionPoint>> s_hshSelPtsPredefined;
    if (s_hshSelPtsPredefined.isEmpty()) {
        QList<SGraphObjSelectionPoint> arSelPts;
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjLine*>(this), ESelectionPoint::Center));
        s_hshSelPtsPredefined.insert(c_strLabelName, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjLine*>(this), 0));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameP1, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjLine*>(this), 1));
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
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::PolygonShapePoint) {
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
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::PolygonShapePoint) {
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
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::PolygonShapePoint) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_idxPt != 0) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameP2) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::PolygonShapePoint) {
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
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::PolygonShapePoint) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_idxPt != 0) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_selPtType != ESelectionPointType::PolygonShapePoint) {
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    // Please note that the boundingRect call of QGraphicsLineItem als takes the pen width
    // into account. So we cannot call this method to get the real bounding rectangle of
    // the line if only the real shape points should be considered.
    QRectF rctBounding = QGraphicsLineItem::boundingRect();
    for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsPolygon) {
        if (pGraphObjSelPt != nullptr) {
            QRectF rctSelPt = pGraphObjSelPt->boundingRect();
            QPolygonF plgSelPt = mapFromItem(pGraphObjSelPt, rctSelPt);
            rctBounding |= plgSelPt.boundingRect();
        }
    }
    if (m_plgP1ArrowHead.size() > 0) {
        rctBounding |= m_plgP1ArrowHead.boundingRect();
    }
    if (m_plgP2ArrowHead.size() > 0) {
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
/*! @brief Called internally by QGraphicsItem::boundingRect.
*/
QPainterPath CGraphObjLine::shape() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "shape",
        /* strAddInfo   */ "" );

    QPainterPath painterPath = QGraphicsLineItem::shape();
    if (m_plgP1ArrowHead.size() > 0) {
        painterPath.addPolygon(m_plgP1ArrowHead);
    }
    if (m_plgP2ArrowHead.size() > 0) {
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    i_pPainter->save();

    QPen pn = pen();

    QLineF lineF = line();

    if (m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || m_bIsHighlighted || isSelected())) {
        QPainterPath outline;
        outline.moveTo(lineF.p1());
        outline.lineTo(lineF.p2());
        if (isSelected()) {
            pn.setColor(s_selectionColor);
            pn.setWidth(3 + m_drawSettings.getPenWidth());
        }
        else {
            pn.setColor(s_highlightColor);
            pn.setWidth(3 + m_drawSettings.getPenWidth());
        }
        pn.setStyle(Qt::SolidLine);
        i_pPainter->strokePath(outline, pn);
        pn.setWidth(1 + m_drawSettings.getPenWidth());
    }

    pn.setColor(m_drawSettings.getPenColor());
    pn.setWidth(m_drawSettings.getPenWidth());
    pn.setStyle(lineStyle2QtPenStyle(m_drawSettings.getLineStyle().enumerator()));

    i_pPainter->setPen(pn);

    // This will draw the bounding rectangle with dashed lines. I don't want this.
    //QGraphicsLineItem::paint(i_pPainter, i_pStyleOption, i_pWdgt);

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
        i_pPainter->setPen(pn);
        i_pPainter->setRenderHint(QPainter::Antialiasing);
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

} // paint

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Watches mouse events of the selection points.

    A mouse press event on one of the selection points selects the line.

    If the line is under construction by mouse events a mouse release event
    in selection point of the line end finishes the creation of the line.
*/
bool CGraphObjLine::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjSceneEventFilter, EMethodTraceDetailLevel::ArgsNormal)) {
        CGraphObj* pGraphObjWatched = dynamic_cast<CGraphObj*>(i_pGraphicsItemWatched);
        strMthInArgs = "ItemWatched: " + QString(pGraphObjWatched == nullptr ? "null" : pGraphObjWatched->path());
        if (isGraphicsSceneHoverEvent(i_pEv)) {
            QGraphicsSceneHoverEvent* pHoverEvent = dynamic_cast<QGraphicsSceneHoverEvent*>(i_pEv);
            strMthInArgs += ", " + qEventType2Str(i_pEv->type()) + " {" + qGraphicsSceneHoverEvent2Str(pHoverEvent) + "}";
        }
        else if (isGraphicsSceneMouseEvent(i_pEv)) {
            QGraphicsSceneMouseEvent* pMouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            strMthInArgs += ", " + qEventType2Str(i_pEv->type()) + " {" + qGraphicsSceneMouseEvent2Str(pMouseEvent) + "}";
        }
        else {
            strMthInArgs += ", {" + qEvent2Str(i_pEv) + "}";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjSceneEventFilter,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "sceneEventFilter",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjSelectionPoint* pGraphObjSelPtWatched = dynamic_cast<CGraphObjSelectionPoint*>(i_pGraphicsItemWatched);
    if (pGraphObjSelPtWatched == nullptr) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "pGraphObjSelPtWatched == nullptr");
    }

    SGraphObjSelectionPoint selPt = pGraphObjSelPtWatched->getSelectionPoint();

    bool bEventHandled = false;
    if (i_pEv->type() == QEvent::GraphicsSceneMousePress) {
        setSelected(true);
        bEventHandled = true;
    }
    else if (i_pEv->type() == QEvent::GraphicsSceneMouseRelease) {
        if (m_editMode == EEditMode::CreatingByMouseEvents && selPt.m_idxPt == 1) {
            setEditMode(EEditMode::None);
            setSelected(false);
            bEventHandled = true;
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bEventHandled);
    }
    return bEventHandled;

} // sceneEventFilter

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );

    // Ignore hover events if the line is currently being created.
    if (m_editMode != EEditMode::CreatingByMouseEvents) {
        QLineF lineF = line();
        QPolygonF plg;
        plg.append(lineF.p1());
        plg.append(lineF.p2());
        showSelectionPointsOfPolygon(plg);
        setIsHit(true);
        setCursor(getProposedCursor(i_pEv->pos()));
    }
}

//------------------------------------------------------------------------------
void CGraphObjLine::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    // Ignore hover events if the line is currently being created.
    if (m_editMode != EEditMode::CreatingByMouseEvents) {
        QLineF lineF = line();
        QPolygonF plg;
        plg.append(lineF.p1());
        plg.append(lineF.p2());
        showSelectionPointsOfPolygon(plg);
        setIsHit(true);
        setCursor(getProposedCursor(i_pEv->pos()));
    }
}

//------------------------------------------------------------------------------
void CGraphObjLine::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjHoverEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneHoverEvent2Str(i_pEv);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strMthInArgs );

    // If the object is selected the selection points should remain visible.
    if (!isSelected()) {
        hideSelectionPoints();
    }
    setIsHit(false);
    unsetCursor();
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
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );

    // Forward the mouse event to the base implementation.
    // This will select the item, creating selection points if not yet created.
    QGraphicsLineItem::mousePressEvent(i_pEv);
}

//------------------------------------------------------------------------------
void CGraphObjLine::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseMoveEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseMoveEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    // Forward the mouse event to the LineItems base implementation.
    // This will move the item resulting in an itemChange call with PositionHasChanged.
    QGraphicsLineItem::mouseMoveEvent(i_pEv);
}

//------------------------------------------------------------------------------
void CGraphObjLine::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

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
} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjLine::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseClickEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qGraphicsSceneMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseClickEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );

    // When double clicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // doubleclick event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".
    //QGraphicsLineItem::mouseDoubleClickEvent(i_pEv);

    CEnumMode modeDrawing = m_pDrawingScene->getMode();
    if (modeDrawing == EMode::Edit) {
        if (isSelected()) {
            onCreateAndExecDlgFormatGraphObjs();
        }
    }
} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strMthInArgs );

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
            QLineF lineF = line();
            QPolygonF plg;
            plg.append(lineF.p1());
            plg.append(lineF.p2());
            updateLineEndArrowHeadPolygons();
            bGeometryChanged = true;
            bTreeEntryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged || i_change == ItemPositionHasChanged) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);

        // The object may be moved or transformed by several methods.
        // "itemChange" is a central point to update the coordinates upon those changes.
        QLineF lineF = line();
        if (m_iItemChangeUpdateOriginalCoorsBlockedCounter == 0) {
            // Update the object shape point in parent coordinates kept in the unit of the drawing scene.
            // If the item is not a group and as long as the item is not added as a child to
            // a group, the current (transformed) and original coordinates are equal.
            // If the item is a child of a group, the current (transformed) coordinates are only
            // taken over as the original coordinates if initially creating the item or when
            // adding the item to or removing the item from a group.
            updateOriginalPhysValCoors();
        }
        updateLineEndArrowHeadPolygons();
        tracePositionInfo(mthTracer, EMethodDir::Leave);
        bGeometryChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemRotationHasChanged) {
        //tracePositionInfo(mthTracer, EMethodDir::Enter);
        //updateLineEndArrowHeadPolygons();
        //tracePositionInfo(mthTracer, EMethodDir::Leave);
        bGeometryChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemSelectedHasChanged) {
        prepareGeometryChange();
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            bringToFront(); 
            showSelectionPoints();
            // Not necessary to bring selection points to front as item has been already brought
            // to front and "showSelectionPoints" sets zValue of selection points above item.
            //bringSelectionPointsToFront();
            //setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
        }
        else {
            //setAcceptedMouseButtons(Qt::NoButton);
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
        emit_geometryChanged();
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
//        /* strObjName   */ m_strName,
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
void CGraphObjLine::onSelectionPointGeometryChanged(CGraphObj* i_pSelectionPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pSelectionPoint->path();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onSelectionPointGeometryChanged",
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

    if (selPt.m_selPtType == ESelectionPointType::PolygonShapePoint) {
        if (selPt.m_idxPt == 0) {
            setP1(physValParentSelPt);
        }
        else if (selPt.m_idxPt == 1) {
            setP2(physValParentSelPt);
        }
    }
}

/*==============================================================================
protected: // instance methods
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateLineEndArrowHeadPolygons",
        /* strAddInfo   */ strMthInArgs );

    QLineF lineF = line();
    if (!i_linePoint.isValid() || i_linePoint == ELinePoint::Start) {
        CEnumLineEndStyle lineEndStyle = m_drawSettings.getLineEndStyle(ELinePoint::Start);
        if (lineEndStyle != ELineEndStyle::Normal) {
            getLineEndPolygons(
                /* line          */ lineF,
                /* drawSetings   */ m_drawSettings,
                /* pplgLineStart */ &m_plgP1ArrowHead,
                /* pplgLineEnd   */ nullptr);
        }
    }
    if (!i_linePoint.isValid() || i_linePoint == ELinePoint::End) {
        CEnumLineEndStyle lineEndStyle = m_drawSettings.getLineEndStyle(ELinePoint::End);
        if (lineEndStyle != ELineEndStyle::Normal) {
            getLineEndPolygons(
                /* line          */ lineF,
                /* drawSetings   */ m_drawSettings,
                /* pplgLineStart */ nullptr,
                /* pplgLineEnd   */ &m_plgP2ArrowHead);
        }
    }
}

/*==============================================================================
protected: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjLine::updateToolTip()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "updateToolTip",
//        /* strAddInfo   */ "" );
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);
//
//    if (pGraphicsItem != nullptr)
//    {
//        QString strNodeSeparator = CDrawingScene::getGraphObjNameNodeSeparator();
//        QLineF  lineF = line();
//        QPointF ptPos;
//
//        m_strToolTip  = "ObjPath:\t" + path();
//
//        m_strToolTip += "\nP1:\t\t" + point2Str(lineF.p1());
//        m_strToolTip += "\nP2:\t\t" + point2Str(lineF.p2());
//
//        // "scenePos" returns mapToScene(0,0). This is NOT equivalent to the
//        // position of the item's top left corner before applying the rotation
//        // transformation matrix but includes the transformation. What we want
//        // (or what I want) is the position of the item before rotating the item
//        // around the rotation origin point. In contrary it looks like "pos"
//        // always returns the top left corner before rotating the object.
//
//        if( pGraphicsItem->parentItem() != nullptr )
//        {
//            ptPos = pGraphicsItem->pos();
//            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
//        }
//        else
//        {
//            ptPos = pGraphicsItem->pos(); // don't use "scenePos" here (see comment above)
//            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
//        }
//
//        m_strToolTip += "\nSize:\t\t" + getSize(Units.Length.px).toString();
//        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());
//
//        pGraphicsItem->setToolTip(m_strToolTip);
//    }
//} // updateToolTip

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the original line coordinates.

    @param [in] i_line
        Line coordinates to be set.

    @return Previous original line coordinates.
*/
CPhysValLine CGraphObjLine::setLineOrig(const CPhysValLine& i_physValLine)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLineOrig",
        /* strAddInfo   */ strMthInArgs );

    CPhysValLine physValLinePrev = m_physValLineOrig;
    m_physValLineOrig = i_physValLine;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol());
    }
    return i_physValLine;
}

//------------------------------------------------------------------------------
/*! @brief Sets the line coordinates at the graphics item.

    @param [in] i_line
        Line coordinates to be set.

    @return Previous line coordinates.
*/
QLineF CGraphObjLine::QGraphicsLineItem_setLine(const QLineF& i_line)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Line {" + qLine2Str(i_line) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "QGraphicsLineItem_setLine",
        /* strAddInfo   */ strMthInArgs );

    QLineF linePrev = QGraphicsLineItem::line();
    QGraphicsLineItem::setLine(i_line);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qLine2Str(linePrev));
    }
    return linePrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the line coordinates at the graphics item.

    @param [in] i_fX1, i_fY1, i_fX2, i_fY2
        Line coordinates to be set.

    @return Previous line coordinates.
*/
QLineF CGraphObjLine::QGraphicsLineItem_setLine(double i_fX1, double i_fY1, double i_fX2, double i_fY2)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Line {" + qLine2Str(QLineF(i_fX1, i_fY1, i_fX2, i_fY2)) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "QGraphicsLineItem_setLine",
        /* strAddInfo   */ strMthInArgs );

    QLineF linePrev = QGraphicsLineItem::line();
    QGraphicsLineItem::setLine(i_fX1, i_fY1, i_fX2, i_fY2);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qLine2Str(linePrev));
    }
    return linePrev;
}

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItem != nullptr) {
            QLineF lineF = line();
            QString strRuntimeInfo;
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "PhysValLine Curr {" + getLine().toString() + "} " + getLine().unit().symbol() +
            ", Orig {" + m_physValLineOrig.toString() + "} " + m_physValLineOrig.unit().symbol();
            i_mthTracer.trace(strRuntimeInfo);
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "ItemLine Curr {" + qLine2Str(line()) + "}";
            i_mthTracer.trace(strRuntimeInfo);
            CGraphObj::traceThisPositionInfo(i_mthTracer, i_mthDir, i_detailLevel);
        }
    }
}
