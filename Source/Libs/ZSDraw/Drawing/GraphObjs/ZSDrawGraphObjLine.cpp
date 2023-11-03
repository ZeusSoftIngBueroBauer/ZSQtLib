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
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSDraw/Widgets/GraphObjs/ZSDrawGraphObjLinePropertiesDlg.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"
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
class CGraphObjLine : public CGraphObj, public QGraphicsLineItem
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjLine::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a graphical line object.

    The number of line objects stored in s_iInstCount is increased to create
    a unique line name when creating objects by passing an empty object name.

    @param [in] i_drawSettings
        Current draw settings like pen width, pen color, line style etc. to be used.
    @param [in] i_strObjName
        Name of the graphical object.
        Names of graphical objects must be unique below its parent.
        If an empty string is passed a unique name is created by adding the
        current number of line objects taken from s_iInstCount to "Line".
    @param [in] i_physValPoint1
        Start point of the line.
        Coordinates are assumed to be relative to the parent item the item
        will later be added to as a child (or in scene coordinates if the item
        is added to the scene).
    @param [in] i_physValPoint2
        End point of the line.
        Coordinates are assumed to be relative to the parent item the item
        will later be added to as a child (or in scene coordinates if the item
        is added to the scene).
*/
CGraphObjLine::CGraphObjLine(
    CDrawingScene* i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString& i_strObjName,
    const CPhysValPoint& i_physValPoint1,
    const CPhysValPoint& i_physValPoint2 ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeLine,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeLine),
        /* strObjName          */ i_strObjName.isEmpty() ? "Line" + QString::number(s_iInstCount) : i_strObjName,
        /* drawSettings        */ i_drawSettings ),
    QGraphicsLineItem(),
    m_physValLine(i_physValPoint1, i_physValPoint2),
    m_plgP1ArrowHead(),
    m_plgP2ArrowHead()
{
    m_strlstPredefinedLabelNames.append("Name");
    m_strlstPredefinedLabelNames.append("P1");
    m_strlstPredefinedLabelNames.append("P2");

    for (const QString& strLabelName : m_strlstPredefinedLabelNames) {
        if (!m_hshpLabels.contains(strLabelName)) {
            if (strLabelName == "P1") {
                addLabel(strLabelName, strLabelName, ESelectionPoint::TopLeft);
            }
            else if (strLabelName == "P2") {
                addLabel(strLabelName, strLabelName, ESelectionPoint::BottomRight);
            }
            else {
                addLabel(strLabelName, strLabelName, ESelectionPoint::Center);
            }
        }
    }

    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    // Initialise list with number of selection points.
    // A line may provide two selection points - at start and end of line.
    m_arpSelPtsPolygon.append(nullptr);
    m_arpSelPtsPolygon.append(nullptr);

    createTraceAdminObjs("StandardShapes::" + ClassName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ObjName: " + i_strObjName +
            ", Pt1 {" + i_physValPoint1.toString() + "}" +
            ", Pt2 {" + i_physValPoint2.toString() + "}" +
            ", DrawSettings {" + i_drawSettings.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );
    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsDetailed)) {
        strMthInArgs = "DrawSettings {" + i_drawSettings.toString() + "}";
        mthTracer.trace(strMthInArgs);
    }

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
           | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges);

    // The line coordinates are passed relative to the parent item
    // (usually the scene coordinates or a group item).
    // But the graphics item expects the coordinates based on the
    // local coordinate system of the graphics item.
    // Items live in their own local coordinate system.
    // Their coordinates are usually centered around its center point (0, 0).
    // As the item is not yet added to a parent we got to adjust
    // the coordinates coorespondingly.
    QLineF lineF = m_physValLine.toQLineF();
    if (m_physValLine.unit() != Units.Length.px) {
        lineF = m_pDrawingScene->convert(m_physValLine, Units.Length.px).toQLineF();
    }

    // Center position in parent coordinates.
    QPointF ptPos = lineF.center();

    // Move the points into the item's local coordinate system.
    QPointF p1 = lineF.p1() - ptPos;
    QPointF p2 = lineF.p2() - ptPos;

    // Set the line in local coordinate system.
    QGraphicsLineItem_setLine(QLineF(p1, p2));

    // GraphicsLineItem::setLine does not update the position.
    // Must be done "manually" afterwards.
    // Move the object to the parent position.
    QGraphicsItem_setPos(ptPos);

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Undefined,
            m_pTrcAdminObjCtorsAndDtor->getRuntimeInfoTraceDetailLevel());
    }
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
    return QGraphicsItem::UserType + EGraphObjTypeLine;
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
    CGraphObjLine* pGraphObj = new CGraphObjLine(
        m_pDrawingScene, m_drawSettings, m_strName,
        getP1(drawingSize.unit()), getP2(drawingSize.unit()) );
    return pGraphObj;
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

    m_bForceConversionToSceneCoors = true;
    setLine(m_pDrawingScene->convert(m_physValLine, i_drawingSize.unit()));
    m_bForceConversionToSceneCoors = false;
}

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
public: // replacing methods of QGraphicsLineItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the item's line to the given line.

    @param [in] i_physValLine
        Line to be set.
        Coordinates must be passed relative to it's parent item
        (or in scene coordinates if the item does not have parent).
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
    if (m_bForceConversionToSceneCoors || m_physValLine != i_physValLine)
    {
        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsLineItem::prepareGeometryChange();

        // Please note that the original, untransformed line coordinates m_physValLine
        // kept in the unit of the drawing scene is update in the "itemChange" method.
        // The line may be moved or transformed by other methods. "itemChange" is a
        // central point to update the coordinates upon those changes.
        //setPhysValLine(i_physValLine);

        // The line coordinates are passed relative to the parent item
        // (usually the scene coordinates or a group item).
        // But the graphics item expects the coordinates based on the
        // local coordinate system of the graphics item.
        // Items live in their own local coordinate system.
        // Their coordinates are usually centered around its center point (0, 0).

        QLineF lineF = i_physValLine.toQLineF();
        if (i_physValLine.unit() != Units.Length.px) {
            lineF = m_pDrawingScene->convert(i_physValLine, Units.Length.px).toQLineF();
        }

        // Center position in parent coordinates.
        QPointF ptPos = lineF.center();

        // Move the points into the item's local coordinate system.
        // Please note that "mapFromParent" cannot be used here as
        // the position of the line will be moved.
        QPointF p1 = lineF.p1() - ptPos;
        QPointF p2 = lineF.p2() - ptPos;

        // Set the line in local coordinate system.
        QGraphicsLineItem_setLine(QLineF(p1, p2));

        // GraphicsLineItem::setLine does not update the position.
        // This has to be done "manually" afterwards.

        // Move the object to the parent position.
        // This has to be done after resizing the line as this will trigger
        // an itemChange call which will update the position of the selection
        // points. And to position the selection points the local coordinate
        // system must be up-to-date.
        QGraphicsItem_setPos(ptPos);
    }
}

//------------------------------------------------------------------------------
void CGraphObjLine::setLine(
    double i_fX1, double i_fY1, double i_fX2, double i_fY2, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    setLine(CPhysValLine(i_fX1, i_fY1, i_fX2, i_fY2, i_unit));
}

//------------------------------------------------------------------------------
void CGraphObjLine::setLine(
    const QPointF& i_p1, const QPointF& i_p2, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    setLine(CPhysValLine(i_p1, i_p2, i_unit));
}

//------------------------------------------------------------------------------
void CGraphObjLine::setLine(const QLineF& i_line, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    setLine(CPhysValLine(i_line, i_unit));
}

//------------------------------------------------------------------------------
void CGraphObjLine::setLine(
    const CPhysValPoint& i_physValP1, const CPhysValPoint& i_physValP2)
//------------------------------------------------------------------------------
{
    setLine(CPhysValLine(i_physValP1, i_physValP2));
}

//------------------------------------------------------------------------------
CPhysValLine CGraphObjLine::getLine(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return m_physValLine;
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjLine::getP1(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return m_physValLine.p1();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjLine::getP2(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return m_physValLine.p2();
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::setWidth( const CPhysVal& i_physValWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strMthInArgs );

    setSize(i_physValWidth, getHeight(i_physValWidth.unit()));
}

//------------------------------------------------------------------------------
void CGraphObjLine::setHeight( const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strMthInArgs );

    setSize(getWidth(i_physValHeight.unit()), i_physValHeight);
}

//------------------------------------------------------------------------------
void CGraphObjLine::setSize( const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString() + ", " + i_physValHeight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strMthInArgs );

    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();

    CPhysVal physValWidth = i_physValWidth;
    CPhysVal physValHeight = i_physValHeight;

    physValWidth.convertValue(drawingSize.unit());
    physValHeight.convertValue(drawingSize.unit());

    if (hasFixedWidth() && physValWidth != getFixedWidth(drawingSize.unit())) {
        physValWidth = getFixedWidth(drawingSize.unit());
    }
    else {
        if (hasMinimumWidth() && physValWidth > getMinimumWidth(drawingSize.unit())) {
            physValWidth = getMinimumWidth(drawingSize.unit());
        }
        if (hasMaximumWidth() && physValWidth > getMaximumWidth(drawingSize.unit())) {
            physValWidth = getMaximumWidth(drawingSize.unit());
        }
    }

    if (hasFixedHeight() && physValHeight != getFixedHeight(drawingSize.unit())) {
        physValHeight = getFixedHeight(drawingSize.unit());
    }
    else {
        if (hasMinimumHeight() && physValHeight > getMinimumHeight(drawingSize.unit())) {
            physValHeight = getMinimumHeight(drawingSize.unit());
        }
        if (hasMaximumHeight() && physValHeight > getMaximumHeight(drawingSize.unit())) {
            physValHeight = getMaximumHeight(drawingSize.unit());
        }
    }

    CPhysValLine physValLine = getLine(drawingSize.unit());

    CPhysValPoint physValPoint1 = physValLine.p1();
    CPhysValPoint physValPoint2 = physValLine.p2();

    physValPoint2.setX(physValPoint1.x() + physValWidth);
    physValPoint2.setY(physValPoint1.y() + physValHeight);

    setLine(physValLine);

} // setSize

//------------------------------------------------------------------------------
void CGraphObjLine::setSize( const CPhysValSize& i_physValSize )
//------------------------------------------------------------------------------
{
    setSize(i_physValSize.width(), i_physValSize.height());
}

//------------------------------------------------------------------------------
bool CGraphObjLine::hasBoundingRect() const
//------------------------------------------------------------------------------
{
    return false;
}

//------------------------------------------------------------------------------
bool CGraphObjLine::hasLineShapePoints() const
//------------------------------------------------------------------------------
{
    return false;
}

//------------------------------------------------------------------------------
bool CGraphObjLine::hasRotationSelectionPoints() const
//------------------------------------------------------------------------------
{
    return false;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the position of the item relative to its parent item.
*/
CPhysValPoint CGraphObjLine::getPos( const CUnit& i_unit, ECoordinatesVersion /*i_version*/ ) const
//------------------------------------------------------------------------------
{
    return getLine(i_unit).center();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjLine::getWidth( const CUnit& i_unit, ECoordinatesVersion /*i_version*/ ) const
//------------------------------------------------------------------------------
{
    CPhysValLine physValLine = getLine(i_unit);
    return physValLine.p2().x() - physValLine.p1().x();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjLine::getHeight( const CUnit& i_unit, ECoordinatesVersion /*i_version*/ ) const
//------------------------------------------------------------------------------
{
    CPhysValLine physValLine = getLine(i_unit);
    return physValLine.p2().y() - physValLine.p1().y();
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObjLine::getSize( const CUnit& i_unit, ECoordinatesVersion /*i_version*/ ) const
//------------------------------------------------------------------------------
{
    return CPhysValSize(getWidth(i_unit), getHeight(i_unit));
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::setIsHit( bool i_bHit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = bool2Str(i_bHit);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setIsHit",
        /* strAddInfo   */ strMthInArgs );

    if (m_bIsHit != i_bHit) {
        m_bIsHit = i_bHit;
        if (m_bIsHit) {
            QLineF lineF = line();
            QPolygonF plg;
            plg.append(lineF.p1());
            plg.append(lineF.p2());
            showSelectionPointsOfPolygon(plg);
        }
        else if (!isSelected()) {
            hideSelectionPoints();
        }
        update();
    }
} // setIsHit

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjLine::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Point:" + point2Str(i_pt) +
            ", HitInfo {" + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo)) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjIsHit,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "isHit",
        /* strAddInfo   */ strMthInArgs );

    bool bIsHit = false;

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
    if (pGraphicsItem != nullptr) {
        if (pGraphicsItem->isSelected()) {
            bIsHit = isPolygonSelectionPointHit(i_pt, o_pHitInfo);
        }
        if (!bIsHit) {
            QLineF lineF = line();
            bIsHit = isLineHit(lineF, i_pt, m_pDrawingScene->getHitToleranceInPx());
            if (o_pHitInfo != nullptr) {
                o_pHitInfo->m_editMode = EEditMode::Move;
                o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                o_pHitInfo->m_idxPolygonShapePoint = -1;
                o_pHitInfo->m_idxLineSegment = 0;
                o_pHitInfo->m_ptSelected = i_pt;
            }
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs;
        if (o_pHitInfo != nullptr) {
            strMthOutArgs = "HitInfo {" + o_pHitInfo->toString() + "}";
            mthTracer.setMethodOutArgs(strMthOutArgs);
        }
        mthTracer.setMethodReturn(bIsHit);
    }
    return bIsHit;
}

/*==============================================================================
public: // reimplementing methods of base class QGraphicItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::setCursor( const QCursor& i_cursor )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qCursorShape2Str(i_cursor.shape());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setCursor",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsLineItem::setCursor(i_cursor);
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns coordinates of selection point in item's coordinate system.
*/
QPointF CGraphObjLine::getSelectionPointCoors( ESelectionPoint i_selPt ) const
//------------------------------------------------------------------------------
{
    return ZS::Draw::getSelectionPointCoors(line(), i_selPt);
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

//------------------------------------------------------------------------------
void CGraphObjLine::updateSelectionPoints( unsigned char i_selPts )
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
        /* strMethod    */ "updateSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    if (parentItem() == nullptr) {
        QLineF lineF = line();
        QPolygonF plg;
        plg.append(lineF.p1());
        plg.append(lineF.p2());
        updateSelectionPointsOfPolygon(plg);
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj
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
QList<ESelectionPoint> CGraphObjLine::getPossibleLabelAnchorPoints(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    static const QHash<QString, QList<ESelectionPoint>> s_hshSelPtsPredefinedNames = {
        { "Name", {ESelectionPoint::Center} },
        { "P1", {ESelectionPoint::TopLeft} },       // Start point of the line
        { "P2", {ESelectionPoint::BottomRight} }    // End point of the line
    };
    static const QList<ESelectionPoint> s_arSelPtsUserDefined = {
        ESelectionPoint::Center,
        ESelectionPoint::TopLeft,       // Start point of the line
        ESelectionPoint::BottomRight    // End point of the line
    };
    if (s_hshSelPtsPredefinedNames.contains(i_strName)) {
        return s_hshSelPtsPredefinedNames[i_strName];
    }
    return s_arSelPtsUserDefined;
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

    QRectF rctBounding = QGraphicsLineItem::boundingRect();

    for (int idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++) {
        CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
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

    //if (m_bIsHit || isSelected()) {
    //    QHashIterator<QString, CGraphObjLabel*> itLabels(m_arpLabels);
    //    CGraphObjLabel* pGraphObjLabel;
    //    QRectF          rctLabel;
    //    QPolygonF       plgLabel;

    //    while( itLabels.hasNext() )
    //    {
    //        itLabels.next();
    //        pGraphObjLabel = itLabels.value();

    //        if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
    //        {
    //            // Calling pGraphObjLabel->boundingRect() may lead to endless recursion as the
    //            // label itself may call the boundingRect method of its parent item (which is
    //            // this item) if the label is selected or has been hit. For this we call
    //            // boundingRect of the label with a different signature to indicate that we
    //            // are only interested in the bounding rect of the simple text item.
    //            rctLabel = pGraphObjLabel->m_pGraphObjLabel->boundingRect(true);
    //            plgLabel = mapFromItem( pGraphObjLabel->m_pGraphObjLabel, rctLabel );
    //            rctBounding |= plgLabel.boundingRect();
    //        }
    //    }
    //}

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qRect2Str(rctBounding);
        mthTracer.setMethodReturn(strMthRet);
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
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ "" );

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter,
            m_pTrcAdminObjPaint->getRuntimeInfoTraceDetailLevel());
    }

    i_pPainter->save();

    QPen pn = pen();
    pn.setColor(m_drawSettings.getPenColor());
    pn.setWidth(m_drawSettings.getPenWidth());
    pn.setStyle(lineStyle2QtPenStyle(m_drawSettings.getLineStyle().enumerator()));

    if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
        //if (m_editMode == EEditMode::Move)
        //{
            pn.setStyle(Qt::DotLine);
            pn.setColor(Qt::blue);
        //}
    }

    i_pPainter->setPen(pn);

    QLineF lineF = line();
    i_pPainter->drawLine(lineF);

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

    if (m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected())) {
        pn.setStyle(Qt::DotLine);
        pn.setColor(Qt::blue);
        i_pPainter->setPen(pn);
        i_pPainter->setBrush(Qt::NoBrush);

        //QHashIterator<QString, CGraphObjLabel*> itLabels(m_arpLabels);
        //while (itLabels.hasNext()) {
        //    itLabels.next();
        //    CGraphObjLabel* pGraphObjLabel = itLabels.value();
        //    if (pGraphObjLabel->m_pGraphObjLabel != nullptr) {
        //        QPointF ptSelPt = getSelectionPointCoors(pGraphObjLabel->m_selPt.enumerator());
        //        QPointF ptLabelSelPt = pGraphObjLabel->m_pGraphObjLabel->getSelectionPointCoors(ESelectionPoint::Center);
        //        ptLabelSelPt = mapFromItem(pGraphObjLabel->m_pGraphObjLabel, ptLabelSelPt);
        //        i_pPainter->drawLine(ptSelPt, ptLabelSelPt);
        //    }
        //}
    }

    i_pPainter->restore();

} // paint

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Watches mouse events of selection points to forward those
          events to this item in order to resize and rotate this item.
*/
bool CGraphObjLine::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CGraphObjSelectionPoint* pGraphObjSelPtWatched = dynamic_cast<CGraphObjSelectionPoint*>(i_pGraphicsItemWatched);

    if (pGraphObjSelPtWatched == nullptr) {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObjSelPtWatched == nullptr" );
    }

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjSceneEventFilter, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ItemWatched: " + pGraphObjSelPtWatched->name();
        if (isGraphicsSceneHoverEvent(i_pEv)) {
            QGraphicsSceneHoverEvent* pHoverEvent = dynamic_cast<QGraphicsSceneHoverEvent*>(i_pEv);
            strMthInArgs += ", " + qEventType2Str(i_pEv->type()) + " {" + qGraphicsSceneHoverEvent2Str(pHoverEvent) + "}";
        }
        else if (isGraphicsSceneMouseEvent(i_pEv)) {
            QGraphicsSceneMouseEvent* pMouseEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            strMthInArgs += ", " + qEventType2Str(i_pEv->type()) + " {" + qGraphicsSceneMouseEvent2Str(pMouseEvent) + "}";
        }
        else {
            strMthInArgs += ", Event: " + qEventType2Str(i_pEv->type());
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjSceneEventFilter,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "sceneEventFilter",
        /* strAddInfo   */ strMthInArgs );

    bool bEventHandled = false;

    switch (i_pEv->type())
    {
        case QEvent::GraphicsSceneMouseMove:
        {
            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            if (pEv != nullptr) {
                QPointF ptEvPos;
                for (int btns = Qt::MouseButton::LeftButton; btns <= Qt::MouseButton::ForwardButton; btns <<= 1) {
                    Qt::MouseButton button = Qt::MouseButton(btns);
                    ptEvPos = mapFromItem(pGraphObjSelPtWatched, pEv->buttonDownPos(button));
                    pEv->setButtonDownPos(button, ptEvPos);
                }
                ptEvPos = mapFromItem(pGraphObjSelPtWatched, pEv->pos());
                pEv->setPos(ptEvPos);
                ptEvPos = mapFromItem(pGraphObjSelPtWatched, pEv->lastPos());
                pEv->setLastPos(ptEvPos);
                mouseMoveEvent(pEv);
                bEventHandled = true;
            }
            break;
        }
        case QEvent::GraphicsSceneMousePress:
        {
            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            if (pEv != nullptr) {
                QPointF ptEvPos;
                for (int btns = Qt::MouseButton::LeftButton; btns <= Qt::MouseButton::ForwardButton; btns <<= 1) {
                    Qt::MouseButton button = Qt::MouseButton(btns);
                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
                    pEv->setButtonDownPos( button, ptEvPos );
                }
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
                pEv->setPos(ptEvPos);
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
                pEv->setLastPos(ptEvPos);
                mousePressEvent(pEv);
                bEventHandled = true;
            }
            break;
        }
        case QEvent::GraphicsSceneMouseRelease:
        {
            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            if (pEv != nullptr) {
                QPointF ptEvPos;
                for (int btns = Qt::MouseButton::LeftButton; btns <= Qt::MouseButton::ForwardButton; btns <<= 1) {
                    Qt::MouseButton button = Qt::MouseButton(btns);
                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
                    pEv->setButtonDownPos( button, ptEvPos );
                }
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
                pEv->setPos(ptEvPos);
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
                pEv->setLastPos(ptEvPos);
                mouseReleaseEvent(pEv);
                bEventHandled = true;
            }
            break;
        }
        case QEvent::GraphicsSceneMouseDoubleClick:
        {
            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            if (pEv != nullptr) {
                QPointF ptEvPos;
                for (int btns = Qt::MouseButton::LeftButton; btns <= Qt::MouseButton::ForwardButton; btns <<= 1) {
                    Qt::MouseButton button = Qt::MouseButton(btns);
                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
                    pEv->setButtonDownPos( button, ptEvPos );
                }
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
                pEv->setPos(ptEvPos);
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
                pEv->setLastPos(ptEvPos);
                mouseDoubleClickEvent(pEv);
                bEventHandled = true;
            }
            break;
        }
        case QEvent::GraphicsSceneContextMenu:
        case QEvent::GraphicsSceneHoverEnter:
        case QEvent::GraphicsSceneHoverMove:
        case QEvent::GraphicsSceneHoverLeave:
        case QEvent::GraphicsSceneHelp:
        case QEvent::GraphicsSceneDragEnter:
        case QEvent::GraphicsSceneDragMove:
        case QEvent::GraphicsSceneDragLeave:
        case QEvent::GraphicsSceneDrop:
        case QEvent::GraphicsSceneWheel:
        default:
        {
            break;
        }
    } // switch( i_pEv->type() )

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

    QPointF ptItemPos = i_pEv->pos();

    CEnumMode modeDrawing = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if (modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select) {
        SGraphObjHitInfo hitInfo;
        bool bIsHit = isHit(ptItemPos, &hitInfo);
        if (bIsHit) {
            if (cursor().shape() != hitInfo.m_cursor.shape()) {
                setCursor(hitInfo.m_cursor);
            }
        }
    }
} // hoverEnterEvent

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

    QPointF ptItemPos = i_pEv->pos();

    CEnumMode modeDrawing = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if (modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select) {
        SGraphObjHitInfo hitInfo;
        bool bIsHit = isHit(ptItemPos, &hitInfo);
        if (bIsHit) {
            if (cursor().shape() != hitInfo.m_cursor.shape()) {
                setCursor(hitInfo.m_cursor);
            }
        }
    }
} // hoverMoveEvent

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
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter,
            m_pTrcAdminObjMouseClickEvents->getRuntimeInfoTraceDetailLevel());
    }

    CEnumMode modeDrawing = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if (modeDrawing == EMode::Edit) {
        if (editToolDrawing == EEditTool::CreateObjects && m_editMode == EEditMode::Creating) {
            QGraphicsLineItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)
            m_idxSelPtSelectedPolygon = 1;
            if (m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon] != nullptr) {
                m_arpSelPtsPolygon[m_idxSelPtSelectedPolygon]->setSelected(true);
            }
            updateEditInfo();
            updateToolTip();
        }
        else if (editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None) {
            QGraphicsLineItem::mousePressEvent(i_pEv); // this will select the item (creating selection points)
            SGraphObjHitInfo hitInfo;
            bool bIsHit = isHit(i_pEv->pos(), &hitInfo);
            m_editMode = hitInfo.m_editMode;
            m_editResizeMode = hitInfo.m_editResizeMode;
            m_idxSelPtSelectedPolygon = hitInfo.m_idxPolygonShapePoint;
            for (int idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++) {
                CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];
                if (pGraphObjSelPt != nullptr) {
                    if (idxSelPt == m_idxSelPtSelectedPolygon) {
                        pGraphObjSelPt->setSelected(true);
                    }
                    else {
                        pGraphObjSelPt->setSelected(false);
                    }
                }
            }
            m_pDrawingScene->setMode( EMode::Ignore, EEditTool::Ignore, m_editMode, m_editResizeMode, false );
            updateEditInfo();
            updateToolTip();
        }
    }
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave,
            m_pTrcAdminObjMouseClickEvents->getRuntimeInfoTraceDetailLevel());
    }
} // mousePressEvent

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
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter,
            m_pTrcAdminObjMouseMoveEvents->getRuntimeInfoTraceDetailLevel());
    }

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if (modeDrawing == EMode::Edit) {
        if (m_editMode == EEditMode::Creating || m_editMode == EEditMode::MoveShapePoint) {
            QRectF sceneRect = m_pDrawingScene->sceneRect();
            QPointF posEv = i_pEv->pos();
            if (sceneRect.contains(mapToScene(posEv))) {
                posEv = mapToParent(posEv);
                CPhysValPoint physValPosEv = m_pDrawingScene->toPhysValPoint(posEv);
                CPhysValLine physValLine = m_physValLine;
                if (m_idxSelPtSelectedPolygon == 0) {
                    physValLine.setP1(physValPosEv);
                }
                else if (m_idxSelPtSelectedPolygon == 1) {
                    physValLine.setP2(physValPosEv);
                }
                setLine(physValLine);
            }
        }
        else if (m_editMode == EEditMode::Move) {
            QGraphicsLineItem::mouseMoveEvent(i_pEv);
            updateEditInfo();
            updateToolTip();
        }
        else if (m_editMode == EEditMode::Resize) {
        }
        else if (m_editMode == EEditMode::Rotate) {
        }
        else if (m_editMode == EEditMode::EditText) {
        }
    }
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave,
            m_pTrcAdminObjMouseMoveEvents->getRuntimeInfoTraceDetailLevel());
    }
} // mouseMoveEvent

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
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter,
            m_pTrcAdminObjMouseClickEvents->getRuntimeInfoTraceDetailLevel());
    }

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if (modeDrawing == EMode::Edit) {
        if (m_editMode == EEditMode::Creating) {
            // The object has been initially created.
            m_pDrawingScene->onGraphObjCreationFinished(this);
        }
        else if (m_editMode == EEditMode::Move) {
            updateEditInfo();
            updateToolTip();
        }
        else if (m_editMode == EEditMode::Resize) {
        }
        else if (m_editMode == EEditMode::Rotate) {
        }
        else if (m_editMode == EEditMode::MoveShapePoint) {
        }
        else if (m_editMode == EEditMode::EditText) {
        }

        m_editMode = EEditMode::None;
        m_editResizeMode = EEditResizeMode::None;
        m_idxSelPtSelectedPolygon = -1;
        m_selPtSelectedBoundingRect = ESelectionPoint::None;
    }

    // The mouse release event would select the object.
    // This is not wanted if the selection tool is not active.
    bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
    bool bIsSelectableReset = false;

    if (bIsSelectable && m_pDrawingScene->getEditTool() != EEditTool::Select) {
        setFlag(QGraphicsItem::ItemIsSelectable, false);
        bIsSelectableReset = true;
    }

    QGraphicsLineItem::mouseReleaseEvent(i_pEv);

    if (bIsSelectableReset) {
        setFlag(QGraphicsItem::ItemIsSelectable, bIsSelectable);
    }
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave,
            m_pTrcAdminObjMouseClickEvents->getRuntimeInfoTraceDetailLevel());
    }
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

    // When doubleclicking an item, the item will first receive a mouse
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
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Enter,
            m_pTrcAdminObjItemChange->getRuntimeInfoTraceDetailLevel());
    }

    QVariant valChanged = i_value;

    bool bGeometryChanged = false;
    bool bSelectedChanged = false;
    bool bZValueChanged = false;
    bool bTreeEntryChanged = false;

    if (i_change == ItemSceneHasChanged)
    {
        // The item may have been removed from the scene.
        if (scene() != nullptr) {
            bGeometryChanged = true;
        }
    }
    else if (i_change == ItemSelectedHasChanged) {
        prepareGeometryChange();
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            // Does not set "m_fZValue" as it is used to restore
            // the stacking order on deselecting the object.
            bringToFront(); 
            showSelectionPoints();
            // Not necessary to bring selection points to front as item has been already brought
            // to front and "showSelectionPoints" sets zValue of selection points above item.
            //bringSelectionPointsToFront();
            setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
        }
        else {
            setAcceptedMouseButtons(Qt::NoButton);
            hideSelectionPoints();
            setZValue(m_fZValue); // restore ZValue as before selecting the object
            m_editMode = EEditMode::None;
            m_editResizeMode = EEditResizeMode::None;
            m_selPtSelectedBoundingRect = ESelectionPoint::None;
            m_idxSelPtSelectedPolygon = -1;
        }
        updateEditInfo();
        updateToolTip();
        bSelectedChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemPositionHasChanged
          || i_change == ItemTransformHasChanged
          || i_change == ItemParentHasChanged
          || i_change == ItemScenePositionHasChanged
          || i_change == ItemRotationHasChanged
          || i_change == ItemScaleHasChanged
          || i_change == ItemTransformOriginPointHasChanged)
    {
        QLineF lineF = line();
        QPointF ptPos = pos();

        // Update the original, untransformed line coordinates kept in the unit of
        // the drawing scene. The line may be moved or transformed by several methods.
        // "itemChange" is a central point to update the coordinates upon those changes.
        QPointF p1 = mapToParent(lineF.p1());
        QPointF p2 = mapToParent(lineF.p2());
        CPhysValPoint physValP1 = m_pDrawingScene->toPhysValPoint(p1);
        CPhysValPoint physValP2 = m_pDrawingScene->toPhysValPoint(p2);
        setPhysValLine(CPhysValLine(physValP1, physValP2));

        QPolygonF plg;
        plg.append(lineF.p1());
        plg.append(lineF.p2());
        updateSelectionPointsOfPolygon(plg);
        updateLineEndArrowHeadPolygons();
        updateEditInfo();
        updateToolTip();

        bGeometryChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemZValueHasChanged) {
        bZValueChanged = true;
        bTreeEntryChanged = true;
    }

    if (bSelectedChanged) {
        emit_selectedChanged();
    }
    if (bGeometryChanged) {
        emit_geometryChanged();
    }
    if (bZValueChanged) {
        emit_zValueChanged();
    }
    if (bTreeEntryChanged && m_pTree != nullptr) {
        m_pTree->onTreeEntryChanged(this);
    }

    valChanged = QGraphicsItem::itemChange(i_change, i_value);

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        traceInternalStates(mthTracer, EMethodDir::Leave,
            m_pTrcAdminObjItemChange->getRuntimeInfoTraceDetailLevel());
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthRet = qGraphicsItemChange2Str(i_change, valChanged, false);
        mthTracer.setMethodReturn(strMthRet);
    }
    return valChanged;
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

//------------------------------------------------------------------------------
void CGraphObjLine::updateToolTip()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateToolTip",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if (pGraphicsItem != nullptr)
    {
        QString strNodeSeparator = CDrawingScene::getGraphObjNameNodeSeparator();
        QLineF  lineF = line();
        QPointF ptPos;

        m_strToolTip  = "ObjPath:\t" + path();

        m_strToolTip += "\nP1:\t\t" + point2Str(lineF.p1());
        m_strToolTip += "\nP2:\t\t" + point2Str(lineF.p2());

        // "scenePos" returns mapToScene(0,0). This is NOT equivalent to the
        // position of the item's top left corner before applying the rotation
        // transformation matrix but includes the transformation. What we want
        // (or what I want) is the position of the item before rotating the item
        // around the rotation origin point. In contrary it looks like "pos"
        // always returns the top left corner before rotating the object.

        if( pGraphicsItem->parentItem() != nullptr )
        {
            ptPos = pGraphicsItem->pos();
            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
        }
        else
        {
            ptPos = pGraphicsItem->pos(); // don't use "scenePos" here (see comment above)
            m_strToolTip += "\nPos:\t\t" + point2Str(ptPos);
        }

        m_strToolTip += "\nSize:\t\t" + getSize(Units.Length.px).toString();
        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());

        pGraphicsItem->setToolTip(m_strToolTip);
    }
} // updateToolTip

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjLine::setPhysValLine(const CPhysValLine& i_physValLine)
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
        /* strMethod    */ "setPhysValLine",
        /* strAddInfo   */ strMthInArgs );

    m_physValLine = i_physValLine;
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
void CGraphObjLine::traceInternalStates(
    CMethodTracer& i_mthTracer,
    EMethodDir i_mthDir,
    ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        QString strTrcInfo;
        if (i_mthDir == EMethodDir::Enter) strTrcInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strTrcInfo = "+- ";
        else strTrcInfo = "";
        strTrcInfo +=
            "Geometry: GraphObj {" + m_physValLine.toString() + "}"
            ", Item { Pos {" + point2Str(pos()) + "}, Line {" + line2Str(line()) + "}";
        i_mthTracer.trace(strTrcInfo);
        if (i_mthDir == EMethodDir::Enter) strTrcInfo = "-+ ";
        else if (i_mthDir == EMethodDir::Leave) strTrcInfo = "+- ";
        else strTrcInfo = "";
        strTrcInfo += "Selected: " + bool2Str(isSelected());
        i_mthTracer.trace(strTrcInfo);
        CGraphObj::traceInternalStates(i_mthTracer, i_mthDir, i_detailLevel);
    }
}
