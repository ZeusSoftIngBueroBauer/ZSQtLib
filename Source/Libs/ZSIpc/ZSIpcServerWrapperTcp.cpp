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

#include <QtNetwork/qhostinfo.h>
#include <QtNetwork/qtcpserver.h>

#include "ZSIpc/ZSIpcServerWrapperTcp.h"
#include "ZSIpc/ZSIpcSocketWrapperTcp.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;


/*******************************************************************************
class CTcpServerWrapper : public CIpcServerWrapper
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTcpServerWrapper::CTcpServerWrapper(
    const QString& i_strObjName,
    CTrcMthFile*   i_pTrcMthFile,
    int            i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    CIpcServerWrapper(i_strObjName, ESocketTypeTcp, i_pTrcMthFile, i_iTrcMthFileDetailLevel),
    m_pTcpServer(nullptr)
{
    // The derived classes must instantiate the trace admin object and trace the ctor.
    if( m_pTrcMthFile == nullptr && !i_strObjName.contains("TrcServer") )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strAddInfo         */ "" );

    m_pServer = m_pTcpServer = new QTcpServer();

} // ctor

//------------------------------------------------------------------------------
CTcpServerWrapper::~CTcpServerWrapper()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strAddInfo         */ "" );

    try
    {
        delete m_pTcpServer;
    }
    catch(...)
    {
    }
    m_pTcpServer = nullptr;

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

} // dtor

/*==============================================================================
public: // must overridables of base class CIpcServerWrapper
==============================================================================*/

//------------------------------------------------------------------------------
void CTcpServerWrapper::setMaxPendingConnections( int i_iMaxConnections )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString::number(i_iMaxConnections);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setMaxPendingConnections",
        /* strAddInfo         */ strAddTrcInfo );

    m_pTcpServer->setMaxPendingConnections(i_iMaxConnections);

} // setMaxPendingConnections

//------------------------------------------------------------------------------
int CTcpServerWrapper::maxPendingConnections() const
//------------------------------------------------------------------------------
{
    return m_pTcpServer->maxPendingConnections();
}

//------------------------------------------------------------------------------
bool CTcpServerWrapper::listen()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "listen",
        /* strAddInfo         */ "" );

    if( !QObject::connect(
        /* pObjSender   */ m_pTcpServer,
        /* szSignal     */ SIGNAL(newConnection()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onNewConnection()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    return m_pTcpServer->listen( QHostAddress::Any, m_uLocalPort );

} // listen

//------------------------------------------------------------------------------
bool CTcpServerWrapper::isListening() const
//------------------------------------------------------------------------------
{
    return m_pTcpServer->isListening();
}

//------------------------------------------------------------------------------
void CTcpServerWrapper::close()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "close",
        /* strAddInfo         */ "" );

    if( !QObject::disconnect(
        /* pObjSender   */ m_pTcpServer,
        /* szSignal     */ SIGNAL(newConnection()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onNewConnection()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    m_pTcpServer->close();

} // close

//------------------------------------------------------------------------------
bool CTcpServerWrapper::hasPendingConnections() const
//------------------------------------------------------------------------------
{
    return m_pTcpServer->hasPendingConnections();
}

//------------------------------------------------------------------------------
CIpcSocketWrapper* CTcpServerWrapper::nextPendingConnection()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "nextPendingConnection",
        /* strAddInfo         */ "" );

    QTcpSocket* pTcpSocket = m_pTcpServer->nextPendingConnection();

    CTcpSocketWrapper* pSocketWrapper = new CTcpSocketWrapper(
        /* strObjName             */ objectName(),
        /* srvCltType             */ ESrvCltTypeServer,
        /* iSocketId              */ ESocketIdUndefined,
        /* pTcpSocket             */ pTcpSocket,
        /* pTrcMthFile            */ m_pTrcMthFile,
        /* iTrcMthFileDetailLevel */ m_iTrcMthFileDetailLevel );

    pSocketWrapper->setServerListenPort(m_uLocalPort);

    pSocketWrapper->setLocalPort( pTcpSocket->localPort() );
    pSocketWrapper->setLocalHostAddress( pTcpSocket->localAddress() );
    pSocketWrapper->setRemotePort( pTcpSocket->peerPort() );
    pSocketWrapper->setRemoteHostAddress( pTcpSocket->peerAddress() );

    QString strRemoteHostName = pTcpSocket->peerName();
    QString strRemoteHostAddr = pTcpSocket->peerAddress().toString();

    if( strRemoteHostName.isEmpty() )
    {
        strRemoteHostName = strRemoteHostAddr;
    }

    pSocketWrapper->setLocalHostName(m_strLocalHostName);
    pSocketWrapper->setRemoteHostName(strRemoteHostName);

    return pSocketWrapper;

} // nextPendingConnection

//------------------------------------------------------------------------------
ZS::System::EResult CTcpServerWrapper::error() const
//------------------------------------------------------------------------------
{
    return CTcpSocketWrapper::TcpSocketError2Result( m_pTcpServer->serverError() );
}

//------------------------------------------------------------------------------
QString CTcpServerWrapper::errorString() const
//------------------------------------------------------------------------------
{
    return m_pTcpServer->errorString();
}

//------------------------------------------------------------------------------
SErrResultInfo CTcpServerWrapper::errResultInfo() const
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo;

    errResultInfo.setSeverity(EResultSeverityError);
    errResultInfo.setResult(error());
    errResultInfo.setAddErrInfoDscr(errorString());

    return errResultInfo;

} // errResultInfo

/*==============================================================================
protected slots: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTcpServerWrapper::onNewConnection()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onNewConnection",
        /* strAddInfo         */ "" );

    emit CIpcServerWrapper::newConnection(this);

} // onNewConnection
