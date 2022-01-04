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

#include "ZSDrawGraphObjElectricityInductor.h"
#include "ZSDrawWdgtFormatGraphObjsElectricityInductor.h"

#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawGraphObjPolyline.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;
using namespace ZS::PhysVal;
using namespace ZS::Trace;


/*******************************************************************************
class CGraphObjInductor : public CGraphObjElectricity
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QSize   CGraphObjInductor::c_sizInitial(42.0,10.0);
const QString CGraphObjInductor::c_strKeyLabelInductance = "Inductance";

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjInductor::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjInductor::CGraphObjInductor(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName ) :
//------------------------------------------------------------------------------
    CGraphObjElectricity(
        /* pDrawingScene */ i_pDrawingScene,
        /* strType       */ "Inductor",
        /* strObjName    */ i_strObjName.isEmpty() ? "L" + QString::number(s_iInstCount) : i_strObjName,
        /* drawSettings  */ i_drawSettings ),
    m_pLinBody(nullptr),
    m_pCnctPt1(nullptr),
    m_pCnctPt2(nullptr),
    m_fInductance_H(1.0)
{
    s_iInstCount++;

    QString strAddTrcInfo;

    QString strNameSpace = NameSpace() + CObjFactory::c_strGroupSeparater + c_strFactoryGroupName;
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
        /* strAddInfo   */ strAddTrcInfo );

    setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges );

    /*
    CnctPt1        Body       CnctPt2
                  |       |
    +--+         | |     | |     +--+
    |  |----|   |   |   |   |----|  |  <-- Line and Body as Polyline
    +--+     | |     | |         +--+
              |       |
    */

    // Please note that before adding graphic items to groups they must have
    // been added to the drawing scene. Otherwise their coordinates will not
    // be mapped correctly to the new parent.

    // Before adding the items to the drawing scene they will be created in their
    // own coordinate system starting at (0.0/0.0). After adding the items to the
    // drawing scene they will be moved by "setPos" relative within the group.
    // The group itself will be positioned by the caller of the ctor relative to
    // the caller's coordinate system.

    // The alignments will be adjusted in the order they are added. The order
    // takes effect on the result. Usually the size should be adjusted before
    // the positions to get relative adjustments working as expected.

    QRectF rctBounding( QPointF(0.0,0.0), c_sizInitial );
    double fCnctPtWidth = 5.0;
    double fBodyWidth = 18.0;
    QRectF rctBody( rctBounding.center().x()-fBodyWidth/2.0, rctBounding.top(), fBodyWidth, rctBounding.height() );
    QRectF rctCnctPt1( rctBounding.left(), rctBounding.center().y()-fCnctPtWidth/2.0, fCnctPtWidth, fCnctPtWidth );
    QRectF rctCnctPt2( rctBounding.right()-fCnctPtWidth, rctBounding.center().y()-fCnctPtWidth/2.0, fCnctPtWidth, fCnctPtWidth );

    SGraphObjAlignment alignment;
    CDrawSettings      drawSettings = m_drawSettings;

    // Draw settings for group item
    //-----------------------------

    m_drawSettings.setAttributesAreUsed( true, EDrawAttributePenMin, EDrawAttributePenCount );
    m_drawSettings.setAttributesAreUsed( true, EDrawAttributeLineStyleMin, EDrawAttributeLineStyleCount );

    m_drawSettings.setPenColor(Qt::black);
    m_drawSettings.setPenWidth(1);
    m_drawSettings.setLineStyle(ELineStyle::SolidLine);

    // Draw settings for elements
    //---------------------------

    CDrawSettings drawSettingsLine(EGraphObjTypeLine);

    drawSettingsLine.setAttributesAreUsed( false, EDrawAttributeLineRecordTypeMin, EDrawAttributeLineRecordTypeCount );
    drawSettingsLine.setAttributesAreUsed( false, EDrawAttributeLineEndStyleMin, EDrawAttributeLineEndStyleCount );

    CDrawSettings drawSettingsCnctPt(EGraphObjTypeConnectionPoint);

    // Connection Line and Body
    //-------------------------

    QPolygonF plgBody;

    plgBody.append( QPointF(0.0,rctBounding.center().y()) );
    plgBody.append( QPointF(rctBody.left(),rctBounding.center().y()) );
    plgBody.append( QPointF(rctBody.left()+1.0*rctBody.width()/8.0,rctBounding.bottom()) );
    plgBody.append( QPointF(rctBody.left()+3.0*rctBody.width()/8.0,rctBounding.top()) );
    plgBody.append( QPointF(rctBody.left()+5.0*rctBody.width()/8.0,rctBounding.bottom()) );
    plgBody.append( QPointF(rctBody.left()+7.0*rctBody.width()/8.0,rctBounding.top()) );
    plgBody.append( QPointF(rctBody.right(),rctBounding.center().y()) );
    plgBody.append( QPointF(rctBounding.right(),rctBounding.center().y()) );

    m_pLinBody = new CGraphObjPolyline(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLine,
        /* strObjName    */ "CnctLine" );

    m_pLinBody->setPolygon(plgBody);
    m_pDrawingScene->addItem(m_pLinBody);
    m_pDrawingScene->onGraphObjCreated(m_pLinBody);
    m_pLinBody->setPos( QPointF(0.0,0.0) );
    addGraphObj(m_pLinBody);

    // Connection Point 1
    //-------------------

    m_pCnctPt1 = new CGraphObjConnectionPoint(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsCnctPt,
        /* strObjName    */ "CnctPt1" );

    m_pCnctPt1->setWidth(fCnctPtWidth);
    m_pCnctPt1->setInnerCircleWidthInPx(fCnctPtWidth);
    m_pCnctPt1->setRect( 0.0, 0.0, rctCnctPt1.width(), rctCnctPt1.height() );
    m_pCnctPt1->setFixedSize( QSize(fCnctPtWidth,fCnctPtWidth) );
    m_pDrawingScene->addItem(m_pCnctPt1);
    m_pDrawingScene->onGraphObjCreated(m_pCnctPt1);
    m_pCnctPt1->setPos( rctCnctPt1.topLeft() );
    addGraphObj(m_pCnctPt1);

    alignment = SGraphObjAlignment( EAlignmentRef::Left, EAlignmentRef::Left, true, 0.0 );
    m_pCnctPt1->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::VCenter, EAlignmentRef::VCenter, true, 0.0 );
    m_pCnctPt1->addAlignment(alignment);

    // Connection Point 2
    //-------------------

    m_pCnctPt2 = new CGraphObjConnectionPoint(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsCnctPt,
        /* strObjName    */ "CnctPt2" );

    m_pCnctPt2->setWidth(fCnctPtWidth);
    m_pCnctPt2->setInnerCircleWidthInPx(fCnctPtWidth);
    m_pCnctPt2->setRect( 0.0, 0.0, rctCnctPt2.width(), rctCnctPt2.height() );
    m_pCnctPt2->setFixedSize( QSize(fCnctPtWidth,fCnctPtWidth) );
    m_pDrawingScene->addItem(m_pCnctPt2);
    m_pDrawingScene->onGraphObjCreated(m_pCnctPt2);
    m_pCnctPt2->setPos( rctCnctPt2.topLeft() );
    addGraphObj(m_pCnctPt2);

    alignment = SGraphObjAlignment( EAlignmentRef::Right, EAlignmentRef::Right, true, 0.0 );
    m_pCnctPt2->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::VCenter, EAlignmentRef::VCenter, true, 0.0 );
    m_pCnctPt2->addAlignment(alignment);

    // Update group coordinates
    //-------------------------

    m_rctCurr = rctBounding;

    m_ptRotOriginCurr = m_rctCurr.center();

    acceptCurrentAsOriginalCoors();

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

//------------------------------------------------------------------------------
CGraphObjInductor::~CGraphObjInductor()
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
    // Moreover on removing (deleting) a group the group's children have already
    // been removed from the drawing scene by the dtor of class QGraphicsItemGroup
    // (which is inherited by CGraphObjGroup) and "scene()" may return nullptr.

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        QGraphicsScene* pGraphicsScene = pGraphicsItem->scene();

        if( pGraphicsScene != nullptr )
        {
            pGraphicsScene->removeItem(pGraphicsItem);
        }
    }

    m_pLinBody = nullptr;
    m_pCnctPt1 = nullptr;
    m_pCnctPt2 = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjInductor::setInductance( double i_fInductance_H )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Inductance:" + QString::number(i_fInductance_H) + " H";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setInductance",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_fInductance_H != i_fInductance_H )
    {
        m_fInductance_H = i_fInductance_H;

        if( m_arpLabels.contains(c_strKeyLabelInductance) )
        {
            SGraphObjLabel* pGraphObjLabel = m_arpLabels[c_strKeyLabelInductance];

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                QString str = QString::number(m_fInductance_H) + "H";
                pGraphObjLabel->m_pGraphObjLabel->setText(str);
            }
        }

    } // if( m_fInductance_H != i_fInductance_H )

} // setInductance

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjInductor::showInductance( ESelectionPoint i_selPtPos )
//------------------------------------------------------------------------------
{
    QString str = QString::number(m_fInductance_H) + "H";

    if( !m_arpLabels.contains(c_strKeyLabelInductance) )
    {
        addLabel(c_strKeyLabelInductance,str,i_selPtPos);
    }
    else
    {
        setLabelText(c_strKeyLabelInductance,str);
    }

    showLabel(c_strKeyLabelInductance);

} // showInductance

//------------------------------------------------------------------------------
void CGraphObjInductor::hideInductance()
//------------------------------------------------------------------------------
{
    hideLabel(c_strKeyLabelInductance);
}

//------------------------------------------------------------------------------
bool CGraphObjInductor::isInductanceVisible() const
//------------------------------------------------------------------------------
{
    return isLabelVisible(c_strKeyLabelInductance);
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjInductor::clone()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjCtorsAndDtor != nullptr && m_pTrcAdminObjCtorsAndDtor->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "clone",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjInductor* pGraphObj = nullptr;

    return pGraphObj;

} // clone

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjInductor::onCreateAndExecDlgFormatGraphObjs()
//------------------------------------------------------------------------------
{
    CDlgFormatGraphObjs* pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene,this);

    QIcon   icon;
    QPixmap pxm(":/ZS/Draw/Electricity/Inductor16x16.bmp");

    pxm.setMask(pxm.createHeuristicMask());
    icon.addPixmap(pxm);

    CWdgtFormatGraphObjsInductor* pWdgt = new CWdgtFormatGraphObjsInductor(m_pDrawingScene,this);

    pDlgFormatGraphObjs->addWidget( icon, "Inductor", pWdgt );

    pDlgFormatGraphObjs->setCurrentWidget("Inductor");

    pDlgFormatGraphObjs->exec();

    delete pDlgFormatGraphObjs;
    pDlgFormatGraphObjs = nullptr;

} // onCreateAndExecDlgFormatGraphObjs

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjInductor::onDrawSettingsChanged()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    CDrawSettings drawSettingsLine = m_pLinBody->getDrawSettings();

    drawSettingsLine.setPenColor(m_drawSettings.getPenColor());
    drawSettingsLine.setPenWidth(m_drawSettings.getPenWidth());
    drawSettingsLine.setLineStyle(m_drawSettings.getLineStyle());

    m_pLinBody->setDrawSettings(drawSettingsLine);

    //CDrawSettings drawSettingsCnctPt = m_pCnctPt1->getDrawSettings();

    //drawSettingsCnctPt.setPenColor(m_drawSettings.getPenColor());
    //drawSettingsCnctPt.setPenWidth(m_drawSettings.getPenWidth());
    //drawSettingsCnctPt.setLineStyle(m_drawSettings.getLineStyle());
    //drawSettingsCnctPt.setFillColor(m_drawSettings.getFillColor()); keep black
    //drawSettingsCnctPt.setFillStyle(m_drawSettings.getFillStyle()); keep SolidPattern

    //m_pCnctPt1->setDrawSettings(drawSettingsCnctPt);
    //m_pCnctPt2->setDrawSettings(drawSettingsCnctPt);

} // onDrawSettingsChanged

/*==============================================================================
protected: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjInductor::updateToolTip()
//------------------------------------------------------------------------------
{
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        QString strNodeSeparator = m_pDrawingScene->getGraphObjNameNodeSeparator();
        QPointF ptPos;

        m_strToolTip  = "ObjName:\t" + name();
        m_strToolTip += "\nObjId:\t\t" + keyInTree();

        m_strToolTip += "Inductance:\t" + QString::number(m_fInductance_H) + " H";

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

        m_strToolTip += "\nSize:\t\t" + size2Str(getSize());
        m_strToolTip += "\nRotation:\t" + QString::number(m_fRotAngleCurr_deg,'f',1) + "�";
        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());

        pGraphicsItem->setToolTip(m_strToolTip);

    } // if( pGraphicsItem != nullptr )

} // updateToolTip