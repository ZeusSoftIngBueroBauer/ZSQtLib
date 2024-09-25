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
public: // class members
==============================================================================*/

qint64 CGraphObjGroup::s_iInstCount = 0;

/*==============================================================================
protected: // class members
==============================================================================*/

QPainter::RenderHints CGraphObjGroup::s_painterRenderHints = QPainter::Antialiasing;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QPainter::RenderHints CGraphObjGroup::painterRenderHints()
//------------------------------------------------------------------------------
{
    return s_painterRenderHints;
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setPainterRenderHints(QPainter::RenderHints i_renderHints)
//------------------------------------------------------------------------------
{
    s_painterRenderHints = i_renderHints;
}

//------------------------------------------------------------------------------
void CGraphObjGroup::resetPainterRenderHints()
//------------------------------------------------------------------------------
{
    s_painterRenderHints = QPainter::Antialiasing;
}

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
    m_divLinesMetricsX(EScaleAxis::X),
    m_divLinesMetricsY(EScaleAxis::Y),
    m_gridSettings(),
    m_rectOrig(),
    m_rectScaled(),
    m_physValRectOrig(*m_pDrawingScene),
    m_physValRectScaled(*m_pDrawingScene),
    m_physValRectScaledAndRotated(*m_pDrawingScene)
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
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameTopLeft);
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameTopRight);
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameBottomLeft);
    m_strlstPredefinedLabelNames.append(c_strGeometryLabelNameBottomRight);

    for (const QString& strLabelName : m_strlstPredefinedLabelNames) {
        if (!m_hshpLabels.contains(strLabelName)) {
            if (strLabelName == c_strGeometryLabelNameTopLeft) {
                addLabel(strLabelName, strLabelName, ESelectionPoint::TopLeft);
            }
            else if (strLabelName == c_strGeometryLabelNameTopRight) {
                addLabel(strLabelName, strLabelName, ESelectionPoint::TopRight);
            }
            else if (strLabelName == c_strGeometryLabelNameBottomRight) {
                addLabel(strLabelName, strLabelName, ESelectionPoint::BottomRight);
            }
            else if (strLabelName == c_strGeometryLabelNameBottomLeft) {
                addLabel(strLabelName, strLabelName, ESelectionPoint::BottomLeft);
            }
            else {
                addLabel(strLabelName, strLabelName, ESelectionPoint::Center);
            }
        }
    }

    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameTopLeft);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameTopRight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameBottomRight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameBottomLeft);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameCenter);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameWidth);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameHeight);
    m_strlstGeometryLabelNames.append(c_strGeometryLabelNameAngle);

    const CUnit& unit = m_pDrawingScene->drawingSize().unit();
    for (const QString& strLabelName : m_strlstGeometryLabelNames) {
        if (strLabelName == c_strGeometryLabelNameTopLeft) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::TopLeft);
        }
        else if (strLabelName == c_strGeometryLabelNameTopRight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::TopRight);
        }
        else if (strLabelName == c_strGeometryLabelNameBottomRight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::BottomRight);
        }
        else if (strLabelName == c_strGeometryLabelNameBottomLeft) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryPosition, ESelectionPoint::BottomLeft);
        }
        else if (strLabelName == c_strGeometryLabelNameCenter) {
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
    m_divLinesMetricsX(EScaleAxis::X),
    m_divLinesMetricsY(EScaleAxis::Y),
    m_gridSettings(),
    m_rectOrig(),
    m_rectScaled(),
    m_physValRectOrig(*m_pDrawingScene),
    m_physValRectScaled(*m_pDrawingScene),
    m_physValRectScaledAndRotated(*m_pDrawingScene)
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
        /* strObjName   */ path(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    emit_aboutToBeDestroyed();

    //m_divLinesMetricsX;
    //m_divLinesMetricsY;
    //m_gridSettings;
    //m_rectOrig;
    //m_rectScaled;
    //m_physValRectOrig;
    //m_physValRectScaled;
    //m_physValRectScaledAndRotated;

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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        m_divLinesMetricsX.update();

        m_divLinesMetricsY.setDivLinesDistMinInPix(EDivLineLayer::Main, m_gridSettings.linesDistMin());
        m_divLinesMetricsY.setFont(font);
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
/*! @brief Adds the given item with all it's child items to this item group.

    The item and child items will be reparented to this group, but its position
    and transformation relative to the scene will stay intact.
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
        /* strObjName   */ path(),
        /* strMethod    */ "addToGroup",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    QGraphicsItem* pGraphicsItemChild = dynamic_cast<QGraphicsItem*>(i_pGraphObj);
    if (i_pGraphObj == nullptr) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pGraphObj == nullptr");
    }
    else if (pGraphicsItemChild == nullptr) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "pGraphicsItemChild == nullptr");
    }
    else if (i_pGraphObj == this) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "Cannot add myself as a child");
    }
    else if (i_pGraphObj->isParentOf(this)) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "Cannot add a parent as a child");
    }
    else {
        // Remember current position of this group in parent coordinates.
        QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
        QPointF ptPosThisPrev = pGraphicsItemThis->pos();

        // Bounding rectangle of this group in local coordinates (relative to this center).
        QRectF rctBoundingThisPrev = getBoundingRect();
        // Map the bounding rectangle of this group into the parent coordinates of this group.
        rctBoundingThisPrev = pGraphicsItemThis->mapRectToParent(rctBoundingThisPrev);

        // The parent of the child to be added is either the drawing scene or another group.
        // The bounding rectangle of the new child item need to be mapped into the parent
        // coordinates of this group. The parent of this group may either be the scene or a group.
        QRectF rctBoundingChild = i_pGraphObj->getEffectiveBoundingRectOnScene();
        //rctBoundingChild = pGraphicsItemChild->mapRectToScene(rctBoundingChild);
        QGraphicsItem* pGraphicsItemParentThis = parentItem();
        CGraphObjGroup* pGraphObjGroupParentThis = dynamic_cast<CGraphObjGroup*>(pGraphicsItemParentThis);
        if (pGraphicsItemParentThis != nullptr) {
            rctBoundingChild = pGraphicsItemParentThis->mapRectFromScene(rctBoundingChild);
        }

        // Resulting, new bounding rectangle of this group in parent coordinates of this group.
        QRectF rctBoundingThisNew = rctBoundingThisPrev | rctBoundingChild;

        if (rctBoundingThisNew != rctBoundingThisPrev) {
            // When adding a new child already existing childs should not calculate new positions and must not
            // resize themselves if the geometry of the parent group is changed by adding the new child.
            // The size of the already existing childs does not change. Only their position within the group.
            // But the group will set the new position of the already existing childs. For this the childs
            // must not react on the "parentGeometryOnSceneChanged" signal if the groups rectangle is set.
            if (count() > 0) {
                QPointF ptPosThisNew = pGraphicsItemThis->pos();
                QPointF ptMove = ptPosThisNew - ptPosThisPrev;
                QVector<CGraphObj*> arpGraphObjChilds = childs();
                for (CGraphObj* pGraphObjChildExisting : arpGraphObjChilds) {
                    pGraphObjChildExisting->setIgnoreParentGeometryChange(true);
                }
            }

            // Convert (map) the new bounding rectangle of this group into the coordinate system of
            // this groups parent in the unit of the drawing scene.
            CPhysValRect physValRectNew(*m_pDrawingScene);
            if (pGraphObjGroupParentThis != nullptr) {
                physValRectNew = pGraphObjGroupParentThis->convert(rctBoundingThisNew);
            }
            else {
                physValRectNew = m_pDrawingScene->convert(rctBoundingThisNew);
            }
            setRect(physValRectNew);

            // If the group's bounding rectangle has been changed, the groups center point (and also
            // the top left or bottom left corner) may have been changed. The position of the childs
            // in parent coordinates must be updated in the graphics item coordinates system (whose
            // center is the center of the parent) but also the physical values whose reference point
            // is the top left or bottom left corner of the parents bounding rectangle.
            if (count() > 0) {
                QPointF ptPosThisNew = pGraphicsItemThis->pos();
                QPointF ptMove = ptPosThisNew - ptPosThisPrev;
                QVector<CGraphObj*> arpGraphObjChilds = childs();
                for (CGraphObj* pGraphObjChildExisting : arpGraphObjChilds) {
                    QGraphicsItem* pGraphicsItemChildExisting = dynamic_cast<QGraphicsItem*>(pGraphObjChildExisting);
                    QPointF ptPosChildPrev = pGraphicsItemChildExisting->pos();
                    QPointF ptPosChildNew = ptPosChildPrev - ptMove;
                    pGraphicsItemChildExisting->setPos(ptPosChildNew);
                    // As on calling "setRect" the position may not have been changed, force the child
                    // to update it's original shape points in physical coordinates relative to either
                    // the top left or bottom left corner of the parents bounding rectangle.
                    // The parent scale transformation also needs to be newly initialized to take over
                    // the new rectangle of the parent group.
                    pGraphObjChildExisting->initParentTransform();
                    pGraphObjChildExisting->updateTransformedCoorsOnParentGeometryChanged();
                    pGraphObjChildExisting->setIgnoreParentGeometryChange(false);
                }
            }
        }

        // The newly added child will be positioned by the graphics system but the positionChange
        // event in the graphics system does not take the current size of the group into account.
        // When mapping local coordinates into parent coordinates and vice versa the new parent
        // group must have been set. For this the new parent graphical object is set and the graph object is
        // moved to its new position in the tree of objects before adding the item to the GraphicsItemGroup.
        // In addition the signal/slot connection of geometryOnSceneChanged need to be newly set.
        CGraphObjGroup* pGraphObjGroupPrev = i_pGraphObj->parentGroup();
        m_pDrawingScene->getGraphObjsIdxTree()->move(i_pGraphObj, this);
        i_pGraphObj->blockItemChangeUpdatePhysValCoors(true);
        QGraphicsItemGroup_addToGroup(pGraphicsItemChild);
        i_pGraphObj->blockItemChangeUpdatePhysValCoors(false);
        i_pGraphObj->onParentGroupChanged(pGraphObjGroupPrev, this);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
}

//------------------------------------------------------------------------------
/*! @brief Removes the specified item from this group.

    The item will be reparented to this group's parent item, or to 0 if this group
    has no parent. Its position and transformation relative to the scene will stay intact.

    @note The groups size and position remains the same and is not resize to its new content.
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
        /* strObjName   */ path(),
        /* strMethod    */ "removeFromGroup",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    QGraphicsItem* pGraphicsItemChild = dynamic_cast<QGraphicsItem*>(i_pGraphObj);
    if (i_pGraphObj == nullptr) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pGraphObj == nullptr");
    }
    else if (pGraphicsItemChild == nullptr) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "pGraphicsItemChild == nullptr");
    }
    else if (!i_pGraphObj->isChildOf(this, true)) {
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "Cannot only remove direct childs");
    }
    else {
        // The removed child will be positioned by the graphics system.
        // When mapping local coordinates into parent coordinates and vice versa the new parent
        // group must have been set. So the new parent graphical object is set and the graph object is
        // moved to its new position in the tree of objects before removing the item from the GraphicsItemGroup.
        // In addition the signal/slot connection of geometryOnSceneChanged need to be newly set.
        CGraphObjGroup* pGraphObjGroupNew = parentGroup();
        m_pDrawingScene->getGraphObjsIdxTree()->move(i_pGraphObj, pGraphObjGroupNew);
        i_pGraphObj->blockItemChangeUpdatePhysValCoors(true);
        QGraphicsItemGroup_removeFromGroup(pGraphicsItemChild);
        i_pGraphObj->blockItemChangeUpdatePhysValCoors(false);
        i_pGraphObj->onParentGroupChanged(this, pGraphObjGroupNew);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
}

//------------------------------------------------------------------------------
/*! @rief Resizes the groups bounding rectangle to the contained childs.
*/
void CGraphObjGroup::resizeToContent()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "resizeToContent",
        /* strAddInfo   */ "" );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    // Remember current position of this group in parent coordinates.
    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    QPointF ptPosThisPrev = pGraphicsItemThis->pos();

    // Bounding rectangle of this group in local coordinates (relative to this center).
    QRectF rctBoundingThisPrev = getBoundingRect();
    // Map the bounding rectangle of this group into the parent coordinates of this group.
    rctBoundingThisPrev = pGraphicsItemThis->mapRectToParent(rctBoundingThisPrev);

    // Resulting, new bounding rectangle of this group in parent coordinates of this group.
    QRectF rctBoundingThisNew;
    if (count() > 0) {
        QVector<CGraphObj*> arpGraphObjChilds = childs();
        for (CGraphObj* pGraphObjChild : arpGraphObjChilds) {
            QRectF rctBoundingChild = pGraphObjChild->getBoundingRect();
            QGraphicsItem* pGraphicsItemChild = dynamic_cast<QGraphicsItem*>(pGraphObjChild);
            rctBoundingChild = pGraphicsItemChild->mapRectToParent(rctBoundingChild);
            rctBoundingThisNew |= rctBoundingChild;
        }
    }
    rctBoundingThisNew = mapToTopLeftOfBoundingRect(rctBoundingThisNew);

    if (rctBoundingThisNew != rctBoundingThisPrev) {
        // When resizing the group to its content the childs should not calculate new positions
        // and must not resize themselves if the geometry of the parent group is changed.
        // The size of the already existing childs does not change. Only their position within the group.
        // But the group will set the new position of the already existing childs. For this the childs
        // must not react on the "parentGeometryOnSceneChanged" signal if the groups rectangle is set.
        if (count() > 0) {
            QPointF ptPosThisNew = pGraphicsItemThis->pos();
            QPointF ptMove = ptPosThisNew - ptPosThisPrev;
            QVector<CGraphObj*> arpGraphObjChilds = childs();
            for (CGraphObj* pGraphObjChildExisting : arpGraphObjChilds) {
                pGraphObjChildExisting->setIgnoreParentGeometryChange(true);
            }
        }

        // Convert (map) the new bounding rectangle of this group into the coordinate system of
        // this groups parent in the unit of the drawing scene.
        CPhysValRect physValRectNew(*m_pDrawingScene);
        if (parentGroup() != nullptr) {
            physValRectNew = parentGroup()->convert(rctBoundingThisNew);
        }
        else {
            physValRectNew = m_pDrawingScene->convert(rctBoundingThisNew);
        }
        setRect(physValRectNew);

        // If the group's bounding rectangle has been changed, the groups center point (and also
        // the top left or bottom left corner) may have been changed. The position of the childs
        // in parent coordinates must be updated in the graphics item coordinates system (whose
        // center is the center of the parent) but also the physical values whose reference point
        // is the top left or bottom left corner of the parents bounding rectangle.
        if (count() > 0) {
            QPointF ptPosThisNew = pGraphicsItemThis->pos();
            QPointF ptMove = ptPosThisNew - ptPosThisPrev;
            QVector<CGraphObj*> arpGraphObjChilds = childs();
            for (CGraphObj* pGraphObjChildExisting : arpGraphObjChilds) {
                QGraphicsItem* pGraphicsItemChildExisting = dynamic_cast<QGraphicsItem*>(pGraphObjChildExisting);
                QPointF ptPosChildPrev = pGraphicsItemChildExisting->pos();
                QPointF ptPosChildNew = ptPosChildPrev - ptMove;
                pGraphicsItemChildExisting->setPos(ptPosChildNew);
                // The child was not notified about the geometry change (see above) but has to reset
                // the scale transformation parameters of the parent.
                // The parent scale transformation also needs to be newly initialized to take over
                // the new rectangle of the parent group.
                pGraphObjChildExisting->initParentTransform();
                pGraphObjChildExisting->updateTransformedCoorsOnParentGeometryChanged();
                pGraphObjChildExisting->setIgnoreParentGeometryChange(false);
            }
        }
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
}

//------------------------------------------------------------------------------
CGraphObj* CGraphObjGroup::findGraphObj( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CGraphObj*>(find(i_strObjName));
}

//------------------------------------------------------------------------------
/*! @rief Returns the child items of the group.
*/
QVector<CGraphObj*> CGraphObjGroup::childs() const
//------------------------------------------------------------------------------
{
    const QVector<CIdxTreeEntry*> arpChildTreeEntries = CIdxTreeEntry::childs();
    QVector<CGraphObj*> arpGraphObjChilds;
    for (CIdxTreeEntry* pTreeEntryChild : arpChildTreeEntries) {
        CGraphObj* pGraphObjChild = dynamic_cast<CGraphObj*>(pTreeEntryChild);
        if (pGraphObjChild != nullptr) {
            arpGraphObjChilds.append(pGraphObjChild);
        }
    }
    return arpGraphObjChilds;
}

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
QString CGraphObjGroup::getScenePolygonShapePointsString() const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPolygonF plgScene = pGraphicsItemThis->mapToScene(getBoundingRect());
    return qPolygon2Str(plgScene);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the item's rectangle to the given rectangle.

    Depending on the Y axis scale orientation of the drawing scene the coordinates
    must be passed either relative to the top left or relative to the bottom left
    corner of the parent item's bounding rectangle.

    The passed rectangle contains may be rotated (the angle may be in range 0 .. 360°).

    @param [in] i_physValRect
        Rectangle to be set in parent coordinates, depending on the Y scale orientation
        relative to the top left or bottom left corner of parent item's bounding rectangle.
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
        /* strObjName   */ path(),
        /* strMethod    */ "setRect",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    QPointF ptPosPrev = pos();

    // Depending on the Y scale orientation of the drawing scene the rectangle coordinates
    // have been passed either relative to the top left or bottom left corner of the
    // parent item's bounding rectangle.
    // The coordinates need to be transformed into the local coordinate system of the graphical
    // object whose origin point is the center of the objects bounding rectangle.

    QRectF rectF;
    CPhysVal physValAngle;
    QPointF ptPos = getItemPosAndLocalCoors(i_physValRect, rectF, physValAngle);

    bool bGeometryOnSceneChanged = false;

    // If the coordinates MUST be updated
    // (initially set, changed or after the drawing size has been changed)
    if (m_rectOrig.isNull() || m_physValRectOrig.isNull() || m_rectOrig != rectF || m_physValRectOrig != i_physValRect || m_physValRotationAngle != i_physValRect.angle() || m_bForceConversionToSceneCoors)
    {
        updateDivLinesMetrics(rectF.size(), QSizeF(i_physValRect.width().getVal(), i_physValRect.height().getVal()));

        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsItem_prepareGeometryChange();

        {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdatePhysValCoorsBlockedCounter);
            CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

            // Store original, untransformed rectangle coordinates.
            CPhysValRect physValRect(i_physValRect);
            physValRect.setAngle(0.0);
            setPhysValRectOrig(physValRect);
            setPhysValRectScaled(physValRect);
            setPhysValRectScaledAndRotated(i_physValRect);

            // Set the groups rectangle in local coordinate system.
            setRectOrig(rectF);
            setRectScaled(rectF);

            // Move the object to the parent position.
            // This has to be done after resizing the groups rectangle which updates the local coordinates
            // of the rectangle with origin (0/0) at the rectangle's center point.
            // "setPos" will trigger an itemChange call which will update the position of the
            // selection points and labels. To position the selection points and labels correctly
            // the local coordinate system must be up-to-date.
            // Also note that itemChange must not overwrite the current rectangle coordinates (refCountGuard).
            if (ptPos != ptPosPrev) {
                QGraphicsItem_setPos(ptPos);
            }
            if (physValAngle != m_physValRotationAngle) {
                setRotationAngle(physValAngle);
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
    @param [in] i_rowVersion
        If set to Current (default), the scaled and rotated coordinates are returned.
        If set to Original, the unscaled and not rotated coordinates are returned.
        The Original coordinates are used by childs to calculate the parent's scale factor.
*/
CPhysValRect CGraphObjGroup::getRect(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated;
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
void CGraphObjGroup::setCenter(const QPointF& i_pt)
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

    CPhysValRect physValRect = getRect();
    physValRect.setCenter(i_pt);
    setRect(physValRect);
}

//------------------------------------------------------------------------------
void CGraphObjGroup::setCenter(const CPhysValPoint& i_physValPoint)
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

    CPhysValRect physValRect = getRect();
    physValRect.setCenter(i_physValPoint);
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
/*! @brief Converts the given point in pixels into the current unit of the drawing scene.

    @param [in] i_pt

    @return Converted value.
*/
CPhysValPoint CGraphObjGroup::convert(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(CPhysValPoint(*m_pDrawingScene, i_pt, Units.Length.px), drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given point in pixels into the desired unit.

    @param [in] i_pt
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValPoint CGraphObjGroup::convert(const QPointF& i_pt, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(CPhysValPoint(*m_pDrawingScene, i_pt, Units.Length.px), i_unitDst);
}

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
            physValPoint = CPhysValPoint(*m_pDrawingScene, physValX, physValY);
        }
        else if ((i_physValPoint.unit() == Units.Length.px) && Units.Length.isMetricUnit(i_unitDst)) {
            QPointF pt = i_physValPoint.toQPointF();
            double fx = m_divLinesMetricsX.getVal(pt.x(), false);
            double fy = m_divLinesMetricsY.getVal(pt.y(), false);
            CPhysVal physValX(fx, drawingSize.unit(), drawingSize.imageCoorsResolution());
            CPhysVal physValY(fy, drawingSize.unit(), drawingSize.imageCoorsResolution());
            physValX.convertValue(i_unitDst);
            physValY.convertValue(i_unitDst);
            physValPoint = CPhysValPoint(*m_pDrawingScene, physValX, physValY);
        }
        else if (Units.Length.isMetricUnit(i_physValPoint.unit()) && (i_unitDst == Units.Length.px)) {
            CPhysVal physValX = i_physValPoint.x();
            CPhysVal physValY = i_physValPoint.y();
            double fX_px = m_divLinesMetricsX.getValInPix(physValX.getVal(drawingSize.unit()));
            double fY_px = m_divLinesMetricsY.getValInPix(physValY.getVal(drawingSize.unit()));
            physValPoint = CPhysValPoint(*m_pDrawingScene, fX_px, fY_px, i_unitDst);
        }
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size in pixels into the current unit of the drawing scene.

    @param [in] i_size

    @return Converted value.
*/
CPhysValSize CGraphObjGroup::convert(const QSizeF& i_size) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(CPhysValSize(*m_pDrawingScene, i_size, Units.Length.px), drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size in pixels into the desired unit.

    @param [in] i_size
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValSize CGraphObjGroup::convert(const QSizeF& i_size, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(CPhysValSize(*m_pDrawingScene, i_size, Units.Length.px), i_unitDst);
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size into the current unit of the drawing scene.

    @param [in] i_physValSize

    @return Converted value.
*/
CPhysValSize CGraphObjGroup::convert(const CPhysValSize& i_physValSize) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(i_physValSize, drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given size into the desired unit.

    @param [in] i_physValSize
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValSize CGraphObjGroup::convert(const CPhysValSize& i_physValSize, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysValSize physValSize = i_physValSize;
    if (i_physValSize.unit() != i_unitDst) {
        if (Units.Length.isMetricUnit(i_physValSize.unit()) && Units.Length.isMetricUnit(i_unitDst)) {
            CPhysVal physValWidth = i_physValSize.width();
            CPhysVal physValHeight = i_physValSize.width();
            physValWidth.convertValue(i_unitDst);
            physValHeight.convertValue(i_unitDst);
            CPhysValRes physValRes = drawingSize.imageCoorsResolution();
            physValRes.convertValue(i_unitDst);
            physValWidth.setRes(physValRes);
            physValHeight.setRes(physValRes);
            physValSize = CPhysValSize(*m_pDrawingScene, physValWidth, physValHeight);
        }
        else if ((i_physValSize.unit() == Units.Length.px) && Units.Length.isMetricUnit(i_unitDst)) {
            // The drawing size in pixels has been incremented by one pixel.
            // If the pixel resolution is e.g. 3.5 px/mm, and the width is 36 px, the width is 10 mm.
            QSizeF sizeF = i_physValSize.toQSizeF();
            double dx = m_divLinesMetricsX.getDistance(sizeF.width());
            double dy = m_divLinesMetricsY.getDistance(sizeF.height());
            CPhysVal physValWidth(dx, drawingSize.unit(), drawingSize.imageCoorsResolution());
            CPhysVal physValHeight(dy, drawingSize.unit(), drawingSize.imageCoorsResolution());
            physValWidth.convertValue(i_unitDst);
            physValHeight.convertValue(i_unitDst);
            physValSize = CPhysValSize(*m_pDrawingScene, physValWidth, physValHeight);
        }
        else if (Units.Length.isMetricUnit(i_physValSize.unit()) && (i_unitDst == Units.Length.px)) {
            // The drawing size in pixels has been incremented by one pixel.
            // If the pixel resolution is e.g. 3.5 px/mm, and the width is 36 px, the width is 10 mm.
            double dx = i_physValSize.width().getVal(drawingSize.unit());
            double dy = i_physValSize.height().getVal(drawingSize.unit());
            double dx_px = m_divLinesMetricsX.getDistanceInPix(dx);
            double dy_px = m_divLinesMetricsY.getDistanceInPix(dy);
            CPhysVal physValWidth(dx_px, Units.Length.px, drawingSize.imageCoorsResolutionInPx());
            CPhysVal physValHeight(dy_px, Units.Length.px, drawingSize.imageCoorsResolutionInPx());
            physValSize = CPhysValSize(*m_pDrawingScene, physValWidth, physValHeight);
        }
    }
    return physValSize;
}

//------------------------------------------------------------------------------
/*! @brief Converts the given line in pixels into the current unit of the drawing scene.

    @param [in] i_line

    @return Converted value.
*/
CPhysValLine CGraphObjGroup::convert(const QLineF& i_line) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(CPhysValLine(*m_pDrawingScene, i_line, Units.Length.px), drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given line in pixels into the desired unit.

    @param [in] i_line
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValLine CGraphObjGroup::convert(const QLineF& i_line, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(CPhysValLine(*m_pDrawingScene, i_line, Units.Length.px), i_unitDst);
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
    CPhysValPoint physValP1 = convert(i_physValLine.p1(), i_unitDst);
    CPhysValPoint physValP2 = convert(i_physValLine.p2(), i_unitDst);
    return CPhysValLine(physValP1, physValP2);
}

//------------------------------------------------------------------------------
/*! @brief Converts the given rectangle in pixels into the current unit of the drawing scene.

    @param [in] i_rect

    @return Converted value.
*/
CPhysValRect CGraphObjGroup::convert(const QRectF& i_rect) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(CPhysValRect(*m_pDrawingScene, i_rect.topLeft(), i_rect.size(), Units.Length.px), drawingSize.unit());
}

//------------------------------------------------------------------------------
/*! @brief Converts the given rectangle in pixels into the desired unit.

    @param [in] i_rect
    @param [in] i_unitDst

    @return Converted value.
*/
CPhysValRect CGraphObjGroup::convert(const QRectF& i_rect, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(CPhysValRect(*m_pDrawingScene, i_rect.topLeft(), i_rect.size(), Units.Length.px), i_unitDst);
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
    return CPhysValRect(physValTL, physValBR);
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Maps the given physical point, which is relative to either the top left
           or bottom left corner (depending on the Y-Axis-Scale-Orientation) of the
           groups bounding rectangle to the physical value on the drawing scene.

    @note The graphics item method "mapToScene" uses the graphics item local coordinate
          system whose origin is the center point of the bounding rectangle.

    @param [in] i_physValPoint
        Point to be mapped.

    @return Point in scene coordinates in the unit of the drawing scene.
*/
CPhysValPoint CGraphObjGroup::mapToScene(const CPhysValPoint& i_physValPoint) const
//------------------------------------------------------------------------------
{
    return mapToScene(i_physValPoint, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical point, which is relative to either the top left
           or bottom left corner (depending on the Y-Axis-Scale-Orientation) of the
           groups bounding rectangle to the physical value on the drawing scene.

    @note The graphics item method "mapToScene" uses the graphics item local coordinate
          system whose origin is the center point of the bounding rectangle.

    @param [in] i_physValPoint
        Point to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Point in scene coordinates in the desired unit.
*/
CPhysValPoint CGraphObjGroup::mapToScene(const CPhysValPoint& i_physValPoint, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pt {" + i_physValPoint.toString() + "} " + i_physValPoint.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mapToScene",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPointF pt = convert(i_physValPoint, Units.Length.px).toQPointF();
    pt = mapFromTopLeftOfBoundingRect(pt);
    pt = pGraphicsItemThis->mapToScene(pt);
    CPhysValPoint physValPoint = m_pDrawingScene->convert(pt, i_unitDst);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Pt {" + physValPoint.toString() + "} " + physValPoint.unit().symbol());
    }
    return physValPoint;
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical line, whose points are relative to either the top left
           or bottom left corner (depending on the Y-Axis-Scale-Orientation) of the
           groups bounding rectangle to the physical value on the drawing scene.

    @note The graphics item method "mapToScene" uses the graphics item local coordinate
          system whose origin is the center point of the bounding rectangle.

    @param [in] i_physValLine
        Line to be mapped.

    @return Line in scene coordinates in the unit of the drawing scene.
*/
CPhysValLine CGraphObjGroup::mapToScene(const CPhysValLine& i_physValLine) const
//------------------------------------------------------------------------------
{
    return mapToScene(i_physValLine, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical line, whose points are relative to either the top left
           or bottom left corner (depending on the Y-Axis-Scale-Orientation) of the
           groups bounding rectangle to the physical value on the drawing scene.

    @note The graphics item method "mapToScene" uses the graphics item local coordinate
          system whose origin is the center point of the bounding rectangle.

    @param [in] i_physValLine
        Line to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Line in scene coordinates in the desired unit.
*/
CPhysValLine CGraphObjGroup::mapToScene(const CPhysValLine& i_physValLine, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Line {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mapToScene",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QLineF lineF = convert(i_physValLine, Units.Length.px).toQLineF();
    lineF = mapFromTopLeftOfBoundingRect(lineF);
    lineF.setP1(pGraphicsItemThis->mapToScene(lineF.p1()));
    lineF.setP2(pGraphicsItemThis->mapToScene(lineF.p2()));
    CPhysValLine physValLine = m_pDrawingScene->convert(lineF, i_unitDst);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Line {" + physValLine.toString() + "} " + physValLine.unit().symbol());
    }
    return physValLine;
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical rectangle, whose points are relative to either the top left
           or bottom left corner (depending on the Y-Axis-Scale-Orientation) of the
           groups bounding rectangle to the physical value on the drawing scene.

    @note The graphics item method "mapToScene" uses the graphics item local coordinate
          system whose origin is the center point of the bounding rectangle.

    @param [in] i_physValRect
        Rectangle to be mapped.

    @return Rectangle in scene coordinates in the unit of the drawing scene.
*/
CPhysValRect CGraphObjGroup::mapToScene(const CPhysValRect& i_physValRect) const
//------------------------------------------------------------------------------
{
    return mapToScene(i_physValRect, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical rectangle, whose points are relative to either the top left
           or bottom left corner (depending on the Y-Axis-Scale-Orientation) of the
           groups bounding rectangle to the physical value on the drawing scene.

    @param [in] i_physValRect
        Rectangle to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Rectangle in scene coordinates in the desired unit.
*/
CPhysValRect CGraphObjGroup::mapToScene(const CPhysValRect& i_physValRect, const CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Rect {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mapToScene",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    CPhysValRect physValRect(i_physValRect);
    physValRect.setAngle(0.0);
    physValRect = convert(physValRect, Units.Length.px);
    QRectF rectF(physValRect.topLeft().toQPointF(), physValRect.size().toQSizeF());
    rectF = pGraphicsItemThis->mapRectToScene(rectF);
    rectF = mapFromTopLeftOfBoundingRect(rectF);
    physValRect.setTopLeft(rectF.topLeft());
    physValRect.setSize(rectF.size());
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Rect {" + physValRect.toString() + "} " + physValRect.unit().symbol());
    }
    return physValRect;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::mapRotationAngleToScene(const CPhysVal& i_physValAngle)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValAngle.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mapRotationAngleToScene",
        /* strAddInfo   */ strMthInArgs );

    CPhysVal physValAngle = i_physValAngle;
    if (parentGroup() != nullptr) {
        physValAngle = parentGroup()->mapRotationAngleToScene(physValAngle);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(physValAngle.toString());
    }
    return physValAngle;
}

//------------------------------------------------------------------------------
CPhysVal CGraphObjGroup::mapRotationAngleTo(const CPhysVal& i_physValAngle, CGraphObjGroup* i_pGraphObjGroup)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValAngle.toString() + ", " + QString(i_pGraphObjGroup == nullptr ? "null" : i_pGraphObjGroup->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "mapRotationAngleTo",
        /* strAddInfo   */ strMthInArgs );

    CPhysVal physValAngle = i_physValAngle;
    if (isChildOf(i_pGraphObjGroup)) {
        CGraphObjGroup* pGraphObjGroupParent = parentGroup();
        physValAngle += pGraphObjGroupParent->rotationAngle();
        while (pGraphObjGroupParent != i_pGraphObjGroup){
            pGraphObjGroupParent = pGraphObjGroupParent->parentGroup();
            physValAngle += pGraphObjGroupParent->rotationAngle();
        }
    }
    else if (i_pGraphObjGroup->isChildOf(this)) {
        #pragma message(__TODO__"Implement method")
        throw CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    }
    else {
        #pragma message(__TODO__"Implement method")
        throw CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(physValAngle.toString());
    }
    return physValAngle;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the scaled but not rotated rectangle coordinates in local
           coordinates relative to the origin of the groups bounding rectangle.

    @return Bounding rectangle in local coordinates.
*/
QRectF CGraphObjGroup::getBoundingRect() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getBoundingRect",
        /* strAddInfo   */ "" );
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(m_rectScaled) + "}");
    }
    return m_rectScaled;
}

//------------------------------------------------------------------------------
/*! @brief Returns the effective (resulting) bounding rectangle of this item
           on the drawing scene.

    To get the effective bounding rectangle the left most, the right most
    as well as the top most and bottom most shape points of the transformed
    (rotated and scaled) object are are taken into account.

    If the object is rotated the effective bounding rectangle is not the
    bounding rectangle (in item coordinates) mapped to the scene.
    Before mapping the points to the scene the TopMost, BottomMost, LeftMost
    and RightMost points of the rotated object have to be calculated and each
    point has to be mapped to the scene.

    E.g. rotated trapez on the scene:

                             + TopMost
                            / \
                           /   \
                          /     \
                LeftMost +       + RightMost
                          \     /
                           \   /
                            \ /
                             + BottomMost
*/
QRectF CGraphObjGroup::getEffectiveBoundingRectOnScene() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getEffectiveBoundingRectOnScene",
        /* strAddInfo   */ "" );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QPolygonF plg({
        pGraphicsItemThis->mapToScene(m_rectScaled.topLeft()),
        pGraphicsItemThis->mapToScene(m_rectScaled.topRight()),
        pGraphicsItemThis->mapToScene(m_rectScaled.bottomRight()),
        pGraphicsItemThis->mapToScene(m_rectScaled.bottomLeft())});
    QRectF rctBounding = plg.boundingRect();
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rctBounding) + "}");
    }
    return rctBounding;
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

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
//        /* strObjName   */ path(),
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
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the clockwise rotation angle, in degrees, around the Z axis.

    The default value is 0 (i.e., the item is not rotated). Assigning a negative
    value will rotate the item counter-clockwise.
    Normally the rotation angle is in the range (-360, 360), but it's also possible
    to assign values outside of this range (e.g., a rotation of 370 degrees is the
    same as a rotation of 10 degrees).
*/
void CGraphObjGroup::setRotationAngle(const CPhysVal& i_physValAngle)
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
        /* strMethod    */ "CGraphObj::setRotationAngle",
        /* strAddInfo   */ strMthInArgs );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    if (m_physValRotationAngle != i_physValAngle) {
        {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);
            m_physValRotationAngle = i_physValAngle;
            CPhysValRect physValRect = m_physValRectScaledAndRotated;
            physValRect.setAngle(i_physValAngle);
            setPhysValRectScaledAndRotated(physValRect);
            QGraphicsItem_setRotation(m_physValRotationAngle.getVal(Units.Angle.Degree));
        }
        emit_geometryOnSceneChanged();
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
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
//        /* strObjName   */ path(),
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
//            o_pHitInfo->setCursor(Math::degree2Rad(m_fRotAngleCurr_deg));
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
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the polar coordinates in length and angle of the given point
           to the selection point of the graphical object.

    How the angle of anchor lines to selection points is interpreted depends on
    the graphical object type and the selection point.

    For the graphical object Group the center point and all selection point on the
    bounding rectangle are allowed for this method.

    Example: Horizontal Line

                       + Pt
                      / Calculated Angle: 60°
                     /           TC
                 TL +------------x------------+ TR
                    |                         |
                    |         Center          |
                 LC x            x            x RC
                    |                         |
                    |                         |
                 BL +------------x------------+ BR
                                 BC
    @note This method is used to keep the relative position of labels to the
          graphical object they are linked to if the linked object is resized,
          rotated or moved.

          For example if the group would be rotated by 90°:
                                RC
                      TR +-------x--------+ BR
                         |                |
                         |                |
                         |                |
                         |    Center      |
                      TC x       x        x BC
                         |                |
                         |                |
                Pt + 60° |                |
                     \   |                |
                      TL +-------x--------+ BL
                                 LC
*/
SPolarCoors CGraphObjGroup::getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QRectF thisRect = getBoundingRect();
    QPointF ptThisLineSceneCoorsP1;
    QPointF ptThisLineSceneCoorsP2;
    QPointF ptSelPtSceneCoors;
    if (i_selPt == ESelectionPoint::TopLeft) {
        ptThisLineSceneCoorsP1 = thisRect.topLeft();
        ptThisLineSceneCoorsP2 = thisRect.topRight();
        ptSelPtSceneCoors = thisRect.topLeft();
    }
    else if (i_selPt == ESelectionPoint::TopRight) {
        ptThisLineSceneCoorsP1 = thisRect.topRight();
        ptThisLineSceneCoorsP2 = thisRect.topLeft();
        ptSelPtSceneCoors = thisRect.topRight();
    }
    else if (i_selPt == ESelectionPoint::BottomRight) {
        ptThisLineSceneCoorsP1 = thisRect.bottomRight();
        ptThisLineSceneCoorsP2 = thisRect.bottomLeft();
        ptSelPtSceneCoors = thisRect.bottomRight();
    }
    else if (i_selPt == ESelectionPoint::BottomLeft) {
        ptThisLineSceneCoorsP1 = thisRect.bottomLeft();
        ptThisLineSceneCoorsP2 = thisRect.bottomRight();
        ptSelPtSceneCoors = thisRect.bottomLeft();
    }
    else if (i_selPt == ESelectionPoint::TopCenter) {
        ptThisLineSceneCoorsP1 = QPointF(thisRect.center().x(), thisRect.top());
        ptThisLineSceneCoorsP2 = thisRect.topRight();
        ptSelPtSceneCoors = QPointF(thisRect.center().x(), thisRect.top());
    }
    else if (i_selPt == ESelectionPoint::RightCenter) {
        ptThisLineSceneCoorsP1 = QPointF(thisRect.right(), thisRect.center().y());
        ptThisLineSceneCoorsP2 = thisRect.bottomRight();
        ptSelPtSceneCoors = QPointF(thisRect.right(), thisRect.center().y());
    }
    else if (i_selPt == ESelectionPoint::BottomCenter) {
        ptThisLineSceneCoorsP1 = QPointF(thisRect.center().x(), thisRect.bottom());
        ptThisLineSceneCoorsP2 = thisRect.bottomLeft();
        ptSelPtSceneCoors = QPointF(thisRect.center().x(), thisRect.bottom());
    }
    else if (i_selPt == ESelectionPoint::LeftCenter) {
        ptThisLineSceneCoorsP1 = QPointF(thisRect.left(), thisRect.center().y());
        ptThisLineSceneCoorsP2 = thisRect.topLeft();
        ptSelPtSceneCoors = QPointF(thisRect.left(), thisRect.center().y());
    }
    else /*if (i_selPt == ESelectionPoint::Center)*/ {
        ptThisLineSceneCoorsP1 = thisRect.center();
        ptThisLineSceneCoorsP2 = QPointF(thisRect.right(), thisRect.center().y());
        ptSelPtSceneCoors = thisRect.center();
    }
    ptThisLineSceneCoorsP1 = pGraphicsItemThis->mapToScene(ptThisLineSceneCoorsP1);
    ptThisLineSceneCoorsP2 = pGraphicsItemThis->mapToScene(ptThisLineSceneCoorsP2);
    ptSelPtSceneCoors = pGraphicsItemThis->mapToScene(ptSelPtSceneCoors);
    QLineF thisLineSceneCoors(ptThisLineSceneCoorsP1, ptThisLineSceneCoorsP2);
    QLineF lineFromSelPtSceneCoors(ptSelPtSceneCoors, i_pt);
    double fAngle_degree = thisLineSceneCoors.angleTo(lineFromSelPtSceneCoors);
    return SPolarCoors(lineFromSelPtSceneCoors.length(), fAngle_degree);
}

//------------------------------------------------------------------------------
/*! @brief Returns a line with the given length and angle with the start point (P1)
           at the given selection point in scene coordinates.

    For the graphical object Group the center point and all selection point on the
    bounding rectangle are allowed for this method.

    For more details see base implementation in CGraphObj.
*/
QLineF CGraphObjGroup::getAnchorLineToSelectionPointFromPolarInSceneCoors(
    const SPolarCoors& i_polarCoors, ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QRectF thisRect = getBoundingRect();
    QPointF ptSelPtLineP1;
    QPointF ptSelPtLineP2;
    if (i_selPt == ESelectionPoint::TopLeft) {
        ptSelPtLineP1 = thisRect.topLeft();
        ptSelPtLineP2 = thisRect.topRight();
    }
    else if (i_selPt == ESelectionPoint::TopRight) {
        ptSelPtLineP1 = thisRect.topRight();
        ptSelPtLineP2 = thisRect.topLeft();
    }
    else if (i_selPt == ESelectionPoint::BottomRight) {
        ptSelPtLineP1 = thisRect.bottomRight();
        ptSelPtLineP2 = thisRect.bottomLeft();
    }
    else if (i_selPt == ESelectionPoint::BottomLeft) {
        ptSelPtLineP1 = thisRect.bottomLeft();
        ptSelPtLineP2 = thisRect.bottomRight();
    }
    else if (i_selPt == ESelectionPoint::TopCenter) {
        ptSelPtLineP1 = QPointF(thisRect.center().x(), thisRect.top());
        ptSelPtLineP2 = thisRect.topRight();
    }
    else if (i_selPt == ESelectionPoint::RightCenter) {
        ptSelPtLineP1 = QPointF(thisRect.right(), thisRect.center().y());
        ptSelPtLineP2 = thisRect.bottomRight();
    }
    else if (i_selPt == ESelectionPoint::BottomCenter) {
        ptSelPtLineP1 = QPointF(thisRect.center().x(), thisRect.bottom());
        ptSelPtLineP2 = thisRect.bottomLeft();
    }
    else if (i_selPt == ESelectionPoint::LeftCenter) {
        ptSelPtLineP1 = QPointF(thisRect.left(), thisRect.center().y());
        ptSelPtLineP2 = thisRect.topLeft();
    }
    else /*if (i_selPt == ESelectionPoint::Center)*/ {
        ptSelPtLineP1 = thisRect.center();
        ptSelPtLineP2 = QPointF(thisRect.right(), thisRect.center().y());
    }
    ptSelPtLineP1 = pGraphicsItemThis->mapToScene(ptSelPtLineP1);
    ptSelPtLineP2 = pGraphicsItemThis->mapToScene(ptSelPtLineP2);
    QLineF lineSelPtSceneCoors(ptSelPtLineP1, ptSelPtLineP2);
    return ZS::Draw::getLineFromPolar(
        i_polarCoors.m_fLength_px, i_polarCoors.m_fAngle_degrees, lineSelPtSceneCoors);
}

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::showSelectionPoints(TSelectionPointTypes i_selPts)
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
        if (i_selPts & c_uSelectionPointsBoundingRectAll) {
            showSelectionPointsOfBoundingRect(getBoundingRect());
        }
    }
}

/*==============================================================================
public: // overridables of base class CGraphObj (text labels)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the list of the possible anchor points for the given label name.

    For the predefined labels of the group the following applies:

    - The "Name" label may be anchored to the center point of the bounding rectangle
      and the center points of the border lines.
    - "TopLeft", "TopRight", "BottomRight" and "BottomLeft" may be anchored to their
       corresponding bounding rectangle corners.

    User defined labels may be anchored to any selection point at the bounding rectangle.

    Please note that the most common used selection points should be at the beginning
    of the list so that combo boxes to select the selection point start with those.

    @return List of possbile selection points.
*/
QList<SGraphObjSelectionPoint> CGraphObjGroup::getPossibleLabelAnchorPoints(const QString& i_strName) const
//------------------------------------------------------------------------------
{
    static QList<SGraphObjSelectionPoint> s_arSelPtsUserDefined;
    if (s_arSelPtsUserDefined.isEmpty()) {
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::Center));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::TopCenter));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::BottomCenter));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::LeftCenter));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::RightCenter));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::TopLeft));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::TopRight));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::BottomRight));
        s_arSelPtsUserDefined.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::BottomLeft));
    }
    static QHash<QString, QList<SGraphObjSelectionPoint>> s_hshSelPtsPredefined;
    if (s_hshSelPtsPredefined.isEmpty()) {
        QList<SGraphObjSelectionPoint> arSelPts;
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::Center));
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::TopCenter));
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::BottomCenter));
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::LeftCenter));
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::RightCenter));
        s_hshSelPtsPredefined.insert(c_strLabelName, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::TopLeft));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameTopLeft, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::TopRight));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameTopRight, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::BottomRight));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameBottomRight, arSelPts);
        arSelPts.clear();
        arSelPts.append(SGraphObjSelectionPoint(const_cast<CGraphObjGroup*>(this), ESelectionPoint::BottomLeft));
        s_hshSelPtsPredefined.insert(c_strGeometryLabelNameBottomLeft, arSelPts);
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
bool CGraphObjGroup::labelHasDefaultValues(const QString& i_strName) const
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
        else if (i_strName == c_strGeometryLabelNameTopLeft) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopLeft) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameTopRight) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopRight) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameBottomRight) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::BottomRight) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameBottomLeft) {
            if (labelDscr.m_strText != i_strName) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::BottomLeft) {
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
bool CGraphObjGroup::geometryLabelHasDefaultValues(const QString& i_strName) const
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
        else if (i_strName == c_strGeometryLabelNameTopLeft) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopLeft) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameTopRight) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopRight) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameBottomRight) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::BottomRight) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameBottomLeft) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::BottomLeft) {
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
        else if (i_strName == c_strGeometryLabelNameWidth) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::LeftCenter) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_selPt != ESelectionPoint::RightCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameHeight) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::TopCenter) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_selPt != ESelectionPoint::BottomCenter) {
                bHasDefaultValues = false;
            }
        }
        else if (i_strName == c_strGeometryLabelNameAngle) {
            if (labelDscr.m_selPt1.m_selPtType != ESelectionPointType::BoundingRectangle) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt1.m_selPt != ESelectionPoint::Center) {
                bHasDefaultValues = false;
            }
            else if (labelDscr.m_selPt2.m_selPt != ESelectionPoint::RightCenter) {
                bHasDefaultValues = false;
            }
        }
    }
    return bHasDefaultValues;
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
        /* strObjName   */ path(),
        /* strMethod    */ "onDrawingSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    m_bForceConversionToSceneCoors = true;

    throw CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);

    // Object shape points in local coordinates.
    QRectF rectF = getBoundingRect();

    // Depending on the Y scale orientation of the drawing scene the item coordinates must
    // be either returned relative to the top left corner or relative to the bottom left
    // corner of the parent's bounding rectangle.
    CPhysValRect physValRect(*m_pDrawingScene, rectF.topLeft(), rectF.size(), Units.Length.px);
    CGraphObjGroup* pGraphObjGroup = parentGroup();
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
        /* strObjName   */ path(),
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ "" );

    QRectF rctBounding = getBoundingRect();
    for (CGraphObjSelectionPoint* pGraphObjSelPt : m_arpSelPtsBoundingRect){
        if (pGraphObjSelPt != nullptr) {
            QRectF rctSelPt = pGraphObjSelPt->boundingRect();
            QPolygonF plgSelPt = mapFromItem(pGraphObjSelPt, rctSelPt);
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
        /* strObjName   */ path(),
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    i_pPainter->save();
    i_pPainter->setRenderHints(s_painterRenderHints);

    QPen pn;
    QBrush brush;

    QRectF rctBounding = getBoundingRect();

    if ((m_pDrawingScene->getMode() == EMode::Edit) && (m_bIsHit || m_bIsHighlighted || isSelected())) {
        QPainterPath outline;
        outline.moveTo(rctBounding.topLeft());
        outline.lineTo(rctBounding.topRight());
        outline.lineTo(rctBounding.bottomRight());
        outline.lineTo(rctBounding.bottomLeft());
        outline.lineTo(rctBounding.topLeft());
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

    brush.setColor(m_drawSettings.getFillColor());
    brush.setStyle(fillStyle2QtBrushStyle(m_drawSettings.getFillStyle().enumerator()));
    i_pPainter->setBrush(brush);

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

    if (m_gridSettings.areLinesVisible() || m_gridSettings.areLabelsVisible()) {
        if (m_gridSettings.areLinesVisible()) {
            paintGridLines(i_pPainter);
        }
        if (m_gridSettings.areLabelsVisible()) {
            paintGridLabelsDivisionLines(i_pPainter);
            paintGridLabels(i_pPainter);
        }
    }
    i_pPainter->restore();
}

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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
    //        m_fRotAngleCurr_deg = Math::rad2Degree(fRotAngle_rad);

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
        /* strObjName   */ path(),
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
    //        QRectF rctBoundingSceneNew         = mapRectToScene(m_rctCurr);
    //        QRectF    rctBoundingSceneNew = plgSceneNew.boundingRect();
    //        QPointF   ptRotOriginSceneNew = rctBoundingSceneNew.center();
    //        double    fAngle_rad          = Math::degree2Rad(m_fRotAngleCurr_deg);
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
        /* strObjName   */ path(),
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
            //updateInternalScenePos();
            bGeometryChanged = true;
            bTreeEntryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged) {
        // The object may be moved or transformed by several methods.
        // "itemChange" is a central point to update the coordinates upon those changes.
        if (m_iItemChangeUpdatePhysValCoorsBlockedCounter == 0) {
            //tracePositionInfo(mthTracer, EMethodDir::Enter);
            // Update the object shape point in parent coordinates kept in the unit of the drawing scene.
            // For groups the original coordinates are only updated when adding the group to
            // or removing the group from another group.
            //initParentScaleParameters();
            //bGeometryChanged = true;
        }
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemPositionHasChanged) {
        // The object may be moved or transformed by several methods.
        // "itemChange" is a central point to update the coordinates upon those changes.
        if (m_iItemChangeUpdatePhysValCoorsBlockedCounter == 0) {
            tracePositionInfo(mthTracer, EMethodDir::Enter);
            // Update the object shape point in parent coordinates kept in the unit of the drawing scene.
            // For groups the original coordinates are only updated when adding the group to
            // or removing the group from another group.
            updateTransformedCoorsOnItemPositionChanged();
            //applyGeometryChangeToChildrens();
            bGeometryChanged = true;
        }
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemRotationHasChanged) {
        tracePositionInfo(mthTracer, EMethodDir::Enter);
        //updateLineEndArrowHeadPolygons();
        //tracePositionInfo(mthTracer, EMethodDir::Leave);
        bGeometryChanged = true;
        bTreeEntryChanged = true;
    }
    else if (i_change == ItemChildAddedChange) {
        // A child is added to this item. The value argument is the new child item
        // (i.e., a QGraphicsItem pointer). Do not pass this item to any item's setParentItem()
        // function as this notification is delivered. The return value is unused; you cannot
        // adjust anything in this notification. Note that the new child might not be fully
        // constructed when this notification is sent; calling pure virtual functions on the
        // child can lead to a crash.
    }
    else if (i_change == ItemChildRemovedChange) {
        // A child is removed from this item. The value argument is the child item that is
        // about to be removed (i.e., a QGraphicsItem pointer). The return value is unused;
        // you cannot adjust anything in this notification.
    }
    else if (i_change == ItemSelectedHasChanged) {
        QGraphicsItem_prepareGeometryChange();
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
//        /* strObjName   */ path(),
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
/*! @brief The slot method is called if the parent item of the item changes
           its geometry on the scene and emits the geometryOnSceneChanged signal.

    When resizing a group all children of the group should be resized and positioned so
    that they keep their relative positions and sizes within the group. If the item
    was added to a new group the current rectangle of the parent group was stored as
    the original group rectangle.

    If the item is removed from a group (but not added to a new group) the original
    parent group rectangle is invalidated.

    The default implementation just calculates the current scale factors of the
    parent group and emits the signal geometryOnSceneChanged.

    Labels override this method to udpate their positions on the scene and to update
    the geometry information of the items they are linked to but do not emit the
    geometryOnSceneChanged signal again.

    Other graphical objects must override this method to update their local graphics
    item coordinates and to update their position in the parent group.

    @note This method must return immediately if the parent group is about to
          add another child (see flag m_iIgnoreParentGeometryChange).

    @param [in] i_pGraphObjParent
        Pointer to parent item whose geometry on the scene has been changed.
    @param [in] i_bParentOfParentChanged
        false (default), if the geometry of the parent has been changed directly.
        true if the geometry has been changed because the parent got a new parent.
*/
void CGraphObjGroup::onGraphObjParentGeometryOnSceneChanged(
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
            CPhysValRect physValRect = getPhysValRectScaled(m_physValRectOrig);
            setPhysValRectScaled(physValRect);
            physValRect.setAngle(m_physValRotationAngle);
            setPhysValRectScaledAndRotated(physValRect);

            QPointF ptPosPrev = pos();

            QRectF rectF;
            CPhysVal physValAngle;
            QPointF ptPos = getItemPosAndLocalCoors(physValRect, rectF, physValAngle);

            updateDivLinesMetrics(rectF.size(), QSizeF(physValRect.width().getVal(), physValRect.height().getVal()));

            // Prepare the item for a geometry change. This function must be called before
            // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
            QGraphicsItem_prepareGeometryChange();

            {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdatePhysValCoorsBlockedCounter);
                CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

                // Set the groups rectangle in local coordinate system.
                setRectScaled(rectF);

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
void CGraphObjGroup::onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint)
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
                //setTopCenter(physValParentSelPt.y());
                break;
            }
            case ESelectionPoint::RightCenter: {
                //setRightCenter(physValParentSelPt.x());
                break;
            }
            case ESelectionPoint::BottomCenter: {
                //setBottomCenter(physValParentSelPt.y());
                break;
            }
            case ESelectionPoint::LeftCenter: {
                //setLeftCenter(physValParentSelPt.x());
                break;
            }
            case ESelectionPoint::Center: {
                setCenter(physValParentSelPt);
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
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Overrides the pure virtual method of base class CGraphObj.
*/
void CGraphObjGroup::updateTransformedCoorsOnParentChanged(
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

        setRectOrig(m_rectScaled);
        CPhysValRect physValRect = getPhysValRectOrig(m_rectOrig);
        setPhysValRectOrig(physValRect);
        physValRect = getPhysValRectScaled(m_physValRectOrig);
        setPhysValRectScaled(physValRect);
        // The relative rotation angle on the screen should remain the same.
        if (i_pGraphObjGroupPrev != nullptr) {
            #pragma message(__TODO__"PhysVal: round2Resolution is different from round2Accuracy.")
            #pragma message(__TODO__"PhysVal: Keep both resolution and accuracy in CPhysVal.")
            #pragma message(__TODO__"PhysVal: Values with resolution may also have an accuracy.")
            #pragma message(__TODO__"PhysVal: Resolutions should not be added, but accuracies should be added.")
            CPhysValRes physValRes = m_physValRotationAngle.getRes();
            CPhysVal physValRotationAngleParentGroup = i_pGraphObjGroupPrev->rotationAngle();
            if (i_pGraphObjGroupNew == nullptr) {
                m_physValRotationAngle += i_pGraphObjGroupPrev->mapRotationAngleToScene(physValRotationAngleParentGroup);
            }
            else {
                m_physValRotationAngle += i_pGraphObjGroupPrev->mapRotationAngleTo(physValRotationAngleParentGroup, i_pGraphObjGroupNew);
            }
            // Keep the resolution.
            m_physValRotationAngle.setRes(physValRes);
        }
        physValRect.setAngle(m_physValRotationAngle);
        setPhysValRectScaledAndRotated(physValRect);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
    emit_geometryOnSceneChanged(true);
}

//------------------------------------------------------------------------------
/*! @brief Overrides the pure virtual method of base class CGraphObj.
*/
void CGraphObjGroup::updateTransformedCoorsOnParentGeometryChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateTransformedCoorsOnParentGeometryChanged",
        /* strAddInfo   */ "" );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    // ItemChange will be called but should not emit the geometryOnSceneChanged signal.
    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

        setRectOrig(m_rectScaled);
        CPhysValRect physValRect = getPhysValRectOrig(m_rectOrig);
        setPhysValRectOrig(physValRect);
        physValRect = getPhysValRectScaled(m_physValRectOrig);
        setPhysValRectScaled(physValRect);
        physValRect.setAngle(m_physValRotationAngle);
        setPhysValRectScaledAndRotated(physValRect);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
    emit_geometryOnSceneChanged(true);
}

//------------------------------------------------------------------------------
/*! @brief Overrides the pure virtual method of base class CGraphObj.
*/
void CGraphObjGroup::updateTransformedCoorsOnItemPositionChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateTransformedCoorsOnItemPositionChanged",
        /* strAddInfo   */ "" );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    // ItemChange will be called but should not emit the geometryOnSceneChanged signal.
    {   CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryOnSceneChangedSignalBlockedCounter);

        CPhysValRect physValRect = getPhysValRectOrig(m_rectOrig);
        setPhysValRectOrig(physValRect);
        physValRect = getPhysValRectScaled(m_physValRectOrig);
        setPhysValRectScaled(physValRect);
        physValRect.setAngle(m_physValRotationAngle);
        setPhysValRectScaledAndRotated(physValRect);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
    emit_geometryOnSceneChanged();
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::updateDivLinesMetrics(
    const QSizeF& i_size_px, const QSizeF& i_size_metric)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Size/px {" + qSize2Str(i_size_px) + "}, Size/metric {" + qSize2Str(i_size_metric) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "updateDivLinesMetrics",
        /* strAddInfo   */ strMthInArgs );

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
            /* fScaleMaxVal */ i_size_px.width() - 1.0,
            /* fScaleResVal */ drawingSize.imageCoorsResolution(Units.Length.px).getVal());
        // The Y scale direction is from top to bottom.
        m_divLinesMetricsY.setUseWorldCoordinateTransformation(false);
        m_divLinesMetricsY.setScale(
            /* fScaleMinVal */ 0.0,
            /* fScaleMaxVal */ i_size_px.height() - 1.0,
            /* fScaleResVal */ drawingSize.imageCoorsResolution(Units.Length.px).getVal());
        //m_divLinesMetricsY.setYScaleAxisOrientation(EYScaleAxisOrientation::TopDown);
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
            /* fScaleMaxVal */ i_size_metric.width(),
            /* fScaleResVal */ drawingSize.imageCoorsResolution(drawingSize.unit()).getVal(),
            /* fMin_px      */ 0,
            /* fMax_px      */ i_size_px.width());
        m_divLinesMetricsY.setUseWorldCoordinateTransformation(true);
        if (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
            m_divLinesMetricsY.setScale(
                /* fScaleMinVal */ 0.0,
                /* fScaleMaxVal */ fabs(i_size_metric.height()),
                /* fScaleResVal */ drawingSize.imageCoorsResolution(drawingSize.unit()).getVal(),
                /* fMin_px      */ 0,
                /* fMax_px      */ i_size_px.height());
        }
        else {
            m_divLinesMetricsY.setScale(
                /* fScaleMinVal */ 0.0,
                /* fScaleMaxVal */ fabs(i_size_metric.height()),
                /* fScaleResVal */ drawingSize.imageCoorsResolution(drawingSize.unit()).getVal(),
                /* fMin_px      */ i_size_px.height(),
                /* fMax_px      */ 0);
        }
        //m_divLinesMetricsY.setYScaleAxisOrientation(drawingSize.yScaleAxisOrientation());
    }
    m_divLinesMetricsX.update();
    m_divLinesMetricsY.update();
}

//------------------------------------------------------------------------------
/*! @brief Calculates the scaled, not rotated rectangle in pixels in item coordinates
           relative to the origin (center point) of the orignal bounding rectangle.

    The relative distance of the center point of the scaled rectangle to the
    origin (center point) of the parent's bounding rectangle remains the same.
    The width and height are scaled to the scale factors of the parent group.

    @param [in] i_rectOrig
        Unscaled rectangle in the item's local coordinates system whose origin
        is the center point of the item.
    @return Scaled rectangle.
*/
QRectF CGraphObjGroup::getRectScaled(const QRectF& i_rectOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qRect2Str(i_rectOrig) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getRectScaled",
        /* strAddInfo   */ strMthInArgs );

    QPointF ptCenter(m_fParentGroupScaleX * i_rectOrig.center().x(), m_fParentGroupScaleY * i_rectOrig.center().y());
    QSizeF sizeF(m_fParentGroupScaleX * i_rectOrig.width(), m_fParentGroupScaleY * i_rectOrig.height());
    QRectF rectF;
    rectF.setSize(sizeF);
    rectF.moveCenter(ptCenter);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + qRect2Str(rectF) + "}");
    }
    return rectF;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the unscaled and not rotated physical rectangle in the
           current unit of the drawing scene relative to the top left or
           bottom left corner of the parent's bounding rectangle.

    @param [in] i_rectOrig
        Unscaled, not rotated rectangle in the item's local coordinates system
        whose origin is the center point of the item.
    @return Physical rectangle whose origin is either the top left or bottom
            left corner of the parent's bounding rectangle.
*/
CPhysValRect CGraphObjGroup::getPhysValRectOrig(const QRectF& i_rectOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + qRect2Str(i_rectOrig) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValRectOrig",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    CGraphObjGroup* pVThis = const_cast<CGraphObjGroup*>(this);
    double fRotationAngle_degree = m_physValRotationAngle.getVal(Units.Angle.Degree);
    if (fRotationAngle_degree != 0.0) {
        pVThis->QGraphicsItem_setRotation(0.0);
    }
    CPhysValPoint physValPointCenter(*m_pDrawingScene);
    CPhysValSize physValSize(*m_pDrawingScene);
    if (parentGroup() != nullptr) {
        QPointF ptCenter = pGraphicsItemThis->mapToParent(i_rectOrig.center());
        ptCenter = parentGroup()->mapToTopLeftOfBoundingRect(ptCenter);
        physValPointCenter = parentGroup()->convert(ptCenter);
        physValSize = parentGroup()->convert(i_rectOrig.size());
    }
    else {
        // Please note that "mapToScene" maps the local coordinates relative to the
        // top left corner of the item's bounding rectangle and there is no need to
        // call "mapToBoundingRectTopLeft" beforehand.
        QPointF ptCenter = pGraphicsItemThis->mapToScene(i_rectOrig.center());
        physValPointCenter = m_pDrawingScene->convert(ptCenter);
        physValSize = m_pDrawingScene->convert(i_rectOrig.size());
    }
    if (fRotationAngle_degree != 0.0) {
        pVThis->QGraphicsItem_setRotation(fRotationAngle_degree);
    }

    CPhysValRect physValRect(*m_pDrawingScene);
    physValRect.setSize(physValSize);
    physValRect.setCenter(physValPointCenter);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValRect.toString() + "} " + physValRect.unit().symbol());
    }
    return physValRect;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the scaled, not rotated physical rectangle in the current
           unit of the drawing scene relative to the top left or bottem left
           corner of the parent's bounding rectangle.

    The relative distance of the center point of the scaled rectangle to the
    top left or bottom left corner of the parent's bounding rectangle remains the same.
    The width and height are scaled to the scale factors of the parent group.

    @param [in] i_physValRectOrig
        Unscaled rectangle in the parent's, physical coordinates system whose origin
        is the top left or bottom right corner of the parent's bounding rectangle.
    @return Scaled rectangle.
*/
CPhysValRect CGraphObjGroup::getPhysValRectScaled(const CPhysValRect& i_physValRectOrig) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValRectOrig.toString() + "} " + i_physValRectOrig.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getPhysValRectScaled",
        /* strAddInfo   */ strMthInArgs );

    CPhysValPoint physValPointCenter(*m_pDrawingScene);
    physValPointCenter.setX(m_fParentGroupScaleX * i_physValRectOrig.center().x().getVal());
    physValPointCenter.setY(m_fParentGroupScaleY * i_physValRectOrig.center().y().getVal());
    CPhysValSize physValSize(*m_pDrawingScene);
    physValSize.setWidth(m_fParentGroupScaleX * i_physValRectOrig.width().getVal());
    physValSize.setHeight(m_fParentGroupScaleY * i_physValRectOrig.height().getVal());

    CPhysValRect physValRect(*m_pDrawingScene);
    physValRect.setSize(physValSize);
    physValRect.setCenter(physValPointCenter);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("{" + physValRect.toString() + "} " + physValRect.unit().symbol());
    }
    return physValRect;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the item position relative to the parent item or the drawing
           scene and the item coordinates of the rectangle in local coordinates.

    @param [in] i_physValRect
        Rectangle in parent coordinates, depending on the Y scale orientation
        relative to the top left or bottom left corner of parent item's bounding
        rectangle. If the item belongs to a parent group the passed rectangle
        must have been resized and the center must have been moved according to the
        parents scale factors.
    @param [out] o_rectF
        Rectangle coordinates in local coordinates.
    @param [out] o_physValAngle
        The rotatian angle of the passed rectangle.
*/
QPointF CGraphObjGroup::getItemPosAndLocalCoors(
    const CPhysValRect& i_physValRect, QRectF& o_rect, CPhysVal& o_physValAngle) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "getItemPosAndLocalCoors",
        /* strAddInfo   */ strMthInArgs );

    // First determine the position of the item in the parent's (scene or group) coordinate system.
    CPhysValRect physValRectTmp(i_physValRect);
    // For the graphics item the rotation angle of the rectangle is set explicitly applied to
    // the unrotated rectangle.
    o_physValAngle = physValRectTmp.angle();
    physValRectTmp.setAngle(0.0);
    if (parentGroup() != nullptr) {
        physValRectTmp = parentGroup()->convert(physValRectTmp, Units.Length.px);
    }
    else {
        physValRectTmp = m_pDrawingScene->convert(physValRectTmp, Units.Length.px);
    }
    o_rect = QRectF(physValRectTmp.topLeft().toQPointF(), physValRectTmp.size().toQSizeF());

    // Transform the parent coordinates into local coordinate system.
    // The origin is the center point of the rectangle.
    QPointF ptPos = o_rect.center(); // rect here still in parent or scene coordinates
    QPointF ptTL = o_rect.topLeft() - ptPos;
    QPointF ptBR = o_rect.bottomRight() - ptPos;
    o_rect = QRectF(ptTL, ptBR); // rect now in local coordinates

    if (parentGroup() != nullptr) {
        ptPos = parentGroup()->mapFromTopLeftOfBoundingRect(ptPos);
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        QString strMthOutArgs = "Rect {" + qRect2Str(o_rect) + "}, Angle: " + o_physValAngle.toString();
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn("{" + qPoint2Str(ptPos) + "}");
    }
    return ptPos;
}

//------------------------------------------------------------------------------
void CGraphObjGroup::paintGridLines(QPainter* i_pPainter)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "paintGridLines",
        /* strAddInfo   */ "" );

    i_pPainter->save();
    QRectF rct = getBoundingRect();
    if (m_divLinesMetricsX.getDivLinesCount(EDivLineLayer::Main) > 0) {
        QPen pen(m_gridSettings.linesColor());
        pen.setStyle(lineStyle2QtPenStyle(m_gridSettings.linesStyle().enumerator()));
        pen.setWidth(m_gridSettings.linesWidth());
        i_pPainter->setPen(pen);
        for (int idxLine = 0; idxLine < m_divLinesMetricsX.getDivLinesCount(EDivLineLayer::Main); ++idxLine ) {
            int x = m_divLinesMetricsX.getDivLineInPix(EDivLineLayer::Main, idxLine);
            x += rct.topLeft().x();
            i_pPainter->drawLine(x, rct.top(), x, rct.bottom());
        }
    }
    if (m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main) > 0) {
        QPen pen(m_gridSettings.linesColor());
        pen.setStyle(lineStyle2QtPenStyle(m_gridSettings.linesStyle().enumerator()));
        pen.setWidth(m_gridSettings.linesWidth());
        i_pPainter->setPen(pen);

        for (int idxLine = 0; idxLine < m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main); ++idxLine ) {
            int y = m_divLinesMetricsY.getDivLineInPix(EDivLineLayer::Main, idxLine);
            y += rct.topLeft().y();
            i_pPainter->drawLine(rct.left(), y, rct.right(), y);
        }
    }
    i_pPainter->restore();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::paintGridLabelsDivisionLines(QPainter* i_pPainter)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "paintGridLabelsDivisionLines",
        /* strAddInfo   */ "" );

    i_pPainter->save();
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QRectF rctBounding = getBoundingRect();
    QPen pen(m_gridSettings.linesColor());
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    i_pPainter->setPen(pen);
    if (m_divLinesMetricsX.getDivLinesCount(EDivLineLayer::Main) > 0) {
        for (int idxLine = 0; idxLine < m_divLinesMetricsX.getDivLinesCount(EDivLineLayer::Main); ++idxLine ) {
            int x = m_divLinesMetricsX.getDivLineInPix(EDivLineLayer::Main, idxLine);
            x += rctBounding.left();
            if (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::TopDown) {
                i_pPainter->drawLine(x, rctBounding.top()-5, x, rctBounding.top());
            }
            else {
                i_pPainter->drawLine(x, rctBounding.bottom()+5, x, rctBounding.bottom());
            }
        }
    }
    if (m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main) > 0) {
        for (int idxLine = 0; idxLine < m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main); ++idxLine ) {
            int y = m_divLinesMetricsY.getDivLineInPix(EDivLineLayer::Main, idxLine);
            y += rctBounding.top();
            i_pPainter->drawLine(rctBounding.left()-5, y, rctBounding.left(), y);
        }
    }
    i_pPainter->restore();
}

//------------------------------------------------------------------------------
void CGraphObjGroup::paintGridLabels(QPainter* i_pPainter)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "paintGridLabels",
        /* strAddInfo   */ "" );

    i_pPainter->save();
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QRectF rctBounding = getBoundingRect();
    QFont fnt = m_gridSettings.labelsFont();
    fnt.setBold(isTextStyleBold(m_gridSettings.labelsTextStyle()));
    fnt.setItalic(isTextStyleItalic(m_gridSettings.labelsTextStyle()));
    fnt.setUnderline(isTextEffectUnderline(m_gridSettings.labelsTextEffect()));
    fnt.setStrikeOut(isTextEffectStrikeout(m_gridSettings.labelsTextEffect()));
    fnt.setPointSize(textSize2SizeInPixels(m_gridSettings.labelsTextSize()));
    QPen pen(m_gridSettings.labelsTextColor());
    i_pPainter->setPen(pen);
    i_pPainter->setFont(fnt);
    if (m_divLinesMetricsX.getDivLinesCount(EDivLineLayer::Main) > 0) {
        for (int idxDivLine = 0; idxDivLine < m_divLinesMetricsX.getDivLinesCount(EDivLineLayer::Main); idxDivLine++) {
            if (m_divLinesMetricsX.isDivLineLabelVisible(EDivLineLayer::Main, idxDivLine)) {
                QString strDivLineLabel = m_divLinesMetricsX.getDivLineLabelText(EDivLineLayer::Main, idxDivLine);
                QRect rectDivLineLabel = m_divLinesMetricsX.getDivLineLabelBoundingRect(EDivLineLayer::Main, idxDivLine);
                rectDivLineLabel.setHeight(rectDivLineLabel.height()+2);
                int x = rctBounding.left() + rectDivLineLabel.left();
                int y = rctBounding.top() + rectDivLineLabel.top() - rectDivLineLabel.height() - 5;
                if (drawingSize.yScaleAxisOrientation() == EYScaleAxisOrientation::BottomUp) {
                    y = rctBounding.bottom() + rectDivLineLabel.top() + 5;
                }
                rectDivLineLabel.moveLeft(x);
                rectDivLineLabel.moveTop(y);
                i_pPainter->drawText(rectDivLineLabel, Qt::AlignVCenter|Qt::AlignHCenter, strDivLineLabel);
            }
        }
    }
    if (m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main) > 0) {
        for (int idxDivLine = 0; idxDivLine < m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main); idxDivLine++) {
            if (m_divLinesMetricsY.isDivLineLabelVisible(EDivLineLayer::Main, idxDivLine)) {
                QString strDivLineLabel = m_divLinesMetricsY.getDivLineLabelText(EDivLineLayer::Main, idxDivLine);
                QRect rectDivLineLabel = m_divLinesMetricsY.getDivLineLabelBoundingRect(EDivLineLayer::Main, idxDivLine);
                rectDivLineLabel.setHeight(rectDivLineLabel.height()+2);
                int x = rctBounding.left() - rectDivLineLabel.left() - rectDivLineLabel.width() - 5;
                int y = rectDivLineLabel.top() + rctBounding.top();
                rectDivLineLabel.moveLeft(x);
                rectDivLineLabel.moveTop(y);
                i_pPainter->drawText(rectDivLineLabel, Qt::AlignVCenter|Qt::AlignRight, strDivLineLabel);
            }
        }
    }
    i_pPainter->restore();
}

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Set the original, untransformed (not scaled, not rotated) rectangle
           coordinates in local coordinates relative to the origin of the
           groups bounding rectangle

    @param [in] i_rect
        Rectangle coordinates in local coordinates to be set.

    @return Previous original rectangle coordinates.
*/
QRectF CGraphObjGroup::setRectOrig(const QRectF& i_rect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + qRect2Str(i_rect) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRectOrig",
        /* strAddInfo   */ strMthInArgs );

    QRectF rectPrev = m_rectOrig;
    m_rectOrig = i_rect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + qRect2Str(rectPrev) + "}");
    }
    return rectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated rectangle coordinates in local
           coordinates relative to the origin of the groups bounding rectangle.

    Other graphics items, like Line, provide methods to set and retrieve the local
    coordinates (e.g. "setLine", "line"). The group item does not have such methods
    (e.g. "setRect", "rect"). To provide the bounding rectangle of the group in
    local coordinates this member is maintained.

    The scaled rectangle is returned by the "boundingRect" method of the graphics
    item to provide the bounding rectangle of the group in local coordinates.

    @param [in] i_rect
        Rectangle coordinates in local coordinates to be set.

    @return Previous rectangle coordinates.
*/
QRectF CGraphObjGroup::setRectScaled(const QRectF& i_rect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + qRect2Str(i_rect) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setRectScaled",
        /* strAddInfo   */ strMthInArgs );

    QRectF rectPrev = m_rectScaled;
    m_rectScaled = i_rect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + qRect2Str(rectPrev) + "}");
    }
    return rectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the original, untransformed (not scaled, not rotated) rectangle
           coordinates with unit in parent coordinates relative to the top left
           or bottom left corner of the parent.

    @param [in] i_physValRect
        Rectangle coordinates relative to the parent (or scene) to be set.

    @return Previous original rectangle coordinates.
*/
CPhysValRect CGraphObjGroup::setPhysValRectOrig(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValRectOrig",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectPrev = m_physValRectOrig;
    m_physValRectOrig = i_physValRect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValRectPrev.toString() + "} " + physValRectPrev.unit().symbol());
    }
    return physValRectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled but not rotated rectangle coordinates with unit in
           parent coordinates relative to the top left or bottom left corner of
           the parent.

    @param [in] i_physValRect
        Rectangle coordinates relative to the parent (or scene) to be set.

    @return Previous rectangle coordinates.
*/
CPhysValRect CGraphObjGroup::setPhysValRectScaled(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValRectScaled",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectPrev = m_physValRectScaled;
    m_physValRectScaled = i_physValRect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValRectPrev.toString() + "} " + physValRectPrev.unit().symbol());
    }
    return physValRectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaled and rotated rectangle coordinates with unit in parent
           coordinates relative to the top left or bottom left corner of the parent.

    @param [in] i_physValRect
        Rectangle coordinates relative to the parent (or scene) to be set.

    @return Previous rectangle coordinates.
*/
CPhysValRect CGraphObjGroup::setPhysValRectScaledAndRotated(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "setPhysValRectScaledAndRotated",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectPrev = m_physValRectScaledAndRotated;
    m_physValRectScaledAndRotated = i_physValRect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValRectPrev.toString() + "} " + physValRectPrev.unit().symbol());
    }
    return physValRectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Adds the given item and item's child items to this item group.

    The item and child items will be reparented to this group, but its position
    and transformation relative to the scene will stay intact.

    @param [in] i_pGraphicsItemChild
*/
void CGraphObjGroup::QGraphicsItemGroup_addToGroup(QGraphicsItem* i_pGraphicsItemChild)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItemChild);
        strMthInArgs = QString(pGraphObj == nullptr ? "null" : pGraphObj->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsItemGroup_addToGroup",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItemGroup::addToGroup(i_pGraphicsItemChild);
}

//------------------------------------------------------------------------------
/*! @brief Removes the specified item from this group.

    The item will be reparented to this group's parent item, or to 0 if this group
    has no parent. Its position and transformation relative to the scene will stay intact.

    @param [in] i_pGraphicsItemChild
*/
void CGraphObjGroup::QGraphicsItemGroup_removeFromGroup(QGraphicsItem* i_pGraphicsItemChild)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(i_pGraphicsItemChild);
        strMthInArgs = QString(pGraphObj == nullptr ? "null" : pGraphObj->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ path(),
        /* strMethod    */ "QGraphicsItemGroup_removeFromGroup",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItemGroup::removeFromGroup(i_pGraphicsItemChild);
}

//------------------------------------------------------------------------------
/*! @brief Internal method reimplementing the prepareGeometryChange method of
           graphics item to trace the method call.

    As the prepareGeometryChange method is a protected method of QGraphicsItem
    this method must be reimplemented by the derived classes.
*/
void CGraphObjGroup::QGraphicsItem_prepareGeometryChange()
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
//void CGraphObjGroup::applyGeometryChangeToChildrens()
////------------------------------------------------------------------------------
//{
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjItemChange,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strObjName   */ path(),
//        /* strMethod    */ "applyGeometryChangeToChildrens",
//        /* strAddInfo   */ "" );
//    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
//        tracePositionInfo(mthTracer);
//    }
//
//    // As long as there is no valid original rectangle childrens cannot be
//    // resized and the method does nothing.
//    if (!m_physValRectOrig.isValid()) {
//        return;
//    }
//
//    QRectF rectCurr = getBoundingRect();
//    CPhysValRect physValRect(m_physValRectOrig);
//    physValRect.setAngle(0.0);
//    if (parentGroup() != nullptr) {
//        physValRect = parentGroup()->convert(physValRect, Units.Length.px);
//    }
//    else {
//        physValRect = m_pDrawingScene->convert(physValRect, Units.Length.px);
//    }
//    QRectF rectOrig(physValRect.topLeft().toQPointF(), physValRect.size().toQSizeF());
//    double fScaleX = rectCurr.width() / rectOrig.width();
//    double fScaleY = rectCurr.height() / rectOrig.height();
//
//    QList<QGraphicsItem*> arpGraphicsItemsChilds = childItems();
//    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
//        mthTracer.trace("Childs [" + QString::number(arpGraphicsItemsChilds.size()) + "]");
//    }
//    for (QGraphicsItem* pGraphicsItemChild : arpGraphicsItemsChilds) {
//        CGraphObj* pGraphObjChild = dynamic_cast<CGraphObj*>(pGraphicsItemChild);
//        if (pGraphObjChild != nullptr) {
//            //pGraphObjChild->setGroupScale(fScaleX, fScaleY);
//
//            //QRectF rectChildCurr;
//            //if (pGraphObjChild->getAlignmentCount() == 0) {
//            //    //rectChildCurr = getScaledChildRect(pGraphObjChild);
//
//            //    QGraphicsItem* pGraphicsItemChild = dynamic_cast<QGraphicsItem*>(i_pGraphObjChild);
//
//            //    QRectF rectChildCurr;
//            //    //QRectF rectChildPrev = i_pGraphObjChild->getOriginalBoundingRectInParent();
//
//            //    //const CUnit& unitThis = m_physValRectCurr.unit();
//
//            //    //// Get coordinates of this group relative to the parent group if this group.
//            //    //QRectF rectThisOrig = m_physValRectOrig.toQRectF();
//            //    //QRectF rectThisCurr = m_physValRectCurr.toQRectF();
//            //    //double fScaleWidth = rectThisCurr.width() / rectThisOrig.width();
//            //    //double fScaleHeight = rectThisCurr.height() / rectThisOrig.height();
//
//            //    //// Map the coordinates of this group to the local coordinates of this group.
//            //    ////rectThisOrig = mapRectFromParent(rectThisOrig);
//            //    ////rectThisCurr = mapRectFromParent(rectThisCurr);
//
//            //    //// Get position of child relative to this parent group.
//            //    ////QPointF ptPosChildPrev = pGraphicsItemChild->pos();
//
//            //    //// Default without any alignments assigned is:
//            //    //// - position relative to center point of the group's bounding rectangle
//            //    //// - size relative to size of the group's bounding rectangle
//            //    //rectChildCurr.setLeft(fScaleWidth * rectChildPrev.left());
//            //    //rectChildCurr.setTop(fScaleHeight * rectChildPrev.top());
//            //    //rectChildCurr.setWidth(fScaleWidth * rectChildPrev.width());
//            //    //rectChildCurr.setHeight(fScaleHeight * rectChildPrev.height());
//
//            //    //// If the object has a fixed width or height they also have a
//            //    //// minimum and maximum width or height.
//            //    //// In addition the minimum and maximum width and height is equal.
//            //    //// So it is sufficient to check the minimum and maximum sizes.
//            //    //if (i_pGraphObjChild->hasMinimumWidth()) {
//            //    //    if (rectChildCurr.width() < i_pGraphObjChild->getMinimumWidth(unitThis).getVal()) {
//            //    //        rectChildCurr.setWidth(i_pGraphObjChild->getMinimumWidth(unitThis).getVal());
//            //    //    }
//            //    //}
//            //    //if (i_pGraphObjChild->hasMaximumWidth()) {
//            //    //    if (rectChildCurr.width() > i_pGraphObjChild->getMaximumWidth(unitThis).getVal()) {
//            //    //        rectChildCurr.setWidth(i_pGraphObjChild->getMaximumWidth(unitThis).getVal());
//            //    //    }
//            //    //}
//            //    //if (i_pGraphObjChild->hasMinimumHeight()) {
//            //    //    if (rectChildCurr.height() < i_pGraphObjChild->getMinimumHeight(unitThis).getVal()) {
//            //    //        rectChildCurr.setHeight(i_pGraphObjChild->getMinimumHeight(unitThis).getVal());
//            //    //    }
//            //    //}
//            //    //if (i_pGraphObjChild->hasMaximumHeight()) {
//            //    //    if (rectChildCurr.height() > i_pGraphObjChild->getMaximumHeight(unitThis).getVal()) {
//            //    //        rectChildCurr.setHeight(i_pGraphObjChild->getMaximumHeight(unitThis).getVal());
//            //    //    }
//            //    //}
//            //}
//            //if (rectChildCurr.width() < 0.0) {
//            //    double fXL = rectChildCurr.right();
//            //    double fXR = rectChildCurr.left();
//            //    rectChildCurr.setLeft(fXL);
//            //    rectChildCurr.setRight(fXR);
//            //}
//            //if (rectChildCurr.height() < 0.0) {
//            //    double fYT = rectChildCurr.bottom();
//            //    double fYB = rectChildCurr.top();
//            //    rectChildCurr.setTop(fYT);
//            //    rectChildCurr.setBottom(fYB);
//            //}
//            //pGraphObjChild->setCurrentBoundingRectInParent(rectChildCurr);
//        }
//    }
//
//    //QGraphicsItem_prepareGeometryChange();
//
//} // applyGeometryChangeToChildrens

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
//        /* strObjName   */ path(),
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

/*==============================================================================
protected: // auxiliary instance methods (method tracing)
==============================================================================*/

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
        /* strObjName   */ path(),
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
    if (m_iTraceBlockedCounter > 0 || m_iTracePositionInfoBlockedCounter > 0 || m_iTraceThisPositionInfoInfoBlockedCounter > 0) {
        return;
    }
    if (i_mthTracer.isRuntimeInfoActive(i_detailLevel)) {
        const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);
        if (pGraphicsItem != nullptr) {
            QString strRuntimeInfo;
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "PhysValRect Orig {" + m_physValRectOrig.toString() + "} " + m_physValRectOrig.unit().symbol()
                + ", Scaled {" + m_physValRectScaled.toString() + "} " + m_physValRectScaled.unit().symbol();
                + ", ScaledRotated {" + m_physValRectScaledAndRotated.toString() + "} " + m_physValRectScaledAndRotated.unit().symbol();
            i_mthTracer.trace(strRuntimeInfo);
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "ItemRect Orig {" + qRect2Str(m_rectOrig) + "}, Scaled {" + qRect2Str(m_rectScaled) + "}";
            i_mthTracer.trace(strRuntimeInfo);
            CGraphObj::traceThisPositionInfo(i_mthTracer, i_mthDir, i_detailLevel);
        }
    }
}
