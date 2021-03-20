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

#include "ZSDrawGraphObjElectricityTransistor.h"
#include "ZSDrawWdgtFormatGraphObjsElectricityTransistor.h"

#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/ZSDrawGraphObjEllipse.h"
#include "ZSDraw/ZSDrawGraphObjLine.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysEnumEntry.h"
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
class CGraphObjTransistor : public CGraphObjElectricity
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QSize   CGraphObjTransistor::c_sizInitial(42.0,48.0);
const QString CGraphObjTransistor::c_strKeyLabelDopingType = "DopingType";

/* EType
==============================================================================*/

static const SEnumEntry s_arEnumStrDopingTypes[CGraphObjTransistor::EDopingTypeCount] =
{
    /*  0 */ SEnumEntry( CGraphObjTransistor::EDopingTypeNPN, "NPN" ),
    /*  1 */ SEnumEntry( CGraphObjTransistor::EDopingTypePNP, "PNP" )
};

//------------------------------------------------------------------------------
QString CGraphObjTransistor::DopingType2Str( int i_type )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrDopingTypes, _ZSArrLen(s_arEnumStrDopingTypes), i_type );
}

//------------------------------------------------------------------------------
CGraphObjTransistor::EDopingType CGraphObjTransistor::Str2DopingType( const QString& i_str )
//------------------------------------------------------------------------------
{
    CGraphObjTransistor::EDopingType type = CGraphObjTransistor::EDopingTypeUndefined;

    int iVal = SEnumEntry::str2Enumerator( s_arEnumStrDopingTypes, _ZSArrLen(s_arEnumStrDopingTypes), i_str );

    if( iVal >= 0 && iVal < CGraphObjTransistor::EDopingTypeCount )
    {
        type = static_cast<CGraphObjTransistor::EDopingType>(iVal);
    }
    return type;

} // str2type

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjTransistor::ResetCtorsDtorsCounters()
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

qint64 CGraphObjTransistor::s_iCtorsCount = 1;
qint64 CGraphObjTransistor::s_iDtorsCount = 1;

CTrcAdminObj* CGraphObjTransistor::s_pTrcAdminObjCtorsAndDtor = nullptr;
CTrcAdminObj* CGraphObjTransistor::s_pTrcAdminObjItemChange = nullptr;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjTransistor::CGraphObjTransistor(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    EDopingType          i_dopingType,
    const QString&       i_strObjName,
    const QString&       i_strObjId ) :
//------------------------------------------------------------------------------
    CGraphObjElectricity(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ "ZS::Draw::Electricity",
        /* strClassName  */ "CGraphObjTransistor",
        /* strType       */ "Transistor",
        /* strObjName    */ i_strObjName.isEmpty() ? "T" + QString::number(s_iCtorsCount) : i_strObjName,
        /* strObjId      */ i_strObjId.isEmpty() ? "T" + QString::number(s_iCtorsCount) : i_strObjId,
        /* drawSettings  */ i_drawSettings ),
    m_pEllBody(nullptr),
    m_pLinBase(nullptr),
    m_pLinBaseUp(nullptr),
    m_pLinBaseDn(nullptr),
    m_pLinCnctPt1(nullptr),
    m_pLinCnctPt2(nullptr),
    m_pLinCnctPt3(nullptr),
    m_pCnctPt1(nullptr),
    m_pCnctPt2(nullptr),
    m_pCnctPt3(nullptr),
    m_dopingType(i_dopingType)
{
    s_iCtorsCount++;

    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor == nullptr )
    {
        s_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj("ZS::Draw::Electricity", "CGraphObjTransistor::CtorsAndDtor", "");
        s_pTrcAdminObjItemChange = CTrcServer::GetTraceAdminObj("ZS::Draw", "CGraphObjTransistor::ItemChange", "");
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
      NPN Transistor          PNP Transistor
                 +-+                     +-+
          CnctPt2| | C            CnctPt2| | E
                 +-+                     +-+
    CnctPt1       |         CnctPt1       |
     B     +--------+        B      ------+-
    +-+    | |    | |       +-+    | |    | |
    | |----|-|-- +  |       | |----|-|-- +  |
    +-+    | |    | |       +-+    | |    | |
           +--------+               ------+-
                  |                       |
                 +-+                     +-+
          CnctPt3| | E            CnctPt3| | C
                 +-+                     +-+
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
    double fBodyDiameter = rctBounding.width()-fCnctPtWidth-7.0;
    QRectF rctBody( rctBounding.right()-fBodyDiameter, rctBounding.center().y()-fBodyDiameter/2.0, fBodyDiameter, fBodyDiameter );
    QRectF rctCnctPt1( rctBounding.left(), rctBounding.center().y()-fCnctPtWidth/2.0, fCnctPtWidth, fCnctPtWidth );
    QRectF rctCnctPt2( rctBody.center().x()+fBodyDiameter/16.0, rctBounding.top(), fCnctPtWidth, fCnctPtWidth );
    QRectF rctCnctPt3( rctBody.center().x()+fBodyDiameter/16.0, rctBounding.bottom()-fCnctPtWidth, fCnctPtWidth, fCnctPtWidth );

    SGraphObjAlignment alignment;

    int idxAttr;

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

    CDrawSettings drawSettingsBody(EGraphObjTypeRect);

    drawSettingsBody.setFillStyle(EFillStyleSolidPattern);

    CDrawSettings drawSettingsLine(EGraphObjTypeLine);

    drawSettingsLine.setAttributesAreUsed( false, EDrawAttributeLineRecordTypeMin, EDrawAttributeLineRecordTypeCount );
    drawSettingsLine.setAttributesAreUsed( false, EDrawAttributeLineEndStyleMin, EDrawAttributeLineEndStyleCount );

    CDrawSettings drawSettingsLineBaseUp = drawSettingsLine;

    for( idxAttr = EDrawAttributeLineEndStyleMin; idxAttr <= EDrawAttributeLineEndStyleMax; idxAttr++ )
    {
        drawSettingsLineBaseUp.setAttributeIsUsed(idxAttr,true);
    }

    if( m_dopingType == EDopingTypePNP )
    {
        drawSettingsLineBaseUp.setLineEndStyle(ELinePointEnd,ELineEndStyleArrowHead);
        drawSettingsLineBaseUp.setLineEndBaseLineType(ELinePointEnd,ELineEndBaseLineTypeNormal);
        drawSettingsLineBaseUp.setLineEndFillStyle(ELinePointEnd,ELineEndFillStyleSolidPattern);
        drawSettingsLineBaseUp.setLineEndWidth(ELinePointEnd,ELineEndWidthThin);
        drawSettingsLineBaseUp.setLineEndLength(ELinePointEnd,ELineEndLengthShort);
    }

    CDrawSettings drawSettingsLineBaseDn = drawSettingsLine;

    for( idxAttr = EDrawAttributeLineEndStyleMin; idxAttr <= EDrawAttributeLineEndStyleMax; idxAttr++ )
    {
        drawSettingsLineBaseDn.setAttributeIsUsed(idxAttr,true);
    }

    if( m_dopingType == EDopingTypeNPN )
    {
        drawSettingsLineBaseDn.setLineEndStyle(ELinePointEnd,ELineEndStyleArrowHead);
        drawSettingsLineBaseDn.setLineEndBaseLineType(ELinePointEnd,ELineEndBaseLineTypeNormal);
        drawSettingsLineBaseDn.setLineEndFillStyle(ELinePointEnd,ELineEndFillStyleSolidPattern);
        drawSettingsLineBaseDn.setLineEndWidth(ELinePointEnd,ELineEndWidthThin);
        drawSettingsLineBaseDn.setLineEndLength(ELinePointEnd,ELineEndLengthShort);
    }

    CDrawSettings drawSettingsCnctPt(EGraphObjTypeConnectionPoint);

    // Body (Circle)
    //----------------

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

    // Base Line
    //----------------

    QPointF ptLinBaseCenter( rctBody.left()+fBodyDiameter/4.0-1.0, rctBody.center().y() );
    QLineF  linBaseCenter( QPointF(0.0,0.0), QPointF(0.0,rctBody.height()-fBodyDiameter/2.0) );

    m_pLinBase = new CGraphObjLine(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLine,
        /* strObjName    */ "BaseLine",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "BaseLine" );

    m_pLinBase->setLine(linBaseCenter);
    m_pDrawingScene->addItem(m_pLinBase);
    m_pDrawingScene->onGraphObjCreated(m_pLinBase);
    m_pLinBase->setPos( QPointF(ptLinBaseCenter.x(),rctBody.top()+fBodyDiameter/4.0) );
    addGraphObj(m_pLinBase);

    // Line from center of base line upwards
    //--------------------------------------

    QPointF ptLinBaseUpStart( ptLinBaseCenter.x(), ptLinBaseCenter.y()-4.0 );
    QPointF ptLinBaseUpEnd( rctCnctPt2.center().x(), rctBody.top()+fBodyDiameter/8.0 );
    QLineF  linBaseUp( QPointF(0.0,0.0), QPointF(ptLinBaseUpEnd.x()-ptLinBaseUpStart.x(),ptLinBaseUpEnd.y()-ptLinBaseUpStart.y()) );

    m_pLinBaseUp = new CGraphObjLine(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLineBaseUp,
        /* strObjName    */ "BaseLineUp",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "BaseLineUp" );

    m_pLinBaseUp->setLine(linBaseUp);
    m_pDrawingScene->addItem(m_pLinBaseUp);
    m_pDrawingScene->onGraphObjCreated(m_pLinBaseUp);
    m_pLinBaseUp->setPos(ptLinBaseUpStart);
    addGraphObj(m_pLinBaseUp);

    alignment = SGraphObjAlignment( EAlignmentRefLeft, EAlignmentRefLeft, false, ptLinBaseUpStart.x()/rctBounding.width() );
    m_pLinBaseUp->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefRight, EAlignmentRefLeft, false, ptLinBaseUpEnd.x()/rctBounding.width() );
    m_pLinBaseUp->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefBottom, EAlignmentRefVCenter, false, (ptLinBaseUpStart.y()-rctBounding.center().y())/rctBounding.height() );
    m_pLinBaseUp->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefTop, EAlignmentRefTop, false, ptLinBaseUpEnd.y()/rctBounding.height() );
    m_pLinBaseUp->addAlignment(alignment);

    // Line from center of base line downwards
    //----------------------------------------

    QPointF ptLinBaseDnStart( ptLinBaseCenter.x(), ptLinBaseCenter.y()+4.0 );
    QPointF ptLinBaseDnEnd( rctCnctPt3.center().x(), rctBody.bottom()-fBodyDiameter/8.0 );
    QLineF  linBaseDn( QPointF(0.0,0.0), QPointF(ptLinBaseDnEnd.x()-ptLinBaseDnStart.x(),ptLinBaseDnEnd.y()-ptLinBaseDnStart.y()) );

    m_pLinBaseDn = new CGraphObjLine(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLineBaseDn,
        /* strObjName    */ "BaseLineDn",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "BaseLineDn" );

    m_pLinBaseDn->setObjName("BaseLineDn");
    m_pLinBaseDn->setLine(linBaseDn);
    m_pDrawingScene->addItem(m_pLinBaseDn);
    m_pDrawingScene->onGraphObjCreated(m_pLinBaseDn);
    m_pLinBaseDn->setPos(ptLinBaseDnStart);
    addGraphObj(m_pLinBaseDn);

    alignment = SGraphObjAlignment( EAlignmentRefLeft, EAlignmentRefLeft, false, ptLinBaseDnStart.x()/rctBounding.width() );
    m_pLinBaseDn->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefRight, EAlignmentRefLeft, false, ptLinBaseDnEnd.x()/rctBounding.width() );
    m_pLinBaseDn->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefTop, EAlignmentRefVCenter, false, (ptLinBaseDnStart.y()-rctBounding.center().y())/rctBounding.height() );
    m_pLinBaseDn->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefBottom, EAlignmentRefBottom, false, (ptLinBaseDnEnd.y()-rctBounding.height())/rctBounding.height() );
    m_pLinBaseDn->addAlignment(alignment);

    // Line from CnctPt1 to base line
    //-------------------------------

    m_pLinCnctPt1 = new CGraphObjLine(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLine,
        /* strObjName    */ "CnctLine1",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "CnctLine1" );

    m_pLinCnctPt1->setLine( QLineF( QPointF(0.0,0.0), QPointF(ptLinBaseCenter.x()-rctCnctPt1.left(),ptLinBaseCenter.y()-rctCnctPt1.center().y()) ) );
    m_pDrawingScene->addItem(m_pLinCnctPt1);
    m_pDrawingScene->onGraphObjCreated(m_pLinCnctPt1);
    m_pLinCnctPt1->setPos( QPointF(rctCnctPt1.left(),rctCnctPt1.center().y()) );
    addGraphObj(m_pLinCnctPt1);

    // Line from end of line Up to CnctPt2
    //------------------------------------

    m_pLinCnctPt2 = new CGraphObjLine(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLine,
        /* strObjName    */ "CnctLine2",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "CnctLine2" );

    m_pLinCnctPt2->setLine( QLineF( QPointF(0.0,0.0), QPointF(rctCnctPt2.center().x()-ptLinBaseUpEnd.x(),rctCnctPt2.bottom()-ptLinBaseUpEnd.y()) ) );
    m_pDrawingScene->addItem(m_pLinCnctPt2);
    m_pDrawingScene->onGraphObjCreated(m_pLinCnctPt2);
    m_pLinCnctPt2->setPos(ptLinBaseUpEnd);
    addGraphObj(m_pLinCnctPt2);

    alignment = SGraphObjAlignment( EAlignmentRefLeft, EAlignmentRefLeft, false, rctCnctPt2.center().x()/rctBounding.width() );
    m_pLinCnctPt2->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefTop, EAlignmentRefTop, true, 0.0 );
    m_pLinCnctPt2->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefBottom, EAlignmentRefTop, false, ptLinBaseUpEnd.y()/rctBounding.height() );
    m_pLinCnctPt2->addAlignment(alignment);

    // Line from end of line Down to CnctPt3
    //--------------------------------------

    m_pLinCnctPt3 = new CGraphObjLine(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLine,
        /* strObjName    */ "CnctLine3",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "CnctLine3" );

    m_pLinCnctPt3->setLine( QLineF( QPointF(0.0,0.0), QPointF(rctCnctPt3.center().x()-ptLinBaseDnEnd.x(),rctCnctPt3.top()-ptLinBaseDnEnd.y()) ) );
    m_pDrawingScene->addItem(m_pLinCnctPt3);
    m_pDrawingScene->onGraphObjCreated(m_pLinCnctPt3);
    m_pLinCnctPt3->setPos(ptLinBaseDnEnd);
    addGraphObj(m_pLinCnctPt3);

    alignment = SGraphObjAlignment( EAlignmentRefLeft, EAlignmentRefLeft, false, rctCnctPt3.center().x()/rctBounding.width() );
    m_pLinCnctPt3->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefTop, EAlignmentRefBottom, false, (ptLinBaseDnEnd.y()-rctBounding.height())/rctBounding.height() );
    m_pLinCnctPt3->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefBottom, EAlignmentRefBottom, true, 0.0 );
    m_pLinCnctPt3->addAlignment(alignment);

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
    addGraphObj(m_pCnctPt2);

    m_pCnctPt2->setPos( rctCnctPt2.topLeft() );
    alignment = SGraphObjAlignment( EAlignmentRefHCenter, EAlignmentRefLeft, false, rctCnctPt2.center().x()/rctBounding.width() );
    m_pCnctPt2->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefTop, EAlignmentRefTop, true, 0.0 );
    m_pCnctPt2->addAlignment(alignment);

    // Connection Point 3
    //-------------------

    m_pCnctPt3 = new CGraphObjConnectionPoint(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsCnctPt,
        /* strObjName    */ "CnctPt3",
        /* strObjId      */ m_strObjId + m_pDrawingScene->getGraphObjNameNodeSeparator() + "CnctPt3" );

    m_pCnctPt3->setWidth(fCnctPtWidth);
    m_pCnctPt3->setInnerCircleWidthInPx(fCnctPtWidth);
    m_pCnctPt3->setRect( 0.0, 0.0, rctCnctPt3.width(), rctCnctPt3.height() );
    m_pCnctPt3->setFixedSize( QSize(fCnctPtWidth,fCnctPtWidth) );
    m_pDrawingScene->addItem(m_pCnctPt3);
    m_pDrawingScene->onGraphObjCreated(m_pCnctPt3);
    m_pCnctPt3->setPos( rctCnctPt3.topLeft() );
    addGraphObj(m_pCnctPt3);

    alignment = SGraphObjAlignment( EAlignmentRefHCenter, EAlignmentRefLeft, false, rctCnctPt3.center().x()/rctBounding.width() );
    m_pCnctPt3->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRefBottom, EAlignmentRefBottom, true, 0.0 );
    m_pCnctPt3->addAlignment(alignment);

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
CGraphObjTransistor::~CGraphObjTransistor()
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

    m_pEllBody = nullptr;
    m_pLinBase = nullptr;
    m_pLinBaseUp = nullptr;
    m_pLinBaseDn = nullptr;
    m_pLinCnctPt1 = nullptr;
    m_pLinCnctPt2 = nullptr;
    m_pLinCnctPt3 = nullptr;
    m_pCnctPt1 = nullptr;
    m_pCnctPt2 = nullptr;
    m_pCnctPt3 = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjTransistor::setDopingType( EDopingType i_type )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( s_pTrcAdminObjItemChange != nullptr && s_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "Type:" + DopingType2Str(i_type);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strObjName,
        /* strMethod    */ "setDopingType",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_dopingType != i_type )
    {
        m_dopingType = i_type;

        CDrawSettings drawSettingsLineBaseUp = m_pLinBaseUp->getDrawSettings();
        CDrawSettings drawSettingsLineBaseDn = m_pLinBaseDn->getDrawSettings();

        if( m_dopingType == EDopingTypeNPN )
        {
            drawSettingsLineBaseUp.setLineEndStyle(ELinePointEnd,ELineEndStyleNormal);

            drawSettingsLineBaseDn.setLineEndStyle(ELinePointEnd,ELineEndStyleArrowHead);
            drawSettingsLineBaseDn.setLineEndBaseLineType(ELinePointEnd,ELineEndBaseLineTypeNormal);
            drawSettingsLineBaseDn.setLineEndFillStyle(ELinePointEnd,ELineEndFillStyleSolidPattern);
            drawSettingsLineBaseDn.setLineEndWidth(ELinePointEnd,ELineEndWidthThin);
            drawSettingsLineBaseDn.setLineEndLength(ELinePointEnd,ELineEndLengthShort);
        }

        else // if( m_dopingType == ETypePNP )
        {
            drawSettingsLineBaseUp.setLineEndStyle(ELinePointEnd,ELineEndStyleArrowHead);
            drawSettingsLineBaseUp.setLineEndBaseLineType(ELinePointEnd,ELineEndBaseLineTypeNormal);
            drawSettingsLineBaseUp.setLineEndFillStyle(ELinePointEnd,ELineEndFillStyleSolidPattern);
            drawSettingsLineBaseUp.setLineEndWidth(ELinePointEnd,ELineEndWidthThin);
            drawSettingsLineBaseUp.setLineEndLength(ELinePointEnd,ELineEndLengthShort);

            drawSettingsLineBaseDn.setLineEndStyle(ELinePointEnd,ELineEndStyleNormal);
        }

        m_pLinBaseUp->setDrawSettings(drawSettingsLineBaseUp);
        m_pLinBaseDn->setDrawSettings(drawSettingsLineBaseDn);

        if( m_arpLabels.contains(c_strKeyLabelDopingType) )
        {
            SGraphObjLabel* pGraphObjLabel = m_arpLabels[c_strKeyLabelDopingType];

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                QString str = DopingType2Str(m_dopingType);
                pGraphObjLabel->m_pGraphObjLabel->setText(str);
            }
        }

    } // if( m_dopingType != i_type )

} // setDopingType

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjTransistor::showDopingType( ESelectionPoint i_selPtPos )
//------------------------------------------------------------------------------
{
    QString str = DopingType2Str(m_dopingType);

    if( !m_arpLabels.contains(c_strKeyLabelDopingType) )
    {
        addLabel(c_strKeyLabelDopingType,str,i_selPtPos);
    }
    else
    {
        setLabelText(c_strKeyLabelDopingType,str);
    }

    showLabel(c_strKeyLabelDopingType);

} // showDopingType

//------------------------------------------------------------------------------
void CGraphObjTransistor::hideDopingType()
//------------------------------------------------------------------------------
{
    hideLabel(c_strKeyLabelDopingType);
}

//------------------------------------------------------------------------------
bool CGraphObjTransistor::isDopingTypeVisible() const
//------------------------------------------------------------------------------
{
    return isLabelVisible(c_strKeyLabelDopingType);
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjTransistor::clone()
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

    CGraphObjTransistor* pGraphObj = nullptr;

    return pGraphObj;

} // clone

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjTransistor::onCreateAndExecDlgFormatGraphObjs()
//------------------------------------------------------------------------------
{
    CDlgFormatGraphObjs* pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene,this);

    QIcon   icon;
    QPixmap pxm(":/ZS/Draw/Electricity/Transistor16x16.bmp");

    pxm.setMask(pxm.createHeuristicMask());
    icon.addPixmap(pxm);

    CWdgtFormatGraphObjsTransistor* pWdgt = new CWdgtFormatGraphObjsTransistor(m_pDrawingScene,this);

    pDlgFormatGraphObjs->addWidget( icon, "Transistor", pWdgt );

    pDlgFormatGraphObjs->setCurrentWidget("Transistor");

    pDlgFormatGraphObjs->exec();

    delete pDlgFormatGraphObjs;
    pDlgFormatGraphObjs = nullptr;

} // onCreateAndExecDlgFormatGraphObjs

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjTransistor::onDrawSettingsChanged()
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

    CDrawSettings drawSettingsLine = m_pLinBase->getDrawSettings();

    drawSettingsLine.setPenColor(m_drawSettings.getPenColor());
    drawSettingsLine.setPenWidth(m_drawSettings.getPenWidth());
    drawSettingsLine.setLineStyle(m_drawSettings.getLineStyle());

    m_pLinBase->setDrawSettings(drawSettingsLine);

    CDrawSettings drawSettingsBody = m_pEllBody->getDrawSettings();

    drawSettingsBody.setPenColor(m_drawSettings.getPenColor());
    drawSettingsBody.setPenWidth(m_drawSettings.getPenWidth());
    drawSettingsBody.setLineStyle(m_drawSettings.getLineStyle());
    drawSettingsBody.setFillColor(m_drawSettings.getFillColor());
    //drawSettingsBody.setFillStyle(m_drawSettings.getFillStyle()); keep SolidPattern

    m_pEllBody->setDrawSettings(drawSettingsBody);

    CDrawSettings drawSettingsLineBaseUp = m_pLinBaseUp->getDrawSettings();

    drawSettingsLineBaseUp.setPenColor(m_drawSettings.getPenColor());
    drawSettingsLineBaseUp.setPenWidth(m_drawSettings.getPenWidth());
    drawSettingsLineBaseUp.setLineStyle(m_drawSettings.getLineStyle());

    m_pLinBaseUp->setDrawSettings(drawSettingsLineBaseUp);

    CDrawSettings drawSettingsLineBaseDn = m_pLinBaseDn->getDrawSettings();

    drawSettingsLineBaseDn.setPenColor(m_drawSettings.getPenColor());
    drawSettingsLineBaseDn.setPenWidth(m_drawSettings.getPenWidth());
    drawSettingsLineBaseDn.setLineStyle(m_drawSettings.getLineStyle());

    m_pLinBaseDn->setDrawSettings(drawSettingsLineBaseDn);

    m_pLinCnctPt1->setDrawSettings(drawSettingsLine);
    m_pLinCnctPt2->setDrawSettings(drawSettingsLine);
    m_pLinCnctPt3->setDrawSettings(drawSettingsLine);

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
void CGraphObjTransistor::updateToolTip()
//------------------------------------------------------------------------------
{
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        QString strNodeSeparator = m_pDrawingScene->getGraphObjNameNodeSeparator();
        QPointF ptPos;

        m_strToolTip  = "ObjName:\t" + getObjName(true,strNodeSeparator);
        m_strToolTip += "\nObjId:\t\t" + getObjId();

        m_strToolTip += "DopingType:\t" + DopingType2Str(m_dopingType);

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
