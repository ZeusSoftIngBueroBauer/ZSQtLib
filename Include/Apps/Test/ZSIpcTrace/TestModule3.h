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

#ifndef ZSApps_TestIpcTrace_TestModule3_h
#define ZSApps_TestIpcTrace_TestModule3_h

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
class CMyClass1;
class CMyClass2;
class CMyClass3;

//******************************************************************************
class CMyClass3Thread : public QThread
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
    static QString ClassName() { return "CMyClass3Thread"; }
public: // ctors and dtor
    CMyClass3Thread( CMyClass2* i_pMyClass2, const QString& i_strMyClass3ObjName );
    virtual  ~CMyClass3Thread();
public: // instance methods
    CMyClass3* getMyClass3() { return m_pMyClass3; }
public: // instance methods
    void sleep( unsigned long i_uTime_s );
public: // overridables of base class QThread
    void run();
private: // class members
    ZS::Trace::CTrcAdminObj* s_pTrcAdminObj;
private: // instance members
    CMyClass2*               m_pMyClass2;
    QString                  m_strMyClass3ObjName;
    CMyClass3*               m_pMyClass3;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CMyClass3Thread

//******************************************************************************
class CMyClass3 : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
    static QString ClassName() { return "CMyClass3"; }
public: // ctors and dtor
    CMyClass3( CMyClass3Thread* i_pMyClass3Thread, const QString& i_strObjName );
    virtual ~CMyClass3();
public: // instance methods
    int recursiveTraceMethod();
public: // instance methods
    void startMessageTimer();
protected slots:
    void onTmrMessagesTimeout();
protected: // overridables of base class QObject
    virtual bool event( QEvent* i_pEv ) override;
private: // class members
    ZS::Trace::CTrcAdminObj* s_pTrcAdminObj;
private: // instance members
    CMyClass3Thread*         m_pMyClass3Thread;
    QTimer*                  m_pTmrMessages;
    int                      m_iMsgCount;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CMyClass3

} // namespace IpcTrace

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTrace_TestModule3_h
