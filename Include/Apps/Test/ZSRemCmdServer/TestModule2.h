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

#ifndef ZSApps_TestRemCmdServer_TestModule2_h
#define ZSApps_TestRemCmdServer_TestModule2_h

#include <QtCore/qstring.h>
#include <QtCore/qthread.h>

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
namespace RemCmdServer
{
class CTestModule1;
class CTestModule2;

//******************************************************************************
class CTestModule2Thread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CTestModule2Thread(
        CTestModule1*  i_pTestModule1,
        const QString& i_strObjNameTestModule2 );
    virtual  ~CTestModule2Thread();
signals:
    void createdTestModule2( QObject* i_pThread, QObject* i_pTestModule );
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
public: // ctors and dtor
    CTestModule2(
        CTestModule2Thread* i_pTestModule2Thread,
        const QString&      i_strObjName );
    virtual  ~CTestModule2();
public: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv );
private: // instance members
    CTestModule2Thread*      m_pTestModule2Thread;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTestModule2

} // namespace RemCmdServer

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestRemCmdServer_TestModule2_h
