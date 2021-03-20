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

#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qsettings.h>
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>
#include <QtGui/qbitmap.h>

#if QT_VERSION < 0x050000
#include <QtGui/qmessagebox.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "App.h"
#include "DatabaseThread.h"
#include "DbClient.h"
#include "DbClientThread.h"
#include "MainWindow.h"
#include "WidgetCentral.h"

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysRequestSequencer.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::AsyncRequests;
using namespace ZS::Apps::Test::AsyncRequests::Database;


/*******************************************************************************
class CApplication : public ZS::System::GUI::CGUIApp
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* enum EState
==============================================================================*/

//------------------------------------------------------------------------------
const SEnumEntry s_arEnumStrStates[CApplication::EStateCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CApplication::EStateCreated, "Created" ),
    /* 1 */ SEnumEntry( CApplication::EStateRunning, "Running" )
};

//------------------------------------------------------------------------------
QString CApplication::State2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str( s_arEnumStrStates, EStateCount, i_iState );
}

/* enum ERequest
==============================================================================*/

const SEnumEntry s_arstrRequests[CApplication::ERequestCount] = {
    /*  0 */ SEnumEntry( CApplication::ERequestNone,                    "None",                    "None"                    ),
    /*  1 */ SEnumEntry( CApplication::ERequestStartup,                 "Startup",                 "Startup"                 ),
    /*  2 */ SEnumEntry( CApplication::ERequestStartupTrcServer,        "StartupTrcServer",        "StartupTrcServer"        ),
    /*  3 */ SEnumEntry( CApplication::ERequestStartupDb,               "StartupDb",               "StartupDb"               ),
    /*  4 */ SEnumEntry( CApplication::ERequestShutdown,                "Shutdown",                "Shutdown"                ),
    /*  5 */ SEnumEntry( CApplication::ERequestShutdownTrcServer,       "ShutdownTrcServer",       "ShutdownTrcServer"       ),
    /*  6 */ SEnumEntry( CApplication::ERequestShutdownDb,              "ShutdownDb",              "ShutdownDb"              ),
    /*  7 */ SEnumEntry( CApplication::ERequestConnect,                 "Connect",                 "Connect"                 ),
    /*  8 */ SEnumEntry( CApplication::ERequestConnectDbClients,        "ConnectDbClients",        "ConnectDbClients"        ),
    /*  9 */ SEnumEntry( CApplication::ERequestConnectDbClientGUI,      "ConnectDbClientGUI",      "ConnectDbClientGUI"      ),
    /* 10 */ SEnumEntry( CApplication::ERequestStartupDbClientSigGen,   "StartupDbClientSigGen",   "StartupDbClientSigGen"   ),
    /* 11 */ SEnumEntry( CApplication::ERequestConnectDbClientSigGen,   "ConnectDbClientSigGen",   "ConnectDbClientSigGen"   ),
    /* 12 */ SEnumEntry( CApplication::ERequestStartupDbClientCapture,  "StartupDbClientCapture",  "StartupDbClientCapture"  ),
    /* 13 */ SEnumEntry( CApplication::ERequestConnectDbClientCapture,  "ConnectDbClientCapture",  "ConnectDbClientCapture"  ),
    /* 14 */ SEnumEntry( CApplication::ERequestDisconnect,              "Disconnect",              "Disconnect"              ),
    /* 15 */ SEnumEntry( CApplication::ERequestDisconnectDbClients,     "DisconnectDbClients",     "DisconnectDbClients"     ),
    /* 16 */ SEnumEntry( CApplication::ERequestDisconnectDbClientGUI,   "DisconnectDbClientGUI",   "DisconnectDbClientGUI"   ),
    /* 17 */ SEnumEntry( CApplication::ERequestShutdownDbClientSigGen,  "ShutdownDbClientSigGen",  "ShutdownDbClientSigGen"  ),
    /* 18 */ SEnumEntry( CApplication::ERequestShutdownDbClientCapture, "ShutdownDbClientCapture", "ShutdownDbClientCapture" ),
    /* 19 */ SEnumEntry( CApplication::ERequestDeleteDbClients,         "DeleteDbClients",         "Deleting Db Clients"     ),
    /* 20 */ SEnumEntry( CApplication::ERequestDeleteDb,                "DeleteDb",                "Deleting Database"       ),
    /* 21 */ SEnumEntry( CApplication::ERequestSelectSchema,            "SelectSchema",            "SelectSchema"            ),
    /* 22 */ SEnumEntry( CApplication::ERequestSaveSchema,              "SaveSchema",              "SaveSchema"              ),
    /* 23 */ SEnumEntry( CApplication::ERequestSelectData,              "SelectData",              "SelectData"              ),
    /* 24 */ SEnumEntry( CApplication::ERequestSaveData,                "SaveData",                "SaveData"                ),
    /* 25 */ SEnumEntry( CApplication::ERequestSet2Default,             "Set2Default",             "Set2Default"             )
};

//------------------------------------------------------------------------------
QString CApplication::Request2Str( int i_iRequest, bool i_bShort )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_bShort )
    {
        str = SEnumEntry::enumerator2Str( s_arstrRequests, _ZSArrLen(s_arstrRequests), i_iRequest, 0 );
    }
    else
    {
        str = SEnumEntry::enumerator2Str( s_arstrRequests, _ZSArrLen(s_arstrRequests), i_iRequest, 1 );
    }
    return str;

} // Request2Str

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
public: // class methods (startup request execution functions)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CApplication::StartupDb( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pApp = dynamic_cast<CApplication*>(i_pObjFctExecute);

    if( pApp != nullptr )
    {
        pReq = pApp->startupDb(i_iReqIdParent);
    }
    return pReq;

} // StartupDb

//------------------------------------------------------------------------------
CRequest* CApplication::ConnectDbClientGUI( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pApp = dynamic_cast<CApplication*>(i_pObjFctExecute);

    if( pApp != nullptr )
    {
        pReq = pApp->connectDbClientGUI(i_iReqIdParent);
    }
    return pReq;

} // ConnectDbClientGUI

//------------------------------------------------------------------------------
CRequest* CApplication::StartupDbClientSigGen( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pApp = dynamic_cast<CApplication*>(i_pObjFctExecute);

    if( pApp != nullptr )
    {
        pReq = pApp->startupDbClientSigGen(i_iReqIdParent);
    }
    return pReq;

} // StartupDbClientSigGen

//------------------------------------------------------------------------------
CRequest* CApplication::ConnectDbClientSigGen( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pApp = dynamic_cast<CApplication*>(i_pObjFctExecute);

    if( pApp != nullptr )
    {
        pReq = pApp->connectDbClientSigGen(i_iReqIdParent);
    }
    return pReq;

} // ConnectDbClientSigGen

//------------------------------------------------------------------------------
CRequest* CApplication::StartupDbClientCapture( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pApp = dynamic_cast<CApplication*>(i_pObjFctExecute);

    if( pApp != nullptr )
    {
        pReq = pApp->startupDbClientCapture(i_iReqIdParent);
    }
    return pReq;

} // StartupDbClientCapture

//------------------------------------------------------------------------------
CRequest* CApplication::ConnectDbClientCapture( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pApp = dynamic_cast<CApplication*>(i_pObjFctExecute);

    if( pApp != nullptr )
    {
        pReq = pApp->connectDbClientCapture(i_iReqIdParent);
    }
    return pReq;

} // ConnectDbClientCapture

/*==============================================================================
public: // class methods (shutdown request execution functions)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CApplication::DisconnectDbClientGUI( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pApp = dynamic_cast<CApplication*>(i_pObjFctExecute);

    if( pApp != nullptr )
    {
        pReq = pApp->disconnectDbClientGUI(i_iReqIdParent);
    }
    return pReq;

} // DisconnectDbClientGUI

//------------------------------------------------------------------------------
CRequest* CApplication::ShutdownDbClientSigGen( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pApp = dynamic_cast<CApplication*>(i_pObjFctExecute);

    if( pApp != nullptr )
    {
        pReq = pApp->shutdownDbClientSigGen(i_iReqIdParent);
    }
    return pReq;

} // ShutdownDbClientSigGen

//------------------------------------------------------------------------------
CRequest* CApplication::ShutdownDbClientCapture( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pApp = dynamic_cast<CApplication*>(i_pObjFctExecute);

    if( pApp != nullptr )
    {
        pReq = pApp->shutdownDbClientCapture(i_iReqIdParent);
    }
    return pReq;

} // ShutdownDbClientCapture

//------------------------------------------------------------------------------
CRequest* CApplication::ShutdownDb( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pApp = dynamic_cast<CApplication*>(i_pObjFctExecute);

    if( pApp != nullptr )
    {
        pReq = pApp->shutdownDb(i_iReqIdParent);
    }
    return pReq;

} // ShutdownDb

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
    m_bReqExecTreeGarbageCollectorEnabled(true),
    m_fReqExecTreeGarbageCollectorInterval_s(5.0),
    m_fReqExecTreeGarbageCollectorElapsed_s(60.0),
    m_pReqExecTree(nullptr),
    m_state(EStateCreated),
    m_pReqSeqrStartup(nullptr),
    m_pReqSeqrShutdown(nullptr),
    m_requestQueue(this,true),
    m_bMsgReqContinuePending(false),
    m_pMainWindow(nullptr),
    m_bDbUseBlockingMethodCalls(false),
    m_bDbReqModeSingleShot(false),
    m_timeoutDb(ETimeoutInfinite),
    m_pDbThread(nullptr),
    m_bDbCltGUIUseBlockingMethodCalls(false),
    m_socketTypeDbCltGUI(ESocketTypeInProcMsg),
    m_timeoutDbCltGUIConnect(ETimeoutInfinite),
    m_timeoutDbCltGUISchema(ETimeoutInfinite),
    m_timeoutDbCltGUIData1(ETimeoutInfinite),
    m_timeoutDbCltGUIData2(ETimeoutInfinite),
    m_timeoutDbCltGUIData3(ETimeoutInfinite),
    m_pDbCltGUI(nullptr),
    m_bDbCltSigGenUseBlockingMethodCalls(false),
    m_socketTypeDbCltSigGen(ESocketTypeInProcMsg),
    m_timeoutDbCltSigGenStartup(ETimeoutInfinite),
    m_timeoutDbCltSigGenConnect(ETimeoutInfinite),
    m_pDbCltThreadSigGen(nullptr),
    m_bDbCltCaptureUseBlockingMethodCalls(false),
    m_socketTypeDbCltCapture(ESocketTypeInProcMsg),
    m_timeoutDbCltCaptureStartup(ETimeoutInfinite),
    m_timeoutDbCltCaptureConnect(ETimeoutInfinite),
    m_pDbCltThreadCapture(nullptr),
    m_trcServerHostSettings(
        /* strLocalHostName       */ "127.0.0.1",
        /* uLocalPort             */ 24763,
        /* uMaxPendingConnections */ 30 ),
    m_trcServerSettings(),
    m_pTrcServer(nullptr),
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

    iconApp.addPixmap(pxmApp16x16);
    iconApp.addPixmap(pxmApp32x32);

    QApplication::setWindowIcon(iconApp);

    SServerHostSettings trcServerHostSettingsDefault = m_trcServerHostSettings;
    STrcServerSettings trcServerSettingsDefault = m_trcServerSettings;

    // Parse command arguments (first part, IniFile)
    //----------------------------------------------

    int         idxArg;
    QString     strArg;
    QString     strVal;
    QStringList strListArgsPar;
    QStringList strListArgsVal;

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

    // Create error manager
    //---------------------

    CErrLog::CreateInstance(true, m_strErrLogFileAbsFilePath);

    // Request Execution Tree
    //------------------------

    m_pReqExecTree = CRequestExecTree::CreateInstance();

    m_pReqExecTree->setGarbageCollectorEnabled(m_bReqExecTreeGarbageCollectorEnabled);
    m_pReqExecTree->setGarbageCollectorIntervalInSec(m_fReqExecTreeGarbageCollectorInterval_s);
    m_pReqExecTree->setGarbageCollectorElapsedInSec(m_fReqExecTreeGarbageCollectorElapsed_s);

    // Create trace server
    //--------------------

    m_pTrcServer = CIpcTrcServer::CreateInstance();

    m_pTrcServer->setHostSettings(m_trcServerHostSettings);
    m_pTrcServer->setTraceSettings(m_trcServerSettings);
    m_pTrcServer->recallAdminObjs();

    m_pTrcServer->startup();

    // Get trace admin object
    //-----------------------

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::AsyncRequests", "CApplication", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // Create database Thread, database client in GUI thread and database client threads
    //-----------------------------------------------------------------------------------

    m_pDbThread = new CDbThread("Db");

    m_pDbThread->setReqModeSingleShot(m_bDbReqModeSingleShot);

    if( !QObject::connect(
        /* pObjSender   */ m_pDbThread,
        /* szSignal     */ SIGNAL(dbCreated(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbCreated(QObject*)),
        /* socketType     */ Qt::DirectConnection ) ) // The signal is emitted in the database thread.
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pDbCltGUI = new CDbClient("DbCltGUI");
    m_pDbCltGUI->setSocketType(m_socketTypeDbCltGUI);

    m_pDbCltThreadSigGen = new CDbClientThread("DbCltSigGen");
    m_pDbCltThreadSigGen->setSocketType(m_socketTypeDbCltSigGen);

    m_pDbCltThreadCapture = new CDbClientThread("DbCltCapture");
    m_pDbCltThreadCapture->setSocketType(m_socketTypeDbCltCapture);

    // Want to trace the sequencer. So it must be created after the trace server.
    // But before the central widget which creates the models accessing the sequencers.
    // In addition the request sequencers must be destroyed before the request queue.
    // On clearing the sequencer the sequence might access (abort) requests
    // which have already been destroyed by the request queue.
    m_pReqSeqrStartup = new CRequestSequencer(
        /* strObjName             */ "ZS::Apps::Test::AsyncRequests::CApp::" + objectName() + "startup",
        /* pObjParent             */ this,
        /* bKeepReqDscrInExecTree */ false );

    m_pReqSeqrShutdown = new CRequestSequencer(
        /* strObjName             */ "ZS::Apps::Test::AsyncRequests::CApp::" + objectName() + ".shutdown",
        /* pObjParent             */ this,
        /* bKeepReqDscrInExecTree */ false );

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

    m_pMainWindow = new CMainWindow(
        /* strWindowTitle   */ i_strWindowTitle,
        /* pSettingsFile    */ m_pSettingsFile,
        /* pReqSeqrStartup  */ m_pReqSeqrStartup,
        /* pReqSeqrShutdown */ m_pReqSeqrShutdown );
    m_pMainWindow->show();

    // <RequestSequencer> Startup
    //----------------------------

    qint64 iReqIdStartupDb = m_pReqSeqrStartup->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestStartupDb,
        /* strRequest               */ Request2Str(ERequestStartupDb),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ StartupDb,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ -1 );

    qint64 iReqIdConnectDbClients = m_pReqSeqrStartup->addGroup(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestConnectDbClients,
        /* strRequest               */ Request2Str(ERequestConnectDbClients),
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdStartupDb );

    /*qint64 iReqIdConnectDbClientGUI =*/ m_pReqSeqrStartup->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestConnectDbClientGUI,
        /* strRequest               */ Request2Str(ERequestConnectDbClientGUI),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ ConnectDbClientGUI,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ iReqIdConnectDbClients,
        /* iReqIdPredecessor        */ -1 );

    qint64 iReqIdStartupDbClientSigGen = m_pReqSeqrStartup->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestStartupDbClientSigGen,
        /* strRequest               */ Request2Str(ERequestStartupDbClientSigGen),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ StartupDbClientSigGen,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ iReqIdConnectDbClients,
        /* iReqIdPredecessor        */ -1 );

    /*qint64 iReqIdConnectDbClientSigGen =*/ m_pReqSeqrStartup->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestConnectDbClientSigGen,
        /* strRequest               */ Request2Str(ERequestConnectDbClientSigGen),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ ConnectDbClientSigGen,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ iReqIdConnectDbClients,
        /* iReqIdPredecessor        */ iReqIdStartupDbClientSigGen );

    qint64 iReqIdStartupDbClientCapture = m_pReqSeqrStartup->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestStartupDbClientCapture,
        /* strRequest               */ Request2Str(ERequestStartupDbClientCapture),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ StartupDbClientCapture,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ iReqIdConnectDbClients,
        /* iReqIdPredecessor        */ -1 );

    /*qint64 iReqIdConnectDbClientCapture =*/ m_pReqSeqrStartup->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestConnectDbClientCapture,
        /* strRequest               */ Request2Str(ERequestConnectDbClientCapture),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ ConnectDbClientCapture,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ iReqIdConnectDbClients,
        /* iReqIdPredecessor        */ iReqIdStartupDbClientCapture );

    // <RequestSequencer> Shutdown
    //----------------------------

    qint64 iReqIdDisconnectDbClients = -1;

    iReqIdDisconnectDbClients = m_pReqSeqrShutdown->addGroup(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestDisconnectDbClients,
        /* strRequest               */ Request2Str(ERequestDisconnectDbClients),
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ -1 );

    /*qint64 iReqIdDisconnectDbClientGUI =*/ m_pReqSeqrShutdown->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestDisconnectDbClientGUI,
        /* strRequest               */ Request2Str(ERequestDisconnectDbClientGUI),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ DisconnectDbClientGUI,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ iReqIdDisconnectDbClients,
        /* iReqIdPredecessor        */ -1 );

    /*qint64 iReqIdDisconnectDbClientSigGen =*/ m_pReqSeqrShutdown->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestShutdownDbClientSigGen,
        /* strRequest               */ Request2Str(ERequestShutdownDbClientSigGen),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ ShutdownDbClientSigGen,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ iReqIdDisconnectDbClients,
        /* iReqIdPredecessor        */ -1 );

    /*qint64 iReqIdDisconnectDbClientCapture =*/ m_pReqSeqrShutdown->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestShutdownDbClientCapture,
        /* strRequest               */ Request2Str(ERequestShutdownDbClientCapture),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ ShutdownDbClientCapture,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ iReqIdDisconnectDbClients,
        /* iReqIdPredecessor        */ -1 );

    /*qint64 iReqIdShutdownDb =*/ m_pReqSeqrShutdown->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CApplication",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestShutdownDb,
        /* strRequest               */ Request2Str(ERequestShutdownDb),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ ShutdownDb,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdDisconnectDbClients );

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

    if( m_pReqSeqrStartup != nullptr && m_pReqSeqrStartup->isInProgress() )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pReqSeqrStartup,
            /* szSignal     */ SIGNAL(finished(qint64)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onStartupFinished(qint64)) );
    }
    if( m_pReqSeqrShutdown != nullptr && m_pReqSeqrShutdown->isInProgress() )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pReqSeqrShutdown,
            /* szSignal     */ SIGNAL(finished(qint64)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onShutdownFinished(qint64)) );
    }

    // Save settings of the application
    //---------------------------------

    m_pTrcServer->saveAdminObjs();

    saveSettings();

    // Destroy objects created and controlled by the application
    //----------------------------------------------------------

    try
    {
        delete m_pDbCltGUI;
    }
    catch(...)
    {
    }
    m_pDbCltGUI = nullptr;

    try
    {
        delete m_pDbCltThreadSigGen;
    }
    catch(...)
    {
    }
    m_pDbCltThreadSigGen = nullptr;

    try
    {
        delete m_pDbCltThreadCapture;
    }
    catch(...)
    {
    }
    m_pDbCltThreadCapture = nullptr;

    try
    {
        delete m_pDbThread;
    }
    catch(...)
    {
    }
    m_pDbThread = nullptr;

    // The request sequencers must be destroyed before the request queue.
    // On clearing the sequencer the sequence might access (abort) requests
    // which have already been destroyed by the request queue.

    try
    {
        delete m_pReqSeqrStartup;
    }
    catch(...)
    {
    }
    m_pReqSeqrStartup = nullptr;

    try
    {
        delete m_pReqSeqrShutdown;
    }
    catch(...)
    {
    }
    m_pReqSeqrShutdown = nullptr;

    try
    {
        delete m_pMainWindow;
    }
    catch(...)
    {
    }
    m_pMainWindow = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    // Please note that the trace server must be deleted after the application has been
    // destroyed and the main event loop has been quit. There might still be events (messages)
    // pending while the application is quit whose dtor will be called. If the message dtor
    // wants to output traces the server must still be alive. Otherwise the trace admin
    // objects referenced by the message dtor would have already been destroyed before the dtor
    // of the event (message) is called. This is why the trace server should be created before
    // the application in the main module and should be destroyed after the application has
    // been destroyed in the main module. But the trace server may also be created in the ctor
    // of the application (but should still be destroyed in main module).

    try
    {
        delete m_pSettingsFile;
    }
    catch(...)
    {
    }
    m_pSettingsFile = nullptr;

    CRequestExecTree::DestroyInstance();

    CErrLog::DestroyInstance();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::readSettings()
//------------------------------------------------------------------------------
{
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

        // Database
        //---------

        strSettingsKey = "Database";
        bSyncSettings  = false;

        if( m_pSettingsFile->contains(strSettingsKey+"/UseBlockingMethodCalls") )
        {
            m_bDbUseBlockingMethodCalls = str2Bool( m_pSettingsFile->value(strSettingsKey+"/UseBlockingMethodCalls",bool2Str(m_bDbUseBlockingMethodCalls)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/UseBlockingMethodCalls",bool2Str(m_bDbUseBlockingMethodCalls));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/ReqModeSingleShot") )
        {
            m_bDbReqModeSingleShot = str2Bool( m_pSettingsFile->value(strSettingsKey+"/ReqModeSingleShot",bool2Str(m_bDbReqModeSingleShot)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/ReqModeSingleShot",bool2Str(m_bDbReqModeSingleShot));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/Timeout") )
        {
            m_timeoutDb = str2Timeout( m_pSettingsFile->value(strSettingsKey+"/Timeout",timeout2Str(m_timeoutDb)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/Timeout",timeout2Str(m_timeoutDb));
            bSyncSettings = true;
        }

        // DatabaseClientGUI
        //------------------

        strSettingsKey = "DatabaseClientGUI";

        if( m_pSettingsFile->contains(strSettingsKey+"/UseBlockingMethodCalls") )
        {
            m_bDbCltGUIUseBlockingMethodCalls = str2Bool( m_pSettingsFile->value(strSettingsKey+"/UseBlockingMethodCalls",bool2Str(m_bDbCltGUIUseBlockingMethodCalls)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/UseBlockingMethodCalls",bool2Str(m_bDbCltGUIUseBlockingMethodCalls));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/ConnectionType") )
        {
            m_socketTypeDbCltGUI = str2SocketType( m_pSettingsFile->value(strSettingsKey+"/ConnectionType",socketType2Str(m_socketTypeDbCltGUI)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/ConnectionType",socketType2Str(m_socketTypeDbCltGUI));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/TimeoutConnect") )
        {
            m_timeoutDbCltGUIConnect = str2Timeout( m_pSettingsFile->value(strSettingsKey+"/TimeoutConnect",timeout2Str(m_timeoutDbCltGUIConnect)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/TimeoutConnect",timeout2Str(m_timeoutDbCltGUIConnect));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/TimeoutSchema") )
        {
            m_timeoutDbCltGUISchema = str2Timeout( m_pSettingsFile->value(strSettingsKey+"/TimeoutSchema",timeout2Str(m_timeoutDbCltGUISchema)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/TimeoutSchema",timeout2Str(m_timeoutDbCltGUISchema));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/TimeoutData1") )
        {
            m_timeoutDbCltGUIData1 = str2Timeout( m_pSettingsFile->value(strSettingsKey+"/TimeoutData1",timeout2Str(m_timeoutDbCltGUIData1)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/TimeoutData1",timeout2Str(m_timeoutDbCltGUIData1));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/TimeoutData2") )
        {
            m_timeoutDbCltGUIData2 = str2Timeout( m_pSettingsFile->value(strSettingsKey+"/TimeoutData2",timeout2Str(m_timeoutDbCltGUIData2)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/TimeoutData2",timeout2Str(m_timeoutDbCltGUIData2));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/TimeoutData3") )
        {
            m_timeoutDbCltGUIData3 = str2Timeout( m_pSettingsFile->value(strSettingsKey+"/TimeoutData3",timeout2Str(m_timeoutDbCltGUIData3)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/TimeoutData3",timeout2Str(m_timeoutDbCltGUIData3));
            bSyncSettings = true;
        }

        // DatabaseClientSigGen
        //---------------------

        strSettingsKey = "DatabaseClientSigGen";

        if( m_pSettingsFile->contains(strSettingsKey+"/UseBlockingMethodCalls") )
        {
            m_bDbCltSigGenUseBlockingMethodCalls = str2Bool( m_pSettingsFile->value(strSettingsKey+"/UseBlockingMethodCalls",bool2Str(m_bDbCltSigGenUseBlockingMethodCalls)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/UseBlockingMethodCalls",bool2Str(m_bDbCltSigGenUseBlockingMethodCalls));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/ConnectionType") )
        {
            m_socketTypeDbCltSigGen = str2SocketType( m_pSettingsFile->value(strSettingsKey+"/ConnectionType",socketType2Str(m_socketTypeDbCltSigGen)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/ConnectionType",socketType2Str(m_socketTypeDbCltSigGen));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/TimeoutStartup") )
        {
            m_timeoutDbCltSigGenStartup = str2Timeout( m_pSettingsFile->value(strSettingsKey+"/TimeoutStartup",timeout2Str(m_timeoutDbCltSigGenStartup)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/TimeoutStartup",timeout2Str(m_timeoutDbCltSigGenStartup));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/TimeoutConnect") )
        {
            m_timeoutDbCltSigGenConnect = str2Timeout( m_pSettingsFile->value(strSettingsKey+"/TimeoutConnect",timeout2Str(m_timeoutDbCltSigGenConnect)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/TimeoutDbCltSigGenConnect",timeout2Str(m_timeoutDbCltSigGenConnect));
            bSyncSettings = true;
        }

        // DatabaseClientCapture
        //----------------------

        strSettingsKey = "DatabaseClientCapture";

        if( m_pSettingsFile->contains(strSettingsKey+"/UseBlockingMethodCalls") )
        {
            m_bDbCltCaptureUseBlockingMethodCalls = str2Bool( m_pSettingsFile->value(strSettingsKey+"/UseBlockingMethodCalls",bool2Str(m_bDbCltCaptureUseBlockingMethodCalls)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/UseBlockingMethodCalls",bool2Str(m_bDbCltCaptureUseBlockingMethodCalls));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/ConnectionType") )
        {
            m_socketTypeDbCltCapture = str2SocketType( m_pSettingsFile->value(strSettingsKey+"/ConnectionType",socketType2Str(m_socketTypeDbCltCapture)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/ConnectionType",socketType2Str(m_socketTypeDbCltCapture));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/TimeoutStartup") )
        {
            m_timeoutDbCltCaptureStartup = str2Timeout( m_pSettingsFile->value(strSettingsKey+"/TimeoutStartup",timeout2Str(m_timeoutDbCltCaptureStartup)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/TimeoutStartup",timeout2Str(m_timeoutDbCltCaptureStartup));
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/TimeoutConnect") )
        {
            m_timeoutDbCltCaptureConnect = str2Timeout( m_pSettingsFile->value(strSettingsKey+"/TimeoutConnect",timeout2Str(m_timeoutDbCltCaptureConnect)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue(strSettingsKey+"/TimeoutConnect",timeout2Str(m_timeoutDbCltCaptureConnect));
            bSyncSettings = true;
        }

        if( bSyncSettings )
        {
            m_pSettingsFile->sync();
        }
    }

} // readSettings

//------------------------------------------------------------------------------
void CApplication::saveSettings()
//------------------------------------------------------------------------------
{
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

        strSettingsKey = "Database";

        m_pSettingsFile->setValue( strSettingsKey+"/UseBlockingMethodCalls", bool2Str(m_bDbUseBlockingMethodCalls) );
        m_pSettingsFile->setValue( strSettingsKey+"/ReqModeSingleShot", bool2Str(m_bDbReqModeSingleShot) );
        m_pSettingsFile->setValue( strSettingsKey+"/Timeout", timeout2Str(m_timeoutDb) );

        strSettingsKey = "DatabaseClientGUI";

        m_pSettingsFile->setValue( strSettingsKey+"/UseBlockingMethodCalls", bool2Str(m_bDbCltGUIUseBlockingMethodCalls) );
        m_pSettingsFile->setValue( strSettingsKey+"/ConnectionType", socketType2Str(m_socketTypeDbCltGUI) );
        m_pSettingsFile->setValue( strSettingsKey+"/TimeoutConnect", timeout2Str(m_timeoutDbCltGUIConnect) );
        m_pSettingsFile->setValue( strSettingsKey+"/TimeoutSchema", timeout2Str(m_timeoutDbCltGUISchema) );
        m_pSettingsFile->setValue( strSettingsKey+"/TimeoutData1", timeout2Str(m_timeoutDbCltGUIData1) );
        m_pSettingsFile->setValue( strSettingsKey+"/TimeoutData2", timeout2Str(m_timeoutDbCltGUIData2) );
        m_pSettingsFile->setValue( strSettingsKey+"/TimeoutData3", timeout2Str(m_timeoutDbCltGUIData3) );

        strSettingsKey = "DatabaseClientSigGen";

        m_pSettingsFile->setValue( strSettingsKey+"/UseBlockingMethodCalls", bool2Str(m_bDbCltSigGenUseBlockingMethodCalls) );
        m_pSettingsFile->setValue( strSettingsKey+"/ConnectionType", socketType2Str(m_socketTypeDbCltSigGen) );
        m_pSettingsFile->setValue( strSettingsKey+"/TimeoutStartup", timeout2Str(m_timeoutDbCltSigGenStartup) );
        m_pSettingsFile->setValue( strSettingsKey+"/TimeoutConnect", timeout2Str(m_timeoutDbCltSigGenConnect) );

        strSettingsKey = "DatabaseClientCapture";

        m_pSettingsFile->setValue( strSettingsKey+"/UseBlockingMethodCalls", bool2Str(m_bDbCltCaptureUseBlockingMethodCalls) );
        m_pSettingsFile->setValue( strSettingsKey+"/ConnectionType", socketType2Str(m_socketTypeDbCltCapture) );
        m_pSettingsFile->setValue( strSettingsKey+"/TimeoutStartup", timeout2Str(m_timeoutDbCltCaptureStartup) );
        m_pSettingsFile->setValue( strSettingsKey+"/TimeoutConnect", timeout2Str(m_timeoutDbCltCaptureConnect) );

        m_pSettingsFile->sync();
    }

} // saveSettings

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CApplication::startup()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startup",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CApplication",
        /* iRequest     */ ERequestStartup,
        /* strRequest   */ Request2Str(ERequestStartup),
        /* iReqIdParent */ -1,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) ..
    if( m_requestQueue.isIdle() )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        m_requestQueue.postponeRequest(pReq);
    }
    else
    {
        executeStartupRequest(pReq);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( pReq == nullptr )
        {
            strAddTrcInfo = "Req: nullptr";
        }
        else
        {
            strAddTrcInfo = "Req: " + pReq->getAddTrcInfoStr( iAddTrcInfoDetailLevel );
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // startup

//------------------------------------------------------------------------------
CRequest* CApplication::shutdown()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "shutdown",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CApplication",
        /* iRequest     */ ERequestShutdown,
        /* strRequest   */ Request2Str(ERequestShutdown),
        /* iReqIdParent */ -1,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        m_requestQueue.postponeRequest(pReq);
    }
    else
    {
        executeShutdownRequest(pReq);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( pReq == nullptr )
        {
            strAddTrcInfo = "Req: nullptr";
        }
        else
        {
            strAddTrcInfo = "Req: " + pReq->getAddTrcInfoStr( iAddTrcInfoDetailLevel );
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // shutdown

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CApplication::useBlockingMethodCalls( const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    bool bUse = false;

    if( i_strObjName == c_strObjNameDb )
    {
        bUse = m_bDbUseBlockingMethodCalls;
    }
    else if( i_strObjName == c_strObjNameDbCltGUI )
    {
        bUse = m_bDbCltGUIUseBlockingMethodCalls;
    }
    else if( i_strObjName == c_strObjNameDbCltSigGen )
    {
        bUse = m_bDbCltSigGenUseBlockingMethodCalls;
    }
    else if( i_strObjName == c_strObjNameDbCltCapture )
    {
        bUse = m_bDbCltCaptureUseBlockingMethodCalls;
    }
    return bUse;

} // useBlockingMethodCalls

//------------------------------------------------------------------------------
void CApplication::setUseBlockingMethodCalls( bool i_bUse, const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    if( i_strObjName == c_strObjNameDb )
    {
        m_bDbUseBlockingMethodCalls = i_bUse;
    }
    else if( i_strObjName == c_strObjNameDbCltGUI )
    {
        m_bDbCltGUIUseBlockingMethodCalls = i_bUse;
    }
    else if( i_strObjName == c_strObjNameDbCltSigGen )
    {
        m_bDbCltSigGenUseBlockingMethodCalls = i_bUse;
    }
    else if( i_strObjName == c_strObjNameDbCltCapture )
    {
        m_bDbCltCaptureUseBlockingMethodCalls = i_bUse;
    }

    saveSettings();

} // setUseBlockingMethodCalls

/*==============================================================================
public slots: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::setDbReqModeSingleShot( bool i_bModeSingleShot )
//------------------------------------------------------------------------------
{
    // In a real application the database should not be accessed directly.
    // As it is hosted in a thread it might be destroyed while accessing it.
    m_bDbReqModeSingleShot = i_bModeSingleShot;

    if( m_pDbThread != nullptr )
    {
        m_pDbThread->setReqModeSingleShot(m_bDbReqModeSingleShot);
    }

} // setDbReqModeSingleShot

//------------------------------------------------------------------------------
void CApplication::executeDbReqSingleShot()
//------------------------------------------------------------------------------
{
    // In a real application the database should not be accessed directly.
    // As it is hosted in a thread it might be destroyed while accessing it.
    if( m_pDbThread != nullptr && m_pDbThread->getDb() != nullptr )
    {
        m_pDbThread->getDb()->executeReqSingleShot();
    }

} // executeDbReqSingleShot

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ETimeout CApplication::getTimeout( const QString& i_strObjName, const QString& i_strRequests ) const
//------------------------------------------------------------------------------
{
    ETimeout timeout = ETimeoutInfinite;

    if( i_strObjName == c_strObjNameDb )
    {
        timeout = m_timeoutDb;
    }
    else if( i_strObjName == c_strObjNameDbCltGUI )
    {
        if( i_strRequests == "Connect" )
        {
            timeout = m_timeoutDbCltGUIConnect;
        }
        else if( i_strRequests == "Schema" )
        {
            timeout = m_timeoutDbCltGUISchema;
        }
        else if( i_strRequests == "Data1" )
        {
            timeout = m_timeoutDbCltGUIData1;
        }
        else if( i_strRequests == "Data2" )
        {
            timeout = m_timeoutDbCltGUIData2;
        }
        else if( i_strRequests == "Data3" )
        {
            timeout = m_timeoutDbCltGUIData3;
        }
    }
    else if( i_strObjName == c_strObjNameDbCltSigGen )
    {
        if( i_strRequests == "Startup" )
        {
            timeout = m_timeoutDbCltSigGenStartup;
        }
        else if( i_strRequests == "Connect" )
        {
            timeout = m_timeoutDbCltSigGenConnect;
        }
    }
    else if( i_strObjName == c_strObjNameDbCltCapture )
    {
        if( i_strRequests == "Startup" )
        {
            timeout = m_timeoutDbCltCaptureStartup;
        }
        else if( i_strRequests == "Connect" )
        {
            timeout = m_timeoutDbCltCaptureConnect;
        }
    }

    return timeout;

} // getTimeout

//------------------------------------------------------------------------------
void CApplication::setTimeout( ETimeout i_timeout, const QString& i_strObjName, const QString& i_strRequests )
//------------------------------------------------------------------------------
{
    if( i_strObjName == c_strObjNameDb )
    {
        m_timeoutDb = i_timeout;
    }
    else if( i_strObjName == c_strObjNameDbCltGUI )
    {
        if( i_strRequests == "Connect" )
        {
            m_timeoutDbCltGUIConnect = i_timeout;
        }
        else if( i_strRequests == "Schema" )
        {
            m_timeoutDbCltGUISchema = i_timeout;
        }
        else if( i_strRequests == "Data1" )
        {
            m_timeoutDbCltGUIData1 = i_timeout;
        }
        else if( i_strRequests == "Data2" )
        {
            m_timeoutDbCltGUIData2 = i_timeout;
        }
        else if( i_strRequests == "Data3" )
        {
            m_timeoutDbCltGUIData3 = i_timeout;
        }
    }
    else if( i_strObjName == c_strObjNameDbCltSigGen )
    {
        if( i_strRequests == "Startup" )
        {
            m_timeoutDbCltSigGenStartup = i_timeout;
        }
        else if( i_strRequests == "Connect" )
        {
            m_timeoutDbCltSigGenConnect = i_timeout;
        }
    }
    else if( i_strObjName == c_strObjNameDbCltCapture )
    {
        if( i_strRequests == "Startup" )
        {
            m_timeoutDbCltCaptureStartup = i_timeout;
        }
        else if( i_strRequests == "Connect" )
        {
            m_timeoutDbCltCaptureConnect = i_timeout;
        }
    }

} // setTimeout

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ESocketType CApplication::getSocketType( const QString& i_strObjName ) const
//------------------------------------------------------------------------------
{
    ESocketType socketType = ESocketTypeUndefined;

    if( i_strObjName == c_strObjNameDbCltGUI )
    {
        socketType = m_socketTypeDbCltGUI;
    }
    else if( i_strObjName == c_strObjNameDbCltSigGen )
    {
        socketType = m_socketTypeDbCltSigGen;
    }
    else if( i_strObjName == c_strObjNameDbCltCapture )
    {
        socketType = m_socketTypeDbCltCapture;
    }

    return socketType;

} // getSocketType

//------------------------------------------------------------------------------
void CApplication::setSocketType( ESocketType i_socketType, const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    if( i_strObjName == c_strObjNameDbCltGUI )
    {
        if( m_socketTypeDbCltGUI != i_socketType )
        {
            m_socketTypeDbCltGUI = i_socketType;

            if( m_pDbCltGUI != nullptr )
            {
                m_pDbCltGUI->setSocketType(m_socketTypeDbCltGUI);
                m_pDbCltGUI->changeSettings( getTimeoutInMs(m_timeoutDbCltGUIConnect), m_bDbCltGUIUseBlockingMethodCalls, -1 );
            }
        }
    }
    else if( i_strObjName == c_strObjNameDbCltSigGen )
    {
        if( m_socketTypeDbCltSigGen != i_socketType )
        {
            m_socketTypeDbCltSigGen = i_socketType;

            if( m_pDbCltThreadSigGen != nullptr )
            {
                m_pDbCltThreadSigGen->setSocketType(m_socketTypeDbCltSigGen);
                m_pDbCltThreadSigGen->changeSettings( getTimeoutInMs(m_timeoutDbCltSigGenConnect), m_bDbCltGUIUseBlockingMethodCalls, -1 );
            }
        }
    }
    else if( i_strObjName == c_strObjNameDbCltCapture )
    {
        if( m_socketTypeDbCltCapture != i_socketType )
        {
            m_socketTypeDbCltCapture = i_socketType;

            if( m_pDbCltThreadCapture != nullptr )
            {
                m_pDbCltThreadCapture->setSocketType(m_socketTypeDbCltCapture);
                m_pDbCltThreadCapture->changeSettings( getTimeoutInMs(m_timeoutDbCltCaptureConnect), m_bDbCltGUIUseBlockingMethodCalls, -1 );
            }
        }
    }

} // setSocketType

/*==============================================================================
public slots: // instance methods of system shutdown
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onLastWindowClosed()
//------------------------------------------------------------------------------
{
    quit();
}

/*==============================================================================
protected: // instance methods of system startup
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CApplication::startupDb( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startupDb",
        /* strAddInfo   */ strAddTrcInfo );

    // Internal request method which is called
    // 1. by the startup request sequencer or
    // 2. if "startup("Db")" is explicitly called.
    // In the first case the end of the whole sequence is signaled via "onStartupFinished".
    // In the second case the end of the request is signaled via the requests "changed"
    // signal and the caller (theApp itself within method "startup("Db"))
    // must connect a "onRequestChanged" slot to the "changed" signal.

    CRequest* pReq = nullptr;

    if( m_pDbThread != nullptr )
    {
        m_pDbThread->setReqModeSingleShot(m_bDbReqModeSingleShot);

        int  iTimeout_ms = getTimeoutInMs(m_timeoutDb);
        bool bWait = m_bDbUseBlockingMethodCalls;

        pReq = m_pDbThread->startup(iTimeout_ms,bWait,i_iReqIdParent);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( pReq == nullptr )
        {
            strAddTrcInfo = "Req: nullptr";
        }
        else
        {
            strAddTrcInfo = "Req: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // startupDb

//------------------------------------------------------------------------------
CRequest* CApplication::connectDbClientGUI( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "connectDbClientGUI",
        /* strAddInfo   */ strAddTrcInfo );

    // Internal request method which is called
    // 1. by the startup request sequencer or
    // 2. if "connect("DbClientGUI")" is explicitly called.
    // In the first case the end of the whole sequence is signaled via "onStartupFinished".
    // In the second case the end of the request is signaled via the requests "changed"
    // signal and the caller (theApp itself within method "connect("DbClientGUI"))
    // must connect a "onRequestChanged" slot to the "changed" signal.

    CRequest* pReq = nullptr;

    if( m_pDbCltGUI != nullptr )
    {
        int  iTimeout_ms = getTimeoutInMs(m_timeoutDbCltGUIConnect);
        bool bWait = m_bDbCltGUIUseBlockingMethodCalls;

        m_pDbCltGUI->setSocketType(m_socketTypeDbCltGUI);

        pReq = m_pDbCltGUI->connect_(iTimeout_ms, bWait, i_iReqIdParent);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( pReq == nullptr )
        {
            strAddTrcInfo = "Req: nullptr";
        }
        else
        {
            strAddTrcInfo = "Req: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // connectDbClientGUI

//------------------------------------------------------------------------------
CRequest* CApplication::startupDbClientSigGen( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startupDbClientSigGen",
        /* strAddInfo   */ strAddTrcInfo );

    // Internal request method which is called
    // 1. by the startup request sequencer or
    // 2. if "startup("DbClientSigGen")" is explicitly called.
    // In the first case the end of the whole sequence is signaled via "onStartupFinished".
    // In the second case the end of the request is signaled via the requests "changed"
    // signal and the caller (theApp itself within method "connect("DbClientSigGen"))
    // must connect a "onRequestChanged" slot to the "changed" signal.

    CRequest* pReq = nullptr;

    if( m_pDbCltThreadSigGen != nullptr )
    {
        int  iTimeout_ms = getTimeoutInMs(m_timeoutDbCltSigGenStartup);
        bool bWait = m_bDbCltSigGenUseBlockingMethodCalls;

        pReq = m_pDbCltThreadSigGen->startup(iTimeout_ms,bWait,i_iReqIdParent);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( pReq == nullptr )
        {
            strAddTrcInfo = "Req: nullptr";
        }
        else
        {
            strAddTrcInfo = "Req: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // startupDbClientSigGen

//------------------------------------------------------------------------------
CRequest* CApplication::connectDbClientSigGen( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "connectDbClientSigGen",
        /* strAddInfo   */ strAddTrcInfo );

    // Internal request method which is called
    // 1. by the startup request sequencer or
    // 2. if "connect("DbClientSigGen")" is explicitly called.
    // In the first case the end of the whole sequence is signaled via "onStartupFinished".
    // In the second case the end of the request is signaled via the requests "changed"
    // signal and the caller (theApp itself within method "connect("DbClientSigGen"))
    // must connect a "onRequestChanged" slot to the "changed" signal.

    CRequest* pReq = nullptr;

    if( m_pDbCltThreadSigGen != nullptr )
    {
        int  iTimeout_ms = getTimeoutInMs(m_timeoutDbCltSigGenConnect);
        bool bWait = m_bDbCltSigGenUseBlockingMethodCalls;

        m_pDbCltThreadSigGen->setSocketType(m_socketTypeDbCltSigGen);

        pReq = m_pDbCltThreadSigGen->connect_(iTimeout_ms, bWait, i_iReqIdParent);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( pReq == nullptr )
        {
            strAddTrcInfo = "Req: nullptr";
        }
        else
        {
            strAddTrcInfo = "Req: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // connectDbClientSigGen

//------------------------------------------------------------------------------
CRequest* CApplication::startupDbClientCapture( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startupDbClientCapture",
        /* strAddInfo   */ strAddTrcInfo );

    // Internal request method which is called
    // 1. by the startup request sequencer or
    // 2. if "startup("DbClientCapture")" is explicitly called.
    // In the first case the end of the whole sequence is signaled via "onStartupFinished".
    // In the second case the end of the request is signaled via the requests "changed"
    // signal and the caller (theApp itself within method "connect("DbClientCapture"))
    // must connect a "onRequestChanged" slot to the "changed" signal.

    CRequest* pReq = nullptr;

    if( m_pDbCltThreadCapture != nullptr )
    {
        int  iTimeout_ms = getTimeoutInMs(m_timeoutDbCltCaptureStartup);
        bool bWait = m_bDbCltCaptureUseBlockingMethodCalls;

        pReq = m_pDbCltThreadCapture->startup(iTimeout_ms,bWait,i_iReqIdParent);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( pReq == nullptr )
        {
            strAddTrcInfo = "Req: nullptr";
        }
        else
        {
            strAddTrcInfo = "Req: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // startupDbClientCapture

//------------------------------------------------------------------------------
CRequest* CApplication::connectDbClientCapture( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "connectDbClientCapture",
        /* strAddInfo   */ strAddTrcInfo );

    // Internal request method which is called
    // 1. by the startup request sequencer or
    // 2. if "connect("DbClientCapture")" is explicitly called.
    // In the first case the end of the whole sequence is signaled via "onStartupFinished".
    // In the second case the end of the request is signaled via the requests "changed"
    // signal and the caller (theApp itself within method "connect("DbClientCapture"))
    // must connect a "onRequestChanged" slot to the "changed" signal.

    CRequest* pReq = nullptr;

    if( m_pDbCltThreadCapture != nullptr )
    {
        int  iTimeout_ms = getTimeoutInMs(m_timeoutDbCltCaptureConnect);
        bool bWait = m_bDbCltCaptureUseBlockingMethodCalls;

        m_pDbCltThreadCapture->setSocketType(m_socketTypeDbCltCapture);

        pReq = m_pDbCltThreadCapture->connect_(iTimeout_ms, bWait, i_iReqIdParent);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( pReq == nullptr )
        {
            strAddTrcInfo = "Req: nullptr";
        }
        else
        {
            strAddTrcInfo = "Req: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // connectDbClientCapture

/*==============================================================================
public: // class methods (shutdown request execution functions)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CApplication::disconnectDbClientGUI( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "disconnectDbClientGUI",
        /* strAddInfo   */ strAddTrcInfo );

    // Internal request method which is called
    // 1. by the shutdown request sequencer or
    // 2. if "disconnect("DbClientGUI")" is explicitly called.
    // In the first case the end of the whole sequence is signaled via "onShutdownFinished".
    // In the second case the end of the request is signaled via the requests "changed"
    // signal and the caller (theApp itself within method "disconnect("DbClientGUI"))
    // must connect a "onRequestChanged" slot to the "changed" signal.

    CRequest* pReq = nullptr;

    if( m_pDbCltGUI != nullptr )
    {
        int  iTimeout_ms = getTimeoutInMs(m_timeoutDbCltGUIConnect);
        bool bWait = m_bDbCltGUIUseBlockingMethodCalls;

        pReq = m_pDbCltGUI->disconnect_(iTimeout_ms,bWait,i_iReqIdParent);
    }
    return pReq;

} // disconnectDbClientGUI

//------------------------------------------------------------------------------
CRequest* CApplication::shutdownDbClientSigGen( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "shutdownDbClientSigGen",
        /* strAddInfo   */ strAddTrcInfo );

    // Internal request method which is called
    // 1. by the shutdown request sequencer or
    // 2. if "disconnect("DbClientSigGen")" is explicitly called.
    // In the first case the end of the whole sequence is signaled via "onShutdownFinished".
    // In the second case the end of the request is signaled via the requests "changed"
    // signal and the caller (theApp itself within method "disconnect("DbClientSigGen"))
    // must connect a "onRequestChanged" slot to the "changed" signal.

    CRequest* pReq = nullptr;

    if( m_pDbCltThreadSigGen != nullptr )
    {
        int  iTimeout_ms = getTimeoutInMs(m_timeoutDbCltSigGenStartup);
        bool bWait = m_bDbCltSigGenUseBlockingMethodCalls;

        pReq = m_pDbCltThreadSigGen->shutdown(iTimeout_ms,bWait,i_iReqIdParent);
    }
    return pReq;

} // shutdownDbClientSigGen

//------------------------------------------------------------------------------
CRequest* CApplication::shutdownDbClientCapture( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "shutdownDbClientCapture",
        /* strAddInfo   */ strAddTrcInfo );

    // Internal request method which is called
    // 1. by the shutdown request sequencer or
    // 2. if "disconnect("DbClientCapture")" is explicitly called.
    // In the first case the end of the whole sequence is signaled via "onShutdownFinished".
    // In the second case the end of the request is signaled via the requests "changed"
    // signal and the caller (theApp itself within method "disconnect("DbClientCapture"))
    // must connect a "onRequestChanged" slot to the "changed" signal.

    CRequest* pReq = nullptr;

    if( m_pDbCltThreadCapture != nullptr )
    {
        int  iTimeout_ms = getTimeoutInMs(m_timeoutDbCltCaptureStartup);
        bool bWait = m_bDbCltCaptureUseBlockingMethodCalls;

        pReq = m_pDbCltThreadCapture->shutdown(iTimeout_ms,bWait,i_iReqIdParent);
    }
    return pReq;

} // shutdownDbClientCapture

//------------------------------------------------------------------------------
CRequest* CApplication::shutdownDb( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "shutdownDb",
        /* strAddInfo   */ strAddTrcInfo );

    // Internal request method which is called
    // 1. by the shutdown request sequencer or
    // 2. if "shutdown("Db")" is explicitly called.
    // In the first case the end of the whole sequence is signaled via "onShutdownFinished".
    // In the second case the end of the request is signaled via the requests "changed"
    // signal and the caller (theApp itself within method "shutdown("Db"))
    // must connect a "onRequestChanged" slot to the "changed" signal.

    CRequest* pReq = nullptr;

    if( m_pDbThread != nullptr )
    {
        int  iTimeout_ms = getTimeoutInMs(m_timeoutDb);
        bool bWait = m_bDbUseBlockingMethodCalls;

        pReq = m_pDbThread->shutdown(iTimeout_ms,bWait,i_iReqIdParent);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( pReq == nullptr )
        {
            strAddTrcInfo = "Req: nullptr";
        }
        else
        {
            strAddTrcInfo = "Req: " + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // shutdownDb

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onStartupFinished( qint64 i_iReqId )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqId: " + QString::number(i_iReqId);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onStartupFinished",
        /* strAddInfo   */ strAddTrcInfo );

    if( !m_requestQueue.isRequestInProgress(i_iReqId) )
    {
        throw CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }

    if( !QObject::disconnect(
        /* pObjSender   */ m_pReqSeqrStartup,
        /* szSignal     */ SIGNAL(finished(qint64)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onStartupFinished(qint64)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    CRequest* pReq = m_requestQueue.takeRequestInProgress();

    SErrResultInfo errResultInfo = pReq->getErrResultInfo();

    EState statePrev = m_state;

    if( errResultInfo.getResult() == EResultUndefined )
    {
        errResultInfo.setErrResult(ErrResultSuccess);
    }
    if( !errResultInfo.isErrorResult() )
    {
        m_state = EStateRunning;
    }

    // Lock Queue before emitting any signals so that new requests triggered as reentries will be postponed.
    m_requestQueue.lock();

    if( m_state != statePrev )
    {
        emit stateChanged( m_state, statePrev );
    }

    pReq->setErrResultInfo(errResultInfo);
    pReq->setProgressInPerCent(100);
    pReq->lock();
    pReq->update();
    pReq->unlock();
    deleteRequest(pReq);
    pReq = nullptr;

    m_requestQueue.unlock();

    if( m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // onStartupFinished

//------------------------------------------------------------------------------
void CApplication::onShutdownFinished( qint64 i_iReqId )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqId: " + QString::number(i_iReqId);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onShutdownFinished",
        /* strAddInfo   */ strAddTrcInfo );

    if( !m_requestQueue.isRequestInProgress(i_iReqId) )
    {
        throw CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }

    if( !QObject::disconnect(
        /* pObjSender   */ m_pReqSeqrShutdown,
        /* szSignal     */ SIGNAL(finished(qint64)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onShutdownFinished(qint64)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    CRequest* pReq = m_requestQueue.takeRequestInProgress();

    SErrResultInfo errResultInfo = pReq->getErrResultInfo();

    EState statePrev = m_state;

    if( errResultInfo.getResult() == EResultUndefined )
    {
        errResultInfo.setErrResult(ErrResultSuccess);
    }
    if( !errResultInfo.isErrorResult() )
    {
        m_state = EStateCreated;
    }

    // Lock Queue before emitting any signals so that new requests triggered as reentries will be postponed.
    m_requestQueue.lock();

    if( m_state != statePrev )
    {
        emit stateChanged( m_state, statePrev );
    }

    pReq->setErrResultInfo(errResultInfo);
    pReq->setProgressInPerCent(100);
    pReq->lock();
    pReq->update();
    pReq->unlock();
    deleteRequest(pReq);
    pReq = nullptr;

    m_requestQueue.unlock();

    if( m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // onShutdownFinished

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqDscr: " + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_requestQueue.isRequestInProgress(i_reqDscr.m_iId) )
    {
        if( i_reqDscr.m_pObjCreator == m_pDbThread )
        {
            switch( i_reqDscr.m_request )
            {
                case Database::ERequestNone:
                case Database::ERequestStartup:
                case Database::ERequestShutdown:
                case Database::ERequestConnect:
                case Database::ERequestDisconnect:
                case Database::ERequestSelectSchema:
                case Database::ERequestSaveSchema:
                case Database::ERequestSelectData:
                case Database::ERequestSaveData:
                case Database::ERequestSet2Default:
                default:
                {
                    break;
                }
            } // switch( i_reqDscr.m_request )
        } // if( i_reqDscr.m_pObjCreator == m_pDbThread )

        else if( i_reqDscr.m_pObjCreator == m_pDbCltGUI )
        {
            switch( i_reqDscr.m_request )
            {
                case Database::ERequestNone:
                case Database::ERequestStartup:
                case Database::ERequestShutdown:
                case Database::ERequestConnect:
                case Database::ERequestDisconnect:
                case Database::ERequestSelectSchema:
                case Database::ERequestSaveSchema:
                case Database::ERequestSelectData:
                case Database::ERequestSaveData:
                case Database::ERequestSet2Default:
                default:
                {
                    break;
                }
            } // switch( i_reqDscr.m_request )
        } // if( i_reqDscr.m_pObjCreator == m_pDbCltGUI )

        else if( i_reqDscr.m_pObjCreator == m_pDbCltThreadSigGen )
        {
            switch( i_reqDscr.m_request )
            {
                case Database::ERequestNone:
                case Database::ERequestStartup:
                case Database::ERequestShutdown:
                case Database::ERequestConnect:
                case Database::ERequestDisconnect:
                case Database::ERequestSelectSchema:
                case Database::ERequestSaveSchema:
                case Database::ERequestSelectData:
                case Database::ERequestSaveData:
                case Database::ERequestSet2Default:
                default:
                {
                    break;
                }
            } // switch( i_reqDscr.m_request )
        } // if( i_reqDscr.m_pObjCreator == m_pDbCltThreadSigGen )

        else if( i_reqDscr.m_pObjCreator == m_pDbCltThreadCapture )
        {
            switch( i_reqDscr.m_request )
            {
                case Database::ERequestNone:
                case Database::ERequestStartup:
                case Database::ERequestShutdown:
                case Database::ERequestConnect:
                case Database::ERequestDisconnect:
                case Database::ERequestSelectSchema:
                case Database::ERequestSaveSchema:
                case Database::ERequestSelectData:
                case Database::ERequestSaveData:
                case Database::ERequestSet2Default:
                default:
                {
                    break;
                }
            } // switch( i_reqDscr.m_request )
        } // if( i_reqDscr.m_pObjCreator == m_pDbCltThreadCapture )

        else if( i_reqDscr.m_pObjCreator == this )
        {
            switch( i_reqDscr.m_request )
            {
                case ERequestNone:
                case ERequestStartup:
                case ERequestStartupTrcServer:
                case ERequestStartupDb:
                case ERequestShutdown:
                case ERequestShutdownTrcServer:
                case ERequestShutdownDb:
                case ERequestConnect:
                case ERequestConnectDbClients:
                case ERequestConnectDbClientGUI:
                case ERequestConnectDbClientSigGen:
                case ERequestConnectDbClientCapture:
                case ERequestDisconnect:
                case ERequestDisconnectDbClients:
                case ERequestDisconnectDbClientGUI:
                case ERequestShutdownDbClientSigGen:
                case ERequestShutdownDbClientCapture:
                case ERequestDeleteDbClients:
                case ERequestDeleteDb:
                case ERequestSelectSchema:
                case ERequestSaveSchema:
                case ERequestSelectData:
                case ERequestSaveData:
                case ERequestSet2Default:
                default:
                {
                    break;
                }
            } // switch( i_reqDscr.m_request )

        } // if( i_reqDscr.m_pObjCreator == this )

        // Lock Queue before emitting any signals so that new requests triggered as reentries will be postponed.
        m_requestQueue.lock();

        if( i_reqDscr.m_iProgress_perCent >= 100 )
        {
            CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

            // If the request has been created by the application ..
            if( pReqInProgress->getCreator() == this )
            {
                // .. the request got to be deleted by the application.

                // The end of the startup and shutdown requests ..
                if( i_reqDscr.m_request == ERequestStartup || i_reqDscr.m_request == ERequestShutdown )
                {
                    // .. are signaled by the sequencer and will be handled by the
                    // "onStartupFinished" and "onShutdownFinished" slots.
                }
                else
                {
                    m_requestQueue.removeRequestInProgress();
                    deleteRequest(pReqInProgress);
                    pReqInProgress = nullptr;

                    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
                    {
                        executeNextPostponedRequest();
                    }
                }
            } // if( pReqInProgress->getCreator() == this )

            else // if( pReqInProgress->getCreator() != this )
            {
                m_requestQueue.removeRequestInProgress();
            }

        } // if( i_reqDscr.m_iProgress_perCent >= 100 )

        m_requestQueue.unlock();

    } // if( m_requestQueue.isRequestInProgress(i_reqDscr.m_iId) )

} // onRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onDbCreated( QObject* i_pDb )
//------------------------------------------------------------------------------
{
    CDb* pDb = dynamic_cast<CDb*>(i_pDb);

    if( m_pDbCltGUI != nullptr )
    {
        m_pDbCltGUI->setDb(pDb);
    }
    if( m_pDbCltThreadSigGen != nullptr )
    {
        m_pDbCltThreadSigGen->setDb(pDb);
    }
    if( m_pDbCltThreadCapture != nullptr )
    {
        m_pDbCltThreadCapture->setDb(pDb);
    }

} // onDbCreated

/*==============================================================================
protected: // instance methods (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::executeNextPostponedRequest()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeNextPostponedRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        if( !m_bMsgReqContinuePending )
        {
            m_bMsgReqContinuePending = true;

            CMsgReqContinue* pMsgReq = new CMsgReqContinue(
                /* pObjSender       */ this,
                /* pObjReceiver     */ this,
                /* bMustBeConfirmed */ false,
                /* iReqId           */ -1,
                /* iMsgId           */ -1 );
            POST_OR_DELETE_MESSAGE(pMsgReq);
        }
    } // if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )

} // executeNextPostponedRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::executeStartupRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeStartupRequest",
        /* strAddInfo   */ strAddTrcInfo );

    m_pReqSeqrStartup->start(i_pReq);

    if( m_pReqSeqrStartup->isInProgress() )
    {
        m_requestQueue.setRequestInProgress(i_pReq);

        if( !QObject::connect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pReqSeqrStartup,
            /* szSignal     */ SIGNAL(finished(qint64)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onStartupFinished(qint64)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }
    else // if( !m_pReqSeqrStartup->isInProgress() )
    {
        // "onStartupFinished" removes the request from the request queue.
        // So it its important to disconnect the "onStartupFinished" slot
        // at the end of the sequence within the "onStartupFinished" slot
        // if the request should be removed here after it has been executed
        // synchronously. For this the "onStartupFinished" slot will only
        // be connected if starting the sequence has become asynchronously.

        EState statePrev = m_state;
        m_state = EStateRunning;
        emit stateChanged( m_state, statePrev );

        m_requestQueue.lock();
        i_pReq->setResult(EResultSuccess);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
        m_requestQueue.unlock();

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. the request sequencer wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);

        // Not possible for application requests but in other scenarios possible:
        // On emitting signals while executing the request further request methods
        // may be called as reentries by the signal receivers. Those requests will
        // be postponed in the request queue of the application and have to be
        // executed now.
        if( m_requestQueue.hasPostponedRequest() )
        {
            executeNextPostponedRequest();
        }
    } // if( !m_pReqSeqrStartup->isInProgress() )

} // executeStartupRequest

//------------------------------------------------------------------------------
void CApplication::executeShutdownRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeShutdownRequest",
        /* strAddInfo   */ strAddTrcInfo );

    m_pReqSeqrShutdown->start(i_pReq);

    if( m_pReqSeqrShutdown->isInProgress() )
    {
        m_requestQueue.setRequestInProgress(i_pReq);

        if( !QObject::connect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pReqSeqrShutdown,
            /* szSignal     */ SIGNAL(finished(qint64)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onShutdownFinished(qint64)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }
    else // if( !m_pReqSeqrShutdown->isInProgress() )
    {
        // "onShutdownFinished" removes the request from the request queue.
        // So it its important to disconnect the "onShutdownFinished" slot
        // at the end of the sequence within the "onShutdownFinished" slot
        // if the request should be removed here after it has been executed
        // synchronously. For this the "onShutdownFinished" slot will only
        // be connected if starting the sequence has become asynchronously.

        EState statePrev = m_state;
        m_state = EStateCreated;
        emit stateChanged( m_state, statePrev );

        m_requestQueue.lock();
        i_pReq->setResult(EResultSuccess);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
        m_requestQueue.unlock();

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. the request sequencer wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);

        // Not possible for application requests but in other scenarios possible:
        // On emitting signals while executing the request further request methods
        // may be called as reentries by the signal receivers. Those requests will
        // be postponed in the request queue of the application and have to be
        // executed now.
        if( m_requestQueue.hasPostponedRequest() )
        {
            executeNextPostponedRequest();
        }
    } // if( !m_pReqSeqrShutdown->isInProgress() )

} // executeShutdownRequest

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CApplication::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    CMsg* pMsg = dynamic_cast<CMsg*>(i_pEv);

    if( pMsg != nullptr )
    {
        QString strAddTrcInfo;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
        {
            int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

            strAddTrcInfo  = "Msg:" + pMsg->msgTypeToStr();
            strAddTrcInfo += ", MsgId:" + QString::number(pMsg->getMsgId());

            if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
            {
                CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(pMsg);
                if( pMsgReq != nullptr )
                {
                    strAddTrcInfo += ", ReqId:" + QString::number(pMsgReq->getRequestId());
                }
            }
            else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )
            {
                CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(pMsg);
                if( pMsgCon != nullptr )
                {
                    strAddTrcInfo += ", ReqId:" + QString::number(pMsgCon->getRequestId());
                    strAddTrcInfo += ", Result:" + pMsgCon->getResultStr();
                    strAddTrcInfo += ", Progress:" + QString::number(pMsgCon->getProgress());
                }
            }

            strAddTrcInfo += ", State: " + State2Str(m_state);
            strAddTrcInfo += ", " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        if( static_cast<int>(pMsg->type()) == EBaseMsgTypeReqContinue )
        {
            CMsgReqContinue* pMsgReq = dynamic_cast<CMsgReqContinue*>(i_pEv);

            if( pMsgReq == nullptr )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgReqContinue == nullptr! " );
            }

            m_bMsgReqContinuePending = false;

            if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
            {
                CRequest* pReqPostponed = m_requestQueue.takeFirstPostponedRequest();

                if( pReqPostponed != nullptr )
                {
                    switch( pReqPostponed->getRequest() )
                    {
                        case ERequestStartup:
                        {
                            executeStartupRequest(pReqPostponed);
                            pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                            break;
                        }
                        case ERequestShutdown:
                        {
                            executeShutdownRequest(pReqPostponed);
                            pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    } // switch( pReqPostponed->getRequest() )
                } // if( pReqPostponed != nullptr )

                delete pReqPostponed;
                pReqPostponed = nullptr;

            } // if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )

            bEventHandled = true;

        } // if( pMsg->type() == EBaseMsgTypeReqContinue )
    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event
