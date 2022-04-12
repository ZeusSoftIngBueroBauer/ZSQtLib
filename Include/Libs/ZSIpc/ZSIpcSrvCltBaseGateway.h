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

#ifndef ZSIpc_SrvCltBaseGateway_h
#define ZSIpc_SrvCltBaseGateway_h

#include <QtCore/qobject.h>
#include <QtCore/qthread.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcServerWrapper.h"
#include "ZSIpc/ZSIpcSocketWrapper.h"

class QTimer;
class QWaitCondition;

namespace ZS
{
namespace System
{
class CErrLog;
class CMsgCon;
}
namespace Trace
{
class CTrcAdminObj;
class CTrcMthFile;
}

namespace Ipc
{
class CBlkType;

//******************************************************************************
class ZSIPCDLL_API CSrvCltBaseGatewayThread : public QThread
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CSrvCltBaseGatewayThread"; }
public: // ctors and dtor
    CSrvCltBaseGatewayThread(
        const QString&       i_strObjNameGateway,
        ZS::System::CErrLog* i_pErrLog,
        int                  i_iTrcMthFileDetailLevel = ZS::Trace::ETraceDetailLevelNone );
    virtual ~CSrvCltBaseGatewayThread();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    QObject* getGateway() { return m_pGateway; }
public: // instance methods
    void msleep( unsigned long i_uTime_ms );
public: // overridables
    virtual void start( qint64 i_iReqIdStartThread );
    virtual void start( QWaitCondition* i_pWaitCondition );
protected: // hiding public method of base class QThread
     // Use start with request id instead (see comment at the template run method of this class).
     void start();
protected: // overridables of base class QThread
    //virtual void run(); // !! Template which MUST be overwritten !!
protected: // instance methods
    bool isMethodTraceActive( int i_iFilterDetailLevel ) const;
    int getMethodTraceDetailLevel() const;
protected: // instance members
    QString              m_strObjNameGateway;
    QObject*             m_pGateway;
    ZS::System::CErrLog* m_pErrLog;
    qint64               m_iReqIdStartThread;
    QWaitCondition*      m_pWaitCondition;
    int                  m_iTrcMthFileDetailLevel;
    Trace::CTrcMthFile*  m_pTrcMthFile;   // Either trace method file with detail level is used or
    Trace::CTrcAdminObj* m_pTrcAdminObj;  // trace admin object with IpcTrace server.

}; // class CSrvCltBaseGatewayThread

//******************************************************************************
class ZSIPCDLL_API CSrvCltBaseGateway : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CSrvCltBaseGateway"; }
public: // type definitions and constants
    enum ERequest {
        ERequestNone           = 0,
        ERequestStartup        = 1,
        ERequestShutdown       = 2,
        ERequestConnect        = 3,
        ERequestDisconnect     = 4,
        ERequestChangeSettings = 5,
        ERequestReceiveData    = 6,
        ERequestSendData       = 7,
        ERequestCount,
        ERequestUndefined
    };
    static QString request2Str( int i_iRequest );
protected: // ctor
    CSrvCltBaseGateway(
        const QString&            i_strObjName,
        ESrvCltType               i_srvCltType,
        QObject*                  i_pCltSrv,
        CSrvCltBaseGatewayThread* i_pThreadGateway,
        ZS::System::CErrLog*      i_pErrLog,
        int                       i_iTrcMthFileDetailLevel = ZS::Trace::ETraceDetailLevelNone );
public: // dtor
    virtual ~CSrvCltBaseGateway();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
protected slots: // must overridables
    virtual void onDisconnected( QObject* i_pSocketWrapper ) = 0;
    virtual void onReadyRead( QObject* i_pSocketWrapper ) = 0;
    virtual void onError( QObject* i_pSocket, ZS::System::SErrResultInfo& i_errResultInfo ) = 0;
public: // instance methods
    void addTrcMsgLogObject( QObject* i_pObj );
    void removeTrcMsgLogObject( QObject* i_pObj );
protected: // instance methods
    bool isMethodTraceActive( int i_iFilterDetailLevel ) const;
    int getMethodTraceDetailLevel() const;
protected: // instance members
    ZS::System::CErrLog*      m_pErrLog;
    ESrvCltType               m_srvCltType;
    QObject*                  m_pSrvClt;
    CSrvCltBaseGatewayThread* m_pGatewayThread;
    CBlkType*                 m_pBlkType;
    QByteArray*               m_pByteArrWatchDog;
    ERequest                  m_requestInProgress;
    qint64                    m_iReqIdInProgress;
    qint64                    m_iMsgIdReqInProgress;
    ZS::System::CMsgCon*      m_pMsgCon;
    QList<QObject*>           m_arpTrcMsgLogObjects;
    int                       m_iTrcMthFileDetailLevel;
    Trace::CTrcMthFile*       m_pTrcMthFile;   // Either trace method file with detail level is used or
    Trace::CTrcAdminObj*      m_pTrcAdminObj;  // trace admin object with IpcTrace server.

}; // class CSrvCltBaseGateway

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_SrvCltBaseGateway_h
