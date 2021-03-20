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

#ifndef ZSApps_TestAsyncRequests_DatabaseThread_h
#define ZSApps_TestAsyncRequests_DatabaseThread_h

#include <QtCore/qstring.h>
#include <QtCore/qthread.h>

#include "Database.h"

#include "ZSSys/ZSSysRequestQueue.h"
#include "ZSSys/ZSSysRequestSequencer.h"

class QMutex;

namespace ZS
{
namespace System
{
class CRequest;
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
//******************************************************************************
class CDbThread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::AsyncRequests::Database"; }
    static QString ClassName() { return "CDbThread"; }
public: // ctors and dtor
    CDbThread( const QString& i_strObjName );
    virtual ~CDbThread();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    void stateChanged( int i_iStateCurr, int i_iStatePrev );
signals:
    void dbCreated( QObject* i_pDb );
public: // instance methods
    CDb* getDb() { return m_pDb; }
    Database::EState state() const { return m_state; }
public: // instance methods
    void setReqModeSingleShot( bool i_bModeSingleShot );
    bool isReqModeSingleShot() const { return m_bReqModeSingleShot; }
public: // instance methods
    ZS::System::CRequest* startup( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
    ZS::System::CRequest* shutdown( int i_iTimeout_ms = 0, bool i_bWait = false, qint64 i_iReqIdParent = -1 );
protected: // overridables (state machine)
    virtual void executeNextPostponedRequest();
protected: // instance methods
    void executeStartupRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
    void executeShutdownRequest( ZS::System::CRequest* i_pReq, ZS::System::CRequest* i_pReqMainInProgress = nullptr );
protected slots:
    void onRequestTimeout();
protected slots:
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
protected: // overridables of base class QThread
    virtual void start(); // use startup instead
    virtual void run();
private: // instance members
    CDb*                      m_pDb;
    Database::EState          m_state;           // Range [Created, Running]
    ZS::System::CRequestQueue m_requestQueue;
    ZS::System::SRequestDscr  m_reqDscrTimeout;
    QTimer*                   m_pTmrReqTimeout;
    bool                      m_bMsgReqContinuePending;
    qint64                    m_iReqIdStartThread;
    bool                      m_bReqModeSingleShot;
    ZS::Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CDbThread

} // namespace AsyncRequests

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestAsyncRequests_DatabaseThread_h
