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

#ifndef ZSApps_TestIpcTraceDllIfWinApp_TestModule1_h
#define ZSApps_TestIpcTraceDllIfWinApp_TestModule1_h

namespace ZS
{
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
namespace IpcTraceDllIfWinApp
{
class CTestModule2Thread;
class CTestModule2;

//******************************************************************************
class CTestModule1
//******************************************************************************
{
public: // class methods
    static const std::string NameSpace() { return "ZS::Apps::Test::IpcTraceDllIfWinApp"; }
    static const std::string ClassName() { return "CTestModule1"; }
public: // ctors and dtor
    CTestModule1( const std::string& i_strObjName, const std::string& i_strTestModule2ObjName );
    ~CTestModule1();
public: // instance methods
    std::string objectName() const { return m_strObjName; }
public: // instance methods
    CTestModule2Thread* getTestModule2Thread() const { return m_pTestModule2Thread; }
    CTestModule2* getTestModule2() const { return m_pTestModule2; }
public: // instance methods
    CTestModule2* createModule2();
    void deleteModule2();
private: // instance members
    std::string                     m_strObjName;
    std::string                     m_strTestModule2ObjName;
    CTestModule2Thread*             m_pTestModule2Thread;
    CTestModule2*                   m_pTestModule2;
    ZS::Trace::DllIf::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTestModule1

} // namespace IpcTraceDllIfWinApp

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTraceDllIfWinApp_TestModule1_h
