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

#ifndef ZSApps_TestIpcTraceDllIfWinApp_TestModule2_h
#define ZSApps_TestIpcTraceDllIfWinApp_TestModule2_h

namespace std
{
class thread;
class recursive_mutex;
}

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
class CTestModule1;
class CTestModule2;

//******************************************************************************
class CTestModule2Thread
//******************************************************************************
{
public: // class methods
    static std::string NameSpace() { return "ZS::Apps::Test::IpcTraceDllIfWinApp"; }
    static std::string ClassName() { return "CTestModule2Thread"; }
public: // ctors and dtor
    CTestModule2Thread( CTestModule1* i_pTestModule1, const std::string& i_strObjNameTestModule2 );
    virtual  ~CTestModule2Thread();
public: // instance methods
    std::string objectName() const { return m_strObjName; }
public: // instance methods
    CTestModule2* getTestModule2();
public: // instance methods
    bool isRunning() const;
    void start();
    void quit();
    bool wait( unsigned long i_uTime_ms = ULONG_MAX );
    int exec();
public: // instance methods
    void run();
private: // instance members
    std::string                     m_strObjName;
    CTestModule1*                   m_pTestModule1;
    std::thread*                    m_pThread;
    bool                            m_bRunning;
    bool                            m_bQuit;
    std::recursive_mutex*           m_pMtx;
    CTestModule2*                   m_pTestModule2;
    ZS::Trace::DllIf::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTestModule2Thread

//******************************************************************************
class CTestModule2
//******************************************************************************
{
public: // class methods
    static const std::string NameSpace() { return "ZS::Apps::Test::IpcTraceDllIfWinApp"; }
    static const std::string ClassName() { return "CTestModule2"; }
public: // ctors and dtor
    CTestModule2( CTestModule2Thread* i_pTestModule2Thread, const std::string& i_strObjName );
    virtual  ~CTestModule2();
public: // instance methods
    std::string objectName() const { return m_strObjName; }
public: // instance methods
    int recursiveTraceMethod();
private: // instance members
    std::string                     m_strObjName;
    CTestModule2Thread*             m_pTestModule2Thread;
    ZS::Trace::DllIf::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTestModule2

} // namespace IpcTraceDllIfWinApp

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTraceDllIfWinApp_TestModule2_h
