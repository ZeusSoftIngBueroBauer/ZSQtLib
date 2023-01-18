/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include "MyThread.h"

#include "ZSSys/ZSSysErrLog.h"

#include <QtCore/qwaitcondition.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Log;
using namespace ZS::Apps::Test::IpcLogDllIfQtApp;


/*******************************************************************************
class CMyThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMyThread::CMyThread(
    const QString&         i_strLoggerName,
    DllIf::ELogDetailLevel i_logLevel,
    const QString&         i_strLogEntry ) :
//------------------------------------------------------------------------------
    QThread(),
    m_strLoggerName(i_strLoggerName),
    m_logLevel(i_logLevel),
    m_strLogEntry(i_strLogEntry),
    m_pMtxWaitWorkDone(nullptr),
    m_pWaitConditionWorkDone(nullptr),
    m_bWorkDone(false)
{
    setObjectName("MyThread");

    m_pMtxWaitWorkDone = new QMutex();
    m_pWaitConditionWorkDone = new QWaitCondition();

} // ctor

//------------------------------------------------------------------------------
CMyThread::~CMyThread()
//------------------------------------------------------------------------------
{
    if( isRunning() )
    {
        quit();

        if( !wait(30000) )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                SErrResultInfo errResultInfo(
                    NameSpace(), ClassName(), objectName(), "dtor",
                    EResultTimeout, EResultSeverityError,
                    "Waiting for thread to quit timed out");
                CErrLog::GetInstance()->addEntry(errResultInfo);
             }
        }
    }

    try
    {
        delete m_pMtxWaitWorkDone;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pWaitConditionWorkDone;
    }
    catch(...)
    {
    }

    //m_strLoggerName;
    m_logLevel = static_cast<DllIf::ELogDetailLevel>(0);
    //m_strLogEntry;
    m_pMtxWaitWorkDone = nullptr;
    m_pWaitConditionWorkDone = nullptr;
    m_bWorkDone = false;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CMyThread::waitForWorkDone()
//------------------------------------------------------------------------------
{
    if( m_pMtxWaitWorkDone->tryLock() )
    {
        while( !m_bWorkDone )
        {
            m_pWaitConditionWorkDone->wait(m_pMtxWaitWorkDone, 100);
        }
        m_pMtxWaitWorkDone->unlock();
    }
    return m_bWorkDone;
}

/*==============================================================================
public: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CMyThread::run()
//------------------------------------------------------------------------------
{
    DllIf::CLogger* pLogger = DllIf::CLogServer::GetLogger(m_strLoggerName.toStdString().c_str());

    pLogger->log(m_logLevel, m_strLogEntry.toStdString().c_str());

    m_bWorkDone = true;
}
