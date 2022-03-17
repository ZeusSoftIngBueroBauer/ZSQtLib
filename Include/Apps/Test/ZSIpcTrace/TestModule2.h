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

#include "ZSSys/ZSSysTrcAdminObj.h"

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

//******************************************************************************
class CMyClass2Thread : public QThread
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
    static QString ClassName() { return "CMyClass2Thread"; }
public: // ctors and dtor
    CMyClass2Thread( const QString& i_strMyClass2ObjName, CMyClass1* i_pMyClass1 = nullptr );
    virtual  ~CMyClass2Thread();
public: // instance methods
    CMyClass2* getMyClass2() { return m_pMyClass2; }
public: // instance methods
    void sleep( unsigned long i_uTime_s );
public: // overridables of base class QThread
    void run();
private: // instance members
    CMyClass1*               m_pMyClass1;
    QString                  m_strMyClass2ObjName;
    CMyClass2*               m_pMyClass2;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CMyClass2Thread

//******************************************************************************
class CMyClass2 : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
    static QString ClassName() { return "CMyClass2"; }
public: // class methods
    static void setTraceServerName( const QString& i_strServerName );
    static QString getTraceServerName();
public: // ctors and dtor
    CMyClass2( const QString& i_strObjName, CMyClass2Thread* i_pMyClass2Thread = nullptr );
    virtual  ~CMyClass2();
public: // instance methods
    QString instMethod(const QString& i_strMthInArgs);
public: // instance methods
    int recursiveTraceMethod();
public: // instance methods
    void startMessageTimer();
protected slots:
    void onTmrMessagesTimeout();
protected: // overridables of base class QObject
    virtual bool event( QEvent* i_pEv ) override;
private: // class members
    static QString s_strTraceServerName;
private: // instance members
    CMyClass2Thread*         m_pMyClass2Thread;
    QTimer*                  m_pTmrMessages;
    int                      m_iMsgCount;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CMyClass2

} // namespace IpcTrace

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTrace_TestModule2_h
