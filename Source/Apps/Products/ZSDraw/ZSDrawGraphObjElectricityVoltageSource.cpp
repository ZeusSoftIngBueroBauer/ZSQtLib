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

#include "ZSDrawGraphObjElectricityVoltageSource.h"
#include "ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.h"

#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/ZSDrawGraphObjEllipse.h"
#include "ZSDraw/ZSDrawGraphObjLine.h"
#include "ZSDraw/ZSDrawGraphObjRect.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawDlgFormatGraphObjs.h"
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
using namespace ZS::Draw::Electricity;
using namespace ZS::PhysVal;
using namespace ZS::Trace;


/*******************************************************************************
class CGraphObjVoltageSource : public CGraphObjElectricity
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QSize   CGraphObjVoltageSource::c_sizInitial(36.0,16.0);
const QString CGraphObjVoltageSource::c_strKeyLabelVoltage = "Voltage";

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjVoltageSource::ResetCtorsDtorsCounters()
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

qint64 CGraphObjVoltageSource::s_iCtorsCount = 1;
qint64 CGraphObjVoltageSource::s_iDtorsCount = 1;

CTrcAdminObj* CGraphObjVoltageSource::s_pTrcAdminObjCtorsAndDtor = nullptr;
CTrcAdminObj* CGraphObjVoltageSource::s_pTrcAdminObjItemChange = nullptr;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjVoltageSource::CGraphObjVoltageSource(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName,
    const QString&       i_strObjId ) :
//------------------------------------------------------------------------------
    CGraphObjElectricity(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ "ZS::Draw::Electricity",
        /* strClassName  */ "CGraphObjVoltageSource",
        /* strType       */ "VoltageSource",
        /* strObjName    */ i_strObjName.isEmpty() ? "V" + QString::number(s_iCtorsCount) : i_strObjName,
        /* strObjId      */ i_strObjId.isEmpty() ? "V" + QString::number(s_iCtorsCount) : i_strObjId,
        /* drawSettings  */ i_drawSettings ),
    m_pLinCnct(nullptr),
    m_pEllBody(nullptr),
    m_pLinSignMinus(nullptr),
    m_pLinSignPlusHor(nullptr),
    m_pLinSignPlusVer(nullptr),
    m_pCnctPt1(nullptr),
    m_pCnctPt2(nullptr),
    m_fVoltage_V(1.0)
{
    s_iCtorsCount++;

    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor == nullptr )
    {
        s_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj("ZS::Draw::Electricity", "CGraphObjVoltageSource::CtorsAndDtor", "");
        s_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjVoltageSource::ItemChange", "");
    }

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

    /*
    CnctPt1  Body   CnctPt2
              +-+
    +--+    +     +    +--+
    |  |---+ -   + +---|  |  <-- CnctLine (hidden below body)
    +--+    +     +    +--+
              +-+
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

    QRectF  rctBounding( QPointF(0.0,0.0), c_sizInitial );
    double  fCnctPtWidth = 5.0;
    double  fSignWidth = 4.0;
    double  fBodyWidth = rctBounding.height();
    QRectF  rctBody( rctBounding.center().x()-fBodyWidth/2.0, rctBounding.top(), fBodyWidth, rctBounding.height() );
    QLineF  linSignMinus( QPointF(0.0,0.0), QPointF(fSignWidth,0.0) );
    QLineF  linSignPlusHor( QPointF(0.0,0.0), QPointF(fSignWidth,0.0) );
    QLineF  linSignPlusVer( QPointF(0.0,0.0), QPointF(0.0,fSignWidth) );
    QPointF ptPosLinSignMinus( rctBody.left() + 2.0, rctBody.center().y() );
    QPointF ptPosLinSignPlusHor( rctBody.right() - 2.0 - fSignWidth, rctBody.center().y() );
    QPointF ptPosLinSignPlusVer( rctBody.right() - 2.0 - fSignWidth/2.0, rctBody.center().y() - fSignWidth/2.0 );
    QRectF  rctCnctPt1( rctBounding.left(), rctBounding.center().y()-fCnctPtWidth/2.0, fCnctPtWidth, fCnctPtWidth );
    QRectF  rctCnctPt2( rctBounding.right()-fCnctPtWidth, rctBounding.center().y()-fCnctPtWidth/2.0, fCnctPtWidth, fCnctPtWidth );

    SGraphObjAlignment alignment;

    // Draw settings for group item
    //-----------------------------

    m_drawSettings.setAttributesAreUsed( true, EDrawAttributePenMin, EDrawAttributePenCount );
    m_drawSettings.setAttributesAreUsed( true, EDrawAttributeLineStyleMin, EDrawAttributeLineStyleCount );
    m_drawSettings.setAttributesAreUsed( true, EDrawAttributeFillMin, EDrawAttributeFillCount );

    m_drawSettings.setPenColor(Qt::black);
    m_drawSettings.setPenWidth(1);
    m_drawSettings.setLineStyle(ELineStyleSolidLine);
    m_drawSettings.setFillColor(Qt::white);
    m_drawSettings.setFillStyle(EFillStyleSolidPattern);

    // Draw settings for elements
    //---------------------------

    CDrawSettings drawSettingsLine(EGraphObjTypeLine);

    drawSettingsLine.setAttributesAreUsed( false, EDrawAttributeLineRecordTypeMin, EDrawAttributeLineRecordTypeCount );
    drawSettingsLine.setAttributesAreUsed( false, EDrawAttributeLineEndStyleMin, EDrawAttributeLineEndStyleCount );

    CDrawSettings drawSettingsBody(EGraphObjTypeEllipse);

    drawSettingsBody.setFillStyle(EFillStyleSolidPattern);

    CDrawSettings drawSettingsCnctPt(EGraphObjTypeConnectionPoint);

    // Connection Line
    //----------------

    m_pLinCnct = new CGraphObjLine(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLine,
        /* strObjName    */ "CnctLine",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "CnctLine" );

    m_pLinCnct->setLine( QLineF( QPointF(0.0,0.0), QPointF(rctBounding.right(),0.0) ) );
    m_pDrawingScene->addItem(m_pLinCnct);
    m_pDrawingScene->onGraphObjCreated(m_pLinCnct);
    m_pLinCnct->setPos( QPointF(0.0,rctCnctPt1.center().y()) );
    addGraphObj(m_pLinCnct);

    //alignment = SGraphObjAlignment( EAlignmentRefWidth, EAlignmentRefWidth, false, 1.0 );
    //m_pLinCnct->addAlignment(alignment);
    //alignment = SGraphObjAlignment( EAlignmentRefLeft, EAlignmentRefLeft, true, 0.0 );
    //m_pLinCnct->addAlignment(alignment);
    //alignment = SGraphObjAlignment( EAlignmentRefVCenter, EAlignmentRefVCenter, true, 0.0 );
    //m_pLinCnct->addAlignment(alignment);

    // Body
    //------------------

    m_pEllBody = new CGraphObjEllipse(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsBody,
        /* strObjName    */ "Body",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "Body" );

    m_pEllBody->setRect( 0.0, 0.0, rctBody.width(), rctBody.height() );
    m_pDrawingScene->addItem(m_pEllBody);
    m_pDrawingScene->onGraphObjCreated(m_pEllBody);
    m_pEllBody->setPos( rctBody.topLeft() );
    addGraphObj(m_pEllBody);

    //alignment = SGraphObjAlignment( EAlignmentRefWidth, EAlignmentRefWidth, false, rctBody.width()/rctBounding.width() );
    //m_pEllBody->addAlignment(alignment);
    //alignment = SGraphObjAlignment( EAlignmentRefHeight, EAlignmentRefHeight, false, rctBody.height()/rctBounding.height() );
    //m_pEllBody->addAlignment(alignment);
    //alignment = SGraphObjAlignment( EAlignmentRefHCenter, EAlignmentRefHCenter, true, 0.0 );
    //m_pEllBody->addAlignment(alignment);
    //alignment = SGraphObjAlignment( EAlignmentRefVCenter, EAlignmentRefVCenter, true, 0.0 );
    //m_pEllBody->addAlignment(alignment);

    // Minus Sign
    //----------------

    m_pLinSignMinus = new CGraphObjLine(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLine,
        /* strObjName    */ "SignMinus",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "SignMinus" );

    m_pLinSignMinus->setLine(linSignMinus);
    m_pDrawingScene->addItem(m_pLinSignMinus);
    m_pDrawingScene->onGraphObjCreated(m_pLinSignMinus);
    m_pLinSignMinus->setPos(ptPosLinSignMinus);
    addGraphObj(m_pLinSignMinus);

    // Plus Sign
    //----------------

    m_pLinSignPlusHor = new CGraphObjLine(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLine,
        /* strObjName    */ "SignPlusLineHor",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "SignPlusLineHor" );

    m_pLinSignPlusHor->setLine(linSignPlusHor);
    m_pDrawingScene->addItem(m_pLinSignPlusHor);
    m_pDrawingScene->onGraphObjCreated(m_pLinSignPlusHor);
    m_pLinSignPlusHor->setPos(ptPosLinSignPlusHor);
    addGraphObj(m_pLinSignPlusHor);

    m_pLinSignPlusVer = new CGraphObjLine(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLine,
        /* strObjName    */ "SignPlusLineVer",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "SignPlusLineVer" );

    m_pLinSignPlusVer->setLine(linSignPlusVer);
    m_pDrawingScene->addItem(m_pLinSignPlusVer);
    m_pDrawingScene->onGraphObjCreated(m_pLinSignPlusVer);
    m_pLinSignPlusVer->setPos(ptPosLinSignPlusVer);
    addGraphObj(m_pLinSignPlusVer);

    // Connection Point 1
    //-------------------

    m_pCnctPt1 = new CGraphObjConnectionPoint(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsCnctPt,
        /* strObjName    */ "CnctPt1",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "CnctPt1" );

    m_pCnctPt1->setWidth(fCnctPtWidth);
    m_pCnctPt1->setInnerCircleWidthInPx(fCnctPtWidth);
    m_pCnctPt1->setRect( 0.0, 0.0, rctCnctPt1.width(), rctCnctPt1.height() );
    m_pCnctPt1->setFixedSize( QSize(fCnctPtWidth,fCnctPtWidth) );
    m_pDrawingScene->addItem(m_pCnctPt1);
    m_pDrawingScene->onGraphObjCreated(m_pCnctPt1);
    m_pCnctPt1->setPos( rctCnctPt1.topLeft() );
    addGraphObj(m_pCnctPt1);

    alignment = SGraphObjAlignment( EAlignmentRefLeft, EAlignmentRefLeft, true, 0.0 );
    m_pCnctPt1->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefVCenter, EAlignmentRefVCenter, true, 0.0 );
    m_pCnctPt1->addAlignment(alignment);

    // Connection Point 2
    //-------------------

    m_pCnctPt2 = new CGraphObjConnectionPoint(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsCnctPt,
        /* strObjName    */ "CnctPt2",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "CnctPt2" );

    m_pCnctPt2->setWidth(fCnctPtWidth);
    m_pCnctPt2->setInnerCircleWidthInPx(fCnctPtWidth);
    m_pCnctPt2->setRect( 0.0, 0.0, rctCnctPt2.width(), rctCnctPt2.height() );
    m_pCnctPt2->setFixedSize( QSize(fCnctPtWidth,fCnctPtWidth) );
    m_pDrawingScene->addItem(m_pCnctPt2);
    m_pDrawingScene->onGraphObjCreated(m_pCnctPt2);
    m_pCnctPt2->setPos( rctCnctPt2.topLeft() );
    addGraphObj(m_pCnctPt2);

    alignment = SGraphObjAlignment( EAlignmentRefRight, EAlignmentRefRight, true, 0.0 );
    m_pCnctPt2->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefVCenter, EAlignmentRefVCenter, true, 0.0 );
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
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjVoltageSource::~CGraphObjVoltageSource()
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

    if( s_iCtorsCount == s_iDtorsCount )
    {
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjCtorsAndDtor);
        s_pTrcAdminObjCtorsAndDtor = nullptr;
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjItemChange);
        s_pTrcAdminObjItemChange = nullptr;
    }

    m_pLinCnct = nullptr;
    m_pEllBody = nullptr;
    m_pLinSignMinus = nullptr;
    m_pLinSignPlusHor = nullptr;
    m_pLinSignPlusVer = nullptr;
    m_pCnctPt1 = nullptr;
    m_pCnctPt2 = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjVoltageSource::setVoltage( double i_fVoltage_V )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Voltage:" + QString::number(i_fVoltage_V) + " V";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setVoltage",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_fVoltage_V != i_fVoltage_V )
    {
        m_fVoltage_V = i_fVoltage_V;

        if( m_arpLabels.contains(c_strKeyLabelVoltage) )
        {
            SGraphObjLabel* pGraphObjLabel = m_arpLabels[c_strKeyLabelVoltage];

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                QString str = QString::number(m_fVoltage_V) + "V";
                pGraphObjLabel->m_pGraphObjLabel->setText(str);
            }
        }

    } // if( m_fVoltage_V != i_fVoltage_V )

} // setVoltage

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjVoltageSource::showVoltage( ESelectionPoint i_selPtPos )
//------------------------------------------------------------------------------
{
    QString str = QString::number(m_fVoltage_V) + "V";

    if( !m_arpLabels.contains(c_strKeyLabelVoltage) )
    {
        addLabel(c_strKeyLabelVoltage,str,i_selPtPos);
    }
    else
    {
        setLabelText(c_strKeyLabelVoltage,str);
    }

    showLabel(c_strKeyLabelVoltage);

} // showVoltage

//------------------------------------------------------------------------------
void CGraphObjVoltageSource::hideVoltage()
//------------------------------------------------------------------------------
{
    hideLabel(c_strKeyLabelVoltage);
}

//------------------------------------------------------------------------------
bool CGraphObjVoltageSource::isVoltageVisible() const
//------------------------------------------------------------------------------
{
    return isLabelVisible(c_strKeyLabelVoltage);
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjVoltageSource::clone()
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

    CGraphObjVoltageSource* pGraphObj = nullptr;

    return pGraphObj;

} // clone

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjVoltageSource::onCreateAndExecDlgFormatGraphObjs()
//------------------------------------------------------------------------------
{
    CDlgFormatGraphObjs* pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene,this);

    QIcon   icon;
    QPixmap pxm(":/ZS/Draw/Electricity/VoltageSource16x16.bmp");

    pxm.setMask(pxm.createHeuristicMask());
    icon.addPixmap(pxm);

    CWdgtFormatGraphObjsVoltageSource* pWdgt = new CWdgtFormatGraphObjsVoltageSource(m_pDrawingScene,this);

    pDlgFormatGraphObjs->addWidget( icon, "VoltageSource", pWdgt );

    pDlgFormatGraphObjs->setCurrentWidget("VoltageSource");

    pDlgFormatGraphObjs->exec();

    delete pDlgFormatGraphObjs;
    pDlgFormatGraphObjs = nullptr;

} // onCreateAndExecDlgFormatGraphObjs

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjVoltageSource::onDrawSettingsChanged()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    CDrawSettings drawSettingsLine = m_pLinCnct->getDrawSettings();

    drawSettingsLine.setPenColor(m_drawSettings.getPenColor());
    drawSettingsLine.setPenWidth(m_drawSettings.getPenWidth());
    drawSettingsLine.setLineStyle(m_drawSettings.getLineStyle());

    m_pLinCnct->setDrawSettings(drawSettingsLine);

    CDrawSettings drawSettingsBody = m_pEllBody->getDrawSettings();

    drawSettingsBody.setPenColor(m_drawSettings.getPenColor());
    drawSettingsBody.setPenWidth(m_drawSettings.getPenWidth());
    drawSettingsBody.setLineStyle(m_drawSettings.getLineStyle());
    drawSettingsBody.setFillColor(m_drawSettings.getFillColor());
    drawSettingsBody.setFillStyle(m_drawSettings.getFillStyle());

    m_pEllBody->setDrawSettings(drawSettingsBody);

    m_pLinSignMinus->setDrawSettings(drawSettingsLine);
    m_pLinSignPlusHor->setDrawSettings(drawSettingsLine);
    m_pLinSignPlusVer->setDrawSettings(drawSettingsLine);

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
void CGraphObjVoltageSource::updateToolTip()
//------------------------------------------------------------------------------
{
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        QString strNodeSeparator = m_pDrawingScene->getGraphObjNameNodeSeparator();
        QPointF ptPos;

        m_strToolTip  = "ObjName:\t" + getObjName(true,strNodeSeparator);
        m_strToolTip += "\nObjId:\t\t" + getObjId();

        m_strToolTip += "Voltage:\t" + QString::number(m_fVoltage_V) + " V";

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
        m_strToolTip += "\nRotation:\t" + QString::number(m_fRotAngleCurr_deg,'f',1) + "°";
        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());

        pGraphicsItem->setToolTip(m_strToolTip);

    } // if( pGraphicsItem != nullptr )

} // updateToolTip
