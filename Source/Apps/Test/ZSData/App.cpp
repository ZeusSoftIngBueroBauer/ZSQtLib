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

#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qsettings.h>
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>
#include <QtGui/qbitmap.h>

#if QT_VERSION < 0x050000
#include <QtGui/qmessagebox.h>
#else
#include <QtWidgets/qmessagebox.h>
#endif

#include "App.h"
#include "MainWindow.h"

#include "ZSData/ZSDatabaseClient.h"
#include "ZSData/ZSDataSet.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Data;
using namespace ZS::Apps::Test::Data;


/*******************************************************************************
class CApplication : public ZS::System::GUI::CGUIApp
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CApplication* CApplication::GetInstance()
//------------------------------------------------------------------------------
{
    return dynamic_cast<CApplication*>(QApplication::instance());
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CApplication::CApplication(
    int            i_argc,
    char*          i_argv[],
    const QString& i_strOrganizationName,
    const QString& i_strOrganizationDomain,
    const QString& i_strAppName,
    const QString& i_strWindowTitle ) :
//------------------------------------------------------------------------------
    CGUIApp(i_argc,i_argv),
    m_pSettingsFile(nullptr),
    // State Machine
    m_bKeepReqDscrInExecTree(false),
    m_bReqExecTreeGarbageCollectorEnabled(true),
    m_fReqExecTreeGarbageCollectorInterval_s(5.0),
    m_fReqExecTreeGarbageCollectorElapsed_s(60.0),
    m_pReqExecTree(nullptr),
    m_iReqTimeout_ms(10000),
    // Trace Server
    m_bTrcServerStartTcpServer(true),
    m_trcServerHostSettings(
        /* strLocalHostName       */ "127.0.0.1",
        /* uLocalPort             */ 24763,
        /* uMaxPendingConnections */ 30 ),
    m_trcServerSettings(),
    m_pTrcServer(nullptr),
    // Test
    m_strTestStepsFileAbsFilePath(),
    m_strDbSchemaFileAbsFilePath(),
    m_strDbDataFileAbsFilePath(),
    m_pTest(nullptr),
    m_pMainWindow(nullptr),
    m_pDb(nullptr),
    m_pDSDb(nullptr),
    m_pDbClt1(nullptr),
    m_pDSDbClt1(nullptr),
    m_pDbClt2(nullptr),
    m_pDSDbClt2(nullptr),
    m_pDbClt3(nullptr),
    m_pDSDbClt3(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName("theApp");

    if( thread()->objectName().length() == 0 )
    {
        thread()->setObjectName("GUIMain");
    }

    QCoreApplication::setOrganizationName(i_strOrganizationName);
    QCoreApplication::setOrganizationDomain(i_strOrganizationDomain);
    QCoreApplication::setApplicationName(i_strAppName);

    QIcon iconApp;

    QPixmap pxmApp16x16(":/ZS/App/Zeus16x16.bmp");
    QPixmap pxmApp32x32(":/ZS/App/Zeus32x32.bmp");

    pxmApp16x16.setMask(pxmApp16x16.createHeuristicMask());
    pxmApp32x32.setMask(pxmApp32x32.createHeuristicMask());

    iconApp.addPixmap(pxmApp32x32);
    iconApp.addPixmap(pxmApp32x32);

    QApplication::setWindowIcon(iconApp);

    //setStyle("Cleanlooks");

    ZS::Ipc::SServerHostSettings trcServerHostSettingsDefault = m_trcServerHostSettings;
    ZS::Trace::STrcServerSettings trcServerSettingsDefault = m_trcServerSettings;

    // Parse command arguments (first part, IniFile)
    //----------------------------------------------

    int         idxArg;
    QString     strArg;
    QString     strVal;
    int         iVal;
    bool        bConverted;
    QStringList strListArgsPar;
    QStringList strListArgsVal;
    QStringList strListVals;

    // Range of IniFileScope: ["AppDir", "User", "System"]
    #ifdef __linux__
    // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
    // where the application has not write access rights. Stupid ...
    QString strIniFileScope = "User";
    #else
    QString strIniFileScope = "System"; // Default
    #endif

    parseAppArgs( i_argc, i_argv, strListArgsPar, strListArgsVal );

    #if QT_VERSION >= 0x040501
    for( idxArg = 0; idxArg < strListArgsPar.length() && idxArg < strListArgsVal.length(); idxArg++ )
    #else
    for( idxArg = 0; idxArg < strListArgsPar.size() && idxArg < strListArgsVal.size(); idxArg++ )
    #endif
    {
        strArg = strListArgsPar[idxArg];
        strVal = strListArgsVal[idxArg];

        // Here only the command arguments concerning the location of the ini file are parsed.
        // Other arguments (e.g. mode) are parsed further below.
        if( strArg.compare("IniFileScope",Qt::CaseInsensitive) == 0 )
        {
            strIniFileScope = strVal;
        }
    }

    // Calculate default file paths and create ini file
    //-------------------------------------------------

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strAppConfigDir = ZS::System::getAppConfigDir(strIniFileScope);
    QString strAppLogDir = ZS::System::getAppLogDir(strIniFileScope);
    QString strAppDataDir = ZS::System::getAppDataDir(strIniFileScope);

    QString strIniFileBaseName = strAppNameNormalized;
    QString strIniFileSuffix = "ini";

    QString strIniFileAbsFilePath = strAppConfigDir + "/" + strIniFileBaseName + "." + strIniFileSuffix;

    m_pSettingsFile = new QSettings( strIniFileAbsFilePath, QSettings::IniFormat );

    QString strErrLogFileBaseName = strAppNameNormalized + "-Error";
    QString strErrLogFileSuffix = "xml";

    m_strErrLogFileAbsFilePath = strAppLogDir + "/" + strErrLogFileBaseName + "." + strErrLogFileSuffix;

    QString strTrcAdminObjFileBaseName = strAppNameNormalized + "-TrcMthAdmObj";
    QString strTrcAdminObjFileSuffix = "xml";

    QString strTrcLogFileBaseName = strAppNameNormalized;
    QString strTrcLogFileSuffix = "log";

    m_trcServerSettings.m_strAdminObjFileAbsFilePath = strAppConfigDir + "/" + strTrcAdminObjFileBaseName + "." + strTrcAdminObjFileSuffix;
    m_trcServerSettings.m_strLocalTrcFileAbsFilePath = strAppLogDir + "/" + strTrcLogFileBaseName + "." + strTrcLogFileSuffix;

    trcServerSettingsDefault.m_strAdminObjFileAbsFilePath = m_trcServerSettings.m_strAdminObjFileAbsFilePath;
    trcServerSettingsDefault.m_strLocalTrcFileAbsFilePath = m_trcServerSettings.m_strLocalTrcFileAbsFilePath;

    QString strTestStepsFileBaseName = strAppNameNormalized + "-TestSteps";
    QString strTestStepsFileSuffix = "xml";

    m_strTestStepsFileAbsFilePath = strAppConfigDir + "/" + strTestStepsFileBaseName + "." + strTestStepsFileSuffix;

    QString strDbSchemaFileBaseName = strAppNameNormalized + "-Schema";
    QString strDbSchemaFileSuffix = "xml";

    QString strDbDataFileBaseName  = strAppNameNormalized + "-Data";
    QString strDbDataFileSuffix = "xml";

    m_strDbSchemaFileAbsFilePath = strAppDataDir + "/" + strDbSchemaFileBaseName + "." + strDbSchemaFileSuffix;
    m_strDbDataFileAbsFilePath = strAppDataDir + "/" + strDbDataFileBaseName + "." + strDbDataFileSuffix;

    readSettings();

    // Parse command arguments (second part, overwriting IniFile settings)
    //--------------------------------------------------------------------

    #if QT_VERSION >= 0x040501
    for( idxArg = 0; idxArg < strListArgsPar.length() && idxArg < strListArgsVal.length(); idxArg++ )
    #else
    for( idxArg = 0; idxArg < strListArgsPar.size() && idxArg < strListArgsVal.size(); idxArg++ )
    #endif
    {
        strArg = strListArgsPar[idxArg];
        strVal = strListArgsVal[idxArg];
    }

    // Read Settings
    //----------------------------

    readSettings(); // Trace Settings,File Names, etc.

    // Parse command arguments (second part, overwrite settings from IniFile)
    //-----------------------------------------------------------------------

    #if QT_VERSION >= 0x040501
    for( idxArg = 0; idxArg < strListArgsPar.length() && idxArg < strListArgsVal.length(); idxArg++ )
    #else
    for( idxArg = 0; idxArg < strListArgsPar.size() && idxArg < strListArgsVal.size(); idxArg++ )
    #endif
    {
        strArg = strListArgsPar[idxArg];
        strVal = strListArgsVal[idxArg];

        if( strArg.compare("ReqTimeout",Qt::CaseInsensitive) == 0 )
        {
            iVal = strVal.toInt(&bConverted);
            if( bConverted )
            {
                m_iReqTimeout_ms = iVal;
            }
        }
        else if( strArg.compare("KeepReqDscrInExecTree",Qt::CaseInsensitive) == 0 )
        {
            if( strVal.isEmpty() )
            {
                m_bKeepReqDscrInExecTree = true;
            }
            else
            {
                m_bKeepReqDscrInExecTree = str2Bool(strVal);
            }
        }
    } // for( idxArg = 0; idxArg < strListArgsPar.size() && idxArg < strListArgsVal.size(); idxArg++ )

    // Create error manager
    //------------------------

    CErrLog::CreateInstance(true, m_strErrLogFileAbsFilePath);

    // Request Execution Tree
    //------------------------

    m_pReqExecTree = CRequestExecTree::CreateInstance();

    m_pReqExecTree->setGarbageCollectorEnabled(m_bReqExecTreeGarbageCollectorEnabled);
    m_pReqExecTree->setGarbageCollectorIntervalInSec(m_fReqExecTreeGarbageCollectorInterval_s);
    m_pReqExecTree->setGarbageCollectorElapsedInSec(m_fReqExecTreeGarbageCollectorElapsed_s);

    // Create trace server
    //------------------------

    m_pTrcServer = ZS::Trace::CIpcTrcServer::CreateInstance();

    m_pTrcServer->setHostSettings(m_trcServerHostSettings);
    m_pTrcServer->setTraceSettings(m_trcServerSettings);
    m_pTrcServer->recallAdminObjs();

    if( m_bTrcServerStartTcpServer )
    {
        m_pTrcServer->startup();
    }
    else
    {
        m_pTrcServer->changeSettings();
    }

    // Get trace admin object
    //-----------------------

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::Data", "CApplication", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // Test
    //----------------------------

    m_pTest = new CTest(m_strTestStepsFileAbsFilePath, m_strDbSchemaFileAbsFilePath, m_strDbDataFileAbsFilePath);

    m_pTest->setKeepReqDscrInExecTree(m_bKeepReqDscrInExecTree);
    m_pTest->setReqTimeoutInMs(m_iReqTimeout_ms);

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbCreated(ZS::Data::CDb*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbCreated(ZS::Data::CDb*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbAboutToBeDestroyed(ZS::Data::CDb*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbAboutToBeDestroyed(ZS::Data::CDb*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbDataSetCreated(ZS::Data::CDataSet*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbDataSetCreated(ZS::Data::CDataSet*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbDataSetAboutToBeDestroyed(ZS::Data::CDataSet*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbDataSetAboutToBeDestroyed(ZS::Data::CDataSet*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt1Created(ZS::Data::CDbClient*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt1Created(ZS::Data::CDbClient*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt1AboutToBeDestroyed(ZS::Data::CDbClient*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt1AboutToBeDestroyed(ZS::Data::CDbClient*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt1DataSetCreated(ZS::Data::CDataSet*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt1DataSetCreated(ZS::Data::CDataSet*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt1DataSetAboutToBeDestroyed(ZS::Data::CDataSet*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt1DataSetAboutToBeDestroyed(ZS::Data::CDataSet*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt2Created(ZS::Data::CDbClient*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt2Created(ZS::Data::CDbClient*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt2AboutToBeDestroyed(ZS::Data::CDbClient*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt2AboutToBeDestroyed(ZS::Data::CDbClient*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt2DataSetCreated(ZS::Data::CDataSet*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt2DataSetCreated(ZS::Data::CDataSet*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt2DataSetAboutToBeDestroyed(ZS::Data::CDataSet*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt2DataSetAboutToBeDestroyed(ZS::Data::CDataSet*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt3Created(ZS::Data::CDbClient*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt3Created(ZS::Data::CDbClient*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt3AboutToBeDestroyed(ZS::Data::CDbClient*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt3AboutToBeDestroyed(ZS::Data::CDbClient*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt3DataSetCreated(ZS::Data::CDataSet*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt3DataSetCreated(ZS::Data::CDataSet*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL(dbClt3DataSetAboutToBeDestroyed(ZS::Data::CDataSet*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbClt3DataSetAboutToBeDestroyed(ZS::Data::CDataSet*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Main Window
    //------------

    setQuitOnLastWindowClosed(false);

    if( !QObject::connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL(lastWindowClosed()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onLastWindowClosed()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pMainWindow = new CMainWindow(i_strWindowTitle);
    m_pMainWindow->show();

} // ctor

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    saveSettings();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcServer->saveAdminObjs();

    if( m_pTrcServer != nullptr )
    {
        m_pTrcServer->saveAdminObjs();
    }

    try
    {
        delete m_pMainWindow;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pTest;
    }
    catch(...)
    {
    }

    try
    {
        ZS::Trace::CTrcServer::DestroyInstance(m_pTrcServer);
    }
    catch(...)
    {
    }

    try
    {
        delete m_pSettingsFile;
    }
    catch(...)
    {
    }

    CRequestExecTree::DestroyInstance();

    CErrLog::DestroyInstance();

    m_pSettingsFile = nullptr;
    //m_strErrLogFileAbsFilePath;
    // State Machine
    m_bKeepReqDscrInExecTree = false;
    m_bReqExecTreeGarbageCollectorEnabled = false;
    m_fReqExecTreeGarbageCollectorInterval_s = 0.0;
    m_fReqExecTreeGarbageCollectorElapsed_s = 0.0;
    m_pReqExecTree = nullptr;
    m_iReqTimeout_ms = 0;
    // Trace Server
    m_bTrcServerStartTcpServer = false;
    //m_trcServerHostSettings;
    //m_trcServerSettings;
    m_pTrcServer = nullptr;
    // Test
    //m_strTestStepsFileAbsFilePath;
    //m_strDbSchemaFileAbsFilePath;
    //m_strDbDataFileAbsFilePath;
    m_pTest = nullptr;
    m_pMainWindow = nullptr;
    m_pDb = nullptr;
    m_pDSDb = nullptr;
    m_pDbClt1 = nullptr;
    m_pDSDbClt1 = nullptr;
    m_pDbClt2 = nullptr;
    m_pDSDbClt2 = nullptr;
    m_pDbClt3 = nullptr;
    m_pDSDbClt3 = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::readSettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "readSettings",
        /* strAddInfo   */ "" );

    if( m_pSettingsFile != nullptr )
    {
        QString strSettingsKey;
        bool    bSyncSettings;

        // Err Log
        //------------------------

        strSettingsKey = "ErrLog";
        bSyncSettings  = false;

        if( m_pSettingsFile->contains(strSettingsKey+"/FileName") )
        {
            m_strErrLogFileAbsFilePath = m_pSettingsFile->value(strSettingsKey+"/FileName",m_strErrLogFileAbsFilePath).toString();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/FileName", m_strErrLogFileAbsFilePath );
            bSyncSettings = true;
        }

        if( bSyncSettings )
        {
            m_pSettingsFile->sync();
        }

        // Request Execution Tree
        //------------------------

        strSettingsKey = "ReqExecTree";
        bSyncSettings  = false;

        if( m_pSettingsFile->contains(strSettingsKey+"/GarbageCollectorEnabled") )
        {
            m_bReqExecTreeGarbageCollectorEnabled = str2Bool( m_pSettingsFile->value(strSettingsKey+"/GarbageCollectorEnabled",bool2Str(m_bReqExecTreeGarbageCollectorEnabled)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/GarbageCollectorEnabled", m_bReqExecTreeGarbageCollectorEnabled );
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/GarbageCollectorIntervalInSec") )
        {
            m_fReqExecTreeGarbageCollectorInterval_s = m_pSettingsFile->value(strSettingsKey+"/GarbageCollectorIntervalInSec",m_fReqExecTreeGarbageCollectorInterval_s).toDouble();
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/GarbageCollectorIntervalInSec",m_fReqExecTreeGarbageCollectorInterval_s);
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/GarbageCollectorElapsedInSec") )
        {
            m_fReqExecTreeGarbageCollectorElapsed_s = m_pSettingsFile->value(strSettingsKey+"/GarbageCollectorElapsedInSec",m_fReqExecTreeGarbageCollectorElapsed_s).toDouble();
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/GarbageCollectorElapsedInSec",m_fReqExecTreeGarbageCollectorElapsed_s);
            bSyncSettings = true;
        }

        if( bSyncSettings )
        {
            m_pSettingsFile->sync();
        }

        // Trace Server
        //-------------

        strSettingsKey = "TrcServer";
        bSyncSettings  = false;

        // Host Settings
        if( m_pSettingsFile->contains(strSettingsKey+"/AutoStartTcpServer") )
        {
            m_bTrcServerStartTcpServer = str2Bool( m_pSettingsFile->value(strSettingsKey+"/AutoStartTcpServer",bool2Str(m_bTrcServerStartTcpServer)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/AutoStartTcpServer", bool2Str(m_bTrcServerStartTcpServer) );
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/SocketType") )
        {
            m_trcServerHostSettings.m_socketType = str2SocketType( m_pSettingsFile->value(strSettingsKey+"/SocketType",socketType2Str(m_trcServerHostSettings.m_socketType)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/SocketType", socketType2Str(m_trcServerHostSettings.m_socketType) );
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/LocalHostName") )
        {
            m_trcServerHostSettings.m_strLocalHostName = m_pSettingsFile->value(strSettingsKey+"/LocalHostName",m_trcServerHostSettings.m_strLocalHostName).toString();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/LocalHostName", m_trcServerHostSettings.m_strLocalHostName );
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/LocalPort") )
        {
            m_trcServerHostSettings.m_uLocalPort = m_pSettingsFile->value(strSettingsKey+"/LocalPort",m_trcServerHostSettings.m_uLocalPort).toUInt();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/LocalPort", m_trcServerHostSettings.m_uLocalPort );
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/MaxPendingConnections") )
        {
            m_trcServerHostSettings.m_uMaxPendingConnections = m_pSettingsFile->value(strSettingsKey+"/MaxPendingConnections",m_trcServerHostSettings.m_uMaxPendingConnections).toUInt();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/MaxPendingConnections", m_trcServerHostSettings.m_uMaxPendingConnections );
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/BufferSize") )
        {
            m_trcServerHostSettings.m_uBufferSize = m_pSettingsFile->value(strSettingsKey+"/BufferSize",m_trcServerHostSettings.m_uBufferSize).toUInt();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/BufferSize", m_trcServerHostSettings.m_uBufferSize );
            bSyncSettings = true;
        }

        // Trace Settings
        if( m_pSettingsFile->contains(strSettingsKey+"/Enabled") )
        {
            m_trcServerSettings.m_bEnabled = str2Bool( m_pSettingsFile->value(strSettingsKey+"/Enabled",bool2Str(m_trcServerSettings.m_bEnabled)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/Enabled",bool2Str(m_trcServerSettings.m_bEnabled));
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/NewTrcAdminObjsEnabledAsDefault") )
        {
            m_trcServerSettings.m_bNewTrcAdminObjsEnabledAsDefault = str2Bool( m_pSettingsFile->value(strSettingsKey+"/NewTrcAdminObjsEnabledAsDefault",bool2Str(m_trcServerSettings.m_bNewTrcAdminObjsEnabledAsDefault)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/NewTrcAdminObjsEnabledAsDefault",bool2Str(m_trcServerSettings.m_bNewTrcAdminObjsEnabledAsDefault));
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/NewTrcAdminObjsDefaultDetailLevel") )
        {
            m_trcServerSettings.m_iNewTrcAdminObjsDefaultDetailLevel = m_pSettingsFile->value(strSettingsKey+"/NewTrcAdminObjsDefaultDetailLevel",m_trcServerSettings.m_iNewTrcAdminObjsDefaultDetailLevel).toInt();
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/NewTrcAdminObjsDefaultDetailLevel",m_trcServerSettings.m_iNewTrcAdminObjsDefaultDetailLevel);
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/CacheDataIfNotConnected") )
        {
            m_trcServerSettings.m_bCacheDataIfNotConnected = str2Bool( m_pSettingsFile->value(strSettingsKey+"/CacheDataIfNotConnected",bool2Str(m_trcServerSettings.m_bCacheDataIfNotConnected)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/CacheDataIfNotConnected",bool2Str(m_trcServerSettings.m_bCacheDataIfNotConnected));
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/CacheDataMaxArrLen") )
        {
            m_trcServerSettings.m_iCacheDataMaxArrLen = m_pSettingsFile->value(strSettingsKey+"/CacheDataMaxArrLen",m_trcServerSettings.m_iCacheDataMaxArrLen).toInt();
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/CacheDataMaxArrLen",m_trcServerSettings.m_iCacheDataMaxArrLen);
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/AdminObjFileAbsFilePath") )
        {
            m_trcServerSettings.m_strAdminObjFileAbsFilePath = m_pSettingsFile->value(strSettingsKey+"/AdminObjFileAbsFilePath",m_trcServerSettings.m_strAdminObjFileAbsFilePath).toString();
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/AdminObjFileAbsFilePath",m_trcServerSettings.m_strAdminObjFileAbsFilePath);
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/UseLocalTrcFile") )
        {
            m_trcServerSettings.m_bUseLocalTrcFile = str2Bool( m_pSettingsFile->value(strSettingsKey+"/UseLocalTrcFile",bool2Str(m_trcServerSettings.m_bUseLocalTrcFile)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/UseLocalTrcFile",bool2Str(m_trcServerSettings.m_bUseLocalTrcFile));
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/LocalTrcFileAbsFilePath") )
        {
            m_trcServerSettings.m_strLocalTrcFileAbsFilePath = m_pSettingsFile->value(strSettingsKey+"/LocalTrcFileAbsFilePath",m_trcServerSettings.m_strLocalTrcFileAbsFilePath).toString();
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/LocalTrcFileAbsFilePath",m_trcServerSettings.m_strLocalTrcFileAbsFilePath);
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/LocalTrcFileAutoSaveIntervalInMs") )
        {
            m_trcServerSettings.m_iLocalTrcFileAutoSaveInterval_ms = m_pSettingsFile->value(strSettingsKey+"/LocalTrcFileAutoSaveIntervalInMs",m_trcServerSettings.m_iLocalTrcFileAutoSaveInterval_ms).toInt();
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/LocalTrcFileAutoSaveIntervalInMs",m_trcServerSettings.m_iLocalTrcFileAutoSaveInterval_ms);
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/LocalTrcFileSubFileCountMax") )
        {
            m_trcServerSettings.m_iLocalTrcFileSubFileCountMax = m_pSettingsFile->value(strSettingsKey+"/LocalTrcFileSubFileCountMax",m_trcServerSettings.m_iLocalTrcFileSubFileCountMax).toInt();
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/LocalTrcFileSubFileCountMax",m_trcServerSettings.m_iLocalTrcFileSubFileCountMax);
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/LocalTrcFileSubFileLineCountMax") )
        {
            m_trcServerSettings.m_iLocalTrcFileSubFileLineCountMax = m_pSettingsFile->value(strSettingsKey+"/LocalTrcFileSubFileLineCountMax",m_trcServerSettings.m_iLocalTrcFileSubFileLineCountMax).toInt();
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/LocalTrcFileSubFileLineCountMax",m_trcServerSettings.m_iLocalTrcFileSubFileLineCountMax);
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/LocalTrcFileCloseFileAfterEachWrite") )
        {
            m_trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite = str2Bool( m_pSettingsFile->value( strSettingsKey + "/LocalTrcFileCloseFileAfterEachWrite", bool2Str(m_trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite) ).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/LocalTrcFileCloseFileAfterEachWrite",bool2Str(m_trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/Enabled") )
        {
            m_trcServerSettings.m_bEnabled = str2Bool( m_pSettingsFile->value(strSettingsKey+"/Enabled",bool2Str(m_trcServerSettings.m_bEnabled)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/Enabled",bool2Str(m_trcServerSettings.m_bEnabled));
            bSyncSettings = true;
        }

        if( bSyncSettings )
        {
            m_pSettingsFile->sync();
        }

        // Test Steps
        //-------------

        strSettingsKey = "TestSteps";
        bSyncSettings  = false;

        if( m_pSettingsFile->contains(strSettingsKey+"/FileName") )
        {
            m_strTestStepsFileAbsFilePath = m_pSettingsFile->value(strSettingsKey+"/FileName",m_strTestStepsFileAbsFilePath).toString();
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/FileName",m_strTestStepsFileAbsFilePath);
            bSyncSettings = true;
        }

        if( bSyncSettings )
        {
            m_pSettingsFile->sync();
        }

    } // if( m_pSettingsFile != nullptr )

} // readSettings

//------------------------------------------------------------------------------
void CApplication::saveSettings()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "saveSettings",
        /* strAddInfo   */ "" );

    if( m_pSettingsFile != nullptr )
    {
        QString strSettingsKey;

        if( m_pReqExecTree != nullptr )
        {
            strSettingsKey = "ReqExecTree";

            bool   bGarbageCollectorEnabled    = m_pReqExecTree->isGarbageCollectorEnabled();
            double fGarbageCollectorInterval_s = m_pReqExecTree->getGarbageCollectorIntervalInSec();
            double fGarbageCollectorElapsed_s  = m_pReqExecTree->getGarbageCollectorElapsedInSec();

            m_pSettingsFile->setValue( strSettingsKey+"/GarbageCollectorEnabled", bGarbageCollectorEnabled );
            m_pSettingsFile->setValue( strSettingsKey+"/GarbageCollectorIntervalInSec", fGarbageCollectorInterval_s );
            m_pSettingsFile->setValue( strSettingsKey+"/GarbageCollectorElapsedInSec", fGarbageCollectorElapsed_s );
        }

        if( m_pTrcServer != nullptr )
        {
            strSettingsKey = "TrcServer";

            SServerHostSettings hostSettings = m_pTrcServer->getHostSettings();
            STrcServerSettings  trcSettings  = m_pTrcServer->getTraceSettings();

            //m_pSettingsFile->setValue( strSettingsKey+"/AutoStartTcpServer", bool2Str(m_bTrcServerStartTcpServer) );
            m_pSettingsFile->setValue( strSettingsKey+"/SocketType", socketType2Str(hostSettings.m_socketType) );
            m_pSettingsFile->setValue( strSettingsKey+"/LocalHostName", hostSettings.m_strLocalHostName );
            m_pSettingsFile->setValue( strSettingsKey+"/LocalPort", hostSettings.m_uLocalPort );
            m_pSettingsFile->setValue( strSettingsKey+"/MaxPendingConnections", hostSettings.m_uMaxPendingConnections );
            m_pSettingsFile->setValue( strSettingsKey+"/BufferSize", hostSettings.m_uBufferSize );

            m_pSettingsFile->setValue( strSettingsKey+"/Enabled", bool2Str(trcSettings.m_bEnabled) );
            m_pSettingsFile->setValue( strSettingsKey+"/NewTrcAdminObjsEnabledAsDefault", bool2Str(trcSettings.m_bNewTrcAdminObjsEnabledAsDefault) );
            m_pSettingsFile->setValue( strSettingsKey+"/NewTrcAdminObjsDefaultDetailLevel", QString::number(trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel) );
            m_pSettingsFile->setValue( strSettingsKey+"/CacheDataIfNotConnected", bool2Str(trcSettings.m_bCacheDataIfNotConnected) );
            m_pSettingsFile->setValue( strSettingsKey+"/CacheDataMaxArrLen", QString::number(trcSettings.m_iCacheDataMaxArrLen) );
            m_pSettingsFile->setValue( strSettingsKey+"/AdminObjFileAbsFilePath", trcSettings.m_strAdminObjFileAbsFilePath );
            m_pSettingsFile->setValue( strSettingsKey+"/UseLocalTrcFile", bool2Str(trcSettings.m_bUseLocalTrcFile) );
            m_pSettingsFile->setValue( strSettingsKey+"/LocalTrcFileAbsFilePath", trcSettings.m_strLocalTrcFileAbsFilePath );
            m_pSettingsFile->setValue( strSettingsKey+"/LocalTrcFileAutoSaveIntervalInMs", trcSettings.m_iLocalTrcFileAutoSaveInterval_ms );
            m_pSettingsFile->setValue( strSettingsKey+"/LocalTrcFileSubFileCountMax", trcSettings.m_iLocalTrcFileSubFileCountMax );
            m_pSettingsFile->setValue( strSettingsKey+"/LocalTrcFileSubFileLineCountMax", trcSettings.m_iLocalTrcFileSubFileLineCountMax );
            m_pSettingsFile->setValue( strSettingsKey+"/LocalTrcFileCloseFileAfterEachWrite", bool2Str(trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite) );

            m_pSettingsFile->sync();

        } // if( m_pTrcServer != nullptr )

        if( m_pTest != nullptr )
        {
            strSettingsKey = "TestSteps";

            m_pSettingsFile->setValue( strSettingsKey+"/FileName", m_pTest->getAdminObjIdxTree()->getFileName() );

            m_pSettingsFile->sync();

        } // if( m_pTest != nullptr )

    } // if( m_pSettingsFile != nullptr )

} // saveSettings

/*==============================================================================
protected slots: // of system startup and system shutdown
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onLastWindowClosed()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onLastWindowClosed",
        /* strAddInfo   */ "" );

    quit();

} // onLastWindowClosed

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onDbCreated( ZS::Data::CDb* i_pDb )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbCreated",
        /* strAddInfo   */ "" );

    m_pDb = i_pDb;

    m_pMainWindow->setDb(m_pDb);

} // onDbCreated

//------------------------------------------------------------------------------
void CApplication::onDbAboutToBeDestroyed( ZS::Data::CDb* /*i_pDb*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbAboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pDb = nullptr;

    m_pMainWindow->setDb(m_pDb);

} // onDbAboutToBeDestroyed

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onDbDataSetCreated( ZS::Data::CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbDataSetCreated",
        /* strAddInfo   */ "" );

    m_pDSDb = i_pDS;

    m_pMainWindow->setDbDataSet(m_pDSDb);

} // onDbDataSetCreated

//------------------------------------------------------------------------------
void CApplication::onDbDataSetAboutToBeDestroyed( ZS::Data::CDataSet* /*i_pDS*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbDataSetAboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pDSDb = nullptr;

    m_pMainWindow->setDbDataSet(m_pDSDb);

} // onDbDataSetAboutToBeDestroyed

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onDbClt1Created( ZS::Data::CDbClient* i_pDbClt )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt1Created",
        /* strAddInfo   */ "" );

    if( m_pDbClt1 != i_pDbClt )
    {
        m_pDbClt1 = i_pDbClt;
        m_pDSDbClt1 = i_pDbClt->getDataSet();

        m_pMainWindow->setDbClt1(m_pDbClt1);
        m_pMainWindow->setDbClt1DataSet(m_pDSDbClt1);
    }

} // onDbClt1Created

//------------------------------------------------------------------------------
void CApplication::onDbClt1AboutToBeDestroyed( ZS::Data::CDbClient* /*i_pDbClt*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt1AboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pDbClt1 = nullptr;
    m_pDSDbClt1 = nullptr;

    m_pMainWindow->setDbClt1(m_pDbClt1);
    m_pMainWindow->setDbClt1DataSet(m_pDSDbClt1);

} // onDbClt1AboutToBeDestroyed

//------------------------------------------------------------------------------
void CApplication::onDbClt1DataSetCreated( ZS::Data::CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt1DataSetCreated",
        /* strAddInfo   */ "" );

    m_pDSDbClt1 = i_pDS;

    m_pMainWindow->setDbClt1DataSet(m_pDSDbClt1);

} // onDbClt1DataSetCreated

//------------------------------------------------------------------------------
void CApplication::onDbClt1DataSetAboutToBeDestroyed( ZS::Data::CDataSet* /*i_pDS*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt1DataSetAboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pDSDbClt1 = nullptr;

    m_pMainWindow->setDbClt1DataSet(m_pDSDbClt1);

} // onDbDataSetAboutToBeDestroyed

//------------------------------------------------------------------------------
void CApplication::onDbClt2Created( ZS::Data::CDbClient* i_pDbClt )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt2Created",
        /* strAddInfo   */ "" );

    if( m_pDbClt2 != i_pDbClt )
    {
        m_pDbClt2 = i_pDbClt;
        m_pDSDbClt2 = i_pDbClt->getDataSet();

        m_pMainWindow->setDbClt2(m_pDbClt2);
        m_pMainWindow->setDbClt2DataSet(m_pDSDbClt2);
    }

} // onDbClt2Created

//------------------------------------------------------------------------------
void CApplication::onDbClt2AboutToBeDestroyed( ZS::Data::CDbClient* /*i_pDbClt*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt2AboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pDbClt2 = nullptr;
    m_pDSDbClt2 = nullptr;

    m_pMainWindow->setDbClt2(m_pDbClt2);
    m_pMainWindow->setDbClt2DataSet(m_pDSDbClt2);

} // onDbClt2AboutToBeDestroyed

//------------------------------------------------------------------------------
void CApplication::onDbClt2DataSetCreated( ZS::Data::CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt2DataSetCreated",
        /* strAddInfo   */ "" );

    m_pDSDbClt2 = i_pDS;

    m_pMainWindow->setDbClt2DataSet(m_pDSDbClt2);

} // onDbClt2DataSetCreated

//------------------------------------------------------------------------------
void CApplication::onDbClt2DataSetAboutToBeDestroyed( ZS::Data::CDataSet* /*i_pDS*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt2DataSetAboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pDSDbClt2 = nullptr;

    m_pMainWindow->setDbClt2DataSet(m_pDSDbClt2);

} // onDbClt2DataSetAboutToBeDestroyed

//------------------------------------------------------------------------------
void CApplication::onDbClt3Created( ZS::Data::CDbClient* i_pDbClt )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt3Created",
        /* strAddInfo   */ "" );

    if( m_pDbClt3 != i_pDbClt )
    {
        m_pDbClt3 = i_pDbClt;
        m_pDSDbClt3 = i_pDbClt->getDataSet();

        m_pMainWindow->setDbClt3(m_pDbClt3);
        m_pMainWindow->setDbClt3DataSet(m_pDSDbClt3);
    }

} // onDbClt3Created

//------------------------------------------------------------------------------
void CApplication::onDbClt3AboutToBeDestroyed( ZS::Data::CDbClient* /*i_pDbClt*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt3AboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pDbClt3 = nullptr;
    m_pDSDbClt3 = nullptr;

    m_pMainWindow->setDbClt3(m_pDbClt3);
    m_pMainWindow->setDbClt3DataSet(m_pDSDbClt3);

} // onDbClt3AboutToBeDestroyed

//------------------------------------------------------------------------------
void CApplication::onDbClt3DataSetCreated( ZS::Data::CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt3DataSetCreated",
        /* strAddInfo   */ "" );

    m_pDSDbClt3 = i_pDS;

    m_pMainWindow->setDbClt3DataSet(m_pDSDbClt3);

} // onDbClt3DataSetCreated

//------------------------------------------------------------------------------
void CApplication::onDbClt3DataSetAboutToBeDestroyed( ZS::Data::CDataSet* /*i_pDS*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClt3DataSetAboutToBeDestroyed",
        /* strAddInfo   */ "" );

    m_pDSDbClt3 = nullptr;

    m_pMainWindow->setDbClt3DataSet(m_pDSDbClt3);

} // onDbClt3DataSetAboutToBeDestroyed
