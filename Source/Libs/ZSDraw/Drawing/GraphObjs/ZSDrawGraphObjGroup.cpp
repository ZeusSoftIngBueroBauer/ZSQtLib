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

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjGroup.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
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
using namespace ZS::System::GUI;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CGraphObjGroup : public CGraphObj, public QGraphicsItemGroup
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjGroup::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjGroup::CGraphObjGroup(
    CDrawingScene* i_pDrawingScene, const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeGroup,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeGroup),
        /* strObjName          */ i_strObjName.isEmpty() ? "Group" + QString::number(s_iInstCount) : i_strObjName),
    QGraphicsItemGroup(),
    m_divLinesMetricsX("Group" + i_strObjName, EScaleAxis::X),
    m_divLinesMetricsY("Group" + i_strObjName, EScaleAxis::Y),
    m_gridSettings("Group" + i_strObjName),
    m_rectOrig()
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

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable
           | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton | Qt::XButton1 | Qt::XButton2);
    setAcceptHoverEvents(true);

} // ctor

/*==============================================================================
protected: // ctor (used by derived classes)
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjGroup::CGraphObjGroup(
    CDrawingScene* i_pDrawingScene,
    const QString& i_strFactoryGroupName,
    EGraphObjType i_type,
    const QString& i_strType,
    const QString& i_strObjName) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ i_strFactoryGroupName,
        /* type                */ i_type,
        /* strType             */ i_strType,
        /* strObjName          */ i_strObjName),
    QGraphicsItemGroup(),
    m_divLinesMetricsX(i_strType + i_strObjName, EScaleAxis::X),
    m_divLinesMetricsY(i_strType + i_strObjName, EScaleAxis::Y),
    m_gridSettings(i_strType + i_strObjName),
    m_rectOrig()
{
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjGroup::~CGraphObjGroup()
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

    //m_divLinesMetricsX;
    //m_divLinesMetricsY;
    //m_gridSettings;
    //m_rectOrig;

} // dtor

/*==============================================================================
public: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the type method of QGraphicsItem.
*/
int CGraphObjGroup::type() const
//------------------------------------------------------------------------------
{
    return EGraphObjTypeGroup;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjGroup::clone()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ "" );

    CGraphObjGroup* pGraphObj = new CGraphObjGroup(m_pDrawingScene, m_strName);
    pGraphObj->setDrawSettings(m_drawSettings);

    QList<QGraphicsItem*> arpGraphicsItemsChilds = childItems();
    for (int idxGraphObjChild = 0; idxGraphObjChild < arpGraphicsItemsChilds.size(); idxGraphObjChild++) {
        CGraphObj* pGraphObjChild = dynamic_cast<CGraphObj*>(arpGraphicsItemsChilds[idxGraphObjChild]);
        if (pGraphObjChild != nullptr) {
            CGraphObj* pGraphObjChildClone = pGraphObjChild->clone();
            if (pGraphObjChildClone != nullptr) {
                throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
                //pGraphObj->addGraphObj(pGraphObjChildClone);
            }
        }
    }

    QPointF ptPos = pos();
    pGraphObj->setPos(ptPos);
#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    pGraphObj->setRotationAngleInDegree(m_fRotAngleCurr_deg);
    //pGraphObj->setScaleFactors(m_fScaleFacXCurr,m_fScaleFacYCurr);
    pGraphObj->acceptCurrentAsOriginalCoors();
#endif

    return pGraphObj;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::setGridSettings( const CDrawGridSettings& i_gridSettings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_gridSettings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setGridSettings",
        /* strAddInfo   */ strMthInArgs );

    if( m_gridSettings != i_gridSettings )
    {
        m_gridSettings = i_gridSettings;

        QFont font = m_gridSettings.labelsFont();
        CEnumTextStyle textStyle = m_gridSettings.labelsTextStyle();
        CEnumTextEffect textEffect = m_gridSettings.labelsTextEffect();
        ETextSize textSize = m_gridSettings.labelsTextSize();

        if (isTextStyleBold(textStyle)) {
            font.setBold(true);
        }
        if (isTextStyleItalic(textStyle)) {
            font.setItalic(true);
        }
        if (isTextEffectUnderline(textEffect)) {
            font.setUnderline(true);
        }
        if (isTextEffectStrikeout(textEffect)) {
            font.setStrikeOut(true);
        }
        font.setPixelSize(textSize2SizeInPixels(textSize));

        m_divLinesMetricsX.setDivLinesDistMinInPix(EDivLineLayer::Main, m_gridSettings.linesDistMin());
        m_divLinesMetricsX.setFont(font);
        //m_divLinesMetricsX.setDigitsCountMax(0);
        //m_divLinesMetricsX.setUseEngineeringFormat(false);
        //m_divLinesMetricsX.setDivLineLabelsMinTextExtent(QSize(0, 0));
        m_divLinesMetricsX.update();

        m_divLinesMetricsY.setDivLinesDistMinInPix(EDivLineLayer::Main, m_gridSettings.linesDistMin());
        m_divLinesMetricsY.setFont(font);
        //m_divLinesMetricsY.setDigitsCountMax(0);
        //m_divLinesMetricsY.setUseEngineeringFormat(false);
        //m_divLinesMetricsY.setDivLineLabelsMinTextExtent(QSize(0, 0));
        m_divLinesMetricsY.update();

        update();

        emit_gridSettingsChanged(m_gridSettings);
    }
}

//------------------------------------------------------------------------------
const CDrawGridSettings& CGraphObjGroup::gridSettings() const
//------------------------------------------------------------------------------
{
    return m_gridSettings;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Adds the given item and item's child items to this item group.

    The item and child items will be reparented to this group, but its position
    and transformation relative to the scene will stay intact.

    @Note This method replaces QGraphicsItemGroup::addtoGroup to trace the method call.
*/
void CGraphObjGroup::addToGroup( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "null" : i_pGraphObj->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "addToGroup",
        /* strAddInfo   */ strMthInArgs );
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);
    QGraphicsItemGroup::addToGroup(pGraphicsItem);
}

//------------------------------------------------------------------------------
/*! @brief Removes the specified item from this group.

    The item will be reparented to this group's parent item, or to 0 if this group
    has no parent. Its position and transformation relative to the scene will stay intact.

    @Note This method replaces QGraphicsItemGroup::removeFromGroup to trace the method call.
*/
void CGraphObjGroup::removeFromGroup( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pGraphObj == nullptr ? "null" : i_pGraphObj->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "removeFromGroup",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);
    QGraphicsItemGroup::removeFromGroup(pGraphicsItem);
}

//------------------------------------------------------------------------------
CGraphObj* CGraphObjGroup::findGraphObj( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CGraphObj*>(find(i_strObjName));
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjGroup::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    QString strScenePolygonShapePoints;
    QPolygonF plgScene = mapToScene(getBoundingRect());
    strScenePolygonShapePoints = polygon2Str(plgScene);
    return strScenePolygonShapePoints;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the item's rectangle to the given rectangle.

    Depending on the Y scale orientation of the drawing scene the line coordinates
    must be passed either relative to the top left or relative to the bottom right
    corner of the parent item's bounding rectangle.

    @param [in] i_physValRect
        Rectangle to be set.
*/
void CGraphObjGroup::setRect( const CPhysValRect& i_physValRect )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValRect.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );

    // Depending on the Y scale orientation of the drawing scene the rectangle coordinates
    // have been passed either relative to the top left or bottom right corner of the
    // parent item's bounding rectangle.
    // The coordinates need to be transformed into the local coordinate system of the graphical
    // object whose origin point is the center of the objects bounding rectangle.
    CPhysValRect physValRect = i_physValRect;

    if (Units.Length.isMetricUnit(physValRect.unit())) {
        QGraphicsItem* pGraphicsItemParent = parentItem();
        CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphicsItemParent);
        // If the item belongs to a group ...
        if (pGraphObjGroup != nullptr) {
            physValRect = pGraphObjGroup->convert(physValRect, Units.Length.px);
        }
        // If the item is not a child of a group ...
        else {
            physValRect = m_pDrawingScene->convert(physValRect, Units.Length.px);
        }
    }

    QRectF rectF = physValRect.toQRectF();

    // The local coordinates of the item are in pixels relative to the center point
    // of the item's bounding rectangle. To provide the coordinates to the user they
    // got to be mapped, depending on the Y-Scale orientation, either to the top left
    // or bottom left corner of parent's (or scene's) bounding rectangle.
    // The conversions from pixels to metric units and vice versa are using the
    // divLinesMetrics instances whose scale values got to be updated correspondingly.
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    if (drawingSize.dimensionUnit() == EScaleDimensionUnit::Pixels) {
        // Just a small note about pixel range and min and max values:
        // If you don't use a metric system like in drawings and define
        // a 500 pixel range, min is at 0, max is at 499. To have min
        // and max set to 0 and 500 a range of 501 pixels must be defined.
        // Pixel drawing: the origin is at the top left corner:
        // XScaleMin = XMin_px, XScaleMax = XMax_px
        // YScaleMin = XMin_px, YScaleMax = XMax_px
        // The greater the value, the greater the pixel coordinate on the screen.
        m_divLinesMetricsX.setUseWorldCoordinateTransformation(false);
        m_divLinesMetricsX.setScale(
            /* fScaleMinVal */ 0.0,
            /* fScaleMaxVal */ rectF.width() - 1,
            /* fScaleResVal */ drawingSize.imageCoorsResolution(Units.Length.px).getVal());
        // The Y scale direction is from top to bottom.
        m_divLinesMetricsY.setUseWorldCoordinateTransformation(false);
        m_divLinesMetricsY.setScale(
            /* fScaleMinVal */ 0.0,
            /* fScaleMaxVal */ rectF.height() - 1,
            /* fScaleResVal */ drawingSize.imageCoorsResolution(Units.Length.px).getVal());
        m_divLinesMetricsY.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);
    }
    else /*if (i_drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric)*/ {
        // In order to draw division lines at min and max scale the width
        // in pixels got to be extended by one pixel when using metric scales
        // (see also documentation at class CScaleDivLines). This must have
        // been taken into account by the CDrawingSize class when calculating
        // the width and height of the image size in pixels.
        // Metric units:
        // Depending on the YScaleAxisOrientation the origin is either
        // at the top left or bottom left corner.
        // XScaleMin = XMin_px, XScaleMax = XMax_px
        // YScaleMin = XMax_px, YScaleMax = XMin_px
        // The greater the value, the less the pixel coordinate on the screen.
        m_divLinesMetricsX.setUseWorldCoordinateTransformation(true);
        m_divLinesMetricsX.setScale(
            /* fScaleMinVal */ 0.0,
            /* fScaleMaxVal */ physValRect.width().getVal(),
            /* fScaleResVal */ drawingSize.imageCoorsResolution(drawingSize.unit()).getVal(),
            /* fMin_px      */ 0,
            /* fMax_px      */ rectF.width() - 1);
        m_divLinesMetricsY.setUseWorldCoordinateTransformation(true);
        m_divLinesMetricsY.setScale(
            /* fScaleMinVal */ 0.0,
            /* fScaleMaxVal */ physValRect.height().getVal(),
            /* fScaleResVal */ drawingSize.imageCoorsResolution(drawingSize.unit()).getVal(),
            /* fMin_px      */ 0,
            /* fMax_px      */ rectF.height() - 1);
        m_divLinesMetricsY.setYScaleAxisOrientation(drawingSize.yScaleAxisOrientation());
    }
    m_divLinesMetricsX.update();
    m_divLinesMetricsY.update();

    // Position of the rectangle in parent (scene or group) coordinates.
    QPointF ptPos = rectF.center();

    // Transform the parent coordinates into local coordinate system.
    // The origin is the center point of the rectangle.
    QPointF ptTL = rectF.topLeft() - ptPos;
    QPointF ptBR = rectF.bottomRight() - ptPos;
    rectF = QRectF(ptTL, ptBR);

    // If the coordinates MUST be updated
    // (initially set, changed or after the drawing size has been changed)
    if (m_rectOrig.isNull() || m_rectOrig != rectF || m_bForceConversionToSceneCoors)
    {
        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsItemGroup::prepareGeometryChange();

        {   CRefCountGuard refCountGuard(&m_iItemChangeUpdateOriginalCoorsBlockedCounter);

            // Store original rectangle coordinates.
            setRectOrig(rectF);

            // Move the object to the parent position.
            // This has to be done after resizing the line which updates the local coordinates
            // of the line with origin (0/0) at the lines center point.
            // "setPos" will trigger an itemChange call which will update the position of the
            // selection points and labels. To position the selection points and labels correctly
            // the local coordinate system must be up-to-date.
            // Also note that itemChange must not overwrite the current line value (refCountGuard).
            QGraphicsItem_setPos(ptPos);

            //updateInternalScenePos();
        }

        applyGeometryChangeToChildrens();
    }
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's rectangle coordinates either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the current unit of the drawing scene.
*/
CPhysValRect CGraphObjGroup::getRect() const
//------------------------------------------------------------------------------
{
    return getRect(m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Depending on the Y scale orientation of the drawing scene returns the
           item's rectangle coordinates either relative to the top left corner or
           relative to the bottom right corner of the parent's bounding rectangle
           in the given unit.

    @param [in] i_unit
        Unit in which the line coordinates should be returned.
*/
CPhysValRect CGraphObjGroup::getRect(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    // Object shape points in local coordinates.
    QRectF rectF = getBoundingRect();
    return mapToPhysValRect(rectF);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setWidth(double i_fWidth)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fWidth);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setWidth(i_fWidth);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setWidth(const CPhysVal& i_physValWidth)
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

    CPhysValRect physValRect = getRect();
    physValRect.setWidth(i_physValWidth);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getWidth() const
//------------------------------------------------------------------------------
{
    return getRect().width();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).width();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setHeight(double i_fHeight)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fHeight);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setHeight(i_fHeight);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setHeight(const CPhysVal& i_physValHeight)
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

    CPhysValRect physValRect = getRect();
    physValRect.setHeight(i_physValHeight);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getHeight() const
//------------------------------------------------------------------------------
{
    return getRect().height();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).height();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setSize(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qSize2Str(i_size) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setSize(i_size);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setSize(const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValSize.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setSize(i_physValSize);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObjGroup::getSize() const
//------------------------------------------------------------------------------
{
    return getRect().size();
}

//------------------------------------------------------------------------------
CPhysValSize CGraphObjGroup::getSize(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).size();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::moveCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "moveCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.moveCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::moveCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "moveCenter",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.moveCenter(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjGroup::getCenter() const
//------------------------------------------------------------------------------
{
    return getRect().center();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjGroup::getCenter(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).center();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setTop(double i_fTop)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fTop);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setTop",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTop(i_fTop);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setTop(const CPhysVal& i_physValTop)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValTop.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setTop",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTop(i_physValTop);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getTop() const
//------------------------------------------------------------------------------
{
    return getRect().top();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getTop(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).top();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setBottom(double i_fBottom)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fBottom);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setBottom",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottom(i_fBottom);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setBottom(const CPhysVal& i_physValBottom)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValBottom.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setBottom",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottom(i_physValBottom);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getBottom() const
//------------------------------------------------------------------------------
{
    return getRect().bottom();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getBottom(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).bottom();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setLeft(double i_fLeft)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fLeft);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setLeft(i_fLeft);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setLeft(const CPhysVal& i_physValLeft)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValLeft.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setLeft(i_physValLeft);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getLeft() const
//------------------------------------------------------------------------------
{
    return getRect().left();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getLeft(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).left();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setRight(double i_fRight)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fRight);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setRight(i_fRight);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setRight(const CPhysVal& i_physValRight)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValRight.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setRight(i_physValRight);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getRight() const
//------------------------------------------------------------------------------
{
    return getRect().right();
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::getRight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).right();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setTopLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setTopLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopLeft(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setTopLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setTopLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopLeft(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjGroup::getTopLeft() const
//------------------------------------------------------------------------------
{
    return getRect().topLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjGroup::getTopLeft(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).topLeft();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setTopRight(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setTopRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopRight(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setTopRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setTopRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setTopRight(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjGroup::getTopRight() const
//------------------------------------------------------------------------------
{
    return getRect().topRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjGroup::getTopRight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).topRight();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setBottomRight(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setBottomRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomRight(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setBottomRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setBottomRight",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomRight(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjGroup::getBottomRight() const
//------------------------------------------------------------------------------
{
    return getRect().bottomRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjGroup::getBottomRight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).bottomRight();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setBottomLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qPoint2Str(i_pt) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setBottomLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomLeft(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setBottomLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValPoint.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setBottomLeft",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRect = getRect();
    physValRect.setBottomLeft(i_physValPoint);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjGroup::getBottomLeft() const
//------------------------------------------------------------------------------
{
    return getRect().bottomLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CGraphObjGroup::getBottomLeft(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).bottomLeft();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Converts the given point value into the current unit of the drawing scene.

    @param [in] i_physValPoint

    @return Converted value.
*/
CPhysValPoint CGraphObjGroup::convert(const CPhysValPoint& i_physValPoint) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(i_physValPoint, drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given point value into the desired unit.

    @param [in] i_physValPoint
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValPoint CGraphObjGroup::convert(const CPhysValPoint& i_physValPoint, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysValPoint physValPoint = i_physValPoint;
    if (i_physValPoint.unit() != i_unitDst) {
        if (Units.Length.isMetricUnit(i_physValPoint.unit()) && Units.Length.isMetricUnit(i_unitDst)) {
            CPhysVal physValX = i_physValPoint.x();
            CPhysVal physValY = i_physValPoint.y();
            physValX.convertValue(i_unitDst);
            physValY.convertValue(i_unitDst);
            physValPoint = CPhysValPoint(physValX, physValY);
        }
        else if ((i_physValPoint.unit() == Units.Length.px) && Units.Length.isMetricUnit(i_unitDst)) {
            QPointF pt = i_physValPoint.toQPointF();
            CPhysVal physValX(m_divLinesMetricsX.getVal(pt.x()), drawingSize.unit(), drawingSize.imageCoorsResolution());
            CPhysVal physValY(m_divLinesMetricsY.getVal(pt.y()), drawingSize.unit(), drawingSize.imageCoorsResolution());
            physValX.convertValue(i_unitDst);
            physValY.convertValue(i_unitDst);
            physValPoint = CPhysValPoint(physValX, physValY);
        }
        else if (Units.Length.isMetricUnit(i_physValPoint.unit()) && (i_unitDst == Units.Length.px)) {
            CPhysVal physValX = i_physValPoint.x();
            CPhysVal physValY = i_physValPoint.y();
            double fX_px = m_divLinesMetricsX.getValInPix(physValX.getVal(drawingSize.unit()));
            double fY_px = m_divLinesMetricsY.getValInPix(physValY.getVal(drawingSize.unit()));
            physValPoint = CPhysValPoint(fX_px, fY_px, drawingSize.imageCoorsResolutionInPx(), i_unitDst);
        }
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size into the current unit of the drawing scene.

    @param [in] i_physValPoint

    @return Converted value.
*/
CPhysValLine CGraphObjGroup::convert(const CPhysValLine& i_physValLine) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(i_physValLine, drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given line value into the desired unit.

    @param [in] i_physValLine
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValLine CGraphObjGroup::convert(const CPhysValLine& i_physValLine, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysValPoint physValP1 = convert(i_physValLine.p1(), i_unitDst);
    CPhysValPoint physValP2 = convert(i_physValLine.p2(), i_unitDst);
    return CPhysValLine(physValP1, physValP2);
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size into the current unit of the drawing scene.

    @param [in] i_physValPoint

    @return Converted value.
*/
CPhysValRect CGraphObjGroup::convert(const CPhysValRect& i_physValRect) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(i_physValRect, drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given rectangle into the desired unit.

    When converting from pixels into metric unit or from metric unit to pixels
    and if the Y Scale is from bottom to top the rectangles top line becomes the
    bottom line and vice versa as the height and width of a rectangle should
    always be greater than 0. If the Y-Scale orientation is from Bottom to Top
    the top line of the rectangle on the screen is below the bottom line.

    @param [in] i_physValRect
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValRect CGraphObjGroup::convert(const CPhysValRect& i_physValRect, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    CPhysValPoint physValTL = convert(i_physValRect.topLeft(), i_unitDst);
    CPhysValPoint physValBR = convert(i_physValRect.bottomRight(), i_unitDst);
    CPhysValRect physValRect(physValTL, physValBR);
    // When converting from pixels into metric or metric to pixels unit
    // and if the Y Scale is from bottom to top the rectangles top line
    // becomes the bottom line and vice versa (the height and width of a
    // rectangle should always be greater than 0).
    if (physValRect.height().getVal() < 0)
    {
        CPhysVal physValYT = physValBR.y();
        CPhysVal physValYB = physValTL.y();
        physValTL.setY(physValYT);
        physValBR.setY(physValYB);
        physValRect = CPhysValRect(physValTL, physValBR);
    }
    return physValRect;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the position of the item relative to its parent item.
*/
CPhysValPoint CGraphObjGroup::getPos() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return getPos(drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Returns the position of the item relative to its parent item in the
           given unit.
*/
CPhysValPoint CGraphObjGroup::getPos(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return getRect(i_unit).center();
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the current bounding rectangle of the object in local coordinates.

    @return Bounding rectangle in local coordinates.
*/
QRectF CGraphObjGroup::getBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ "" );

    QPointF ptTL = m_transform.map(m_rectOrig.topLeft());
    QPointF ptBR = m_transform.map(m_rectOrig.bottomRight());

    QRectF rctBounding(ptTL, ptBR);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

////------------------------------------------------------------------------------
///*! @brief Returns the bounding rectangle of the object in local coordinates.
//
//    This method is used by a group to resize its children.
//
//    @param [in] i_version
//        Transform (default) will return the current bounding rectangle.
//        For Origin the original line values before adding the object as a child
//        to a group is returned.
//
//    @return Bounding rectangle in local coordinates.
//*/
//QRectF CGraphObjGroup::getOriginalBoundingRectInParent() const
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "getOriginalBoundingRectInParent",
//        /* strAddInfo   */ "" );
//
//    QRectF rctBounding = m_physValRectOrig.toQRectF();
//    QPointF ptTL = rctBounding.topLeft();
//    QPointF ptBR = rctBounding.bottomRight();
//    QGraphicsItem* pGraphicsItemParent = parentItem();
//    CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(pGraphicsItemParent);
//    // If the line belongs to a group ...
//    if (pGraphObjParent != nullptr) {
//        // The original coordinates in m_physValRectOrig are relative to the
//        // top left corner of the parent's bounding rectangle.
//        // The coordinates must be mapped to local coordinates.
//        QRectF rctBoundingParent = pGraphObjParent->getOriginalBoundingRectInParent();
//        QPointF ptOriginParent = rctBoundingParent.topLeft();
//        ptTL -= ptOriginParent;
//        ptBR -= ptOriginParent;
//        rctBounding = QRectF(ptTL, ptBR);
//    }
//    // If the item does not belong to a group ...
//    else {
//        // .. the current (transformed) and original coordinates must be the same
//        // and relative to the top left corner of the scene.
//        // The coordinates must be mapped to local coordinates.
//        // The center point of the original rectangle is the original position
//        // of the group on the scene. mapFromScene cannot be used as the current
//        // and original position may be different.
//        QPointF ptOriginParent = rctBounding.center();
//        ptTL -= ptOriginParent;
//        ptBR -= ptOriginParent;
//        rctBounding = QRectF(ptTL, ptBR);
//    }
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
//    }
//    return rctBounding;
//}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
//bool CGraphObjGroup::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjIsHit, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Point:" + point2Str(i_pt) +
//            ", HitInfo, " + QString(o_pHitInfo == nullptr ? "null" : pointer2Str(o_pHitInfo));
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjIsHit,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "isHit",
//        /* strAddInfo   */ strMthInArgs );
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
//    {
//        strMthInArgs = "Rect: " + rect2Str(m_rctCurr);
//        mthTracer.trace(strMthInArgs);
//    }
//#endif
//
//    bool bIsHit = false;
//
//    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
//
//    if (pGraphicsItem != nullptr) {
//        if (pGraphicsItem->isSelected()) {
//            bIsHit = isBoundingRectSelectionPointHit(
//                /* pt               */ i_pt,
//                /* iSelPtsCount     */ -1,
//                /* pSelPts          */ nullptr,
//                /* pGraphObjHitInfo */ o_pHitInfo );
//        }
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//        if (!bIsHit) {
//            bIsHit = isRectHit(m_rctCurr, EFillStyle::SolidPattern, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo);
//        }
//#endif
//        if (!bIsHit) {
//            if (pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyle::SolidPattern) {
//                bIsHit = pGraphicsItem->contains(i_pt);
//                if (o_pHitInfo != nullptr) {
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
//        if (bIsHit && o_pHitInfo != nullptr) {
//            o_pHitInfo->setCursor(Math::deg2Rad(m_fRotAngleCurr_deg));
//        }
//#endif
//    }
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        if (o_pHitInfo != nullptr) {
//            QString strMthOutArgs = "HitInfo {" + o_pHitInfo->toString() + "}";
//            mthTracer.setMethodOutArgs(strMthOutArgs);
//        }
//        mthTracer.setMethodReturn(bIsHit);
//    }
//    return bIsHit;
//}

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::showSelectionPoints( unsigned char i_selPts )
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
        if (i_selPts & ESelectionPointsBoundingRectAll) {
            showSelectionPointsOfBoundingRect(getBoundingRect());
        }
    }
}

/*==============================================================================
protected: // overridable slots of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Called by the drawing scene if the drawing size is changed.

    When changing the drawing size in metric unit dimension
    (e.g. on changing the Y Scale Orientation) the scene coordinates must be
    newly calculated even if the original values stored in metric units have not
    been changed. On changing the drawing size the the drawing scene will emit
    the signal "drawingSizeChanged" and the method MUST set the flag
    "m_bForceConversionToSceneCoors" to true before converting the coordinates
    and setting the converted values.
*/
void CGraphObjGroup::onDrawingSizeChanged(const CDrawingSize& i_drawingSize)
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

    throw CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);

    // Object shape points in local coordinates.
    QRectF rectF = getBoundingRect();

    // Depending on the Y scale orientation of the drawing scene the item coordinates must
    // be either returned relative to the top left corner or relative to the bottom right
    // corner of the parent's bounding rectangle.
    CPhysValRect physValRect(rectF, i_drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
    QGraphicsItem* pGraphicsItemParent = parentItem();
    CGraphObjGroup* pGraphObjGroup = dynamic_cast<CGraphObjGroup*>(pGraphicsItemParent);
    // If the item belongs to a group ...
    if (pGraphObjGroup != nullptr) {
        physValRect = pGraphObjGroup->convert(physValRect, i_drawingSize.unit());
    }
    // If the item is not a child of a group ...
    else {
        physValRect = m_pDrawingScene->convert(physValRect, i_drawingSize.unit());
    }
    setRect(physValRect);

    m_bForceConversionToSceneCoors = false;
}

/*==============================================================================
public: // overridables of base class QGraphicsItemGroup
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjGroup::boundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = getBoundingRect();
    for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsBoundingRect){
        if (pGraphObjSelPt != nullptr) {
            QRectF rctSelPt = pGraphObjSelPt->boundingRect();
            QPolygonF plgSelPt = mapFromItem( pGraphObjSelPt, rctSelPt );
            rctBounding |= plgSelPt.boundingRect();
        }
    }
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
void CGraphObjGroup::paint(
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

    QPen pn(Qt::DotLine);
    pn.setColor(Qt::black);

    QRectF rctBounding = getBoundingRect();

    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHit || m_bIsHighlighted || isSelected())) {
        QPainterPath outline;
        outline.moveTo(rctBounding.topLeft());
        outline.lineTo(rctBounding.topRight());
        outline.lineTo(rctBounding.bottomRight());
        outline.lineTo(rctBounding.bottomLeft());
        outline.lineTo(rctBounding.topLeft());
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

    pn.setColor(Qt::black);
    i_pPainter->setPen(pn);
    i_pPainter->setBrush(Qt::NoBrush);
    i_pPainter->drawRect(rctBounding);

    if ((m_pDrawingScene->getMode() == EMode::Edit) && isSelected()) {
        pn.setColor(Qt::blue);
        pn.setStyle(Qt::DotLine);
        pn.setWidth(1);
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
            i_pPainter->drawLine(ptRctM, ptRotM);
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
*/
bool CGraphObjGroup::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
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
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bEventHandled);
    }
    return bEventHandled;

} // sceneEventFilter

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv )
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

    showSelectionPointsOfBoundingRect(getBoundingRect());
    setIsHit(true);
    setCursor(getProposedCursor(i_pEv->pos()));
}

//------------------------------------------------------------------------------
void CGraphObjGroup::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
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

    showSelectionPointsOfBoundingRect(getBoundingRect());
    setIsHit(true);
    setCursor(getProposedCursor(i_pEv->pos()));
}

//------------------------------------------------------------------------------
void CGraphObjGroup::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
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
void CGraphObjGroup::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
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

    //CEnumMode modeDrawing = m_pDrawingScene->getMode();
    //CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    //if (modeDrawing == EMode::Edit) {
    //    if (editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None) {
    //        QGraphicsItemGroup::mousePressEvent(i_pEv); // this will select the item (creating selection points)
    //        QPointF ptMouseItemPos = i_pEv->pos();
    //        SGraphObjHitInfo hitInfo;
    //        bool bIsHit = isHit(ptMouseItemPos, &hitInfo);
    //        m_editMode = hitInfo.m_editMode;
    //        m_editResizeMode = hitInfo.m_editResizeMode;
    //        m_selPtSelectedBoundingRect = hitInfo.m_selPtBoundingRect;
    //        for (int idxSelPt = 0; idxSelPt < m_arpSelPtsBoundingRect.size(); ++idxSelPt) {
    //            ESelectionPoint selPt = static_cast<ESelectionPoint>(idxSelPt);
    //            CGraphObjSelectionPoint* pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];
    //            if (pGraphObjSelPt != nullptr) {
    //                if (m_selPtSelectedBoundingRect == selPt) {
    //                    pGraphObjSelPt->setSelected(true);
    //                }
    //                else {
    //                    pGraphObjSelPt->setSelected(false);
    //                }
    //            }
    //        }

    //        #ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    //        m_rctOnMousePressEvent = m_rctCurr;
    //        m_ptRotOriginOnMousePressEvent = mapToScene(m_ptRotOriginCurr);
    //        #endif
    //        m_pDrawingScene->setMode( EMode::Undefined, EEditTool::Undefined, m_editMode, m_editResizeMode, false );
    //        //updateEditInfo();
    //        //updateToolTip();
    //    }
    //}
    //else if (modeDrawing == EMode::View) {
    //    for (SGraphObjMouseEventFct& fctEntry : m_arMousePressEventFunctions) {
    //        if (fctEntry.m_pFct != nullptr) {
    //            fctEntry.m_pFct(fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv);
    //        }
    //    }
    //}

    // Forward the mouse event to the base implementation.
    // This will select the item, creating selection points if not yet created.
    QGraphicsItemGroup::mousePressEvent(i_pEv);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
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

    //CEnumMode modeDrawing = m_pDrawingScene->getMode();

    //if (modeDrawing == EMode::Edit) {
    //    // Mouse events may first be dispatched to childrens of the group. Mapping the mouse
    //    // coordinates from the childrens to the parent's (the group's) coordinate system
    //    // does not work for succeeding mouse move events. So the mouse item pos returned
    //    // by "i_pEv->pos()" is not correct here and we need to map the mouse scene event
    //    // pos to the group's mouse item pos.

    //    if (m_editMode == EEditMode::Move) {
    //        QGraphicsItemGroup::mouseMoveEvent(i_pEv);
    //    }
    //    else if (m_editMode == EEditMode::Resize) {
    //        QPointF ptMouseItemPos = i_pEv->pos();

    //        #ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    //        m_rctCurr = resizeRect( m_rctOnMousePressEvent, m_selPtSelectedBoundingRect.enumerator(), ptMouseItemPos, nullptr );

    //        // Don't change the position of the object as the position of further
    //        // mouse events should be received relative to the object's position
    //        // on starting the edit process on pressing the mouse.
    //        //setPos(ptPosNew); // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

    //        applyGeometryChangeToChildrens();
    //        updateSelectionPointsOfBoundingRect(m_rctCurr);
    //        #endif
    //        //updateEditInfo();
    //        //updateToolTip();
    //        update();
    //    }
    //    else if (m_editMode == EEditMode::Rotate) {
    //        QPointF ptMouseScenePos = i_pEv->scenePos(); // see comment above
    //        double fRotAngle_rad = getAngleRad( m_ptRotOriginOnMousePressEvent, ptMouseScenePos );

    //        #ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    //        m_fRotAngleCurr_deg = Math::rad2Deg(fRotAngle_rad);

    //        switch( m_selPtSelectedBoundingRect.enumerator() )
    //        {
    //            case ESelectionPoint::RotateTop:
    //            {
    //                m_fRotAngleCurr_deg -= 90.0;
    //                break;
    //            }
    //            case ESelectionPoint::RotateBottom:
    //            {
    //                m_fRotAngleCurr_deg -= 270.0;
    //                break;
    //            }
    //            default:
    //            {
    //                break;
    //            }
    //        }

    //        m_fRotAngleCurr_deg = Math::round2Resolution( m_fRotAngleCurr_deg, m_pDrawingScene->getRotationAngleResolutionInDegree() );

    //        while( m_fRotAngleCurr_deg >= 360.0 )
    //        {
    //            m_fRotAngleCurr_deg -= 360.0;
    //        }
    //        while( m_fRotAngleCurr_deg < 0.0 )
    //        {
    //            m_fRotAngleCurr_deg += 360.0;
    //        }
    //        #endif

    //        updateTransform();
    //        //updateEditInfo();
    //        //updateToolTip();

    //        update();
    //    }
    //}
    //else if (modeDrawing == EMode::View) {
    //    for (SGraphObjMouseEventFct& fctEntry : m_arMouseMoveEventFunctions) {
    //        if (fctEntry.m_pFct != nullptr) {
    //            fctEntry.m_pFct(fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv);
    //        }
    //    }
    //}

    // Forward the mouse event to the LineItems base implementation.
    // This will move the item resulting in an itemChange call with PositionHasChanged.
    QGraphicsItemGroup::mouseMoveEvent(i_pEv);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
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

    //CEnumMode modeDrawing = m_pDrawingScene->getMode();

    //if (modeDrawing == EMode::Edit) {
    //    if( m_editMode == EEditMode::Resize ) {
    //        // The item will not be resized to the position of the mouse release event.
    //        // A selection point might have been clicked and released immediately (without
    //        // moving the mouse). In this case changing the size of the item according to
    //        // position of the mouse is not expected.

    //        #ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    //        QPolygonF plgSceneNew         = mapToScene(m_rctCurr);
    //        QRectF    rctBoundingSceneNew = plgSceneNew.boundingRect();
    //        QPointF   ptRotOriginSceneNew = rctBoundingSceneNew.center();
    //        double    fAngle_rad          = Math::deg2Rad(m_fRotAngleCurr_deg);
    //        //QPointF   ptLTScenePosOld   = mapToScene( QPointF(0.0,0.0) );
    //        QPointF   ptLTScenePosNew     = mapToScene( m_rctCurr.topLeft() );
    //        QPointF   ptItemScenePosNew   = rotatePoint( ptRotOriginSceneNew, ptLTScenePosNew, -fAngle_rad );

    //        if( m_rctCurr.left() != 0.0 )
    //        {
    //            m_rctCurr.moveLeft(0.0);
    //        }
    //        if( m_rctCurr.top() != 0.0 )
    //        {
    //            m_rctCurr.moveTop(0.0);
    //        }

    //        m_ptRotOriginCurr = m_rctCurr.center();

    //        setPos(ptItemScenePosNew);    // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

    //        updateTransform();

    //        applyGeometryChangeToChildrens();

    //        updateSelectionPointsOfBoundingRect(m_rctCurr);
    //        #endif

    //        // Not for group items. Otherwise the layout information would get lost.
    //        //acceptCurrentAsOriginalCoors();
    //        //updateEditInfo();
    //        //updateToolTip();
    //    }
    //    m_editMode = EEditMode::None;
    //    m_editResizeMode = EEditResizeMode::None;
    //    m_idxSelPtSelectedPolygon = -1;
    //    m_selPtSelectedBoundingRect = ESelectionPoint::None;
    //}

    //else if (modeDrawing == EMode::View) {
    //    for (SGraphObjMouseEventFct& fctEntry : m_arMouseReleaseEventFunctions) {
    //        if (fctEntry.m_pFct != nullptr) {
    //            fctEntry.m_pFct(fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv);
    //        }
    //    }
    //}

    //// The mouse release event would select the object.
    //// This is not wanted if the selection tool is not active.
    //bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
    //bool bIsSelectableReset = false;

    //if (bIsSelectable && m_pDrawingScene->getEditTool() != EEditTool::Select) {
    //    setFlag(QGraphicsItem::ItemIsSelectable, false);
    //    bIsSelectableReset = true;
    //}

    //QGraphicsItemGroup::mouseReleaseEvent(i_pEv);

    //if (bIsSelectableReset) {
    //    setFlag(QGraphicsItem::ItemIsSelectable, bIsSelectable);
    //}

    // Forward the mouse event to the LineItems base implementation.
    // This will move the item resulting in an itemChange call with PositionHasChanged.
    QGraphicsItemGroup::mouseReleaseEvent(i_pEv);

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
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
    // double click event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".

    //CEnumMode modeDrawing = m_pDrawingScene->getMode();

    //if (modeDrawing == EMode::Edit) {
    //    if (isSelected()) {
    //        onCreateAndExecDlgFormatGraphObjs();
    //    }
    //}
    //else if (modeDrawing == EMode::View) {
    //    for (SGraphObjMouseEventFct& fctEntry : m_arMouseDoubleClickEventFunctions) {
    //        if (fctEntry.m_pFct != nullptr) {
    //            fctEntry.m_pFct(fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv);
    //        }
    //    }
    //}
} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qKeyEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strMthInArgs );

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if (modeDrawing == EMode::Edit) {
        QGraphicsItemGroup::keyPressEvent(i_pEv);
    }
    //else if (modeDrawing == EMode::View) {
    //    for (SGraphObjKeyEventFct& fctEntry : m_arKeyPressEventFunctions) {
    //        if (fctEntry.m_pFct != nullptr) {
    //            fctEntry.m_pFct(fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv);
    //        }
    //    }
    //}
} // keyPressEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjKeyEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qKeyEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if (modeDrawing == EMode::Edit) {
        QGraphicsItemGroup::keyReleaseEvent(i_pEv);
    }
    //else if (modeDrawing == EMode::View) {
    //    for (SGraphObjKeyEventFct& fctEntry : m_arKeyReleaseEventFunctions) {
    //        if (fctEntry.m_pFct != nullptr) {
    //            fctEntry.m_pFct( fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv );
    //        }
    //    }
    //}
}

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjGroup::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
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
            //updateInternalScenePos();
            bGeometryChanged = true;
            bTreeEntryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged || i_change == ItemPositionHasChanged) {
        //traceGraphicsItemStates(mthTracer);
        //traceGraphObjStates(mthTracer);
        tracePositionInfo(mthTracer, EMethodDir::Enter);

        // Update the transformed line coordinates kept in the unit of the drawing scene.
        // The group may be moved or transformed by several methods.
        // "itemChange" is a central point to update the coordinates upon those changes.
        if (m_iItemChangeUpdateOriginalCoorsBlockedCounter == 0) {
            QRectF rectF = getBoundingRect();
            //QPointF pTL = mapToParent(rectF.topLeft());
            //QPointF pBR = mapToParent(rectF.bottomRight());
            //CPhysValPoint physValPTL = m_pDrawingScene->toPhysValPoint(pTL);
            //CPhysValPoint physValPBR = m_pDrawingScene->toPhysValPoint(pBR);
            // For groups the original coordinates are only updated when adding the group to
            // or removing the group from another group.
            if (i_change == ItemParentHasChanged) {
                if (parentItem() == nullptr) {
                    setRectOrig(rectF);
                }
            }
            else if (i_change == ItemParentHasChanged) {
                    setRectOrig(rectF);
            }
            applyGeometryChangeToChildrens();
        }
        tracePositionInfo(mthTracer, EMethodDir::Leave);
        bGeometryChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemSelectedHasChanged) {
        prepareGeometryChange();
        if (m_pDrawingScene->getMode() == EMode::Edit && isSelected()) {
            bringToFront();
            showSelectionPoints();
            // Not necessary as item has been brought to front and "showSelectionPoints"
            // sets zValue of selection points above item.
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
    else if (i_change == ItemChildAddedChange || i_change == ItemChildRemovedChange) {
        //prepareGeometryChange();

        //CGraphObjSelectionPoint* pGraphObjSelPt;
        //QPointF                  ptSel;
        //ESelectionPoint          selPt;
        //int                      idxSelPt;

        //for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
        //{
        //    selPt = static_cast<ESelectionPoint>(idxSelPt);

        //    pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

        //    if( pGraphObjSelPt != nullptr )
        //    {
        //        if( idxSelPt == ESelectionPoint::RotateTop && m_fScaleFacYCurr != 0.0 )
        //        {
        //            ptSel = getSelectionPointCoors(m_rctCurr,ESelectionPoint::TopCenter);
        //            ptSel.setY( ptSel.y() - getSelectionPointRotateDistance()/m_fScaleFacYCurr );
        //            ptSel = mapToScene(ptSel);
        //        }
        //        else if( idxSelPt == ESelectionPoint::RotateBottom && m_fScaleFacYCurr != 0.0 )
        //        {
        //            ptSel = getSelectionPointCoors(m_rctCurr,ESelectionPoint::BottomCenter);
        //            ptSel.setY( ptSel.y() + getSelectionPointRotateDistance()/m_fScaleFacYCurr );
        //            ptSel = mapToScene(ptSel);
        //        }
        //        else
        //        {
        //            ptSel = getSelectionPointCoors(m_rctCurr,selPt);
        //            ptSel = mapToScene(ptSel);
        //        }
        //        pGraphObjSelPt->setPos(ptSel);
        //    }
        //}

        //updateEditInfo();
        //updateToolTip();
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
//void CGraphObjGroup::onDrawingSizeChanged(const CDrawingSize& i_drawingSize)
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
//    if (m_physValRectCurr.unit() != i_drawingSize.unit()) {
//        m_bForceConversionToSceneCoors = true;
//        setRect(m_pDrawingScene->convert(m_physValRectCurr, i_drawingSize.unit()));
//        m_bForceConversionToSceneCoors = false;
//        emit_geometryValuesUnitChanged();
//    }
//}

//------------------------------------------------------------------------------
void CGraphObjGroup::onSelectionPointGeometryChanged(CGraphObj* i_pSelectionPoint)
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
    CPhysValPoint physValParentSelPt = m_pDrawingScene->convert(ptParentPosSelPt);
    SGraphObjSelectionPoint selPt = pGraphObjSelPt->getSelectionPoint();

    if (selPt.m_selPtType == ESelectionPointType::BoundingRectangle) {
        switch (selPt.m_selPt) {
            case ESelectionPoint::TopLeft: {
                setTopLeft(physValParentSelPt);
                break;
            }
            case ESelectionPoint::TopRight: {
                setTopRight(physValParentSelPt);
                break;
            }
            case ESelectionPoint::BottomRight: {
                setBottomRight(physValParentSelPt);
                break;
            }
            case ESelectionPoint::BottomLeft: {
                setBottomLeft(physValParentSelPt);
                break;
            }
            case ESelectionPoint::TopCenter: {
                setTop(physValParentSelPt.y());
                break;
            }
            case ESelectionPoint::RightCenter: {
                setRight(physValParentSelPt.x());
                break;
            }
            case ESelectionPoint::BottomCenter: {
                setBottom(physValParentSelPt.y());
                break;
            }
            case ESelectionPoint::LeftCenter: {
                setLeft(physValParentSelPt.x());
                break;
            }
            case ESelectionPoint::Center: {
                moveCenter(physValParentSelPt);
                break;
            }
            case ESelectionPoint::RotateTop: {
                break;
            }
            case ESelectionPoint::RotateBottom: {
                break;
            }
            default: {
                break;
            }
        }
    }
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the original rectangle coordinates.

    @param [in] i_rect
        Rectangle coordinates to be set.

    @return Previous original rectangle coordinates.
*/
QRectF CGraphObjGroup::setRectOrig(const QRectF& i_rect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qRect2Str(i_rect);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setRectOrig",
        /* strAddInfo   */ strMthInArgs );

    QRectF rectPrev = m_rectOrig;
    m_rectOrig = i_rect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qRect2Str(rectPrev));
    }
    return rectPrev;
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Applies the geometry change of the group to its children.

    A group may be resized or rotated. If resized also the children need to
    be resized correspondingly.

    The group may be resized by editing the geometry values or by moving the
    selection points.

    At any time the group's width or height may become zero.
    Once the width or height of the group becomes zero resizing the children
    would no longer be possible.

    For this the original size of the group must be stored and the current
    scale factor to resize the children is calculated using the current and
    the original size.

    The first "setRect" call whose passed rectangle width and height is not zero
    is taken as the original group size. As long as there is no valid original
    rectangle childrens cannot be resized and the method does nothing.
*/
void CGraphObjGroup::applyGeometryChangeToChildrens()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "applyGeometryChangeToChildrens",
        /* strAddInfo   */ "" );
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer);
    }

    // As long as there is no valid original rectangle childrens cannot be
    // resized and the method does nothing.
    if (!m_rectOrig.isValid()) {
        return;
    }

    QRectF rectCurr = getBoundingRect();
    double fScaleX = rectCurr.width() / m_rectOrig.width();
    double fScaleY = rectCurr.height() / m_rectOrig.height();

    QList<QGraphicsItem*> arpGraphicsItemsChilds = childItems();
    for (QGraphicsItem* pGraphicsItemChild : arpGraphicsItemsChilds) {
        CGraphObj* pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);
        if (pGraphObjChild != nullptr) {
            pGraphObjChild->setGroupScale(fScaleX, fScaleY);

            //QRectF rectChildCurr;
            //if (pGraphObjChild->getAlignmentCount() == 0) {
            //    //rectChildCurr = getScaledChildRect(pGraphObjChild);

            //    QGraphicsItem* pGraphicsItemChild = dynamic_cast<QGraphicsItem*>(i_pGraphObjChild);

            //    QRectF rectChildCurr;
            //    //QRectF rectChildPrev = i_pGraphObjChild->getOriginalBoundingRectInParent();

            //    //const CUnit& unitThis = m_physValRectCurr.unit();

            //    //// Get coordinates of this group relative to the parent group if this group.
            //    //QRectF rectThisOrig = m_physValRectOrig.toQRectF();
            //    //QRectF rectThisCurr = m_physValRectCurr.toQRectF();
            //    //double fScaleWidth = rectThisCurr.width() / rectThisOrig.width();
            //    //double fScaleHeight = rectThisCurr.height() / rectThisOrig.height();

            //    //// Map the coordinates of this group to the local coordinates of this group.
            //    ////rectThisOrig = mapFromParent(rectThisOrig).boundingRect();
            //    ////rectThisCurr = mapFromParent(rectThisCurr).boundingRect();

            //    //// Get position of child relative to this parent group.
            //    ////QPointF ptPosChildPrev = pGraphicsItemChild->pos();

            //    //// Default without any alignments assigned is:
            //    //// - position relative to center point of the group's bounding rectangle
            //    //// - size relative to size of the group's bounding rectangle
            //    //rectChildCurr.setLeft(fScaleWidth * rectChildPrev.left());
            //    //rectChildCurr.setTop(fScaleHeight * rectChildPrev.top());
            //    //rectChildCurr.setWidth(fScaleWidth * rectChildPrev.width());
            //    //rectChildCurr.setHeight(fScaleHeight * rectChildPrev.height());

            //    //// If the object has a fixed width or height they also have a
            //    //// minimum and maximum width or height.
            //    //// In addition the minimum and maximum width and height is equal.
            //    //// So it is sufficient to check the minimum and maximum sizes.
            //    //if (i_pGraphObjChild->hasMinimumWidth()) {
            //    //    if (rectChildCurr.width() < i_pGraphObjChild->getMinimumWidth(unitThis).getVal()) {
            //    //        rectChildCurr.setWidth(i_pGraphObjChild->getMinimumWidth(unitThis).getVal());
            //    //    }
            //    //}
            //    //if (i_pGraphObjChild->hasMaximumWidth()) {
            //    //    if (rectChildCurr.width() > i_pGraphObjChild->getMaximumWidth(unitThis).getVal()) {
            //    //        rectChildCurr.setWidth(i_pGraphObjChild->getMaximumWidth(unitThis).getVal());
            //    //    }
            //    //}
            //    //if (i_pGraphObjChild->hasMinimumHeight()) {
            //    //    if (rectChildCurr.height() < i_pGraphObjChild->getMinimumHeight(unitThis).getVal()) {
            //    //        rectChildCurr.setHeight(i_pGraphObjChild->getMinimumHeight(unitThis).getVal());
            //    //    }
            //    //}
            //    //if (i_pGraphObjChild->hasMaximumHeight()) {
            //    //    if (rectChildCurr.height() > i_pGraphObjChild->getMaximumHeight(unitThis).getVal()) {
            //    //        rectChildCurr.setHeight(i_pGraphObjChild->getMaximumHeight(unitThis).getVal());
            //    //    }
            //    //}
            //}
            //if (rectChildCurr.width() < 0.0) {
            //    double fXL = rectChildCurr.right();
            //    double fXR = rectChildCurr.left();
            //    rectChildCurr.setLeft(fXL);
            //    rectChildCurr.setRight(fXR);
            //}
            //if (rectChildCurr.height() < 0.0) {
            //    double fYT = rectChildCurr.bottom();
            //    double fYB = rectChildCurr.top();
            //    rectChildCurr.setTop(fYT);
            //    rectChildCurr.setBottom(fYB);
            //}
            //pGraphObjChild->setCurrentBoundingRectInParent(rectChildCurr);
        }
    }

    //prepareGeometryChange();

} // applyGeometryChangeToChildrens

////------------------------------------------------------------------------------
//QRectF CGraphObjGroup::getAlignedChildRect(CGraphObj* i_pGraphObjChild) const
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_pGraphObjChild->keyInTree();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "getAlignedChildRect",
//        /* strAddInfo   */ strMthInArgs );
//
//    QRect rectChildCurr;
//
//    //const CUnit& unitThis = m_physValRectCurr.unit();
//    //QRectF rectThisOrig = m_physValRectOrig.toQRectF();
//    //QRectF rectThisCurr = m_physValRectCurr.toQRectF();
//    //double fScaleWidth = rectThisCurr.width() / rectThisOrig.width();
//    //double fScaleHeight = rectThisCurr.height() / rectThisOrig.height();
//
//    //QGraphicsItem*        pGraphicsItemChild;
//    //CGraphObj*            pGraphObjChild;
//    //int                   idxGraphObjChild;
//    //SGraphObjAlignment    alignment;
//    //int                   idxAlignment;
//    //QPointF               ptPosChildOrig;
//    //double                fxPosChildOrig;
//    //double                fyPosChildOrig;
//    //double                fWidthChildOrig;
//    //double                fHeightChildOrig;
//    //QRectF                rectChildCurr;
//
//    //else { // if( pGraphObjChild->getAlignmentCount() > 0 )
//    //    rectChildCurr.setLeft(fxPosChildOrig);
//    //    rectChildCurr.setTop(fyPosChildOrig);
//
//    //    rectChildCurr.setWidth(fWidthChildOrig);
//    //    rectChildCurr.setHeight(fHeightChildOrig);
//
//    //    bool bAlignLeft    = false;
//    //    bool bAlignHCenter = false;
//    //    bool bAlignRight   = false;
//    //    bool bAlignWidth   = false;
//    //    bool bAlignTop     = false;
//    //    bool bAlignVCenter = false;
//    //    bool bAlignBottom  = false;
//    //    bool bAlignHeight  = false;
//
//    //    // If the width of the child has to be aligned the left or right border will be moved
//    //    // keeping the width (if aligning the width it just makes sense to either align the
//    //    // left or the right border but not both border lines).
//    //    // If both the left and the right border has to be aligned (but not the width) the left
//    //    // and right border will be set implicitly adjusting (changing) the width.
//    //    // Same applies for height, top and bottom.
//    //    for( idxAlignment = 0; idxAlignment < pGraphObjChild->getAlignmentCount(); idxAlignment++ )
//    //    {
//    //        alignment = pGraphObjChild->getAlignment(idxAlignment);
//
//    //        if( alignment.m_alignmentRefChild == EAlignmentRef::Left )
//    //        {
//    //            bAlignLeft = true;
//    //        }
//    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::HCenter )
//    //        {
//    //            bAlignHCenter = true;
//    //        }
//    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::Right )
//    //        {
//    //            bAlignRight = true;
//    //        }
//    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::Width )
//    //        {
//    //            bAlignWidth = true;
//    //        }
//    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::Top )
//    //        {
//    //            bAlignTop = true;
//    //        }
//    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::VCenter )
//    //        {
//    //            bAlignVCenter = true;
//    //        }
//    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::Bottom )
//    //        {
//    //            bAlignBottom = true;
//    //        }
//    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::Height )
//    //        {
//    //            bAlignHeight = true;
//    //        }
//    //    }
//
//    //    bool bMoveHor = true;
//    //    bool bMoveVer = true;
//
//    //    if( !bAlignWidth && !bAlignHCenter && bAlignLeft && bAlignRight )
//    //    {
//    //        bMoveHor = false;
//    //    }
//    //    if( !bAlignHeight && !bAlignVCenter && bAlignTop && bAlignBottom )
//    //    {
//    //        bMoveVer = false;
//    //    }
//
//    //    for( idxAlignment = 0; idxAlignment < pGraphObjChild->getAlignmentCount(); idxAlignment++ )
//    //    {
//    //        alignment = pGraphObjChild->getAlignment(idxAlignment);
//
//    //        switch( alignment.m_alignmentRefChild.enumerator() )
//    //        {
//    //            case EAlignmentRef::Left:
//    //            {
//    //                switch( alignment.m_alignmentRefParent.enumerator() )
//    //                {
//    //                    case EAlignmentRef::Left:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::HCenter:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Right:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Width:   // aligning left border of child to width of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Height:   // aligning left border of child to height of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Top:      // aligning left border of child to top border of parent will very likely never been used ..
//    //                    case EAlignmentRef::VCenter:  // aligning left border of child to vertical center of parent will very likely never been used ..
//    //                    case EAlignmentRef::Bottom:   // aligning left border of child to bottom border of parent will very likely never been used ..
//    //                    default:
//    //                    {
//    //                        break;
//    //                    }
//    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
//    //                break;
//    //            } // case m_alignmentRefChild == EAlignmentRef::Left
//
//    //            case EAlignmentRef::HCenter:
//    //            {
//    //                switch( alignment.m_alignmentRefParent.enumerator() )
//    //                {
//    //                    case EAlignmentRef::Left:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::HCenter:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Right:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Width: // aligning horizontal center of child to width of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Height: // aligning horizontal center of child to height of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.width()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Top:      // aligning horizontal center of child to top border of parent will very likely never been used ..
//    //                    case EAlignmentRef::VCenter:  // aligning horizontal center of child to vertical center of parent will very likely never been used ..
//    //                    case EAlignmentRef::Bottom:   // aligning horizontal center of child to bottom border of parent will very likely never been used ..
//    //                    default:
//    //                    {
//    //                        break;
//    //                    }
//    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
//    //                break;
//    //            } // case m_alignmentRefChild == EAlignmentRef::HCenter
//
//    //            case EAlignmentRef::Right:
//    //            {
//    //                switch( alignment.m_alignmentRefParent.enumerator() )
//    //                {
//    //                    case EAlignmentRef::Left:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::HCenter:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Right:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Width:   // aligning right border of child to width of parent will very likely never been used ..
//    //                    {
//    //                        if( !alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Height:   // aligning right border of child to height of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveHor )
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fWidthChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Top:      // aligning right border of child to top border of parent will very likely never been used ..
//    //                    case EAlignmentRef::VCenter:  // aligning right border of child to vertical center of parent will very likely never been used ..
//    //                    case EAlignmentRef::Bottom:   // aligning right border of child to bottom border of parent will very likely never been used ..
//    //                    default:
//    //                    {
//    //                        break;
//    //                    }
//    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
//    //                break;
//    //            } // case m_alignmentRefChild == EAlignmentRef::Right
//
//    //            case EAlignmentRef::Top:
//    //            {
//    //                switch( alignment.m_alignmentRefParent.enumerator() )
//    //                {
//    //                    case EAlignmentRef::Top:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::VCenter:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Bottom:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Width: // aligning top border of child to width of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Height: // aligning top border of child to height of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Left:     // aligning top border of child to left border of parent will very likely never been used ..
//    //                    case EAlignmentRef::HCenter:  // aligning top border of child to horizontal center of parent will very likely never been used ..
//    //                    case EAlignmentRef::Right:    // aligning top border of child to right border of parent will very likely never been used ..
//    //                    default:
//    //                    {
//    //                        break;
//    //                    }
//    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
//    //                break;
//    //            } // case m_alignmentRefChild == EAlignmentRef::Top
//
//    //            case EAlignmentRef::VCenter:
//    //            {
//    //                switch( alignment.m_alignmentRefParent.enumerator() )
//    //                {
//    //                    case EAlignmentRef::Top:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::VCenter:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Bottom:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Width: // aligning vertical center of child to width of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Height: // aligning vertical center of child to height of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Left:     // aligning vertical center of child to left border of parent will very likely never been used ..
//    //                    case EAlignmentRef::HCenter:  // aligning vertical center of child to horizontal center of parent will very likely never been used ..
//    //                    case EAlignmentRef::Right:    // aligning vertical center of child to right border of parent will very likely never been used ..
//    //                    default:
//    //                    {
//    //                        break;
//    //                    }
//    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
//    //                break;
//    //            } // case m_alignmentRefChild == EAlignmentRef::VCenter
//
//    //            case EAlignmentRef::Bottom:
//    //            {
//    //                switch( alignment.m_alignmentRefParent.enumerator() )
//    //                {
//    //                    case EAlignmentRef::Top:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::VCenter:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Bottom:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Width: // aligning bottom border of child to width of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Height: // aligning bottom border of child to height of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        else
//    //                        {
//    //                            if( bMoveVer )
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                            else
//    //                            {
//    //                                if( fHeightChildOrig >= 0.0 )
//    //                                {
//    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                                else
//    //                                {
//    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
//    //                                }
//    //                            }
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Left:     // aligning bottom border of child to left border of parent will very likely never been used ..
//    //                    case EAlignmentRef::HCenter:  // aligning bottom border of child to horizontal center of parent will very likely never been used ..
//    //                    case EAlignmentRef::Right:    // aligning bottom border of child to right border of parent will very likely never been used ..
//    //                    default:
//    //                    {
//    //                        break;
//    //                    }
//    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
//    //                break;
//    //            } // case m_alignmentRefChild == EAlignmentRef::Bottom
//
//    //            case EAlignmentRef::Width:
//    //            {
//    //                switch( alignment.m_alignmentRefParent.enumerator() )
//    //                {
//    //                    case EAlignmentRef::Width:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            rectChildCurr.setWidth( m_rctCurr.width() + alignment.m_fVal );
//    //                        }
//    //                        else
//    //                        {
//    //                            rectChildCurr.setWidth( alignment.m_fVal * m_rctCurr.width() );
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Height: // aligning width of child to height of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            rectChildCurr.setWidth( m_rctCurr.height() + alignment.m_fVal );
//    //                        }
//    //                        else
//    //                        {
//    //                            rectChildCurr.setWidth( alignment.m_fVal * m_rctCurr.height() );
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Left:     // aligning width of child to left border of parent will very likely never been used ..
//    //                    case EAlignmentRef::HCenter:  // aligning width of child to horizontal center of parent will very likely never been used ..
//    //                    case EAlignmentRef::Right:    // aligning width of child to right border of parent will very likely never been used ..
//    //                    case EAlignmentRef::Top:      // aligning width of child to top border of parent will very likely never been used ..
//    //                    case EAlignmentRef::VCenter:  // aligning width of child to vertical center of parent will very likely never been used ..
//    //                    case EAlignmentRef::Bottom:   // aligning width of child to bottom border of parent will very likely never been used ..
//    //                    default:
//    //                    {
//    //                        break;
//    //                    }
//    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
//    //                break;
//    //            } // case m_alignmentRefChild == EAlignmentRef::Width
//
//    //            case EAlignmentRef::Height:
//    //            {
//    //                switch( alignment.m_alignmentRefParent.enumerator() )
//    //                {
//    //                    case EAlignmentRef::Width: // aligning height of child to width of parent will very likely never been used ..
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            rectChildCurr.setHeight( m_rctCurr.width() + alignment.m_fVal );
//    //                        }
//    //                        else
//    //                        {
//    //                            rectChildCurr.setHeight( alignment.m_fVal * m_rctCurr.width() );
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Height:
//    //                    {
//    //                        if( alignment.m_bAlignAbsolute )
//    //                        {
//    //                            rectChildCurr.setHeight( m_rctCurr.height() + alignment.m_fVal );
//    //                        }
//    //                        else
//    //                        {
//    //                            rectChildCurr.setHeight( alignment.m_fVal * m_rctCurr.height() );
//    //                        }
//    //                        break;
//    //                    }
//    //                    case EAlignmentRef::Left:     // aligning height of child to left border of parent will very likely never been used ..
//    //                    case EAlignmentRef::HCenter:  // aligning height of child to horizontal center of parent will very likely never been used ..
//    //                    case EAlignmentRef::Right:    // aligning height of child to right border of parent will very likely never been used ..
//    //                    case EAlignmentRef::Top:      // aligning height of child to top border of parent will very likely never been used ..
//    //                    case EAlignmentRef::VCenter:  // aligning height of child to vertical center of parent will very likely never been used ..
//    //                    case EAlignmentRef::Bottom:   // aligning height of child to bottom border of parent will very likely never been used ..
//    //                    default:
//    //                    {
//    //                        break;
//    //                    }
//    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
//    //                break;
//    //            } // case m_alignmentRefChild == EAlignmentRef::Height
//
//    //            default:
//    //            {
//    //                break;
//    //            }
//
//    //        } // switch( alignment.m_alignmentRefChild.enumerator() )
//
//    //        // If the object has a fixed width or height they also have a
//    //        // the minimum and maximum width or height. In addition the
//    //        // minimum and maximum width and height is equal. So it is sufficient
//    //        // to check the minimum and maximum sizes.
//    //        if( pGraphObjChild->hasMinimumWidth() )
//    //        {
//    //            if( rectChildCurr.width() < pGraphObjChild->getMinimumWidth() )
//    //            {
//    //                rectChildCurr.setWidth( pGraphObjChild->getMinimumWidth() );
//    //            }
//    //        }
//    //        if( pGraphObjChild->hasMaximumWidth() )
//    //        {
//    //            if( rectChildCurr.width() > pGraphObjChild->getMaximumWidth() )
//    //            {
//    //                rectChildCurr.setWidth( pGraphObjChild->getMaximumWidth() );
//    //            }
//    //        }
//    //        if( pGraphObjChild->hasMinimumHeight() )
//    //        {
//    //            if( rectChildCurr.height() < pGraphObjChild->getMinimumHeight() )
//    //            {
//    //                rectChildCurr.setHeight( pGraphObjChild->getMinimumHeight() );
//    //            }
//    //        }
//    //        if( pGraphObjChild->hasMaximumHeight() )
//    //        {
//    //            if( rectChildCurr.height() > pGraphObjChild->getMaximumHeight() )
//    //            {
//    //                rectChildCurr.setHeight( pGraphObjChild->getMaximumHeight() );
//    //            }
//    //        }
//
//    //    } // for( idxAlignment = 0; idxAlignment < pGraphObjChild->getAlignmentCount(); idxAlignment++ )
//
//    //} // if( pGraphObjChild->getAlignmentCount() > 0 )
//
//    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
//        mthTracer.setMethodReturn(qRect2Str(rectChildCurr));
//    }
//    return rectChildCurr;
//
//} // getAlignedChildRect

//------------------------------------------------------------------------------
void CGraphObjGroup::emit_gridSettingsChanged( const CDrawGridSettings& i_settings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_settings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_gridSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    emit gridSettingsChanged(i_settings);
}

/*==============================================================================
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::traceThisPositionInfo(
    CMethodTracer& i_mthTracer, EMethodDir i_mthDir, ELogDetailLevel i_detailLevel) const
//------------------------------------------------------------------------------
{
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItem != nullptr) {
            QString strRuntimeInfo;
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "PhysValRect {" + getRect().toString() + "}";
            i_mthTracer.trace(strRuntimeInfo);
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "RectCurr {" + qRect2Str(getBoundingRect()) + "}, Orig {" + qRect2Str(m_rectOrig) + "}";
            i_mthTracer.trace(strRuntimeInfo);
            CGraphObj::traceThisPositionInfo(i_mthTracer, i_mthDir, i_detailLevel);
        }
    }
}
