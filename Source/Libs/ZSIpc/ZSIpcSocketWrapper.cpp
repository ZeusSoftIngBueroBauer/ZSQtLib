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

#include "ZSIpc/ZSIpcSocketWrapper.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;


/*******************************************************************************
class CIpcSocketWrapper : public QObject
*******************************************************************************/

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CIpcSocketWrapper::CIpcSocketWrapper(
    const QString& i_strObjName,
    ESrvCltType    i_srvCltType,
    ESocketType    i_socketType,
    int            i_iSocketId,
    QObject*       i_pSocket,
    CTrcMthFile*   i_pTrcMthFile,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QObject(),
    m_socketDscr(i_srvCltType, i_socketType),
    m_fLastTimeDataBlockReceived_ms(ZS::System::Time::getProcTimeInMilliSec()),
    m_pSocket(i_pSocket),
    m_eTrcMthFileDetailLevel(i_eTrcMthFileDetailLevel),
    m_pTrcMthFile(i_pTrcMthFile),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    // The derived classes must instantiate the trace admin object and trace the ctor.
    //if( m_pTrcMthFile == nullptr && !i_strObjName.contains("TrcServer") && !i_strObjName.contains("TrcClient") )
    //{
    //    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    //}

    //CMethodTracer mthTracer(
    //    /* pAdminObj          */ m_pTrcAdminObj,
    //    /* pTrcMthFile        */ m_pTrcMthFile,
    //    /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
    //    /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
    //    /* strNameSpace       */ NameSpace(),
    //    /* strClassName       */ ClassName(),
    //    /* strObjName         */ objectName(),
    //    /* strMethod          */ "ctor",
    //    /* strAddInfo         */ "" );

    m_socketDscr.m_iSocketId = i_iSocketId;

} // ctor

//------------------------------------------------------------------------------
CIpcSocketWrapper::CIpcSocketWrapper(
    const QString&     i_strObjName,
    const SSocketDscr& i_socketDscr,
    QObject*           i_pSocket,
    CTrcMthFile*       i_pTrcMthFile,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QObject(),
    m_socketDscr(i_socketDscr),
    m_fLastTimeDataBlockReceived_ms(ZS::System::Time::getProcTimeInMilliSec()),
    m_pSocket(i_pSocket),
    m_eTrcMthFileDetailLevel(i_eTrcMthFileDetailLevel),
    m_pTrcMthFile(i_pTrcMthFile),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    // The derived classes must instantiate the trace admin object and trace the ctor.
    //if( m_pTrcMthFile == nullptr && !i_strObjName.contains("TrcServer") && !i_strObjName.contains("TrcClient") )
    //{
    //    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    //}

    //CMethodTracer mthTracer(
    //    /* pAdminObj          */ m_pTrcAdminObj,
    //    /* pTrcMthFile        */ m_pTrcMthFile,
    //    /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
    //    /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
    //    /* strNameSpace       */ NameSpace(),
    //    /* strClassName       */ ClassName(),
    //    /* strObjName         */ objectName(),
    //    /* strMethod          */ "ctor",
    //    /* strAddInfo         */ "" );

} // ctor


/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CIpcSocketWrapper::~CIpcSocketWrapper()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strAddInfo         */ "" );

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

    //m_socketDscr;
    m_fLastTimeDataBlockReceived_ms = 0.0;
    m_pSocket = nullptr;
    m_eTrcMthFileDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcSocketWrapper::setSocketId( int i_iId )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString::number(i_iId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setSocketId",
        /* strAddInfo         */ strAddTrcInfo );

    // This method should only be called once right after creating an instance of this class.
    m_socketDscr.m_iSocketId = i_iId;

} // setSocketId

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcSocketWrapper::setServerListenPort( unsigned int i_uPort )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString::number(i_uPort);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setServerListenPort",
        /* strAddInfo         */ strAddTrcInfo );

    m_socketDscr.m_uServerListenPort = i_uPort;

} // setServerListenPort

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcSocketWrapper::setLocalHostName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = i_strName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalHostName",
        /* strAddInfo         */ strAddTrcInfo );

    m_socketDscr.m_strLocalHostName = i_strName;

} // setLocalHostName

//------------------------------------------------------------------------------
void CIpcSocketWrapper::setLocalHostAddress( const QHostAddress& i_hostAddr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = i_hostAddr.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalHostAddress",
        /* strAddInfo         */ strAddTrcInfo );

    m_socketDscr.m_hostAddrLocal = i_hostAddr;

} // setLocalHostAddress

//------------------------------------------------------------------------------
void CIpcSocketWrapper::setLocalPort( unsigned int i_uPort )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString::number(i_uPort);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalPort",
        /* strAddInfo         */ strAddTrcInfo );

    m_socketDscr.m_uLocalPort = i_uPort;

} // setLocalPort

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcSocketWrapper::setRemoteHostName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = i_strName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setRemoteHostName",
        /* strAddInfo         */ strAddTrcInfo );

    m_socketDscr.m_strRemoteHostName = i_strName;

} // setRemoteHostName

//------------------------------------------------------------------------------
void CIpcSocketWrapper::setRemoteHostAddress( const QHostAddress& i_hostAddr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = i_hostAddr.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setRemoteHostAddress",
        /* strAddInfo         */ strAddTrcInfo );

    m_socketDscr.m_hostAddrRemote = i_hostAddr;

} // setRemoteHostAddress

//------------------------------------------------------------------------------
void CIpcSocketWrapper::setRemotePort( unsigned int i_uPort )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString::number(i_uPort);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setRemotePort",
        /* strAddInfo         */ strAddTrcInfo );

    m_socketDscr.m_uRemotePort = i_uPort;

} // setRemotePort

//------------------------------------------------------------------------------
void CIpcSocketWrapper::setBufferSize( unsigned int i_uBufferSize )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString::number(i_uBufferSize);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setBufferSize",
        /* strAddInfo         */ strAddTrcInfo );

    m_socketDscr.m_uBufferSize = i_uBufferSize;

} // setBufferSize

//------------------------------------------------------------------------------
void CIpcSocketWrapper::setLastTimeDataBlockReceivedInMs( double i_fTime_ms )
//------------------------------------------------------------------------------
{
    m_fLastTimeDataBlockReceived_ms = i_fTime_ms;
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CIpcSocketWrapper::areTraceMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(i_eFilterDetailLevel) )
    {
        bIsActive = true;
    }
    else if( m_pTrcMthFile != nullptr && m_eTrcMthFileDetailLevel >= i_eFilterDetailLevel )
    {
        bIsActive = true;
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
EMethodTraceDetailLevel CIpcSocketWrapper::getMethodCallsTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    EMethodTraceDetailLevel eDetailLevel = EMethodTraceDetailLevel::None;

    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getMethodCallsTraceDetailLevel();
    }
    else if( m_pTrcMthFile != nullptr )
    {
        eDetailLevel = m_eTrcMthFileDetailLevel;
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
bool CIpcSocketWrapper::isTraceRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isRuntimeInfoActive(i_eFilterDetailLevel) )
    {
        bIsActive = true;
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
ELogDetailLevel CIpcSocketWrapper::getRuntimeInfoTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    ELogDetailLevel eDetailLevel = ELogDetailLevel::None;

    if( m_pTrcAdminObj != nullptr )
    {
        eDetailLevel = m_pTrcAdminObj->getRuntimeInfoTraceDetailLevel();
    }
    return eDetailLevel;
}
