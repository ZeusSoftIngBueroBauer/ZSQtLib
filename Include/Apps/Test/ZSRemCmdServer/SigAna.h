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

#ifndef ZSApps_TestRemCmdServer_SigAna_h
#define ZSApps_TestRemCmdServer_SigAna_h

#include <QtCore/qstring.h>
#include <QtCore/qthread.h>

#include "ZSSys/ZSSysRequestQueue.h"

class QTimer;

namespace ZS
{
namespace System
{
class CRequest;
struct SErrResultInfo;
}

namespace Trace
{
class CTrcAdminObj;
}

namespace Apps
{
namespace Test
{
namespace RemCmdServer
{
class CDatabase;
class CSigAna;
class CSigGen;

//******************************************************************************
class CSigAnaThread : public QThread
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
    CSigAnaThread( const QString& i_strSigAnaObjName );
    virtual  ~CSigAnaThread();
public: // instance methods
    ZS::System::CRequest* startup( CDatabase* i_pDb, CSigGen* i_pSigGen, qint64 i_iReqIdParent = -1 );
public: // instance methods
    CSigAna* getSigAna() { return m_pSigAna; }
protected: // overridables of base class QThread
    virtual void start(); // use startup instead
    virtual void run();
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
private: // instance members
    QString                   m_strSigAnaObjName;
    CDatabase*                m_pDb;
    CSigGen*                  m_pSigGen;
    EState                    m_state;
    ZS::System::CRequest*     m_pReqInProgress;
    CSigAna*                  m_pSigAna;
    ZS::Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CSigAnaThread

//******************************************************************************
class CSigAna : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CSigAna(
        CSigAnaThread* i_pSigAnaThread,
        CDatabase*     i_pDb,
        CSigGen*       i_pSigGen,
        const QString& i_strObjName );
    virtual  ~CSigAna();
public: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv );
protected slots:
    void onTmrSampleTimeout();
protected slots:
    void onDbDestroyed( QObject* i_pDb );
    void onSigGenDestroyed( QObject* i_pSigGen );
private: // instance members
    CSigAnaThread*           m_pSigAnaThread;
    CDatabase*               m_pDb;
    CSigGen*                 m_pSigGen;
    double                   m_fSamplingRate_Hz;
    double                   m_fYVal;
    QTimer*                  m_pTmrSample;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CSigAna

} // namespace RemCmdServer

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestRemCmdServer_SigAna_h
