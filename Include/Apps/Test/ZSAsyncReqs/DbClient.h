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

#ifndef ZSApps_TestAsyncRequests_DbClient_h
#define ZSApps_TestAsyncRequests_DbClient_h

#include <QtCore/qstring.h>
#include <QtCore/qthread.h>

#include "Database.h"

#include "ZSSys/ZSSysRequestQueue.h"

class QMutex;
class QTimer;

namespace ZS
{
namespace System
{
class CRequest;
class CRequestSequencer;
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
class CDbClientGatewayThread;
class CDbClientGateway;

//******************************************************************************
class CDbClient : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::AsyncRequests"; }
    static QString ClassName() { return "CDbClient"; }
public: // class methods (request sequencer execution functions)
    static ZS::System::CRequest* startGatewayThread( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* stopGatewayThread( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* dbConnect( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* dbDisconnect( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* register_( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* unregister( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* selectSchema( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
    static ZS::System::CRequest* selectData( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );
public: // ctors and dtor
    CDbClient( const QString& i_strDbObjName );
    virtual  ~CDbClient();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    void stateChanged( int i_iStateCurr, int i_iStatePrev );
public: // instance methods (changing connection settings, the request method "changeSettings" need to be called afterwards)
    void setSocketType( Ipc::ESocketType i_socketType );
    Ipc::ESocketType getSocketType() const { return m_socketTypeCurr; }
public: // instance methods (changing connection settings, the request method "changeSettings" need to be called afterwards)
    void setDb( CDb* i_pDb );
    CDb* getDb() { return m_pDb; }
public: // instance methods
    Database::EState state() const { return m_state; }
public: // instance methods
    ZS::System::CRequest* changeSettings( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
public: // instance methods
    ZS::System::CRequest* connect_( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );    // underscore appended to distinguish from QObject::connect (important to create python bindings)
    ZS::System::CRequest* disconnect_( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 ); // underscore appended to distinguish from QObject::disconnect(important to create python bindings)
public: // instance methods
    ZS::System::CRequest* register_( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
    ZS::System::CRequest* unregister( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
public: // instance methods
    ZS::System::CRequest* selectSchema( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
    ZS::System::CRequest* updateSchema( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
public: // instance methods
    ZS::System::CRequest* selectData( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
    ZS::System::CRequest* updateData( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
protected: // instance methods
    ZS::System::CRequest* onReceivedMsgReqUpdateSchema( CMsgReqUpdateSchema* i_pMsgReq );
protected: // instance methods
    ZS::System::CRequest* onReceivedMsgReqUpdateData( CMsgReqUpdateData* i_pMsgReq );
protected: // overridables (state machine)
    virtual void executeNextPostponedRequest();
protected: // instance methods
    void executeChangeSettingsRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // instance methods
    void executeConnectRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeDisconnectRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // instance methods
    void executeRegisterRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeUnregisterRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // instance methods
    void executeSelectSchemaRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeUpdateSchemaRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // instance methods
    void executeSelectDataRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeUpdateDataRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected: // internal request sequencer methods
    ZS::System::CRequest* startGatewayThread( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent );
    ZS::System::CRequest* stopGatewayThread( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent );
protected: // internal request sequencer methods
    ZS::System::CRequest* dbConnect( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent );
    ZS::System::CRequest* dbDisconnect( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent );
protected slots:
    void onRequestTimeout();
protected slots:
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onDbDestroyed( QObject* i_pDb );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
private: // instance members
    Ipc::ESocketType               m_socketTypeCurr;
    Ipc::ESocketType               m_socketTypeNew;
    // Connection settings (depending on connection type):
    CDb*                           m_pDb;
    Ipc::SClientHostSettings       m_hostSettings;
    CDbClientGatewayThread*        m_pDbCltGtwThread;
    CDbClientGateway*              m_pDbCltGtw;
    // Requests from the client itself:
    ZS::System::CRequestSequencer* m_pReqSeqrConnect;
    ZS::System::CRequestSequencer* m_pReqSeqrDisconnect;
    ZS::System::CRequestSequencer* m_pReqSeqrChangeSettings;
    ZS::System::CRequestQueue      m_requestQueue;
    ZS::System::SRequestDscr       m_reqDscrTimeout;
    QTimer*                        m_pTmrReqTimeout;
    bool                           m_bMsgReqContinuePending;
    // Requests received by the database:
    qint64                         m_iReqMsgIdDb;     // = pMsgReq->getMsgId()
    qint64                         m_iReqIdDb;        // = pMsgReq->getRequestId()
    ZS::System::CRequest*          m_pReqDb;          // Request initiated by request message received from database.
    // Admin:
    Database::EState               m_state;           // Range [Disconnected, Connected]
    ZS::Trace::CTrcAdminObj*       m_pTrcAdminObj;

}; // class CDbClient

} // namespace AsyncRequests

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestAsyncRequests_DbClient_h
