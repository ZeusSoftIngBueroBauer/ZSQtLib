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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qmutex.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequest.h"
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

QString                m_strMthThreadName;          // Name of the thread in which context the method was called from.
QDateTime              m_dt;                        // Current date time of the method call.
double                 m_fSysTimeInSec;             // Time in seconds since start of the program at which the method was called.
ZS::System::EMethodDir m_mthDir;                    // Methods may be entered and left.
int                    m_iTrcAdminObjId;
QString                m_strNameSpace;
QString                m_strClassName;
QString                m_strObjName;                // Name of the object for which the method was applied.
QString                m_strMthName;                // Name of the method to be traced.
QString                m_strMthInArgs;              // Only valid if mthDir == Enter
QString                m_strMthOutArgs;             // Only valid if mthDir == Leave
QString                m_strMthRet;                 // Only valid if mthDir == Leave
QString                m_strMthAddInfo;             // Only valid if mthDir == None

//==============================================================================
public: // ctors and dtor
//==============================================================================

//------------------------------------------------------------------------------
SMthTrcData() :
//------------------------------------------------------------------------------
    m_strMthThreadName(),
    m_dt(),
    m_fSysTimeInSec(0.0),
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
    m_fSysTimeInSec(i_fSysTimeInSec),
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
    //m_fSysTimeInSec;
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
    /*    23 */ str += "SysTime=\"" + QString::number(m_fSysTimeInSec,'f',6) + "\" ";
    if( m_mthDir == EMethodDir::Enter )
    {
        QString strMthInArgs = m_strMthInArgs;
        if( strMthInArgs.contains('<') )
        {
            strMthInArgs.replace("<","&lt;");
        }
        if( strMthInArgs.contains('>') )
        {
            strMthInArgs.replace(">","&gt;");
        }
    /*    10 */ str += "InArgs=\"" + strMthInArgs + "\" ";
    }
    else if( m_mthDir == EMethodDir::Leave )
    {
        QString strMthRet = m_strMthRet;
        if( strMthRet.contains('<') )
        {
            strMthRet.replace("<","&lt;");
        }
        if( strMthRet.contains('>') )
        {
            strMthRet.replace(">","&gt;");
        }
        QString strMthOutArgs = m_strMthOutArgs;
        if( strMthOutArgs.contains('<') )
        {
            strMthOutArgs.replace("<","&lt;");
        }
        if( strMthOutArgs.contains('>') )
        {
            strMthOutArgs.replace(">","&gt;");
        }
    /*    10 */ str += "Return=\"" + strMthRet + "\" ";
    /*    10 */ str += "OutArgs=\"" + strMthOutArgs + "\" ";
    }
    else // if( i_dir == EMethodDir::None )
    {
        QString strMthAddInfo = m_strMthAddInfo;
        if( strMthAddInfo.contains('<') )
        {
            strMthAddInfo.replace("<","&lt;");
        }
        if( strMthAddInfo.contains('>') )
        {
            strMthAddInfo.replace(">","&gt;");
        }
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
CIpcTrcServer* CIpcTrcServer::GetInstance( const QString& i_strName )
//------------------------------------------------------------------------------
{
    CIpcTrcServer* pIpcTrcServer = nullptr;

    CTrcServer* pTrcServer = CTrcServer::GetInstance(i_strName);

    if( pTrcServer != nullptr )
    {
        pIpcTrcServer = dynamic_cast<CIpcTrcServer*>(pTrcServer);
    }
    return pIpcTrcServer;

} // GetInstance

//------------------------------------------------------------------------------
/*! Returns a reference to an instance of the class.

    Checks whether a trace server instance with the desired name already exist.
    If not the trace server instance is created.
    Depending on the flag CreateOnlyIfNotYetExisting either an exception is thrown
    if a server with name is already existing or a reference to the already existing
    instance is return and a reference counter is incremented.

    \param i_strName [in] Name of the trace server (default "ZSTrcServer")
    \param i_bCreateOnlyIfNotYetExisting [in] (default: false)
        If this flag is set to false and a trace server with the given name
        is already existing an exception will be thrown.
        If set to true and a trace server with the given name is already existing
        a reference to the already existing server is returned and the reference
        count for the server is incremented.
        If a trace server with the given name is not yet existing the server
        will be created anyway and the reference count is set to 1.
    \param i_iTrcDetailLevel [in]
        If the methods of the trace server itself should be logged a value
        greater than 0 (ETraceDetailLevelNone) could be passed here.

    \return Pointer to trace server instance.
*/
CIpcTrcServer* CIpcTrcServer::CreateInstance(
    const QString& i_strName,
    bool           i_bCreateOnlyIfNotYetExisting,
    int            i_iTrcDetailLevel )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CTrcServer* pTrcServer = s_hshpInstances.value(i_strName, nullptr);

    if (pTrcServer != nullptr && !i_bCreateOnlyIfNotYetExisting)
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, "CIpcTrcServer::" + i_strName);
    }

    CIpcTrcServer* pIpcTrcServer = nullptr;

    if (pTrcServer == NULL)
    {
        pIpcTrcServer = new CIpcTrcServer(i_strName, i_iTrcDetailLevel);
    }
    else
    {
        pIpcTrcServer = dynamic_cast<CIpcTrcServer*>(pTrcServer);

        if (pIpcTrcServer == nullptr)
        {
            throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, "CTrcServer::" + i_strName);
        }
    }

    pIpcTrcServer->incrementRefCount();

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
void CIpcTrcServer::ReleaseInstance( const QString& i_strName )
//------------------------------------------------------------------------------
{
    CTrcServer::ReleaseInstance(i_strName);
}

//------------------------------------------------------------------------------
void CIpcTrcServer::ReleaseInstance( CIpcTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    CTrcServer::ReleaseInstance(i_pTrcServer);
}

//------------------------------------------------------------------------------
void CIpcTrcServer::DestroyAllInstances()
//------------------------------------------------------------------------------
{
    CTrcServer::DestroyAllInstances();
}

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CIpcTrcServer::CIpcTrcServer( const QString& i_strName, int i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    CTrcServer(i_strName, i_iTrcDetailLevel),
    m_pIpcServer(nullptr),
    m_bIsBeingDestroyed(false),
    m_ariSocketIdsConnectedTrcClients(),
    m_ariSocketIdsRegisteredTrcClients(),
    m_bOnReceivedDataUpdateInProcess(false),
    m_pMtxListTrcDataCached(nullptr),
    m_iTrcDataCachedCount(0),
    m_arpTrcDataCached()
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

    // If used via the dll interface the trace server might be created in another thread than
    // the main thread. As it cannot be ensured in all cases that the thread has an own
    // event loop the trace server will be moved to the main thread. But this implies that also
    // the index tree is moved together with the trace server (otherwise the Ipc server will not
    // be able to receive queued signal/slots). "moveToThread" moves the object together with its children.
    m_pTrcAdminObjIdxTree->setParent(this);

    // Please note that all Ipc objects used by the trace server may only trace into the local trace file.
    // On the one hand when creating e.g. the gateway instance and the gateway would try to add or modify
    // a trace admin object in the index tree this change shold be sent to all connected trace clients.
    // But the Ipc server is not yet started and cannot send anything to clients.
    // On the other hand the "sendData" method is traced wherupon data will be sent to the client by
    // calling "sendData" which again is traced calling "sendData" and so on (endless recursion, deadlock).
    // And by the way - tracing the tracer is not always necessary.

    // The trace server must aggregate the IpcServer to avoid multiple inheritance of QObject.
    m_pIpcServer = new CServer(i_strName, true, m_pTrcMthFile, m_iTrcDetailLevel),

    // If used via the dll interface the trace server might be created in another thread than
    // the main thread. As it cannot be ensured in all cases that the thread has an own
    // event loop the trace server will be moved to the main thread. But this implies that also
    // the Ipc server is moved together with the trace server (otherwise the Ipc server will not
    // be able to receive events). "moveToThread" moves the object together with its children.
    m_pIpcServer->setParent(this);

    m_pMtxListTrcDataCached = new QMutex(QMutex::Recursive);

    if( !QObject::connect(
        /* pObjSender   */ m_pTrcAdminObjIdxTree,
        /* szSignal     */ SIGNAL( treeEntryAdded(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTrcAdminObjIdxTreeEntryAdded(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*) ),
        /* cnctType     */ Qt::DirectConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTrcAdminObjIdxTree,
        /* szSignal     */ SIGNAL( treeEntryChanged(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTrcAdminObjIdxTreeEntryChanged(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*) ),
        /* cnctType     */ Qt::DirectConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pIpcServer,
        /* szSignal     */ SIGNAL( connected(QObject*,const ZS::Ipc::SSocketDscr&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onIpcServerConnected(QObject*,const ZS::Ipc::SSocketDscr&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pIpcServer,
        /* szSignal     */ SIGNAL( disconnected(QObject*,const ZS::Ipc::SSocketDscr&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onIpcServerDisconnected(QObject*,const ZS::Ipc::SSocketDscr&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pIpcServer,
        /* szSignal     */ SIGNAL( receivedData(QObject*,int,const QByteArray&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onIpcServerReceivedData(QObject*,int,const QByteArray&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CIpcTrcServer::~CIpcTrcServer()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMthInArgs       */ "" );

    m_bIsBeingDestroyed = true;

    int idx;

    for( idx = 0; idx < m_iTrcDataCachedCount; idx++ )
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

    // Please see comment in ctor concerning the "setParent" for the Ipc server.
    // The Ipc server got to be deleted before the trace admin object pool and
    // therefore all living trace admin objects. For this we have to reparent
    // the Ipc server so it will not be automatically deleted as a child of this
    // trace server by Qt.
    m_pIpcServer->setParent(nullptr);

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

    m_pIpcServer = nullptr;
    m_bIsBeingDestroyed = false;
    //m_ariSocketIdsConnectedTrcClients.clear();
    //m_ariSocketIdsRegisteredTrcClients.clear();
    m_bOnReceivedDataUpdateInProcess = false;
    m_pMtxListTrcDataCached = nullptr;
    m_iTrcDataCachedCount = 0;
    m_arpTrcDataCached.clear();

} // dtor

/*==============================================================================
public: // overridables of base class CTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
bool CIpcTrcServer::isActive() const
//------------------------------------------------------------------------------
{
    return isLocalTrcFileActive() || getCacheTrcDataIfNotConnected() || isConnected();
}

/*==============================================================================
public: // overridables of base class CTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = bool2Str(i_bEnabled);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setEnabled",
        /* strMthInArgs       */ strMthInArgs );

    if( i_bEnabled != m_trcSettings.m_bEnabled )
    {
        m_trcSettings.m_bEnabled = i_bEnabled;

        emit traceSettingsChanged(this);

        if( isConnected() )
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
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = bool2Str(i_bEnabled);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setNewTrcAdminObjsEnabledAsDefault",
        /* strMthInArgs       */ strMthInArgs );

    if( m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault != i_bEnabled )
    {
        m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault = i_bEnabled;

        emit traceSettingsChanged(this);

        if( isConnected() )
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
void CIpcTrcServer::setNewTrcAdminObjsDefaultDetailLevel( int i_iDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = QString::number(i_iDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setNewTrcAdminObjsDefaultDetailLevel",
        /* strMthInArgs       */ strMthInArgs );

    if( m_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel != i_iDetailLevel )
    {
        m_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel = i_iDetailLevel;

        emit traceSettingsChanged(this);

        if( isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings NewTrcAdminObjsDefaultDetailLevel=" + QString::number(m_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel) + "/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }

} // setNewTrcAdminObjsDefaultDetailLevel

/*==============================================================================
public: // overridables of base class CTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setAdminObjFileAbsoluteFilePath",
        /* strMthInArgs       */ strMthInArgs );

    if( m_trcSettings.m_strAdminObjFileAbsFilePath != i_strAbsFilePath )
    {
        m_trcSettings.m_strAdminObjFileAbsFilePath = i_strAbsFilePath;

        emit traceSettingsChanged(this);

        if( isConnected() )
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
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = bool2Str(i_bUse);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setUseLocalTrcFile",
        /* strMthInArgs       */ strMthInArgs );

    if( m_trcSettings.m_bUseLocalTrcFile != i_bUse )
    {
        m_trcSettings.m_bUseLocalTrcFile = i_bUse;

        if( !m_trcSettings.m_bUseLocalTrcFile && m_pTrcMthFile != nullptr )
        {
            m_pTrcMthFile->close();
        }

        emit traceSettingsChanged(this);

        if( isConnected() )
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
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileAbsoluteFilePath",
        /* strMthInArgs       */ strMthInArgs );

    if( (m_trcSettings.m_strLocalTrcFileAbsFilePath != i_strAbsFilePath)
     || (m_pTrcMthFile == nullptr && !m_trcSettings.m_strLocalTrcFileAbsFilePath.isEmpty()) )
    {
        m_trcSettings.m_strLocalTrcFileAbsFilePath = i_strAbsFilePath;

        if( m_trcSettings.m_strLocalTrcFileAbsFilePath.isEmpty() )
        {
            if( m_pTrcMthFile != nullptr )
            {
                CTrcMthFile::Free(m_pTrcMthFile);
                m_pTrcMthFile = nullptr;
            }
        }
        else // if( !m_trcSettings.m_strLocalTrcFileAbsFilePath.isEmpty() )
        {
            if( m_pTrcMthFile != nullptr && m_pTrcMthFile->absoluteFilePath() != m_trcSettings.m_strLocalTrcFileAbsFilePath )
            {
                CTrcMthFile::Free(m_pTrcMthFile);
                m_pTrcMthFile = nullptr;
            }
            if( m_pTrcMthFile == nullptr )
            {
                m_pTrcMthFile = CTrcMthFile::Alloc(m_trcSettings.m_strLocalTrcFileAbsFilePath);
            }
            m_pTrcMthFile->setAutoSaveInterval(m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms);
            m_pTrcMthFile->setSubFileCountMax(m_trcSettings.m_iLocalTrcFileSubFileCountMax);
            m_pTrcMthFile->setSubFileLineCountMax(m_trcSettings.m_iLocalTrcFileSubFileLineCountMax);
            m_pTrcMthFile->setCloseFileAfterEachWrite(m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite);

        } // if( !m_trcSettings.m_strLocalTrcFileAbsFilePath.isEmpty() )

        emit traceSettingsChanged(this);

        if( isConnected() )
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
void CIpcTrcServer::setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = bool2Str(i_bCloseFile);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileCloseFileAfterEachWrite",
        /* strMthInArgs       */ strMthInArgs );

    if( m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite != i_bCloseFile )
    {
        m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite = i_bCloseFile;

        if( m_pTrcMthFile != nullptr )
        {
            if( m_pTrcMthFile->getCloseFileAfterEachWrite() != i_bCloseFile )
            {
                m_pTrcMthFile->setCloseFileAfterEachWrite(i_bCloseFile);
            }
        }

        emit traceSettingsChanged(this);

        if( isConnected() )
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
void CIpcTrcServer::setCacheTrcDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = bool2Str(i_bCacheData);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setCacheTrcDataIfNotConnected",
        /* strMthInArgs       */ strMthInArgs );

    if( m_trcSettings.m_bCacheDataIfNotConnected != i_bCacheData )
    {
        m_trcSettings.m_bCacheDataIfNotConnected = i_bCacheData;

        emit traceSettingsChanged(this);

        if( isConnected() )
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
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = QString::number(i_iMaxArrLen);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setCacheTrcDataMaxArrLen",
        /* strMthInArgs       */ strMthInArgs );

    if( m_trcSettings.m_iCacheDataMaxArrLen != i_iMaxArrLen )
    {
        m_trcSettings.m_iCacheDataMaxArrLen = i_iMaxArrLen;

        emit traceSettingsChanged(this);

        if( isConnected() )
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
public: // overridables of base class CTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcServer::traceMethodEnter(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strMethodInArgs )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelMethodCalls) && isEnabled() && isActive() )
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
void CIpcTrcServer::traceMethodEnter(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strMethodInArgs )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelMethodCalls) && isEnabled() && isActive() )
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
void CIpcTrcServer::traceMethod(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strAddInfo )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // Send trace message to the trace server.
    // Using messages avoids using a mutex to protect the list of traced method
    // calls as "traceMethodEnter" is usually called from within different
    // thread contexts.

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelRuntimeInfo) && isEnabled() && isActive() )
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
void CIpcTrcServer::traceMethod(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strAddInfo )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // Send trace message to the trace server.
    // Using messages avoids using a mutex to protect the list of traced method
    // calls as "traceMethodEnter" is usually called from within different
    // thread contexts.

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelRuntimeInfo) && isEnabled() && isActive() )
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
void CIpcTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strMethodReturn,
    const QString&      i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // Send trace message to the trace server.
    // Using messages avoids using a mutex to protect the list of traced method
    // calls as "traceMethodEnter" is usually called from within different
    // thread contexts.

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelMethodCalls) && isEnabled() && isActive() )
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
void CIpcTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strMethodReturn,
    const QString&      i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // Send trace message to the trace server.
    // Using messages avoids using a mutex to protect the list of traced method
    // calls as "traceMethodEnter" is usually called from within different
    // thread contexts.

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelMethodCalls) && isEnabled() && isActive() )
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
        // A client must have queried the trace admin objects in order to assign the
        // received trace data to the trace admin objects by their index tree ids.
        // Furthermore the client must have registered itself via a request select
        // command for trace method data.
        // Please note that only the first registered client will received the cached data.

        // If no client is registered for receiving online trace data yet ..
        if( m_ariSocketIdsRegisteredTrcClients.length() == 0 )
        {
            // .. the trace data will be cached.
            bAdd2Cache = true;
        }
    } // if( m_trcSettings.m_bCacheDataIfNotConnected && m_trcSettings.m_iCacheDataMaxArrLen > 0 )

    if( bAdd2Cache )
    {
        // ... temporarily store the trace message until a client connects to the trace server.
        QMutexLocker mutexLocker(m_pMtxListTrcDataCached);

        int idx;

        if( m_iTrcDataCachedCount == m_arpTrcDataCached.size() )
        {
            const int ciMaxArrLen   = m_trcSettings.m_iCacheDataMaxArrLen;
            const int ciRemoveCount = 200;

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

    else if( m_ariSocketIdsRegisteredTrcClients.length() > 0 )
    {
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

        for( auto& iSocketId : m_ariSocketIdsRegisteredTrcClients )
        {
            sendData(iSocketId, byteArrMsg);
        }
    } // if( !bAdd2Cache && m_ariSocketIdsRegisteredTrcClients.length() > 0 )

} // addEntry

/*==============================================================================
public: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CIpcTrcServer::startup( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strMthInArgs += ", Wait: " + bool2Str(i_bWait);
        strMthInArgs += ", ReqIdParent: " + QString::number(i_iReqIdParent);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strMthInArgs += ", Wait: " + bool2Str(i_bWait);
        strMthInArgs += ", ReqIdParent: " + QString::number(i_iReqIdParent);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "ReqId: " + QString::number(i_iRequestId);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    CBranchIdxTreeEntry*        i_pBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", MsgType: " + systemMsgType2Str(i_systemMsgType);
        strMthInArgs += ", Cmd: " + command2Str(i_cmd);
        strMthInArgs += ", Branch: " + QString(i_pBranch == nullptr ? "null" : i_pBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendBranch",
        /* strMthInArgs       */ strMthInArgs );

    if( isConnected() && i_pBranch != nullptr )
    {
        if( i_pBranch->entryType() != EIdxTreeEntryType::Root )
        {
            QString strMsg;
            QString strBranchName = i_pBranch->name();

            if( strBranchName.contains('<') )
            {
                strBranchName.replace("<","&lt;");
            }
            if( strBranchName.contains('>') )
            {
                strBranchName.replace(">","&gt;");
            }

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

        } // if( i_pBranch->entryType() != EIdxTreeEntryType::Root )

        CAbstractIdxTreeEntry* pTreeEntry;
        int                    idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->entryType() == EIdxTreeEntryType::Root
                 || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
                {
                    sendBranch(
                        /* iSocketId     */ i_iSocketId,
                        /* systemMsgType */ i_systemMsgType,
                        /* cmd           */ i_cmd,
                        /* pBranch       */ dynamic_cast<CBranchIdxTreeEntry*>(pTreeEntry) );
                }
                else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    sendAdminObj(
                        /* iSocketId     */ i_iSocketId,
                        /* systemMsgType */ i_systemMsgType,
                        /* cmd           */ i_cmd,
                        /* pTrcAdminObj  */ dynamic_cast<CTrcAdminObj*>(pTreeEntry) );
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
    CTrcAdminObj*               i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", MsgType: " + systemMsgType2Str(i_systemMsgType);
        strMthInArgs += ", Cmd: " + command2Str(i_cmd);
        strMthInArgs += ", TrcAdminObj: " + QString(i_pTrcAdminObj == nullptr ? "null" : i_pTrcAdminObj->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    if( isConnected() )
    {
        if( i_pTrcAdminObj != nullptr )
        {
            QString strMsg;
            QString strObjName = i_pTrcAdminObj->getObjectName();
            QString strThreadName = i_pTrcAdminObj->getObjectThreadName();

            if( strObjName.contains('<') )
            {
                strObjName.replace("<","&lt;");
            }
            if( strObjName.contains('>') )
            {
                strObjName.replace(">","&gt;");
            }
            if( strThreadName.contains('<') )
            {
                strThreadName.replace("<","&lt;");
            }
            if( strThreadName.contains('>') )
            {
                strThreadName.replace(">","&gt;");
            }

            strMsg += systemMsgType2Str(i_systemMsgType) + " ";
            strMsg += command2Str(i_cmd) + " ";
            strMsg += "<TrcAdminObj";
            if( i_pTrcAdminObj->parentBranch() != nullptr )
            {
                strMsg += " ParentBranchIdxInTree=\"" + QString::number(i_pTrcAdminObj->parentBranch()->indexInTree()) + "\"";
            }
            strMsg += " Name=\"" + strObjName + "\"";
            strMsg += " IdxInTree=\"" + QString::number(i_pTrcAdminObj->indexInTree()) + "\"";
            strMsg += " Thread=\"" + strThreadName + "\"";
            strMsg += " Enabled=\"" + CEnumEnabled::toString(i_pTrcAdminObj->getEnabled()) + "\"";
            strMsg += " DetailLevel=\"" + QString::number(i_pTrcAdminObj->getTraceDetailLevel()) + "\"";
            strMsg += " RefCount=\"" + QString::number(i_pTrcAdminObj->getRefCount()) + "\"";
            strMsg += "/>";

            sendData( i_iSocketId, str2ByteArr(strMsg) );

        } // if( i_pTrcAdminObj != nullptr )
    } // if( isConnected() )

} // sendAdminObj

//------------------------------------------------------------------------------
void CIpcTrcServer::sendBranch(
    int                         i_iSocketId,
    MsgProtocol::TSystemMsgType i_systemMsgType,
    MsgProtocol::TCommand       i_cmd,
    CBranchIdxTreeEntry*        i_pBranch,
    EEnabled                    i_enabled,
    int                         i_iDetailLevel )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", MsgType: " + systemMsgType2Str(i_systemMsgType);
        strMthInArgs += ", Cmd: " + command2Str(i_cmd);
        strMthInArgs += ", TrcAdminObj: " + QString(i_pBranch == nullptr ? "null" : i_pBranch->keyInTree());
        strMthInArgs += ", Enabled: " + CEnumEnabled(i_enabled).toString();
        strMthInArgs += ", DetailLevel: " + QString::number(i_iDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendBranch",
        /* strMthInArgs       */ strMthInArgs );

    if( i_pBranch != nullptr && isConnected() )
    {
        QString strMsg;
        QString strBranchName = i_pBranch->name();

        if( strBranchName.contains('<') )
        {
            strBranchName.replace("<","&lt;");
        }
        if( strBranchName.contains('>') )
        {
            strBranchName.replace(">","&gt;");
        }

        strMsg += systemMsgType2Str(i_systemMsgType) + " ";
        strMsg += command2Str(i_cmd) + " ";

        strMsg += "<Branch ";

        if( i_pBranch->parentBranch() != nullptr )
        {
            strMsg += " ParentBranchIdxInTree=" + QString::number(i_pBranch->parentBranch()->indexInTree());
        }
        strMsg += " Name=\"" + strBranchName + "\"";
        strMsg += " IdxInTree=" + QString::number(i_pBranch->indexInTree());

        if( static_cast<int>(i_enabled) >= 0 && static_cast<int>(i_enabled) < CEnumEnabled::count() )
        {
            strMsg += " Enabled=\"" + CEnumEnabled::toString(i_enabled) + "\"";
        }
        if( i_iDetailLevel >= 0 )
        {
            strMsg += " DetailLevel=" + QString::number(i_iDetailLevel);
        }
        strMsg += "/>";

        sendData( i_iSocketId, str2ByteArr(strMsg) );

    } // if( i_pBranch != nullptr && isConnected() )

} // sendBranch

/*==============================================================================
protected: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CIpcTrcServer::sendData( int i_iSocketId, const QByteArray& i_byteArr, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
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
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "{" + i_socketDscr.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcServerConnected",
        /* strMthInArgs       */ strMthInArgs );

    if( !m_ariSocketIdsConnectedTrcClients.contains(i_socketDscr.m_iSocketId) )
    {
        m_ariSocketIdsConnectedTrcClients.append(i_socketDscr.m_iSocketId);
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
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "{" + i_socketDscr.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcServerDisconnected",
        /* strMthInArgs       */ strMthInArgs );

    if( m_ariSocketIdsConnectedTrcClients.contains(i_socketDscr.m_iSocketId) )
    {
        if( !m_ariSocketIdsRegisteredTrcClients.contains(i_socketDscr.m_iSocketId) )
        {
            QString strAddErrInfo = "Received disconnected signal for socket " + i_socketDscr.getConnectionString() + " whose client did not register to receive trace data";
            SErrResultInfo errResultInfo = ErrResultInfoError("onIpcServerDisconnected", EResultSocketIdOutOfRange, strAddErrInfo);
        }
        m_ariSocketIdsConnectedTrcClients.removeOne(i_socketDscr.m_iSocketId);
    }
    else
    {
        QString strAddErrInfo = "Received disconnected signal for not connected socket " + i_socketDscr.getConnectionString();
        SErrResultInfo errResultInfo = ErrResultInfoError("onIpcServerDisconnected", EResultSocketIdOutOfRange, strAddErrInfo);
    }

    if( m_ariSocketIdsRegisteredTrcClients.contains(i_socketDscr.m_iSocketId) )
    {
        m_ariSocketIdsRegisteredTrcClients.removeOne(i_socketDscr.m_iSocketId);
    }

} // onIpcServerDisconnected

//------------------------------------------------------------------------------
void CIpcTrcServer::onIpcServerReceivedData(
    QObject*          /*i_pServer*/,
    int               i_iSocketId,
    const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", ByteArr: " + i_byteArr.left(20);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcServerReceivedData",
        /* strMthInArgs       */ strMthInArgs );

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
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // The trace admin object index tree will be locked so it will not be changed
    // when sending the whole content of the index tree to the client.
    QMutexLocker mutexLocker(m_pTrcAdminObjIdxTree->mutex());

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", Data: " + i_strData.left(20);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcServerReceivedReqSelect",
        /* strMthInArgs       */ strMthInArgs );

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
        while( xmlStreamTokenType != QXmlStreamReader::EndElement )
        {
            xmlStreamTokenType = xmlStreamReader.readNext();

            if( xmlStreamReader.isStartElement() )
            {
                strElemName = xmlStreamReader.name().toString();

                if( strElemName == "ServerSettings" )
                {
                    // .. parse the incoming XML data string (but here we expect no more entries).

                    // Send attributes of the server itself.
                    strDataSnd  = systemMsgType2Str(MsgProtocol::ESystemMsgTypeCon) + " ";
                    strDataSnd += command2Str(MsgProtocol::ECommandSelect) + " ";
                    strDataSnd += "<ServerSettings ";
                    strDataSnd += " Enabled=\"" + bool2Str(m_trcSettings.m_bEnabled) + "\"";
                    strDataSnd += " NewTrcAdminObjsEnabledAsDefault=\"" + bool2Str(m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault) + "\"";
                    strDataSnd += " NewTrcAdminObjsDefaultDetailLevel=\"" + QString::number(m_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel) + "\"";
                    strDataSnd += " CacheDataIfNotConnected=\"" + bool2Str(m_trcSettings.m_bCacheDataIfNotConnected) + "\"";
                    strDataSnd += " CacheDataMaxArrLen=\"" + QString::number(m_trcSettings.m_iCacheDataMaxArrLen) + "\"";
                    strDataSnd += " AdminObjFileAbsFilePath=\"" + m_trcSettings.m_strAdminObjFileAbsFilePath + "\"";
                    strDataSnd += " UseLocalTrcFile=\"" + bool2Str(m_trcSettings.m_bUseLocalTrcFile) + "\"";
                    strDataSnd += " LocalTrcFileAbsFilePath=\"" + m_trcSettings.m_strLocalTrcFileAbsFilePath + "\"";
                    strDataSnd += " LocalTrcFileCloseAfterEachWrite=\"" + bool2Str(m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite) + "\"";
                    strDataSnd += "/>";

                    sendData(i_iSocketId, str2ByteArr(strDataSnd));

                } // if( strElemName == "ServerSettings" )

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
                    if( !m_ariSocketIdsRegisteredTrcClients.contains(i_iSocketId) )
                    {
                        m_ariSocketIdsRegisteredTrcClients.append(i_iSocketId);
                    }

                    // If traced data has been temporarily stored ...
                    if( m_iTrcDataCachedCount > 0 )
                    {
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

                    } // if( m_iTrcDataCachedCount > 0 )
                } // if( strElemName == "TrcData" )

                else
                {
                    xmlStreamReader.raiseError("Invalid element \"" + strElemName + "\" in select request");
                }
            } // if( xmlStreamTokenType == QXmlStreamReader::StartElement )
        } // while( !xmlStreamReader.atEnd() )
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
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // The trace admin object index tree will be locked so it will not be changed
    // when sending the whole content of the index tree to the client.
    QMutexLocker mutexLocker(m_pTrcAdminObjIdxTree->mutex());

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SocketId: " + QString::number(i_iSocketId);
        strMthInArgs += ", Data: " + i_strData.left(20);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcServerReceivedReqUpdate",
        /* strMthInArgs       */ strMthInArgs );

    QString strMth = "onIpcServerReceivedReqUpdate";

    m_bOnReceivedDataUpdateInProcess = true;

    QXmlStreamReader xmlStreamReader(i_strData);

    QXmlStreamReader::TokenType xmlStreamTokenType;
    QString                     strElemName;
    QString                     strAttr;
    int                         iVal;
    int                         iObjId;
    EEnabled                    enabled;
    int                         iDetailLevel;
    bool                        bConverted;

    xmlStreamTokenType = xmlStreamReader.readNext();

    if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
    {
        xmlStreamReader.raiseError("Invalid XML command");
    }
    else // if( xmlStreamTokenType == QXmlStreamReader::StartDocument )
    {
        while( !xmlStreamReader.atEnd() )
        {
            xmlStreamTokenType = xmlStreamReader.readNext();

            if( xmlStreamReader.isStartElement() )
            {
                strElemName = xmlStreamReader.name().toString();

                // For best performance start with the most frequently used element names ..

                if( strElemName == "Server" )
                {
                    if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                    {
                        strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                        enabled = CEnumEnabled::toEnumerator(strAttr);
                        if( enabled == EEnabled::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( xmlStreamReader.attributes().hasAttribute("NewTrcAdminObjsEnabledAsDefault") )
                    {
                        strAttr = xmlStreamReader.attributes().value("NewTrcAdminObjsEnabledAsDefault").toString();
                        setNewTrcAdminObjsEnabledAsDefault( str2Bool(strAttr) );
                    }
                    if( xmlStreamReader.attributes().hasAttribute("NewTrcAdminObjsDefaultDetailLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("NewTrcAdminObjsDefaultDetailLevel").toString();
                        iDetailLevel = strAttr.toInt(&bConverted);
                        if( !bConverted )
                        {
                            xmlStreamReader.raiseError("Attribute \"NewTrcAdminObjsDefaultDetailLevel\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                        else
                        {
                            setNewTrcAdminObjsDefaultDetailLevel(iDetailLevel);
                        }
                    }
                    if( xmlStreamReader.attributes().hasAttribute("CacheDataIfNotConnected") )
                    {
                        strAttr = xmlStreamReader.attributes().value("CacheDataIfNotConnected").toString();
                        setCacheTrcDataIfNotConnected( str2Bool(strAttr) );
                    }
                    if( xmlStreamReader.attributes().hasAttribute("CacheDataMaxArrLen") )
                    {
                        strAttr = xmlStreamReader.attributes().value("CacheDataMaxArrLen").toString();
                        iVal = strAttr.toInt(&bConverted);
                        if( !bConverted )
                        {
                            xmlStreamReader.raiseError("Attribute \"CacheDataMaxArrLen\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                        else
                        {
                            setNewTrcAdminObjsDefaultDetailLevel(iVal);
                        }
                    }
                    if( xmlStreamReader.attributes().hasAttribute("LocalTrcFileCloseAfterEachWrite") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LocalTrcFileCloseAfterEachWrite").toString();
                        setLocalTrcFileCloseFileAfterEachWrite( str2Bool(strAttr) );
                    }
                    if( xmlStreamReader.attributes().hasAttribute("UseLocalTrcFile") )
                    {
                        strAttr = xmlStreamReader.attributes().value("UseLocalTrcFile").toString();
                        setUseLocalTrcFile( str2Bool(strAttr) );
                    }
                } // if( strElemName == "Server" )

                if( strElemName == "TrcAdminObj" )
                {
                    iObjId = -1;
                    enabled = EEnabled::Undefined;
                    iDetailLevel = -1;

                    if( xmlStreamReader.attributes().hasAttribute("ObjId") )
                    {
                        strAttr = xmlStreamReader.attributes().value("ObjId").toString();
                        iObjId = strAttr.toInt(&bConverted);
                        if( !bConverted )
                        {
                            xmlStreamReader.raiseError("Attribute \"ObjId\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                    {
                        strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                        enabled = CEnumEnabled::toEnumerator(strAttr);
                        if( enabled == EEnabled::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( xmlStreamReader.attributes().hasAttribute("DetailLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("DetailLevel").toString();
                        iDetailLevel = strAttr.toInt(&bConverted);
                        if( !bConverted )
                        {
                            xmlStreamReader.raiseError("Attribute \"DetailLevel\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }

                    if( iObjId < 0 || iObjId >= m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                    {
                        xmlStreamReader.raiseError("ObjectId " + QString::number(iObjId) + " for \"" + strElemName + "\" is out of range");
                    }
                    else // if( iObjId >= 0 && iObjId < m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                    {
                        CTrcAdminObj* pTrcAdminObj = m_pTrcAdminObjIdxTree->getTraceAdminObj(iObjId);

                        if( pTrcAdminObj == nullptr )
                        {
                            xmlStreamReader.raiseError("An Object with Id " + QString::number(iObjId) + " for \"" + strElemName + "\" is not existing");
                        }
                        else
                        {
                            pTrcAdminObj->setEnabled(enabled);
                            pTrcAdminObj->setTraceDetailLevel(iDetailLevel);
                        }
                    } // if( iObjId >= 0 && iObjId < m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                } // if( strElemName == "TrcAdminObj" )

                if( strElemName == "NameSpace" )
                {
                    iObjId = -1;
                    enabled = EEnabled::Undefined;
                    iDetailLevel = -1;

                    if( xmlStreamReader.attributes().hasAttribute("ObjId") )
                    {
                        strAttr = xmlStreamReader.attributes().value("ObjId").toString();
                        iObjId = strAttr.toInt(&bConverted);
                        if( !bConverted )
                        {
                            xmlStreamReader.raiseError("Attribute \"ObjId\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                    {
                        strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                        enabled = CEnumEnabled::toEnumerator(strAttr);
                        if( enabled == EEnabled::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( xmlStreamReader.attributes().hasAttribute("DetailLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("DetailLevel").toString();
                        iDetailLevel = strAttr.toInt(&bConverted);
                        if( !bConverted )
                        {
                            xmlStreamReader.raiseError("Attribute \"DetailLevel\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }

                    if( iObjId < 0 || iObjId >= m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                    {
                        xmlStreamReader.raiseError("ObjectId " + QString::number(iObjId) + " for \"" + strElemName + "\" is out of range");
                    }
                    else // if( iObjId >= 0 && iObjId < m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                    {
                        CAbstractIdxTreeEntry* pTreeEntry = m_pTrcAdminObjIdxTree->getEntry(iObjId);

                        if( pTreeEntry == nullptr )
                        {
                            xmlStreamReader.raiseError("An Object with Id " + QString::number(iObjId) + " for \"" + strElemName + "\" is not existing");
                        }
                        else if( pTreeEntry->entryType() != EIdxTreeEntryType::Root
                              && pTreeEntry->entryType() != EIdxTreeEntryType::Branch )
                        {
                            xmlStreamReader.raiseError("The Object with Id " + QString::number(iObjId) + " for \"" + strElemName + "\" is not a name space node");
                        }
                        else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                        {
                            m_pTrcAdminObjIdxTree->setEnabled(iObjId, enabled);
                            m_pTrcAdminObjIdxTree->setTraceDetailLevel(iObjId, iDetailLevel);
                        }
                    } // if( iObjId >= 0 && iObjId < m_pTrcAdminObjIdxTree->treeEntriesVectorSize() )
                } // if( strElemName == "NameSpace" )
            } // if( xmlStreamReader.isStartElement() )
        } // while( !xmlStreamReader.atEnd() )
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
void CIpcTrcServer::onTrcAdminObjIdxTreeEntryAdded(
    CIdxTree*              /*i_pIdxTree*/,
    CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // The trace admin object index tree will be locked so it will not be changed
    // when accessing it here.
    QMutexLocker mutexLocker(m_pTrcAdminObjIdxTree->mutex());

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = QString(i_pTreeEntry == nullptr ? "null" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTrcAdminObjIdxTreeEntryAdded",
        /* strMthInArgs       */ strMthInArgs );

    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

    if( i_pTreeEntry != nullptr )
    {
        if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            sendBranch(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandInsert,
                /* pBranch       */ dynamic_cast<CBranchIdxTreeEntry*>(i_pTreeEntry) );
        }
        else if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            sendAdminObj(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandInsert,
                /* pTrcAdminObj  */ dynamic_cast<CTrcAdminObj*>(i_pTreeEntry) );
        }
    }
} // onTrcAdminObjIdxTreeEntryAdded

//------------------------------------------------------------------------------
void CIpcTrcServer::onTrcAdminObjIdxTreeEntryChanged(
    CIdxTree*              /*i_pIdxTree*/,
    CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // The trace admin object index tree will be locked so it will not be changed
    // when accessing it here.
    QMutexLocker mutexLocker(m_pTrcAdminObjIdxTree->mutex());

    QString strMthInArgs;

    if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = QString(i_pTreeEntry == nullptr ? "null" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTrcAdminObjIdxTreeEntryChanged",
        /* strMthInArgs       */ strMthInArgs );

    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

    if( i_pTreeEntry != nullptr )
    {
        if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Root || i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            sendBranch(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandInsert,
                /* pBranch       */ dynamic_cast<CBranchIdxTreeEntry*>(i_pTreeEntry) );
        }
        else if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            sendAdminObj(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandUpdate,
                /* pTrcAdminObj  */ dynamic_cast<CTrcAdminObj*>(i_pTreeEntry) );
        }
    } // if( i_pTreeEntry != nullptr )

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

        if( m_pTrcMthFile != nullptr && m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            int iAddTrcInfoDetailLevel = 0;
            if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
            else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

            strAddTrcInfo = "Msg {" + QString( pMsg == nullptr ? "nullptr" : pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* iTrcDetailLevel    */ m_iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ nameSpace(),
            /* strClassName       */ className(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "event",
            /* strAddInfo         */ strAddTrcInfo );

        if( pMsg != nullptr )
        {
            // The class (and all instances of the class) may be accessed from within
            // different thread contexts and therefore accessing the class and the
            // instances must be serialized using a mutex ..
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

                //        POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
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
