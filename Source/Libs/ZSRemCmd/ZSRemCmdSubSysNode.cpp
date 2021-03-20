/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qthread.h>

#include "ZSRemCmd/ZSRemCmdSubSysNode.h"
#include "ZSRemCmd/ZSRemCmdObjPool.h"
#include "ZSRemCmd/ZSRemCmdServer.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::RemCmd;
using namespace ZS::Trace;


/*******************************************************************************
class CRemCmdSubSysNode : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdSubSysNode::CRemCmdSubSysNode( CRemCmdServer* i_pServer, const QString& i_strNode ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pServer(i_pServer),
    m_pObjPool(nullptr),
    m_strNode(i_strNode),
    m_strDscr(),
    m_pTreeEntry(nullptr),
    m_pTrcAdminObj(nullptr)
{
    if( m_pServer == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pRemCmdServer == nullptr");
    }
    if( m_pServer->getObjPool() == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pRemCmdServer->getObjPool() == nullptr");
    }

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmd", "CRemCmdSubSysNode", i_strNode);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pObjPool = m_pServer->getObjPool();

    m_pTreeEntry = m_pObjPool->addNameSpaceNode(i_strNode);

    m_pTreeEntry->setObj(this);

} // ctor

//------------------------------------------------------------------------------
CRemCmdSubSysNode::CRemCmdSubSysNode( CRemCmdObjPool* i_pObjPool, const QString& i_strNode ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pServer(nullptr),
    m_pObjPool(i_pObjPool),
    m_strNode(i_strNode),
    m_strDscr(),
    m_pTreeEntry(nullptr),
    m_pTrcAdminObj(nullptr)
{
    if( m_pObjPool == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObjPool == nullptr");
    }

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmd", "CRemCmdSubSysNode", i_strNode);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pTreeEntry = m_pObjPool->addNameSpaceNode(i_strNode);

    m_pTreeEntry->setObj(this);

} // ctor

//------------------------------------------------------------------------------
CRemCmdSubSysNode::~CRemCmdSubSysNode()
//------------------------------------------------------------------------------
{
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pServer = nullptr;
    m_pObjPool = nullptr;
    m_pTreeEntry = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdSubSysNode::setDescription( const QString& i_strDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strDscr;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDescription",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_strDscr != i_strDscr )
    {
        m_strDscr = i_strDscr;
        m_pObjPool->dataChanged(m_pTreeEntry);
        emit changed(this);
    }

} // setDescription

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CRemCmdSubSysNode::toXmlString() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "toXmlString",
        /* strAddInfo   */ "" );

    QString strXml;
    QString strNode = m_strNode;
    QString strDscr = m_strDscr;

    if( strNode.contains("\n") )
    {
        strNode.replace("\n","&lt;br&gt;");
    }
    if( strNode.contains('<') )
    {
        strNode.replace("<","&lt;");
    }
    if( strNode.contains('>') )
    {
        strNode.replace(">","&gt;");
    }

    if( strDscr.contains("\n") )
    {
        strDscr.replace("\n","&lt;br&gt;");
    }
    if( strDscr.contains('<') )
    {
        strDscr.replace("<","&lt;");
    }
    if( strDscr.contains('>') )
    {
        strDscr.replace(">","&gt;");
    }

    strXml += "<SubSysNode>";
    strXml += "<Node>" + strNode + "</Node>";
    strXml += "<Dscr>" + strDscr + "</Dscr>";
    strXml += "</SubSysNode>";

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        mthTracer.setMethodReturn(strXml);
    }

    return strXml;

} // toXmlString

