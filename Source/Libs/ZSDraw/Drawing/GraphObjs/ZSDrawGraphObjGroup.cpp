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
    m_physValRect(*i_pDrawingScene)
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
    m_physValRect(*i_pDrawingScene)
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

    //m_physValRect;

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

////------------------------------------------------------------------------------
//void CGraphObjGroup::addGraphObj( CGraphObj* i_pGraphObj )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    QString strMthInArgs;
//
//    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal))
//    {
//        strMthInArgs  = "GraphObj:" + i_pGraphObj->name();
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "addGraphObj",
//        /* strAddInfo   */ strMthInArgs );
//
//    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
//    {
//        strMthInArgs  = "Selected:" + bool2Str(isSelected());
//        strMthInArgs += ", EditMode:" + m_editMode.toString();
//        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
//        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
//        mthTracer.trace(strMthInArgs);
//    }
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);
//
//    QGraphicsItemGroup::addToGroup(pGraphicsItem);
//
//    i_pGraphObj->acceptCurrentAsOriginalCoors();
//
//    add(i_pGraphObj);
//
//} // addGraphObj

////------------------------------------------------------------------------------
//void CGraphObjGroup::removeGraphObj( CGraphObj* i_pGraphObj )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    QString strMthInArgs;
//
//    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal))
//    {
//        strMthInArgs = i_pGraphObj->keyInTree();
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "removeGraphObj",
//        /* strAddInfo   */ strMthInArgs );
//
//    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
//    {
//        strMthInArgs  = "Selected:" + bool2Str(isSelected());
//        strMthInArgs += ", EditMode:" + m_editMode.toString();
//        strMthInArgs += ", ResizeMode:" + m_editResizeMode.toString();
//        strMthInArgs += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
//        mthTracer.trace(strMthInArgs);
//    }
//
//    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);
//
//    QGraphicsItemGroup::removeFromGroup(pGraphicsItem);
//
//    i_pGraphObj->acceptCurrentAsOriginalCoors();
//
//    remove(i_pGraphObj);
//
//} // removeGraphObj

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
    QPolygonF plgScene = mapToScene(getBoundingRect(true));
    strScenePolygonShapePoints = polygon2Str(plgScene);
    return strScenePolygonShapePoints;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the item's rectangle to the given rectangle.

    @param [in] i_physValRect
        Rectangle to be set.
        Coordinates must be passed relative to the top left corner of the
        parent item's bounding rectangle (real shape points only),
        or in scene coordinates if the item does not have parent.
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

    // If the coordinates MUST be updated (e.g. after the drawing size has been changed)
    // or if they have been changed ...
    if (m_bForceConversionToSceneCoors || m_physValRect != i_physValRect)
    {
        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsItemGroup::prepareGeometryChange();

        // The rectangle coordinates are passed relative to the parent item
        // (usually the scene coordinates or another group item).
        QRectF rectF = i_physValRect.toQRectF();
        if (i_physValRect.unit() != Units.Length.px) {
            rectF = m_pDrawingScene->convert(i_physValRect, Units.Length.px).toQRectF();
        }

        // Rectangle points in parent coordinates.
        QPointF ptTL = rectF.topLeft();
        QPointF ptBR = rectF.bottomRight();
        QPointF ptPos = rectF.center();

        // But the graphics item expects the coordinates based on the
        // local coordinate system of the graphics item.
        // Items live in their own local coordinate system.
        // Their coordinates are usually centered around its center point (0, 0).

        // Move the points into the item's local coordinate system.
        // If the line is a child of another item ...
        QGraphicsItem* pGraphicsItemParent = parentItem();
        if (pGraphicsItemParent != nullptr) {
            // .. the coordinates were passed relative to the top left
            // corner of the parent item's bounding rectangle.
            ptTL = mapFromParent(ptTL);
            ptBR = mapFromParent(ptBR);
            CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(pGraphicsItemParent);
            if (pGraphObjParent != nullptr) {
                QRectF rectBoundingParent = pGraphObjParent->getBoundingRect(true);
                QPointF ptOriginParent = rectBoundingParent.topLeft();
                ptTL += ptOriginParent;
                ptBR += ptOriginParent;
            }
        }
        // If the line is not a child of another item but has already been added to the scene ...
        else if (scene() != nullptr) {
            // .. the coordinates were passed relative to the top left corner of the scene.
            // The origin of the local coordinates is the center point of the line. We need
            // to move P1 and P2 so that the center line will get the local coordinates (0/0).
            QPointF ptCenter = mapFromScene(ptPos);
            ptTL = mapFromScene(ptTL) - ptCenter;
            ptBR = mapFromScene(ptBR) - ptCenter;
        }
        // If the line has not added to the scene yet ...
        else {
            // .. we must do the transformation to the local coordinate system
            // on our own. The origin is the center of the line.
            ptTL -= rectF.center();
            ptBR -= rectF.center();
        }

        // Local coordinates. The origin is the center point of the rectangle.
        rectF = QRectF(ptTL, ptBR);

        {   CRefCountGuard refCountGuard(&m_iItemChangeUpdateOriginalCoorsBlockedCounter);

            // Update original rectangle coordinates in unit of drawing scene.
            // Please note that the original, untransformed rectangle coordinates m_physValRect,
            // kept in the unit of the drawing scene, is also updated in the "itemChange" method.
            // The line may be moved or transformed by other methods. "itemChange" is a
            // central point to update the coordinates upon those changes.
            // When explicitly setting the line coordinates the itemChange method must not
            // overwrite the current line value.
            setPhysValRect(i_physValRect);

            // Move the object to the parent position.
            // This has to be done after resizing the line which updates the local coordinates
            // of the line with origin (0/0) at the lines center point.
            // "setPos" will trigger an itemChange call which will update the position of the
            // selection points and labels. To position the selection points and labels correctly
            // the local coordinate system must be up-to-date.
            // Also note that itemChange must not overwrite the current line value (revCountGuard).
            QGraphicsItem_setPos(ptPos);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's line in the current unit of the drawing in parent
           coordinates. The origin of the returned line coordinates is the top
           left corner of the parent's bounding rectangle (real shape points only).
*/
CPhysValRect CGraphObjGroup::getRect() const
//------------------------------------------------------------------------------
{
    return m_physValRect;
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's line in the given unit.

    @param [in] i_unit
        Unit in which the line coordinates should be returned.
*/
CPhysValRect CGraphObjGroup::getRect(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysValRect physValRect = m_physValRect;
    if (i_unit != physValRect.unit()) {
        physValRect = m_pDrawingScene->convert(physValRect, i_unit);
    }
    return physValRect;
}

////------------------------------------------------------------------------------
//void CGraphObjGroup::setWidth( const CPhysVal& i_physValWidth )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValWidth.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setWidth",
//        /* strAddInfo   */ strMthInArgs );
//
//    setSize(i_physValWidth, getHeight(i_physValWidth.unit()));
//}
//
////------------------------------------------------------------------------------
//CPhysVal CGraphObjGroup::getWidth( const CUnit& i_unit, ECoordinatesVersion /*i_version*/ ) const
////------------------------------------------------------------------------------
//{
//    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
//    CPhysValLine physValLine(
//        getBoundingRect(true).topLeft(), getBoundingRect(true).topRight(),
//        drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
//    return m_pDrawingScene->convert(physValLine, i_unit).length();
//}
//
////------------------------------------------------------------------------------
//void CGraphObjGroup::setHeight( const CPhysVal& i_physValHeight )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValHeight.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setHeight",
//        /* strAddInfo   */ strMthInArgs );
//
//    setSize(getWidth(i_physValHeight.unit()), i_physValHeight);
//}
//
////------------------------------------------------------------------------------
//CPhysVal CGraphObjGroup::getHeight( const CUnit& i_unit, ECoordinatesVersion /*i_version*/ ) const
////------------------------------------------------------------------------------
//{
//    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
//    CPhysValLine physValLine(
//        getBoundingRect(true).topLeft(), getBoundingRect(true).bottomLeft(),
//        drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
//    return m_pDrawingScene->convert(physValLine, i_unit).length();
//}
//
////------------------------------------------------------------------------------
//void CGraphObjGroup::setSize( const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_physValWidth.toString() + ", " + i_physValHeight.toString();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "setSize",
//        /* strAddInfo   */ strMthInArgs );
//
//    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
//
//    CPhysVal physValWidth = i_physValWidth;
//    CPhysVal physValHeight = i_physValHeight;
//
//    physValWidth.convertValue(drawingSize.unit());
//    physValHeight.convertValue(drawingSize.unit());
//
//    if (hasFixedWidth() && physValWidth != getFixedWidth(drawingSize.unit())) {
//        physValWidth = getFixedWidth(drawingSize.unit());
//    }
//    else {
//        if (hasMinimumWidth() && physValWidth > getMinimumWidth(drawingSize.unit())) {
//            physValWidth = getMinimumWidth(drawingSize.unit());
//        }
//        if (hasMaximumWidth() && physValWidth > getMaximumWidth(drawingSize.unit())) {
//            physValWidth = getMaximumWidth(drawingSize.unit());
//        }
//    }
//
//    if (hasFixedHeight() && physValHeight != getFixedHeight(drawingSize.unit())) {
//        physValHeight = getFixedHeight(drawingSize.unit());
//    }
//    else {
//        if (hasMinimumHeight() && physValHeight > getMinimumHeight(drawingSize.unit())) {
//            physValHeight = getMinimumHeight(drawingSize.unit());
//        }
//        if (hasMaximumHeight() && physValHeight > getMaximumHeight(drawingSize.unit())) {
//            physValHeight = getMaximumHeight(drawingSize.unit());
//        }
//    }
//
//#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
//    if( fWidth != m_rctCurr.width() || fHeight != m_rctCurr.height() )
//    {
//        m_rctCurr.setWidth(fWidth);
//        m_rctCurr.setHeight(fHeight);
//
//        m_ptRotOriginCurr = m_rctCurr.center();
//
//        applyGeometryChangeToChildrens();
//
//        if( isSelected() )
//        {
//            updateSelectionPointsOfBoundingRect(m_rctCurr);
//        }
//
//        updateLabelPositionsAndContents();
//
//        //itemChange( ItemPositionHasChanged, pos() ); // ItemSizeHasChanged does not exist
//
//        updateEditInfo();
//        updateToolTip();
//    }
//#endif
//
//} // setSize
//
////------------------------------------------------------------------------------
//void CGraphObjGroup::setSize( const CPhysValSize& i_physValSize )
////------------------------------------------------------------------------------
//{
//    setSize(i_physValSize.width(), i_physValSize.height());
//}
//
////------------------------------------------------------------------------------
//CPhysValSize CGraphObjGroup::getSize( const CUnit& i_unit, ECoordinatesVersion /*i_version*/ ) const
////------------------------------------------------------------------------------
//{
//    return CPhysValSize(getWidth(i_unit), getHeight(i_unit));
//}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the position of the item relative to its parent item.
*/
CPhysValPoint CGraphObjGroup::getPos( const CUnit& i_unit, ECoordinatesVersion /*i_version*/ ) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QPointF ptCenter = getBoundingRect(true).center();
    CPhysValPoint physValPoint(ptCenter, drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
    return m_pDrawingScene->convert(physValPoint, drawingSize.unit());
}

//------------------------------------------------------------------------------
QRectF CGraphObjGroup::getBoundingRect(bool i_bOnlyRealShapePoints) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjBoundingRect, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "OnlyRealShapePoints: " + bool2Str(i_bOnlyRealShapePoints);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ strMthInArgs );

    QRectF rctBounding = rect();
    if (!i_bOnlyRealShapePoints) {
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
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::applyGeometryChangeToChildrens()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "applyGeometryChangeToChildrens",
        /* strAddInfo   */ "" );

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strMthInArgs = "Rect:" + rect2Str(m_rctCurr);
        mthTracer.trace(strMthInArgs);
    }
#endif

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    double fScaleWidthCurr  = 1.0;
    double fScaleHeightCurr = 1.0;

    if( m_bHasValidOrigCoors )
    {
        if( m_sizOrig.width() != 0.0 )
        {
            fScaleWidthCurr = fabs( m_rctCurr.width() / m_sizOrig.width() );
        }
        if( m_sizOrig.height() != 0.0 )
        {
            fScaleHeightCurr = fabs( m_rctCurr.height() / m_sizOrig.height() );
        }
    }

    QList<QGraphicsItem*> arpGraphicsItemsChilds = childItems();
    QGraphicsItem*        pGraphicsItemChild;
    CGraphObj*            pGraphObjChild;
    int                   idxGraphObjChild;
    SGraphObjAlignment    alignment;
    int                   idxAlignment;
    QPointF               ptPosChildOrig;
    double                fxPosChildOrig;
    double                fyPosChildOrig;
    double                fWidthChildOrig;
    double                fHeightChildOrig;
    QRectF                rctChildCurr;

    for( idxGraphObjChild = 0; idxGraphObjChild < arpGraphicsItemsChilds.size(); idxGraphObjChild++ )
    {
        pGraphicsItemChild = arpGraphicsItemsChilds[idxGraphObjChild];
        pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);

        if( pGraphObjChild != nullptr )
        {
            ptPosChildOrig = pGraphObjChild->getPos(ECoordinatesVersion::Original);
            fxPosChildOrig = ptPosChildOrig.x();
            fyPosChildOrig = ptPosChildOrig.y();

            fWidthChildOrig  = pGraphObjChild->getWidth(ECoordinatesVersion::Original);
            fHeightChildOrig = pGraphObjChild->getHeight(ECoordinatesVersion::Original);

            if( pGraphObjChild->getAlignmentCount() == 0 )
            {
                // Default without any alignments assigned is:
                // - position relative to top left corner of groups bounding rectangle
                // - size relative to size of groups bounding rectangle
                rctChildCurr.setLeft( m_rctCurr.left() + fScaleWidthCurr * fxPosChildOrig );
                rctChildCurr.setTop( m_rctCurr.top() + fScaleHeightCurr * fyPosChildOrig );
                rctChildCurr.setWidth( fScaleWidthCurr * fWidthChildOrig );
                rctChildCurr.setHeight( fScaleHeightCurr * fHeightChildOrig );

                // If the object has a fixed width or height they also have a minimum and maximum width or height.
                // In addition the minimum and maximum width and height is equal.
                // So it is sufficient to check the minimum and maximum sizes.
                if( pGraphObjChild->hasMinimumWidth() )
                {
                    if( rctChildCurr.width() < pGraphObjChild->getMinimumWidth() )
                    {
                        rctChildCurr.setWidth( pGraphObjChild->getMinimumWidth() );
                    }
                }
                if( pGraphObjChild->hasMaximumWidth() )
                {
                    if( rctChildCurr.width() > pGraphObjChild->getMaximumWidth() )
                    {
                        rctChildCurr.setWidth( pGraphObjChild->getMaximumWidth() );
                    }
                }
                if( pGraphObjChild->hasMinimumHeight() )
                {
                    if( rctChildCurr.height() < pGraphObjChild->getMinimumHeight() )
                    {
                        rctChildCurr.setHeight( pGraphObjChild->getMinimumHeight() );
                    }
                }
                if( pGraphObjChild->hasMaximumHeight() )
                {
                    if( rctChildCurr.height() > pGraphObjChild->getMaximumHeight() )
                    {
                        rctChildCurr.setHeight( pGraphObjChild->getMaximumHeight() );
                    }
                }

            } // if( pGraphObjChild->getAlignmentCount() == 0 )

            else // if( pGraphObjChild->getAlignmentCount() > 0 )
            {
                rctChildCurr.setLeft(fxPosChildOrig);
                rctChildCurr.setTop(fyPosChildOrig);

                rctChildCurr.setWidth(fWidthChildOrig);
                rctChildCurr.setHeight(fHeightChildOrig);

                bool bAlignLeft    = false;
                bool bAlignHCenter = false;
                bool bAlignRight   = false;
                bool bAlignWidth   = false;
                bool bAlignTop     = false;
                bool bAlignVCenter = false;
                bool bAlignBottom  = false;
                bool bAlignHeight  = false;

                // If the width of the child has to be aligned the left or right border will be moved
                // keeping the width (if aligning the width it just makes sense to either align the
                // left or the right border but not both border lines).
                // If both the left and the right border has to be aligned (but not the width) the left
                // and right border will be set implicitly adjusting (changing) the width.
                // Same applies for height, top and bottom.
                for( idxAlignment = 0; idxAlignment < pGraphObjChild->getAlignmentCount(); idxAlignment++ )
                {
                    alignment = pGraphObjChild->getAlignment(idxAlignment);

                    if( alignment.m_alignmentRefChild == EAlignmentRef::Left )
                    {
                        bAlignLeft = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRef::HCenter )
                    {
                        bAlignHCenter = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRef::Right )
                    {
                        bAlignRight = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRef::Width )
                    {
                        bAlignWidth = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRef::Top )
                    {
                        bAlignTop = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRef::VCenter )
                    {
                        bAlignVCenter = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRef::Bottom )
                    {
                        bAlignBottom = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRef::Height )
                    {
                        bAlignHeight = true;
                    }
                }

                bool bMoveHor = true;
                bool bMoveVer = true;

                if( !bAlignWidth && !bAlignHCenter && bAlignLeft && bAlignRight )
                {
                    bMoveHor = false;
                }
                if( !bAlignHeight && !bAlignVCenter && bAlignTop && bAlignBottom )
                {
                    bMoveVer = false;
                }

                for( idxAlignment = 0; idxAlignment < pGraphObjChild->getAlignmentCount(); idxAlignment++ )
                {
                    alignment = pGraphObjChild->getAlignment(idxAlignment);

                    switch( alignment.m_alignmentRefChild.enumerator() )
                    {
                        case EAlignmentRef::Left:
                        {
                            switch( alignment.m_alignmentRefParent.enumerator() )
                            {
                                case EAlignmentRef::Left:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::HCenter:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Right:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Width:   // aligning left border of child to width of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Height:   // aligning left border of child to height of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Top:      // aligning left border of child to top border of parent will very likely never been used ..
                                case EAlignmentRef::VCenter:  // aligning left border of child to vertical center of parent will very likely never been used ..
                                case EAlignmentRef::Bottom:   // aligning left border of child to bottom border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent.enumerator() )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRef::Left

                        case EAlignmentRef::HCenter:
                        {
                            switch( alignment.m_alignmentRefParent.enumerator() )
                            {
                                case EAlignmentRef::Left:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::HCenter:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Right:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Width: // aligning horizontal center of child to width of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Height: // aligning horizontal center of child to height of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.width()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.width()/2.0 );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Top:      // aligning horizontal center of child to top border of parent will very likely never been used ..
                                case EAlignmentRef::VCenter:  // aligning horizontal center of child to vertical center of parent will very likely never been used ..
                                case EAlignmentRef::Bottom:   // aligning horizontal center of child to bottom border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent.enumerator() )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRef::HCenter

                        case EAlignmentRef::Right:
                        {
                            switch( alignment.m_alignmentRefParent.enumerator() )
                            {
                                case EAlignmentRef::Left:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::HCenter:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Right:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Width:   // aligning right border of child to width of parent will very likely never been used ..
                                {
                                    if( !alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Height:   // aligning right border of child to height of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveHor )
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fWidthChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Top:      // aligning right border of child to top border of parent will very likely never been used ..
                                case EAlignmentRef::VCenter:  // aligning right border of child to vertical center of parent will very likely never been used ..
                                case EAlignmentRef::Bottom:   // aligning right border of child to bottom border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent.enumerator() )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRef::Right

                        case EAlignmentRef::Top:
                        {
                            switch( alignment.m_alignmentRefParent.enumerator() )
                            {
                                case EAlignmentRef::Top:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::VCenter:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Bottom:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Width: // aligning top border of child to width of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Height: // aligning top border of child to height of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Left:     // aligning top border of child to left border of parent will very likely never been used ..
                                case EAlignmentRef::HCenter:  // aligning top border of child to horizontal center of parent will very likely never been used ..
                                case EAlignmentRef::Right:    // aligning top border of child to right border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent.enumerator() )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRef::Top

                        case EAlignmentRef::VCenter:
                        {
                            switch( alignment.m_alignmentRefParent.enumerator() )
                            {
                                case EAlignmentRef::Top:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::VCenter:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Bottom:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Width: // aligning vertical center of child to width of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Height: // aligning vertical center of child to height of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                            else
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rctChildCurr.height()/2.0 );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Left:     // aligning vertical center of child to left border of parent will very likely never been used ..
                                case EAlignmentRef::HCenter:  // aligning vertical center of child to horizontal center of parent will very likely never been used ..
                                case EAlignmentRef::Right:    // aligning vertical center of child to right border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent.enumerator() )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRef::VCenter

                        case EAlignmentRef::Bottom:
                        {
                            switch( alignment.m_alignmentRefParent.enumerator() )
                            {
                                case EAlignmentRef::Top:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::VCenter:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Bottom:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal );
                                            }
                                            else
                                            {
                                                rctChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Width: // aligning bottom border of child to width of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Height: // aligning bottom border of child to height of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if( bMoveVer )
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                        else
                                        {
                                            if( fHeightChildOrig >= 0.0 )
                                            {
                                                rctChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                            else
                                            {
                                                rctChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
                                            }
                                        }
                                    }
                                    break;
                                }
                                case EAlignmentRef::Left:     // aligning bottom border of child to left border of parent will very likely never been used ..
                                case EAlignmentRef::HCenter:  // aligning bottom border of child to horizontal center of parent will very likely never been used ..
                                case EAlignmentRef::Right:    // aligning bottom border of child to right border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent.enumerator() )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRef::Bottom

                        case EAlignmentRef::Width:
                        {
                            switch( alignment.m_alignmentRefParent.enumerator() )
                            {
                                case EAlignmentRef::Width:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        rctChildCurr.setWidth( m_rctCurr.width() + alignment.m_fVal );
                                    }
                                    else
                                    {
                                        rctChildCurr.setWidth( alignment.m_fVal * m_rctCurr.width() );
                                    }
                                    break;
                                }
                                case EAlignmentRef::Height: // aligning width of child to height of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        rctChildCurr.setWidth( m_rctCurr.height() + alignment.m_fVal );
                                    }
                                    else
                                    {
                                        rctChildCurr.setWidth( alignment.m_fVal * m_rctCurr.height() );
                                    }
                                    break;
                                }
                                case EAlignmentRef::Left:     // aligning width of child to left border of parent will very likely never been used ..
                                case EAlignmentRef::HCenter:  // aligning width of child to horizontal center of parent will very likely never been used ..
                                case EAlignmentRef::Right:    // aligning width of child to right border of parent will very likely never been used ..
                                case EAlignmentRef::Top:      // aligning width of child to top border of parent will very likely never been used ..
                                case EAlignmentRef::VCenter:  // aligning width of child to vertical center of parent will very likely never been used ..
                                case EAlignmentRef::Bottom:   // aligning width of child to bottom border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent.enumerator() )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRef::Width

                        case EAlignmentRef::Height:
                        {
                            switch( alignment.m_alignmentRefParent.enumerator() )
                            {
                                case EAlignmentRef::Width: // aligning height of child to width of parent will very likely never been used ..
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        rctChildCurr.setHeight( m_rctCurr.width() + alignment.m_fVal );
                                    }
                                    else
                                    {
                                        rctChildCurr.setHeight( alignment.m_fVal * m_rctCurr.width() );
                                    }
                                    break;
                                }
                                case EAlignmentRef::Height:
                                {
                                    if( alignment.m_bAlignAbsolute )
                                    {
                                        rctChildCurr.setHeight( m_rctCurr.height() + alignment.m_fVal );
                                    }
                                    else
                                    {
                                        rctChildCurr.setHeight( alignment.m_fVal * m_rctCurr.height() );
                                    }
                                    break;
                                }
                                case EAlignmentRef::Left:     // aligning height of child to left border of parent will very likely never been used ..
                                case EAlignmentRef::HCenter:  // aligning height of child to horizontal center of parent will very likely never been used ..
                                case EAlignmentRef::Right:    // aligning height of child to right border of parent will very likely never been used ..
                                case EAlignmentRef::Top:      // aligning height of child to top border of parent will very likely never been used ..
                                case EAlignmentRef::VCenter:  // aligning height of child to vertical center of parent will very likely never been used ..
                                case EAlignmentRef::Bottom:   // aligning height of child to bottom border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent.enumerator() )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRef::Height

                        default:
                        {
                            break;
                        }

                    } // switch( alignment.m_alignmentRefChild.enumerator() )

                    // If the object has a fixed width or height they also have a
                    // the minimum and maximum width or height. In addition the
                    // minimum and maximum width and height is equal. So it is sufficient
                    // to check the minimum and maximum sizes.
                    if( pGraphObjChild->hasMinimumWidth() )
                    {
                        if( rctChildCurr.width() < pGraphObjChild->getMinimumWidth() )
                        {
                            rctChildCurr.setWidth( pGraphObjChild->getMinimumWidth() );
                        }
                    }
                    if( pGraphObjChild->hasMaximumWidth() )
                    {
                        if( rctChildCurr.width() > pGraphObjChild->getMaximumWidth() )
                        {
                            rctChildCurr.setWidth( pGraphObjChild->getMaximumWidth() );
                        }
                    }
                    if( pGraphObjChild->hasMinimumHeight() )
                    {
                        if( rctChildCurr.height() < pGraphObjChild->getMinimumHeight() )
                        {
                            rctChildCurr.setHeight( pGraphObjChild->getMinimumHeight() );
                        }
                    }
                    if( pGraphObjChild->hasMaximumHeight() )
                    {
                        if( rctChildCurr.height() > pGraphObjChild->getMaximumHeight() )
                        {
                            rctChildCurr.setHeight( pGraphObjChild->getMaximumHeight() );
                        }
                    }

                } // for( idxAlignment = 0; idxAlignment < pGraphObjChild->getAlignmentCount(); idxAlignment++ )

            } // if( pGraphObjChild->getAlignmentCount() > 0 )

            //if( fWidthChildOrig < 0.0 )
            //{
            //    double fXLeft  = rctChildCurr.right();
            //    double fXRight = rctChildCurr.left();
            //    rctChildCurr.setLeft(fXLeft);
            //    rctChildCurr.setRight(fXRight);
            //}
            //if( fHeightChildOrig < 0.0 )
            //{
            //    double fYTop    = rctChildCurr.bottom();
            //    double fYBottom = rctChildCurr.top();
            //    rctChildCurr.setTop(fYTop);
            //    rctChildCurr.setBottom(fYBottom);
            //}

            pGraphicsItemChild->setPos( rctChildCurr.topLeft() );
            pGraphObjChild->setSize( rctChildCurr.size() );

        } // if( pGraphObjChild != nullptr )

    } // for( idxGraphObjChild = 0; idxGraphObjChild < arpGraphicsItemsChilds.size(); idxGraphObjChild++ )
#endif

    prepareGeometryChange();

} // applyGeometryChangeToChildrens

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
            showSelectionPointsOfBoundingRect(getBoundingRect(true));
        }
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Called by the parent graphic item to inform the child about geometry changes.

    Connection lines don't belong to groups. But their connection points do. So if the
    group is moved also the connection points are moved by Qt's graphics scene.
    But not the connection lines which are linked to the connection points.
    "onGraphObjParentGeometryChanged" is called to inform the connection points
    and so that child groups forward the call to their child groups and child
    connection points.
*/
void CGraphObjGroup::onGraphObjParentGeometryChanged( CGraphObj* i_pGraphObjParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjParent->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onGraphObjParentGeometryChanged",
        /* strAddInfo   */ strMthInArgs );

    QList<QGraphicsItem*> arpGraphicsItemChilds = childItems();
    for (int idxGraphObj = 0; idxGraphObj < arpGraphicsItemChilds.size(); idxGraphObj++) {
        QGraphicsItem* pGraphicsItem = arpGraphicsItemChilds[idxGraphObj];
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
        if (pGraphObj != nullptr) {
            pGraphObj->onGraphObjParentGeometryChanged(this);
        }
    }
    //updateEditInfo();
    //updateToolTip();
}

/*==============================================================================
public: // overridables of base class QGraphicsItemGroup
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjGroup::boundingRect() const
//------------------------------------------------------------------------------
{
    return getBoundingRect(false);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* /*i_pStyleOption*/,
    QWidget*                        /*i_pWdgt*/ )
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

    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHit || isSelected())) {
        i_pPainter->save();
        QPen pn(Qt::DotLine);
        if (m_bIsHit || isSelected()) {
            pn.setColor(Qt::blue);
        }
        else {
            pn.setColor(Qt::black);
        }
        i_pPainter->setPen(pn);
        i_pPainter->setBrush(Qt::NoBrush);
        QRectF rctBounding = getBoundingRect(true);
        i_pPainter->drawRect(rctBounding);
        if (isSelected()) {
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
    }
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
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObjSelPtWatched == nullptr" );
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

    showSelectionPointsOfBoundingRect(getBoundingRect(true));
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

    showSelectionPointsOfBoundingRect(getBoundingRect(true));
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
    else if (modeDrawing == EMode::View) {
        for (SGraphObjKeyEventFct& fctEntry : m_arKeyPressEventFunctions) {
            if (fctEntry.m_pFct != nullptr) {
                fctEntry.m_pFct(fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv);
            }
        }
    }
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
    else if (modeDrawing == EMode::View) {
        for (SGraphObjKeyEventFct& fctEntry : m_arKeyReleaseEventFunctions) {
            if (fctEntry.m_pFct != nullptr) {
                fctEntry.m_pFct( fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv );
            }
        }
    }
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
            bGeometryChanged = true;
            bTreeEntryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged) {
        traceGraphicsItemStates(mthTracer);
        traceGraphObjStates(mthTracer);
        tracePositionInfo(mthTracer);

        // Update the original, untransformed line coordinates kept in the unit of
        // the drawing scene. The line may be moved or transformed by several methods.
        // "itemChange" is a central point to update the coordinates upon those changes.
        QRectF rectF = rect();
        if (m_iItemChangeUpdateOriginalCoorsBlockedCounter == 0) {
            QPointF pTL = mapToParent(rectF.topLeft());
            QPointF pBR = mapToParent(rectF.bottomRight());
            CPhysValPoint physValPTL = m_pDrawingScene->toPhysValPoint(pTL);
            CPhysValPoint physValPBR = m_pDrawingScene->toPhysValPoint(pBR);
            setPhysValRect(CPhysValRect(physValPTL, physValPBR));
        }
        bGeometryChanged = true;
        bTreeEntryChanged = true;
    }
    else if( i_change == ItemPositionHasChanged) {
        traceGraphicsItemStates(mthTracer);
        traceGraphObjStates(mthTracer);
        tracePositionInfo(mthTracer);

        // Update the original, untransformed rectangle coordinates kept in the unit of
        // the drawing scene. The rectangle may be moved or transformed by several methods.
        // "itemChange" is a central point to update the coordinates upon those changes.
        QRectF rectF = rect();
        if (m_iItemChangeUpdateOriginalCoorsBlockedCounter == 0) {
            QPointF pTL = mapToParent(rectF.topLeft());
            QPointF pBR = mapToParent(rectF.bottomRight());
            CPhysValPoint physValPTL = m_pDrawingScene->toPhysValPoint(pTL);
            CPhysValPoint physValPBR = m_pDrawingScene->toPhysValPoint(pBR);
            setPhysValRect(CPhysValRect(physValPTL, physValPBR));
        }
        //// Connection lines don't belong to groups. But their connection points do. So if the
        //// group is moved also the connection points are moved by Qt's graphics scene.
        //// But not the connection lines which are linked to the connection points. We
        //// call "onGraphObjParentGeometryChanged" to inform the connection points and so that
        //// child groups forward the call to their child groups and child connection points.
        //QList<QGraphicsItem*> arpGraphicsItemChilds = childItems();
        //for (int idxGraphObj = 0; idxGraphObj < arpGraphicsItemChilds.size(); idxGraphObj++) {
        //    QGraphicsItem* pGraphicsItem = arpGraphicsItemChilds[idxGraphObj];
        //    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
        //    if (pGraphObj != nullptr) {
        //        pGraphObj->onGraphObjParentGeometryChanged(this);
        //    }
        //}
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
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the rectangle of the group in local pixel coordinates.

    This is the same as the bounding rectangle of the group in local coordinates.
    The physical rectangle coordinates of the group in the drawing scenes unit
    are relative to the parent item (or drawing scene if the group does not have
    another group as parent). The coordinates must be transformed to the local
    coordinate system of the group whose origin is at the center point of the group.
*/
QRectF CGraphObjGroup::rect() const
//------------------------------------------------------------------------------
{
    QRectF rectF = m_pDrawingScene->convert(m_physValRect, Units.Length.px).toQRectF();
    rectF.moveCenter(QPointF(0, 0));
    return rectF;
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the original physical rectangle coordinates.

    If the given rectangle unit is different from the drawing scenes current unit
    the rectangle coordinates will be converted into the drawing scenes unit.
*/
void CGraphObjGroup::setPhysValRect(const CPhysValRect& i_physValRect)
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
        /* strMethod    */ "setPhysValRect",
        /* strAddInfo   */ strMthInArgs );

    if (m_pDrawingScene->drawingSize().unit() == i_physValRect.unit()) {
        m_physValRect = i_physValRect;
    }
    else {
        CPhysValRect physValRect = m_pDrawingScene->convert(i_physValRect);
        m_physValRect = i_physValRect;
    }
}

