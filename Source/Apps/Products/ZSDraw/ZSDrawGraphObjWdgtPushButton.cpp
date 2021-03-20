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
#include <QtGui/QPushButton>
#include <QtGui/QGraphicsSceneEvent>
#include <QtGui/QStyleOption>
#else
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtWidgets/QStyleOption>
#endif

#include "ZSDrawGraphObjWdgtPushButton.h"

#include "ZSDraw/ZSDrawGraphObjGroup.h"
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
using namespace ZS::Draw::QtWidgets;
using namespace ZS::PhysVal;
using namespace ZS::Trace;


/*******************************************************************************
class CGraphObjWdgtPushButton : public CGraphObjWdgt
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CGraphObjWdgtPushButton::ResetCtorsDtorsCounters()
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

qint64 CGraphObjWdgtPushButton::s_iCtorsCount = 1;
qint64 CGraphObjWdgtPushButton::s_iDtorsCount = 1;

CTrcAdminObj* CGraphObjWdgtPushButton::s_pTrcAdminObjCtorsAndDtor = nullptr;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjWdgtPushButton::CGraphObjWdgtPushButton(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName,
    const QString&       i_strObjId ) :
//------------------------------------------------------------------------------
    CGraphObjWdgt(
        /* pDrawingScene */ i_pDrawingScene,
        /* strNameSpace  */ "ZS::Draw::QtWidgets",
        /* strClassName  */ "CGraphObjWdgtPushButton",
        /* strType       */ "PushButton",
        /* strObjName    */ i_strObjName.isEmpty() ? "PushButton" + QString::number(s_iCtorsCount) : i_strObjName,
        /* strObjId      */ i_strObjId.isEmpty() ? "PushButton" + QString::number(s_iCtorsCount) : i_strObjId,
        /* drawSettings  */ i_drawSettings ),
    m_pPushButton(nullptr)
{
    s_iCtorsCount++;

    QString strAddTrcInfo;

    if( s_pTrcAdminObjCtorsAndDtor == nullptr )
    {
        s_pTrcAdminObjCtorsAndDtor = CTrcServer::GetTraceAdminObj("ZS::Draw::QtWidgets", "CGraphObjWdgtPushButton::CtorsAndDtor", "");
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

    m_pPushButton = new QPushButton();

    setWidget(m_pPushButton);

    QRectF rctBounding = QGraphicsProxyWidget::boundingRect();

    m_ptRotOriginCurr = rctBounding.center();

    setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges );

    //setAcceptHoverEvents(true);

    m_drawSettings.setAttributeIsUsed(EDrawAttributeTextColor,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeTextFont,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeTextStyle,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeTextSize,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeTextEffect,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeFillColor,true);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeFillStyle,true);
    m_drawSettings.setAttributeIsUsed(EDrawAttributePenColor,true);
    m_drawSettings.setAttributeIsUsed(EDrawAttributePenWidth,true);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineStyle,true);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineRecordType,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineExtent,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineStartLineEndStyle,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineStartLineEndBaseLineType,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineStartLineEndFillStyle,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineStartLineEndWidth,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineStartLineEndLength,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineEndLineEndStyle,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineEndLineEndBaseLineType,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineEndLineEndFillStyle,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineEndLineEndWidth,false);
    m_drawSettings.setAttributeIsUsed(EDrawAttributeLineEndLineEndLength,false);

    onDrawSettingsChanged();

    updateToolTip();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo  = "Selected:" + bool2Str(isSelected());
        strAddTrcInfo += ", EditMode:" + editMode2Str(m_editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(m_editResizeMode);
        strAddTrcInfo += ", SelectedPoint:" + selectionPoint2Str(m_selPtSelectedBoundingRect);
        strAddTrcInfo += ", BoundingRect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjWdgtPushButton::~CGraphObjWdgtPushButton()
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

    m_pPushButton = nullptr;

    if( s_iCtorsCount == s_iDtorsCount )
    {
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObjCtorsAndDtor);
        s_pTrcAdminObjCtorsAndDtor = nullptr;
    }

} // dtor

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjWdgtPushButton::clone()
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

    CGraphObjWdgtPushButton* pGraphObj = nullptr;

    return pGraphObj;

} // clone
