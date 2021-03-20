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

#ifndef ZSApps_TestRemCmdServer_Database_h
#define ZSApps_TestRemCmdServer_Database_h

#include <QtCore/qstring.h>
#include <QtCore/qthread.h>

#include "SigGen.h"

class QMutex;

namespace ZS
{
namespace System
{
class CRequest;
}
namespace Ipc
{
struct SSocketDscr;
}
namespace Trace
{
class CTrcAdminObj;
}

namespace RemCmd
{
class CRemCmdObj;
class CRemCmdServer;
}

namespace Apps
{
namespace Test
{
namespace RemCmdServer
{
class CDatabase;

/*******************************************************************************
Type definitions and constants
*******************************************************************************/

// Signal generator:
const QString c_strDbParSigGenWaveform     = "SigGenWaveform";
const QString c_strDbParSigGenYMin         = "SigGenYMin";
const QString c_strDbParSigGenYMax         = "SigGenYMax";
const QString c_strDbParSigGenFreq         = "SigGenFreq";

// Signal analyzer:
const QString c_strDbParSigAnaSamplingRate = "SigAnaSamplingRate";
const QString c_strDbParSigAnaYVal         = "SigAnaYVal";

//******************************************************************************
class CDatabaseThread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum {
        EStateIdle     = 0,
        EStateStarting = 1,
        EStateRunning  = 2,
        EStateCount
    }   EState;
    static QString State2Str( int i_iState );
    typedef enum {
        ERequestNone     = 0,
        ERequestStartup  = 1,
        ERequestShutdown = 2,
        ERequestCount,
        ERequestUndefined
    }   ERequest;
    static QString Request2Str( int i_iRequest );
public: // ctors and dtor
    CDatabaseThread( const QString& i_strDbObjName );
    virtual  ~CDatabaseThread();
public: // instance methods
    ZS::System::CRequest* startup( ZS::RemCmd::CRemCmdServer* i_pRemCmdServer, qint64 i_iReqIdParent = -1 );
public: // instance methods
    CDatabase* getDb() { return m_pDb; }
protected: // overridables of base class QThread
    virtual void start(); // use startup instead
    virtual void run();
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
private: // instance members
    QString                    m_strDbObjName;
    EState                     m_state;
    ZS::System::CRequest*      m_pReqInProgress;
    CDatabase*                 m_pDb;
    ZS::RemCmd::CRemCmdServer* m_pRemCmdServer;
    ZS::Trace::CTrcAdminObj*   m_pTrcAdminObj;

}; // class CDatabaseThread

//******************************************************************************
class CDatabase : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods (Remote Commands, Signal Generator)
    static ZS::System::CRequest* OnExecuteRemCmdSourFunShap(
        QObject*                    i_pObjFctExecute,
        const ZS::Ipc::SSocketDscr& i_socketDscr,
        bool                        i_bIsQuery,
        const QByteArray&           i_byteArrArgs,
        QByteArray&                 o_byteArrResults,
        qint64                      i_iReqIdParent );
    static ZS::System::CRequest* OnExecuteRemCmdSourValMax(
        QObject*                    i_pObjFctExecute,
        const ZS::Ipc::SSocketDscr& i_socketDscr,
        bool                        i_bIsQuery,
        const QByteArray&           i_byteArrArgs,
        QByteArray&                 o_byteArrResults,
        qint64                      i_iReqIdParent );
    static ZS::System::CRequest* OnExecuteRemCmdSourValMin(
        QObject*                    i_pObjFctExecute,
        const ZS::Ipc::SSocketDscr& i_socketDscr,
        bool                        i_bIsQuery,
        const QByteArray&           i_byteArrArgs,
        QByteArray&                 o_byteArrResults,
        qint64                      i_iReqIdParent );
public: // class methods (Remote Commands, Signal Analyzer)
    static ZS::System::CRequest* OnExecuteRemCmdRead(
        QObject*                    i_pObjFctExecute,
        const ZS::Ipc::SSocketDscr& i_socketDscr,
        bool                        i_bIsQuery,
        const QByteArray&           i_byteArrArgs,
        QByteArray&                 o_byteArrResults,
        qint64                      i_iReqIdParent );
public: // ctors and dtor
    CDatabase(
        CDatabaseThread*           i_pDbThread,
        ZS::RemCmd::CRemCmdServer* i_pRemCmdServer,
        const QString&             i_strDbObjName );
    virtual  ~CDatabase();
public: // instance methods
    void registerSigGenClient( QObject* i_pObjClient );
    void unregisterSigGenClient( QObject* i_pObjClient );
public: // instance methods
    void registerSigAnaClient( QObject* i_pObjClient );
    void unregisterSigAnaClient( QObject* i_pObjClient );
public: // instance methods( Signal Generator)
    ESigGenWaveform getSigGenWaveform() const { return m_sigGenWaveform; }
    void setSigGenWaveform( ESigGenWaveform i_sigGenWaveform );
    double getSigGenYMin() const { return m_fSigGenYMin; }
    void setSigGenYMin( double i_fVal );
    double getSigGenYMax() const { return m_fSigGenYMax; }
    void setSigGenYMax( double i_fVal );
    double getSigGenFreqInHz() const { return m_fSigGenFreq_Hz; }
    void setSigGenFreqInHz( double i_fVal );
public: // instance methods( Signal Analyzer)
    double getSigAnySamplingRateInHz() const { return m_fSigAnaSamplingRate_Hz; }
    void setSigAnaSamplingRateInHz( double i_fVal );
    double getSigAnaYVal() const { return m_fSigAnaYVal; }
    void setSigAnaYVal( double i_fVal );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
private: // instance members
    CDatabaseThread*           m_pDbThread;
    ZS::RemCmd::CRemCmdServer* m_pRemCmdServer;
    // Signal Generator
    QMutex*                    m_pMtxSigGenClients;
    QList<QObject*>            m_arpObjSigGenClients; // List of clients to be informed if SigGen parameter values will be changed (the SigGen itself is treated as a database client).
    ESigGenWaveform            m_sigGenWaveform;
    double                     m_fSigGenYMin;
    double                     m_fSigGenYMax;
    double                     m_fSigGenFreq_Hz;
    // Signal Analyzer
    QMutex*                    m_pMtxSigAnaClients;
    QList<QObject*>            m_arpObjSigAnaClients; // List of clients to be informed if SigAna parameter values will be changed (the SigAna itself is treated as a database client)..
    double                     m_fSigAnaSamplingRate_Hz;
    double                     m_fSigAnaYVal;
    // Remote Commands (Signal Generator)
    ZS::RemCmd::CRemCmdObj*    m_pRemCmdObjSourFunShap; // sets waveform type
    ZS::RemCmd::CRemCmdObj*    m_pRemCmdObjSourValMax;
    ZS::RemCmd::CRemCmdObj*    m_pRemCmdObjSourValMin;
    // Remote Commands (Signal Analyzer)
    ZS::RemCmd::CRemCmdObj*    m_pRemCmdObjRead;        // reads current value
    // Tracing
    ZS::Trace::CTrcAdminObj*   m_pTrcAdminObj;

}; // class CDatabase

} // namespace RemCmdServer

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestRemCmdServer_Database_h
