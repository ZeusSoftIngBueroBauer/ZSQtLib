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

#include "ZSDrawGraphObjElectricitySwitch.h"
#include "ZSDrawWdgtFormatGraphObjsElectricitySwitch.h"

#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawGraphObjConnectionPoint.h"
#include "ZSDraw/ZSDrawGraphObjLine.h"
#include "ZSDraw/ZSDrawGraphObjRect.h"
#include "ZSDraw/ZSDrawGraphObjLabel.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
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
using namespace ZS::Trace;


/*******************************************************************************
class CGraphObjSwitch : public CGraphObjElectricity
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

const QSize   CGraphObjSwitch::c_sizInitial(24.0,10.0);
const QString CGraphObjSwitch::c_strKeyLabelState = "State";

/* EState
==============================================================================*/

static const SEnumEntry s_arEnumStrSwitchStates[CGraphObjSwitch::EStateCount] =
{
    /*  0 */ SEnumEntry( CGraphObjSwitch::EStateOpened, "Opened" ),
    /*  1 */ SEnumEntry( CGraphObjSwitch::EStateClosed, "Closed" )
};

//------------------------------------------------------------------------------
QString CGraphObjSwitch::State2Str( int i_state )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrSwitchStates, _ZSArrLen(s_arEnumStrSwitchStates), i_state );
}

//------------------------------------------------------------------------------
CGraphObjSwitch::EState CGraphObjSwitch::Str2State( const QString& i_str )
//------------------------------------------------------------------------------
{
    CGraphObjSwitch::EState state = CGraphObjSwitch::EStateUndefined;

    int iVal = SEnumEntry::str2Enumerator( s_arEnumStrSwitchStates, _ZSArrLen(s_arEnumStrSwitchStates), i_str );

    if( iVal >= 0 && iVal < CGraphObjSwitch::EStateCount )
    {
        state = static_cast<CGraphObjSwitch::EState>(iVal);
    }
    return state;

} // Str2State

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjSwitch::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjSwitch::CGraphObjSwitch(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    EState               i_state,
    const QString&       i_strObjName ) :
//------------------------------------------------------------------------------
    CGraphObjElectricity(
        /* pDrawingScene */ i_pDrawingScene,
        /* strType       */ "Switch",
        /* strObjName    */ i_strObjName.isEmpty() ? "S" + QString::number(s_iInstCount) : i_strObjName,
        /* drawSettings  */ i_drawSettings ),
    m_pCnctPt1(nullptr),
    m_pCnctPt2(nullptr),
    m_pLinSwitch(nullptr),
    m_idxLinSwitchAlignmentTop(-1),
    m_state(i_state)
{
    s_iInstCount++;

    QString strAddTrcInfo;

    QString strNameSpace = NameSpace();
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
    CnctPt1 SwitchLine CnctPt2
       +--+            +--+
       |  +============+  |
       +--+            +--+
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
    QRectF rctCnctPt1( rctBounding.left(), rctBounding.bottom()-fCnctPtWidth, fCnctPtWidth, fCnctPtWidth );
    QRectF rctCnctPt2( rctBounding.right()-fCnctPtWidth, rctBounding.bottom()-fCnctPtWidth, fCnctPtWidth, fCnctPtWidth );

    SGraphObjAlignment alignment;

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
    alignment = SGraphObjAlignment( EAlignmentRef::Bottom, EAlignmentRef::Bottom, true, 0.0 );
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
    m_pCnctPt2->setPos( rctCnctPt2.topLeft() );
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pCnctPt2);

    alignment = SGraphObjAlignment( EAlignmentRef::Right, EAlignmentRef::Right, true, 0.0 );
    m_pCnctPt2->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Bottom, EAlignmentRef::Bottom, true, 0.0 );
    m_pCnctPt2->addAlignment(alignment);

    // Switch Line
    //----------------

    QPointF ptSwitchLineStart( rctBounding.left()+fCnctPtWidth, rctBounding.bottom()-fCnctPtWidth/2.0 );
    QPointF ptSwitchLineEnd( rctBounding.right()-fCnctPtWidth, rctBounding.bottom()-fCnctPtWidth/2.0 );
    QLineF  lnSwitch( QPointF(0.0,0.0), QPointF(ptSwitchLineEnd.x()-ptSwitchLineStart.x(),ptSwitchLineEnd.y()-ptSwitchLineStart.y()) );

    if( m_state == EStateOpened )
    {
        ptSwitchLineEnd.setY( rctBounding.top() );
        lnSwitch.setP2( QPointF(ptSwitchLineEnd.x()-ptSwitchLineStart.x(),ptSwitchLineEnd.y()-ptSwitchLineStart.y()) );
    }

    m_pLinSwitch = new CGraphObjLine(
        /* pDrawingScene */ m_pDrawingScene,
        /* drawSettings  */ drawSettingsLine,
        /* strObjName    */ "SwitchLine" );

    m_pLinSwitch->setLine(lnSwitch);
    m_pDrawingScene->addGraphObj(m_pLinSwitch);
    m_pDrawingScene->onGraphObjCreationFinished(m_pLinSwitch);
    m_pLinSwitch->setPos(ptSwitchLineStart);
    throw ZS::System::CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    //addGraphObj(m_pLinSwitch);

    alignment = SGraphObjAlignment( EAlignmentRef::Left, EAlignmentRef::Left, true, ptSwitchLineStart.x() );
    m_pLinSwitch->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Right, EAlignmentRef::Right, true, ptSwitchLineEnd.x()-rctBounding.width() );
    m_pLinSwitch->addAlignment(alignment);
    alignment = SGraphObjAlignment( EAlignmentRef::Bottom, EAlignmentRef::Bottom, true, -fCnctPtWidth/2.0 );
    m_pLinSwitch->addAlignment(alignment);

    if( m_state == EStateOpened )
    {
        alignment = SGraphObjAlignment( EAlignmentRef::Top, EAlignmentRef::Top, true, 0.0 );
        m_idxLinSwitchAlignmentTop = m_pLinSwitch->addAlignment(alignment);
    }
    else // if( m_state == EStateClosed )
    {
        alignment = SGraphObjAlignment( EAlignmentRef::Top, EAlignmentRef::Bottom, true, -fCnctPtWidth/2.0 );
        m_idxLinSwitchAlignmentTop = m_pLinSwitch->addAlignment(alignment);
    }

    // Update group coordinates
    //-------------------------

    m_rctCurr = rctBounding;

    m_ptRotOriginCurr = m_rctCurr.center();

    #pragma message(__TODO__"The methods onGraphObjCreationFinished and acceptCurrentAsOriginalCoors should become protected and it should not be necessary to explicitly call them")
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
CGraphObjSwitch::~CGraphObjSwitch()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pCnctPt1 = nullptr;
    m_pCnctPt2 = nullptr;
    m_pLinSwitch = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSwitch::setState( EState i_state )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "State:" + State2Str(i_state);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "setState",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_state != i_state )
    {
        m_state = i_state;

        QLineF  lnSwitch = m_pLinSwitch->line();
        QPointF ptSwitchLineEnd = lnSwitch.p2();

        if( m_state == EStateOpened )
        {
            ptSwitchLineEnd.setY( -(m_rctCurr.height()-m_pCnctPt2->getHeight()/2.0) );
        }
        else // if( m_state == EStateClosed )
        {
            ptSwitchLineEnd.setY(0.0);
        }

        lnSwitch.setP2(ptSwitchLineEnd);
        m_pLinSwitch->setLine(lnSwitch);

        SGraphObjAlignment alignment;

        if( m_state == EStateOpened )
        {
            alignment = SGraphObjAlignment( EAlignmentRef::Top, EAlignmentRef::Top, true, 0.0 );
            m_pLinSwitch->setAlignment(m_idxLinSwitchAlignmentTop,alignment);
        }
        else // if( m_state == EStateClosed )
        {
            alignment = SGraphObjAlignment( EAlignmentRef::Top, EAlignmentRef::Bottom, true, -m_pCnctPt2->getHeight()/2.0 );
            m_pLinSwitch->setAlignment(m_idxLinSwitchAlignmentTop,alignment);
        }

        if( m_arpLabels.contains(c_strKeyLabelState) )
        {
            SGraphObjLabel* pGraphObjLabel = m_arpLabels[c_strKeyLabelState];

            if( pGraphObjLabel->m_pGraphObjLabel != nullptr )
            {
                QString str = State2Str(m_state);
                pGraphObjLabel->m_pGraphObjLabel->setText(str);
            }
        }

    } // if( m_state != i_state )

} // setState

//------------------------------------------------------------------------------
void CGraphObjSwitch::toggleState()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjItemChange != nullptr && m_pTrcAdminObjItemChange->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo = "StateCurr:" + State2Str(m_state);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjItemChange,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "toggleState",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_state == EStateOpened )
    {
        setState(EStateClosed);
    }
    else // if( m_state == EStateClosed )
    {
        setState(EStateOpened);
    }

} // toggleState

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSwitch::showState( ESelectionPoint i_selPtPos )
//------------------------------------------------------------------------------
{
    QString str = State2Str(m_state);

    if( !m_arpLabels.contains(c_strKeyLabelState) )
    {
        addLabel(c_strKeyLabelState,str,i_selPtPos);
    }
    else
    {
        setLabelText(c_strKeyLabelState,str);
    }

    showLabel(c_strKeyLabelState);

} // showState

//------------------------------------------------------------------------------
void CGraphObjSwitch::hideState()
//------------------------------------------------------------------------------
{
    hideLabel(c_strKeyLabelState);
}

//------------------------------------------------------------------------------
bool CGraphObjSwitch::isStateVisible() const
//------------------------------------------------------------------------------
{
    return isLabelVisible(c_strKeyLabelState);
}

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjSwitch::clone()
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

    CGraphObjSwitch* pGraphObj = nullptr;

    return pGraphObj;

} // clone

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSwitch::onCreateAndExecDlgFormatGraphObjs()
//------------------------------------------------------------------------------
{
    CDlgFormatGraphObjs* pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene,this);

    QIcon   icon;
    QPixmap pxm(":/ZS/Draw/Electricity/Switch16x16.bmp");

    pxm.setMask(pxm.createHeuristicMask());
    icon.addPixmap(pxm);

    CWdgtFormatGraphObjsSwitch* pWdgt = new CWdgtFormatGraphObjsSwitch(m_pDrawingScene,this);

    pDlgFormatGraphObjs->addWidget( icon, "Switch", pWdgt );

    pDlgFormatGraphObjs->setCurrentWidget("Switch");

    pDlgFormatGraphObjs->exec();

    delete pDlgFormatGraphObjs;
    pDlgFormatGraphObjs = nullptr;

} // onCreateAndExecDlgFormatGraphObjs

/*==============================================================================
public: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSwitch::onDrawSettingsChanged()
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

    CDrawSettings drawSettingsLine = m_pLinSwitch->getDrawSettings();

    drawSettingsLine.setPenColor(m_drawSettings.getPenColor());
    drawSettingsLine.setPenWidth(m_drawSettings.getPenWidth());
    drawSettingsLine.setLineStyle(m_drawSettings.getLineStyle());

    m_pLinSwitch->setDrawSettings(drawSettingsLine);

} // onDrawSettingsChanged

/*==============================================================================
protected: // overridables of base class CGraphObjGroup
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSwitch::mousePressEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mousePressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjGroup::mousePressEvent(i_pEv);

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Simulation )
    {
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
void CGraphObjSwitch::mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjGroup::mouseReleaseEvent(i_pEv);

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Simulation )
    {
        toggleState();
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
void CGraphObjSwitch::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Pos:(" + QString::number(i_pEv->pos().x()) + "," + QString::number(i_pEv->pos().y()) + ")";
        strAddTrcInfo += ", Ev.ScenePos:(" + QString::number(i_pEv->scenePos().x()) + "," + QString::number(i_pEv->scenePos().y()) + ")";
        strAddTrcInfo += ", Ev.ScreenPos:(" + QString::number(i_pEv->screenPos().x()) + "," + QString::number(i_pEv->screenPos().y()) + ")";
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strAddInfo   */ strAddTrcInfo );

    // When double clicking an item, the item will first receive a mouse
    // press event, followed by a release event (i.e., a click), then a
    // double click event, and finally a release event.
    // The default implementation of "mouseDoubleClickEvent" calls "mousePressEvent".

    CGraphObjGroup::mouseDoubleClickEvent(i_pEv);

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
protected: // overridables of base class CGraphObjGroup
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSwitch::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjKeyEvents != nullptr && m_pTrcAdminObjKeyEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Key:" + qKeyCode2Str(i_pEv->key());
        strAddTrcInfo += ", Ev.Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "keyPressEvent",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjGroup::keyPressEvent(i_pEv);

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Simulation )
    {
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
void CGraphObjSwitch::keyReleaseEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObjKeyEvents != nullptr && m_pTrcAdminObjKeyEvents->isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "Ev.Key:" + qKeyCode2Str(i_pEv->key());
        strAddTrcInfo += ", Ev.Modifiers:" + qKeyboardModifiers2Str(i_pEv->modifiers());
        strAddTrcInfo += ", Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjKeyEvents,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "keyReleaseEvent",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjGroup::keyReleaseEvent(i_pEv);

    CEnumMode modeDrawing = m_pDrawingScene->getMode();

    if( modeDrawing == EMode::Simulation )
    {
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
protected: // overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjSwitch::updateToolTip()
//------------------------------------------------------------------------------
{
    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(this);

    if( pGraphicsItem != nullptr )
    {
        QString strNodeSeparator = m_pDrawingScene->getGraphObjNameNodeSeparator();
        QPointF ptPos;

        m_strToolTip  = "ObjName:\t" + name();
        m_strToolTip += "\nObjId:\t\t" + keyInTree();

        m_strToolTip += "State:\t" + State2Str(m_state);

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
        m_strToolTip += "\nRotation:\t" + QString::number(m_fRotAngleCurr_deg,'f',1) + " " + ZS::PhysVal::c_strSymbolDegree;
        m_strToolTip += "\nZValue:\t\t" + QString::number(pGraphicsItem->zValue());

        pGraphicsItem->setToolTip(m_strToolTip);

    } // if( pGraphicsItem != nullptr )

} // updateToolTip
