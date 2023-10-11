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

#include "Electricity/ZSDrawGraphObjElectricityTransistor.h"
#include "Electricity/ZSDrawWdgtFormatGraphObjsElectricityTransistor.h"

#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjEllipse.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLine.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjLabel.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/Widgets/GraphObjFormat/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSSys/ZSSysAux.h"
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


/*******************************************************************************
class CGraphObjTransistor : public CGraphObjElectricity
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QSize CGraphObjTransistor::c_sizInitial(42.0, 48.0);

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
protected: // class members
==============================================================================*/

qint64 CGraphObjTransistor::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjTransistor::CGraphObjTransistor(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    EDopingType          i_dopingType,
    const QString&       i_strObjName ) :
//------------------------------------------------------------------------------
    CGraphObjElectricity(
        /* pDrawingScene */ i_pDrawingScene,
        /* strType       */ "Transistor",
        /* strObjName    */ i_strObjName.isEmpty() ? "T" + QString::number(s_iInstCount) : i_strObjName,
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
    // Just incremented by the ctor but not decremented by the dtor.
    // Used to create a unique name for newly created objects of this type.
    s_iInstCount++;

    QString strAddTrcInfo;

    createTraceAdminObjs("Electricity::" + ClassName());

    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
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

    //m_drawSettings.setAttributesAreUsed( true, EDrawAttributePenMin, EDrawAttributePenCount );
    ////m_drawSettings.setAttributesAreUsed( true, EDrawAttributeLineStyleMin, EDrawAttributeLineStyleCount );
    //m_drawSettings.setAttributesAreUsed( true, EDrawAttributeFillMin, EDrawAttributeFillCount );

    m_drawSettings.setPenColor(Qt::black);
    m_drawSettings.setPenWidth(1);
    m_drawSettings.setLineStyle(ELineStyle::SolidLine);
    m_drawSettings.setFillColor(Qt::white);
    m_drawSettings.setFillStyle(EFillStyle::SolidPattern);

    // Draw settings for elements
    //---------------------------

    CDrawSettings drawSettingsBody(EGraphObjTypeRect);

    drawSettingsBody.setFillStyle(EFillStyle::SolidPattern);

    CDrawSettings drawSettingsLine(EGraphObjTypeLine);

    //drawSettingsLine.setAttributesAreUsed( false, EDrawAttributeLineRecordTypeMin, EDrawAttributeLineRecordTypeCount );
    //drawSettingsLine.setAttributesAreUsed( false, EDrawAttributeLineEndStyleMin, EDrawAttributeLineEndStyleCount );

    CDrawSettings drawSettingsLineBaseUp = drawSettingsLine;

    //for( idxAttr = EDrawAttributeLineEndStyleMin; idxAttr <= EDrawAttributeLineEndStyleMax; idxAttr++ )
    //{
    //    drawSettingsLineBaseUp.setAttributeIsUsed(idxAttr,true);
    //}

    if( m_dopingType == EDopingTypePNP )
    {
        drawSettingsLineBaseUp.setLineEndStyle(ELinePoint::End,ELineEndStyle::ArrowHead);
        drawSettingsLineBaseUp.setArrowHeadBaseLineType(ELinePoint::End,EArrowHeadBaseLineType::Normal);
        drawSettingsLineBaseUp.setArrowHeadFillStyle(ELinePoint::End,EArrowHeadFillStyle::SolidPattern);
        drawSettingsLineBaseUp.setArrowHeadWidth(ELinePoint::End,EArrowHeadWidth::Thin);
        drawSettingsLineBaseUp.setArrowHeadLength(ELinePoint::End,EArrowHeadLength::Short);
    }

    CDrawSettings drawSettingsLineBaseDn = drawSettingsLine;

    //for( idxAttr = EDrawAttributeLineEndStyleMin; idxAttr <= EDrawAttributeLineEndStyleMax; idxAttr++ )
    //{
    //    drawSettingsLineBaseDn.setAttributeIsUsed(idxAttr,true);
    //}

    if( m_dopingType == EDopingTypeNPN )
    {
        drawSettingsLineBaseDn.setLineEndStyle(ELinePoint::End,ELineEndStyle::ArrowHead);
        drawSettingsLineBaseDn.setArrowHeadBaseLineType(ELinePoint::End,EArrowHeadBaseLineType::Normal);
        drawSettingsLineBaseDn.setArrowHeadFillStyle(ELinePoint::End,EArrowHeadFillStyle::SolidPattern);
        drawSettingsLineBaseDn.setArrowHeadWidth(ELinePoint::End,EArrowHeadWidth::Thin);
        drawSettingsLineBaseDn.setArrowHeadLength(ELinePoint::End,EArrowHeadLength::Short);
    }

    CDrawSettings drawSettingsCnctPt(EGraphObjTypeConnectionPoint);

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
    // Body (Circle)
    //----------------

    m_pEllBody = new CGraphObjEllipse(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsBody,
        /* strObjName    */ "Body" );

    m_pEllBody->setRect( 0.0, 0.0, rctBody.width(), rctBody.height() );
    m_pDrawingScene->addGraphObj(m_pEllBody);
    m_pDrawingScene->onGraphObjCreationFinished(m_pEllBody);
    m_pEllBody->setPos( rctBody.topLeft() );
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pEllBody);

    // Base Line
    //----------------

    QPointF ptLinBaseCenter( rctBody.left()+fBodyDiameter/4.0-1.0, rctBody.center().y() );
    QLineF  linBaseCenter( QPointF(0.0,0.0), QPointF(0.0,rctBody.height()-fBodyDiameter/2.0) );

    m_pLinBase = new CGraphObjLine(
        m_pDrawingScene, drawSettingsLine, "BaseLine",
        QPointF(0.0,0.0), QPointF(0.0,0.0));

    m_pLinBase->setLine(linBaseCenter);
    m_pDrawingScene->addGraphObj(m_pLinBase);
    m_pDrawingScene->onGraphObjCreationFinished(m_pLinBase);
    m_pLinBase->setPos( QPointF(ptLinBaseCenter.x(),rctBody.top()+fBodyDiameter/4.0) );
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pLinBase);

    // Line from center of base line upwards
    //--------------------------------------

    QPointF ptLinBaseUpStart( ptLinBaseCenter.x(), ptLinBaseCenter.y()-4.0 );
    QPointF ptLinBaseUpEnd( rctCnctPt2.center().x(), rctBody.top()+fBodyDiameter/8.0 );
    QLineF  linBaseUp( QPointF(0.0,0.0), QPointF(ptLinBaseUpEnd.x()-ptLinBaseUpStart.x(),ptLinBaseUpEnd.y()-ptLinBaseUpStart.y()) );

    m_pLinBaseUp = new CGraphObjLine(
        m_pDrawingScene, drawSettingsLine, "BaseLineUp",
        QPointF(0.0,0.0), QPointF(0.0,0.0));

    m_pLinBaseUp->setLine(linBaseUp);
    m_pDrawingScene->addGraphObj(m_pLinBaseUp);
    m_pDrawingScene->onGraphObjCreationFinished(m_pLinBaseUp);
    m_pLinBaseUp->setPos(ptLinBaseUpStart);
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pLinBaseUp);

    alignment = SGraphObjAlignment( EAlignmentRef::Left, EAlignmentRef::Left, false, ptLinBaseUpStart.x()/rctBounding.width() );
    m_pLinBaseUp->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Right, EAlignmentRef::Left, false, ptLinBaseUpEnd.x()/rctBounding.width() );
    m_pLinBaseUp->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Bottom, EAlignmentRef::VCenter, false, (ptLinBaseUpStart.y()-rctBounding.center().y())/rctBounding.height() );
    m_pLinBaseUp->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Top, EAlignmentRef::Top, false, ptLinBaseUpEnd.y()/rctBounding.height() );
    m_pLinBaseUp->addAlignment(alignment);

    // Line from center of base line downwards
    //----------------------------------------

    QPointF ptLinBaseDnStart( ptLinBaseCenter.x(), ptLinBaseCenter.y()+4.0 );
    QPointF ptLinBaseDnEnd( rctCnctPt3.center().x(), rctBody.bottom()-fBodyDiameter/8.0 );
    QLineF  linBaseDn( QPointF(0.0,0.0), QPointF(ptLinBaseDnEnd.x()-ptLinBaseDnStart.x(),ptLinBaseDnEnd.y()-ptLinBaseDnStart.y()) );

    m_pLinBaseDn = new CGraphObjLine(
        m_pDrawingScene, drawSettingsLine, "BaseLineDn",
        QPointF(0.0,0.0), QPointF(0.0,0.0));

    m_pLinBaseDn->setLine(linBaseDn);
    m_pDrawingScene->addGraphObj(m_pLinBaseDn);
    m_pDrawingScene->onGraphObjCreationFinished(m_pLinBaseDn);
    m_pLinBaseDn->setPos(ptLinBaseDnStart);
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pLinBaseDn);

    alignment = SGraphObjAlignment( EAlignmentRef::Left, EAlignmentRef::Left, false, ptLinBaseDnStart.x()/rctBounding.width() );
    m_pLinBaseDn->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Right, EAlignmentRef::Left, false, ptLinBaseDnEnd.x()/rctBounding.width() );
    m_pLinBaseDn->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Top, EAlignmentRef::VCenter, false, (ptLinBaseDnStart.y()-rctBounding.center().y())/rctBounding.height() );
    m_pLinBaseDn->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Bottom, EAlignmentRef::Bottom, false, (ptLinBaseDnEnd.y()-rctBounding.height())/rctBounding.height() );
    m_pLinBaseDn->addAlignment(alignment);

    // Line from CnctPt1 to base line
    //-------------------------------

    m_pLinCnctPt1 = new CGraphObjLine(
        m_pDrawingScene, drawSettingsLine, "CnctLine1",
        QPointF(0.0,0.0), QPointF(0.0,0.0));

    m_pLinCnctPt1->setLine( QLineF( QPointF(0.0,0.0), QPointF(ptLinBaseCenter.x()-rctCnctPt1.left(),ptLinBaseCenter.y()-rctCnctPt1.center().y()) ) );
    m_pDrawingScene->addGraphObj(m_pLinCnctPt1);
    m_pDrawingScene->onGraphObjCreationFinished(m_pLinCnctPt1);
    m_pLinCnctPt1->setPos( QPointF(rctCnctPt1.left(),rctCnctPt1.center().y()) );
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pLinCnctPt1);

    // Line from end of line Up to CnctPt2
    //------------------------------------

    m_pLinCnctPt2 = new CGraphObjLine(
        m_pDrawingScene, drawSettingsLine, "CnctLine2",
        QPointF(0.0,0.0), QPointF(0.0,0.0));

    m_pLinCnctPt2->setLine( QLineF( QPointF(0.0,0.0), QPointF(rctCnctPt2.center().x()-ptLinBaseUpEnd.x(),rctCnctPt2.bottom()-ptLinBaseUpEnd.y()) ) );
    m_pDrawingScene->addGraphObj(m_pLinCnctPt2);
    m_pDrawingScene->onGraphObjCreationFinished(m_pLinCnctPt2);
    m_pLinCnctPt2->setPos(ptLinBaseUpEnd);
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pLinCnctPt2);

    alignment = SGraphObjAlignment( EAlignmentRef::Left, EAlignmentRef::Left, false, rctCnctPt2.center().x()/rctBounding.width() );
    m_pLinCnctPt2->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Top, EAlignmentRef::Top, true, 0.0 );
    m_pLinCnctPt2->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Bottom, EAlignmentRef::Top, false, ptLinBaseUpEnd.y()/rctBounding.height() );
    m_pLinCnctPt2->addAlignment(alignment);

    // Line from end of line Down to CnctPt3
    //--------------------------------------

    m_pLinCnctPt3 = new CGraphObjLine(
        m_pDrawingScene, drawSettingsLine, "CnctLine3",
        QPointF(0.0,0.0), QPointF(0.0,0.0));

    m_pLinCnctPt3->setLine( QLineF( QPointF(0.0,0.0), QPointF(rctCnctPt3.center().x()-ptLinBaseDnEnd.x(),rctCnctPt3.top()-ptLinBaseDnEnd.y()) ) );
    m_pDrawingScene->addGraphObj(m_pLinCnctPt3);
    m_pDrawingScene->onGraphObjCreationFinished(m_pLinCnctPt3);
    m_pLinCnctPt3->setPos(ptLinBaseDnEnd);
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pLinCnctPt3);

    alignment = SGraphObjAlignment( EAlignmentRef::Left, EAlignmentRef::Left, false, rctCnctPt3.center().x()/rctBounding.width() );
    m_pLinCnctPt3->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Top, EAlignmentRef::Bottom, false, (ptLinBaseDnEnd.y()-rctBounding.height())/rctBounding.height() );
    m_pLinCnctPt3->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Bottom, EAlignmentRef::Bottom, true, 0.0 );
    m_pLinCnctPt3->addAlignment(alignment);

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
    m_pDrawingScene->addGraphObj(m_pCnctPt1);
    m_pDrawingScene->onGraphObjCreationFinished(m_pCnctPt1);
    m_pCnctPt1->setPos( rctCnctPt1.topLeft() );
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pCnctPt1);

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
    m_pDrawingScene->addGraphObj(m_pCnctPt2);
    m_pDrawingScene->onGraphObjCreationFinished(m_pCnctPt2);
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pCnctPt2);

    m_pCnctPt2->setPos( rctCnctPt2.topLeft() );
    alignment = SGraphObjAlignment( EAlignmentRef::HCenter, EAlignmentRef::Left, false, rctCnctPt2.center().x()/rctBounding.width() );
    m_pCnctPt2->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Top, EAlignmentRef::Top, true, 0.0 );
    m_pCnctPt2->addAlignment(alignment);

    // Connection Point 3
    //-------------------

    m_pCnctPt3 = new CGraphObjConnectionPoint(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsCnctPt,
        /* strObjName    */ "CnctPt3" );

    m_pCnctPt3->setWidth(fCnctPtWidth);
    m_pCnctPt3->setInnerCircleWidthInPx(fCnctPtWidth);
    m_pCnctPt3->setRect( 0.0, 0.0, rctCnctPt3.width(), rctCnctPt3.height() );
    m_pCnctPt3->setFixedSize( QSize(fCnctPtWidth,fCnctPtWidth) );
    m_pDrawingScene->addGraphObj(m_pCnctPt3);
    m_pDrawingScene->onGraphObjCreationFinished(m_pCnctPt3);
    m_pCnctPt3->setPos( rctCnctPt3.topLeft() );
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pCnctPt3);

    alignment = SGraphObjAlignment( EAlignmentRef::HCenter, EAlignmentRef::Left, false, rctCnctPt3.center().x()/rctBounding.width() );
    m_pCnctPt3->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Bottom, EAlignmentRef::Bottom, true, 0.0 );
    m_pCnctPt3->addAlignment(alignment);

    // Update group coordinates
    //-------------------------

    m_rctCurr = rctBounding;
    m_ptRotOriginCurr = m_rctCurr.center();
    #pragma message(__TODO__"The methods onGraphObjCreationFinished and acceptCurrentAsOriginalCoors should become protected and it should not be necessary to explicitly call them")
    acceptCurrentAsOriginalCoors();
#endif

    updateToolTip();

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjTransistor::~CGraphObjTransistor()
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

    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal))
    {
        strAddTrcInfo = "Type:" + DopingType2Str(i_type);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setDopingType",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_dopingType != i_type )
    {
        m_dopingType = i_type;

        CDrawSettings drawSettingsLineBaseUp = m_pLinBaseUp->getDrawSettings();
        CDrawSettings drawSettingsLineBaseDn = m_pLinBaseDn->getDrawSettings();

        if( m_dopingType == EDopingTypeNPN )
        {
            drawSettingsLineBaseUp.setLineEndStyle(ELinePoint::End,ELineEndStyle::Normal);

            drawSettingsLineBaseDn.setLineEndStyle(ELinePoint::End,ELineEndStyle::ArrowHead);
            drawSettingsLineBaseDn.setArrowHeadBaseLineType(ELinePoint::End,EArrowHeadBaseLineType::Normal);
            drawSettingsLineBaseDn.setArrowHeadFillStyle(ELinePoint::End,EArrowHeadFillStyle::SolidPattern);
            drawSettingsLineBaseDn.setArrowHeadWidth(ELinePoint::End,EArrowHeadWidth::Thin);
            drawSettingsLineBaseDn.setArrowHeadLength(ELinePoint::End,EArrowHeadLength::Short);
        }

        else // if( m_dopingType == ETypePNP )
        {
            drawSettingsLineBaseUp.setLineEndStyle(ELinePoint::End,ELineEndStyle::ArrowHead);
            drawSettingsLineBaseUp.setArrowHeadBaseLineType(ELinePoint::End,EArrowHeadBaseLineType::Normal);
            drawSettingsLineBaseUp.setArrowHeadFillStyle(ELinePoint::End,EArrowHeadFillStyle::SolidPattern);
            drawSettingsLineBaseUp.setArrowHeadWidth(ELinePoint::End,EArrowHeadWidth::Thin);
            drawSettingsLineBaseUp.setArrowHeadLength(ELinePoint::End,EArrowHeadLength::Short);

            drawSettingsLineBaseDn.setLineEndStyle(ELinePoint::End,ELineEndStyle::Normal);
        }

        m_pLinBaseUp->setDrawSettings(drawSettingsLineBaseUp);
        m_pLinBaseDn->setDrawSettings(drawSettingsLineBaseDn);

        //setDescription( DopingType2Str(m_dopingType) );

    } // if( m_dopingType != i_type )

} // setDopingType

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjTransistor::showDopingType( ESelectionPoint i_selPtPos )
//------------------------------------------------------------------------------
{
    //showDescriptionLabel(i_selPtPos);
}

//------------------------------------------------------------------------------
void CGraphObjTransistor::hideDopingType()
//------------------------------------------------------------------------------
{
    //hideDescriptionLabel();
}

//------------------------------------------------------------------------------
bool CGraphObjTransistor::isDopingTypeVisible( ESelectionPoint i_selPtPos ) const
//------------------------------------------------------------------------------
{
    return false; //isDescriptionLabelVisible(i_selPtPos);
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjTransistor::clone()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObjCtorsAndDtor, EMethodTraceDetailLevel::ArgsNormal))
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
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
void CGraphObjTransistor::onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjItemChange, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "OldSettings {" + i_drawSettingsOld.toString() + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ m_strName,
        /* strMethod    */ "onDrawSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

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

        m_strToolTip  = "ObjName:\t" + name();
        m_strToolTip += "\nObjId:\t\t" + keyInTree();

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

#ifdef ZSDRAW_GRAPHOBJ_USE_OBSOLETE_INSTANCE_MEMBERS
        m_strToolTip += "\nSize:\t\t" + size2Str(getSize());
        m_strToolTip += "\nRotation:\t" + QString::number(m_fRotAngleCurr_deg,'f',1) + " " + ZS::PhysVal::c_strSymbolDegree;
#endif
        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());

        pGraphicsItem->setToolTip(m_strToolTip);

    } // if( pGraphicsItem != nullptr )

} // updateToolTip
