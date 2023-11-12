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

#include <QtNetwork/qhostinfo.h>

#include "ZSIpc/ZSIpcServerWrapperInProcMsg.h"
#include "ZSIpc/ZSIpcSocketWrapperInProcMsg.h"
#include "ZSIpc/ZSIpcInProcMsgServer.h"
#include "ZSIpc/ZSIpcInProcMsgSocket.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;


/*******************************************************************************
class CInProcMsgServerWrapper : public CIpcServerWrapper
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInProcMsgServerWrapper::CInProcMsgServerWrapper(
    const QString& i_strObjName,
    CTrcMthFile*   i_pTrcMthFile,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CIpcServerWrapper(i_strObjName, ESocketTypeInProcMsg, i_pTrcMthFile, i_eTrcMthFileDetailLevel),
    m_pInProcMsgServer(nullptr)
{
    // The derived classes must instantiate the trace admin object and trace the ctor.
    if( m_pTrcMthFile == nullptr && !i_strObjName.contains("TrcServer") )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strAddInfo         */ "" );

    m_pServer = m_pInProcMsgServer = new CInProcMsgServer(i_strObjName);

    m_strLocalHostName = QHostInfo::localHostName();

} // ctor

//------------------------------------------------------------------------------
CInProcMsgServerWrapper::~CInProcMsgServerWrapper()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strAddInfo         */ "" );

    try
    {
        delete m_pInProcMsgServer;
    }
    catch(...)
    {
    }
    m_pInProcMsgServer = nullptr;

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ECopyDepth CInProcMsgServerWrapper::getMsgReadBuffCopyDepth() const
//------------------------------------------------------------------------------
{
    return m_pInProcMsgServer->getMsgReadBuffCopyDepth();
}

//------------------------------------------------------------------------------
void CInProcMsgServerWrapper::setMsgReadBuffCopyDepth( ECopyDepth i_copyDepth )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = CEnumCopyDepth::toString(i_copyDepth);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setMsgReadBuffCopyDepth",
        /* strAddInfo         */ strAddTrcInfo );

    m_pInProcMsgServer->setMsgReadBuffCopyDepth(i_copyDepth);

} // setMsgReadBuffCopyDepth

/*==============================================================================
public: // must overridables of base class CIpcServerWrapper
==============================================================================*/

//------------------------------------------------------------------------------
void CInProcMsgServerWrapper::setMaxPendingConnections( int i_iMaxConnections )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString::number(i_iMaxConnections);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setMaxPendingConnections",
        /* strAddInfo         */ strAddTrcInfo );

    m_pInProcMsgServer->setMaxPendingConnections(i_iMaxConnections);

} // setMaxPendingConnections

//------------------------------------------------------------------------------
int CInProcMsgServerWrapper::maxPendingConnections() const
//------------------------------------------------------------------------------
{
    return m_pInProcMsgServer->maxPendingConnections();
}

//------------------------------------------------------------------------------
bool CInProcMsgServerWrapper::listen()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "listen",
        /* strAddInfo         */ "" );

    QObject::connect(
        m_pInProcMsgServer, &CInProcMsgServer::newConnection,
        this, &CInProcMsgServerWrapper::onNewConnection);

    return m_pInProcMsgServer->listen(m_uLocalPort);

} // listen

//------------------------------------------------------------------------------
bool CInProcMsgServerWrapper::isListening() const
//------------------------------------------------------------------------------
{
    return m_pInProcMsgServer->isListening();
}

//------------------------------------------------------------------------------
void CInProcMsgServerWrapper::close()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "close",
        /* strAddInfo         */ "" );

    QObject::disconnect(
        m_pInProcMsgServer, &CInProcMsgServer::newConnection,
        this, &CInProcMsgServerWrapper::onNewConnection);

    m_pInProcMsgServer->close();

} // close

//------------------------------------------------------------------------------
bool CInProcMsgServerWrapper::hasPendingConnections() const
//------------------------------------------------------------------------------
{
    return m_pInProcMsgServer->hasPendingConnections();
}

//------------------------------------------------------------------------------
CIpcSocketWrapper* CInProcMsgServerWrapper::nextPendingConnection()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "nextPendingConnection",
        /* strAddInfo         */ "" );

    CInProcMsgSocket* pInProcMsgSocket = m_pInProcMsgServer->nextPendingConnection();

    QString strSocketName = objectName() + "-" + pInProcMsgSocket->getRemoteHostName();

    CInProcMsgSocketWrapper* pSocketWrapper = new CInProcMsgSocketWrapper(
        /* strObjName             */ strSocketName,
        /* srvCltType             */ ESrvCltTypeServer,
        /* iSocketId              */ ESocketIdUndefined,
        /* pTcpSocket             */ pInProcMsgSocket,
        /* pTrcMthFile            */ m_pTrcMthFile,
        /* iTrcMthFileDetailLevel */ m_eTrcMthFileDetailLevel );

    return pSocketWrapper;

} // nextPendingConnection

//------------------------------------------------------------------------------
ZS::System::EResult CInProcMsgServerWrapper::error() const
//------------------------------------------------------------------------------
{
    return m_pInProcMsgServer->error();
}

//------------------------------------------------------------------------------
QString CInProcMsgServerWrapper::errorString() const
//------------------------------------------------------------------------------
{
    return m_pInProcMsgServer->errorString();
}

//------------------------------------------------------------------------------
SErrResultInfo CInProcMsgServerWrapper::errResultInfo() const
//------------------------------------------------------------------------------
{
    return m_pInProcMsgServer->errResultInfo();
}


/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CInProcMsgServerWrapper::onNewConnection()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onNewConnection",
        /* strAddInfo         */ "" );

    emit CIpcServerWrapper::newConnection(this);

} // onNewConnection
