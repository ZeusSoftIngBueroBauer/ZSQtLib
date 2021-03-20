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

#ifndef ZSApps_TestRemCmdServer_SigGen_h
#define ZSApps_TestRemCmdServer_SigGen_h

#include <QtCore/qstring.h>
#include <QtCore/qthread.h>

#include "ZSSys/ZSSysRequestQueue.h"

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
namespace RemCmdServer
{
class CDatabase;
class CSigGen;

//==============================================================================
enum ESigGenWaveform
//==============================================================================
{
    ESigGenWaveformSine     = 0, // f(t) = A * sin(2*PI*t*f) + D; A = yMax-yMin; D = yMin + (yMax-yMin)/2; t = Time/s; f = Freq/Hz = 1/T; T = Period/s
    //ESigGenWaveformSquare   = 1, // f(t) = 
    //ESigGenWaveformTriangle = 2, // f(t) = 
    //ESigGenWaveformSawtooth = 3, // f(t) = 
    ESigGenWaveformCount,
    ESigGenWaveformUndefined
};

QString sigGenWaveform2Str( int i_iWaveform );
ESigGenWaveform str2SigGenWaveform( const QString& i_strWaveform );

QByteArray sigGenWaveform2ByteArr( int i_iWaveform );
ESigGenWaveform byteArr2SigGenWaveform( const QByteArray& i_strWaveform );

//******************************************************************************
class CSigGenThread : public QThread
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
    CSigGenThread( const QString& i_strSigGenObjName );
    virtual  ~CSigGenThread();
public: // instance methods
    ZS::System::CRequest* startup( CDatabase* i_pDb, qint64 i_iReqIdParent = -1 );
public: // instance methods
    CSigGen* getSigGen() { return m_pSigGen; }
protected: // overridables of base class QThread
    virtual void start(); // use startup instead
    virtual void run();
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
private: // instance members
    QString                   m_strSigGenObjName;
    CDatabase*                m_pDb;
    EState                    m_state;
    ZS::System::CRequest*     m_pReqInProgress;
    CSigGen*                  m_pSigGen;
    ZS::Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CSigGenThread

//******************************************************************************
class CSigGen : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CSigGen(
        CSigGenThread* i_pSigGenThread,
        CDatabase*     i_pDb,
        const QString& i_strObjName );
    virtual  ~CSigGen();
public: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv );
protected slots:
    void onDbDestroyed( QObject* i_pDb );
private: // instance members
    CSigGenThread*           m_pSigGenThread;
    CDatabase*               m_pDb;
    ESigGenWaveform          m_waveform;
    double                   m_fYMin;
    double                   m_fYMax;
    double                   m_fFreq_Hz;
    double                   m_fStartTime_s;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CSigGen

} // namespace RemCmdServer

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestRemCmdServer_SigGen_h
