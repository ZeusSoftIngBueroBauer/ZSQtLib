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

#ifndef ZSApps_TestData_Test_h
#define ZSApps_TestData_Test_h

#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepAdminObj.h"
#include "ZSData/ZSDataCommon.h"
#include "ZSSys/ZSSysRequest.h"

class QTimer;

namespace ZS
{
namespace System
{
class CException;
struct SErrResultInfo;
}

namespace PhysVal
{
class CPhysUnit;
}
namespace Data
{
class CDataSet;
class CDataSetObjGroup;
class CDb;
class CDbClient;
}

namespace Apps
{
namespace Test
{
namespace Data
{
//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    static const QString c_strObjNameDb;
    static const QString c_strObjNameDbDS;
    static const QString c_strObjNameDbClt1;
    static const QString c_strObjNameDbClt2;
    static const QString c_strObjNameDbClt3;
    static const QString c_strObjNameDbClt1DS;
    static const QString c_strObjNameDbClt2DS;
    static const QString c_strObjNameDbClt3DS;
    //static const QString c_strObjNameGrpCommon;
    //static const QString c_strObjNameDESamplesPerSec;
    //static const QString c_strObjNameDPSingleEnumValue;
    //static const QString c_strObjNameDPOneDimEnumValues;
    //static const QString c_strObjNameDPMoreDimEnumValues;
    //static const QString c_strObjNameDPSinglePhysValue;
    //static const QString c_strObjNameDPOneDimPhysValues;
    //static const QString c_strObjNameDPMoreDimPhysValues;
    //static const QString c_strObjNameGrp1;
    //static const QString c_strObjNameGrp2;
public: // ctors and dtor
    CTest( const QString& i_strTestStepsFileName, const QString& i_strDbSchemaFilePath, const QString& i_strDbDataFilePath );
    ~CTest();
signals:
    void dbCreated( ZS::Data::CDb* i_pDb );
    void dbAboutToBeDestroyed( ZS::Data::CDb* i_pDb );
signals:
    void dbDataSetCreated( ZS::Data::CDataSet* i_pDS );
    void dbDataSetAboutToBeDestroyed( ZS::Data::CDataSet* i_pDS );
signals:
    void dbClt1Created( ZS::Data::CDbClient* i_pDbClt );
    void dbClt1AboutToBeDestroyed( ZS::Data::CDbClient* i_pDbClt );
    void dbClt1DataSetCreated( ZS::Data::CDataSet* i_pDS );
    void dbClt1DataSetAboutToBeDestroyed( ZS::Data::CDataSet* i_pDS );
    void dbClt2Created( ZS::Data::CDbClient* i_pDbClt );
    void dbClt2AboutToBeDestroyed( ZS::Data::CDbClient* i_pDbClt );
    void dbClt2DataSetCreated( ZS::Data::CDataSet* i_pDS );
    void dbClt2DataSetAboutToBeDestroyed( ZS::Data::CDataSet* i_pDS );
    void dbClt3Created( ZS::Data::CDbClient* i_pDbClt );
    void dbClt3AboutToBeDestroyed( ZS::Data::CDbClient* i_pDbClt );
    void dbClt3DataSetCreated( ZS::Data::CDataSet* i_pDS );
    void dbClt3DataSetAboutToBeDestroyed( ZS::Data::CDataSet* i_pDS );
public: // instance methods
    void setKeepReqDscrInExecTree( bool i_bKeep );
    bool keepReqDscrInExecTree() const { return m_bKeepReqDscrInExecTree; }
public: // instance methods
    void setReqTimeoutInMs( int i_iTimeout_ms );
    int getReqTimeoutInMs() const { return m_iReqTimeout_ms; }
public slots: // test step methods (GrpStartupDb)
    void doTestStepGrpStartupDbCreateDatabase( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbCreateDataSet( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbInitSettings( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbStartupDatabase( ZS::Test::CTestStep* i_pTestStep );
public slots: // test step methods (GrpShutdownDb)
    void doTestStepGrpShutdownDbShutdownDatabase( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbDestroyDataSet( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbDestroyDatabase( ZS::Test::CTestStep* i_pTestStep );
public slots: // test step methods (GrpStartupDbClt1)
    void doTestStepGrpStartupDbClt1CreateClient( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt1CreateDataSet( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt1InitSettings( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt1StartupClient( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt1Connect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt1Register( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt1SelectSchema( ZS::Test::CTestStep* i_pTestStep );
protected slots: // future callbacks (GrpStartupDbClt1)
    void onTestStepGrpStartupDbClt1RequestChanged( ZS::System::SRequestDscr i_reqDscr );
    void onTestStepGrpStartupDbClt1Timeout();
public slots: // test step methods (GrpStartupDbClt1)
    void doTestStepGrpStartupDbClt2CreateClient( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt2CreateDataSet( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt2InitSettings( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt2StartupClient( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt2Connect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt2Register( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt2SelectSchema( ZS::Test::CTestStep* i_pTestStep );
protected slots: // future callbacks (GrpStartupDbClt1)
    void onTestStepGrpStartupDbClt2RequestChanged( ZS::System::SRequestDscr i_reqDscr );
    void onTestStepGrpStartupDbClt2Timeout();
public slots: // test step methods (GrpStartupDbClt1)
    void doTestStepGrpStartupDbClt3CreateClient( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt3CreateDataSet( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt3InitSettings( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt3StartupClient( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt3Connect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt3Register( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpStartupDbClt3SelectSchema( ZS::Test::CTestStep* i_pTestStep );
protected slots: // future callbacks (GrpStartupDbClt1)
    void onTestStepGrpStartupDbClt3RequestChanged( ZS::System::SRequestDscr i_reqDscr );
    void onTestStepGrpStartupDbClt3Timeout();
public slots: // test step methods (GrpShutdownDbClt1)
    void doTestStepGrpShutdownDbClt1Unregister( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt1Disconnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt1ShutdownClient( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt1DestroyDataSet( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt1DestroyClient( ZS::Test::CTestStep* i_pTestStep );
protected slots: // future callbacks (GrpShutdownDbClt1)
    void onTestStepGrpShutdownDbClt1RequestChanged( ZS::System::SRequestDscr i_reqDscr );
    void onTestStepGrpShutdownDbClt1Timeout();
public slots: // test step methods (GrpShutdownDbClt1)
    void doTestStepGrpShutdownDbClt2Unregister( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt2Disconnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt2ShutdownClient( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt2DestroyDataSet( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt2DestroyClient( ZS::Test::CTestStep* i_pTestStep );
protected slots: // future callbacks (GrpShutdownDbClt1)
    void onTestStepGrpShutdownDbClt2RequestChanged( ZS::System::SRequestDscr i_reqDscr );
    void onTestStepGrpShutdownDbClt2Timeout();
public slots: // test step methods (GrpShutdownDbClt1)
    void doTestStepGrpShutdownDbClt3Unregister( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt3Disconnect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt3ShutdownClient( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt3DestroyDataSet( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepGrpShutdownDbClt3DestroyClient( ZS::Test::CTestStep* i_pTestStep );
protected slots: // future callbacks (GrpShutdownDbClt1)
    void onTestStepGrpShutdownDbClt3RequestChanged( ZS::System::SRequestDscr i_reqDscr );
    void onTestStepGrpShutdownDbClt3Timeout();
protected: // instance members
    bool                                                        m_bKeepReqDscrInExecTree;
    int                                                         m_iReqTimeout_ms;
    int                                                         m_iTestStepTimeoutSlotInterval_ms;
    int                                                         m_iTestStepTimeoutSlotCalled;
    QStringList                                                 m_strlstActualValuesTmp;
    // Database
    QList<ZS::Data::EProtocolType>                              m_arProtocolTypes;
    QHash<ZS::Data::EProtocolType, QList<ZS::Ipc::ESocketType>> m_hsharSocketTypes;
    QHash<ZS::Data::SCnctType, ZS::Ipc::SServerHostSettings>    m_hshHostSettingsDefault;
    QHash<ZS::Data::SCnctType, ZS::Ipc::SServerHostSettings>    m_hshHostSettings;
    QString                                                     m_strDbSchemaFilePath;
    QString                                                     m_strDbDataFilePath;
    ZS::Data::CDb*                                              m_pDb;
    ZS::Data::CDataSet*                                         m_pDSDb;
    // Database Clients
    QHash<ZS::Data::SCnctType, ZS::Ipc::STimerSettings>         m_hshWatchDogSettingsDefault;
    QHash<ZS::Data::SCnctType, ZS::Ipc::STimerSettings>         m_hshWatchDogSettings;
    ZS::Data::CDbClient*                                        m_pDbClt1;
    ZS::Ipc::SSocketDscr                                        m_socketDscrDbClt1Tmp;
    ZS::Data::CDataSet*                                         m_pDSDbClt1;
    ZS::Data::CDbClient*                                        m_pDbClt2;
    ZS::Ipc::SSocketDscr                                        m_socketDscrDbClt2Tmp;
    ZS::Data::CDataSet*                                         m_pDSDbClt2;
    ZS::Data::CDbClient*                                        m_pDbClt3;
    ZS::Ipc::SSocketDscr                                        m_socketDscrDbClt3Tmp;
    ZS::Data::CDataSet*                                         m_pDSDbClt3;
    QHash<qint64, ZS::System::CRequest*>                        m_hshClientRequestQueueReqsInProgress;

}; // class CTest

} // namespace Data

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestData_Test_h
