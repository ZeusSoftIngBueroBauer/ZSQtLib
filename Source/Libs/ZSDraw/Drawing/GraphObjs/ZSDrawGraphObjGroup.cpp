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
    m_physValRectOrig(*i_pDrawingScene),
    m_physValRectCurr(*i_pDrawingScene)
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
    m_physValRectOrig(*i_pDrawingScene),
    m_physValRectCurr(*i_pDrawingScene)
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

    //m_physValRectOrig;
    //m_physValRectCurr;

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
    QPolygonF plgScene = mapToScene(getCurrentBoundingRect());
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
    if (m_bForceConversionToSceneCoors || m_physValRectCurr != i_physValRect)
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
                QRectF rectBoundingParent = pGraphObjParent->getCurrentBoundingRect();
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
            // Please note that the untransformed rectangle coordinates m_physValRectCurr,
            // kept in the unit of the drawing scene, is also updated in the "itemChange" method.
            // The line may be moved or transformed by other methods. "itemChange" is a
            // central point to update the coordinates upon those changes.
            // When explicitly setting the line coordinates the itemChange method must not
            // overwrite the current line value.
            // For groups the original coordinates are only updated when adding or removing the
            // item from another group. So here only the current (transformed) coordinates are set.
            setPhysValRect(i_physValRect, ECoordinatesVersion::Transformed);

            // Move the object to the parent position.
            // This has to be done after resizing the line which updates the local coordinates
            // of the line with origin (0/0) at the lines center point.
            // "setPos" will trigger an itemChange call which will update the position of the
            // selection points and labels. To position the selection points and labels correctly
            // the local coordinate system must be up-to-date.
            // Also note that itemChange must not overwrite the current line value (revCountGuard).
            QGraphicsItem_setPos(ptPos);
        }

        applyGeometryChangeToChildrens();
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
    return m_physValRectCurr;
}

//------------------------------------------------------------------------------
/*! @brief Returns the item's line in the given unit.

    @param [in] i_unit
        Unit in which the line coordinates should be returned.
*/
CPhysValRect CGraphObjGroup::getRect(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    CPhysValRect physValRect = m_physValRectCurr;
    if (i_unit != physValRect.unit()) {
        physValRect = m_pDrawingScene->convert(physValRect, i_unit);
    }
    return physValRect;
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
    setWidth(CPhysVal(i_fWidth, m_physValRectCurr.unit(), m_physValRectCurr.resolution()));
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
    CPhysValRect physValRect = m_physValRectCurr;
    if (i_physValWidth.unit() != m_physValRectCurr.unit()) {
        CPhysVal physValWidth(i_physValWidth);
        physValWidth.convertValue(m_physValRectCurr.unit());
        physValRect.setWidth(physValWidth);
    }
    else {
        physValRect.setWidth(i_physValWidth);
    }
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
    setHeight(CPhysVal(i_fHeight, m_physValRectCurr.unit(), m_physValRectCurr.resolution()));
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
    CPhysValRect physValRect = m_physValRectCurr;
    if (i_physValHeight.unit() != m_physValRectCurr.unit()) {
        CPhysVal physValHeight(i_physValHeight);
        physValHeight.convertValue(m_physValRectCurr.unit());
        physValRect.setHeight(physValHeight);
    }
    else {
        physValRect.setHeight(i_physValHeight);
    }
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
    physValRect.setSize(CPhysValSize(i_size, physValRect.resolution(), physValRect.unit()));
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
    CPhysValRect physValRect = m_physValRectCurr;
    if (i_physValSize.unit() != m_physValRectCurr.unit()) {
        CPhysValSize physValSize = m_pDrawingScene->convert(i_physValSize, m_physValRectCurr.unit());
        physValRect.setSize(physValSize);
    }
    else {
        physValRect.setSize(i_physValSize);
    }
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
    physValRect.moveCenter(CPhysValPoint(i_pt, physValRect.resolution(), physValRect.unit()));
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
    if (i_physValPoint.unit() != m_physValRectCurr.unit()) {
        CPhysValPoint physValPoint = m_pDrawingScene->convert(i_physValPoint, m_physValRectCurr.unit());
        physValRect.moveCenter(physValPoint);
    }
    else {
        physValRect.moveCenter(i_physValPoint);
    }
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
    setTop(CPhysVal(i_fTop, m_physValRectCurr.unit(), m_physValRectCurr.resolution()));
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
    CPhysValRect physValRect = m_physValRectCurr;
    if (i_physValTop.unit() != m_physValRectCurr.unit()) {
        CPhysVal physValTop(i_physValTop);
        physValTop.convertValue(m_physValRectCurr.unit());
        physValRect.setTop(physValTop);
    }
    else {
        physValRect.setTop(i_physValTop);
    }
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
    setBottom(CPhysVal(i_fBottom, m_physValRectCurr.unit(), m_physValRectCurr.resolution()));
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
    CPhysValRect physValRect = m_physValRectCurr;
    if (i_physValBottom.unit() != m_physValRectCurr.unit()) {
        CPhysVal physValBottom(i_physValBottom);
        physValBottom.convertValue(m_physValRectCurr.unit());
        physValRect.setBottom(physValBottom);
    }
    else {
        physValRect.setBottom(i_physValBottom);
    }
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
    setLeft(CPhysVal(i_fLeft, m_physValRectCurr.unit(), m_physValRectCurr.resolution()));
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
    CPhysValRect physValRect = m_physValRectCurr;
    if (i_physValLeft.unit() != m_physValRectCurr.unit()) {
        CPhysVal physValLeft(i_physValLeft);
        physValLeft.convertValue(m_physValRectCurr.unit());
        physValRect.setLeft(physValLeft);
    }
    else {
        physValRect.setLeft(i_physValLeft);
    }
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
    setRight(CPhysVal(i_fRight, m_physValRectCurr.unit(), m_physValRectCurr.resolution()));
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
    CPhysValRect physValRect = m_physValRectCurr;
    if (i_physValRight.unit() != m_physValRectCurr.unit()) {
        CPhysVal physValRight(i_physValRight);
        physValRight.convertValue(m_physValRectCurr.unit());
        physValRect.setRight(physValRight);
    }
    else {
        physValRect.setRight(i_physValRight);
    }
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
    physValRect.setTopLeft(CPhysValPoint(i_pt, physValRect.resolution(), physValRect.unit()));
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
    if (i_physValPoint.unit() != m_physValRectCurr.unit()) {
        CPhysValPoint physValPoint = m_pDrawingScene->convert(i_physValPoint, m_physValRectCurr.unit());
        physValRect.setTopLeft(physValPoint);
    }
    else {
        physValRect.setTopLeft(i_physValPoint);
    }
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
    physValRect.setTopRight(CPhysValPoint(i_pt, physValRect.resolution(), physValRect.unit()));
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
    if (i_physValPoint.unit() != m_physValRectCurr.unit()) {
        CPhysValPoint physValPoint = m_pDrawingScene->convert(i_physValPoint, m_physValRectCurr.unit());
        physValRect.setTopRight(physValPoint);
    }
    else {
        physValRect.setTopRight(i_physValPoint);
    }
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
    physValRect.setBottomRight(CPhysValPoint(i_pt, physValRect.resolution(), physValRect.unit()));
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
    if (i_physValPoint.unit() != m_physValRectCurr.unit()) {
        CPhysValPoint physValPoint = m_pDrawingScene->convert(i_physValPoint, m_physValRectCurr.unit());
        physValRect.setBottomRight(physValPoint);
    }
    else {
        physValRect.setBottomRight(i_physValPoint);
    }
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
    physValRect.setBottomLeft(CPhysValPoint(i_pt, physValRect.resolution(), physValRect.unit()));
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
    if (i_physValPoint.unit() != m_physValRectCurr.unit()) {
        CPhysValPoint physValPoint = m_pDrawingScene->convert(i_physValPoint, m_physValRectCurr.unit());
        physValRect.setBottomLeft(physValPoint);
    }
    else {
        physValRect.setBottomLeft(i_physValPoint);
    }
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
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QPointF ptCenter = getCurrentBoundingRect().center();
    CPhysValPoint physValPoint(ptCenter, drawingSize.imageCoorsResolutionInPx(), Units.Length.px);
    return m_pDrawingScene->convert(physValPoint, drawingSize.unit());
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the current bounding rectangle of the object in local coordinates.

    @return Bounding rectangle in local coordinates.
*/
QRectF CGraphObjGroup::getCurrentBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "getCurrentBoundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = rect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

//------------------------------------------------------------------------------
/*! @brief Returns the bounding rectangle of the object in local coordinates.

    This method is used by a group to resize its children.

    @param [in] i_version
        Transform (default) will return the current bounding rectangle.
        For Origin the original line values before adding the object as a child
        to a group is returned.

    @return Bounding rectangle in local coordinates.
*/
QRectF CGraphObjGroup::getOriginalBoundingRectInParent() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "getOriginalBoundingRectInParent",
        /* strAddInfo   */ "" );

    QRectF rctBounding = m_physValRectOrig.toQRectF();
    QPointF ptTL = rctBounding.topLeft();
    QPointF ptBR = rctBounding.bottomRight();
    QGraphicsItem* pGraphicsItemParent = parentItem();
    CGraphObj* pGraphObjParent = dynamic_cast<CGraphObj*>(pGraphicsItemParent);
    // If the line belongs to a group ...
    if (pGraphObjParent != nullptr) {
        // The original coordinates in m_physValRectOrig are relative to the
        // top left corner of the parent's bounding rectangle.
        // The coordinates must be mapped to local coordinates.
        QRectF rctBoundingParent = pGraphObjParent->getOriginalBoundingRectInParent();
        QPointF ptOriginParent = rctBoundingParent.topLeft();
        ptTL -= ptOriginParent;
        ptBR -= ptOriginParent;
        rctBounding = QRectF(ptTL, ptBR);
    }
    // If the item does not belong to a group ...
    else {
        // .. the current (transformed) and original coordinates must be the same
        // and relative to the top left corner of the scene.
        // The coordinates must be mapped to local coordinates.
        // The center point of the original rectangle is the original position
        // of the group on the scene. mapFromScene cannot be used as the current
        // and original position may be different.
        QPointF ptOriginParent = rctBounding.center();
        ptTL -= ptOriginParent;
        ptBR -= ptOriginParent;
        rctBounding = QRectF(ptTL, ptBR);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

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
            showSelectionPointsOfBoundingRect(getCurrentBoundingRect());
        }
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

////------------------------------------------------------------------------------
///*! @brief Called by the parent graphic item to inform the child about geometry changes.
//
//    Connection lines don't belong to groups. But their connection points do. So if the
//    group is moved also the connection points are moved by Qt's graphics scene.
//    But not the connection lines which are linked to the connection points.
//    "onGraphObjParentGeometryChanged" is called to inform the connection points
//    and so that child groups forward the call to their child groups and child
//    connection points.
//*/
//void CGraphObjGroup::onGraphObjParentGeometryChanged( CGraphObj* i_pGraphObjParent )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = i_pGraphObjParent->keyInTree();
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "onGraphObjParentGeometryChanged",
//        /* strAddInfo   */ strMthInArgs );
//
//    //QList<QGraphicsItem*> arpGraphicsItemChilds = childItems();
//    //for (int idxGraphObj = 0; idxGraphObj < arpGraphicsItemChilds.size(); idxGraphObj++) {
//    //    QGraphicsItem* pGraphicsItem = arpGraphicsItemChilds[idxGraphObj];
//    //    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
//    //    if (pGraphObj != nullptr) {
//    //        pGraphObj->onGraphObjParentGeometryChanged(this);
//    //    }
//    //}
//    //updateEditInfo();
//    //updateToolTip();
//}

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

    QRectF rctBounding = rect();
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

    //if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHit || m_bIsHighlighted || isSelected())) {
        i_pPainter->save();
        QPen pn(Qt::DotLine);
        if (m_bIsHit || m_bIsHighlighted || isSelected()) {
            pn.setColor(Qt::blue);
        }
        else {
            pn.setColor(Qt::black);
        }
        i_pPainter->setPen(pn);
        i_pPainter->setBrush(Qt::NoBrush);
        QRectF rctBounding = getCurrentBoundingRect();
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
    //}
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

    showSelectionPointsOfBoundingRect(getCurrentBoundingRect());
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

    showSelectionPointsOfBoundingRect(getCurrentBoundingRect());
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
        QRectF rectF = rect();
        if (m_iItemChangeUpdateOriginalCoorsBlockedCounter == 0) {
            QPointF pTL = mapToParent(rectF.topLeft());
            QPointF pBR = mapToParent(rectF.bottomRight());
            CPhysValPoint physValPTL = m_pDrawingScene->toPhysValPoint(pTL);
            CPhysValPoint physValPBR = m_pDrawingScene->toPhysValPoint(pBR);
            // For groups the original coordinates are only updated when adding the group to
            // or removing the group from another group.
            if( i_change == ItemParentHasChanged) {
                setPhysValRect(CPhysValRect(physValPTL, physValPBR), CEnumCoordinatesVersion());
            }
            else {
                setPhysValRect(CPhysValRect(physValPTL, physValPBR), ECoordinatesVersion::Transformed);
            }
        }
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

//------------------------------------------------------------------------------
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

    if (m_physValRectCurr.unit() != i_drawingSize.unit()) {
        m_bForceConversionToSceneCoors = true;
        setRect(m_pDrawingScene->convert(m_physValRectCurr, i_drawingSize.unit()));
        m_bForceConversionToSceneCoors = false;
        emit_geometryValuesUnitChanged();
    }
}

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
    CPhysValPoint physValParentSelPt = m_pDrawingScene->toPhysValPoint(ptParentPosSelPt);
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
    QRectF rectF = m_pDrawingScene->convert(m_physValRectCurr, Units.Length.px).toQRectF();
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

    @param [in] i_physValRect
        Rectangle coordinates to be set.
    @param [in] i_eVersion
        Transformed (default) will set the current line coordinates.
        If set to Origin the original coordinate values are set.
        If an invalid enum value is passed both the current (transformed)
        and the original coordinates are set.
        Please refer to documentation about the difference between Transformed and
        Original coordinates at base class CGraphObj.
*/
void CGraphObjGroup::setPhysValRect(
    const CPhysValRect& i_physValRect, const CEnumCoordinatesVersion& i_eVersion)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValRect.toString() + ", " + QString(i_eVersion.isValid() ? i_eVersion.toString() : "All");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setPhysValRect",
        /* strAddInfo   */ strMthInArgs );

    if (!i_eVersion.isValid() || i_eVersion == ECoordinatesVersion::Transformed) {
        if (m_pDrawingScene->drawingSize().unit() == i_physValRect.unit()) {
            m_physValRectCurr = i_physValRect;
        }
        else {
            CPhysValRect physValRect = m_pDrawingScene->convert(i_physValRect);
            m_physValRectCurr = i_physValRect;
        }
    }
    if (!i_eVersion.isValid() || i_eVersion == ECoordinatesVersion::Original) {
        if (m_pDrawingScene->drawingSize().unit() == i_physValRect.unit()) {
            m_physValRectOrig = i_physValRect;
        }
        else {
            CPhysValRect physValRect = m_pDrawingScene->convert(i_physValRect);
            m_physValRectOrig = i_physValRect;
        }
    }

    // For the first "setRect" call whose passed rectangle width and height is not zero
    // the current values are taken over as the original coordinates. As long as there
    // is no valid original rectangle childrens cannot be resized by the group.
    // See also comment at applyGeometryChangeToChildrens.
    if (!m_physValRectOrig.isValid()) {
        m_physValRectOrig = m_physValRectCurr;
    }
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
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "RectOrig {" + m_physValRectOrig.toString() + "}" +
            ", RectCurr { " + m_physValRectCurr.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "applyGeometryChangeToChildrens",
        /* strAddInfo   */ strMthInArgs );

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        tracePositionInfo(mthTracer);
    }

    // As long as there is no valid original rectangle childrens cannot be
    // resized and the method does nothing.
    if (!m_physValRectOrig.isValid()) {
        return;
    }

    QList<QGraphicsItem*> arpGraphicsItemsChilds = childItems();
    for (QGraphicsItem* pGraphicsItemChild : arpGraphicsItemsChilds) {
        CGraphObj* pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);
        if (pGraphObjChild != nullptr) {
            QRectF rectChildCurr;
            if (pGraphObjChild->getAlignmentCount() == 0) {
                rectChildCurr = getScaledChildRect(pGraphObjChild);
            }
            if (rectChildCurr.width() < 0.0) {
                double fXL = rectChildCurr.right();
                double fXR = rectChildCurr.left();
                rectChildCurr.setLeft(fXL);
                rectChildCurr.setRight(fXR);
            }
            if (rectChildCurr.height() < 0.0) {
                double fYT = rectChildCurr.bottom();
                double fYB = rectChildCurr.top();
                rectChildCurr.setTop(fYT);
                rectChildCurr.setBottom(fYB);
            }
            pGraphObjChild->setCurrentBoundingRectInParent(rectChildCurr);
        }
    }

    //prepareGeometryChange();

} // applyGeometryChangeToChildrens

//------------------------------------------------------------------------------
QRectF CGraphObjGroup::getScaledChildRect(CGraphObj* i_pGraphObjChild) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjChild->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "getScaledChildRect",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItemChild = dynamic_cast<QGraphicsItem*>(i_pGraphObjChild);

    QRectF rectChildPrev = i_pGraphObjChild->getOriginalBoundingRectInParent();
    QRectF rectChildCurr;

    const CUnit& unitThis = m_physValRectCurr.unit();

    // Get coordinates of this group relative to the parent group if this group.
    QRectF rectThisOrig = m_physValRectOrig.toQRectF();
    QRectF rectThisCurr = m_physValRectCurr.toQRectF();
    double fScaleWidth = rectThisCurr.width() / rectThisOrig.width();
    double fScaleHeight = rectThisCurr.height() / rectThisOrig.height();

    // Map the coordinates of this group to the local coordinates of this group.
    //rectThisOrig = mapFromParent(rectThisOrig).boundingRect();
    //rectThisCurr = mapFromParent(rectThisCurr).boundingRect();

    // Get position of child relative to this parent group.
    //QPointF ptPosChildPrev = pGraphicsItemChild->pos();

    // Default without any alignments assigned is:
    // - position relative to center point of the group's bounding rectangle
    // - size relative to size of the group's bounding rectangle
    rectChildCurr.setLeft(fScaleWidth * rectChildPrev.left());
    rectChildCurr.setTop(fScaleHeight * rectChildPrev.top());
    rectChildCurr.setWidth(fScaleWidth * rectChildPrev.width());
    rectChildCurr.setHeight(fScaleHeight * rectChildPrev.height());

    // If the object has a fixed width or height they also have a
    // minimum and maximum width or height.
    // In addition the minimum and maximum width and height is equal.
    // So it is sufficient to check the minimum and maximum sizes.
    if (i_pGraphObjChild->hasMinimumWidth()) {
        if (rectChildCurr.width() < i_pGraphObjChild->getMinimumWidth(unitThis).getVal()) {
            rectChildCurr.setWidth(i_pGraphObjChild->getMinimumWidth(unitThis).getVal());
        }
    }
    if (i_pGraphObjChild->hasMaximumWidth()) {
        if (rectChildCurr.width() > i_pGraphObjChild->getMaximumWidth(unitThis).getVal()) {
            rectChildCurr.setWidth(i_pGraphObjChild->getMaximumWidth(unitThis).getVal());
        }
    }
    if (i_pGraphObjChild->hasMinimumHeight()) {
        if (rectChildCurr.height() < i_pGraphObjChild->getMinimumHeight(unitThis).getVal()) {
            rectChildCurr.setHeight(i_pGraphObjChild->getMinimumHeight(unitThis).getVal());
        }
    }
    if (i_pGraphObjChild->hasMaximumHeight()) {
        if (rectChildCurr.height() > i_pGraphObjChild->getMaximumHeight(unitThis).getVal()) {
            rectChildCurr.setHeight(i_pGraphObjChild->getMaximumHeight(unitThis).getVal());
        }
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qRect2Str(rectChildCurr));
    }
    return rectChildCurr;
}

//------------------------------------------------------------------------------
QRectF CGraphObjGroup::getAlignedChildRect(CGraphObj* i_pGraphObjChild) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_pGraphObjChild->keyInTree();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "getAlignedChildRect",
        /* strAddInfo   */ strMthInArgs );

    QRect rectChildCurr;

    //const CUnit& unitThis = m_physValRectCurr.unit();
    //QRectF rectThisOrig = m_physValRectOrig.toQRectF();
    //QRectF rectThisCurr = m_physValRectCurr.toQRectF();
    //double fScaleWidth = rectThisCurr.width() / rectThisOrig.width();
    //double fScaleHeight = rectThisCurr.height() / rectThisOrig.height();

    //QGraphicsItem*        pGraphicsItemChild;
    //CGraphObj*            pGraphObjChild;
    //int                   idxGraphObjChild;
    //SGraphObjAlignment    alignment;
    //int                   idxAlignment;
    //QPointF               ptPosChildOrig;
    //double                fxPosChildOrig;
    //double                fyPosChildOrig;
    //double                fWidthChildOrig;
    //double                fHeightChildOrig;
    //QRectF                rectChildCurr;

    //else { // if( pGraphObjChild->getAlignmentCount() > 0 )
    //    rectChildCurr.setLeft(fxPosChildOrig);
    //    rectChildCurr.setTop(fyPosChildOrig);

    //    rectChildCurr.setWidth(fWidthChildOrig);
    //    rectChildCurr.setHeight(fHeightChildOrig);

    //    bool bAlignLeft    = false;
    //    bool bAlignHCenter = false;
    //    bool bAlignRight   = false;
    //    bool bAlignWidth   = false;
    //    bool bAlignTop     = false;
    //    bool bAlignVCenter = false;
    //    bool bAlignBottom  = false;
    //    bool bAlignHeight  = false;

    //    // If the width of the child has to be aligned the left or right border will be moved
    //    // keeping the width (if aligning the width it just makes sense to either align the
    //    // left or the right border but not both border lines).
    //    // If both the left and the right border has to be aligned (but not the width) the left
    //    // and right border will be set implicitly adjusting (changing) the width.
    //    // Same applies for height, top and bottom.
    //    for( idxAlignment = 0; idxAlignment < pGraphObjChild->getAlignmentCount(); idxAlignment++ )
    //    {
    //        alignment = pGraphObjChild->getAlignment(idxAlignment);

    //        if( alignment.m_alignmentRefChild == EAlignmentRef::Left )
    //        {
    //            bAlignLeft = true;
    //        }
    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::HCenter )
    //        {
    //            bAlignHCenter = true;
    //        }
    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::Right )
    //        {
    //            bAlignRight = true;
    //        }
    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::Width )
    //        {
    //            bAlignWidth = true;
    //        }
    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::Top )
    //        {
    //            bAlignTop = true;
    //        }
    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::VCenter )
    //        {
    //            bAlignVCenter = true;
    //        }
    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::Bottom )
    //        {
    //            bAlignBottom = true;
    //        }
    //        else if( alignment.m_alignmentRefChild == EAlignmentRef::Height )
    //        {
    //            bAlignHeight = true;
    //        }
    //    }

    //    bool bMoveHor = true;
    //    bool bMoveVer = true;

    //    if( !bAlignWidth && !bAlignHCenter && bAlignLeft && bAlignRight )
    //    {
    //        bMoveHor = false;
    //    }
    //    if( !bAlignHeight && !bAlignVCenter && bAlignTop && bAlignBottom )
    //    {
    //        bMoveVer = false;
    //    }

    //    for( idxAlignment = 0; idxAlignment < pGraphObjChild->getAlignmentCount(); idxAlignment++ )
    //    {
    //        alignment = pGraphObjChild->getAlignment(idxAlignment);

    //        switch( alignment.m_alignmentRefChild.enumerator() )
    //        {
    //            case EAlignmentRef::Left:
    //            {
    //                switch( alignment.m_alignmentRefParent.enumerator() )
    //                {
    //                    case EAlignmentRef::Left:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::HCenter:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Right:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Width:   // aligning left border of child to width of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Height:   // aligning left border of child to height of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Top:      // aligning left border of child to top border of parent will very likely never been used ..
    //                    case EAlignmentRef::VCenter:  // aligning left border of child to vertical center of parent will very likely never been used ..
    //                    case EAlignmentRef::Bottom:   // aligning left border of child to bottom border of parent will very likely never been used ..
    //                    default:
    //                    {
    //                        break;
    //                    }
    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
    //                break;
    //            } // case m_alignmentRefChild == EAlignmentRef::Left

    //            case EAlignmentRef::HCenter:
    //            {
    //                switch( alignment.m_alignmentRefParent.enumerator() )
    //                {
    //                    case EAlignmentRef::Left:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::HCenter:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Right:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Width: // aligning horizontal center of child to width of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Height: // aligning horizontal center of child to height of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.width()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.width()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Top:      // aligning horizontal center of child to top border of parent will very likely never been used ..
    //                    case EAlignmentRef::VCenter:  // aligning horizontal center of child to vertical center of parent will very likely never been used ..
    //                    case EAlignmentRef::Bottom:   // aligning horizontal center of child to bottom border of parent will very likely never been used ..
    //                    default:
    //                    {
    //                        break;
    //                    }
    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
    //                break;
    //            } // case m_alignmentRefChild == EAlignmentRef::HCenter

    //            case EAlignmentRef::Right:
    //            {
    //                switch( alignment.m_alignmentRefParent.enumerator() )
    //                {
    //                    case EAlignmentRef::Left:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::HCenter:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.center().x() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Right:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.right() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Width:   // aligning right border of child to width of parent will very likely never been used ..
    //                    {
    //                        if( !alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Height:   // aligning right border of child to height of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveHor )
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fWidthChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setRight( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setLeft( m_rctCurr.left() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Top:      // aligning right border of child to top border of parent will very likely never been used ..
    //                    case EAlignmentRef::VCenter:  // aligning right border of child to vertical center of parent will very likely never been used ..
    //                    case EAlignmentRef::Bottom:   // aligning right border of child to bottom border of parent will very likely never been used ..
    //                    default:
    //                    {
    //                        break;
    //                    }
    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
    //                break;
    //            } // case m_alignmentRefChild == EAlignmentRef::Right

    //            case EAlignmentRef::Top:
    //            {
    //                switch( alignment.m_alignmentRefParent.enumerator() )
    //                {
    //                    case EAlignmentRef::Top:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::VCenter:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Bottom:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Width: // aligning top border of child to width of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Height: // aligning top border of child to height of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Left:     // aligning top border of child to left border of parent will very likely never been used ..
    //                    case EAlignmentRef::HCenter:  // aligning top border of child to horizontal center of parent will very likely never been used ..
    //                    case EAlignmentRef::Right:    // aligning top border of child to right border of parent will very likely never been used ..
    //                    default:
    //                    {
    //                        break;
    //                    }
    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
    //                break;
    //            } // case m_alignmentRefChild == EAlignmentRef::Top

    //            case EAlignmentRef::VCenter:
    //            {
    //                switch( alignment.m_alignmentRefParent.enumerator() )
    //                {
    //                    case EAlignmentRef::Top:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::VCenter:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Bottom:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Width: // aligning vertical center of child to width of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.width() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Height: // aligning vertical center of child to height of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() - rectChildCurr.height()/2.0 );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Left:     // aligning vertical center of child to left border of parent will very likely never been used ..
    //                    case EAlignmentRef::HCenter:  // aligning vertical center of child to horizontal center of parent will very likely never been used ..
    //                    case EAlignmentRef::Right:    // aligning vertical center of child to right border of parent will very likely never been used ..
    //                    default:
    //                    {
    //                        break;
    //                    }
    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
    //                break;
    //            } // case m_alignmentRefChild == EAlignmentRef::VCenter

    //            case EAlignmentRef::Bottom:
    //            {
    //                switch( alignment.m_alignmentRefParent.enumerator() )
    //                {
    //                    case EAlignmentRef::Top:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::VCenter:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.center().y() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Bottom:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.bottom() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Width: // aligning bottom border of child to width of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top()+ alignment.m_fVal  + m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.width() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Height: // aligning bottom border of child to height of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal + m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        else
    //                        {
    //                            if( bMoveVer )
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.moveBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.moveTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                            else
    //                            {
    //                                if( fHeightChildOrig >= 0.0 )
    //                                {
    //                                    rectChildCurr.setBottom( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                                else
    //                                {
    //                                    rectChildCurr.setTop( m_rctCurr.top() + alignment.m_fVal * m_rctCurr.height() );
    //                                }
    //                            }
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Left:     // aligning bottom border of child to left border of parent will very likely never been used ..
    //                    case EAlignmentRef::HCenter:  // aligning bottom border of child to horizontal center of parent will very likely never been used ..
    //                    case EAlignmentRef::Right:    // aligning bottom border of child to right border of parent will very likely never been used ..
    //                    default:
    //                    {
    //                        break;
    //                    }
    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
    //                break;
    //            } // case m_alignmentRefChild == EAlignmentRef::Bottom

    //            case EAlignmentRef::Width:
    //            {
    //                switch( alignment.m_alignmentRefParent.enumerator() )
    //                {
    //                    case EAlignmentRef::Width:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            rectChildCurr.setWidth( m_rctCurr.width() + alignment.m_fVal );
    //                        }
    //                        else
    //                        {
    //                            rectChildCurr.setWidth( alignment.m_fVal * m_rctCurr.width() );
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Height: // aligning width of child to height of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            rectChildCurr.setWidth( m_rctCurr.height() + alignment.m_fVal );
    //                        }
    //                        else
    //                        {
    //                            rectChildCurr.setWidth( alignment.m_fVal * m_rctCurr.height() );
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Left:     // aligning width of child to left border of parent will very likely never been used ..
    //                    case EAlignmentRef::HCenter:  // aligning width of child to horizontal center of parent will very likely never been used ..
    //                    case EAlignmentRef::Right:    // aligning width of child to right border of parent will very likely never been used ..
    //                    case EAlignmentRef::Top:      // aligning width of child to top border of parent will very likely never been used ..
    //                    case EAlignmentRef::VCenter:  // aligning width of child to vertical center of parent will very likely never been used ..
    //                    case EAlignmentRef::Bottom:   // aligning width of child to bottom border of parent will very likely never been used ..
    //                    default:
    //                    {
    //                        break;
    //                    }
    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
    //                break;
    //            } // case m_alignmentRefChild == EAlignmentRef::Width

    //            case EAlignmentRef::Height:
    //            {
    //                switch( alignment.m_alignmentRefParent.enumerator() )
    //                {
    //                    case EAlignmentRef::Width: // aligning height of child to width of parent will very likely never been used ..
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            rectChildCurr.setHeight( m_rctCurr.width() + alignment.m_fVal );
    //                        }
    //                        else
    //                        {
    //                            rectChildCurr.setHeight( alignment.m_fVal * m_rctCurr.width() );
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Height:
    //                    {
    //                        if( alignment.m_bAlignAbsolute )
    //                        {
    //                            rectChildCurr.setHeight( m_rctCurr.height() + alignment.m_fVal );
    //                        }
    //                        else
    //                        {
    //                            rectChildCurr.setHeight( alignment.m_fVal * m_rctCurr.height() );
    //                        }
    //                        break;
    //                    }
    //                    case EAlignmentRef::Left:     // aligning height of child to left border of parent will very likely never been used ..
    //                    case EAlignmentRef::HCenter:  // aligning height of child to horizontal center of parent will very likely never been used ..
    //                    case EAlignmentRef::Right:    // aligning height of child to right border of parent will very likely never been used ..
    //                    case EAlignmentRef::Top:      // aligning height of child to top border of parent will very likely never been used ..
    //                    case EAlignmentRef::VCenter:  // aligning height of child to vertical center of parent will very likely never been used ..
    //                    case EAlignmentRef::Bottom:   // aligning height of child to bottom border of parent will very likely never been used ..
    //                    default:
    //                    {
    //                        break;
    //                    }
    //                } // switch( alignment.m_alignmentRefParent.enumerator() )
    //                break;
    //            } // case m_alignmentRefChild == EAlignmentRef::Height

    //            default:
    //            {
    //                break;
    //            }

    //        } // switch( alignment.m_alignmentRefChild.enumerator() )

    //        // If the object has a fixed width or height they also have a
    //        // the minimum and maximum width or height. In addition the
    //        // minimum and maximum width and height is equal. So it is sufficient
    //        // to check the minimum and maximum sizes.
    //        if( pGraphObjChild->hasMinimumWidth() )
    //        {
    //            if( rectChildCurr.width() < pGraphObjChild->getMinimumWidth() )
    //            {
    //                rectChildCurr.setWidth( pGraphObjChild->getMinimumWidth() );
    //            }
    //        }
    //        if( pGraphObjChild->hasMaximumWidth() )
    //        {
    //            if( rectChildCurr.width() > pGraphObjChild->getMaximumWidth() )
    //            {
    //                rectChildCurr.setWidth( pGraphObjChild->getMaximumWidth() );
    //            }
    //        }
    //        if( pGraphObjChild->hasMinimumHeight() )
    //        {
    //            if( rectChildCurr.height() < pGraphObjChild->getMinimumHeight() )
    //            {
    //                rectChildCurr.setHeight( pGraphObjChild->getMinimumHeight() );
    //            }
    //        }
    //        if( pGraphObjChild->hasMaximumHeight() )
    //        {
    //            if( rectChildCurr.height() > pGraphObjChild->getMaximumHeight() )
    //            {
    //                rectChildCurr.setHeight( pGraphObjChild->getMaximumHeight() );
    //            }
    //        }

    //    } // for( idxAlignment = 0; idxAlignment < pGraphObjChild->getAlignmentCount(); idxAlignment++ )

    //} // if( pGraphObjChild->getAlignmentCount() > 0 )

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(qRect2Str(rectChildCurr));
    }
    return rectChildCurr;

} // getAlignedChildRect
