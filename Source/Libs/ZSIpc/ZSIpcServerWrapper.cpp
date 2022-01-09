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

#include "ZSIpc/ZSIpcServerWrapper.h"
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
class CIpcServerWrapper : public QObject
*******************************************************************************/

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CIpcServerWrapper::CIpcServerWrapper(
    const QString& i_strObjName,
    ESocketType    i_socketType,
    CTrcMthFile*   i_pTrcMthFile,
    int            i_iTrcMthFileDetailLevel ) :
//------------------------------------------------------------------------------
    QObject(),
    m_socketType(i_socketType),
    m_strLocalHostName(),
    m_hostAddrLocal(),
    m_uLocalPort(0),
    m_pServer(nullptr),
    m_iTrcMthFileDetailLevel(i_iTrcMthFileDetailLevel),
    m_pTrcMthFile(i_pTrcMthFile),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    // The derived classes must instantiate the trace admin object and trace the ctor.
    //if( m_pTrcMthFile == nullptr )
    //{
    //    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    //}

    //CMethodTracer mthTracer(
    //    /* pAdminObj          */ m_pTrcAdminObj,
    //    /* pTrcMthFile        */ m_pTrcMthFile,
    //    /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
    //    /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
CIpcServerWrapper::~CIpcServerWrapper()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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

    m_socketType = static_cast<ESocketType>(0);
    //m_strLocalHostName;
    //m_hostAddrLocal;
    m_uLocalPort = 0;
    m_pServer = nullptr;
    m_iTrcMthFileDetailLevel = 0;
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ESocketType CIpcServerWrapper::socketType() const
//------------------------------------------------------------------------------
{
    return m_socketType;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcServerWrapper::setLocalHostName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalHostName",
        /* strAddInfo         */ strAddTrcInfo );

    m_strLocalHostName = i_strName;

} // setLocalHostName

//------------------------------------------------------------------------------
void CIpcServerWrapper::setLocalPort( unsigned int i_uPort )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( isMethodTraceActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString::number(i_uPort);
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalPort",
        /* strAddInfo         */ strAddTrcInfo );

    m_uLocalPort = i_uPort;

} // setLocalPort

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CIpcServerWrapper::isMethodTraceActive( int i_iFilterDetailLevel ) const
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(i_iFilterDetailLevel) )
    {
        bIsActive = true;
    }
    else if( m_pTrcMthFile != nullptr && m_iTrcMthFileDetailLevel >= i_iFilterDetailLevel )
    {
        bIsActive = true;
    }
    return bIsActive;

} // isMethodTraceActive

//------------------------------------------------------------------------------
int CIpcServerWrapper::getMethodTraceDetailLevel() const
//------------------------------------------------------------------------------
{
    int iDetailLevel = ETraceDetailLevelNone;

    if( m_pTrcAdminObj != nullptr )
    {
        iDetailLevel = getMethodTraceDetailLevel();
    }
    else if( m_pTrcMthFile != nullptr )
    {
        iDetailLevel = m_iTrcMthFileDetailLevel;
    }
    return iDetailLevel;

} // getMethodTraceDetailLevel

