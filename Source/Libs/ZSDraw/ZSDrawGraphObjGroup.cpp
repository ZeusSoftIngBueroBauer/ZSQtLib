/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
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
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::ResetCtorsDtorsCounters()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "CtorsCount: " + QString::number(s_iCtorsCount);
        strAddTrcInfo += ", DtorsCount: " + QString::number(s_iDtorsCount);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ "",
        /* strMethod    */ "ResetCtorsDtorsCounters",
        /* strAddInfo   */ strAddTrcInfo );

    if( s_iCtorsCount != s_iDtorsCount )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "CtorsCount(=" + QString::number(s_iCtorsCount) + ") != DtorsCount(=" + QString::number(s_iDtorsCount) + ")" );
    }

    s_iCtorsCount = 1;
    s_iDtorsCount = 1;

} // ResetCtorsDtorsCounters

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjGroup::s_iCtorsCount = 1;
qint64 CGraphObjGroup::s_iDtorsCount = 1;

CTrcAdminObj* CGraphObjGroup::s_pTrcAdminObjCtorsAndDtor = nullptr;
CTrcAdminObj* CGraphObjGroup::s_pTrcAdminObjBoundingRect = nullptr;
CTrcAdminObj* CGraphObjGroup::s_pTrcAdminObjPaint = nullptr;
CTrcAdminObj* CGraphObjGroup::s_pTrcAdminObjSceneEventFilter = nullptr;
CTrcAdminObj* CGraphObjGroup::s_pTrcAdminObjHoverEvents = nullptr;
CTrcAdminObj* CGraphObjGroup::s_pTrcAdminObjMouseEvents = nullptr;
CTrcAdminObj* CGraphObjGroup::s_pTrcAdminObjKeyEvents = nullptr;
CTrcAdminObj* CGraphObjGroup::s_pTrcAdminObjItemChange = nullptr;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjGroup::CGraphObjGroup(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName,
    const QString&       i_strObjId ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ "ZS::Draw",
        /* strClassName  */ "CGraphObjGroup",
        /* type          */ EGraphObjTypeGroup,
        /* strType       */ ZS::Draw::graphObjType2Str(EGraphObjTypeGroup),
        /* strObjName    */ i_strObjName.isEmpty() ? "Group" + QString::number(s_iCtorsCount) : i_strObjName,
        /* strObjId      */ i_strObjId.isEmpty() ? "Group" + QString::number(s_iCtorsCount) : i_strObjId,
        /* drawSettings  */ i_drawSettings ),
    QGraphicsItemGroup(),
    m_hshpGraphObjs()
    //m_fMoveOffsXOnMousePressEvent(0.0),
    //m_fMoveOffsYOnMousePressEvent(0.0)
{
    s_iCtorsCount++;

    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor == nullptr )
    {
        s_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjGroup", "CtorsAndDtor");
        s_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjGroup", "BoundingRect");
        s_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjGroup", "Paint");
        s_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjGroup", "SceneEventFilter");
        s_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjGroup", "HoverEvents");
        s_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjGroup", "MouseEvents");
        s_pTrcAdminObjKeyEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjGroup", "KeyEvents");
        s_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjGroup", "ItemChange");

    } // if( s_pTrcAdminObjCtorsAndDtor == nullptr )

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strAddTrcInfo );

    setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges );

    updateToolTip();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

/*==============================================================================
protected: // ctor (used by derived classes)
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjGroup::CGraphObjGroup(
    CDrawingScene*       i_pDrawingScene,
    const QString&       i_strNameSpace,
    const QString&       i_strClassName,
    EGraphObjType        i_type,
    const QString&       i_strType,
    const QString&       i_strObjName,
    const QString&       i_strObjId,
    const CDrawSettings& i_drawSettings ) :
//------------------------------------------------------------------------------
    CGraphObj(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ i_strNameSpace,
        /* strClassName  */ i_strClassName,
        /* type          */ i_type,
        /* strType       */ i_strType,
        /* strObjName    */ i_strObjName,
        /* strObjId      */ i_strObjId,
        /* drawSettings  */ i_drawSettings ),
    QGraphicsItemGroup(),
    m_hshpGraphObjs()
    //m_fMoveOffsXOnMousePressEvent(0.0),
    //m_fMoveOffsYOnMousePressEvent(0.0)
{
    s_iCtorsCount++;

    if( s_iCtorsCount == 1 )
    {
        s_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjGroup", "CtorsAndDtor");
        s_pTrcAdminObjBoundingRect = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "BoundingRect");
        s_pTrcAdminObjPaint = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "Paint");
        s_pTrcAdminObjSceneEventFilter = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "SceneEventFilter");
        s_pTrcAdminObjHoverEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "HoverEvents");
        s_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "MouseEvents");
        s_pTrcAdminObjKeyEvents = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "KeyEvents");
        s_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj("ZS::Draw", i_strClassName, "ItemChange");
    }

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjGroup::~CGraphObjGroup()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    s_iDtorsCount++;

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
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
            if( !m_strObjId.isEmpty() )
            {
                try
                {
                    // Cannot be called from within dtor of "CGraphObj" as the dtor
                    // of class "QGraphicsItem" may have already been processed and
                    // models and Views may still try to access the graphical object
                    // if the drawing scene emits the signal "graphObjDestroying".
                    m_pDrawingScene->onGraphObjDestroying(m_strObjId);
                }
                catch(...)
                {
                }
            }

            // On removing (deleting) a group the group's children have already been
            // removed from the graphics scene. The dtor of class CGraphObjGroup
            // removes the item from the drawing scene (see following line) whereupon
            // also the children of the group are removed by the QGraphicsScene class.
            // In this case "scene()" will return nullptr.
            QGraphicsScene* pGraphicsScene = pGraphicsItem->scene();
            if( pGraphicsScene != nullptr ) // if not already removed from the scene ...
            {
                m_pDrawingScene->removeItem(pGraphicsItem);
            }

        } // if( m_pDrawingScene != nullptr )

    } // if( pGraphicsItem != nullptr )

    if( s_iCtorsCount == s_iDtorsCount )
    {
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjCtorsAndDtor);
        s_pTrcAdminObjCtorsAndDtor = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjBoundingRect);
        s_pTrcAdminObjBoundingRect = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjPaint);
        s_pTrcAdminObjPaint = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjSceneEventFilter);
        s_pTrcAdminObjSceneEventFilter = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjHoverEvents);
        s_pTrcAdminObjHoverEvents = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjMouseEvents);
        s_pTrcAdminObjMouseEvents = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjKeyEvents);
        s_pTrcAdminObjKeyEvents = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjItemChange);
        s_pTrcAdminObjItemChange = nullptr;
    }

} // dtor

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjGroup::clone()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjGroup* pGraphObj = new CGraphObjGroup(m_pDrawingScene,m_drawSettings);

    pGraphObj->setObjName(m_strObjName);

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
                pGraphObj->addGraphObj(pGraphObjChildClone);
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

//------------------------------------------------------------------------------
void CGraphObjGroup::addGraphObj( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "GraphObj:" + i_pGraphObj->getObjName();
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "addGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);

    QGraphicsItemGroup::addToGroup(pGraphicsItem);

    i_pGraphObj->acceptCurrentAsOriginalCoors();

    if( m_hshpGraphObjs.contains(i_pGraphObj->getObjName()) )
    {
        if( m_hshpGraphObjs[i_pGraphObj->getObjName()] != i_pGraphObj )
        {
            // Has to be changed to: "findUniqueName" and call "i_pGraphObj->setObjName()"
            throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, i_pGraphObj->getObjName() );
        }
    }
    else
    {
        m_hshpGraphObjs.insert( i_pGraphObj->getObjName(), i_pGraphObj );
    }

} // addGraphObj

//------------------------------------------------------------------------------
void CGraphObjGroup::removeGraphObj( CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor != nullptr && s_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "GraphObj:" + i_pGraphObj->getObjName();
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "removeGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(i_pGraphObj);

    QGraphicsItemGroup::removeFromGroup(pGraphicsItem);

    i_pGraphObj->acceptCurrentAsOriginalCoors();

    if( m_hshpGraphObjs.contains(i_pGraphObj->getObjName()) )
    {
        m_hshpGraphObjs.remove( i_pGraphObj->getObjName() );
    }

} // removeGraphObj

//------------------------------------------------------------------------------
CGraphObj* CGraphObjGroup::findGraphObj( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObj = nullptr;

    if( m_hshpGraphObjs.contains(i_strObjName) )
    {
        pGraphObj = m_hshpGraphObjs[i_strObjName];
    }
    else
    {
        QList<QGraphicsItem*> arpGraphicsItems = childItems();
        QGraphicsItem*        pGraphicsItemTmp;
        CGraphObj*            pGraphObjTmp;
        int                   idxGraphObj;

        for( idxGraphObj = 0; idxGraphObj < arpGraphicsItems.size(); idxGraphObj++ )
        {
            pGraphicsItemTmp = arpGraphicsItems[idxGraphObj];

            pGraphObjTmp = dynamic_cast<CGraphObj*>(pGraphicsItemTmp);

            if( pGraphObjTmp != nullptr && pGraphObjTmp->getObjName() == i_strObjName )
            {
                pGraphObj = pGraphObjTmp;
                break;
            }
        }
    }

    return pGraphObj;

} // findGraphObj

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
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Width:" + QString::number(i_fWidth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setWidth",
        /* strAddInfo   */ strAddTrcInfo );

    setSize( i_fWidth, m_rctCurr.height() );

} // setWidth

//------------------------------------------------------------------------------
void CGraphObjGroup::setHeight( double i_fHeight )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Height:" + QString::number(i_fHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setHeight",
        /* strAddInfo   */ strAddTrcInfo );

    setSize( m_rctCurr.width(), i_fHeight );

} // setHeight

//------------------------------------------------------------------------------
void CGraphObjGroup::setSize( double i_fWidth, double i_fHeight )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Width:" + QString::number(i_fWidth);
        strAddTrcInfo += ", Height:" + QString::number(i_fHeight);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Width:" + QString::number(i_size.width());
        strAddTrcInfo += ", Height:" + QString::number(i_size.height());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setSize",
        /* strAddInfo   */ strAddTrcInfo );

    setSize( i_size.width(), i_size.height() );

} // setSize

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::applyGeometryChangeToChildrens()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Rect:" + rect2Str(m_rctCurr);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "applyGeometryChangeToChildrens",
        /* strAddInfo   */ strAddTrcInfo );

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
            ptPosChildOrig = pGraphObjChild->getPos(ECoordinatesVersionOriginal);
            fxPosChildOrig = ptPosChildOrig.x();
            fyPosChildOrig = ptPosChildOrig.y();

            fWidthChildOrig  = pGraphObjChild->getWidth(ECoordinatesVersionOriginal);
            fHeightChildOrig = pGraphObjChild->getHeight(ECoordinatesVersionOriginal);

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

                    if( alignment.m_alignmentRefChild == EAlignmentRefLeft )
                    {
                        bAlignLeft = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRefHCenter )
                    {
                        bAlignHCenter = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRefRight )
                    {
                        bAlignRight = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRefWidth )
                    {
                        bAlignWidth = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRefTop )
                    {
                        bAlignTop = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRefVCenter )
                    {
                        bAlignVCenter = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRefBottom )
                    {
                        bAlignBottom = true;
                    }
                    else if( alignment.m_alignmentRefChild == EAlignmentRefHeight )
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

                    switch( alignment.m_alignmentRefChild )
                    {
                        case EAlignmentRefLeft:
                        {
                            switch( alignment.m_alignmentRefParent )
                            {
                                case EAlignmentRefLeft:
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
                                case EAlignmentRefHCenter:
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
                                case EAlignmentRefRight:
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
                                case EAlignmentRefWidth:   // aligning left border of child to width of parent will very likely never been used ..
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
                                case EAlignmentRefHeight:   // aligning left border of child to height of parent will very likely never been used ..
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
                                case EAlignmentRefTop:      // aligning left border of child to top border of parent will very likely never been used ..
                                case EAlignmentRefVCenter:  // aligning left border of child to vertical center of parent will very likely never been used ..
                                case EAlignmentRefBottom:   // aligning left border of child to bottom border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRefLeft

                        case EAlignmentRefHCenter:
                        {
                            switch( alignment.m_alignmentRefParent )
                            {
                                case EAlignmentRefLeft:
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
                                case EAlignmentRefHCenter:
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
                                case EAlignmentRefRight:
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
                                case EAlignmentRefWidth: // aligning horizontal center of child to width of parent will very likely never been used ..
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
                                case EAlignmentRefHeight: // aligning horizontal center of child to height of parent will very likely never been used ..
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
                                case EAlignmentRefTop:      // aligning horizontal center of child to top border of parent will very likely never been used ..
                                case EAlignmentRefVCenter:  // aligning horizontal center of child to vertical center of parent will very likely never been used ..
                                case EAlignmentRefBottom:   // aligning horizontal center of child to bottom border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRefHCenter

                        case EAlignmentRefRight:
                        {
                            switch( alignment.m_alignmentRefParent )
                            {
                                case EAlignmentRefLeft:
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
                                case EAlignmentRefHCenter:
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
                                case EAlignmentRefRight:
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
                                case EAlignmentRefWidth:   // aligning right border of child to width of parent will very likely never been used ..
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
                                case EAlignmentRefHeight:   // aligning right border of child to height of parent will very likely never been used ..
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
                                case EAlignmentRefTop:      // aligning right border of child to top border of parent will very likely never been used ..
                                case EAlignmentRefVCenter:  // aligning right border of child to vertical center of parent will very likely never been used ..
                                case EAlignmentRefBottom:   // aligning right border of child to bottom border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRefRight

                        case EAlignmentRefTop:
                        {
                            switch( alignment.m_alignmentRefParent )
                            {
                                case EAlignmentRefTop:
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
                                case EAlignmentRefVCenter:
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
                                case EAlignmentRefBottom:
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
                                case EAlignmentRefWidth: // aligning top border of child to width of parent will very likely never been used ..
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
                                case EAlignmentRefHeight: // aligning top border of child to height of parent will very likely never been used ..
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
                                case EAlignmentRefLeft:     // aligning top border of child to left border of parent will very likely never been used ..
                                case EAlignmentRefHCenter:  // aligning top border of child to horizontal center of parent will very likely never been used ..
                                case EAlignmentRefRight:    // aligning top border of child to right border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRefTop

                        case EAlignmentRefVCenter:
                        {
                            switch( alignment.m_alignmentRefParent )
                            {
                                case EAlignmentRefTop:
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
                                case EAlignmentRefVCenter:
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
                                case EAlignmentRefBottom:
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
                                case EAlignmentRefWidth: // aligning vertical center of child to width of parent will very likely never been used ..
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
                                case EAlignmentRefHeight: // aligning vertical center of child to height of parent will very likely never been used ..
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
                                case EAlignmentRefLeft:     // aligning vertical center of child to left border of parent will very likely never been used ..
                                case EAlignmentRefHCenter:  // aligning vertical center of child to horizontal center of parent will very likely never been used ..
                                case EAlignmentRefRight:    // aligning vertical center of child to right border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRefVCenter

                        case EAlignmentRefBottom:
                        {
                            switch( alignment.m_alignmentRefParent )
                            {
                                case EAlignmentRefTop:
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
                                case EAlignmentRefVCenter:
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
                                case EAlignmentRefBottom:
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
                                case EAlignmentRefWidth: // aligning bottom border of child to width of parent will very likely never been used ..
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
                                case EAlignmentRefHeight: // aligning bottom border of child to height of parent will very likely never been used ..
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
                                case EAlignmentRefLeft:     // aligning bottom border of child to left border of parent will very likely never been used ..
                                case EAlignmentRefHCenter:  // aligning bottom border of child to horizontal center of parent will very likely never been used ..
                                case EAlignmentRefRight:    // aligning bottom border of child to right border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRefBottom

                        case EAlignmentRefWidth:
                        {
                            switch( alignment.m_alignmentRefParent )
                            {
                                case EAlignmentRefWidth:
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
                                case EAlignmentRefHeight: // aligning width of child to height of parent will very likely never been used ..
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
                                case EAlignmentRefLeft:     // aligning width of child to left border of parent will very likely never been used ..
                                case EAlignmentRefHCenter:  // aligning width of child to horizontal center of parent will very likely never been used ..
                                case EAlignmentRefRight:    // aligning width of child to right border of parent will very likely never been used ..
                                case EAlignmentRefTop:      // aligning width of child to top border of parent will very likely never been used ..
                                case EAlignmentRefVCenter:  // aligning width of child to vertical center of parent will very likely never been used ..
                                case EAlignmentRefBottom:   // aligning width of child to bottom border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRefWidth

                        case EAlignmentRefHeight:
                        {
                            switch( alignment.m_alignmentRefParent )
                            {
                                case EAlignmentRefWidth: // aligning height of child to width of parent will very likely never been used ..
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
                                case EAlignmentRefHeight:
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
                                case EAlignmentRefLeft:     // aligning height of child to left border of parent will very likely never been used ..
                                case EAlignmentRefHCenter:  // aligning height of child to horizontal center of parent will very likely never been used ..
                                case EAlignmentRefRight:    // aligning height of child to right border of parent will very likely never been used ..
                                case EAlignmentRefTop:      // aligning height of child to top border of parent will very likely never been used ..
                                case EAlignmentRefVCenter:  // aligning height of child to vertical center of parent will very likely never been used ..
                                case EAlignmentRefBottom:   // aligning height of child to bottom border of parent will very likely never been used ..
                                default:
                                {
                                    break;
                                }
                            } // switch( alignment.m_alignmentRefParent )
                            break;
                        } // case m_alignmentRefChild == EAlignmentRefHeight

                        default:
                        {
                            break;
                        }

                    } // switch( alignment.m_alignmentRefChild )

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
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Hit:" + bool2Str(i_bHit);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setIsHit",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Point:" + point2Str(i_pt);
        strAddTrcInfo += ", Rect(x,y,w,h):" + rect2Str(m_rctCurr);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "isHit",
        /* strAddInfo   */ strAddTrcInfo );

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
            bIsHit = isRectHit( m_rctCurr, EFillStyleSolidPattern, i_pt, m_pDrawingScene->getHitToleranceInPx(), o_pHitInfo );
        }

        if( !bIsHit )
        {
            if( pGraphicsItem->isSelected() || m_drawSettings.getFillStyle() == EFillStyleSolidPattern )
            {
                bIsHit = pGraphicsItem->contains(i_pt);

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditModeMove;
                    o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
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
        strAddTrcInfo = "IsHit:" + bool2Str(bIsHit);

        if( o_pHitInfo != nullptr )
        {
            strAddTrcInfo += ", EditMode:" + editMode2Str(o_pHitInfo->m_editMode);
            strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(o_pHitInfo->m_editResizeMode);
            strAddTrcInfo += ", SelPtBoundingRect:" + selectionPoint2Str(o_pHitInfo->m_selPtBoundingRect);
            strAddTrcInfo += ", PolygonShapePoint:" + QString::number(o_pHitInfo->m_idxPolygonShapePoint);
            strAddTrcInfo += ", LineSegment:" + QString::number(o_pHitInfo->m_idxLineSegment);
            strAddTrcInfo += ", PointSelected:" + point2Str(o_pHitInfo->m_ptSelected);
            strAddTrcInfo += ", Cursor:" + qCursorShape2Str(o_pHitInfo->m_cursor.shape());
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return bIsHit;

} // isHit

/*==============================================================================
protected: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::showSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "SelectionPoints:" + selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "showSelectionPoints",
        /* strAddInfo   */ strAddTrcInfo );

    showSelectionPointsOfBoundingRect( m_rctCurr, i_selPts );

} // showSelectionPoints

//------------------------------------------------------------------------------
void CGraphObjGroup::updateSelectionPoints( unsigned char i_selPts )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "SelectionPoints:" + selectionPoints2Str(i_selPts);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "updateSelectionPoints",
        /* strAddInfo   */ strAddTrcInfo );

    updateSelectionPointsOfBoundingRect( m_rctCurr, i_selPts );

} // updateSelectionPoints

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::onParentItemCoorsHasChanged( CGraphObj* /*i_pGraphObjParent*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onParentItemCoorsHasChanged",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( s_pTrcAdminObjBoundingRect != nullptr && s_pTrcAdminObjBoundingRect->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjBoundingRect,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "boundingRect",
        /* strAddInfo   */ strAddTrcInfo );

    QRectF rctBounding = childrenBoundingRect();

    CGraphObjSelectionPoint* pGraphObjSelPt;
    QRectF                   rctSelPt;
    QPolygonF                plgSelPt;
    int                      idxSelPt;

    for( idxSelPt = 0; idxSelPt < ESelectionPointCount; idxSelPt++ )
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

    if( m_bIsHit || isSelected() )
    {
        QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
        SGraphObjLabel* pGraphObjLabel;
        QRectF          rctLabel;
        QPolygonF       plgLabel;

        while( itLabels.hasNext() )
        {
            itLabels.next();
            pGraphObjLabel = itLabels.value();

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                // Calling pGraphObjLabel->boundingRect() may lead to endless recursion as the
                // label itself may call the boundingRect method of its parent item (which is
                // this item) if the label is selected or has been hit. For this we call
                // boundingRect of the label with a different signature to indicate that we
                // are only interested in the bounding rect of the simple text item.
                rctLabel = pGraphObjLabel->m_pGraphObjLabel->boundingRect(true);
                plgLabel = mapFromItem( pGraphObjLabel->m_pGraphObjLabel, rctLabel );
                rctBounding |= plgLabel.boundingRect();
            }
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Rect(x,y,w,h):(";
        strAddTrcInfo += QString::number(rctBounding.x(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.y(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.width(),'f',1);
        strAddTrcInfo += "," + QString::number(rctBounding.height(),'f',1) + ")";
        mthTracer.setMethodReturn(strAddTrcInfo);
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
    QString strAddTrcInfo;

    if( s_pTrcAdminObjPaint != nullptr && s_pTrcAdminObjPaint->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjPaint,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "paint",
        /* strAddInfo   */ strAddTrcInfo );

    i_pPainter->save();

    if( m_pDrawingScene->getMode() == EMode::Edit && (m_bIsHit || isSelected()) )
    {
        QPen pn(Qt::DotLine);
        pn.setColor(Qt::blue);

        i_pPainter->setPen(pn);
        i_pPainter->setBrush(Qt::NoBrush);

        i_pPainter->drawRect(m_rctCurr);

        if( isSelected() )
        {
            if( m_arpSelPtsBoundingRect[ESelectionPointTopCenter] != nullptr && m_arpSelPtsBoundingRect[ESelectionPointRotateTop] != nullptr )
            {
                CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[ESelectionPointTopCenter];
                CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[ESelectionPointRotateTop];

                QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                QPointF ptRctM = mapFromScene(ptRct);
                QPointF ptRotM = mapFromScene(ptRot);

                i_pPainter->drawLine( ptRctM, ptRotM );
            }

            if( m_arpSelPtsBoundingRect[ESelectionPointBottomCenter] != nullptr && m_arpSelPtsBoundingRect[ESelectionPointRotateBottom] != nullptr )
            {
                CGraphObjSelectionPoint* pGraphObjSelPtRct = m_arpSelPtsBoundingRect[ESelectionPointBottomCenter];
                CGraphObjSelectionPoint* pGraphObjSelPtRot = m_arpSelPtsBoundingRect[ESelectionPointRotateBottom];

                QPointF ptRct = QPointF( pGraphObjSelPtRct->scenePos().x(), pGraphObjSelPtRct->scenePos().y() );
                QPointF ptRot = QPointF( pGraphObjSelPtRot->scenePos().x(), pGraphObjSelPtRot->scenePos().y() );

                QPointF ptRctM = mapFromScene(ptRct);
                QPointF ptRotM = mapFromScene(ptRot);

                i_pPainter->drawLine( ptRctM, ptRotM );
            }
        } // if( isSelected() )

        QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);
        SGraphObjLabel*                        pGraphObjLabel;

        QPointF ptSelPt;
        QPointF ptLabelSelPt;

        while( itLabels.hasNext() )
        {
            itLabels.next();
            pGraphObjLabel = itLabels.value();

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                ptSelPt = getSelectionPoint(pGraphObjLabel->m_selPt);

                ptLabelSelPt = pGraphObjLabel->m_pGraphObjLabel->getSelectionPoint(ESelectionPointCenter);
                ptLabelSelPt = mapFromItem( pGraphObjLabel->m_pGraphObjLabel, ptLabelSelPt );

                i_pPainter->drawLine( ptSelPt, ptLabelSelPt );
            }
        }

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

    QString strAddTrcInfo;

    if( s_pTrcAdminObjSceneEventFilter != nullptr && s_pTrcAdminObjSceneEventFilter->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "ItemWatched:" + pGraphObjSelPtWatched->getObjName();
        strAddTrcInfo += ", Event:" + qEventType2Str(i_pEv->type());
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjSceneEventFilter,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "sceneEventFilter",
        /* strAddInfo   */ strAddTrcInfo );

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
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
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
    QString strAddTrcInfo;

    if( s_pTrcAdminObjHoverEvents != nullptr && s_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "hoverEnterEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

    EMode     modeDrawing     = m_pDrawingScene->getMode();
    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit && editToolDrawing == EEditToolSelect )
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

    } // if( modeDrawing == EMode::Edit && editToolDrawing == EEditToolSelect )

} // hoverEnterEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjHoverEvents != nullptr && s_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ 1,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "hoverMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    QPointF ptItemPos = mapFromScene(i_pEv->scenePos());

    EMode     modeDrawing     = m_pDrawingScene->getMode();
    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit && editToolDrawing == EEditToolSelect )
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

    } // if( modeDrawing == EMode::Edit && editToolDrawing == EEditToolSelect )

} // hoverMoveEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjHoverEvents != nullptr && s_pTrcAdminObjHoverEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjHoverEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "hoverLeaveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    unsetCursor();

} // hoverLeaveEvent

/*==============================================================================
protected: // overridables of base class QGraphicsItem
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjGroup::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    EMode     modeDrawing = m_pDrawingScene->getMode();
    EEditTool editToolDrawing = m_pDrawingScene->getEditTool();

    if( modeDrawing == EMode::Edit )
    {
        if( editToolDrawing == EEditToolSelect && m_editMode == EEditModeUndefined )
        {
            QGraphicsItemGroup::mousePressEvent(i_pEv); // this will select the item (creating selection points)

            CGraphObjSelectionPoint* pGraphObjSelPt;
            ESelectionPoint          selPt;
            int                      idxSelPt;

            SGraphObjHitInfo hitInfo;

            //bool bIsHit = isHit( ptMouseItemPos, &hitInfo );

            m_editMode                  = hitInfo.m_editMode;
            m_editResizeMode            = hitInfo.m_editResizeMode;
            m_selPtSelectedBoundingRect = hitInfo.m_selPtBoundingRect;

            for( idxSelPt = 0; idxSelPt < ESelectionPointCount; idxSelPt++ )
            {
                selPt = static_cast<ESelectionPoint>(idxSelPt);

                pGraphObjSelPt = m_arpSelPtsBoundingRect[selPt];

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

            m_pDrawingScene->setMode( EMode::Ignore, EEditToolIgnore, m_editMode, m_editResizeMode, false );

            updateEditInfo();
            updateToolTip();

        } // if( editToolDrawing == EEditToolSelect && m_editMode == EEditModeUndefined )

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
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ 2,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "mouseMoveEvent",
        /* strAddInfo   */ strAddTrcInfo );

    EMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        // Mouse events may first be dispatched to childrens of the group. Mapping the mouse
        // coordinates from the childrens to the parent's (the group's) coordinate system
        // does not work for succeeding mouse move events. So the mouse item pos returned
        // by "i_pEv->pos()" is not correct here and we need to map the mouse scene event
        // pos to the group's mouse item pos.

        if( m_editMode == EEditModeCreating )
        {
        } // if( m_editMode == EEditModeCreating )

        else if( m_editMode == EEditModeMove )
        {
            QGraphicsItemGroup::mouseMoveEvent(i_pEv);

        } // if( m_editMode == EEditModeMove )

        else if( m_editMode == EEditModeResize )
        {
            QPointF ptMouseItemPos = i_pEv->pos();

            m_rctCurr = resizeRect( m_rctOnMousePressEvent, m_selPtSelectedBoundingRect, ptMouseItemPos, nullptr );

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

        } // if( m_editMode == EEditModeResize )

        else if( m_editMode == EEditModeRotate )
        {
            QPointF ptMouseScenePos = i_pEv->scenePos(); // see comment above

            double fRotAngle_rad = getAngleRad( m_ptRotOriginOnMousePressEvent, ptMouseScenePos );

            m_fRotAngleCurr_deg = Math::rad2Deg(fRotAngle_rad);

            switch( m_selPtSelectedBoundingRect )
            {
                case ESelectionPointRotateTop:
                {
                    m_fRotAngleCurr_deg -= 90.0;
                    break;
                }
                case ESelectionPointRotateBottom:
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

        } // if( m_editMode == EEditModeRotate )

        else if( m_editMode == EEditModeMoveShapePoint )
        {
        } // if( m_editMode == EEditModeMoveShapePoint )

        else if( m_editMode == EEditModeEditText )
        {
        } // if( m_editMode == EEditModeEditText )

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
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseMoveEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    EMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Edit )
    {
        if( m_editMode == EEditModeCreating )
        {
            // Groups are not created by mouse events.

        } // if( m_editMode == EEditModeCreating )

        else if( m_editMode == EEditModeMove )
        {
        } // if( m_editMode == EEditModeMove )

        else if( m_editMode == EEditModeResize )
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

        } // if( m_editMode == EEditModeResize )

        else if( m_editMode == EEditModeRotate )
        {
        } // if( m_editMode == EEditModeRotate )

        else if( m_editMode == EEditModeMoveShapePoint )
        {
            // Groups don't have movable shape points.

        } // if( m_editMode == EEditModeMoveShapePoint )

        m_editMode = EEditModeUndefined;
        m_editResizeMode = EEditResizeModeUndefined;
        m_idxSelPtSelectedPolygon = -1;
        m_selPtSelectedBoundingRect = ESelectionPointUndefined;

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

    if( bIsSelectable && m_pDrawingScene->getEditTool() != EEditToolSelect )
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
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        mthTracer.trace(strAddTrcInfo);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjMouseEvents != nullptr && s_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strAddTrcInfo );

    // When double clicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // double click event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".

    EMode modeDrawing = m_pDrawingScene->getMode();

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
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
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
    QString strAddTrcInfo;

    if( s_pTrcAdminObjKeyEvents != nullptr && s_pTrcAdminObjKeyEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Key:" + qKeyCode2Str(i_pEv->key());
        strAddTrcInfo += ", Ev.Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    EMode modeDrawing = m_pDrawingScene->getMode();

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
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        mthTracer.trace(strAddTrcInfo);
    }

} // keyPressEvent

//------------------------------------------------------------------------------
void CGraphObjGroup::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjKeyEvents != nullptr && s_pTrcAdminObjKeyEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Key:" + qKeyCode2Str(i_pEv->key());
        strAddTrcInfo += ", Ev.Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    EMode modeDrawing = m_pDrawingScene->getMode();

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
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
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

    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Changed:" + graphicsItemChange2Str(i_change);

        if( i_value.type() == QVariant::PointF )
        {
            strAddTrcInfo += ", Value(" + qVariantType2Str(i_value.type()) + "):" + point2Str(i_value.toPointF());
        }
        else
        {
            strAddTrcInfo += ", Value(" + qVariantType2Str(i_value.type()) + "):" + i_value.toString();
        }
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "itemChange",
        /* strAddInfo   */ strAddTrcInfo );

    QVariant valChanged = i_value;

    if( i_change == ItemSelectedHasChanged )
    {
        prepareGeometryChange();

        if( m_pDrawingScene->getMode() == EMode::Edit && isSelected() )
        {
            bringToFront(); // does not set "m_fZValue" as it is used to restore the stacking order on deselecting the object

            if( m_editMode == EEditModeCreating )
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

            m_editMode = EEditModeUndefined;
            m_editResizeMode = EEditResizeModeUndefined;
            m_selPtSelectedBoundingRect = ESelectionPointUndefined;
            m_idxSelPtSelectedPolygon = -1;

        } // if( !isSelected() )

        updateEditInfo();
        updateToolTip();

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

        //for( idxSelPt = 0; idxSelPt < ESelectionPointCount; idxSelPt++ )
        //{
        //    selPt = static_cast<ESelectionPoint>(idxSelPt);

        //    pGraphObjSelPt = m_arpSelPtsBoundingRect[idxSelPt];

        //    if( pGraphObjSelPt != nullptr )
        //    {
        //        if( idxSelPt == ESelectionPointRotateTop && m_fScaleFacYCurr != 0.0 )
        //        {
        //            ptSel = getSelectionPoint(m_rctCurr,ESelectionPointTopCenter);
        //            ptSel.setY( ptSel.y() - getSelectionPointRotateDistance()/m_fScaleFacYCurr );
        //            ptSel = mapToScene(ptSel);
        //        }
        //        else if( idxSelPt == ESelectionPointRotateBottom && m_fScaleFacYCurr != 0.0 )
        //        {
        //            ptSel = getSelectionPoint(m_rctCurr,ESelectionPointBottomCenter);
        //            ptSel.setY( ptSel.y() + getSelectionPointRotateDistance()/m_fScaleFacYCurr );
        //            ptSel = mapToScene(ptSel);
        //        }
        //        else
        //        {
        //            ptSel = getSelectionPoint(m_rctCurr,selPt);
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

        for( idxSelPt = 0; idxSelPt < ESelectionPointCount; idxSelPt++ )
        {
            selPt = static_cast<ESelectionPoint>(idxSelPt);

            pGraphObjSelPt = m_arpSelPtsBoundingRect[selPt];

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

        QHashIterator<QString,SGraphObjLabel*> itLabels(m_arpLabels);

        SGraphObjLabel* pGraphObjLabel;

        while( itLabels.hasNext() )
        {
            itLabels.next();

            pGraphObjLabel = itLabels.value();

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                pGraphObjLabel->m_pGraphObjLabel->setZValue( zValue() + 0.02 );
            }
        }
    }


    valChanged = QGraphicsItem::itemChange(i_change,i_value);

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_value.type() == QVariant::PointF )
        {
            strAddTrcInfo = "ValChanged(" + qVariantType2Str(valChanged.type()) + "):" + point2Str(valChanged.toPointF());
        }
        else
        {
            strAddTrcInfo = "ValChanged(" + qVariantType2Str(valChanged.type()) + "):" + valChanged.toString();
        }
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return valChanged;

} // itemChange
