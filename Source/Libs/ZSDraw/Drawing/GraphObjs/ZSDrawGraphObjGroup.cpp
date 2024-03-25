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
public: // type definitions and constants
==============================================================================*/

const QString CGraphObjGroup::c_strGeometryLabelNameTopLeft = "TL";
const QString CGraphObjGroup::c_strGeometryLabelNameTopRight = "TR";
const QString CGraphObjGroup::c_strGeometryLabelNameBottomRight = "BR";
const QString CGraphObjGroup::c_strGeometryLabelNameBottomLeft = "BL";
const QString CGraphObjGroup::c_strGeometryLabelNameCenter = "Center";
const QString CGraphObjGroup::c_strGeometryLabelNameWidth = "Width";
const QString CGraphObjGroup::c_strGeometryLabelNameHeight = "Height";
const QString CGraphObjGroup::c_strGeometryLabelNameAngle = "Angle";

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
    m_rectOrig(),
    m_physValRectOrig(*m_pDrawingScene)
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
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryWidth, ESelectionPoint::LeftCenter, ESelectionPoint::RightCenter);
        }
        else if (strLabelName == c_strGeometryLabelNameHeight) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryHeight, ESelectionPoint::TopCenter, ESelectionPoint::BottomCenter);
        }
        else if (strLabelName == c_strGeometryLabelNameAngle) {
            addGeometryLabel(strLabelName, EGraphObjTypeLabelGeometryAngle, ESelectionPoint::Center, ESelectionPoint::RightCenter);
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
    m_divLinesMetricsX(i_strType + i_strObjName, EScaleAxis::X),
    m_divLinesMetricsY(i_strType + i_strObjName, EScaleAxis::Y),
    m_gridSettings(i_strType + i_strObjName),
    m_rectOrig(),
    m_physValRectOrig(*m_pDrawingScene)
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
    //m_physValRectOrig;

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
        rctBoundingThisPrev = pGraphicsItemThis->mapToParent(rctBoundingThisPrev).boundingRect();

        // The parent of the child to be added is either the drawing scene or another group.
        // The bounding rectangle of the new child item need to be added mapped into the parent
        // coordinates of this group. The parent of this group may either be the scene or a group.
        QRectF rctBoundingChild = i_pGraphObj->getBoundingRect();
        rctBoundingChild = pGraphicsItemChild->mapToScene(rctBoundingChild).boundingRect();
        QGraphicsItem* pGraphicsItemParentThis = parentItem();
        CGraphObjGroup* pGraphObjGroupParentThis = dynamic_cast<CGraphObjGroup*>(pGraphicsItemParentThis);
        if (pGraphicsItemParentThis != nullptr) {
            rctBoundingChild = pGraphicsItemParentThis->mapFromScene(rctBoundingChild).boundingRect();
        }

        // Resulting, new bounding rectangle of this group in parent coordinates of this group.
        QRectF rctBoundingThisNew = rctBoundingThisPrev | rctBoundingChild;

        if (rctBoundingThisNew != rctBoundingThisPrev) {
            // Convert (map) the new bounding rectangle of this group into the coordinate system of
            // this groups parent in the unit of the drawing scene.
            CPhysValRect physValRectNew;
            if (pGraphObjGroupParentThis != nullptr) {
                physValRectNew = pGraphObjGroupParentThis->convert(rctBoundingThisNew);
            }
            else {
                physValRectNew = m_pDrawingScene->convert(rctBoundingThisNew);
            }
            setRect(physValRectNew);

            // If the group's bounding rectangle has been extended, the groups physical origin point
            // (top left or bottom left corner) may have been changed and the childs must update their
            // original shape point coordinates. This is also necessary if the groups position and
            // also the childs position (in graphics item pixel coordinates) have not been changed.
            if (count() > 0) {
                QPointF ptPosThisNew = pGraphicsItemThis->pos();
                QPointF ptMove = ptPosThisNew - ptPosThisPrev;
                QVector<CGraphObj*> arpGraphObjChilds = childs();
                for (CGraphObj* pGraphObjChildExisting : arpGraphObjChilds) {
                    QGraphicsItem* pGraphicsItemChildExisting = dynamic_cast<QGraphicsItem*>(pGraphObjChildExisting);
                    QPointF ptPosChildPrev = pGraphicsItemChildExisting->pos();
                    QPointF ptPosChildNew = ptPosChildPrev - ptMove;
                    pGraphicsItemChildExisting->setPos(ptPosChildNew);
                    // As on calling "setPos" the position may not have been changed, force the child
                    // to update it's original shape points in physical coordinates relative to either
                    // the top left or bottom left corner of the parents bounding rectangle:
                    pGraphObjChildExisting->updateOriginalPhysValCoors();
                }
            }
        }
        // The newly added child will be positioned by the graphics system.
        // But the positionChange event in the graphics system does not take the current
        // size of the group into account.
        // When mapping local coordinates into parent coordinates and vice versa the parent
        // group must have been set. So the new parent graphical object is set before adding
        // the item to the GraphicsItemGroup.
        m_pDrawingScene->getGraphObjsIdxTree()->move(i_pGraphObj, this);
        QGraphicsItemGroup::addToGroup(pGraphicsItemChild);
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
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
        QRectF rctBoundingThis = getBoundingRect(); // Used if the group will have no more childs anymore
        QGraphicsItemGroup::removeFromGroup(pGraphicsItemChild);
        m_pDrawingScene->getGraphObjsIdxTree()->move(i_pGraphObj, parentBranch());
        // Keep the current bounding rect.
        // If childs still belong to the group ..
        if (count() > 0) {
            QVector<CGraphObj*> arpGraphObjChilds = childs();
            rctBoundingThis = ZS::Draw::boundingRect(arpGraphObjChilds);
        }
        CPhysValRect physValRect(rctBoundingThis, m_pDrawingScene->drawingSize().imageCoorsResolutionInPx(), Units.Length.px);
        physValRect = convert(physValRect);
        setRect(physValRect);
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
        /* strObjName   */ m_strName,
        /* strMethod    */ "resizeToContent",
        /* strAddInfo   */ "" );
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    // Remember current position of this group in parent coordinates.
    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    QPointF ptPosThisPrev = pGraphicsItemThis->pos();

    //// Bounding rectangle of this group in local coordinates (relative to this center).
    //QRectF rctBoundingThisPrev = getBoundingRect();
    //// Map the bounding rectangle of this group into the parent coordinates of this group.
    //rctBoundingThisPrev = pGraphicsItemThis->mapToParent(rctBoundingThisPrev).boundingRect();

    // Resulting, new bounding rectangle of this group in parent coordinates of this group.
    QRectF rctBoundingThisNew;
    if (count() > 0) {
        QVector<CGraphObj*> arpGraphObjChilds = childs();
        for (CGraphObj* pGraphObjChild : arpGraphObjChilds) {
            QRectF rctBoundingChild = pGraphObjChild->getBoundingRect();
            QGraphicsItem* pGraphicsItemChild = dynamic_cast<QGraphicsItem*>(pGraphObjChild);
            rctBoundingChild = pGraphicsItemChild->mapToParent(rctBoundingChild).boundingRect();
            rctBoundingThisNew |= rctBoundingChild;
        }
    }
    rctBoundingThisNew = mapToTopLeftOfBoundingRect(rctBoundingThisNew);

    //QGraphicsItem* pGraphicsItemParentThis = parentItem();
    //CGraphObjGroup* pGraphObjGroupParentThis = dynamic_cast<CGraphObjGroup*>(pGraphicsItemParentThis);

    // Convert (map) the new bounding rectangle of this group into the coordinate system of
    // this groups parent in the unit of the drawing scene.
    CPhysValRect physValRectNew;
    if (parentGroup() != nullptr) {
        physValRectNew = parentGroup()->convert(rctBoundingThisNew);
    }
    else {
        //rctBoundingThisNew = pGraphicsItemThis->mapToScene(rctBoundingThisNew).boundingRect();
        physValRectNew = m_pDrawingScene->convert(rctBoundingThisNew);
    }
    setRect(physValRectNew);

    if (count() > 0) {
        QPointF ptPosThisNew = pGraphicsItemThis->pos();
        QPointF ptMove = ptPosThisNew - ptPosThisPrev;
        QVector<CGraphObj*> arpGraphObjChilds = childs();
        for (CGraphObj* pGraphObjChildExisting : arpGraphObjChilds) {
            QGraphicsItem* pGraphicsItemChildExisting = dynamic_cast<QGraphicsItem*>(pGraphObjChildExisting);
            QPointF ptPosChildPrev = pGraphicsItemChildExisting->pos();
            QPointF ptPosChildNew = ptPosChildPrev - ptMove;
            pGraphicsItemChildExisting->setPos(ptPosChildNew);
            pGraphObjChildExisting->updateOriginalPhysValCoors();
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
    tracePositionInfo(mthTracer, EMethodDir::Enter);

    // Depending on the Y scale orientation of the drawing scene the rectangle coordinates
    // have been passed either relative to the top left or bottom left corner of the
    // parent item's bounding rectangle.
    // The coordinates need to be transformed into the local coordinate system of the graphical
    // object whose origin point is the center of the objects bounding rectangle.
    //CPhysValRect physValRect = i_physValRect;
    //if (Units.Length.isMetricUnit(physValRect.unit())) {
    //    QGraphicsItem* pGraphicsItemParent = parentItem();
    //    CGraphObjGroup* pGraphObjGroupParent = dynamic_cast<CGraphObjGroup*>(pGraphicsItemParent);
    //    // If the item belongs to a group ...
    //    if (pGraphObjGroupParent != nullptr) {
    //        physValRect = pGraphObjGroupParent->convert(physValRect, Units.Length.px);
    //    }
    //    // If the item is not a child of a group ...
    //    else {
    //        physValRect = m_pDrawingScene->convert(physValRect, Units.Length.px);
    //    }
    //}
    //QRectF rectF = physValRect.toQRectF();

    // First determine the position of the item in the parent's (scene or group) coordinate system.
    QRectF rectF;
    if (parentGroup() != nullptr) {
        rectF = parentGroup()->convert(i_physValRect, Units.Length.px).toQRectF();
    }
    else {
        rectF = m_pDrawingScene->convert(i_physValRect, Units.Length.px).toQRectF();
    }

    // Transform the parent coordinates into local coordinate system.
    // The origin is the center point of the rectangle.
    QPointF ptPos = rectF.center();
    QPointF ptTL = rectF.topLeft() - ptPos;
    QPointF ptBR = rectF.bottomRight() - ptPos;
    rectF = QRectF(ptTL, ptBR);

    if (parentGroup() != nullptr) {
        ptPos = parentGroup()->mapFromTopLeftOfBoundingRect(ptPos);
    }

    // If the coordinates MUST be updated
    // (initially set, changed or after the drawing size has been changed)
    if (m_rectOrig.isNull() || m_rectOrig != rectF || m_bForceConversionToSceneCoors)
    {
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
                /* fScaleMaxVal */ rectF.width() - 1.0,
                /* fScaleResVal */ drawingSize.imageCoorsResolution(Units.Length.px).getVal());
            // The Y scale direction is from top to bottom.
            m_divLinesMetricsY.setUseWorldCoordinateTransformation(false);
            m_divLinesMetricsY.setScale(
                /* fScaleMinVal */ 0.0,
                /* fScaleMaxVal */ rectF.height() - 1.0,
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
                /* fScaleMaxVal */ i_physValRect.width().getVal(),
                /* fScaleResVal */ drawingSize.imageCoorsResolution(drawingSize.unit()).getVal(),
                /* fMin_px      */ 0,
                /* fMax_px      */ rectF.width());
            m_divLinesMetricsY.setUseWorldCoordinateTransformation(true);
            m_divLinesMetricsY.setScale(
                /* fScaleMinVal */ 0.0,
                /* fScaleMaxVal */ fabs(i_physValRect.height().getVal()),
                /* fScaleResVal */ drawingSize.imageCoorsResolution(drawingSize.unit()).getVal(),
                /* fMin_px      */ 0,
                /* fMax_px      */ rectF.height());
            m_divLinesMetricsY.setYScaleAxisOrientation(drawingSize.yScaleAxisOrientation());
        }
        m_divLinesMetricsX.update();
        m_divLinesMetricsY.update();

        // Prepare the item for a geometry change. This function must be called before
        // changing the bounding rect of an item to keep QGraphicsScene's index up to date.
        QGraphicsItemGroup::prepareGeometryChange();

        {   CRefCountGuard refCountGuardUpdateOriginalCoors(&m_iItemChangeUpdateOriginalCoorsBlockedCounter);
            CRefCountGuard refCountGuardGeometryChangedSignal(&m_iGeometryChangedSignalBlockedCounter);

            // Store the rectangle coordinates as the original coordinates.
            setRectOrig(i_physValRect);
            setRectOrig(rectF);

            // Move the object to the parent position.
            // This has to be done after resizing the line which updates the local coordinates
            // of the line with origin (0/0) at the lines center point.
            // "setPos" will trigger an itemChange call which will update the position of the
            // selection points and labels. To position the selection points and labels correctly
            // the local coordinate system must be up-to-date.
            // Also note that itemChange must not overwrite the current line value (refCountGuard).
            QGraphicsItem_setPos(ptPos);
        }
        emit_geometryChanged();
    }
    tracePositionInfo(mthTracer, EMethodDir::Leave);
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
    CPhysValRect physValRect = m_physValRectOrig;
    #pragma message(__TODO__"Take transformation into account")
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
/*! @brief Converts the given point in pixels into the current unit of the drawing scene.

    @param [in] i_pt

    @return Converted value.
*/
CPhysValPoint CGraphObjGroup::convert(const QPointF& i_pt) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(CPhysValPoint(i_pt, drawingSize.imageCoorsResolutionInPx(), Units.Length.px), drawingSize.unit());
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
    return convert(CPhysValPoint(i_pt, drawingSize.imageCoorsResolutionInPx(), Units.Length.px), i_unitDst);
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
            physValPoint = CPhysValPoint(physValX, physValY);
        }
        else if ((i_physValPoint.unit() == Units.Length.px) && Units.Length.isMetricUnit(i_unitDst)) {
            QPointF pt = i_physValPoint.toQPointF();
            double fx = m_divLinesMetricsX.getVal(pt.x(), false);
            double fy = m_divLinesMetricsY.getVal(pt.y(), false);
            CPhysVal physValX(fx, drawingSize.unit(), drawingSize.imageCoorsResolution());
            CPhysVal physValY(fy, drawingSize.unit(), drawingSize.imageCoorsResolution());
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
/*! @brief Converts the given size in pixels into the current unit of the drawing scene.

    @param [in] i_size

    @return Converted value.
*/
CPhysValSize CGraphObjGroup::convert(const QSizeF& i_size) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(CPhysValSize(i_size, drawingSize.imageCoorsResolutionInPx(), Units.Length.px), drawingSize.unit());
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
    return convert(CPhysValSize(i_size, drawingSize.imageCoorsResolutionInPx(), Units.Length.px), i_unitDst);
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
            physValSize = CPhysValSize(physValWidth, physValHeight);
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
            physValSize = CPhysValSize(physValWidth, physValHeight);
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
            physValSize = CPhysValSize(physValWidth, physValHeight);
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
    return convert(CPhysValLine(i_line, drawingSize.imageCoorsResolutionInPx(), Units.Length.px), drawingSize.unit());
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
    return convert(CPhysValLine(i_line, drawingSize.imageCoorsResolutionInPx(), Units.Length.px), i_unitDst);
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
/*! @brief Converts the given rectangle in pixels into the current unit of the drawing scene.

    @param [in] i_rect

    @return Converted value.
*/
CPhysValRect CGraphObjGroup::convert(const QRectF& i_rect) const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return convert(CPhysValRect(i_rect, drawingSize.imageCoorsResolutionInPx(), Units.Length.px), drawingSize.unit());
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
    return convert(CPhysValRect(i_rect, drawingSize.imageCoorsResolutionInPx(), Units.Length.px), i_unitDst);
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
CPhysValPoint CGraphObjGroup::mapToScene(const CPhysValPoint& i_physValPoint) const
//------------------------------------------------------------------------------
{
    return mapToScene(i_physValPoint, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical point, which is, depending on the Y-Axis-Scale
           Orientation, relative to either the top left or bottom left corner of
           the parent's bounding rectangle to the physical value in the drawing scene.

    @param [in] i_physValPoint
        Point to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Point in scene coordinates in the desired unit.
*/
CPhysValPoint CGraphObjGroup::mapToScene(const CPhysValPoint& i_physValPoint, const ZS::PhysVal::CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Pt {" + i_physValPoint.toString() + "} " + i_physValPoint.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
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
CPhysValLine CGraphObjGroup::mapToScene(const CPhysValLine& i_physValLine) const
//------------------------------------------------------------------------------
{
    return mapToScene(i_physValLine, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical line, which is, depending on the Y-Axis-Scale
           Orientation, relative to either the top left or bottom left corner of
           the item's bounding rectangle to the physical value in the drawing scene.

    @param [in] i_physValLine
        Line to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Line in scene coordinates in the desired unit.
*/
CPhysValLine CGraphObjGroup::mapToScene(const CPhysValLine& i_physValLine, const ZS::PhysVal::CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Line {" + i_physValLine.toString() + "} " + i_physValLine.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
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
CPhysValRect CGraphObjGroup::mapToScene(const CPhysValRect& i_physValRect) const
//------------------------------------------------------------------------------
{
    return mapToScene(i_physValRect, m_pDrawingScene->drawingSize().unit());
}

//------------------------------------------------------------------------------
/*! @brief Maps the given physical point, which is, depending on the Y-Axis-Scale
           Orientation, relative to either the top left or bottom left corner of
           the item's bounding rectangle to the physical value in the drawing scene.

    @param [in] i_physValPoint
        Point to be mapped.
    @param [in] i_unitDst
        Unit in which the coordinate should be returned.

    @return Point in scene coordinates in the desired unit.
*/
CPhysValRect CGraphObjGroup::mapToScene(const CPhysValRect& i_physValRect, const ZS::PhysVal::CUnit& i_unitDst) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjCoordinateConversions, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Rect {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol() + ", UnitDst: " + i_unitDst.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCoordinateConversions,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mapToScene",
        /* strAddInfo   */ strMthInArgs );

    const QGraphicsItem* pGraphicsItemThis = dynamic_cast<const QGraphicsItem*>(this);
    QRectF rectF = convert(i_physValRect, Units.Length.px).toQRectF();
    rectF = pGraphicsItemThis->mapToScene(rectF).boundingRect();
    rectF = mapFromTopLeftOfBoundingRect(rectF);
    CPhysValRect physValRect = m_pDrawingScene->convert(rectF, i_unitDst);
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Rect {" + physValRect.toString() + "} " + physValRect.unit().symbol());
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

    // Local coordinates, scaled and rotated.
    QPointF ptTL = m_transform.map(m_rectOrig.topLeft());
    QPointF ptBR = m_transform.map(m_rectOrig.bottomRight());
    QRectF rctBounding(ptTL, ptBR);
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
*/
void CGraphObjGroup::updateOriginalPhysValCoors()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateOriginalPhysValCoors",
        /* strAddInfo   */ "" );

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    QRectF rectF = getBoundingRect();
    if (parentGroup() != nullptr) {
        QPointF ptTL = pGraphicsItemThis->mapToParent(rectF.topLeft());
        QPointF ptBR = pGraphicsItemThis->mapToParent(rectF.bottomRight());
        ptTL = parentGroup()->mapToTopLeftOfBoundingRect(ptTL);
        ptBR = parentGroup()->mapToTopLeftOfBoundingRect(ptBR);
        CPhysValPoint physValPointTL = parentGroup()->convert(ptTL);
        CPhysValPoint physValPointBR = parentGroup()->convert(ptBR);
        setRectOrig(CPhysValRect(physValPointTL, physValPointBR));
    }
    else {
        // Please note that "mapToScene" maps the local coordinates relative to the
        // top left corner of the item's bounding rectangle and there is no need to
        // call "mapToBoundingRectTopLeft" beforehand.
        QPointF ptTL = pGraphicsItemThis->mapToScene(rectF.topLeft());
        QPointF ptBR = pGraphicsItemThis->mapToScene(rectF.bottomRight());
        CPhysValPoint physValPointTL = m_pDrawingScene->convert(ptTL);
        CPhysValPoint physValPointBR = m_pDrawingScene->convert(ptBR);
        setRectOrig(CPhysValRect(physValPointTL, physValPointBR));
    }
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
    //i_pPainter->setRenderHint(QPainter::Antialiasing);

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
            //updateInternalScenePos();
            bGeometryChanged = true;
            bTreeEntryChanged = true;
        }
    }
    else if (i_change == ItemParentHasChanged || i_change == ItemPositionHasChanged) {
        //traceGraphicsItemStates(mthTracer);
        //traceGraphObjStates(mthTracer);
        tracePositionInfo(mthTracer, EMethodDir::Enter);

        // The object may be moved or transformed by several methods.
        // "itemChange" is a central point to update the coordinates upon those changes.
        if (m_iItemChangeUpdateOriginalCoorsBlockedCounter == 0) {
            // Update the object shape point in parent coordinates kept in the unit of the drawing scene.
            // For groups the original coordinates are only updated when adding the group to
            // or removing the group from another group.
            if (i_change == ItemPositionHasChanged) {
                if (parentGroup() == nullptr) {
                    // Please note that "mapToScene" maps the local coordinates relative to the
                    // top left corner of the item's bounding rectangle and there is no need to
                    // call "mapToBoundingRectTopLeft" beforehand.
                    QRectF rectF = getBoundingRect();
                    QPointF ptTL = pGraphicsItemThis->mapToScene(rectF.topLeft());
                    QPointF ptBR = pGraphicsItemThis->mapToScene(rectF.bottomRight());
                    CPhysValPoint physValPointTL = m_pDrawingScene->convert(ptTL);
                    CPhysValPoint physValPointBR = m_pDrawingScene->convert(ptBR);
                    setRectOrig(CPhysValRect(physValPointTL, physValPointBR));
                }
            }
            else if (i_change == ItemParentHasChanged) {
                if (parentGroup() != nullptr) {
                    QRectF rectF = getBoundingRect();
                    QPointF ptTL = pGraphicsItemThis->mapToParent(rectF.topLeft());
                    QPointF ptBR = pGraphicsItemThis->mapToParent(rectF.bottomRight());
                    ptTL = parentGroup()->mapToTopLeftOfBoundingRect(ptTL);
                    ptBR = parentGroup()->mapToTopLeftOfBoundingRect(ptBR);
                    CPhysValPoint physValPointTL = parentGroup()->convert(ptTL);
                    CPhysValPoint physValPointBR = parentGroup()->convert(ptBR);
                    setRectOrig(CPhysValRect(physValPointTL, physValPointBR));
                }
            }
            //applyGeometryChangeToChildrens();
        }
        tracePositionInfo(mthTracer, EMethodDir::Leave);
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

    QGraphicsItem* pGraphicsItemThis = dynamic_cast<QGraphicsItem*>(this);
    CGraphObjSelectionPoint* pGraphObjSelPt = dynamic_cast<CGraphObjSelectionPoint*>(i_pSelectionPoint);
    QGraphicsItem* pGraphicsItemSelPt = dynamic_cast<QGraphicsItem*>(pGraphObjSelPt);
    QPointF ptScenePosSelPt = pGraphicsItemSelPt->scenePos();
    QPointF ptPosSelPt = mapFromScene(ptScenePosSelPt);
    QPointF ptParentPosSelPt = pGraphicsItemThis->mapToParent(ptPosSelPt);
    CPhysValPoint physValParentSelPt;
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
void CGraphObjGroup::paintGridLines(QPainter* i_pPainter)
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
        /* strMethod    */ "paintGridLabelsDivisionLines",
        /* strAddInfo   */ "" );

    i_pPainter->save();
    QRectF rctBounding = getBoundingRect();
    QPen pen(m_gridSettings.linesColor());
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    i_pPainter->setPen(pen);
    if (m_divLinesMetricsX.getDivLinesCount(EDivLineLayer::Main) > 0) {
        for (int idxLine = 0; idxLine < m_divLinesMetricsX.getDivLinesCount(EDivLineLayer::Main); ++idxLine ) {
            int x = m_divLinesMetricsX.getDivLineInPix(EDivLineLayer::Main, idxLine);
            x += rctBounding.topLeft().x();
            i_pPainter->drawLine(x, rctBounding.top()-10, x, rctBounding.top());
            i_pPainter->drawLine(x, rctBounding.bottom()+10, x, rctBounding.bottom());
        }
    }
    if (m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main) > 0) {
        for (int idxLine = 0; idxLine < m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main); ++idxLine ) {
            int y = m_divLinesMetricsY.getDivLineInPix(EDivLineLayer::Main, idxLine);
            y += rctBounding.topLeft().y();
            i_pPainter->drawLine(rctBounding.left()-10, y, rctBounding.left(), y);
            i_pPainter->drawLine(rctBounding.right()+10, y, rctBounding.right(), y);
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
        /* strMethod    */ "paintGridLabels",
        /* strAddInfo   */ "" );

    i_pPainter->save();
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
                rectDivLineLabel.setBottom(rctBounding.bottom());
                rectDivLineLabel.setTop(rctBounding.bottom() - rectDivLineLabel.height());
                i_pPainter->drawText(rectDivLineLabel, Qt::AlignVCenter|Qt::AlignHCenter, strDivLineLabel);
            }
        }
    }
    if (m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main) > 0) {
        for (int idxDivLine = 0; idxDivLine < m_divLinesMetricsY.getDivLinesCount(EDivLineLayer::Main); idxDivLine++) {
            if (m_divLinesMetricsY.isDivLineLabelVisible(EDivLineLayer::Main, idxDivLine)) {
                QString strDivLineLabel = m_divLinesMetricsY.getDivLineLabelText(EDivLineLayer::Main, idxDivLine);
                QRect rectDivLineLabel = m_divLinesMetricsY.getDivLineLabelBoundingRect(EDivLineLayer::Main, idxDivLine);
                rectDivLineLabel.setLeft(rctBounding.left() - rectDivLineLabel.width());
                rectDivLineLabel.setRight(rctBounding.left());
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
/*! @brief Sets the original rectangle coordinates in parent coordinates.

    @param [in] i_physValRect
        Rectangle coordinates to be set.

    @return Previous original rectangle coordinates.
*/
CPhysValRect CGraphObjGroup::setRectOrig(const CPhysValRect& i_physValRect)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "New {" + i_physValRect.toString() + "} " + i_physValRect.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setRectOrig",
        /* strAddInfo   */ strMthInArgs );

    CPhysValRect physValRectPrev = m_physValRectOrig;
    m_physValRectOrig = i_physValRect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + physValRectPrev.toString() + "} " + physValRectPrev.unit().symbol());
    }
    return physValRectPrev;
}

//------------------------------------------------------------------------------
/*! @brief Sets the original rectangle coordinates in local coordinates.

    @param [in] i_rect
        Rectangle coordinates to be set.

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
        /* strObjName   */ m_strName,
        /* strMethod    */ "setRectOrig",
        /* strAddInfo   */ strMthInArgs );

    QRectF rectPrev = m_rectOrig;
    m_rectOrig = i_rect;
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn("Prev {" + qRect2Str(rectPrev) + "}");
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
    if (!m_physValRectOrig.isValid()) {
        return;
    }

    QRectF rectCurr = getBoundingRect();
    QRectF rectOrig;
    if (parentGroup() != nullptr) {
        rectOrig = parentGroup()->convert(m_physValRectOrig, Units.Length.px).toQRectF();
    }
    else {
        rectOrig = m_pDrawingScene->convert(m_physValRectOrig, Units.Length.px).toQRectF();
    }
    double fScaleX = rectCurr.width() / rectOrig.width();
    double fScaleY = rectCurr.height() / rectOrig.height();

    QList<QGraphicsItem*> arpGraphicsItemsChilds = childItems();
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        mthTracer.trace("Childs [" + QString::number(arpGraphicsItemsChilds.size()) + "]");
    }
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
            strRuntimeInfo += "PhysValRect {" + getRect().toString() + "} " + getRect().unit().symbol() +
            ", Orig {" + m_physValRectOrig.toString() + "} " + m_physValRectOrig.unit().symbol();
            i_mthTracer.trace(strRuntimeInfo);
            if (i_mthDir == EMethodDir::Enter) strRuntimeInfo = "-+ ";
            else if (i_mthDir == EMethodDir::Leave) strRuntimeInfo = "+- ";
            else strRuntimeInfo = "   ";
            strRuntimeInfo += "ItemRect Orig {" + qRect2Str(m_rectOrig) + "}";
            i_mthTracer.trace(strRuntimeInfo);
            CGraphObj::traceThisPositionInfo(i_mthTracer, i_mthDir, i_detailLevel);
        }
    }
}
