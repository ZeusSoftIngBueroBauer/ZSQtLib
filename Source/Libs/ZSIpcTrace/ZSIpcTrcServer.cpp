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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qmutex.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequest.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcMthFile.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;


//******************************************************************************
struct ZS::Trace::SMthTrcData
//******************************************************************************
{
//==============================================================================
public: // struct members
//==============================================================================

/*!< Name of the thread in which context the method was called from. */
QString m_strMthThreadName;
/*!< Current date time of the method call. */
QDateTime m_dt;
/*!< Time in seconds since start of the program the method was called. */
double m_fSysTime_s;
/*!< Methods may be entered and left. */
ZS::System::EMethodDir m_mthDir;
/*!< Index of trace admin object in index tree. */
int m_iTrcAdminObjId;
/*!< Name space of the methods class. */
QString m_strNameSpace;
/*!< Name of the class the method belongs to. */
QString m_strClassName;
/*!< Name of the object for which the method was applied. */
QString m_strObjName;
/*!< Name of the method to be traced. */
QString m_strMthName;
/*!< Only valid if mthDir == Enter */
QString m_strMthInArgs;
/*!< Only valid if mthDir == Leave */
QString m_strMthOutArgs;
/*!< Only valid if mthDir == Leave */
QString m_strMthRet;
/*!< Only valid if mthDir == None */
QString m_strMthAddInfo;

//==============================================================================
public: // ctors and dtor
//==============================================================================

//------------------------------------------------------------------------------
SMthTrcData() :
//------------------------------------------------------------------------------
    m_strMthThreadName(),
    m_dt(),
    m_fSysTime_s(0.0),
    m_mthDir(EMethodDir::None),
    m_iTrcAdminObjId(-1),
    m_strNameSpace(),
    m_strClassName(),
    m_strObjName(),
    m_strMthName(),
    m_strMthInArgs(),
    m_strMthOutArgs(),
    m_strMthRet(),
    m_strMthAddInfo()
{
} // ctor

//------------------------------------------------------------------------------
SMthTrcData(
    const QString&         i_strMthThreadName,
    const QDateTime&       i_dt,
    double                 i_fSysTimeInSec,
    ZS::System::EMethodDir i_mthDir,
    int                    i_iTrcAdminObjId,
    const QString&         i_strNameSpace,
    const QString&         i_strClassName,
    const QString&         i_strObjName,
    const QString&         i_strMthName,
    const QString&         i_strAddInfo,
    const QString&         i_strMethodOutArgs = "" ) :
//------------------------------------------------------------------------------
    m_strMthThreadName(i_strMthThreadName),
    m_dt(i_dt),
    m_fSysTime_s(i_fSysTimeInSec),
    m_mthDir(i_mthDir),
    m_iTrcAdminObjId(i_iTrcAdminObjId),
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_strMthName(i_strMthName),
    m_strMthInArgs(),
    m_strMthOutArgs(),
    m_strMthRet(),
    m_strMthAddInfo()
{
    if( i_mthDir == EMethodDir::Enter )
    {
        m_strMthInArgs = i_strAddInfo;
    }
    else if( i_mthDir == EMethodDir::Leave )
    {
        m_strMthOutArgs = i_strMethodOutArgs;
        m_strMthRet = i_strAddInfo;
    }
    else if( i_mthDir == EMethodDir::None )
    {
        m_strMthAddInfo = i_strAddInfo;
    }
} // ctor

//------------------------------------------------------------------------------
~SMthTrcData()
//------------------------------------------------------------------------------
{
    //m_strMthThreadName;
    //m_dt;
    //m_fSysTime_s;
    m_mthDir = static_cast<EMethodDir>(0);
    m_iTrcAdminObjId = 0;
    //m_strNameSpace;
    //m_strClassName;
    //m_strObjName;
    //m_strMthName;
    //m_strMthInArgs;
    //m_strMthOutArgs;
    //m_strMthRet;
    //m_strMthAddInfo;

} // dtor

//==============================================================================
public: // struct methods
//==============================================================================

//------------------------------------------------------------------------------
QString toXmlString() const
//------------------------------------------------------------------------------
{
    QString str;

    str.reserve(240);

    /*   Len */
    /*    12 */ str += "<TrcData>";
    /*    25 */ str += "<TrcAdminObj IdxInTree=\"" + QString::number(m_iTrcAdminObjId) + "\"/>";
    /*     8 */ str += "<Method ";
    /*    40 */ str += "ObjName=\"" + m_strObjName + "\" ";
    /*    20 */ str += "Name=\"" + m_strMthName + "\" ";
    /*    20 */ str += "Thread=\"" + m_strMthThreadName + "\" ";
    /*    11 */ str += "Dir=\"" + CEnumMethodDir::toString(m_mthDir) + "\" ";
    /*    23 */ str += "DateTime=\"" + m_dt.toString("yyyy-MM-dd hh:mm:ss:zzz") + "\" ";
    /*    23 */ str += "SysTime=\"" + QString::number(m_fSysTime_s,'f',6) + "\" ";
    if( m_mthDir == EMethodDir::Enter )
    {
        QString strMthInArgs = encodeForXml(m_strMthInArgs);
    /*    10 */ str += "InArgs=\"" + strMthInArgs + "\" ";
    }
    else if( m_mthDir == EMethodDir::Leave )
    {
        QString strMthRet = encodeForXml(m_strMthRet);
        QString strMthOutArgs = encodeForXml(m_strMthOutArgs);
    /*    10 */ str += "Return=\"" + strMthRet + "\" ";
    /*    10 */ str += "OutArgs=\"" + strMthOutArgs + "\" ";
    }
    else // if( i_dir == EMethodDir::None )
    {
        QString strMthAddInfo = encodeForXml(m_strMthAddInfo);
    /*    10 */ str += "AddInfo=\"" + strMthAddInfo + "\" ";
    }
    /*     3 */ str += "/>";
    /*    13 */ str += "</TrcData>";
    /*-------*/
    /* = 207 ... estimated value */

    return str;

} // toXmlString

}; // struct SMthTrcData


/*******************************************************************************
class CIpcTrcServer : public ZS::Trace::CTrcServer
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the address of the trace server.

    This method does neither create an instance of the class nor increments the reference counter.
    If no instance has been created yet the method returns nullptr.

    If you just need to access the already existing instance and you can be sure that the instance
    is already existing this method should be preferred to the createInstance call as this method
    does not affect the reference counter and there is no need to call releaseInstance later on.

    @note After a getInstance call a releaseInstance MUST NOT be called.

    @return Pointer to trace server or nullptr, if the instance has not been created yet.
*/
CIpcTrcServer* CIpcTrcServer::GetInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CIpcTrcServer* pIpcTrcServer = nullptr;

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pIpcTrcServer = dynamic_cast<CIpcTrcServer*>(pTrcServer);
    }
    return pIpcTrcServer;

} // GetInstance

//------------------------------------------------------------------------------
/*! Creates the trace server if the trace server is not already existing.

    If the trace server is already existing the reference to the existing
    trace server is returned and a reference counter is incremented.

    @param i_eTrcDetailLevel [in]
        If the methods of the trace server itself should be logged a value
        greater than 0 (EMethodTraceDetailLevel::None) could be passed here.
    @param i_eTrcDetailLevelMutex [in]
        If locking and unlocking the mutex of the trace server should be
        logged a value greater than 0 (EMethodTraceDetailLevel::None) could be passed here.
    @param i_eTrcDetailLevelIpcServer [in]
        If the methods of the trace server's Ipc Server should be logged
        a value greater than 0 (EMethodTraceDetailLevel::None) could be passed here.
    @param i_eTrcDetailLevelIpcServerMutex [in]
        If locking and unlocking the mutex of the trace server's Ipc Server should be
        logged a value greater than 0 (EMethodTraceDetailLevel::None) could be passed here.
    @param i_eTrcDetailLevelIpcServerGateway [in]
        If the methods of the trace server's Ipc Server's Gateway should
        be logged a value greater than 0 (EMethodTraceDetailLevel::None) could be
        passed here.

    @return Pointer to trace server instance.
*/
CIpcTrcServer* CIpcTrcServer::CreateInstance(
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eTrcDetailLevelMutex,
    EMethodTraceDetailLevel i_eTrcDetailLevelIpcServer,
    EMethodTraceDetailLevel i_eTrcDetailLevelIpcServerMutex,
    EMethodTraceDetailLevel i_eTrcDetailLevelIpcServerGateway )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CTrcServer* pTrcServer = s_pTheInst;

    CIpcTrcServer* pIpcTrcServer = nullptr;

    if( pTrcServer == nullptr )
    {
        pIpcTrcServer = new CIpcTrcServer(
            i_eTrcDetailLevel, i_eTrcDetailLevelMutex,
            i_eTrcDetailLevelIpcServer, i_eTrcDetailLevelIpcServerMutex,
            i_eTrcDetailLevelIpcServerGateway);
    }
    else
    {
        pIpcTrcServer = dynamic_cast<CIpcTrcServer*>(pTrcServer);

        if( pIpcTrcServer == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated, "ZS::Trace::CTrcServer");
        }
    }

    if( pIpcTrcServer != nullptr )
    {
        pIpcTrcServer->incrementRefCount();
    }

    // The ctor adds the reference to the instance to the hash.
    // If the ctor itself calls methods of other classes which again recursively
    // call "GetInstance" this way "GetInstance" does not return null but the
    // pointer to the server instance currently beeing created.
    // But of course this requires special caution as within the ctor it must
    // be assured that recursively accessed instance members are already valid.
    //s_hshpInstances[i_strName] = pIpcTrcServer;

    return pIpcTrcServer;

} // CreateInstance

//------------------------------------------------------------------------------
/*! @brief Releases the trace server.

    Before invoking this method a reference to the instance must have been retrieved
    with a createInstance call.

    This method decrements the reference counter of the instance.
    If the reference counter reaches 0 the instance will be destroyed.

    @note A reference returned by getInstance MUST NOT be freed.
*/
void CIpcTrcServer::ReleaseInstance()
//------------------------------------------------------------------------------
{
    CTrcServer::ReleaseInstance();
}

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the Ipc Trace Server.

    To avoid multiple inheritance from QObject the server is not derived from
    CIpcServer but aggregates an instance of the Ipc server.

    The IpcServer of the trace server uses the block type "<Len>[Data]".

    @param i_iTrcDetailLevel [in]
        If the methods of the trace server itself should be logged a value
        greater than 0 (EMethodTraceDetailLevel::None) could be passed here.
    @param i_iTrcDetailLevelIpcServer [in]
        If the methods of the trace server's Ipc Server should be logged
        a value greater than 0 (EMethodTraceDetailLevel::None) could be passed here.
    @param i_iTrcDetailLevelIpcServerMutex [in]
        If the locking and unlocking of the mutex of ipc server
        should be logged a value greater than 0 (EMethodTraceDetailLevel::None)
        could be passed here. But the value will be ignored if the detail
        level for the server tracer is None.
    @param i_iTrcDetailLevelIpcServerGateway [in]
        If the methods of the trace server's gateway should be logged a value
        greater than 0 (EMethodTraceDetailLevel::None) could be passed here.
*/
CIpcTrcServer::CIpcTrcServer(
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eTrcDetailLevelMutex,
    EMethodTraceDetailLevel i_eTrcDetailLevelIpcServer,
    EMethodTraceDetailLevel i_eTrcDetailLevelIpcServerMutex,
    EMethodTraceDetailLevel i_eTrcDetailLevelIpcServerGateway ) :
//------------------------------------------------------------------------------
    CTrcServer(i_eTrcDetailLevel, i_eTrcDetailLevelMutex),
    m_pIpcServer(nullptr),
    m_bIsBeingDestroyed(false),
    m_ariSocketIdsConnectedTrcClients(),
    m_bOnReceivedDataUpdateInProcess(false),
    m_pMtxListTrcDataCached(nullptr),
    m_iTrcDataCachedCount(0),
    m_arpTrcDataCached()
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

    // Please note that all Ipc objects used by the trace server may only trace into the local trace file.
    // On the one hand when creating e.g. the gateway instance and the gateway would try to add or modify
    // a trace admin object in the index tree this change shold be sent to all connected trace clients.
    // But the Ipc server is not yet started and cannot send anything to clients.
    // On the other hand the "sendData" method is traced wherupon data will be sent to the client by
    // calling "sendData" which again is traced calling "sendData" and so on (endless recursion, deadlock).
    // And by the way - tracing the tracer is not always necessary.

    // The trace server must aggregate the IpcServer to avoid multiple inheritance of QObject.
    // The parent object got to be set to avoid that the IpcServer tries to register a trace
    // admin object at myself.
    m_pIpcServer = new CServer(
        /* strObjName                    */ objectName(),
        /* bMultiThreadedAccess          */ true,
        /* iTrcMthFileDetailLevel        */ i_eTrcDetailLevelIpcServer,
        /* iTrcMthFileDetailLevelMutex   */ i_eTrcDetailLevelIpcServerMutex,
        /* iTrcMthFileDetailLevelGateway */ i_eTrcDetailLevelIpcServerGateway );

    m_pMtxListTrcDataCached = new QRecursiveMutex();

    // Need direct connections to signals of index tree.
    // If in another thread a trace admin object is created, removed or modified
    // the corresponding message must be send by the server to the connected clients
    // before sending trace data. If the signals would be queued the client may receive
    // trace data for trace admin objects which may not yet exist.
    QObject::connect(
        m_pTrcAdminObjIdxTree, &CIdxTree::treeEntryAdded,
        this, &CIpcTrcServer::onTrcAdminObjIdxTreeEntryAdded,
        Qt::DirectConnection);
    QObject::connect(
        m_pTrcAdminObjIdxTree, &CIdxTree::treeEntryAboutToBeRemoved,
        this, &CIpcTrcServer::onTrcAdminObjIdxTreeEntryAboutToBeRemoved,
        Qt::DirectConnection);
    QObject::connect(
        m_pTrcAdminObjIdxTree, &CIdxTreeTrcAdminObjs::treeEntryChanged,
        this, &CIpcTrcServer::onTrcAdminObjIdxTreeEntryChanged,
        Qt::DirectConnection);

    QObject::connect(
        m_pIpcServer, &CServer::connected,
        this, &CIpcTrcServer::onIpcServerConnected);
    QObject::connect(
        m_pIpcServer, &CServer::disconnected,
        this, &CIpcTrcServer::onIpcServerDisconnected);
    QObject::connect(
        m_pIpcServer, &CServer::receivedData,
        this, &CIpcTrcServer::onIpcServerReceivedData);

} // ctor

//------------------------------------------------------------------------------
CIpcTrcServer::~CIpcTrcServer()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMthInArgs       */ "" );

    m_bIsBeingDestroyed = true;

    // The index tree will be destroyed by the base class after this dtor has been processed.
    // The IpcServer will be destroyed before. The slots are called to send data to the
    // client with the deleted object status. But there is not IpcServer anymore.
    // So we disconnect the slots before destroying the IpcServer.
    QObject::disconnect(
        m_pTrcAdminObjIdxTree, &CIdxTree::treeEntryAdded,
        this, &CIpcTrcServer::onTrcAdminObjIdxTreeEntryAdded);
    QObject::disconnect(
        m_pTrcAdminObjIdxTree, &CIdxTree::treeEntryAboutToBeRemoved,
        this, &CIpcTrcServer::onTrcAdminObjIdxTreeEntryAboutToBeRemoved);
    QObject::disconnect(
        m_pTrcAdminObjIdxTree, &CIdxTreeTrcAdminObjs::treeEntryChanged,
        this, &CIpcTrcServer::onTrcAdminObjIdxTreeEntryChanged );

    for( int idx = 0; idx < m_iTrcDataCachedCount; idx++ )
    {
        try
        {
            delete m_arpTrcDataCached[idx];
        }
        catch(...)
        {
        }
        m_arpTrcDataCached[idx] = nullptr;
    }

    try
    {
        delete m_pIpcServer;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pMtxListTrcDataCached;
    }
    catch(...)
    {
    }

    // I am the parent of the IpcServer. The IpcServer will be destroyed by Qt.
    m_pIpcServer = nullptr;
    m_bIsBeingDestroyed = false;
    //m_ariSocketIdsConnectedTrcClients.clear();
    m_bOnReceivedDataUpdateInProcess = false;
    m_pMtxListTrcDataCached = nullptr;
    m_iTrcDataCachedCount = 0;
    m_arpTrcDataCached.clear();

} // dtor

/*==============================================================================
public: // overridables of base class CTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing is active.

    Tracing is active if tracing is enabled at all (flag enabled of the
    trace settings) and either

    - if the local trace file is used or
    - if output to remote client is activated.
      Output to the rmeote client is activated if
      - caching data is enabled or
      - if the flag to use the IpcServer is enabled and if a client is
        connected to the IpcServer.

    @return true if tracing is active, false otherwise.
*/
bool CIpcTrcServer::isActive() const
//------------------------------------------------------------------------------
{
    return isEnabled() && (isLocalTrcFileActive() || getCacheTrcDataIfNotConnected() || (isIpcServerUsed() && isConnected()));
}

/*==============================================================================
public: // overridables of base class CTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bEnabled);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setEnabled",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( i_bEnabled != m_trcSettings.m_bEnabled )
    {
        CTrcServer::setEnabled(i_bEnabled);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings Enabled=\"" + bool2Str(m_trcSettings.m_bEnabled) + "\"/>";

            sendData(ESocketIdAllSockets, str2ByteArr(strMsg));
        }
    }

} // setEnabled

//------------------------------------------------------------------------------
void CIpcTrcServer::setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bEnabled);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setNewTrcAdminObjsEnabledAsDefault",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault != i_bEnabled )
    {
        CTrcServer::setNewTrcAdminObjsEnabledAsDefault(i_bEnabled);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings NewTrcAdminObjsEnabledAsDefault=\"" + bool2Str(m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setNewTrcAdminObjsEnabledAsDefault

//------------------------------------------------------------------------------
void CIpcTrcServer::setNewTrcAdminObjsMethodCallsDefaultDetailLevel( EMethodTraceDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = CEnumMethodTraceDetailLevel(i_eDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setNewTrcAdminObjsMethodCallsDefaultDetailLevel",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel != i_eDetailLevel )
    {
        CTrcServer::setNewTrcAdminObjsMethodCallsDefaultDetailLevel(i_eDetailLevel);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings NewTrcAdminObjsMethodCallsDefaultDetailLevel=\""
                   + CEnumMethodTraceDetailLevel(m_trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel).toString() + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setNewTrcAdminObjsMethodCallsDefaultDetailLevel

//------------------------------------------------------------------------------
void CIpcTrcServer::setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel( ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = CEnumLogDetailLevel(i_eDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel != i_eDetailLevel )
    {
        CTrcServer::setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel(i_eDetailLevel);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings NewTrcAdminObjsRuntimeInfoDefaultDetailLevel=\""
                   + CEnumLogDetailLevel(m_trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel).toString() + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel

/*==============================================================================
public: // overridables of base class CTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setAdminObjFileAbsoluteFilePath",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_strAdminObjFileAbsFilePath != i_strAbsFilePath )
    {
        CTrcServer::setAdminObjFileAbsoluteFilePath(i_strAbsFilePath);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings AdminObjFileAbsFilePath=\"" + m_trcSettings.m_strAdminObjFileAbsFilePath + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setAdminObjFileAbsoluteFilePath

/*==============================================================================
public: // overridables of base class CTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::setUseLocalTrcFile( bool i_bUse )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bUse);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setUseLocalTrcFile",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bUseLocalTrcFile != i_bUse )
    {
        CTrcServer::setUseLocalTrcFile(i_bUse);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings UseLocalTrcFile=\"" + bool2Str(m_trcSettings.m_bUseLocalTrcFile) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setUseLocalTrcFile

//------------------------------------------------------------------------------
void CIpcTrcServer::setLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileAbsoluteFilePath",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( (m_trcSettings.m_strLocalTrcFileAbsFilePath != i_strAbsFilePath)
     || (m_pTrcMthFile == nullptr && !m_trcSettings.m_strLocalTrcFileAbsFilePath.isEmpty()) )
    {
        CTrcServer::setLocalTrcFileAbsoluteFilePath(i_strAbsFilePath);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings LocalTrcFileAbsFilePath=\"" + m_trcSettings.m_strLocalTrcFileAbsFilePath + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setLocalTrcFileAbsFilePath

//------------------------------------------------------------------------------
void CIpcTrcServer::setLocalTrcFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = QString::number(i_iAutoSaveInterval_ms);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileAutoSaveIntervalInMs",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms != i_iAutoSaveInterval_ms )
    {
        CTrcServer::setLocalTrcFileAutoSaveIntervalInMs(i_iAutoSaveInterval_ms);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings LocalTrcFileAutoSaveInterval_ms=\"" + QString::number(m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setLocalTrcFileAutoSaveIntervalInMs

//------------------------------------------------------------------------------
void CIpcTrcServer::setLocalTrcFileSubFileCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = QString::number(i_iCountMax);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileSubFileCountMax",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_iLocalTrcFileSubFileCountMax != i_iCountMax )
    {
        CTrcServer::setLocalTrcFileSubFileCountMax(i_iCountMax);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings LocalTrcFileSubFileCountMax=\"" + QString::number(m_trcSettings.m_iLocalTrcFileSubFileCountMax) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setLocalTrcFileSubFileCountMax

//------------------------------------------------------------------------------
void CIpcTrcServer::setLocalTrcFileSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = QString::number(i_iCountMax);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileSubFileLineCountMax",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_iLocalTrcFileSubFileLineCountMax != i_iCountMax )
    {
        CTrcServer::setLocalTrcFileSubFileLineCountMax(i_iCountMax);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings LocalTrcFileSubFileLineCountMax=\"" + QString::number(m_trcSettings.m_iLocalTrcFileSubFileLineCountMax) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setLocalTrcFileSubFileLineCountMax

//------------------------------------------------------------------------------
void CIpcTrcServer::setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bCloseFile);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileCloseFileAfterEachWrite",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite != i_bCloseFile )
    {
        CTrcServer::setLocalTrcFileCloseFileAfterEachWrite(i_bCloseFile);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings LocalTrcFileCloseAfterEachWrite=\"" + bool2Str(m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setLocalTrcFileCloseFileAfterEachWrite

/*==============================================================================
public: // instance methods (trace settings)
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::setUseIpcServer( bool i_bUse )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bUse);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setUseIpcServer",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bUseIpcServer != i_bUse )
    {
        CTrcServer::setUseIpcServer(i_bUse);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings UseIpcServer=\"" + bool2Str(m_trcSettings.m_bUseIpcServer) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setUseIpcServer

//------------------------------------------------------------------------------
void CIpcTrcServer::setCacheTrcDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bCacheData);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setCacheTrcDataIfNotConnected",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bCacheDataIfNotConnected != i_bCacheData )
    {
        CTrcServer::setCacheTrcDataIfNotConnected(i_bCacheData);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings CacheDataIfNotConnected=\"" + bool2Str(m_trcSettings.m_bCacheDataIfNotConnected) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setCacheTrcDataIfNotConnected

//------------------------------------------------------------------------------
void CIpcTrcServer::setCacheTrcDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = QString::number(i_iMaxArrLen);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setCacheTrcDataMaxArrLen",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_iCacheDataMaxArrLen != i_iMaxArrLen )
    {
        CTrcServer::setCacheTrcDataMaxArrLen(i_iMaxArrLen);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings CacheDataMaxArrLen=" + QString::number(m_trcSettings.m_iCacheDataMaxArrLen) + "/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setCacheTrcDataIfNotConnected

/*==============================================================================
public: // instance methods to enable and disable the client and server
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::setTraceSettings( const STrcServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setTraceSettings",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings != i_settings )
    {
        // If not called on receiving trace settings from the server and if the client is connected ..
        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings";

            if( m_trcSettings.m_bEnabled != i_settings.m_bEnabled )
            {
                strMsg += " Enabled=\"" + bool2Str(i_settings.m_bEnabled) + "\"";
            }
            if( m_trcSettings.m_strAdminObjFileAbsFilePath != i_settings.m_strAdminObjFileAbsFilePath )
            {
                strMsg += " AdminObjFileAbsFilePath=\"" + i_settings.m_strAdminObjFileAbsFilePath + "\"";
            }
            if( m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault != i_settings.m_bNewTrcAdminObjsEnabledAsDefault )
            {
                strMsg += " NewTrcAdminObjsEnabledAsDefault=\"" + bool2Str(i_settings.m_bNewTrcAdminObjsEnabledAsDefault) + "\"";
            }
            if( m_trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel != i_settings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel )
            {
                strMsg += " NewTrcAdminObjsMethodCallsDefaultDetailLevel=\""
                       + CEnumMethodTraceDetailLevel(i_settings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel).toString() + "\"";
            }
            if( m_trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel != i_settings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel )
            {
                strMsg += " NewTrcAdminObjsRuntimeInfoDefaultDetailLevel=\""
                       + CEnumLogDetailLevel(i_settings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel).toString() + "\"";
            }
            if( m_trcSettings.m_bUseIpcServer != i_settings.m_bUseIpcServer )
            {
                strMsg += " UseIpcServer=\"" + bool2Str(i_settings.m_bUseIpcServer) + "\"";
            }
            if( m_trcSettings.m_bCacheDataIfNotConnected != i_settings.m_bCacheDataIfNotConnected )
            {
                strMsg += " CacheDataIfNotConnected=\"" + bool2Str(i_settings.m_bCacheDataIfNotConnected) + "\"";
            }
            if( m_trcSettings.m_iCacheDataMaxArrLen != i_settings.m_iCacheDataMaxArrLen )
            {
                strMsg += " CacheDataMaxArrLen =\"" + QString::number(i_settings.m_iCacheDataMaxArrLen) + "\"";
            }
            if( m_trcSettings.m_bUseLocalTrcFile != i_settings.m_bUseLocalTrcFile )
            {
                strMsg += " UseLocalTrcFile=\"" + bool2Str(i_settings.m_bUseLocalTrcFile) + "\"";
            }
            if( m_trcSettings.m_strLocalTrcFileAbsFilePath != i_settings.m_strLocalTrcFileAbsFilePath )
            {
                strMsg += " LocalTrcFileAbsFilePath=\"" + i_settings.m_strLocalTrcFileAbsFilePath + "\"";
            }
            if( m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms != i_settings.m_iLocalTrcFileAutoSaveInterval_ms )
            {
                strMsg += " LocalTrcFileAutoSaveInterval_ms=\"" + QString::number(i_settings.m_iLocalTrcFileAutoSaveInterval_ms) + "\"";
            }
            if( m_trcSettings.m_iLocalTrcFileSubFileCountMax != i_settings.m_iLocalTrcFileSubFileCountMax )
            {
                strMsg += " LocalTrcFileSubFileCountMax=\"" + QString::number(i_settings.m_iLocalTrcFileSubFileCountMax) + "\"";
            }
            if( m_trcSettings.m_iLocalTrcFileSubFileLineCountMax != i_settings.m_iLocalTrcFileSubFileLineCountMax )
            {
                strMsg += " LocalTrcFileSubFileLineCountMax=\"" + QString::number(i_settings.m_iLocalTrcFileSubFileLineCountMax) + "\"";
            }
            if( m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite != i_settings.m_bLocalTrcFileCloseFileAfterEachWrite )
            {
                strMsg += " LocalTrcFileCloseAfterEachWrite=\"" + bool2Str(i_settings.m_bLocalTrcFileCloseFileAfterEachWrite) + "\"";
            }
            strMsg += "/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }

        CTrcServer::setTraceSettings(i_settings);
    }

} // setTraceSettings

/*==============================================================================
public: // overridables of base class CTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Traces entering a method.

    This method is intended for such trace calls if the transferred trace admin
    object was created as an instance tracer and has a valid object name assigned
    or if a class method should be traced.

    The threads call depth counter is incremented after the trace string has been output.

    @param i_pTrcAdminObj [in] Pointer to trace admin object.
        In addition to the trace detail level the name space, the class name
        and - in case of an instance tracer - the instance name is taken from
        the trace admin object.
    @param i_strMethod [in] Name of the entered method.
    @param i_strMethodInArgs [in] String describing the input arguments.
*/
void CIpcTrcServer::traceMethodEnter(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strMethodInArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) && isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Enter,
            /* pTrcAdminObj  */ i_pTrcAdminObj,
            /* strObjName    */ i_pTrcAdminObj->getObjectName(),
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodInArgs );
    }

} // traceMethodEnter

//------------------------------------------------------------------------------
/*! @brief Traces entering a method.

    This method is intended for such trace calls if the transferred trace admin
    object was created as an class tracer and has no valid object name assigned
    and if an instance method should be traced.

    The threads call depth counter is incremented after the trace string has been output.

    @param i_pTrcAdminObj [in] Pointer to trace admin object.
        In addition to the trace detail level the name space, the class name
        and - in case of an instance tracer - the instance name is taken from
        the trace admin object.
    @param i_strObjName [in] Object name of the instance for which the method is applied.
    @param i_strMethod [in] Name of the entered method.
    @param i_strMethodInArgs [in] String describing the input arguments.
*/
void CIpcTrcServer::traceMethodEnter(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strMethodInArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) && isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Enter,
            /* pTrcAdminObj  */ i_pTrcAdminObj,
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodInArgs );
    }

} // traceMethodEnter

//------------------------------------------------------------------------------
/*! @brief Traces additional info within a method call.

    Usually used to trace internal states or additional runtime info which might
    be helpful.

    This method is intended for such trace calls if the transferred trace admin
    object was created as an instance tracer and has a valid object name assigned
    or if a class method should be traced.

    @param i_pTrcAdminObj [in] Pointer to trace admin object.
        In addition to the trace detail level the name space, the class name
        and - in case of an instance tracer - the instance name is taken from
        the trace admin object.
    @param i_strMethod [in] Name of the entered method.
    @param i_strAddInfo [in] String containing the additional info to be output
        during the method call.
*/
void CIpcTrcServer::traceMethod(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strAddInfo )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) && isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::None,
            /* pTrcAdminObj  */ i_pTrcAdminObj,
            /* strObjName    */ i_pTrcAdminObj->getObjectName(),
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strAddInfo );
    }

} // traceMethod

//------------------------------------------------------------------------------
/*! @brief Traces entering a method.

    This method is intended for such trace calls if the transferred trace admin
    object was created as an class tracer and has no valid object name assigned
    and if an instance method should be traced.

    The threads call depth counter is not changed by this method.

    @param i_pTrcAdminObj [in] Pointer to trace admin object.
        In addition to the trace detail level the name space, the class name
        and - in case of an instance tracer - the instance name is taken from
        the trace admin object.
    @param i_strObjName [in] Object name of the instance for which the method is applied.
    @param i_strMethod [in] Name of the entered method.
    @param i_strAddInfo [in] String containing the additional info to be output
        during the method call.
*/
void CIpcTrcServer::traceMethod(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strAddInfo )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) && isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::None,
            /* pTrcAdminObj  */ i_pTrcAdminObj,
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strAddInfo );
    }

} // traceMethod

//------------------------------------------------------------------------------
/*! @brief Traces leaving a method.

    This method is intended for such trace calls if the transferred trace admin
    object was created as an instance tracer and has a valid object name assigned
    or if a class method should be traced.

    The threads call depth counter is decremented before the trace string is output.

    @param i_pTrcAdminObj [in] Pointer to trace admin object.
        In addition to the trace detail level the name space, the class name
        and - in case of an instance tracer - the instance name is taken from
        the trace admin object.
    @param i_strMethod [in] Name of the entered method.
    @param i_strMethodReturn [in] String describing the return value of the method.
    @param i_strMethodOutArgs [in] String describing the output arguments of the method.
*/
void CIpcTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strMethodReturn,
    const QString&      i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) && isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Leave,
            /* pTrcAdminObj  */ i_pTrcAdminObj,
            /* strObjName    */ i_pTrcAdminObj->getObjectName(),
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodReturn,
            /* strMthOutArgs */ i_strMethodOutArgs );
    }

} // traceMethodLeave

//------------------------------------------------------------------------------
/*! @brief Traces leaving a method.

    This method is intended for such trace calls if the transferred trace admin
    object was created as an class tracer and has no valid object name assigned
    and if an instance method should be traced.

    The threads call depth counter is decremented before the trace string is output.

    @param i_pTrcAdminObj [in] Pointer to trace admin object.
        In addition to the trace detail level the name space, the class name
        and - in case of an instance tracer - the instance name is taken from
        the trace admin object.
    @param i_strObjName [in] Object name of the instance for which the method is applied.
    @param i_strMethod [in] Name of the entered method.
    @param i_strMethodReturn [in] String describing the return value of the method.
    @param i_strMethodOutArgs [in] String describing the output arguments of the method.
*/
void CIpcTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strMethodReturn,
    const QString&      i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) && isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Leave,
            /* pTrcAdminObj  */ i_pTrcAdminObj,
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodReturn,
            /* strMthOutArgs */ i_strMethodOutArgs );
    }

} // traceMethodLeave

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::addEntry(
    const QString&         i_strThreadName,
    const QDateTime&       i_dt,
    double                 i_fSysTimeInSec,
    ZS::System::EMethodDir i_mthDir,
    const CTrcAdminObj*    i_pTrcAdminObj,
    const QString&         i_strObjName,
    const QString&         i_strMethod,
    const QString&         i_strAddInfo,
    const QString&         i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    if( m_bIsBeingDestroyed )
    {
        return;
    }

    if( m_trcSettings.m_bUseLocalTrcFile && m_pTrcMthFile != nullptr )
    {
        CTrcServer::addEntry(
            /* strThreadName */ i_strThreadName,
            /* dt            */ i_dt,
            /* fSysTimeInSec */ i_fSysTimeInSec,
            /* mthDir        */ i_mthDir,
            /* strNameSpace  */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName  */ i_pTrcAdminObj->getClassName(),
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strAddInfo,
            /* strMthOutArgs */ i_strMethodOutArgs );
    }

    bool bAdd2Cache = false;

    if( m_trcSettings.m_bCacheDataIfNotConnected && m_trcSettings.m_iCacheDataMaxArrLen > 0 )
    {
        // Please note that only the first connected client will receive the cached data.

        // If no client is connected ..
        if( m_ariSocketIdsConnectedTrcClients.length() == 0 )
        {
            // .. the trace data will be cached.
            bAdd2Cache = true;
        }
    }

    if( bAdd2Cache )
    {
        // ... temporarily store the trace message until a client connects to the trace server.
        QMutexLocker mutexLocker(m_pMtxListTrcDataCached);

        int idx;

        if( m_iTrcDataCachedCount == m_arpTrcDataCached.size() )
        {
            const int ciMaxArrLen   = m_trcSettings.m_iCacheDataMaxArrLen;
            const int ciRemoveCount = (ciMaxArrLen / 5) + 1; // 20 % (but at least one)

            // To save memory not more than MaxArrLen entries will be temporarily stored:
            if( m_iTrcDataCachedCount < ciMaxArrLen )
            {
                m_arpTrcDataCached.resize(ciMaxArrLen);

                for( idx = m_iTrcDataCachedCount; idx < m_arpTrcDataCached.size(); idx++ )
                {
                    m_arpTrcDataCached[idx] = nullptr;
                }
            }
            // If MaxArrLen or more entries have already been stored ...
            else
            {
                // .. the oldest entries will be removed.
                for( idx = 0; idx < ciRemoveCount; idx++ )
                {
                    delete m_arpTrcDataCached[idx];
                    m_arpTrcDataCached[idx] = nullptr;
                }
                for( idx = 0; idx < m_iTrcDataCachedCount-ciRemoveCount; idx++ )
                {
                    m_arpTrcDataCached[idx] = m_arpTrcDataCached[idx+ciRemoveCount];
                }
                for( ; idx < m_arpTrcDataCached.size(); idx++ )
                {
                    m_arpTrcDataCached[idx] = nullptr;
                }
                m_iTrcDataCachedCount -= ciRemoveCount;
            }
        }

        // Add the entry to the list:
        SMthTrcData* pTrcData = new SMthTrcData(
            /* strMthThreadName */ i_strThreadName,
            /* dt               */ i_dt,
            /* fSysTimeInSec    */ i_fSysTimeInSec,
            /* mthDir           */ i_mthDir,
            /* iTrcAdminObjId   */ i_pTrcAdminObj->indexInTree(),
            /* strNameSpace     */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName     */ i_pTrcAdminObj->getClassName(),
            /* strObjName       */ i_strObjName,
            /* strMthName       */ i_strMethod,
            /* strAddInfo       */ i_strAddInfo,
            /* strMethodOutArgs */ i_strMethodOutArgs );
        m_arpTrcDataCached[m_iTrcDataCachedCount] = pTrcData;
        m_iTrcDataCachedCount++;

    } // if( bAdd2Cache )

    else if( m_ariSocketIdsConnectedTrcClients.length() > 0 && isIpcServerUsed() )
    {
        // Please note that the cached data should have already been sent to the first
        // conencted client and cleared afterwards. So the cache must be empty here.

        QString strMsg;

        strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
        strMsg += command2Str(MsgProtocol::ECommandInsert) + " ";

        SMthTrcData trcData(
            /* strMthThreadName */ i_strThreadName,
            /* dt               */ i_dt,
            /* fSysTimeInSec    */ i_fSysTimeInSec,
            /* mthDir           */ i_mthDir,
            /* iTrcAdminObjId   */ i_pTrcAdminObj->indexInTree(),
            /* strNameSpace     */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName     */ i_pTrcAdminObj->getClassName(),
            /* strObjName       */ i_strObjName,
            /* strMthName       */ i_strMethod,
            /* strAddInfo       */ i_strAddInfo,
            /* strMethodOutArgs */ i_strMethodOutArgs );
        strMsg += trcData.toXmlString();

        QByteArray byteArrMsg = str2ByteArr(strMsg);

        for( auto& iSocketId : m_ariSocketIdsConnectedTrcClients )
        {
            sendData(iSocketId, byteArrMsg);
        }
    }

} // addEntry

/*==============================================================================
public: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CIpcTrcServer::startup( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strMthInArgs += ", Wait: " + bool2Str(i_bWait);
        strMthInArgs += ", ReqIdParent: " + QString::number(i_iReqIdParent);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "startup",
        /* strMthInArgs       */ strMthInArgs );

    return m_pIpcServer->startup(i_iTimeout_ms, i_bWait, i_iReqIdParent);

} // startup

//------------------------------------------------------------------------------
CRequest* CIpcTrcServer::shutdown( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strMthInArgs += ", Wait: " + bool2Str(i_bWait);
        strMthInArgs += ", ReqIdParent: " + QString::number(i_iReqIdParent);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "shutdown",
        /* strMthInArgs       */ strMthInArgs );

    return m_pIpcServer->shutdown(i_iTimeout_ms, i_bWait, i_iReqIdParent);

} // shutdown

/*==============================================================================
public: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CIpcTrcServer::changeSettings( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    return m_pIpcServer->changeSettings(i_iTimeout_ms, i_bWait, i_iReqIdParent);
}

/*==============================================================================
public: // instance methods changing and reading the host settings
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Ipc::SServerHostSettings CIpcTrcServer::getHostSettings() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getHostSettings();
}

//------------------------------------------------------------------------------
void CIpcTrcServer::setHostSettings( const ZS::Ipc::SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    m_pIpcServer->setHostSettings(i_hostSettings);
}

/*==============================================================================
public: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::setBlkType( ZS::Ipc::CBlkType* i_pBlkType )
//------------------------------------------------------------------------------
{
    m_pIpcServer->setBlkType(i_pBlkType);
}

//------------------------------------------------------------------------------
ZS::Ipc::CBlkType* CIpcTrcServer::getBlkType()
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getBlkType();
}

/*==============================================================================
public: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
int CIpcTrcServer::getSocketId( int i_idxSocket ) const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getSocketId(i_idxSocket);
}

//------------------------------------------------------------------------------
ZS::Ipc::SSocketDscr CIpcTrcServer::getSocketDscr( int i_iSocketId ) const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getSocketDscr(i_iSocketId);
}

//------------------------------------------------------------------------------
int CIpcTrcServer::getArrLenConnections() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getArrLenConnections();
}

//------------------------------------------------------------------------------
int CIpcTrcServer::getActiveConnections() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getActiveConnections();
}

/*==============================================================================
public: // instance methods (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Ipc::CServer::EState CIpcTrcServer::getState() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getState();
}

//------------------------------------------------------------------------------
bool CIpcTrcServer::isListening() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->isListening();
}

//------------------------------------------------------------------------------
bool CIpcTrcServer::isStartingUp() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->isStartingUp();
}

//------------------------------------------------------------------------------
bool CIpcTrcServer::isConnected( int /*i_iSocketId*/ ) const
//------------------------------------------------------------------------------
{
    bool bConnected = false;
    if( m_ariSocketIdsConnectedTrcClients.length() > 0 )
    {
        bConnected = true;
    }
    return bConnected;

} // isConnected

/*==============================================================================
public: // instance methods (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CIpcTrcServer::isBusy() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->isBusy();
}

//------------------------------------------------------------------------------
ZS::Ipc::CServer::ERequest CIpcTrcServer::requestInProgress() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->requestInProgress();
}

//------------------------------------------------------------------------------
QString CIpcTrcServer::requestInProgress2Str( bool i_bShort ) const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->requestInProgress2Str(i_bShort);
}

//------------------------------------------------------------------------------
ZS::System::CRequest* CIpcTrcServer::getRequestInProgress() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getRequestInProgress();
}

/*==============================================================================
public: // instance methods (aborting requests)
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::abortRequest( qint64 i_iRequestId )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ReqId: " + QString::number(i_iRequestId);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "abortRequest",
        /* strMthInArgs       */ strMthInArgs );

    m_pIpcServer->abortRequest(i_iRequestId);

} // abortRequest

//------------------------------------------------------------------------------
void CIpcTrcServer::abortRequestInProgress()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "abortRequestInProgress",
        /* strMthInArgs       */ strMthInArgs );

    m_pIpcServer->abortRequestInProgress();

} // abortRequestInProgress

//------------------------------------------------------------------------------
void CIpcTrcServer::abortAllRequests()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "abortAllRequests",
        /* strMthInArgs       */ strMthInArgs );

    m_pIpcServer->abortAllRequests();

} // abortAllRequests

/*==============================================================================
protected: // instance methods to recursively send index tree entries to the connected clients
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::sendBranch(
    int                         i_iSocketId,
    MsgProtocol::TSystemMsgType i_systemMsgType,
    MsgProtocol::TCommand       i_cmd,
    CIdxTreeEntry*              i_pBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", MsgType: " + systemMsgType2Str(i_systemMsgType);
        strMthInArgs += ", Cmd: " + command2Str(i_cmd);
        strMthInArgs += ", Branch: " + QString(i_pBranch == nullptr ? "null" : i_pBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendBranch",
        /* strMthInArgs       */ strMthInArgs );

    if( isConnected() && i_pBranch != nullptr )
    {
        if( !i_pBranch->isRoot() )
        {
            QString strMsg;
            QString strBranchName = i_pBranch->name();

            strBranchName = encodeForXml(strBranchName);

            strMsg += systemMsgType2Str(i_systemMsgType) + " ";
            strMsg += command2Str(i_cmd) + " ";

            strMsg += "<Branch ";

            if( i_pBranch->parentBranch() != nullptr )
            {
                strMsg += " ParentBranchIdxInTree=\"" + QString::number(i_pBranch->parentBranch()->indexInTree()) + "\"";
            }
            strMsg += " Name=\"" + strBranchName + "\"";
            strMsg += " IdxInTree=\"" + QString::number(i_pBranch->indexInTree()) + "\"";

            strMsg += "/>";

            sendData( i_iSocketId, str2ByteArr(strMsg) );

        } // if( !i_pBranch->isRoot() )

        CIdxTreeEntry* pTreeEntry;
        int            idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->isRoot() || pTreeEntry->isBranch() )
                {
                    sendBranch(
                        /* iSocketId     */ i_iSocketId,
                        /* systemMsgType */ i_systemMsgType,
                        /* cmd           */ i_cmd,
                        /* pBranch       */ pTreeEntry );
                }
                else if( pTreeEntry->isLeave() )
                {
                    sendAdminObj(
                        /* iSocketId     */ i_iSocketId,
                        /* systemMsgType */ i_systemMsgType,
                        /* cmd           */ i_cmd,
                        /* strKeyInTree  */ pTreeEntry->keyInTree(),
                        /* idxInTree     */ pTreeEntry->indexInTree() );
                }
            }
        } // for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
    } // if( isConnected() && i_pBranch != nullptr )

} // sendBranch

/*==============================================================================
protected: // instance methods to send admin objects to the connected clients
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::sendAdminObj(
    int                         i_iSocketId,
    MsgProtocol::TSystemMsgType i_systemMsgType,
    MsgProtocol::TCommand       i_cmd,
    const QString&              i_strKeyInTree,
    int                         i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", MsgType: " + systemMsgType2Str(i_systemMsgType);
        strMthInArgs += ", Cmd: " + command2Str(i_cmd);
        strMthInArgs += ", TrcAdminObj: " + i_strKeyInTree;
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    if( isConnected() )
    {
        QString strMsg;

        strMsg += systemMsgType2Str(i_systemMsgType) + " ";
        strMsg += command2Str(i_cmd) + " ";
        strMsg += "<TrcAdminObj";
        strMsg += " IdxInTree=\"" + QString::number(i_idxInTree) + "\"";

        CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(m_pTrcAdminObjIdxTree->findLeave(i_strKeyInTree));

        // If removed the command is deleted. It is sufficient to send the IdxInTree which was deleted.
        if( pTrcAdminObj != nullptr && !pTrcAdminObj->isAboutToBeDestroyed() )
        {
            QString strNameSpace = encodeForXml(pTrcAdminObj->getNameSpace());
            QString strClassName = encodeForXml(pTrcAdminObj->getClassName());
            QString strObjName = encodeForXml(pTrcAdminObj->getObjectName());
            QString strThreadName = encodeForXml(pTrcAdminObj->getObjectThreadName());

            strMsg += " NameSpace=\"" + strNameSpace + "\"";
            strMsg += " ClassName=\"" + strClassName + "\"";
            strMsg += " ObjName=\"" + strObjName + "\"";
            strMsg += " Thread=\"" + strThreadName + "\"";

            if( pTrcAdminObj->parentBranch() != nullptr )
            {
                strMsg += " ParentBranchIdxInTree=\"" + QString::number(pTrcAdminObj->parentBranch()->indexInTree()) + "\"";
            }
            strMsg += " Enabled=\"" + CEnumEnabled::toString(pTrcAdminObj->getEnabled()) + "\"";
            strMsg += " MethodCallsDetailLevel=\"" + CEnumMethodTraceDetailLevel(pTrcAdminObj->getMethodCallsTraceDetailLevel()).toString() + "\"";
            strMsg += " RuntimeInfoDetailLevel=\"" + CEnumLogDetailLevel(pTrcAdminObj->getRuntimeInfoTraceDetailLevel()).toString() + "\"";
            strMsg += " DataFilter=\"" + pTrcAdminObj->getTraceDataFilter() + "\"";
            strMsg += " RefCount=\"" + QString::number(pTrcAdminObj->getRefCount()) + "\"";
        }
        strMsg += "/>";

        sendData( i_iSocketId, str2ByteArr(strMsg) );

    } // if( isConnected() )

} // sendAdminObj

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sends the server settings including application and server name to
           the connected client.

    @param i_iSocketId [in] Socket of the connected client.
*/
void CIpcTrcServer::sendServerSettings(int i_iSocketId)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendServerSettings",
        /* strMthInArgs       */ strMthInArgs );

    QString strDataSnd;
    strDataSnd  = systemMsgType2Str(MsgProtocol::ESystemMsgTypeCon) + " ";
    strDataSnd += command2Str(MsgProtocol::ECommandSelect) + " ";
    strDataSnd += "<ServerSettings";
    strDataSnd += " ApplicationName=\"" + QCoreApplication::applicationName() + "\"";
    strDataSnd += " ServerName=\"" + objectName() + "\"";
    strDataSnd += " Enabled=\"" + bool2Str(m_trcSettings.m_bEnabled) + "\"";
    strDataSnd += " AdminObjFileAbsFilePath=\"" + m_trcSettings.m_strAdminObjFileAbsFilePath + "\"";
    strDataSnd += " NewTrcAdminObjsEnabledAsDefault=\"" + bool2Str(m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault) + "\"";
    strDataSnd += " NewTrcAdminObjsMethodCallsDefaultDetailLevel=\""
               + CEnumMethodTraceDetailLevel(m_trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel).toString() + "\"";
    strDataSnd += " NewTrcAdminObjsRuntimeInfoDefaultDetailLevel=\""
               + CEnumLogDetailLevel(m_trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel).toString() + "\"";
    strDataSnd += " UseIpcServer=\"" + bool2Str(m_trcSettings.m_bUseIpcServer) + "\"";
    strDataSnd += " CacheDataIfNotConnected=\"" + bool2Str(m_trcSettings.m_bCacheDataIfNotConnected) + "\"";
    strDataSnd += " CacheDataMaxArrLen=\"" + QString::number(m_trcSettings.m_iCacheDataMaxArrLen) + "\"";
    strDataSnd += " UseLocalTrcFile=\"" + bool2Str(m_trcSettings.m_bUseLocalTrcFile) + "\"";
    strDataSnd += " LocalTrcFileAbsFilePath=\"" + m_trcSettings.m_strLocalTrcFileAbsFilePath + "\"";
    strDataSnd += " LocalTrcFileAutoSaveInterval_ms=\"" + QString::number(m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms) + "\"";
    strDataSnd += " LocalTrcFileSubFileCountMax=\"" + QString::number(m_trcSettings.m_iLocalTrcFileSubFileCountMax) + "\"";
    strDataSnd += " LocalTrcFileSubFileLineCountMax=\"" + QString::number(m_trcSettings.m_iLocalTrcFileSubFileLineCountMax) + "\"";
    strDataSnd += " LocalTrcFileCloseAfterEachWrite=\"" + bool2Str(m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite) + "\"";
    strDataSnd += "/>";

    sendData(i_iSocketId, str2ByteArr(strDataSnd));

} // sendServerSettings

//------------------------------------------------------------------------------
/*! @brief Sends the cached trace data to the connected client.

    @param i_iSocketId [in] Socket of the connected client.
*/
void CIpcTrcServer::sendCachedTrcData(int i_iSocketId)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendCachedTrcData",
        /* strMthInArgs       */ strMthInArgs );

    // If traced data has been temporarily stored ...
    if( m_iTrcDataCachedCount > 0 )
    {
        QString      strDataSnd;
        SMthTrcData* pTrcData;
        int          idx;

        QMutexLocker mutexLocker(m_pMtxListTrcDataCached);

        for( idx = 0; idx < m_iTrcDataCachedCount; idx++ )
        {
            pTrcData = m_arpTrcDataCached[idx];

            if( pTrcData != nullptr )
            {
                strDataSnd  = systemMsgType2Str(MsgProtocol::ESystemMsgTypeCon) + " ";
                strDataSnd += command2Str(MsgProtocol::ECommandInsert) + " ";
                strDataSnd += pTrcData->toXmlString();

                sendData(i_iSocketId, str2ByteArr(strDataSnd));

                delete pTrcData;
                pTrcData = nullptr;
            }
            m_arpTrcDataCached[idx] = nullptr;
        }
        m_arpTrcDataCached.clear();
        m_iTrcDataCachedCount = 0;
    }

} // sendCachedTrcData

/*==============================================================================
protected: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CIpcTrcServer::sendData( int i_iSocketId, const QByteArray& i_byteArr, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", ByteArr[" + QString::number(i_byteArr.size()) + "]";
        if( m_eTrcDetailLevel < EMethodTraceDetailLevel::ArgsVerbose ) {
            strMthInArgs += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 30) + ")";
        } else {
            strMthInArgs += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 100) + ")";
        }
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendData",
        /* strMthInArgs       */ strMthInArgs );

    // The sendData method of the trace server may be called from any thread in the application.
    return m_pIpcServer->sendData(
        /* iSocketId    */ i_iSocketId,
        /* byteArr      */ i_byteArr,
        /* iTimeout_ms  */ 0,
        /* bWait        */ false,
        /* iReqIdParent */ i_iReqIdParent );

} // sendData

/*==============================================================================
protected slots: // connected to the signals of the Ipc Server
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::onIpcServerConnected( QObject* /*i_pServer*/, const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "{" + i_socketDscr.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcServerConnected",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( !m_ariSocketIdsConnectedTrcClients.contains(i_socketDscr.m_iSocketId) )
    {
        m_ariSocketIdsConnectedTrcClients.append(i_socketDscr.m_iSocketId);

        sendServerSettings(i_socketDscr.m_iSocketId);

        // Send the whole admin object tree to the client ...
        sendBranch(
            /* iSocketId     */ i_socketDscr.m_iSocketId,
            /* systemMsgType */ MsgProtocol::ESystemMsgTypeCon,
            /* cmd           */ MsgProtocol::ECommandSelect,
            /* pBranch       */ m_pTrcAdminObjIdxTree->root() );

        // If traced data has been temporarily stored ...
        if( m_iTrcDataCachedCount > 0 )
        {
            sendCachedTrcData(i_socketDscr.m_iSocketId);
        }
    }
    else
    {
        QString strAddErrInfo = "Received connected signal for already connected socket " + i_socketDscr.getConnectionString();
        SErrResultInfo errResultInfo = ErrResultInfoError("onIpcServerConnected", EResultSocketIdOutOfRange, strAddErrInfo);
    }

} // onIpcServerConnected

//------------------------------------------------------------------------------
void CIpcTrcServer::onIpcServerDisconnected( QObject* /*i_pServer*/, const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "{" + i_socketDscr.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcServerDisconnected",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_ariSocketIdsConnectedTrcClients.contains(i_socketDscr.m_iSocketId) )
    {
        m_ariSocketIdsConnectedTrcClients.removeOne(i_socketDscr.m_iSocketId);
    }
    else
    {
        QString strAddErrInfo = "Received disconnected signal for not connected socket " + i_socketDscr.getConnectionString();
        SErrResultInfo errResultInfo = ErrResultInfoError("onIpcServerDisconnected", EResultSocketIdOutOfRange, strAddErrInfo);
    }
} // onIpcServerDisconnected

//------------------------------------------------------------------------------
void CIpcTrcServer::onIpcServerReceivedData(
    QObject*          /*i_pServer*/,
    int               i_iSocketId,
    const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", ByteArr[" + QString::number(i_byteArr.size()) + "]";
        if( m_eTrcDetailLevel < EMethodTraceDetailLevel::ArgsVerbose ) {
            strMthInArgs += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 30) + ")";
        } else {
            strMthInArgs += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 100) + ")";
        }
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcServerReceivedData",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    QString                     str = byteArr2Str(i_byteArr);
    const QChar*                pcData = str.data();
    int                         iDataLen = str.size();
    int                         iSection = 0;
    QString                     strSystemMsgTypeOfData;
    MsgProtocol::TSystemMsgType systemMsgTypeOfData;
    QString                     strCmd;
    MsgProtocol::TCommand       cmd;

    strSystemMsgTypeOfData = str.section(" ",iSection,iSection);
    systemMsgTypeOfData = str2SystemMsgType(strSystemMsgTypeOfData);
    if( systemMsgTypeOfData != MsgProtocol::ESystemMsgTypeUndefined )
    {
        pcData   += (strSystemMsgTypeOfData.length() + 1);
        iDataLen -= (strSystemMsgTypeOfData.length() + 1);
        iSection++;
    }

    strCmd = str.section(" ",iSection,iSection);
    cmd = str2Command(strCmd);
    if( cmd != MsgProtocol::ECommandUndefined )
    {
        pcData   += (strCmd.length() + 1);
        iDataLen -= (strCmd.length() + 1);
        iSection++;
    }

    QString strData(pcData, iDataLen);

    switch( systemMsgTypeOfData )
    {
        case MsgProtocol::ESystemMsgTypeReq:
        {
            switch( cmd )
            {
                case MsgProtocol::ECommandSelect:
                {
                    onIpcServerReceivedReqSelect(i_iSocketId, strData);
                    break;
                }
                case MsgProtocol::ECommandUpdate:
                {
                    onIpcServerReceivedReqUpdate(i_iSocketId, strData);
                    break;
                }
                case MsgProtocol::ECommandInsert:
                case MsgProtocol::ECommandDelete:
                default:
                {
                    break;
                }
            }
            break;
        }
        case MsgProtocol::ESystemMsgTypeCon:
        case MsgProtocol::ESystemMsgTypeInd:
        case MsgProtocol::ESystemMsgTypeAck:
        default:
        {
            break;
        }
    } // switch( systemMsgTypeOfData )

} // onIpcServerReceivedData

/*==============================================================================
protected: // overridables to parse and execute the incoming data stream
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::onIpcServerReceivedReqSelect( int i_iSocketId, const QString& i_strData )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", Data: " + i_strData.left(20);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcServerReceivedReqSelect",
        /* strMthInArgs       */ strMthInArgs );

    // Already locked by onIpcServerReceivedData
    //QMutexLocker mtxLocker(&s_mtx);

    // The trace admin object index tree will be locked so it will not be changed
    // when sending the whole content of the index tree to the client.
    QMutexLocker mutexLocker(m_pTrcAdminObjIdxTree->mutex());

    QString strMth = "onIpcServerReceivedReqSelect";

    QXmlStreamReader xmlStreamReader(i_strData);

    QXmlStreamReader::TokenType xmlStreamTokenType;
    QString                     strElemName;
    QString                     strDataSnd;

    xmlStreamTokenType = xmlStreamReader.readNext();

    if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
    {
        xmlStreamReader.raiseError("Invalid XML command");
    }
    else // if( xmlStreamTokenType == QXmlStreamReader::StartDocument )
    {
        while( xmlStreamTokenType != QXmlStreamReader::EndElement && xmlStreamTokenType != QXmlStreamReader::Invalid )
        {
            xmlStreamTokenType = xmlStreamReader.readNext();

            if( xmlStreamReader.isStartElement() )
            {
                strElemName = xmlStreamReader.name().toString();

                if( strElemName == "ServerSettings" )
                {
                    sendServerSettings(i_iSocketId);
                }
                else if( strElemName == "TrcAdminObjs" )
                {
                    // Send the whole admin object tree to the client ...
                    sendBranch(
                        /* iSocketId     */ i_iSocketId,
                        /* systemMsgType */ MsgProtocol::ESystemMsgTypeCon,
                        /* cmd           */ MsgProtocol::ECommandSelect,
                        /* pBranch       */ m_pTrcAdminObjIdxTree->root() );
                }
                else if( strElemName == "TrcData" )
                {
                    // If traced data has been temporarily stored ...
                    if( m_iTrcDataCachedCount > 0 )
                    {
                        sendCachedTrcData(i_iSocketId);
                    }
                } // if( strElemName == "TrcData" )

                else
                {
                    xmlStreamReader.raiseError("Invalid element \"" + strElemName + "\" in select request");
                }
            } // if( xmlStreamTokenType == QXmlStreamReader::StartElement )
        } // while( xmlStreamTokenType != QXmlStreamReader::EndElement && xmlStreamTokenType != QXmlStreamReader::Invalid )
    } // if( xmlStreamTokenType == QXmlStreamReader::StartDocument )

    if( xmlStreamReader.hasError() )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileReadContent);
            errResultInfo.setAddErrInfoDscr(xmlStreamReader.errorString());

            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
} // onIpcServerReceivedReqSelect

//------------------------------------------------------------------------------
void CIpcTrcServer::onIpcServerReceivedReqUpdate( int i_iSocketId, const QString& i_strData )
//------------------------------------------------------------------------------
{
    // The trace admin object index tree will be locked so it will not be changed
    // when sending the whole content of the index tree to the client.
    QMutexLocker mutexLocker(m_pTrcAdminObjIdxTree->mutex());

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", Data: " + i_strData.left(20);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcServerReceivedReqUpdate",
        /* strMthInArgs       */ strMthInArgs );

    // Already locked by onIpcServerReceivedData
    //QMutexLocker mtxLocker(&s_mtx);

    QString strMth = "onIpcServerReceivedReqUpdate";

    m_bOnReceivedDataUpdateInProcess = true;

    QXmlStreamReader xmlStreamReader(i_strData);

    QXmlStreamReader::TokenType xmlStreamTokenType;

    QString  strElemName;
    QString  strAttr;
    bool     bVal;
    int      iVal;
    int      iObjId;
    EEnabled enabled;
    bool     bSetDataFilter;
    QString  strDataFilter;
    bool     bOk;

    EMethodTraceDetailLevel eDetailLevelMethodCalls;
    ELogDetailLevel eDetailLevelRuntimeInfo;

    xmlStreamTokenType = xmlStreamReader.readNext();

    if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
    {
        xmlStreamReader.raiseError("Invalid XML command");
    }
    else // if( xmlStreamTokenType == QXmlStreamReader::StartDocument )
    {
        while( xmlStreamTokenType != QXmlStreamReader::EndElement && xmlStreamTokenType != QXmlStreamReader::Invalid )
        {
            xmlStreamTokenType = xmlStreamReader.readNext();

            if( xmlStreamReader.isStartElement() )
            {
                strElemName = xmlStreamReader.name().toString();

                // For best performance start with the most frequently used element names ..

                if( strElemName == "ServerSettings" )
                {
                    STrcServerSettings trcServerSettings = m_trcSettings;

                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Enabled") )
                    {
                        strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) trcServerSettings.m_bEnabled = bVal;
                        else xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("AdminObjFileAbsFilePath") )
                    {
                        strAttr = xmlStreamReader.attributes().value("AdminObjFileAbsFilePath").toString();
                        trcServerSettings.m_strAdminObjFileAbsFilePath = strAttr;
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("NewTrcAdminObjsEnabledAsDefault") )
                    {
                        strAttr = xmlStreamReader.attributes().value("NewTrcAdminObjsEnabledAsDefault").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) trcServerSettings.m_bNewTrcAdminObjsEnabledAsDefault = bVal;
                        else xmlStreamReader.raiseError("Attribute \"NewTrcAdminObjsEnabledAsDefault\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("NewTrcAdminObjsMethodCallsDefaultDetailLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("NewTrcAdminObjsMethodCallsDefaultDetailLevel").toString();
                        CEnumMethodTraceDetailLevel eDetailLevel = CEnumMethodTraceDetailLevel::fromString(strAttr);
                        if( eDetailLevel != EMethodTraceDetailLevel::Undefined ) trcServerSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel = eDetailLevel.enumerator();
                        else xmlStreamReader.raiseError("Attribute \"NewTrcAdminObjsMethodCallsDefaultDetailLevel\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("NewTrcAdminObjsRuntimeInfoDefaultDetailLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("NewTrcAdminObjsRuntimeInfoDefaultDetailLevel").toString();
                        CEnumLogDetailLevel eDetailLevel = CEnumLogDetailLevel::fromString(strAttr);
                        if( eDetailLevel != ELogDetailLevel::Undefined ) trcServerSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = eDetailLevel.enumerator();
                        else xmlStreamReader.raiseError("Attribute \"NewTrcAdminObjsRuntimeInfoDefaultDetailLevel\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("UseIpcServer") )
                    {
                        strAttr = xmlStreamReader.attributes().value("UseIpcServer").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) trcServerSettings.m_bUseIpcServer = bVal;
                        else xmlStreamReader.raiseError("Attribute \"UseIpcServer\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("CacheDataIfNotConnected") )
                    {
                        strAttr = xmlStreamReader.attributes().value("CacheDataIfNotConnected").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) trcServerSettings.m_bCacheDataIfNotConnected = bVal;
                        else xmlStreamReader.raiseError("Attribute \"CacheDataIfNotConnected\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("CacheDataMaxArrLen") )
                    {
                        strAttr = xmlStreamReader.attributes().value("CacheDataMaxArrLen").toString();
                        iVal = strAttr.toInt(&bOk);
                        if( bOk ) trcServerSettings.m_iCacheDataMaxArrLen = iVal;
                        else xmlStreamReader.raiseError("Attribute \"CacheDataMaxArrLen\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("UseLocalTrcFile") )
                    {
                        strAttr = xmlStreamReader.attributes().value("UseLocalTrcFile").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) trcServerSettings.m_bUseLocalTrcFile = bVal;
                        else xmlStreamReader.raiseError("Attribute \"UseLocalTrcFile\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LocalTrcFileAbsFilePath") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LocalTrcFileAbsFilePath").toString();
                        trcServerSettings.m_strLocalTrcFileAbsFilePath = strAttr;
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LocalTrcFileAutoSaveInterval_ms") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LocalTrcFileAutoSaveInterval_ms").toString();
                        iVal = strAttr.toInt(&bOk);
                        if( bOk ) trcServerSettings.m_iLocalTrcFileAutoSaveInterval_ms = iVal;
                        else xmlStreamReader.raiseError("Attribute \"LocalTrcFileAutoSaveInterval_ms\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LocalTrcFileSubFileCountMax") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LocalTrcFileSubFileCountMax").toString();
                        iVal = strAttr.toInt(&bOk);
                        if( bOk ) trcServerSettings.m_iLocalTrcFileSubFileCountMax = iVal;
                        else xmlStreamReader.raiseError("Attribute \"LocalTrcFileSubFileCountMax\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LocalTrcFileSubFileLineCountMax") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LocalTrcFileSubFileLineCountMax").toString();
                        iVal = strAttr.toInt(&bOk);
                        if( bOk ) trcServerSettings.m_iLocalTrcFileSubFileLineCountMax = iVal;
                        else xmlStreamReader.raiseError("Attribute \"LocalTrcFileSubFileLineCountMax\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LocalTrcFileCloseAfterEachWrite") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LocalTrcFileCloseAfterEachWrite").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite = bVal;
                        else xmlStreamReader.raiseError("Attribute \"LocalTrcFileCloseAfterEachWrite\" (" + strAttr + ") is out of range");
                    }

                    // While receiving the trace settings emitting the traceSettingsChanged signal is blocked
                    // by setting the flag m_bOnReceivedDataUpdateInProcess to true to accumulate all changes
                    // and emit the signal just once. setTraceSettings will check if the settings have been
                    // changed before taken them over. But setTraceSettings will not emit the settings changed
                    // signal as the flag m_bOnReceivedDataUpdateInProcess is set. So we need to do this check
                    // here also and emit the signal.
                    if( trcServerSettings != m_trcSettings )
                    {
                        setTraceSettings(trcServerSettings);
                        emit traceSettingsChanged(this);
                    }
                } // if( strElemName == "ServerSettings" )

                else if( strElemName == "TrcAdminObj" )
                {
                    iObjId = -1;
                    enabled = EEnabled::Undefined;
                    bSetDataFilter = false;
                    strDataFilter = "";
                    eDetailLevelMethodCalls = EMethodTraceDetailLevel::None;
                    eDetailLevelRuntimeInfo = ELogDetailLevel::None;

                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("ObjId") )
                    {
                        strAttr = xmlStreamReader.attributes().value("ObjId").toString();
                        iObjId = strAttr.toInt(&bOk);
                        if( !bOk )
                        {
                            xmlStreamReader.raiseError("Attribute \"ObjId\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Enabled") )
                    {
                        strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                        enabled = CEnumEnabled::toEnumerator(strAttr);
                        if( enabled == EEnabled::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("MethodCallsDetailLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("MethodCallsDetailLevel").toString();
                        eDetailLevelMethodCalls = CEnumMethodTraceDetailLevel::toEnumerator(strAttr);
                        if( eDetailLevelMethodCalls == EMethodTraceDetailLevel::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"MethodCallsDetailLevel\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("RuntimeInfoDetailLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("RuntimeInfoDetailLevel").toString();
                        eDetailLevelRuntimeInfo = CEnumLogDetailLevel::toEnumerator(strAttr);
                        if( eDetailLevelRuntimeInfo == ELogDetailLevel::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"RuntimeInfoDetailLevel\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("DataFilter") )
                    {
                        strDataFilter = xmlStreamReader.attributes().value("DataFilter").toString();
                        bSetDataFilter = true;
                    }

                    if( iObjId < 0 || iObjId >= m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                    {
                        xmlStreamReader.raiseError("ObjectId " + QString::number(iObjId) + " for \"" + strElemName + "\" is out of range");
                    }
                    else // if( iObjId >= 0 && iObjId < m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                    {
                        CTrcAdminObj* pTrcAdminObj = m_pTrcAdminObjIdxTree->getTraceAdminObj(iObjId, false);

                        if( pTrcAdminObj == nullptr )
                        {
                            xmlStreamReader.raiseError("An Object with Id " + QString::number(iObjId) + " for \"" + strElemName + "\" is not existing");
                        }
                        else
                        {
                            pTrcAdminObj->setEnabled(enabled);
                            pTrcAdminObj->setMethodCallsTraceDetailLevel(eDetailLevelMethodCalls);
                            pTrcAdminObj->setRuntimeInfoTraceDetailLevel(eDetailLevelRuntimeInfo);
                            if( bSetDataFilter ) {
                                pTrcAdminObj->setTraceDataFilter(strDataFilter);
                            }
                        }
                    } // if( iObjId >= 0 && iObjId < m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                } // if( strElemName == "TrcAdminObj" )

                else if( strElemName == "NameSpace" )
                {
                    iObjId = -1;
                    enabled = EEnabled::Undefined;
                    bSetDataFilter = false;
                    strDataFilter = "";
                    eDetailLevelMethodCalls = EMethodTraceDetailLevel::None;
                    eDetailLevelRuntimeInfo = ELogDetailLevel::None;

                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("ObjId") )
                    {
                        strAttr = xmlStreamReader.attributes().value("ObjId").toString();
                        iObjId = strAttr.toInt(&bOk);
                        if( !bOk )
                        {
                            xmlStreamReader.raiseError("Attribute \"ObjId\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Enabled") )
                    {
                        strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                        enabled = CEnumEnabled::toEnumerator(strAttr);
                        if( enabled == EEnabled::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("MethodCallsDetailLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("MethodCallsDetailLevel").toString();
                        eDetailLevelMethodCalls = CEnumMethodTraceDetailLevel::toEnumerator(strAttr);
                        if( eDetailLevelMethodCalls == EMethodTraceDetailLevel::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"MethodCallsDetailLevel\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("RuntimeInfoDetailLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("RuntimeInfoDetailLevel").toString();
                        eDetailLevelRuntimeInfo = CEnumLogDetailLevel::toEnumerator(strAttr);
                        if( eDetailLevelRuntimeInfo == ELogDetailLevel::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"RuntimeInfoDetailLevel\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("DataFilter") )
                    {
                        strDataFilter = xmlStreamReader.attributes().value("DataFilter").toString();
                        bSetDataFilter = true;
                    }

                    if( iObjId < 0 || iObjId >= m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                    {
                        xmlStreamReader.raiseError("ObjectId " + QString::number(iObjId) + " for \"" + strElemName + "\" is out of range");
                    }
                    else // if( iObjId >= 0 && iObjId < m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                    {
                        CIdxTreeEntry* pTreeEntry = m_pTrcAdminObjIdxTree->getEntry(iObjId);

                        if( pTreeEntry == nullptr )
                        {
                            xmlStreamReader.raiseError("An Object with Id " + QString::number(iObjId) + " for \"" + strElemName + "\" is not existing");
                        }
                        else if( !pTreeEntry->isRoot() && !pTreeEntry->isBranch() )
                        {
                            xmlStreamReader.raiseError("The Object with Id " + QString::number(iObjId) + " for \"" + strElemName + "\" is not a name space node");
                        }
                        else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                        {
                            m_pTrcAdminObjIdxTree->setEnabled(iObjId, enabled);
                            m_pTrcAdminObjIdxTree->setMethodCallsTraceDetailLevel(iObjId, eDetailLevelMethodCalls);
                            m_pTrcAdminObjIdxTree->setRuntimeInfoTraceDetailLevel(iObjId, eDetailLevelRuntimeInfo);
                            if( bSetDataFilter ) {
                                m_pTrcAdminObjIdxTree->setTraceDataFilter(iObjId, strDataFilter);
                            }
                        }
                    } // if( iObjId >= 0 && iObjId < m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                } // if( strElemName == "NameSpace" )
            } // if( xmlStreamReader.isStartElement() )
        } // while( xmlStreamTokenType != QXmlStreamReader::EndElement && xmlStreamTokenType != QXmlStreamReader::Invalid )
    } // if( xmlStreamTokenType == QXmlStreamReader::StartDocument )

    if( xmlStreamReader.hasError() )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileReadContent);
            errResultInfo.setAddErrInfoDscr(xmlStreamReader.errorString());

            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    if( !m_trcSettings.m_strAdminObjFileAbsFilePath.isEmpty() )
    {
        saveAdminObjs();
    }

    m_bOnReceivedDataUpdateInProcess = false;

} // onIpcServerReceivedReqUpdate

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::onTrcAdminObjIdxTreeEntryAdded( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    // The trace admin object index tree will be locked so it will not be changed
    // when accessing it here.
    CMutexLocker mutexLocker(m_pTrcAdminObjIdxTree->mutex());

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTrcAdminObjIdxTreeEntryAdded",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

    CIdxTreeLocker idxTreeLocker(m_pTrcAdminObjIdxTree);

    CIdxTreeEntry* pTreeEntry = m_pTrcAdminObjIdxTree->findEntry(i_strKeyInTree);

    if( pTreeEntry != nullptr && isConnected() )
    {
        if( pTreeEntry->isBranch() )
        {
            sendBranch(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandInsert,
                /* pBranch       */ pTreeEntry );
        }
        else if( pTreeEntry->isLeave() )
        {
            sendAdminObj(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandInsert,
                /* strKeyInTree  */ pTreeEntry->keyInTree(),
                /* idxInTree     */ pTreeEntry->indexInTree() );
        }
    }
} // onTrcAdminObjIdxTreeEntryAdded

//------------------------------------------------------------------------------
void CIpcTrcServer::onTrcAdminObjIdxTreeEntryAboutToBeRemoved(
    const QString& i_strKeyInTree, int i_idxInTree )
//------------------------------------------------------------------------------
{
    // The trace admin object index tree will be locked so it will not be changed
    // when accessing it here.
    CMutexLocker mutexLocker(m_pTrcAdminObjIdxTree->mutex());

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Key: " + i_strKeyInTree;
        strMthInArgs += ", Idx: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTrcAdminObjIdxTreeEntryAboutToBeRemoved",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

    if( isConnected() )
    {
        // Could have already been removed and may no longer exist.
        CIdxTreeEntry* pTreeEntry = m_pTrcAdminObjIdxTree->findEntry(i_strKeyInTree);

        if( pTreeEntry != nullptr )
        {
            if( pTreeEntry->isBranch() )
            {
                sendBranch(
                    /* iSocketId     */ ESocketIdAllSockets,
                    /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                    /* cmd           */ MsgProtocol::ECommandDelete,
                    /* pBranch       */ pTreeEntry );
            }
            else if( pTreeEntry->isLeave() )
            {
                sendAdminObj(
                    /* iSocketId     */ ESocketIdAllSockets,
                    /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                    /* cmd           */ MsgProtocol::ECommandDelete,
                    /* strKeyInTree  */ i_strKeyInTree,
                    /* idxInTree     */ i_idxInTree );
            }
        }
    }
} // onTrcAdminObjIdxTreeEntryAboutToBeRemoved

//------------------------------------------------------------------------------
void CIpcTrcServer::onTrcAdminObjIdxTreeEntryChanged( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTrcAdminObjIdxTreeEntryChanged",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    // The trace admin object index tree will be locked so it will not be changed
    // when accessing it here.
    CMutexLocker mutexLocker(m_pTrcAdminObjIdxTree->mutex());

    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

    CIdxTreeLocker idxTreeLocker(m_pTrcAdminObjIdxTree);

    CIdxTreeEntry* pTreeEntry = m_pTrcAdminObjIdxTree->findEntry(i_strKeyInTree);

    if( pTreeEntry != nullptr && isConnected() )
    {
        if( pTreeEntry->isRoot() || pTreeEntry->isBranch() )
        {
            sendBranch(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandInsert,
                /* pBranch       */ pTreeEntry );
        }
        else if( pTreeEntry->isLeave() )
        {
            sendAdminObj(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandUpdate,
                /* strKeyInTree  */ pTreeEntry->keyInTree(),
                /* idxInTree     */ pTreeEntry->indexInTree() );
        }
    }
} // onTrcAdminObjIdxTreeEntryChanged

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CIpcTrcServer::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pMsg->type() >= QEvent::User )
    {
        QString strAddTrcInfo;
        QString strAddErrInfo;

        CMsg* pMsg = nullptr;

        try
        {
            pMsg = dynamic_cast<CMsg*>(i_pMsg);
        }
        catch(...)
        {
            pMsg = nullptr;
        }

        if( m_pTrcMthFile != nullptr && m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            int iAddTrcInfoDetailLevel = 0;
            if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
            else if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

            strAddTrcInfo = "Msg {" + QString( pMsg == nullptr ? "nullptr" : pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* eTrcDetailLevel    */ m_eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ nameSpace(),
            /* strClassName       */ className(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "event",
            /* strAddInfo         */ strAddTrcInfo );

        if( pMsg != nullptr )
        {
            QMutexLocker mtxLocker(&s_mtx);

            bEventHandled = true;

            if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
            {
                //--------------------------------------------------------------
                // Sending request messages to the server by proxies is not yet supported.
                // For this setting request and confirmation messages at the request instance
                // must be revised as for blocking requests the gateway also sets the
                // confirmation message to indicate the result of the executed request.
                //--------------------------------------------------------------

                CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(pMsg);
                if( pMsgReq == nullptr )
                {
                    throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch);
                }

                //qint64   iReqIdParent = pMsgReq->getRequestId();
                //CMsgCon* pMsgCon = nullptr;

                //if( isAsynchronousRequest(pReq) )
                //{
                //    pReq->setConfirmationMessage(pMsgCon);
                //    pMsgCon = nullptr;
                //}
                //else // if( !isAsynchronousRequest(pReq) )
                //{
                //    bool bIsWaiting = false;

                //    if( iReqIdParent >= 0 )
                //    {
                //        bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
                //    }

                //    if( bIsWaiting != 0 )
                //    {
                //        CRequestExecTree::GetInstance()->wake(iReqIdParent);
                //    }
                //    else if( pMsgCon != nullptr )
                //    {
                //        if( pReq != nullptr )
                //        {
                //            errResultInfo = pReq->getErrResultInfo();
                //        }
                //        else if( errResultInfo.getResult() == EResultUndefined )
                //        {
                //            errResultInfo.setResult(EResultSuccess);
                //        }
                //        pMsgCon->setErrResultInfo(errResultInfo);
                //        pMsgCon->setProgress(100);

                //        POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ELogDetailLevel);
                //        pMsgCon = nullptr;
                //    }
                //} // if( !isAsynchronousRequest(pReq) )

                //delete pMsgCon;
                //pMsgCon = nullptr;

                switch( static_cast<int>(pMsgReq->type()) )
                {
                    case EMsgTypeReqStartup:
                    {
                        CMsgReqStartup* pMsgReq = dynamic_cast<CMsgReqStartup*>(pMsg);
                        if( pMsgReq == nullptr )
                        {
                            throw ZS::System::CException(__FILE__, __LINE__, EResultMessageTypeMismatch, "MsgReqStartup == nullptr! " + strAddTrcInfo);
                        }
                        setHostSettings(pMsgReq->getHostSettings());
                        setBlkType(pMsgReq->getBlkType(true)); // The trace server takes ownership of the block.
                        startup(10000, true);
                        break;
                    } // case EMsgTypeReqStartup

                    case EMsgTypeReqShutdown:
                    {
                        CMsgReqShutdown* pMsgReq = dynamic_cast<CMsgReqShutdown*>(pMsg);
                        if( pMsgReq == nullptr )
                        {
                            throw ZS::System::CException(__FILE__, __LINE__, EResultMessageTypeMismatch, "MsgReqShutdown == nullptr! " + strAddTrcInfo);
                        }
                        shutdown(10000, true);
                        break;
                    }

                    case EMsgTypeReqChangeSettings:
                    {
                        CMsgReqChangeSettings* pMsgReq = dynamic_cast<CMsgReqChangeSettings*>(pMsg);
                        if( pMsgReq == nullptr )
                        {
                            throw ZS::System::CException(__FILE__, __LINE__, EResultMessageTypeMismatch, "MsgReqChangeSettings == nullptr! " + strAddTrcInfo);
                        }
                        setHostSettings(pMsgReq->getServerHostSettings());
                        changeSettings(10000, true);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                } // switch( static_cast<int>(pMsg->type()) )
            } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
        } // if( pMsg != nullptr )
    } // if( pMsg->type() >= QEvent::User )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event
