/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::Trace;


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
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ CObjFactory::c_strGroupNameStandardShapes,
        /* type                */ EGraphObjTypeGroup,
        /* strType             */ ZS::Draw::graphObjType2Str(EGraphObjTypeGroup),
        /* strObjName          */ i_strObjName.isEmpty() ? "Group" + QString::number(s_iInstCount) : i_strObjName,
        /* drawSettings        */ i_drawSettings ),
    QGraphicsItemGroup()
{
    s_iInstCount++;

    QString strMthInArgs;
    QString strAddTrcInfo;

    QString strNameSpace = NameSpace() + CObjFactory::c_strGroupSeparater + CObjFactory::c_strGroupNameStandardShapes;
    m_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "CtorsAndDtor");
    m_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "ItemChange");
    m_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "BoundingRect");
    m_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "Paint");
    m_pTrcAdminObjSceneEvent = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "SceneEvent");
    m_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "SceneEventFilter");
    m_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "HoverEvents");
    m_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "MouseEvents");
    m_pTrcAdminObjKeyEvents = CTrcServer::GetTraceAdminObj(strNameSpace, ClassName(), "KeyEvents");

    if( m_pTrcAdminObjCtorsAndDtor != nullptr && m_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges );

    updateToolTip();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

/*==============================================================================
protected: // ctor (used by derived classes)
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjGroup::CGraphObjGroup(
    CDrawingScene*       i_pDrawingScene,
    const QString&       i_strFactoryGroupName,
    EGraphObjType        i_type,
    const QString&       i_strType,
    const QString&       i_strObjName,
    const CDrawSettings& i_drawSettings ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene       */ i_pDrawingScene,
        /* strFactoryGroupName */ i_strFactoryGroupName,
        /* type                */ i_type,
        /* strType             */ i_strType,
        /* strObjName          */ i_strObjName,
        /* drawSettings        */ i_drawSettings ),
    QGraphicsItemGroup()
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
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // Please note that the dynamic cast to QGraphicsItem returns nullptr if the
    // dtor of QGraphicsItem has already been executed. The order the dtors
    // of inherited classes are called depends on the order the classes
    // appear in the list of the inherited classes on defining the
    // class implementation. So we can't call "removeItem" from within the
    // dtor of the base class CGraphObj but must remove the graphics item from
    // the drawing scene's item list before the dtor of class QGraphicsItem is
    // called. And this is only always the case in the dtor of the class
    // derived from QGraphicsItem.

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        if( m_pDrawingScene != nullptr )
        {
            if( !m_strKeyInTree.isEmpty() )
            {
                try
                {
                    // Cannot be called from within dtor of "CGraphObj" as the dtor
                    // of class "QGraphicsItem" may have already been processed and
                    // models and Views may still try to access the graphical object.
                    m_pDrawingScene->onGraphObjDestroying(m_strKeyInTree);
                }
                catch(...)
                {
                }
            }

            // Please note that the dynamic cast to QGraphicsItem returns nullptr if the
            // dtor of QGraphicsItem has already been executed. The order the dtors
            // of inherited classes are called depends on the order the classes
            // appear in the list of the inherited classes on defining the
            // class implementation. So we can't call "removeItem" from within the
            // dtor of the base class CGraphObj but must remove the graphics item from
            // the drawing scene's item list before the dtor of class QGraphicsItem is
            // called. And this is only always the case in the dtor of the class
            // derived from QGraphicsItem.
            // Moreover on removing (deleting) a group the group's children have already
            // been removed from the drawing scene by the dtor of class QGraphicsItemGroup
            // (which is inherited by CGraphObjGroup) and "scene()" may return nullptr.
            m_pDrawingScene->removeGraphObj(this);

        } // if( m_pDrawingScene != nullptr )
    } // if( pGraphicsItem != nullptr )

} // dtor

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjGroup::clone()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjCtorsAndDtor != nullptr && m_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ strMthInArgs );

    CGraphObjGroup* pGraphObj = new CGraphObjGroup(m_pDrawingScene,m_drawSettings);

    pGraphObj->setName(m_strName);

    QList<QGraphicsItem*> arpGraphicsItemsChilds = childItems();
    CGraphObj*            pGraphObjChild;
    CGraphObj*            pGraphObjChildClone;
    int                   idxGraphObjChild;

    for( idxGraphObjChild = 0; idxGraphObjChild < arpGraphicsItemsChilds.size(); idxGraphObjChild++ )
    {
        pGraphObjChild = dynamic_cast<CGraphObj*>(arpGraphicsItemsChilds[idxGraphObjChild]);

        if( pGraphObjChild != nullptr )
        {
            pGraphObjChildClone = pGraphObjChild->clone();

            if( pGraphObjChildClone != nullptr )
            {
                throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
                //pGraphObj->addGraphObj(pGraphObjChildClone);
            }
        }
    }

    QPointF ptPos = pos();

    pGraphObj->setPos(ptPos);
    pGraphObj->setRotationAngleInDegree(m_fRotAngleCurr_deg);
    //pGraphObj->setScaleFactors(m_fScaleFacXCurr,m_fScaleFacYCurr);
    pGraphObj->acceptCurrentAsOriginalCoors();

    return pGraphObj;

} // clone

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CGraphObjGroup::addGraphObj( CGraphObj* i_pGraphObj )
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObjCtorsAndDtor != nullptr && m_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
//    {
//        strMthInArgs  = "GraphObj:" + i_pGraphObj->name();
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "addGraphObj",
//        /* strAddInfo   */ strMthInArgs );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
//        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
//        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
//        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
//        mthTracer.trace(strAddTrcInfo);
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
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObjCtorsAndDtor != nullptr && m_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
//    {
//        strMthInArgs = i_pGraphObj->keyInTree();
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strObjName   */ m_strName,
//        /* strMethod    */ "removeGraphObj",
//        /* strAddInfo   */ strMthInArgs );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
//        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
//        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
//        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
//        mthTracer.trace(strAddTrcInfo);
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
    QString   strScenePolygonShapePoints;
    QPolygonF plgScene = mapToScene(m_rctCurr);

    strScenePolygonShapePoints = polygon2Str(plgScene);

    return strScenePolygonShapePoints;

} // getScenePolygonShapePointsString

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::setWidth( double i_fWidth )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs = "Width:" + QString::number(i_fWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strMthInArgs );

    setSize( i_fWidth, m_rctCurr.height() );

} // setWidth

//------------------------------------------------------------------------------
void CGraphObjGroup::setHeight( double i_fHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs = "Height:" + QString::number(i_fHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strMthInArgs );

    setSize( m_rctCurr.width(), i_fHeight );

} // setHeight

//------------------------------------------------------------------------------
void CGraphObjGroup::setSize( double i_fWidth, double i_fHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs  = "Width:" + QString::number(i_fWidth);
        strMthInArgs += ", Height:" + QString::number(i_fHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strMthInArgs );

    double fWidth  = i_fWidth;
    double fHeight = i_fHeight;

    if( hasFixedWidth() && fWidth != getFixedWidth() )
    {
        fWidth = getFixedWidth();
    }
    else
    {
        if( hasMinimumWidth() && fWidth > getMinimumWidth() )
        {
            fWidth = getMinimumWidth();
        }
        if( hasMaximumWidth() && fWidth > getMaximumWidth() )
        {
            fWidth = getMaximumWidth();
        }
    }

    if( hasFixedHeight() && fHeight != getFixedHeight() )
    {
        fHeight = getFixedHeight();
    }
    else
    {
        if( hasMinimumHeight() && fHeight > getMinimumHeight() )
        {
            fHeight = getMinimumHeight();
        }
        if( hasMaximumHeight() && fHeight > getMaximumHeight() )
        {
            fHeight = getMaximumHeight();
        }
    }

    if( fWidth != m_rctCurr.width() || fHeight != m_rctCurr.height() )
    {
        m_rctCurr.setWidth(fWidth);
        m_rctCurr.setHeight(fHeight);

        m_ptRotOriginCurr = m_rctCurr.center();

        applyGeometryChangeToChildrens();

        if( isSelected() )
        {
            updateSelectionPointsOfBoundingRect(m_rctCurr);
        }

        updateLabelPositions();

        //itemChange( ItemPositionHasChanged, pos() ); // ItemSizeHasChanged does not exist

        updateEditInfo();
        updateToolTip();
    }

} // setSize

//------------------------------------------------------------------------------
void CGraphObjGroup::setSize( const QSizeF& i_size )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs  = "Width:" + QString::number(i_size.width());
        strMthInArgs += ", Height:" + QString::number(i_size.height());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strMthInArgs );

    setSize( i_size.width(), i_size.height() );

} // setSize

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::applyGeometryChangeToChildrens()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "applyGeometryChangeToChildrens",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Rect:" + rect2Str(m_rctCurr);
        mthTracer.trace(strAddTrcInfo);
    }

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

    prepareGeometryChange();

} // applyGeometryChangeToChildrens

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::setIsHit( bool i_bHit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs = "Hit:" + bool2Str(i_bHit);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setIsHit",
        /* strAddInfo   */ strMthInArgs );

    if( m_bIsHit != i_bHit )
    {
        m_bIsHit = i_bHit;

        if( i_bHit )
        {
            showSelectionPoints(ESelectionPointsBoundingRectCorner|ESelectionPointsBoundingRectLineCenter);
        }
        else if( !isSelected() )
        {
            hideSelectionPoints();
        }
        update();
    }

} // setIsHit

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjGroup::isHit( const QPointF& i_pt, SGraphObjHitInfo* o_pHitInfo ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthOutArgs;
    QString strAddTrcInfo;
    QString strMthReturn;

    if( m_pTrcAdminObjBoundingRect != nullptr && m_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs  = "Point:" + point2Str(i_pt);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "isHit",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "Rect: " + rect2Str(m_rctCurr);
        mthTracer.trace(strAddTrcInfo);
    }

    bool bIsHit = false;

    const QGraphicsItem* pGraphicsItem = dynamic_cast<const QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        if( pGraphicsItem->isSelected() )
        {
            bIsHit = isBoundingRectSelectionPointHit(
                /* pt               */ i_pt,
                /* iSelPtsCount     */ -1,
                /* pSelPts          */ nullptr,
                /* pGraphObjHitInfo */ o_pHitInfo );
        }

        if( !bIsHit )
        {
            bIsHit = isRectHit( m_rctCurr, EFillStyle::SolidPattern, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );
        }

        if( !bIsHit )
        {
            if( pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyle::SolidPattern )
            {
                bIsHit = pGraphicsItem->contains(i_pt);

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditMode::Move;
                    o_pHitInfo->m_editResizeMode = EEditResizeMode::None;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPoint::None;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                }
            }
        }

        if( bIsHit && o_pHitInfo != nullptr )
        {
            o_pHitInfo->setCursor( Math::deg2Rad(m_fRotAngleCurr_deg) );
        }
    } // if( pGraphicsItem != nullptr )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        if( o_pHitInfo != nullptr )
        {
            strMthOutArgs  = "EditMode:" + o_pHitInfo->m_editMode.toString();
            strMthOutArgs += ", ResizeMode:" + o_pHitInfo->m_editResizeMode.toString();
            strMthOutArgs += ", SelPtBoundingRect:" + o_pHitInfo->m_selPtBoundingRect.toString();
            strMthOutArgs += ", PolygonShapePoint:" + QString::number(o_pHitInfo->m_idxPolygonShapePoint);
            strMthOutArgs += ", LineSegment:" + QString::number(o_pHitInfo->m_idxLineSegment);
            strMthOutArgs += ", PointSelected:" + point2Str(o_pHitInfo->m_ptSelected);
            strMthOutArgs += ", Cursor:" + qCursorShape2Str(o_pHitInfo->m_cursor.shape());
        }
        mthTracer.setMethodOutArgs(strMthOutArgs);
        mthTracer.setMethodReturn(bIsHit);
    }

    return bIsHit;

} // isHit

/*==============================================================================
public: // reimplementing methods of base class QGraphicItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::setCursor( const QCursor& i_cursor )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = qCursorShape2Str(i_cursor.shape());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setCursor",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItemGroup::setCursor(i_cursor);

} // setCursor

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::showSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs = selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    if( parentItem() == nullptr )
    {
        showSelectionPointsOfBoundingRect( m_rctCurr, i_selPts );
    }
} // showSelectionPoints

//------------------------------------------------------------------------------
void CGraphObjGroup::updateSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs = selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "updateSelectionPoints",
        /* strAddInfo   */ strMthInArgs );

    if( parentItem() == nullptr )
    {
        updateSelectionPointsOfBoundingRect( m_rctCurr, i_selPts );
    }
} // updateSelectionPoints

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::onParentItemCoorsHasChanged( CGraphObj* i_pGraphObjParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs = QString(i_pGraphObjParent == nullptr ? "nullptr" : i_pGraphObjParent->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onParentItemCoorsHasChanged",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    // Connection lines don't belong to groups. But their connection points do. So if the
    // group is moved also the connection points are moved by Qt's graphics scene.
    // But not the connection lines which are linked to the connection points. We
    // call "onParentItemCoorsHasChanged" to inform the connection points and so that
    // child groups forward the call to their child groups and child connection points.

    QList<QGraphicsItem*> arpGraphicsItemChilds = childItems();
    QGraphicsItem*        pGraphicsItem;
    CGraphObj*            pGraphObj;
    int                   idxGraphObj;

    for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemChilds.size(); idxGraphObj++ )
    {
        pGraphicsItem = arpGraphicsItemChilds[idxGraphObj];

        pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

        if( pGraphObj != nullptr )
        {
            pGraphObj->onParentItemCoorsHasChanged(this);
        }
    }

    updateEditInfo();
    updateToolTip();

} // onParentItemCoorsHasChanged

/*==============================================================================
public: // overridables of base class QGraphicsPolygonItem
==============================================================================*/

//------------------------------------------------------------------------------
QRectF CGraphObjGroup::boundingRect() const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthReturn;

    if( m_pTrcAdminObjBoundingRect != nullptr && m_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strMthInArgs );

    QRectF rctBounding = childrenBoundingRect();

    CGraphObjSelectionPoint* pGraphObjSelPt;
    QRectF                   rctSelPt;
    QPolygonF                plgSelPt;
    int                      idxSelPt;

    for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
    {
        pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

        if( pGraphObjSelPt != nullptr )
        {
            rctSelPt = pGraphObjSelPt->boundingRect();
            plgSelPt = mapFromItem( pGraphObjSelPt, rctSelPt );
            rctBounding |= plgSelPt.boundingRect();
        }
    }

    if( m_pDrawingScene->getMode() == EMode::Edit && isSelected() )
    {
        // Half pen width of the selection rectangle would be enough.
        // But the whole pen width is also not a bad choice.
        rctBounding.adjust( -2.0, -2.0, 2.0, 2.0 );
    }

    //if( m_bIsHit || isSelected() )
    //{
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

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthReturn = rect2Str(rctBounding);
        mthTracer.setMethodReturn(strMthReturn);
    }

    return rctBounding;

} // boundingRect

//------------------------------------------------------------------------------
void CGraphObjGroup::paint(
    QPainter*                       i_pPainter,
    const QStyleOptionGraphicsItem* /*i_pStyleOption*/,
    QWidget*                        /*i_pWdgt*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjPaint != nullptr && m_pTrcAdminObjPaint->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjPaint,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    i_pPainter->save();

    QPen pn(Qt::DotLine);

    if( m_bBoundRectVisible || (m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected())) )
    {
        if( m_bIsHit || isSelected() )
        {
            pn.setColor(Qt::blue);
        }
        else
        {
            pn.setColor(Qt::black);
        }

        i_pPainter->setPen(pn);
        i_pPainter->setBrush(Qt::NoBrush);

        i_pPainter->drawRect(m_rctCurr);

        if( isSelected() )
        {
            if( m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)] != nullptr && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)] != nullptr )
            {
                CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::TopCenter)];
                CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateTop)];

                QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                QPointF ptRctM = mapFromScene(ptRct);
                QPointF ptRotM = mapFromScene(ptRot);

                i_pPainter->drawLine( ptRctM, ptRotM );
            }

            if( m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)] != nullptr && m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)] != nullptr )
            {
                CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::BottomCenter)];
                CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[static_cast<int>(ESelectionPoint::RotateBottom)];

                QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                QPointF ptRctM = mapFromScene(ptRct);
                QPointF ptRotM = mapFromScene(ptRot);

                i_pPainter->drawLine( ptRctM, ptRotM );
            }
        } // if( isSelected() )
    } // if( m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected()) )

    i_pPainter->restore();

} // paint

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
bool CGraphObjGroup::sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    //--------------------------------------------------------------------
    // Mouse events of selection points will be "watched" to forward those
    // events to this item in order to resize and rotate this item.
    //--------------------------------------------------------------------

    CGraphObjSelectionPoint* pGraphObjSelPtWatched = dynamic_cast<CGraphObjSelectionPoint*>(i_pGraphicsItemWatched);

    if( pGraphObjSelPtWatched == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObjSelPtWatched == nullptr" );
    }

    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjSceneEventFilter != nullptr && m_pTrcAdminObjSceneEventFilter->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs  = "ItemWatched:" + pGraphObjSelPtWatched->name();
        strMthInArgs += ", Event:" + qEventType2Str(i_pEv->type());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjSceneEventFilter,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "sceneEventFilter",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    bool bEventHandled = false;

    switch( i_pEv->type() )
    {
        case QEvent::GraphicsSceneMouseMove:
        {
            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            if( pEv != nullptr )
            {
                QPointF ptEvPos;
                for( int i = 0x1; i <= 0x10; i <<= 1 )
                {
                    Qt::MouseButton button = Qt::MouseButton(i);
                    ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->buttonDownPos(button) );
                    pEv->setButtonDownPos( button, ptEvPos );
                }
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->pos() );
                pEv->setPos(ptEvPos);
                ptEvPos = mapFromItem( pGraphObjSelPtWatched, pEv->lastPos() );
                pEv->setLastPos(ptEvPos);
                mouseMoveEvent(pEv);
                bEventHandled = true;
            }
            break;
        }
        case QEvent::GraphicsSceneMousePress:
        {
            QGraphicsSceneMouseEvent* pEv = dynamic_cast<QGraphicsSceneMouseEvent*>(i_pEv);
            if( pEv != nullptr )
            {
                QPointF ptEvPos;
                for( int i = 0x1; i <= 0x10; i <<= 1 )
                {
                    Qt::MouseButton button = Qt::MouseButton(i);
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
            if( pEv != nullptr )
            {
                QPointF ptEvPos;
                for( int i = 0x1; i <= 0x10; i <<= 1 )
                {
                    Qt::MouseButton button = Qt::MouseButton(i);
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
            if( pEv != nullptr )
            {
                QPointF ptEvPos;
                for( int i = 0x1; i <= 0x10; i <<= 1 )
                {
                    Qt::MouseButton button = Qt::MouseButton(i);
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

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
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
    QString strAddTrcInfo;

    if( m_pTrcAdminObjHoverEvents != nullptr && m_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
    {
        SGraphObjHitInfo hitInfo;

        bool bIsHit = isHit(ptItemPos,&hitInfo);

        if( bIsHit )
        {
            if( cursor().shape() != hitInfo.m_cursor.shape() )
            {
                setCursor(hitInfo.m_cursor);
            }
        }
    } // if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )

} // hoverEnterEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjHoverEvents != nullptr && m_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

    CEnumMode     modeDrawing     = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )
    {
        SGraphObjHitInfo hitInfo;

        bool bIsHit = isHit(ptItemPos,&hitInfo);

        if( bIsHit )
        {
            if( cursor().shape() != hitInfo.m_cursor.shape() )
            {
                setCursor(hitInfo.m_cursor);
            }
        }
    } // if( modeDrawing == EMode::Edit && editToolDrawing == EEditTool::Select )

} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjHoverEvents != nullptr && m_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    unsetCursor();

} // hoverLeaveEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    CEnumMode     modeDrawing = m_pDrawingScene->getMode();
    CEnumEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit )
    {
        if( editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None )
        {
            QGraphicsItemGroup::mousePressEvent(i_pEv); // this will select the item (creating selection points)

            CGraphObjSelectionPoint* pGraphObjSelPt;
            ESelectionPoint          selPt;
            int                      idxSelPt;

            SGraphObjHitInfo hitInfo;

            QPointF ptMouseItemPos = i_pEv->pos();

            bool bIsHit = isHit(ptMouseItemPos, &hitInfo);

            m_editMode                  = hitInfo.m_editMode;
            m_editResizeMode            = hitInfo.m_editResizeMode;
            m_selPtSelectedBoundingRect = hitInfo.m_selPtBoundingRect;

            for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
            {
                selPt = static_cast<ESelectionPoint>(idxSelPt);

                pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

                if( pGraphObjSelPt != nullptr )
                {
                    if( m_selPtSelectedBoundingRect == selPt )
                    {
                        pGraphObjSelPt->setSelected(true);
                    }
                    else
                    {
                        pGraphObjSelPt->setSelected(false);
                    }
                }
            }

            m_rctOnMousePressEvent = m_rctCurr;

            m_ptRotOriginOnMousePressEvent = mapToScene(m_ptRotOriginCurr);

            m_pDrawingScene->setMode( EMode::Ignore, EEditTool::Ignore, m_editMode, m_editResizeMode, false );

            updateEditInfo();
            updateToolTip();

        } // if( editToolDrawing == EEditTool::Select && m_editMode == EEditMode::None )

    } // if( modeDrawing == EMode::Edit )

    else if( modeDrawing == EMode::Simulation )
    {
        SGraphObjMouseEventFct fctEntry;
        int                    idxFct;

        for( idxFct = 0; idxFct < m_arMousePressEventFunctions.size(); idxFct++ )
        {
            fctEntry = m_arMousePressEventFunctions[idxFct];

            if( fctEntry.m_pFct != nullptr )
            {
                fctEntry.m_pFct( fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv );
            }
        }

    } // if( modeDrawing == EMode::Simulation )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelInternalStates) )
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        // Mouse events may first be dispatched to childrens of the group. Mapping the mouse
        // coordinates from the childrens to the parent's (the group's) coordinate system
        // does not work for succeeding mouse move events. So the mouse item pos returned
        // by "i_pEv->pos()" is not correct here and we need to map the mouse scene event
        // pos to the group's mouse item pos.

        if( m_editMode == EEditMode::Creating )
        {
        } // if( m_editMode == EEditMode::Creating )

        else if( m_editMode == EEditMode::Move )
        {
            QGraphicsItemGroup::mouseMoveEvent(i_pEv);

        } // if( m_editMode == EEditMode::Move )

        else if( m_editMode == EEditMode::Resize )
        {
            QPointF ptMouseItemPos = i_pEv->pos();

            m_rctCurr = resizeRect( m_rctOnMousePressEvent, m_selPtSelectedBoundingRect.enumerator(), ptMouseItemPos, nullptr );

            // Don't change the position of the object as the position of further
            // mouse events should be received relative to the object's position
            // on starting the edit process on pressing the mouse.
            //setPos(ptPosNew); // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

            applyGeometryChangeToChildrens();

            updateSelectionPointsOfBoundingRect(m_rctCurr);

            updateLabelPositions();

            updateEditInfo();
            updateToolTip();

            update();

        } // if( m_editMode == EEditMode::Resize )

        else if( m_editMode == EEditMode::Rotate )
        {
            QPointF ptMouseScenePos = i_pEv->scenePos(); // see comment above

            double fRotAngle_rad = getAngleRad( m_ptRotOriginOnMousePressEvent, ptMouseScenePos );

            m_fRotAngleCurr_deg = Math::rad2Deg(fRotAngle_rad);

            switch( m_selPtSelectedBoundingRect.enumerator() )
            {
                case ESelectionPoint::RotateTop:
                {
                    m_fRotAngleCurr_deg -= 90.0;
                    break;
                }
                case ESelectionPoint::RotateBottom:
                {
                    m_fRotAngleCurr_deg -= 270.0;
                    break;
                }
                default:
                {
                    break;
                }
            }

            m_fRotAngleCurr_deg = Math::round2Res( m_fRotAngleCurr_deg, m_pDrawingScene->getRotationAngleResolutionInDegree() );

            while( m_fRotAngleCurr_deg >= 360.0 )
            {
                m_fRotAngleCurr_deg -= 360.0;
            }
            while( m_fRotAngleCurr_deg < 0.0 )
            {
                m_fRotAngleCurr_deg += 360.0;
            }

            updateTransform();
            updateEditInfo();
            updateToolTip();

            update();

        } // if( m_editMode == EEditMode::Rotate )

        else if( m_editMode == EEditMode::MoveShapePoint )
        {
        } // if( m_editMode == EEditMode::MoveShapePoint )

        else if( m_editMode == EEditMode::EditText )
        {
        } // if( m_editMode == EEditMode::EditText )

    } // if( modeDrawing == EMode::Edit )

    else if( modeDrawing == EMode::Simulation )
    {
        SGraphObjMouseEventFct fctEntry;
        int                    idxFct;

        for( idxFct = 0; idxFct < m_arMouseMoveEventFunctions.size(); idxFct++ )
        {
            fctEntry = m_arMouseMoveEventFunctions[idxFct];

            if( fctEntry.m_pFct != nullptr )
            {
                fctEntry.m_pFct( fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv );
            }
        }

    } // if( modeDrawing == EMode::Simulation )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( m_editMode == EEditMode::Creating )
        {
            // Groups are not created by mouse events.

        } // if( m_editMode == EEditMode::Creating )

        else if( m_editMode == EEditMode::Move )
        {
        } // if( m_editMode == EEditMode::Move )

        else if( m_editMode == EEditMode::Resize )
        {
            // The item will not be resized to the position of the mouse release event.
            // A selection point might have been clicked and released immediately (without
            // moving the mouse). In this case changing the size of the item according to
            // position of the mouse is not expected.

            QPolygonF plgSceneNew         = mapToScene(m_rctCurr);
            QRectF    rctBoundingSceneNew = plgSceneNew.boundingRect();
            QPointF   ptRotOriginSceneNew = rctBoundingSceneNew.center();
            double    fAngle_rad          = Math::deg2Rad(m_fRotAngleCurr_deg);
            //QPointF   ptLTScenePosOld   = mapToScene( QPointF(0.0,0.0) );
            QPointF   ptLTScenePosNew     = mapToScene( m_rctCurr.topLeft() );
            QPointF   ptItemScenePosNew   = rotatePoint( ptRotOriginSceneNew, ptLTScenePosNew, -fAngle_rad );

            if( m_rctCurr.left() != 0.0 )
            {
                m_rctCurr.moveLeft(0.0);
            }
            if( m_rctCurr.top() != 0.0 )
            {
                m_rctCurr.moveTop(0.0);
            }

            m_ptRotOriginCurr = m_rctCurr.center();

            setPos(ptItemScenePosNew);    // does not lead to "itemChange" call even if flag ItemSendsGeometryChanges is set.

            updateTransform();

            applyGeometryChangeToChildrens();

            updateSelectionPointsOfBoundingRect(m_rctCurr);

            updateLabelPositions();

            // Not for group items. Otherwise the layout information would get lost.
            //acceptCurrentAsOriginalCoors();

            updateEditInfo();
            updateToolTip();

        } // if( m_editMode == EEditMode::Resize )

        else if( m_editMode == EEditMode::Rotate )
        {
        } // if( m_editMode == EEditMode::Rotate )

        else if( m_editMode == EEditMode::MoveShapePoint )
        {
            // Groups don't have movable shape points.

        } // if( m_editMode == EEditMode::MoveShapePoint )

        m_editMode = EEditMode::None;
        m_editResizeMode = EEditResizeMode::None;
        m_idxSelPtSelectedPolygon = -1;
        m_selPtSelectedBoundingRect = ESelectionPoint::None;

    } // if( modeDrawing == EMode::Edit )

    else if( modeDrawing == EMode::Simulation )
    {
        SGraphObjMouseEventFct fctEntry;
        int                    idxFct;

        for( idxFct = 0; idxFct < m_arMouseReleaseEventFunctions.size(); idxFct++ )
        {
            fctEntry = m_arMouseReleaseEventFunctions[idxFct];

            if( fctEntry.m_pFct != nullptr )
            {
                fctEntry.m_pFct( fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv );
            }
        }

    } // if( modeDrawing == EMode::Simulation )

    // The mouse release event would select the object.
    // This is not wanted if the selection tool is not active.
    bool bIsSelectable = flags() & QGraphicsItem::ItemIsSelectable;
    bool bIsSelectableReset = false;

    if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditTool::Select )
    {
        setFlag(QGraphicsItem::ItemIsSelectable,false);
        bIsSelectableReset = true;
    }

    QGraphicsItemGroup::mouseReleaseEvent(i_pEv);

    if( bIsSelectableReset )
    {
        setFlag(QGraphicsItem::ItemIsSelectable,bIsSelectable);
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strMthInArgs += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strMthInArgs += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    // When double clicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // double click event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( isSelected() )
        {
            onCreateAndExecDlgFormatGraphObjs();
        }

    } // if( modeDrawing == EMode::Edit )

    else if( modeDrawing == EMode::Simulation )
    {
        SGraphObjMouseEventFct fctEntry;
        int                    idxFct;

        for( idxFct = 0; idxFct < m_arMouseDoubleClickEventFunctions.size(); idxFct++ )
        {
            fctEntry = m_arMouseDoubleClickEventFunctions[idxFct];

            if( fctEntry.m_pFct != nullptr )
            {
                fctEntry.m_pFct( fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv );
            }
        }

    } // if( modeDrawing == EMode::Simulation )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjKeyEvents != nullptr && m_pTrcAdminObjKeyEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs  = "Ev.Key:" + qKeyCode2Str(i_pEv->key());
        strMthInArgs += ", Ev.Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        QGraphicsItemGroup::keyPressEvent(i_pEv);

    } // if( modeDrawing == EMode::Edit )

    else if( modeDrawing == EMode::Simulation )
    {
        SGraphObjKeyEventFct fctEntry;
        int                  idxFct;

        for( idxFct = 0; idxFct < m_arKeyPressEventFunctions.size(); idxFct++ )
        {
            fctEntry = m_arKeyPressEventFunctions[idxFct];

            if( fctEntry.m_pFct != nullptr )
            {
                fctEntry.m_pFct( fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv );
            }
        }

    } // if( modeDrawing == EMode::Simulation )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

} // keyPressEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if( m_pTrcAdminObjKeyEvents != nullptr && m_pTrcAdminObjKeyEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs  = "Ev.Key:" + qKeyCode2Str(i_pEv->key());
        strMthInArgs += ", Ev.Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        QGraphicsItemGroup::keyReleaseEvent(i_pEv);

    } // if( modeDrawing == EMode::Edit )

    else if( modeDrawing == EMode::Simulation )
    {
        SGraphObjKeyEventFct fctEntry;
        int                  idxFct;

        for( idxFct = 0; idxFct < m_arKeyReleaseEventFunctions.size(); idxFct++ )
        {
            fctEntry = m_arKeyReleaseEventFunctions[idxFct];

            if( fctEntry.m_pFct != nullptr )
            {
                fctEntry.m_pFct( fctEntry.m_pvThis, fctEntry.m_pvData, this, i_pEv );
            }
        }

    } // if( modeDrawing == EMode::Simulation )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

} // keyReleaseEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
QVariant CGraphObjGroup::itemChange( GraphicsItemChange i_change, const QVariant& i_value )
//------------------------------------------------------------------------------
{
    if( m_bDtorInProgress )
    {
        return i_value;
    }

    QString strMthInArgs;
    QString strAddTrcInfo;
    QString strMthReturn;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strMthInArgs = "Changed:" + qGraphicsItemChange2Str(i_change);

        if( i_value.type() == QVariant::PointF )
        {
            strMthInArgs += ", Value(" + qVariantType2Str(i_value.type()) + "):" + point2Str(i_value.toPointF());
        }
        else
        {
            strMthInArgs += ", Value(" + qVariantType2Str(i_value.type()) + "):" + i_value.toString();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    QVariant valChanged = i_value;

    bool bTreeEntryChanged = false;

    if( i_change == ItemSelectedHasChanged )
    {
        prepareGeometryChange();

        if( m_pDrawingScene->getMode() == EMode::Edit && isSelected() )
        {
            bringToFront(); // does not set "m_fZValue" as it is used to restore the stacking order on deselecting the object

            if( m_editMode == EEditMode::Creating )
            {
                showSelectionPoints(ESelectionPointsBoundingRectCorner|ESelectionPointsBoundingRectLineCenter|ESelectionPointsPolygonShapePoints);
            }
            else
            {
                showSelectionPoints();
            }

            // Not necessary as item has been brought to front and "showSelectionPoints"
            // sets zValue of selection points above item.
            //bringSelectionPointsToFront();

            setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton|Qt::MiddleButton|Qt::XButton1|Qt::XButton2);
        }
        else
        {
            setAcceptedMouseButtons(Qt::NoButton);

            hideSelectionPoints();

            setZValue(m_fZValue); // restore ZValue as before selecting the object

            m_editMode = EEditMode::None;
            m_editResizeMode = EEditResizeMode::None;
            m_selPtSelectedBoundingRect = ESelectionPoint::None;
            m_idxSelPtSelectedPolygon = -1;

        } // if( !isSelected() )

        updateEditInfo();
        updateToolTip();

        bTreeEntryChanged = true;

    } // if( i_change == ItemSelectedHasChanged )

    else if( i_change == ItemToolTipChange || i_change == ItemToolTipHasChanged
          || i_change == ItemFlagsChange || i_change == ItemFlagsHaveChanged
          || i_change == ItemPositionChange
          || i_change == ItemVisibleChange
          || i_change == ItemEnabledChange
          || i_change == ItemSelectedChange
          || i_change == ItemParentChange
          || i_change == ItemTransformChange
          || i_change == ItemSceneChange
          || i_change == ItemCursorChange
          || i_change == ItemZValueChange
          #if QT_VERSION >= 0x040700
          || i_change == ItemOpacityChange
          || i_change == ItemRotationChange
          || i_change == ItemScaleChange
          || i_change == ItemTransformOriginPointChange )
          #else
          || i_change == ItemOpacityChange )
          #endif
    {
    }

    else if( i_change == ItemVisibleHasChanged
          || i_change == ItemEnabledHasChanged
          || i_change == ItemCursorHasChanged
          || i_change == ItemOpacityHasChanged )
    {
    }

    else if( i_change == ItemChildAddedChange
          || i_change == ItemChildRemovedChange )
    {
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

    else if( i_change == ItemTransformChange
          || i_change == ItemPositionHasChanged
          || i_change == ItemTransformHasChanged
          || i_change == ItemParentHasChanged
          || i_change == ItemSceneHasChanged
          #if QT_VERSION >= 0x040700
          || i_change == ItemScenePositionHasChanged
          || i_change == ItemRotationHasChanged
          || i_change == ItemScaleHasChanged
          || i_change == ItemTransformOriginPointHasChanged )
          #else
          || i_change == ItemScenePositionHasChanged )
          #endif
    {
        // Connection lines don't belong to groups. But their connection points do. So if the
        // group is moved also the connection points are moved by Qt's graphics scene.
        // But not the connection lines which are linked to the connection points. We
        // call "onParentItemCoorsHasChanged" to inform the connection points and so that
        // child groups forward the call to their child groups and child connection points.

        QList<QGraphicsItem*> arpGraphicsItemChilds = childItems();
        QGraphicsItem*        pGraphicsItem;
        CGraphObj*            pGraphObj;
        int                   idxGraphObj;

        for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemChilds.size(); idxGraphObj++ )
        {
            pGraphicsItem = arpGraphicsItemChilds[idxGraphObj];

            pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

            if( pGraphObj != nullptr )
            {
                pGraphObj->onParentItemCoorsHasChanged(this);
            }
        }

        updateSelectionPointsOfBoundingRect(m_rctCurr);

        updateLabelPositions();

        updateEditInfo();
        updateToolTip();

        update();
    }

    else if( i_change == ItemZValueHasChanged )
    {
        CGraphObjSelectionPoint* pGraphObjSelPt;
        ESelectionPoint          selPt;
        int                      idxSelPt;

        for( idxSelPt = 0; idxSelPt < CEnumSelectionPoint::count(); idxSelPt++ )
        {
            selPt = static_cast<ESelectionPoint>(idxSelPt);

            pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

            if( pGraphObjSelPt != nullptr )
            {
                pGraphObjSelPt->setZValue( zValue() + 0.05 );
            }
        }

        for( idxSelPt = 0; idxSelPt < m_arpSelPtsPolygon.size(); idxSelPt++ )
        {
            pGraphObjSelPt = m_arpSelPtsPolygon[idxSelPt];

            if( pGraphObjSelPt != nullptr )
            {
                pGraphObjSelPt->setZValue( zValue()+0.05 );
            }
        }

        //QHashIterator<QString, CGraphObjLabel*> itLabels(m_arpLabels);

        //CGraphObjLabel* pGraphObjLabel;

        //while( itLabels.hasNext() )
        //{
        //    itLabels.next();

        //    pGraphObjLabel = itLabels.value();

        //    if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
        //    {
        //        pGraphObjLabel->m_pGraphObjLabel->setZValue( zValue() + 0.02 );
        //    }
        //}
    }

    if( bTreeEntryChanged && m_pTree != nullptr )
    {
        m_pTree->onTreeEntryChanged(this);
    }

    valChanged = QGraphicsItem::itemChange(i_change,i_value);

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_value.type() == QVariant::PointF )
        {
            strMthReturn = "ValChanged(" + qVariantType2Str(valChanged.type()) + "):" + point2Str(valChanged.toPointF());
        }
        else
        {
            strMthReturn = "ValChanged(" + qVariantType2Str(valChanged.type()) + "):" + valChanged.toString();
        }
        mthTracer.setMethodReturn(strMthReturn);
    }

    return valChanged;

} // itemChange
