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

#ifndef ZSApps_TestIpcTrace_MyClass3_h
#define ZSApps_TestIpcTrace_MyClass3_h

#include "ZSSys/ZSSysTrcAdminObj.h"

#include <QtCore/qdeadlinetimer.h>
#include <QtCore/qthread.h>

class QTimer;

namespace ZS
{
namespace System
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
class CRecursiveMutex;
#else
class CMutex;
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

//******************************************************************************
class CMyClass3Thread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
    static QString ClassName() { return "CMyClass3Thread"; }
public: // ctors and dtor
    CMyClass3Thread( const QString& i_strMyClass3ObjName, CMyClass2* i_pMyClass2 = nullptr );
    virtual ~CMyClass3Thread();
signals:
    void running();
    void aboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
public: // instance methods (reimplementing methods of base class QObject)
    void setObjectName(const QString& i_strObjName);
public: // instance methods
    CMyClass3* getMyClass3() { return m_pMyClass3; }
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
    void onClass3AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
private: // instance members
    CMyClass2* m_pMyClass2;
    QString m_strMyClass3ObjName;
    CMyClass3* m_pMyClass3;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CMyClass3Thread

//******************************************************************************
class CMyClass3 : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
    static QString ClassName() { return "CMyClass3"; }
public: // class methods
    static QString classMethod(const QString& i_strMthInArgs);
    static QString noisyClassMethod(const QString& i_strMthInArgs);
    static QString veryNoisyClassMethod(const QString& i_strMthInArgs);
public: // ctors and dtor
    CMyClass3( const QString& i_strObjName, CMyClass3Thread* i_pMyClass3Thread = nullptr );
    virtual ~CMyClass3();
signals:
    void aboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
public: // instance methods (reimplementing methods of base class QObject)
    void setObjectName(const QString& i_strObjName);
public: // instance methods
     QString instMethod(const QString& i_strMthInArgs);
     QString noisyInstMethod(const QString& i_strMthInArgs);
     QString veryNoisyInstMethod(const QString& i_strMthInArgs);
public: // instance methods
    int recursiveTraceMethod();
protected: // overridables of base class QObject
    virtual bool event( QEvent* i_pEv ) override;
private: // class members
    static ZS::System::CTrcAdminObjRefAnchor s_trcAdminObjRefAnchor;
    static ZS::System::CTrcAdminObjRefAnchor s_trcAdminObjRefAnchorNoisyMethods;
    static ZS::System::CTrcAdminObjRefAnchor s_trcAdminObjRefAnchorVeryNoisyMethods;
private: // instance members
    CMyClass3Thread* m_pMyClass3Thread;
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    ZS::System::CRecursiveMutex* m_pMtxCounters;
    #else
    ZS::System::CMutex* m_pMtxCounters;
    #endif
    int m_iRecursionCount;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjNoisyMethods;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjVeryNoisyMethods;

}; // class CMyClass3

} // namespace IpcTrace

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTrace_MyClass3_h
