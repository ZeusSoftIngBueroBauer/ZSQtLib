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

#ifndef ZSApps_TestData_App_h
#define ZSApps_TestData_App_h

#include <QtCore/qglobal.h>

#include "Test.h"

#include "ZSSysGUI/ZSSysGUIApp.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysRequestQueue.h"

class QSettings;

namespace ZS
{
namespace System
{
class CErrLog;
class CRequest;
class CRequestExecTree;
struct SErrResultInfo;
}

namespace Trace
{
class CIpcTrcServer;
class CTrcAdminObj;
}

namespace Data
{
class CDataSet;
class CDb;
//class CDbClient;
}

namespace Apps
{
namespace Test
{
namespace Data
{
class CMainWindow;
class CTest;

//******************************************************************************
class CApplication : public ZS::System::GUI::CGUIApp
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static CApplication* GetInstance();
public: // ctors and dtor
    CApplication(
        int            i_argc,
        char*          i_argv[],
        const QString& i_strOrganizationName,
        const QString& i_strOrganizationDomain,
        const QString& i_strAppName,
        const QString& i_strWindowTitle );
    ~CApplication();
public: // instance methods
    QSettings* getSettingsFile() const { return m_pSettingsFile; }
    CTest* getTest() { return m_pTest; }
    ZS::Trace::CTrcServer* getTraceServer() { return m_pTrcServer; }
public: // instance methods
    void readSettings();
    void saveSettings();
protected slots: // of system startup and system shutdown
    void onLastWindowClosed();
protected slots:
    void onDbCreated( ZS::Data::CDb* i_pDb );
    void onDbAboutToBeDestroyed( ZS::Data::CDb* i_pDb );
    void onDbDataSetCreated( ZS::Data::CDataSet* i_pDS );
    void onDbDataSetAboutToBeDestroyed( ZS::Data::CDataSet* i_pDS );
protected slots:
    void onDbClt1Created( ZS::Data::CDbClient* i_pDbClt );
    void onDbClt1AboutToBeDestroyed( ZS::Data::CDbClient* i_pDbClt );
    void onDbClt1DataSetCreated( ZS::Data::CDataSet* i_pDS );
    void onDbClt1DataSetAboutToBeDestroyed( ZS::Data::CDataSet* i_pDS );
    void onDbClt2Created( ZS::Data::CDbClient* i_pDbClt );
    void onDbClt2AboutToBeDestroyed( ZS::Data::CDbClient* i_pDbClt );
    void onDbClt2DataSetCreated( ZS::Data::CDataSet* i_pDS );
    void onDbClt2DataSetAboutToBeDestroyed( ZS::Data::CDataSet* i_pDS );
    void onDbClt3Created( ZS::Data::CDbClient* i_pDbClt );
    void onDbClt3AboutToBeDestroyed( ZS::Data::CDbClient* i_pDbClt );
    void onDbClt3DataSetCreated( ZS::Data::CDataSet* i_pDS );
    void onDbClt3DataSetAboutToBeDestroyed( ZS::Data::CDataSet* i_pDS );
private: // instance members
    QSettings*                    m_pSettingsFile;
    QString                       m_strErrLogFileAbsFilePath;
    ZS::System::CErrLog*          m_pErrLog;
    // State Machine
    bool                          m_bKeepReqDscrInExecTree;
    bool                          m_bReqExecTreeGarbageCollectorEnabled;
    double                        m_fReqExecTreeGarbageCollectorInterval_s;
    double                        m_fReqExecTreeGarbageCollectorElapsed_s;
    ZS::System::CRequestExecTree* m_pReqExecTree;
    int                           m_iReqTimeout_ms;
    // Trace Server
    bool                          m_bTrcServerStartTcpServer;
    ZS::Ipc::SServerHostSettings  m_trcServerHostSettings;
    ZS::Trace::STrcServerSettings m_trcServerSettings;
    ZS::Trace::CIpcTrcServer*     m_pTrcServer;
    // Test
    QString                       m_strTestStepsFileAbsFilePath;
    QString                       m_strDbSchemaFileAbsFilePath;
    QString                       m_strDbDataFileAbsFilePath;
    CTest*                        m_pTest;
    CMainWindow*                  m_pMainWindow;
    ZS::Data::CDb*                m_pDb;
    ZS::Data::CDataSet*           m_pDSDb;
    ZS::Data::CDbClient*          m_pDbClt1;
    ZS::Data::CDataSet*           m_pDSDbClt1;
    ZS::Data::CDbClient*          m_pDbClt2;
    ZS::Data::CDataSet*           m_pDSDbClt2;
    ZS::Data::CDbClient*          m_pDbClt3;
    ZS::Data::CDataSet*           m_pDSDbClt3;
    ZS::Trace::CTrcAdminObj*      m_pTrcAdminObj;

}; // class CApplication

} // namespace Data

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestData_App_h
