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

#ifndef ZSApps_TestAsyncRequests_DbClientGateway_h
#define ZSApps_TestAsyncRequests_DbClientGateway_h

#include <QtCore/qstring.h>
#include <QtCore/qthread.h>

#include "Database.h"

class QWaitCondition;

namespace ZS
{
namespace System
{
class CRequest;
}

namespace Ipc
{
class CIpcSocketWrapper;
}

namespace Trace
{
class CTrcAdminObj;
}

namespace Apps
{
namespace Test
{
namespace AsyncRequests
{
class CDb;
class CDbClient;
class CDbClientGateway;

//******************************************************************************
class CDbClientGatewayThread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::AsyncRequests"; }
    static QString ClassName() { return "CDbClientGatewayThread"; }
public: // ctors and dtor
    CDbClientGatewayThread(
        const QString& i_strObjName,
        CDbClient*     i_pDbClient );
    virtual ~CDbClientGatewayThread();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    CDbClient* getClient() { return m_pDbClt; }
    CDbClientGateway* getGateway() { return m_pDbCltGtw; }
protected: // must overridables (overridables of base class QThread)
    virtual void start(); // use start method with wait condition argument instead
public: // instance methods
    virtual void start( QWaitCondition* i_pWaitCond );
public: // overridables of base class QThread
    virtual void run();
protected: // instance members
    CDbClient*           m_pDbClt;
    QWaitCondition*      m_pWaitCond;
    CDbClientGateway*    m_pDbCltGtw;
    Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDbClientGatewayThread

//******************************************************************************
class CDbClientGateway : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::AsyncRequests"; }
    static QString ClassName() { return "CDbClientGateway"; }
public: // ctors and dtor
    CDbClientGateway( const QString& i_strObjName, CDbClient* i_pDbClt );
    virtual ~CDbClientGateway();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
protected slots: // overridables
    virtual void onConnected( QObject* i_pSocketWrapper );
    virtual void onDisconnected( QObject* i_pSocketWrapper );
    virtual void onReadyRead( QObject* i_pSocketWrapper );
    virtual void onError( QObject* i_pSocketWrapper, ZS::System::SErrResultInfo& i_errResultInfo );
protected slots: // overridables
    virtual void onRequestTimeout();
    virtual void onWatchDogTimeout();
protected: // instance methods
    QByteArray receiveDataBlock_L( Ipc::CIpcSocketWrapper* i_pSocketWrapper );
protected: // overridables
    ZS::System::SErrResultInfo writeDataBlock_L( ZS::Ipc::CIpcSocketWrapper* i_pSocketWrapper, const QByteArray& i_byteArr );
public: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pMsg );
protected: // instance members
    // Request initiated by request message received from the gateway's client:
    CDbClient*                          m_pDbClt;
    Ipc::SSocketDscr                    m_socketDscr;
    Database::EState                    m_state;            // Range [Disconnected, Connected]
    qint64                              m_iReqMsgIdDbClt;   // pMsgReqConnect/Disconnect->getMsgId()
    qint64                              m_iReqIdDbClt;      // pMsgReqConnect/Disconnect->getRequestId()
    Database::ERequest                  m_reqDbClt;         // Range [Connect, Disconnect]
    QTimer*                             m_pTmrReqTimeout;
    QHash<qint64,ZS::System::CRequest*> m_hshReqsDb;        // Sending requests to the database.
    ZS::Ipc::STimerSettings             m_watchDogSettings;
    QTimer*                             m_pTmrWatchDog;
    QByteArray*                         m_pByteArrWatchDog;
    // Admin:
    ZS::Ipc::CIpcSocketWrapper*         m_pIpcSocketWrapper;
    QList<QObject*>                     m_arpTrcMsgLogObjects;
    Trace::CTrcAdminObj*                m_pTrcAdminObj;

}; // class CDbClientGateway

} // namespace AsyncRequests

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestAsyncRequests_DbClientGateway_h
