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

#ifndef ZSApps_TestIpcTrace_MyClass2_h
#define ZSApps_TestIpcTrace_MyClass2_h

#include "ZSSys/ZSSysTrcAdminObj.h"

#include <QtCore/qdeadlinetimer.h>
#include <QtCore/qthread.h>

class QTimer;

namespace ZS
{
namespace System
{
class CMutex;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
class CRecursiveMutex;
#endif
class CWaitCondition;
class CTrcAdminObj;
}

namespace Apps
{
namespace Test
{
namespace IpcTrace
{
class CMyClass1;
class CMyClass2;
class CMyClass3;
class CMyClass3Thread;

//******************************************************************************
class CMyClass2Thread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
    static QString ClassName() { return "CMyClass2Thread"; }
public: // ctors and dtor
    CMyClass2Thread( const QString& i_strMyClass2ObjName, CMyClass1* i_pMyClass1 = nullptr );
    virtual ~CMyClass2Thread();
signals:
    void aboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
public: // instance methods (reimplementing methods of base class QObject)
    void setObjectName(const QString& i_strObjName);
public: // instance methods
    CMyClass2* waitForMyClass2Created();
    CMyClass2* getMyClass2() { return m_pMyClass2; }
public: // instance methods
    void sleep( unsigned long i_uTime_s );
public: // overridables of base class QThread
    void run();
public: // replacing methods of base class QThread
    void start( QThread::Priority i_priority = InheritPriority );
    void quit();
    #if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    bool wait( QDeadlineTimer i_deadline = QDeadlineTimer(QDeadlineTimer::Forever) );
    #endif
    bool wait( unsigned long i_time_ms );
protected: // replacing methods of base class QThread
    int exec();
protected slots:
    void onClass2AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
private: // instance members
    ZS::System::CMutex* m_pMtxWaitForClass2Created;
    ZS::System::CWaitCondition* m_pWaitConditionClass2Created;
    CMyClass1* m_pMyClass1;
    QString m_strMyClass2ObjName;
    CMyClass2* m_pMyClass2;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CMyClass2Thread

//******************************************************************************
class CMyClass2 : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
    static QString ClassName() { return "CMyClass2"; }
public: // ctors and dtor
    CMyClass2( const QString& i_strObjName, CMyClass2Thread* i_pMyClass2Thread = nullptr );
    virtual ~CMyClass2();
signals:
    void aboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
public: // instance methods (reimplementing methods of base class QObject)
    void setObjectName(const QString& i_strObjName);
public: // instance methods
    CMyClass3Thread* getMyClass3Thread() const { return m_pMyClass3Thread; }
    CMyClass3* getMyClass3() const { return m_pMyClass3; }
    ZS::System::CTrcAdminObj* getTrcAdminObj() { return m_pTrcAdminObj; }
public: // instance methods
    QString instMethod(const QString& i_strMthInArgs);
public: // instance methods
    int recursiveTraceMethod();
public: // instance methods
    void startMessageTimer();
public: // instance methods
    CMyClass3* startClass3Thread(const QString& i_strMyClass3ObjName);
    void stopClass3Thread();
protected slots:
    void onClass3ThreadRunning();
    void onTmrMessagesTimeout();
protected: // overridables of base class QObject
    virtual bool event( QEvent* i_pEv ) override;
private: // instance members
    CMyClass2Thread* m_pMyClass2Thread;
    QTimer* m_pTmrMessages;
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    ZS::System::CRecursiveMutex* m_pMtxCounters;
    #else
    ZS::System::CMutex* m_pMtxCounters;
    #endif
    int m_iRecursionCount;
    int m_iMsgCount;
    QString m_strMyClass3ObjName;
    CMyClass3Thread* m_pMyClass3Thread;
    CMyClass3* m_pMyClass3;
    ZS::System::CMutex* m_pMtxWaitClass3ThreadRunning;
    ZS::System::CWaitCondition* m_pWaitClass3ThreadRunning;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CMyClass2

} // namespace IpcTrace

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTrace_MyClass2_h
