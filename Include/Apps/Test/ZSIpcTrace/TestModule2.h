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

#ifndef ZSApps_TestIpcTrace_TestModule2_h
#define ZSApps_TestIpcTrace_TestModule2_h

#include <QtCore/qstring.h>
#include <QtCore/qthread.h>

class QTimer;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Apps
{
namespace Test
{
namespace IpcTrace
{
class CTestModule1;
class CTestModule2;

//******************************************************************************
class CTestModule2Thread : public QThread
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
    static QString ClassName() { return "CTestModule2Thread"; }
public: // ctors and dtor
    CTestModule2Thread( CTestModule1* i_pTestModule1, const QString& i_strTestModule2ObjName );
    virtual  ~CTestModule2Thread();
public: // instance methods
    CTestModule2* getTestModule2() { return m_pTestModule2; }
public: // instance methods
    void sleep( unsigned long i_uTime_s );
public: // overridables of base class QThread
    void run();
private: // instance members
    CTestModule1*            m_pTestModule1;
    QString                  m_strTestModule2ObjName;
    CTestModule2*            m_pTestModule2;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTestModule2Thread

//******************************************************************************
class CTestModule2 : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
    static QString ClassName() { return "CTestModule2"; }
public: // ctors and dtor
    CTestModule2( CTestModule2Thread* i_pTestModule2Thread, const QString& i_strObjName );
    virtual  ~CTestModule2();
public: // instance methods
    int recursiveTraceMethod();
public: // instance methods
    void startMessageTimer();
protected slots:
    void onTmrMessagesTimeout();
protected: // overridables of base class QObject
    virtual bool event( QEvent* i_pEv ) override;
private: // instance members
    CTestModule2Thread*      m_pTestModule2Thread;
    QTimer*                  m_pTmrMessages;
    int                      m_iMsgCount;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTestModule2

} // namespace IpcTrace

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTrace_TestModule2_h
