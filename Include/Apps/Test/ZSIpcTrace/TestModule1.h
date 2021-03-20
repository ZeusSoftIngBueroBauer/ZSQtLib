/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestIpcTrace_TestModule1_h
#define ZSApps_TestIpcTrace_TestModule1_h

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>

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
class CTestModule2Thread;
class CTestModule2;

//******************************************************************************
class CTestModule1 : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcTrace"; }
    static QString ClassName() { return "CTestModule1"; }
public: // ctors and dtor
    CTestModule1( const QString& i_strObjName, const QString& i_strTestModule2ObjName );
    virtual ~CTestModule1();
public: // instance methods
    CTestModule2Thread* getTestModule2Thread() const { return m_pTestModule2Thread; }
    CTestModule2* getTestModule2() const { return m_pTestModule2; }
public: // instance methods
    CTestModule2* createModule2();
    void deleteModule2();
private: // instance members
    QString                  m_strTestModule2ObjName;
    CTestModule2Thread*      m_pTestModule2Thread;
    CTestModule2*            m_pTestModule2;
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTestModule1

} // namespace IpcTrace

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTrace_TestModule1_h
