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
    m_physValLineOrig(*i_pDrawingScene),
    m_physValLineCurr(*i_pDrawingScene),
    m_plgP1ArrowHead(),
    m_plgP2ArrowHead()
{
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

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
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryAngle, 0 ,1);
        }
    }

    // Initialise list with number of selection points.
    // A line may provide two selection points - at start and end of line.
    m_arpSelPtsPolygon.append(nullptr);
    m_arpSelPtsPolygon.append(nullptr);

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
    //m_physValLineCurr;
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
    QString strScenePolygonShapePoints;
    QLineF lineF = line();
    QPolygonF plgScene;
    plgScene.append(mapToScene(lineF.p1()));
    plgScene.append(mapToScene(lineF.p2()));
    return polygon2Str(plgScene);
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

    @param [in] i_physValLine
        Line to be set.
        Coordinates must be passed relative to the top left corner of the
        parent item's bounding rectangle (real shape points only),
        or in scene coordinates if the item does not have parent.
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

    // If the coordinates MUST be updated (e.g. after the drawing size has been changed)
    // or if they have been changed ...
    if (m_bForceConversionToSceneCoors || m_physValLineCurr != i_physValLine)
    {
        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsLineItem::prepareGeometryChange();

        // The line coordinates are passed relative to the parent item
        // (usually the scene coordinates or a group item).
        QLineF lineF = i_physValLine.toQLineF();
        if (i_physValLine.unit() != Units.Length.px) {
            lineF = m_pDrawingScene->convert(i_physValLine, Units.Length.px).toQLineF();
        }

        // Line points in parent coordinates.
        QPointF pt1 = lineF.p1();
        QPointF pt2 = lineF.p2();
        QPointF ptPos = lineF.center();

        // But the graphics item expects the coordinates based on the
        // local coordinate system of the graphics item.
        // Items live in their own local coordinate system.
        // Their coordinates are usually centered around its center point (0, 0).

        // Move the points into the item's local coordinate system.
        // If the item belongs to a group ...
        QGraphicsItem* pGraphicsItemParent = parentItem();
        CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(pGraphicsItemParent);
        if (pGraphObjParent != nullptr) {
            // .. the coordinates were passed relative to the top left
            // corner of the parent item's bounding rectangle.
            pt1 = mapFromParent(pt1);
            pt2 = mapFromParent(pt2);
            QRectF rectBoundingParent = pGraphObjParent->getCurrentBoundingRect();
            QPointF ptOriginParent = rectBoundingParent.topLeft();
            pt1 += ptOriginParent;
            pt2 += ptOriginParent;
        }
        // If the item does not belong to a group but has already been added to the scene ...
        else if (scene() != nullptr) {
            // .. the coordinates were passed relative to the top left corner of the scene.
            // The origin of the local coordinates is the center point of the line. We need
            // to move P1 and P2 so that the center line will get the local coordinates (0/0).
            QPointF ptCenter = mapFromScene(ptPos);
            pt1 = mapFromScene(pt1) - ptCenter;
            pt2 = mapFromScene(pt2) - ptCenter;
        }
        // If the line has not added to the scene yet ...
        else {
            // .. we must do the transformation to the local coordinate system
            // on our own. The origin is the center of the line.
            pt1 -= lineF.center();
            pt2 -= lineF.center();
        }

        // Local coordinates. The origin is the center point of the line.
        lineF = QLineF(pt1, pt2);

        {   CRefCountGuard refCountGuard(&m_iItemChangeUpdateOriginalCoorsBlockedCounter);

            // Update original line coordinates in unit of drawing scene.
            // Please note that the current, untransformed line coordinates m_physValLineCurr,
            // kept in the unit of the drawing scene, is also updated in the "itemChange" method.
            // The line may be moved or transformed by other methods. "itemChange" is a
            // central point to update the coordinates upon those changes.
            // When explicitly setting the line coordinates the itemChange method must not
            // overwrite the current line value.
            // If the item is not a group and as long as the item is not added as a child to
            // a group, the current (transformed) and original coordinates are equal.
            // If the item is a child of a group, the current (transformed) coordinates are only
            // taken over as the original coordinates if initially creating the item or when
            // adding the item to or removing the item from a group.
            CEnumCoordinatesVersion eVersion;
            if (parentItem() != nullptr) {
                eVersion = ECoordinatesVersion::Transformed;
            }
            setPhysValLine(i_physValLine, eVersion);

            // Set the line in local coordinate system.
            // Also note that itemChange must not overwrite the current line value (revCountGuard).
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
            QGraphicsItem_setPos(ptPos);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the item's line to the given line coordinates.

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
    setLine(CPhysValLine(i_fX1, i_fY1, i_fX2, i_fY2, i_fRes, i_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the item's line to the given line coordinates.

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
    setLine(CPhysValLine(i_p1, i_p2, i_fRes, i_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the item's line to the given line coordinates.

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
    setLine(CPhysValLine(i_line, i_fRes, i_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the line coordinates by passing P1 and P2.

    @param [in] i_physValP1
        Start point of the line.
        Coordinates are assumed to be relative to the parent item the item
        will later be added to as a child (or in scene coordinates if the item
        is added to the scene).
    @param [in] i_physValP2
        End point of the line.
        Coordinates are assumed to be relative to the parent item the item
        will later be added to as a child (or in scene coordinates if the item
        is added to the scene).
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
/*! @brief Returns the item's line in the current unit of the drawing in parent
           coordinates. The origin of the returned line coordinates is the top
           left corner of the parent's bounding rectangle (real shape points only).
*/
CPhysValLine CGraphObjLine::getLine() const
//------------------------------------------------------------------------------
{
    return m_physValLineCurr;
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's line in the given unit.

    @param [in] i_unit
        Unit in which the line coordinates should be returned.
*/
CPhysValLine CGraphObjLine::getLine(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysValLine physValLine = m_physValLineCurr;
    if (i_unit != physValLine.unit()) {
        physValLine = m_pDrawingScene->convert(physValLine, i_unit);
    }
    return physValLine;
}

//------------------------------------------------------------------------------
/*! @brief Sets the coordinates of P1.

    Point 2 remains unchanged.

    @param [in] i_p1
        X and Y coordinate of point 1.
        The coordinates must be passed in the current unit of the line.
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
    setP1(CPhysValPoint(i_p1, m_physValLineCurr.resolution(), m_physValLineCurr.unit()));
}

//------------------------------------------------------------------------------
/*! @brief Sets the coordinates of P1.

    Point 2 remains unchanged.

    @param [in] i_physValP1
        X and Y coordinate of point 1. If the unit of the passed point is
        different from the current unit of the line, the point coordinates
        will be converted to the lines current unit.
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
    CPhysValLine physValLine = m_physValLineCurr;
    if (i_physValP1.unit() != m_physValLineCurr.unit()) {
        CPhysValPoint physValP1 = m_pDrawingScene->convert(i_physValP1, m_physValLineCurr.unit());
        physValLine.setP1(physValP1);
    }
    else {
        physValLine.setP1(i_physValP1);
    }
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's point 1 in the current unit.
*/
CPhysValPoint CGraphObjLine::getP1() const
//------------------------------------------------------------------------------
{
    return getLine().p1();
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's point 1 in the given unit.

    @param [in] i_unit
        Unit in which the coordinates should be returned.
*/
CPhysValPoint CGraphObjLine::getP1(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getLine(i_unit).p1();
}

//------------------------------------------------------------------------------
/*! @brief Sets the coordinates of P2.

    Point 1 remains unchanged.

    @param [in] i_p1
        X and Y coordinate of point 2.
        The coordinates must be passed in the current unit of the line.
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
    setP2(CPhysValPoint(i_p2, m_physValLineCurr.resolution(), m_physValLineCurr.unit()));
}

//------------------------------------------------------------------------------
/*! @brief Sets the coordinates of P2.

    Point 1 remains unchanged.

    @param [in] i_physValP2
        X and Y coordinate of point 2. If the unit of the passed point is
        different from the current unit of the line, the point coordinates
        will be converted to the lines current unit.
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
    CPhysValLine physValLine = m_physValLineCurr;
    if (i_physValP2.unit() != m_physValLineCurr.unit()) {
        CPhysValPoint physValP2 = m_pDrawingScene->convert(i_physValP2, m_physValLineCurr.unit());
        physValLine.setP2(physValP2);
    }
    else {
        physValLine.setP2(i_physValP2);
    }
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's point 2 in the current unit.
*/
CPhysValPoint CGraphObjLine::getP2() const
//------------------------------------------------------------------------------
{
    return getLine().p2();
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's point 2 in the given unit.

    @param [in] i_unit
        Unit in which the coordinates should be returned.
*/
CPhysValPoint CGraphObjLine::getP2(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getLine(i_unit).p2();
}

//------------------------------------------------------------------------------
/*! @brief Sets the center of the line.

    Both point 1 and point 2 are updated correspondingly.
    The length and the angle remain the same.

    @param [in] i_pCenter
        X and Y coordinate of point 1.
        The coordinates must be passed in the current unit of the line.
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
    physValLine.setCenter(CPhysValPoint(i_pCenter, physValLine.resolution(), physValLine.unit()));
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Sets the center of the line.

    Both point 1 and point 2 are updated correspondingly.
    The length and the angle remain the same.

    @param [in] i_physValCenter
        X and Y coordinate of the center point. If the unit of the passed point is
        different from the current unit of the line, the point coordinates
        will be converted to the lines current unit.
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
    CPhysValLine physValLine = m_physValLineCurr;
    if (i_physValCenter.unit() != m_physValLineCurr.unit()) {
        CPhysValPoint physValCenter = m_pDrawingScene->convert(i_physValCenter, m_physValLineCurr.unit());
        physValLine.setCenter(physValCenter);
    }
    else {
        physValLine.setCenter(i_physValCenter);
    }
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's center point in the current unit.
*/
CPhysValPoint CGraphObjLine::getCenter() const
//------------------------------------------------------------------------------
{
    return getLine().center();
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's center point in the given unit.

    @param [in] i_unit
        Unit in which the coordinates should be returned.
*/
CPhysValPoint CGraphObjLine::getCenter(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getLine(i_unit).center();
}

//------------------------------------------------------------------------------
/*! @brief Sets the length of the line in the given unit.

    Point 1 remains unchanged. Point 2 is updated correspondingly.
    The angle remains the same.

    @param [in] i_physValLength
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
    setLength(CPhysVal(i_fLength, m_physValLineCurr.unit(), m_physValLineCurr.resolution()));
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
    CPhysValLine physValLine = m_physValLineCurr;
    if (i_physValLength.unit() != m_physValLineCurr.unit()) {
        CPhysVal physValLength(i_physValLength);
        physValLength.convertValue(m_physValLineCurr.unit());
        physValLine.setLength(physValLength);
    }
    else {
        physValLine.setLength(i_physValLength);
    }
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's length in the current unit.
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
/*! @brief Sets the angle of the line in degrees.

    Point 1 remains unchanged. Point 2 is updated correspondingly.
    The length remains the same.

    @param [in] i_physValLength
        Length to be set.
*/
void CGraphObjLine::setAngle(double i_fAngle_degree)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fAngle_degree);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setAngle",
        /* strAddInfo   */ strMthInArgs );
    setAngle(CPhysVal(i_fAngle_degree, Units.Angle.Degree, 0.1));
}

//------------------------------------------------------------------------------
/*! @brief Sets the angle of the line in the given unit.

    Point 1 remains unchanged. Point 2 is updated correspondingly.
    The length remains the same.

    @param [in] i_physValLength
        Length to be set.
*/
void CGraphObjLine::setAngle(const CPhysVal& i_physValAngle)
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
    CPhysValLine physValLine = m_physValLineCurr;
    physValLine.setAngle(i_physValAngle);
    setLine(physValLine);
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's angle in degrees.
*/
double CGraphObjLine::getAngleInDegrees() const
//------------------------------------------------------------------------------
{
    return m_physValLineCurr.angle().getVal(Units.Angle.Degree);
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's angle in the given unit.

    @param [in] i_unit
        Unit in which the angle should be returned.
*/
CPhysVal CGraphObjLine::getAngle(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_physValLineCurr.angle().getVal(i_unit), i_unit, 0.1);
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the position of the item relative to its parent item in the
           current unit of the drawing scene.
*/
CPhysValPoint CGraphObjLine::getPos() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return getLine(drawingSize.unit()).center();
}

//------------------------------------------------------------------------------
/*! @brief Returns the position of the item relative to its parent item in the
           given unit.
*/
CPhysValPoint CGraphObjLine::getPos(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getLine(i_unit).center();
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the current bounding rectangle of the object in local coordinates.

    @return Bounding rectangle in local coordinates.
*/
QRectF CGraphObjLine::getCurrentBoundingRect() const
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
    QRectF rctBounding = ZS::Draw::boundingRect(lineF);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Returns the bounding rectangle of the object in local coordinates.

    This method is used by a group to resize its children.

    Please refer to documentation about the difference between current (transformed)
    and original coordinates at base class CGraphObj.

    @return Original bounding rectangle in local coordinates.
*/
QRectF CGraphObjLine::getOriginalBoundingRectInParent() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "getOriginalBoundingRectInParent",
        /* strAddInfo   */ "" );

    // Line points in local coordinates.
    QLineF lineF = line();
    QGraphicsItem* pGraphicsItemParent = parentItem();
    CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(pGraphicsItemParent);
    // If the line belongs to a group ...
    if (pGraphObjParent != nullptr) {
        lineF = m_physValLineOrig.toQLineF();
        // The original coordinates in m_physValLineOrig are relative to the
        // top left corner of the parent's bounding rectangle.
        QPointF pt1 = lineF.p1();
        QPointF pt2 = lineF.p2();
        // The coordinates must be mapped to local coordinates.
        QRectF rctBoundingParent = pGraphObjParent->getOriginalBoundingRectInParent();
        QPointF ptOriginParent = rctBoundingParent.topLeft();
        pt1 += ptOriginParent;
        pt2 += ptOriginParent;
        lineF = QLineF(pt1, pt2);
    }
    // If the item does not belong to a group ...
    else {
        // .. the current (transformed) and original coordinates must be the same.
        // lineF already contains the local coordinates.
    }
    QRectF rctBounding = ZS::Draw::boundingRect(lineF);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Pure virtual method which must be overridden by derived classes to
           set the bounding rectangle of the object in local coordinates.

    This method is used by a group to resize its children.

    P1 and P2 should not be swapped when invoking this method.

    If P1 is right of P2, P1 should stay right of P1.
    If P1 is below of P2, P1 should stay below of P1.

    @param [in] i_rectBounding
        New bounding rectangle of the object in local coordinates.
*/
void CGraphObjLine::setCurrentBoundingRectInParent(const QRectF& i_rectBounding)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qRect2Str(i_rectBounding);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setCurrentBoundingRectInParent",
        /* strAddInfo   */ strMthInArgs );

    double fXL = i_rectBounding.left();
    double fYT = i_rectBounding.top();
    double fXR = i_rectBounding.right();
    double fYB = i_rectBounding.bottom();

    QLineF lineF = line();
    QPointF pt1 = lineF.p1();
    QPointF pt2 = lineF.p2();

    if (pt1.x() < pt2.x()) {
        pt1.setX(i_rectBounding.left());
        pt2.setX(i_rectBounding.right());
    }
    else {
        pt1.setX(i_rectBounding.right());
        pt2.setX(i_rectBounding.left());
    }
    if (lineF.y1() < lineF.y2()) {
        pt1.setY(i_rectBounding.top());
        pt2.setY(i_rectBounding.bottom());
    }
    else {
        pt1.setY(i_rectBounding.bottom());
        pt2.setY(i_rectBounding.top());
    }

    lineF = QLineF(pt1, pt2);

    // The points are in local coordinates whose origin is at the center point of the line.
    // The physical line values as provided to the user must be relative to the top left
    // corner of the parent's bounding rectangle or to the scene, if the item does not
    // have another item as parent.
    QGraphicsItem* pGraphicsItemParent = parentItem();
    CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(pGraphicsItemParent);
    if (pGraphObjParent != nullptr) {
        // .. the coordinates were passed relative to the top left
        // corner of the parent item's bounding rectangle.
        QRectF rectBoundingParent = pGraphObjParent->getCurrentBoundingRect();
        QPointF ptOriginParent = rectBoundingParent.topLeft();
        pt1 -= ptOriginParent;
        pt2 -= ptOriginParent;
    }
    // If the item does not belong to a group but has already been added to the scene ...
    else if (scene() != nullptr) {
        // .. the coordinates were passed relative to the top left corner of the scene.
        // The origin of the local coordinates is the center point of the line. We need
        // to move P1 and P2 so that the center line will get the local coordinates (0/0).
        pt1 = mapToScene(pt1);
        pt2 = mapToScene(pt2);
    }

    // The current coordinates need to be updated (if changed)
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysValLine physValLine(pt1, pt2, drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
    if (m_pDrawingScene->drawingSize().unit() != physValLine.unit()) {
        physValLine = m_pDrawingScene->convert(physValLine);
    }

    if (m_physValLineCurr != physValLine) {
        setPhysValLine(physValLine, ECoordinatesVersion::Transformed);
        // The coordinates of the bounding rectangle are passed in local coordinates.
        QGraphicsLineItem_setLine(lineF);
        emit_geometryChanged();
    }

    // If the scene position has been changed, the position of the labels got to be updated.
    updateInternalScenePos();
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
        strMthInArgs = "Point:" + point2Str(i_pt);
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
/*! @brief Returns coordinates of selection point in scene coordinates.
*/
QPointF CGraphObjLine::getSelectionPointCoorsInSceneCoors( ESelectionPoint i_selPt ) const
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
QPointF CGraphObjLine::getSelectionPointCoorsInSceneCoors( int i_idxPt ) const
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
public: // overridables of base class CGraphObj
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
                      / Calculated Angle: 60�
        +------------x------------+
        P1         Center         P2

    @note This method is used to keep the relative position of labels to the
          graphical object they are linked to if the linked object is resized,
          rotated or moved.

          For example if the line would be rotated by 180�:

            P2         Center         P1
            +------------x------------+
             Angle: 60� /
                       /
                   Pt +
*/
SPolarCoors CGraphObjLine::getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    QLineF thisLine = line();
    QLineF thisLineSceneCoors(mapToScene(thisLine.p1()), mapToScene(thisLine.p2()));
    QPointF ptSelPtSceneCoors(mapToScene(thisLine.center()));
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
         / Calculated Angle: 60�
        +------------x------------+
        P1         Center         P2

    @note This method is used to keep the relative position of labels to the
          graphical object they are linked to if the linked object is resized,
          rotated or moved.

          For example if the line would be rotated by 180�:

            P2         Center         P1
            +------------x------------+
                          Angle: 60� /
                                    /
                                Pt +
*/
SPolarCoors CGraphObjLine::getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, int i_idxPt) const
//------------------------------------------------------------------------------
{
    QLineF thisLine = line();
    QLineF thisLineSceneCoors;
    QPointF ptSelPtSceneCoors;
    if (i_idxPt == 0) {
        thisLineSceneCoors = QLineF(mapToScene(thisLine.p1()), mapToScene(thisLine.p2()));
        ptSelPtSceneCoors = QPointF(mapToScene(thisLine.p1()));
    }
    else {
        thisLineSceneCoors = QLineF(mapToScene(thisLine.p2()), mapToScene(thisLine.p1()));
        ptSelPtSceneCoors = QPointF(mapToScene(thisLine.p2()));
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
    QLineF lineSelPt = line();
    QLineF lineSelPtSceneCoors(mapToScene(lineSelPt.center()), mapToScene(lineSelPt.p2()));
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
    QLineF lineSelPt = line();
    QLineF lineSelPtSceneCoors;
    if (i_idxPt == 0) {
        lineSelPtSceneCoors = QLineF(mapToScene(lineSelPt.p1()), mapToScene(lineSelPt.p2()));
    }
    else {
        lineSelPtSceneCoors = QLineF(mapToScene(lineSelPt.p2()), mapToScene(lineSelPt.p1()));
    }
    return ZS::Draw::getLineFromPolar(
        i_polarCoors.m_fLength_px, i_polarCoors.m_fAngle_degrees, lineSelPtSceneCoors);
}

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::showSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
        if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = selectionPoints2Str(i_selPts);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    if (parentItem() == nullptr) {
        if (i_selPts & ESelectionPointsPolygonShapePoints) {
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

    Please note that the most common used selection points should be at the
    beginning of the list so that combo boxes to select the selection point
    start with those.

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

/*==============================================================================
public: // overridables of base class CGraphObj (geometry labels)
==============================================================================*/

//------------------------------------------------------------------------------
QStringList CGraphObjLine::getGeometryLabelNames() const
//------------------------------------------------------------------------------
{
    static const QStringList s_strlstValueNames = {
        c_strGeometryLabelNameP1, c_strGeometryLabelNameP2, c_strGeometryLabelNameCenter,
        c_strGeometryLabelNameDX, c_strGeometryLabelNameDY, c_strGeometryLabelNameLength,
        c_strGeometryLabelNameAngle
    };
    return s_strlstValueNames;
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    i_pPainter->save();
    i_pPainter->setRenderHint(QPainter::Antialiasing);

    QPen pn = pen();

    QLineF lineF = line();

    if (m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || m_bIsHighlighted || isSelected())) {
        if (m_bIsHit || m_bIsHighlighted || isSelected()) {
            QPainterPath outline;
            outline.moveTo(lineF.p1());
            outline.lineTo(lineF.p2());
            if (isSelected()) {
                pn.setColor(Qt::magenta);
                pn.setWidth(3 + m_drawSettings.getPenWidth());
            }
            else {
                pn.setColor(Qt::cyan);
                pn.setWidth(3 + m_drawSettings.getPenWidth());
            }
            pn.setStyle(Qt::SolidLine);
            i_pPainter->strokePath(outline, pn);
            pn.setWidth(1 + m_drawSettings.getPenWidth());
        }
    }
    else {
        pn.setWidth(m_drawSettings.getPenWidth());
    }

    pn.setColor(m_drawSettings.getPenColor());
    pn.setStyle(lineStyle2QtPenStyle(m_drawSettings.getLineStyle().enumerator()));

    i_pPainter->setPen(pn);
    i_pPainter->drawLine(lineF);

    //#pragma message(__TODO__"To be removed")
    //i_pPainter->setPen(Qt::red);
    //QLineF lineCenterHor(lineF.center().x()-5, lineF.center().y(), lineF.center().x()+5, lineF.center().y());
    //QLineF lineCenterVer(lineF.center().x(), lineF.center().y()-5, lineF.center().x(), lineF.center().y()+5);
    //i_pPainter->drawLine(lineCenterHor);
    //i_pPainter->drawLine(lineCenterVer);
    //#pragma message(__TODO__"To be removed")

    i_pPainter->setPen(pn);

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

    QVariant valChanged = i_value;

    bool bGeometryChanged = false;
    bool bSelectedChanged = false;
    bool bZValueChanged = false;
    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged) {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
            bGeometryChanged = true;
            bTreeEntryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged || i_change == ItemPositionHasChanged) {
        traceGraphicsItemStates(mthTracer);
        traceGraphObjStates(mthTracer);
        tracePositionInfo(mthTracer);

        // Update the original, untransformed line coordinates kept in the unit of
        // the drawing scene. The line may be moved or transformed by several methods.
        // "itemChange" is a central point to update the coordinates upon those changes.
        if (m_iItemChangeUpdateOriginalCoorsBlockedCounter == 0) {
            QLineF lineF = line();
            // Transform local coordinates to parent groups coordinate system.
            // The origin of the parents coordinate system is at the center point
            // of the parents bounding rectangle. The original, untransformed as
            // provided to the user must be relative to the top left corner of
            // the parent groups bounding rectangle.
            // First map to parents coordinate system with origin at center point of parent.
            QPointF p1 = mapToParent(lineF.p1());
            QPointF p2 = mapToParent(lineF.p2());
            // Move the coordinates so that they become relative to the top left corner
            // of the parents bounding rectangle.
            CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(parentItem());
            if (pGraphObjGroup != nullptr) {
                QRectF rectFParent = pGraphObjGroup->getCurrentBoundingRect();
                p1 -= rectFParent.topLeft();
                p2 -= rectFParent.topLeft();
            }
            // Convert to unit of drawing scene.
            CPhysValPoint physValP1 = m_pDrawingScene->toPhysValPoint(p1);
            CPhysValPoint physValP2 = m_pDrawingScene->toPhysValPoint(p2);
            // If the item is not a group and as long as the item is not added as a child to
            // a group, the current (transformed) and original coordinates are equal.
            // If the item is a child of a group, the current (transformed) coordinates are only
            // taken over as the original coordinates if initially creating the item or when
            // adding the item to or removing the item from a group.
            CEnumCoordinatesVersion eVersion = ECoordinatesVersion::Transformed;
            if (i_change == ItemPositionHasChanged) {
                if (parentItem() == nullptr) {
                    eVersion = CEnumCoordinatesVersion();
                }
            }
            else if (i_change == ItemParentHasChanged) {
                eVersion = CEnumCoordinatesVersion();
            }
            setPhysValLine(CPhysValLine(physValP1, physValP2), eVersion);
        }
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
        QLineF lineF = line();
        QPolygonF plg;
        plg.append(lineF.p1());
        plg.append(lineF.p2());
        updateLineEndArrowHeadPolygons();
        updateInternalScenePos();
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

//------------------------------------------------------------------------------
void CGraphObjLine::onDrawingSizeChanged(const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawingSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_physValLineCurr.unit() != i_drawingSize.unit()) {
        m_bForceConversionToSceneCoors = true;
        setLine(m_pDrawingScene->convert(m_physValLineCurr, i_drawingSize.unit()));
        m_bForceConversionToSceneCoors = false;
        emit_geometryValuesUnitChanged();
    }
}

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

    CGraphObjSelectionPoint* pGraphObjSelPt = dynamic_cast<CGraphObjSelectionPoint*>(i_pSelectionPoint);
    QGraphicsItem* pGraphicsItemSelPt = dynamic_cast<QGraphicsItem*>(pGraphObjSelPt);
    QPointF ptScenePosSelPt = pGraphicsItemSelPt->scenePos();
    QPointF ptPosSelPt = mapFromScene(ptScenePosSelPt);
    QPointF ptParentPosSelPt = mapToParent(ptPosSelPt);
    CPhysValPoint physValParentSelPt = m_pDrawingScene->toPhysValPoint(ptParentPosSelPt);
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
/*! @brief Sets the original physical line coordinates.

    If the given lines unit is different from the drawing scenes current unit
    the line coordinates will be converted into the drawing scenes unit.

    @param [in] i_physValLine
        Line coordinates to be set.
    @param [in] i_eVersion
        Transformed will set the current line coordinates.
        If set to Origin the original coordinate values are set.
        If an invalid enum value (default) is passed both the current (transformed)
        and the original coordinates are set.
        Please refer to documentation about the difference between Transformed and
        Original coordinates at base class CGraphObj.
*/
void CGraphObjLine::setPhysValLine(
    const CPhysValLine& i_physValLine, const CEnumCoordinatesVersion& i_eVersion)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValLine.toString() + ", " + QString(i_eVersion.isValid() ? i_eVersion.toString() : "All");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setPhysValLine",
        /* strAddInfo   */ strMthInArgs );

    if (!i_eVersion.isValid() || i_eVersion == ECoordinatesVersion::Transformed) {
        if (m_pDrawingScene->drawingSize().unit() == i_physValLine.unit()) {
            m_physValLineCurr = i_physValLine;
        }
        else {
            CPhysValLine physValLine = m_pDrawingScene->convert(i_physValLine);
            m_physValLineCurr = i_physValLine;
        }
    }
    if (!i_eVersion.isValid() || i_eVersion == ECoordinatesVersion::Original) {
        if (m_pDrawingScene->drawingSize().unit() == i_physValLine.unit()) {
            m_physValLineOrig = i_physValLine;
        }
        else {
            CPhysValLine physValLine = m_pDrawingScene->convert(i_physValLine);
            m_physValLineOrig = i_physValLine;
        }
    }

    // The first "setLine" call whose passed coordinates are not zero
    // are taken as the original values. As long as there is are valid
    // original coordinates childrens cannot be resized by groups.
    // See also comment at CGraphObjGroup::applyGeometryChangeToChildrens.
    if (!m_physValLineOrig.isValid()) {
        m_physValLineOrig = m_physValLineCurr;
    }
}

//------------------------------------------------------------------------------
void CGraphObjLine::QGraphicsLineItem_setLine(const QLineF& i_line)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = line2Str(i_line);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "QGraphicsLineItem_setLine",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsLineItem::setLine(i_line);
}

//------------------------------------------------------------------------------
void CGraphObjLine::QGraphicsLineItem_setLine(double i_fX1, double i_fY1, double i_fX2, double i_fY2)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = line2Str(QLineF(i_fX1, i_fY1, i_fX2, i_fY2));
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "QGraphicsLineItem_setLine",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsLineItem::setLine(i_fX1, i_fY1, i_fX2, i_fY2);
}

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::tracePositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItem != nullptr) {
            QString strRuntimeInfo;

            QPointF ptPos = pGraphicsItem->pos();
            QPointF ptScenePos = pGraphicsItem->scenePos();
            QLineF lineF = line();
            QRectF rectBounding = getCurrentBoundingRect();
            QPointF ptCenterPos = rectBounding.center();
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "PhysValLine {" + m_physValLineCurr.toString() + "}" +
                ", Pos {" + qPoint2Str(ptPos) + "}, ScenePos {" + qPoint2Str(ptScenePos) + "}" +
                ", Line {" + qLine2Str(lineF) + "}" +
                ", BoundingRect {" + qRect2Str(rectBounding) + "}" +
                ", Center {" + qPoint2Str(ptCenterPos) + "}}";
            i_mthTracer.trace(strRuntimeInfo);

            QGraphicsItem* pGraphicsItemParent = pGraphicsItem->parentItem();
            CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphicsItemParent);
            if (pGraphicsItemParent != nullptr && pGraphObjGroup != nullptr) {
                QPointF ptPosParent = pGraphicsItemParent->pos();
                QPointF ptScenePosParent = pGraphicsItemParent->scenePos();
                QRectF rectBoundingParent = pGraphObjGroup->getCurrentBoundingRect();
                QPointF ptCenterPosParent = rectBoundingParent.center();
                if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
                else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
                else strRuntimeInfo = "   ";
                strRuntimeInfo += "ParentGroup {" + QString(pGraphObjGroup == nullptr ? "null" : pGraphObjGroup->path()) +
                    ", PhysValRect {" + pGraphObjGroup->getRect().toString() + "}" +
                    ", Pos {" + qPoint2Str(ptPosParent) + "}, ScenePos {" + qPoint2Str(ptScenePosParent) + "}" +
                    ", BoundingRect {" + qRect2Str(rectBoundingParent) + "}" +
                    ", Center {" + qPoint2Str(ptCenterPosParent) + "}}";
                i_mthTracer.trace(strRuntimeInfo);
            }
        }
    }
}
