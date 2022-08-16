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

#ifndef ZSApps_TestIpcLogDllIfQtApp_MyThread_h
#define ZSApps_TestIpcLogDllIfQtApp_MyThread_h

#include "ZSSys/ZSSysCommon.h"

#include <QtCore/qthread.h>

#include "ZSIpcLog/ZSIpcLogDllIf.h"

class QMutex;
class QWaitCondition;

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace IpcLogDllIfQtApp
{
//******************************************************************************
class CMyThread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcLog"; }
    static QString ClassName() { return "CMyThread"; }
public: // ctors and dtor
    CMyThread(
        const QString&                  i_strLoggerName,
        ZS::Log::DllIf::ELogDetailLevel i_logLevel,
        const QString&                  i_strLogEntry );
    virtual ~CMyThread();
public: // instance methods
    bool waitForWorkDone();
public: // overridables of base class QThread
    void run();
private: // instance members
    QString                         m_strLoggerName;
    ZS::Log::DllIf::ELogDetailLevel m_logLevel;
    QString                         m_strLogEntry;
    QMutex*                         m_pMtxWaitWorkDone;
    QWaitCondition*                 m_pWaitConditionWorkDone;
    bool                            m_bWorkDone;

}; // class CMyThread

} // namespace IpcLogDllIfQtApp

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcLogDllIfQtApp_MyThread_h
