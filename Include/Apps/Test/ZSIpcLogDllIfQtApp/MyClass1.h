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

#ifndef ZSApps_TestIpcLogDllIfQtApp_MyClass1_h
#define ZSApps_TestIpcLogDllIfQtApp_MyClass1_h

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>

namespace ZS
{
namespace Log
{
namespace DllIf
{
class CLogger;
}
}
namespace Apps
{
namespace Test
{
namespace IpcLogDllIfQtApp
{
class CMyClass2Thread;
class CMyClass2;

//******************************************************************************
class CMyClass1 : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Test::IpcLogDllIfQtApp"; }
    static QString ClassName() { return "CMyClass1"; }
public: // class methods
    static QString classMethod(const QString& i_strMthInArgs);
public: // ctors and dtor
    CMyClass1( const QString& i_strObjName );
    virtual ~CMyClass1();
signals:
    void aboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName);
public: // instance methods
    CMyClass2Thread* getMyClass2Thread() const { return m_pMyClass2Thread; }
    CMyClass2* getMyClass2() const { return m_pMyClass2; }
public: // instance methods
    CMyClass2* startClass2Thread(const QString& i_strMyClass2ObjName);
    void stopClass2Thread();
public: // instance methods
    void sendMuchData();
private: // instance methods
    int sendData1(const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s);
    int sendData2(const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s);
    int sendData3(const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s);
private: // class members
    static ZS::Log::DllIf::CLogger* s_pLogger;
    static int s_iInstCount;
private: // instance members
    QString          m_strMyClass2ObjName;
    CMyClass2Thread* m_pMyClass2Thread;
    CMyClass2*       m_pMyClass2;

}; // class CMyClass1

} // namespace IpcLogDllIfQtApp

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcLogDllIfQtApp_MyClass1_h
