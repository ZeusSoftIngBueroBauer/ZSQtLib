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
#include <QtGui/QLabel>
#include <QtGui/QGraphicsSceneEvent>
#include <QtGui/QStyleOption>
#else
#include <QtWidgets/QLabel>
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtWidgets/QStyleOption>
#endif

#include "ZSDrawGraphObjWdgtLabel.h"

#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
#include "ZSDraw/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawObjFactory.h"
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
using namespace ZS::Draw::QtWidgets;
using namespace ZS::PhysVal;
using namespace ZS::Trace;


/*******************************************************************************
class CGraphObjWdgtLabel : public CGraphObjWdgt
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

qint64 CGraphObjWdgtLabel::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObjWdgtLabel::CGraphObjWdgtLabel(
    CDrawingScene*       i_pDrawingScene,
    const CDrawSettings& i_drawSettings,
    const QString&       i_strObjName ) :
//------------------------------------------------------------------------------
    CGraphObjWdgt(
        /* pDrawingScene */ i_pDrawingScene,
        /* strType       */ "Label",
        /* strObjName    */ i_strObjName.isEmpty() ? "Label" + QString::number(s_iInstCount) : i_strObjName,
        /* drawSettings  */ i_drawSettings ),
    m_pLabel(nullptr)
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

    m_pLabel = new QLabel();

    setWidget(m_pLabel);

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
        strAddTrcInfo += ", EditMode:" + m_editMode.toString();
        strAddTrcInfo += ", ResizeMode:" + m_editResizeMode.toString();
        strAddTrcInfo += ", SelectedPoint:" + m_selPtSelectedBoundingRect.toString();
        strAddTrcInfo += ", BoundingRect(x,y,w,h):(" + QString::number(rctBounding.x()) + "," + QString::number(rctBounding.y());
        strAddTrcInfo += "," + QString::number(rctBounding.width()) + "," + QString::number(rctBounding.height()) + ")";
        mthTracer.trace(strAddTrcInfo);
    }

} // ctor

//------------------------------------------------------------------------------
CGraphObjWdgtLabel::~CGraphObjWdgtLabel()
//------------------------------------------------------------------------------
{
    m_bDtorInProgress = true;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjCtorsAndDtor,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ m_strName,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    m_pLabel = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CGraphObj
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CGraphObjWdgtLabel::clone()
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

    CGraphObjWdgtLabel* pGraphObj = nullptr;

    return pGraphObj;

} // clone
