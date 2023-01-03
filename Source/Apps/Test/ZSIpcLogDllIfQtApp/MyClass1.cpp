/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include <QtCore/qthread.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h>

#include "MyClass1.h"
#include "MyClass2.h"

#include "ZSIpcLog/ZSIpcLogDllIf.h"

#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysSleeperThread.h"

#include "ZSSys/ZSSysMemLeakDump.h"

#ifdef __linux__
#include <unistd.h>
#endif

using namespace ZS::System;
using namespace ZS::Log::DllIf;
using namespace ZS::Apps::Test::IpcLogDllIfQtApp;


/*******************************************************************************
class CMyClass1 : public QObject
*******************************************************************************/

ZS::Log::DllIf::CLogger* CMyClass1::s_pLogger = nullptr;
int CMyClass1::s_iInstCount = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass1::CMyClass1( const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strMyClass2ObjName(),
    m_pMyClass2Thread(nullptr),
    m_pMyClass2(nullptr)
{
    setObjectName(i_strObjName);

    if( s_iInstCount == 0 )
    {
        QString strLoggerName = NameSpace() + "::" + ClassName();
        s_pLogger = Log::DllIf::CLogServer::GetLogger(strLoggerName.toLatin1().data());
        if( s_pLogger != nullptr )
        {
            s_pLogger->setLogLevel(Log::DllIf::ELogDetailLevelDebug);
            s_pLogger->setAddThreadName(true);
            s_pLogger->setAddDateTime(true);
            s_pLogger->setAddSystemTime(true);
            s_pLogger->setNameSpace(NameSpace().toLatin1().data());
            s_pLogger->setClassName(ClassName().toLatin1().data());
        }
    }

    ++s_iInstCount;

    if( s_pLogger != nullptr )
    {
        s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "ctor");
    }

} // ctor

//------------------------------------------------------------------------------
CMyClass1::~CMyClass1()
//------------------------------------------------------------------------------
{
    if( s_pLogger != nullptr )
    {
        s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "dtor");
    }

    emit aboutToBeDestroyed(this, objectName());

    stopClass2Thread();

    try
    {
        delete m_pMyClass2Thread;
    }
    catch(...)
    {
    }
    m_pMyClass2Thread = nullptr;
    m_pMyClass2 = nullptr;

    --s_iInstCount;

    if( s_iInstCount == 0 )
    {
        s_pLogger = nullptr;
    }

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass2* CMyClass1::startClass2Thread(const QString& i_strMyClass2ObjName)
//------------------------------------------------------------------------------
{
    if( s_pLogger != nullptr )
    {
        s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "startClass2Thread");
    }

    m_strMyClass2ObjName = i_strMyClass2ObjName;

    if( m_pMyClass2Thread == nullptr )
    {
        m_pMyClass2Thread = new CMyClass2Thread(i_strMyClass2ObjName, this);
    }

    if( !m_pMyClass2Thread->isRunning() )
    {
        m_pMyClass2Thread->start();
        m_pMyClass2 = m_pMyClass2Thread->waitForMyClass2Created();
    }

    return m_pMyClass2;

} // startClass2Thread

//------------------------------------------------------------------------------
void CMyClass1::stopClass2Thread()
//------------------------------------------------------------------------------
{
    if( s_pLogger != nullptr )
    {
        s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "stopClass2Thread");
    }

    if( m_pMyClass2Thread != nullptr && m_pMyClass2Thread->isRunning() )
    {
        m_pMyClass2Thread->quit();

        // Let the thread quit the event loop (return from exec) before calling wait
        // to get the same trace output each time.
        CSleeperThread::msleep(50);

        int iTimeout_ms = 30000; // So the value can be modified during debugging session.
        if( !m_pMyClass2Thread->wait(iTimeout_ms) )
        {
            SErrResultInfo errResultInfo = ErrResultInfoError(
                "stopClass2Thread", EResultTimeout, "Waiting for thread to quit timed out");
            CErrLog::GetInstance()->addEntry(errResultInfo);
            m_pMyClass2Thread->terminate();
        }
    }

    delete m_pMyClass2Thread;
    m_pMyClass2Thread = nullptr;
    m_pMyClass2 = nullptr;

} // stopClass2Thread

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass1::sendMuchData()
//------------------------------------------------------------------------------
{
    if( s_pLogger != nullptr )
    {
        s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "sendMuchData");
    }

    CMyClass2* pMyClass2 = startClass2Thread(objectName());
    pMyClass2->sendMuchData();

    QJsonObject jsonObj;

    jsonObj.insert("Hello", "Class1");
    jsonObj.insert("Zahl1", 1.0);
    jsonObj.insert("Text1", "Text1");
    jsonObj.insert("Zahl2", 2.0);
    jsonObj.insert("Text2", "Text2");
    jsonObj.insert("Hello", "Class1");

    double fStartTime_s = System::Time::getProcTimeInSec();
    double fCurrTime_s = System::Time::getProcTimeInSec();
    double fDuration_s = 20.0;

    while ((fCurrTime_s - fStartTime_s) < fDuration_s)
    {
        jsonObj.insert("StartTime_s", fStartTime_s);
        jsonObj.insert("CurrTime_s", fCurrTime_s);
        jsonObj.insert("Duration_s", fDuration_s);

        if( s_pLogger != nullptr )
        {
            QString strLog = "Im sending data now for " + QString::number(fCurrTime_s - fStartTime_s, 'f', 3) + " seconds. ";
            s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, strLog.toLatin1().data());
        }
        fCurrTime_s = System::Time::getProcTimeInSec();
        sendData1(QJsonDocument(jsonObj).toJson(QJsonDocument::Compact), fStartTime_s, fCurrTime_s, fDuration_s);
    }

    stopClass2Thread();
    pMyClass2 = nullptr;
}

/*==============================================================================
private: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass1::sendData1( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
//------------------------------------------------------------------------------
{
    static int s_iCount = 0;

    QString strLog;

    if( s_pLogger != nullptr )
    {
        strLog = "sendData1(" + i_strData;
        strLog += "Data: " + i_strData;
        strLog += ", StartTime: " + QString::number(i_fStartTime_s) + "s";
        strLog += ", CurrTime: " + QString::number(i_fStartTime_s) + "s";
        strLog += ", Duration: " + QString::number(i_fStartTime_s) + "s";
        strLog += ")";
        s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, strLog.toLatin1().data());
    }

    ++s_iCount;

    sendData2(i_strData, i_fStartTime_s, i_fCurrTime_s, i_fDuration_s);

    if( s_pLogger != nullptr )
    {
        strLog = "Time remaining: " + QString::number(i_fDuration_s - (i_fCurrTime_s - i_fStartTime_s), 'f', 3) + " seconds.";
        s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, strLog.toLatin1().data());
    }
    return s_iCount;
}

//------------------------------------------------------------------------------
int CMyClass1::sendData2( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
//------------------------------------------------------------------------------
{
    static int s_iCount = 0;

    QString strLog;

    if( s_pLogger != nullptr )
    {
        strLog = "sendData2(" + i_strData;
        strLog += "Data: " + i_strData;
        strLog += ", StartTime: " + QString::number(i_fStartTime_s) + "s";
        strLog += ", CurrTime: " + QString::number(i_fStartTime_s) + "s";
        strLog += ", Duration: " + QString::number(i_fStartTime_s) + "s";
        strLog += ")";
        s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, strLog.toLatin1().data());
    }

    ++s_iCount;

    sendData3(i_strData, i_fStartTime_s, i_fCurrTime_s, i_fDuration_s);

    if( s_pLogger != nullptr )
    {
        strLog = "Time remaining: " + QString::number(i_fDuration_s - (i_fCurrTime_s - i_fStartTime_s), 'f', 3) + " seconds.";
        s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, strLog.toLatin1().data());
    }
    return s_iCount;
}

//------------------------------------------------------------------------------
int CMyClass1::sendData3( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
//------------------------------------------------------------------------------
{
    static int s_iCount = 0;

    QString strLog;

    if( s_pLogger != nullptr )
    {
        strLog = "sendData3(" + i_strData;
        strLog += "Data: " + i_strData;
        strLog += ", StartTime: " + QString::number(i_fStartTime_s) + "s";
        strLog += ", CurrTime: " + QString::number(i_fStartTime_s) + "s";
        strLog += ", Duration: " + QString::number(i_fStartTime_s) + "s";
        strLog += ")";
        s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, strLog.toLatin1().data());
    }

    ++s_iCount;

    if( s_pLogger != nullptr )
    {
        strLog = "Time remaining: " + QString::number(i_fDuration_s - (i_fCurrTime_s - i_fStartTime_s), 'f', 3) + " seconds.";
        s_pLogger->log(Log::DllIf::ELogDetailLevelDebug, strLog.toLatin1().data());
    }
    return s_iCount;
}
