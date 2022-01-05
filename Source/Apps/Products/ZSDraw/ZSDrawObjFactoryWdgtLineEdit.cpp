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

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsItem>
#else
#include <QtWidgets/QGraphicsItem>
#endif

#include "ZSDrawObjFactoryWdgtLineEdit.h"
#include "ZSDrawGraphObjWdgtLineEdit.h"

#include "ZSDraw/ZSDrawGraphObjGroup.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::QtWidgets;
using namespace ZS::Trace;


/*******************************************************************************
class CObjFactoryWdgtLineEdit : public CObjFactory
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CObjFactoryWdgtLineEdit::CObjFactoryWdgtLineEdit( const QIcon& i_toolIcon ) :
//------------------------------------------------------------------------------
    CObjFactory(
        /* strGroupName    */ CGraphObjWdgt::c_strFactoryGroupName,
        /* iGraphObjType   */ EGraphObjTypeUserDefined,
        /* strGraphObjType */ "LineEdit",
        /* toolIcon        */ i_toolIcon )
{
} // default ctor

//------------------------------------------------------------------------------
CObjFactoryWdgtLineEdit::~CObjFactoryWdgtLineEdit()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // interface methods
==============================================================================*/

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryWdgtLineEdit::createGraphObj(
    CDrawingScene*       i_pDrawingScene,
    const QPointF&       i_ptItemPos,
    const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "ItemPos:" + QString::number(i_ptItemPos.x()) + "," + QString::number(i_ptItemPos.y());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObj* pGraphObj = new CGraphObjWdgtLineEdit(
        /* pDrawingScene */ i_pDrawingScene,
        /* drawSettings  */ i_drawSettings );

    return pGraphObj;

} // createGraphObj

//------------------------------------------------------------------------------
SErrResultInfo CObjFactoryWdgtLineEdit::saveGraphObj(
    CGraphObj*        i_pGraphObj,
    QXmlStreamWriter& /*i_xmlStreamWriter*/ )
//------------------------------------------------------------------------------
{
    if( i_pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pGraphObj == nullptr" );
    }

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GraphObj:" + i_pGraphObj->nameSpace();
        strAddTrcInfo += "::" + i_pGraphObj->className();
        strAddTrcInfo += "::" + i_pGraphObj->name();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "saveGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo;

    CGraphObjWdgtLineEdit* pGraphObj = dynamic_cast<CGraphObjWdgtLineEdit*>(i_pGraphObj);

    if( pGraphObj == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObjWdgtLineEdit == nullptr" );
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // saveGraphObj

//------------------------------------------------------------------------------
CGraphObj* CObjFactoryWdgtLineEdit::loadGraphObj(
    CDrawingScene*    i_pDrawingScene,
    CGraphObjGroup*   /*i_pGraphObjGroup*/,
    const QString&    /*i_strObjName*/,
    const QString&    /*i_strObjId*/,
    QXmlStreamReader& /*i_xmlStreamReader*/,
    SErrResultInfo&   io_errResultInfo )
//------------------------------------------------------------------------------
{
    if( i_pDrawingScene == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "pDrawingScene == nullptr" );
    }

    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "loadGraphObj",
        /* strAddInfo   */ strAddTrcInfo );

    CGraphObjWdgtLineEdit* pGraphObj = nullptr;

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "ErrResultInfo {" + io_errResultInfo.toString() + "}";
        mthTracer.setMethodOutArgs(strAddTrcInfo);
    }

    return pGraphObj;

} // loadGraphObj
