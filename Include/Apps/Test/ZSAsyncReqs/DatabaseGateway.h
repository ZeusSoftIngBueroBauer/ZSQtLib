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

#ifndef ZSApps_TestAsyncRequests_DatabaseGateway_h
#define ZSApps_TestAsyncRequests_DatabaseGateway_h

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
class CIpcServerWrapper;
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
class CDbGateway;

//******************************************************************************
class CDbGatewayThread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::AsyncRequests::Database"; }
    static QString ClassName() { return "CDbGatewayThread"; }
public: // ctors and dtor
    CDbGatewayThread(
        const QString&   i_strObjName,
        Ipc::ESocketType i_socketType,
        CDb*             i_pDb );
    virtual ~CDbGatewayThread();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    Ipc::ESocketType getSocketType() { return m_socketType; }
    CDb* getDb() { return m_pDb; }
    CDbGateway* getGateway() { return m_pDbGtw; }
protected: // must overridables (overridables of base class QThread)
    virtual void start(); // use start method with wait condition argument instead
public: // instance methods
    virtual void start( QWaitCondition* i_pWaitCond );
public: // overridables of base class QThread
    virtual void run();
protected: // instance members
    CDb*                 m_pDb;
    Ipc::ESocketType     m_socketType;
    QWaitCondition*      m_pWaitCond;
    CDbGateway*          m_pDbGtw;
    Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDbGatewayThread

//******************************************************************************
class CDbGateway : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::AsyncRequests::Database"; }
    static QString ClassName() { return "CDbGateway"; }
public: // ctors and dtor
    CDbGateway( const QString& i_strObjName, Ipc::ESocketType i_socketType, CDb* i_pDb );
    virtual ~CDbGateway();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
protected slots:
    void onNewConnectionPending( QObject* i_pServerWrapper );
    void onDisconnected( QObject* i_pSocketWrapper );
    void onReadyRead( QObject* i_pSocketWrapper );
    void onError( QObject* i_pSocketWrapper, ZS::System::SErrResultInfo& i_errResultInfo );
protected: // instance methods
    QByteArray receiveDataBlock_L( ZS::Ipc::CIpcSocketWrapper* i_pSocketWrapper );
protected: // instance methods
    ZS::System::SErrResultInfo writeDataBlock_L( ZS::Ipc::CIpcSocketWrapper* i_pSocketWrapper, const QByteArray& i_byteArr );
public: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pMsg );
protected: // instance members
    CDb*                               m_pDb;
    Ipc::SServerHostSettings           m_hostSettings;
    ZS::Ipc::CIpcServerWrapper*        m_pIpcServerWrapper;
    QHash<int,Ipc::CIpcSocketWrapper*> m_hshpIpcSocketWrappers;
    QList<QObject*>                    m_arpTrcMsgLogObjects;
    Trace::CTrcAdminObj*               m_pTrcAdminObj;

}; // class CDbGateway

} // namespace AsyncRequests

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestAsyncRequests_DatabaseGateway_h
