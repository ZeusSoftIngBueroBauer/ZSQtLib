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

#ifndef ZSApps_TestIpcTraceDllIfQtApp_MyClass3_h
#define ZSApps_TestIpcTraceDllIfQtApp_MyClass3_h

#include <QtCore/qthread.h>

class QTimer;

namespace ZS
{
namespace System
{
class CMutex;
class CWaitCondition;
}
namespace Trace
{
namespace DllIf
{
class CTrcAdminObj;
}
}

namespace Apps
{
namespace Test
{
namespace IpcTraceDllIfQtApp
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
    static QString NameSpace() { return "ZS::Apps::Test::IpcTraceDllIfQtApp"; }
    static QString ClassName() { return "CMyClass3Thread"; }
public: // ctors and dtor
    CMyClass3Thread( const QString& i_strMyClass3ObjName, CMyClass2* i_pMyClass2 = nullptr );
    virtual ~CMyClass3Thread();
signals:
    void running();
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
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
    bool wait( QDeadlineTimer i_deadline = QDeadlineTimer(QDeadlineTimer::Forever) );
    bool wait( unsigned long i_time_ms );
protected: // replacing methods of base class QThread
    int exec();
protected slots:
    void onClass3AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
private: // instance members
    CMyClass2*                      m_pMyClass2;
    QString                         m_strMyClass3ObjName;
    CMyClass3*                      m_pMyClass3;
    ZS::Trace::DllIf::CTrcAdminObj* m_pTrcAdminObj;

}; // class CMyClass3Thread

//******************************************************************************
class CMyClass3 : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTraceDllIfQtApp"; }
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
public: // overridables
    virtual QString nameSpace() { return NameSpace(); }
    virtual QString className() { return ClassName(); }
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
private: // instance members
    CMyClass3Thread*                m_pMyClass3Thread;
    ZS::System::CMutex*             m_pMtxCounters;
    int                             m_iRecursionCount;
    ZS::Trace::DllIf::CTrcAdminObj* m_pTrcAdminObj;
    ZS::Trace::DllIf::CTrcAdminObj* m_pTrcAdminObjNoisyMethods;
    ZS::Trace::DllIf::CTrcAdminObj* m_pTrcAdminObjVeryNoisyMethods;

}; // class CMyClass3

} // namespace IpcTraceDllIfQtApp

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTraceDllIfQtApp_MyClass3_h