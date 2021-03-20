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
#include "Database.h"
#include "MainWindow.h"
#include "SigAna.h"
#include "SigGen.h"
#include "TestModule1.h"
#include "TestModule2.h"

#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdServer.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSIpc/ZSIpcBlkTypeNone.h"
#include "ZSIpc/ZSIpcBlkTypeL.h"
#include "ZSIpc/ZSIpcBlkTypeE.h"
#include "ZSIpc/ZSIpcBlkTypeSL.h"
#include "ZSIpc/ZSIpcBlkTypeTLV.h"
#include "ZSIpc/ZSIpcBlkTypeUser.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::RemCmd;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::RemCmdServer;


/*******************************************************************************
class CApplication : public ZS::System::GUI::CGUIApp
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* enum EState
==============================================================================*/

static const SEnumEntry s_arEnumEntryStates[CApplication::EStateCount] = {
    /*  0: */ SEnumEntry( CApplication::EStateIdle,                     "Idle"                       ),
    /*  1: */ SEnumEntry( CApplication::EStateStartingRemCmdServer,     "Starting RemCmdServer"      ),
    /*  2: */ SEnumEntry( CApplication::EStateShuttingDownRemCmdServer, "Shutting Down RemCmdServer" ),
    /*  3: */ SEnumEntry( CApplication::EStateStartingDbThread,         "Starting DbThread"          ),
    /*  4: */ SEnumEntry( CApplication::EStateShuttingDownDbThread,     "Shutting Down DbThread"     ),
    /*  5: */ SEnumEntry( CApplication::EStateStartingSigGenThread,     "Starting SigGenThread"      ),
    /*  6: */ SEnumEntry( CApplication::EStateShuttingDownSigGenThread, "Shutting Down SigGenThread" ),
    /*  7: */ SEnumEntry( CApplication::EStateStartingSigAnaThread,     "Starting SigAnaThread"      ),
    /*  8: */ SEnumEntry( CApplication::EStateShuttingDownSigAnaThread, "Shutting Down SigAnaThread" ),
    /*  9: */ SEnumEntry( CApplication::EStateCreatingModule1,          "Creating Module 1"          ),
    /* 10: */ SEnumEntry( CApplication::EStateDeletingModule1,          "Deleting Module 1"          ),
    /* 11: */ SEnumEntry( CApplication::EStateCreatingModule2,          "Creating Module 2"          ),
    /* 12: */ SEnumEntry( CApplication::EStateDeletingModule1,          "Deleting Module 2"          )
};

//------------------------------------------------------------------------------
QString CApplication::State2Str( int i_state )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumEntryStates, _ZSArrLen(s_arEnumEntryStates), i_state );
}

/* enum ERequest
==============================================================================*/

static const SEnumEntry s_arEnumEntryRequests[CApplication::ERequestCount] = {
    /* 0: */ SEnumEntry( CApplication::ERequestNone,     "None"     ),
    /* 1: */ SEnumEntry( CApplication::ERequestStartup,  "Startup"  ),
    /* 2: */ SEnumEntry( CApplication::ERequestShutdown, "Shutdown" )
};

//------------------------------------------------------------------------------
QString CApplication::Request2Str( int i_request )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumEntryRequests, _ZSArrLen(s_arEnumEntryRequests), i_request );
}

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
public: // class methods (remote commands)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CApplication::OnExecuteRemCmdCreateModule1(
    QObject*                    /*i_pObjFctExecute*/,
    const ZS::Ipc::SSocketDscr& /*i_socketDscr*/,
    bool                        /*i_bIsQuery*/,
    const QByteArray&           /*i_byteArrArgs*/,
    QByteArray&                 /*o_byteArrResults*/,
    qint64                      /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pThis = GetInstance();

    if( pThis != nullptr )
    {
        pThis->createModule1();
    }
    return pReq;

} // OnExecuteRemCmdCreateModule1

//------------------------------------------------------------------------------
CRequest* CApplication::OnExecuteRemCmdDeleteModule1(
    QObject*                    /*i_pObjFctExecute*/,
    const ZS::Ipc::SSocketDscr& /*i_socketDscr*/,
    bool                        /*i_bIsQuery*/,
    const QByteArray&           /*i_byteArrArgs*/,
    QByteArray&                 /*o_byteArrResults*/,
    qint64                      /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pThis = GetInstance();

    if( pThis != nullptr )
    {
        pThis->deleteModule1();
    }
    return pReq;

} // OnExecuteRemCmdDeleteModule1

//------------------------------------------------------------------------------
CRequest* CApplication::OnExecuteRemCmdCreateModule2(
    QObject*                    /*i_pObjFctExecute*/,
    const ZS::Ipc::SSocketDscr& /*i_socketDscr*/,
    bool                        /*i_bIsQuery*/,
    const QByteArray&           /*i_byteArrArgs*/,
    QByteArray&                 /*o_byteArrResults*/,
    qint64                      /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pThis = GetInstance();

    if( pThis != nullptr )
    {
        pThis->createModule2();
    }
    return pReq;

} // OnExecuteRemCmdCreateModule2

//------------------------------------------------------------------------------
CRequest* CApplication::OnExecuteRemCmdDeleteModule2(
    QObject*                    /*i_pObjFctExecute*/,
    const ZS::Ipc::SSocketDscr& /*i_socketDscr*/,
    bool                        /*i_bIsQuery*/,
    const QByteArray&           /*i_byteArrArgs*/,
    QByteArray&                 /*o_byteArrResults*/,
    qint64                      /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CApplication* pThis = GetInstance();

    if( pThis != nullptr )
    {
        pThis->deleteModule2();
    }
    return pReq;

} // OnExecuteRemCmdDeleteModule2

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
    // Trace Server
    m_bTrcServerStartTcpServer(true),
    m_trcServerHostSettings(ESocketTypeTcp),
    m_trcServerSettings(),
    m_pTrcServer(nullptr),
    // Remote Command Server
    m_remCmdServerHostSettings(
        /* strLocalHostName       */ "127.0.0.1",
        /* uLocalPort             */ 26297,
        /* uMaxPendingConnections */ 30 ),
    m_blkTypeRemCmdServer(EBlkTypeL),
    m_pRemCmdServer(nullptr),
    m_pRemCmdObjCreateModule1(nullptr),
    m_pRemCmdObjDeleteModule1(nullptr),
    m_pRemCmdObjCreateModule2(nullptr),
    m_pRemCmdObjDeleteModule2(nullptr),
    // Test Modules
    m_pDbThread(nullptr),
    m_pDb(nullptr),
    m_pSigGenThread(nullptr),
    m_pSigGen(nullptr),
    m_pSigAnaThread(nullptr),
    m_pSigAna(nullptr),
    m_pTestModule1(nullptr),
    m_pTestModule2Thread(nullptr),
    m_pTestModule2(nullptr),
    // State Machine
    m_bReqExecTreeGarbageCollectorEnabled(true),
    m_fReqExecTreeGarbageCollectorInterval_s(5.0),
    m_fReqExecTreeGarbageCollectorElapsed_s(60.0),
    m_pReqExecTree(nullptr),
    m_state(EStateIdle),
    m_pReqInProgress(nullptr),
    m_hshReqsInProgress(),
    // Main Window
    m_pMainWindow(nullptr),
    // Method Trace
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

    ZS::Ipc::SServerHostSettings trcServerHostSettingsDefault = m_trcServerHostSettings;
    ZS::Trace::STrcServerSettings trcServerSettingsDefault = m_trcServerSettings;

    ZS::Ipc::SServerHostSettings remCmdServerHostSettingsDefault = m_remCmdServerHostSettings;

    // Parse command arguments (first part, IniFile)
    //----------------------------------------------

    int         idxArg;
    QString     strArg;
    QString     strVal;
    int         iVal;
    bool        bConverted;
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
        else if( strArg.compare("MthTrcServer",Qt::CaseInsensitive) == 0 )
        {
            m_bTrcServerStartTcpServer = str2Bool(strVal);
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

        if( strArg.compare("LocalPort",Qt::CaseInsensitive) == 0 )
        {
            iVal = strVal.toInt(&bConverted);
            if( bConverted && iVal >= 1000 && iVal <= _UI16_MAX )
            {
                m_remCmdServerHostSettings.m_uLocalPort = static_cast<quint16>(iVal);
            }
        }
        else if( strArg.compare("BlockType",Qt::CaseInsensitive) == 0 )
        {
            EBlkType blkType = str2BlkType(strVal);
            if( blkType >= 0 && blkType < EBlkTypeCount )
            {
                m_blkTypeRemCmdServer = blkType;
            }
        }
    }

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

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CApplication", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // Create remote command server
    //-----------------------------

    m_pRemCmdServer = new ZS::RemCmd::CRemCmdServer("RemCmdServer");

    m_pRemCmdServer->setHostSettings(m_remCmdServerHostSettings);

    if( m_blkTypeRemCmdServer != m_pRemCmdServer->getBlkType()->type() )
    {
        CBlkType* pBlkType = nullptr;

        switch( m_blkTypeRemCmdServer )
        {
            case EBlkTypeNone:  // <Data>
            {
                pBlkType = new CBlkTypeNone();
                break;
            }
            case EBlkTypeL:     // <Len>[Data]
            {
                pBlkType = new CBlkTypeL(EBlkLenDataTypeBinUINT32);
                break;
            }
            case EBlkTypeE:     // [Data]<End>
            {
                pBlkType = new CBlkTypeE(QByteArray(1,'\n'));
                break;
            }
            case EBlkTypeSL:    // <Start><Len>[Data]
            {
                pBlkType = new CBlkTypeSL(
                    /* byteArrBlkStart */ QByteArray(1, (char)0xff),
                    /* blkLenDataType  */ EBlkLenDataTypeBinUINT32 );
                break;
            }
            case EBlkTypeTLV:   // <Tag><Len>[Value]
            {
                break;
            }
            case EBlkTypeUser:
            {
                //pBlkType = new CBlkTypeUser();
                break;
            }
            default:
            {
                break;
            }
        }

        if( pBlkType != nullptr )
        {
            m_pRemCmdServer->setBlkType(pBlkType);
        }
    } // if( m_blkTypeRemCmdServer != m_pRemCmdServer->getBlkType()->type() )

    // Use "direct connection" to avoid that the "onDestroyed" slot is called after
    // the object has already been completely destroyed.
    if( !QObject::connect(
        /* pObjSender     */ m_pRemCmdServer,
        /* szSignal       */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onObjectDestroyed(QObject*)),
        /* connectionType */ Qt::DirectConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Create remote command objects
    //------------------------------

    m_pRemCmdObjCreateModule1 = new CRemCmdObj(
        /* pServer        */ m_pRemCmdServer,
        /* strCmd         */ "Module1:Create",
        /* pObjFctExecute */ this,
        /* pFctExecute    */ OnExecuteRemCmdCreateModule1 );

    m_pRemCmdObjDeleteModule1 = new CRemCmdObj(
        /* pServer        */ m_pRemCmdServer,
        /* strCmd         */ "Module1:Delete",
        /* pObjFctExecute */ this,
        /* pFctExecute    */ OnExecuteRemCmdDeleteModule1 );

    m_pRemCmdObjCreateModule2 = new CRemCmdObj(
        /* pServer        */ m_pRemCmdServer,
        /* strCmd         */ "Module2:Create",
        /* pObjFctExecute */ this,
        /* pFctExecute    */ OnExecuteRemCmdCreateModule2 );

    m_pRemCmdObjDeleteModule2 = new CRemCmdObj(
        /* pServer        */ m_pRemCmdServer,
        /* strCmd         */ "Module2:Delete",
        /* pObjFctExecute */ this,
        /* pFctExecute    */ OnExecuteRemCmdDeleteModule2 );

    // Create Database Thread
    //-----------------------

    m_pDbThread = new CDatabaseThread("Db");

    if( !QObject::connect(
        /* pObjSender     */ m_pDbThread,
        /* szSignal       */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onObjectDestroyed(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Create Thread of Signal Generator
    //----------------------------------

    m_pSigGenThread = new CSigGenThread("SigGen");

    if( !QObject::connect(
        /* pObjSender     */ m_pSigGenThread,
        /* szSignal       */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onObjectDestroyed(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Create Thread of Signal Analyzer
    //----------------------------------

    m_pSigAnaThread = new CSigAnaThread("SigAna");

    if( !QObject::connect(
        /* pObjSender     */ m_pSigAnaThread,
        /* szSignal       */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onObjectDestroyed(QObject*)) ) )
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

    m_pMainWindow = new CMainWindow(
        /* strWindowTitle */ i_strWindowTitle,
        /* pRemCmdServer  */ m_pRemCmdServer );
    m_pMainWindow->show();

    // Startup system
    //---------------

    startup();

} // ctor

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ strAddTrcInfo );

    // Save settings of the application
    //---------------------------------

    m_pTrcServer->saveAdminObjs();

    saveSettings();

    // Destroy objects created and controlled by the application
    //----------------------------------------------------------

    if( m_pTestModule2Thread != nullptr )
    {
        m_pTestModule2Thread->quit();
        m_pTestModule2Thread->wait();
    }

    try
    {
        delete m_pTestModule2Thread;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pTestModule1;
    }
    catch(...)
    {
    }

    if( m_pSigAnaThread != nullptr )
    {
        m_pSigAnaThread->quit();
        m_pSigAnaThread->wait();
    }

    try
    {
        delete m_pSigAnaThread;
    }
    catch(...)
    {
    }

    if( m_pSigGenThread != nullptr )
    {
        m_pSigGenThread->quit();
        m_pSigGenThread->wait();
    }

    try
    {
        delete m_pSigGenThread;
    }
    catch(...)
    {
    }

    if( m_pDbThread != nullptr )
    {
        m_pDbThread->quit();
        m_pDbThread->wait();
    }

    try
    {
        delete m_pDbThread;
    }
    catch(...)
    {
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
        delete m_pRemCmdServer;
    }
    catch(...)
    {
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

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
    // Trace Server
    m_bTrcServerStartTcpServer = false;
    //m_trcServerHostSettings;
    //m_trcServerSettings;
    m_pTrcServer = nullptr;
    // Remote Command Server
    //m_remCmdServerHostSettings;
    m_blkTypeRemCmdServer = static_cast<EBlkType>(0);
    m_pRemCmdServer = nullptr;
    m_pRemCmdObjCreateModule1 = nullptr;
    m_pRemCmdObjDeleteModule1 = nullptr;
    m_pRemCmdObjCreateModule2 = nullptr;
    m_pRemCmdObjDeleteModule2 = nullptr;
    // Test Modules
    m_pDbThread = nullptr;
    m_pDb = nullptr;
    m_pSigGenThread = nullptr;
    m_pSigGen = nullptr;
    m_pSigAnaThread = nullptr;
    m_pSigAna = nullptr;
    m_pTestModule1 = nullptr;
    m_pTestModule2Thread = nullptr;
    m_pTestModule2 = nullptr;
    // State Machine
    m_bReqExecTreeGarbageCollectorEnabled = false;
    m_fReqExecTreeGarbageCollectorInterval_s = 0.0;
    m_fReqExecTreeGarbageCollectorElapsed_s = 0.0;
    m_pReqExecTree = nullptr;
    m_state = static_cast<EState>(0);
    m_pReqInProgress = nullptr;
    //m_hshReqsInProgress;
    // Main Window
    m_pMainWindow = nullptr;
    // Method Trace
    m_pTrcAdminObj = nullptr;

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

        // Remote Command Server
        //----------------------

        strSettingsKey = "RemCmdServer";
        bSyncSettings = false;

        if( m_pSettingsFile->contains(strSettingsKey+"/SocketType") )
        {
            m_remCmdServerHostSettings.m_socketType = str2SocketType( m_pSettingsFile->value(strSettingsKey+"/SocketType",socketType2Str(m_remCmdServerHostSettings.m_socketType)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/SocketType", socketType2Str(m_remCmdServerHostSettings.m_socketType) );
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/LocalHostName") )
        {
            m_remCmdServerHostSettings.m_strLocalHostName = m_pSettingsFile->value(strSettingsKey+"/LocalHostName",m_remCmdServerHostSettings.m_strLocalHostName).toString();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/LocalHostName", m_remCmdServerHostSettings.m_strLocalHostName );
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/LocalPort") )
        {
            m_remCmdServerHostSettings.m_uLocalPort = m_pSettingsFile->value(strSettingsKey+"/LocalPort",m_remCmdServerHostSettings.m_uLocalPort).toUInt();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/LocalPort", m_remCmdServerHostSettings.m_uLocalPort );
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/MaxPendingConnections") )
        {
            m_remCmdServerHostSettings.m_uMaxPendingConnections = m_pSettingsFile->value(strSettingsKey+"/MaxPendingConnections",m_remCmdServerHostSettings.m_uMaxPendingConnections).toUInt();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/MaxPendingConnections", m_remCmdServerHostSettings.m_uMaxPendingConnections );
            bSyncSettings = true;
        }
        if( m_pSettingsFile->contains(strSettingsKey+"/BufferSize") )
        {
            m_remCmdServerHostSettings.m_uBufferSize = m_pSettingsFile->value(strSettingsKey+"/BufferSize",m_remCmdServerHostSettings.m_uBufferSize).toUInt();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/BufferSize", m_remCmdServerHostSettings.m_uBufferSize );
            bSyncSettings = true;
        }

        // Update settings file
        //---------------------

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

        if( m_pRemCmdServer != nullptr )
        {
            strSettingsKey = "RemCmdServer";

            SServerHostSettings hostSettings = m_pRemCmdServer->getHostSettings();

            m_pSettingsFile->setValue( strSettingsKey+"/SocketType", socketType2Str(hostSettings.m_socketType) );
            m_pSettingsFile->setValue( strSettingsKey+"/LocalHostName", hostSettings.m_strLocalHostName );
            m_pSettingsFile->setValue( strSettingsKey+"/LocalPort", hostSettings.m_uLocalPort );
            m_pSettingsFile->setValue( strSettingsKey+"/MaxPendingConnections", hostSettings.m_uMaxPendingConnections );
            m_pSettingsFile->setValue( strSettingsKey+"/BufferSize", hostSettings.m_uBufferSize );

            m_pSettingsFile->sync();

        } // if( m_pRemCmdServer != nullptr )

    } // if( m_pSettingsFile != nullptr )

} // saveSettings

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ZS::System::CRequest* CApplication::createModule1( qint64 /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "createModule1",
        /* szAddInfo    */ strAddTrcInfo );

    if( m_pReqInProgress == nullptr )
    {
        if( m_state != EStateIdle )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "No request is in progress but state is not Idle." );
        }

        if( m_pTestModule1 == nullptr )
        {
            m_state = EStateCreatingModule1;

            m_pTestModule1 = new CTestModule1(
                /* strObjName            */ "TestModule1",
                /* strTestModule2ObjName */ "TestModule2" );

            if( !QObject::connect(
                /* pObjSender     */ m_pTestModule1,
                /* szSignal       */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver   */ this,
                /* szSlot         */ SLOT(onObjectDestroyed(QObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            m_state = EStateIdle;

        } // if( m_pTestModule1 == nullptr )
    } // if( m_pReqInProgress == nullptr )

    return m_pReqInProgress;

} // createModule1

//------------------------------------------------------------------------------
ZS::System::CRequest* CApplication::deleteModule1( qint64 /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "deleteModule1",
        /* szAddInfo    */ strAddTrcInfo );

    if( m_pReqInProgress == nullptr )
    {
        if( m_pTestModule1 != nullptr )
        {
            delete m_pTestModule1; // emits signal destroyed
            m_pTestModule1 = nullptr;
        }

    } // if( m_pReqInProgress == nullptr )

    return m_pReqInProgress;

} // deleteModule1

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CApplication::createModule2( qint64 /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "createModule2",
        /* szAddInfo    */ strAddTrcInfo );

    if( m_pReqInProgress == nullptr )
    {
        if( m_state != EStateIdle )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "No request is in progress but state is not Idle." );
        }

        if( m_pTestModule1 != nullptr && m_pTestModule2 == nullptr )
        {
            m_state = EStateCreatingModule2;

            m_pReqInProgress = m_pTestModule1->startup();

            if( isAsynchronousRequest(m_pReqInProgress) )
            {
                if( !QObject::connect(
                    /* pObjSender   */ m_pReqInProgress,
                    /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onTestModule1PendingRequestChanged(ZS::System::SRequestDscr)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
            else
            {
                m_state = EStateIdle;
                m_pReqInProgress = nullptr; // deleted later by request queue of test module 1
            }
        } // if( m_pTestModule1 != nullptr && m_pTestModule2 == nullptr )
    } // if( m_pReqInProgress == nullptr )

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return m_pReqInProgress;

} // createModule2

//------------------------------------------------------------------------------
CRequest* CApplication::deleteModule2( qint64 /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "deleteModule2",
        /* szAddInfo    */ strAddTrcInfo );

    if( m_pReqInProgress == nullptr )
    {
        if( m_state != EStateIdle )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "No request is in progress but state is not Idle." );
        }

        if( m_pTestModule1 != nullptr )
        {
            m_state = EStateDeletingModule2;

            m_pReqInProgress = m_pTestModule1->shutdown();

            if( isAsynchronousRequest(m_pReqInProgress) )
            {
                if( !QObject::connect(
                    /* pObjSender   */ m_pReqInProgress,
                    /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onTestModule1PendingRequestChanged(ZS::System::SRequestDscr)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
            else
            {
                m_state = EStateIdle;
                m_pReqInProgress = nullptr; // deleted later by request queue of test module 1
            }
        } // if( m_pTestModule1 != nullptr && m_pTestModule2 == nullptr )
    } // if( m_pReqInProgress == nullptr )

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return m_pReqInProgress;

} // deleteModule2

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CApplication::requestInProgress2Str() const
//------------------------------------------------------------------------------
{
    QString strRequest = Request2Str(ERequestNone);

    if( m_pReqInProgress != nullptr )
    {
        strRequest = m_pReqInProgress->getAddTrcInfoStr();
    }
    return strRequest;

} // requestInProgress2Str

/*==============================================================================
public: // instance methods of system startup and system shutdown
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::startup()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startup",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_state != EStateIdle )
    {
        throw CException( __FILE__, __LINE__, EResultInternalStateMachineError, "State != Idle" );
    }
    if( m_pReqInProgress != nullptr )
    {
        throw CException( __FILE__, __LINE__, EResultInternalStateMachineError, "RequestIsInProgressOrHasBeenPostponed" );
    }

    m_pReqInProgress = new CRequest(
        /* strNameSpace */ "ZS::Apps::Test::RemCmdServer",
        /* strClassName */ "CApplication",
        /* pObjCreator  */ this,
        /* iRequest     */ ERequestStartup,
        /* strRequest   */ Request2Str(ERequestStartup),
        /* iReqIdParent */ -1 );

    // Startup remote command server
    //------------------------------

    m_state = EStateStartingRemCmdServer;

    CRequest* pReqPending = nullptr;

    if( m_pRemCmdServer != nullptr )
    {
        pReqPending = m_pRemCmdServer->startup(
            /* iTimeout_ms  */ 0,
            /* bWait        */ false,
            /* iReqIdParent */ m_pReqInProgress->getId());
    }

    if( isAsynchronousRequest(pReqPending) )
    {
        m_hshReqsInProgress[pReqPending->getId()] = pReqPending;

        if( !QObject::connect(
            /* pObjSender   */ pReqPending,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRemCmdServerPendingRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }
    else // if( !isAsynchronousRequest(pReqPending) )
    {
        pReqPending = nullptr; // deleted later by request queue of remote command server
        m_state = EStateStartingDbThread;
        QTimer::singleShot( 20, this, SLOT(onStartupDbThread()) );
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // startup

//------------------------------------------------------------------------------
void CApplication::shutdown()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "shutdown",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pReqInProgress != nullptr )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo(
                /* errSource  */ "ZS::Apps::Test::RemCmdServer", "CApplication", "theInst", "shutdown",
                /* result     */ EResultInternalStateMachineError,
                /* severity   */ EResultSeverityError,
                /* strAddInfo */ "Shutdown while the request " + m_pReqInProgress->getRequestName(true) + " is still in progress." );
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }

        deleteRequest(m_pReqInProgress);
        m_pReqInProgress = nullptr;

    } // if( m_pReqInProgress != nullptr )

    if( m_state != EStateIdle )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo(
                /* errSource  */ "ZS::Apps::Test::RemCmdServer", "CApplication", "theInst", "shutdown",
                /* result     */ EResultInternalStateMachineError,
                /* severity   */ EResultSeverityError,
                /* strAddInfo */ "Shutdown while State != Idle" );
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
        m_state = EStateIdle;
    }

    m_pReqInProgress = new CRequest(
        /* strNameSpace */ "ZS::Apps::Test::RemCmdServer",
        /* strClassName */ "CApplication",
        /* pObjCreator  */ this,
        /* iRequest     */ ERequestShutdown,
        /* strRequest   */ Request2Str(ERequestShutdown),
        /* iReqIdParent */ -1 );

    // Shutdown signal analyser
    //-------------------------

    m_state = EStateShuttingDownSigAnaThread;

    if( m_pSigAnaThread != nullptr )
    {
        m_pSigAnaThread->quit();
        m_pSigAnaThread->wait();
    }

    try
    {
        delete m_pSigAnaThread;
    }
    catch(...)
    {
    }
    m_pSigAnaThread = nullptr;
    m_pSigAna = nullptr;

    // Shutdown signal generator
    //--------------------------

    m_state = EStateShuttingDownSigGenThread;

    if( m_pSigGenThread != nullptr )
    {
        m_pSigGenThread->quit();
        m_pSigGenThread->wait();
    }

    try
    {
        delete m_pSigGenThread;
    }
    catch(...)
    {
    }
    m_pSigGenThread = nullptr;
    m_pSigGen = nullptr;

    // Shutdown database
    //-------------------------

    m_state = EStateShuttingDownDbThread;

    if( m_pDbThread != nullptr )
    {
        m_pDbThread->quit();
        m_pDbThread->wait();
    }

    try
    {
        delete m_pDbThread;
    }
    catch(...)
    {
    }
    m_pDbThread = nullptr;
    m_pDb = nullptr;

    // Shutdown remote command server
    //-------------------------------

    m_state = EStateShuttingDownRemCmdServer;

    if( m_pRemCmdServer != nullptr )
    {
        m_pRemCmdServer->abortAllRequests();

        CRequest* pReqPending = m_pRemCmdServer->shutdown(m_pReqInProgress->getId());

        if( isAsynchronousRequest(pReqPending) )
        {
            m_hshReqsInProgress[pReqPending->getId()] = pReqPending;

            if( !QObject::connect(
                /* pObjSender   */ pReqPending,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRemCmdServerPendingRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
        else // if( !isAsynchronousRequest(pReq) )
        {
            pReqPending = nullptr; // deleted later by request queue
        }

    } // if( m_pRemCmdServer != nullptr )

    if( m_hshReqsInProgress.size() == 0 )
    {
        deleteRequest(m_pReqInProgress);
        m_pReqInProgress = nullptr;

        m_state = EStateIdle;

        // Quit the application (terminate main event loop)
        quit();
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // shutdown

/*==============================================================================
protected slots: // of system startup and system shutdown
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onLastWindowClosed()
//------------------------------------------------------------------------------
{
    shutdown();
}

//------------------------------------------------------------------------------
void CApplication::onStartupDbThread()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "State: " + State2Str(m_state);

        if( m_pReqInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + m_pReqInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onStartupDbThread",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pReqInProgress == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }
    if( m_pReqInProgress->getRequest() != ERequestStartup )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }
    if( m_state != EStateStartingDbThread )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }

    CRequest* pReqPending = nullptr;

    if( m_pDbThread != nullptr )
    {
        pReqPending = m_pDbThread->startup( m_pRemCmdServer, m_pReqInProgress->getId() );
    }

    if( isAsynchronousRequest(pReqPending) )
    {
        m_hshReqsInProgress[pReqPending->getId()] = pReqPending;

        if( !QObject::connect(
            /* pObjSender   */ pReqPending,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDbThreadPendingRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }
    else // if( !isAsynchronousRequest(pReqPending) )
    {
        pReqPending = nullptr; // deleted later by request queue of database thread
        m_state = EStateStartingSigGenThread;
        QTimer::singleShot( 20, this, SLOT(onStartupSigGenThread()) );
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // onStartupDbThread

//------------------------------------------------------------------------------
void CApplication::onStartupSigGenThread()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "State: " + State2Str(m_state);

        if( m_pReqInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + m_pReqInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onStartupSigGenThread",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pReqInProgress == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }
    if( m_pReqInProgress->getRequest() != ERequestStartup )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }
    if( m_state != EStateStartingSigGenThread )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }

    CRequest* pReqPending = nullptr;

    if( m_pSigGenThread != nullptr )
    {
        pReqPending = m_pSigGenThread->startup( m_pDb, m_pReqInProgress->getId() );
    }

    if( isAsynchronousRequest(pReqPending) )
    {
        m_hshReqsInProgress[pReqPending->getId()] = pReqPending;

        pReqPending->setExecutionData("ObjRef",m_pSigGenThread);

        if( !QObject::connect(
            /* pObjSender   */ pReqPending,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onSigGenThreadPendingRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }
    else // if( !isAsynchronousRequest(pReqPending) )
    {
        pReqPending = nullptr; // deleted later by request queue of signal generator thread
        m_state = EStateStartingSigAnaThread;
        QTimer::singleShot( 20, this, SLOT(onStartupSigAnaThread()) );
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // onStartupSigGenThread

//------------------------------------------------------------------------------
void CApplication::onStartupSigAnaThread()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "State: " + State2Str(m_state);

        if( m_pReqInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + m_pReqInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onStartupSigAnaThread",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pReqInProgress == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }
    if( m_pReqInProgress->getRequest() != ERequestStartup )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }
    if( m_state != EStateStartingSigAnaThread )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }

    CRequest* pReqPending = nullptr;

    if( m_pSigAnaThread != nullptr )
    {
        pReqPending = m_pSigAnaThread->startup( m_pDb, m_pSigGen, m_pReqInProgress->getId() );
    }

    if( isAsynchronousRequest(pReqPending) )
    {
        m_hshReqsInProgress[pReqPending->getId()] = pReqPending;

        pReqPending->setExecutionData("ObjRef",m_pSigAnaThread);

        if( !QObject::connect(
            /* pObjSender   */ pReqPending,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onSigAnaThreadPendingRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }
    else // if( !isAsynchronousRequest(pReqPending) )
    {
        pReqPending = nullptr; // deleted later by request queue of signal generator thread
        m_state = EStateIdle;

        m_pReqInProgress->setErrResult(ErrResultSuccess);
        m_pReqInProgress->setProgressInPerCent(100);
        m_pReqInProgress->update();

        deleteRequest(m_pReqInProgress);
        m_pReqInProgress = nullptr;
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // onStartupSigAnaThread

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onRemCmdServerPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "Request: " + i_reqDscr.m_strRequest;
        strAddTrcInfo += ", ReqId: " + QString::number(i_reqDscr.m_iId);
        strAddTrcInfo += ", Progress: " + QString::number(i_reqDscr.m_iProgress_perCent);
        strAddTrcInfo += ", Result: " + i_reqDscr.m_errResultInfo.getResultStr();

        strAddTrcInfo += ", State: " + State2Str(m_state);

        if( m_pReqInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + m_pReqInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdServerPendingRequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( CErrLog::GetInstance() != nullptr && i_reqDscr.m_errResultInfo.isErrorResult() )
    {
        if( !i_reqDscr.m_errResultInfo.isAddedToErrLogModel() )
        {
            CErrLog::GetInstance()->addEntry(i_reqDscr.m_errResultInfo);
            i_reqDscr.m_errResultInfo.setAddedToErrLogModel(true);
        }
    } // if( CErrLog::GetInstance() != nullptr && i_pReq != nullptr && i_reqDscr.m_errResultInfo.isErrorResult() )

    if( m_pReqInProgress != nullptr )
    {
        switch( m_pReqInProgress->getRequest() )
        {
            case ERequestStartup:
            {
                if( m_state == EStateStartingRemCmdServer )
                {
                    if( i_reqDscr.m_iProgress_perCent == 100 && m_hshReqsInProgress.contains(i_reqDscr.m_iId) )
                    {
                        m_hshReqsInProgress.remove(i_reqDscr.m_iId);
                        m_state = EStateStartingDbThread;
                        QTimer::singleShot( 20, this, SLOT(onStartupDbThread()) );
                    }
                } // if( m_state == EStateStartingRemCmdServer )
                break;
            } // case ERequestStartup

            case ERequestShutdown:
            {
                if( m_state == EStateShuttingDownRemCmdServer )
                {
                    if( i_reqDscr.m_iProgress_perCent == 100 && m_hshReqsInProgress.contains(i_reqDscr.m_iId) )
                    {
                        m_hshReqsInProgress.remove(i_reqDscr.m_iId);

                        if( m_pRemCmdServer != nullptr )
                        {
                            saveSettings();
                        }

                        deleteRequest(m_pReqInProgress);
                        m_pReqInProgress = nullptr;

                        m_state = EStateIdle;

                        // Quit the application (terminate main event loop)
                        quit();
                    }
                } // if( m_state == EStateShuttingDownRemCmdServer )
                break;
            } // case ERequestShutdown
        } // switch( pReqInProgress->getRequest() )
    } // if( pReqInProgress != nullptr )

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // onRemCmdServerPendingRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onDbThreadPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "Request: " + i_reqDscr.m_strRequest;
        strAddTrcInfo += ", ReqId: " + QString::number(i_reqDscr.m_iId);
        strAddTrcInfo += ", Progress: " + QString::number(i_reqDscr.m_iProgress_perCent);
        strAddTrcInfo += ", Result: " + i_reqDscr.m_errResultInfo.getResultStr();

        strAddTrcInfo += ", State: " + State2Str(m_state);

        if( m_pReqInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + m_pReqInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbThreadPendingRequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( CErrLog::GetInstance() != nullptr && i_reqDscr.m_errResultInfo.isErrorResult() )
    {
        if( !i_reqDscr.m_errResultInfo.isAddedToErrLogModel() )
        {
            CErrLog::GetInstance()->addEntry(i_reqDscr.m_errResultInfo);
            i_reqDscr.m_errResultInfo.setAddedToErrLogModel(true);
        }
    }

    if( m_pReqInProgress != nullptr )
    {
        switch( m_pReqInProgress->getRequest() )
        {
            case ERequestStartup:
            {
                if( m_state == EStateStartingDbThread )
                {
                    if( i_reqDscr.m_iProgress_perCent == 100 && m_hshReqsInProgress.contains(i_reqDscr.m_iId) )
                    {
                        m_hshReqsInProgress.remove(i_reqDscr.m_iId);
                        m_pDb = m_pDbThread->getDb();
                        m_pMainWindow->setDb(m_pDb);
                        m_state = EStateStartingSigGenThread;
                        QTimer::singleShot( 20, this, SLOT(onStartupSigGenThread()) );
                    }
                } // if( m_state == EStateStartingDbThread )
                break;
            } // case ERequestStartup
        } // switch( pReqInProgress->getRequest() )
    } // if( pReqInProgress != nullptr )

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // onDbThreadPendingRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onSigGenThreadPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "Request: " + i_reqDscr.m_strRequest;
        strAddTrcInfo += ", ReqId: " + QString::number(i_reqDscr.m_iId);
        strAddTrcInfo += ", Progress: " + QString::number(i_reqDscr.m_iProgress_perCent);
        strAddTrcInfo += ", Result: " + i_reqDscr.m_errResultInfo.getResultStr();

        strAddTrcInfo += ", State: " + State2Str(m_state);

        if( m_pReqInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + m_pReqInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onSigGenThreadPendingRequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( CErrLog::GetInstance() != nullptr && i_reqDscr.m_errResultInfo.isErrorResult() )
    {
        if( !i_reqDscr.m_errResultInfo.isAddedToErrLogModel() )
        {
            CErrLog::GetInstance()->addEntry(i_reqDscr.m_errResultInfo);
            i_reqDscr.m_errResultInfo.setAddedToErrLogModel(true);
        }
    }

    if( m_pReqInProgress != nullptr )
    {
        switch( m_pReqInProgress->getRequest() )
        {
            case ERequestStartup:
            {
                if( m_state == EStateStartingSigGenThread )
                {
                    if( i_reqDscr.m_iProgress_perCent == 100 && m_hshReqsInProgress.contains(i_reqDscr.m_iId) )
                    {
                        m_hshReqsInProgress.remove(i_reqDscr.m_iId);
                        m_pSigGen = m_pSigGenThread->getSigGen();
                        m_state = EStateStartingSigAnaThread;
                        QTimer::singleShot( 20, this, SLOT(onStartupSigAnaThread()) );
                    }
                } // if( m_state == EStateStartingSigGenThread )
                break;
            } // case ERequestStartup
        } // switch( m_pReqInProgress->getRequest() )
    } // if( m_pReqInProgress != nullptr )

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // onSigGenThreadPendingRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onSigAnaThreadPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "Request: " + i_reqDscr.m_strRequest;
        strAddTrcInfo += ", ReqId: " + QString::number(i_reqDscr.m_iId);
        strAddTrcInfo += ", Progress: " + QString::number(i_reqDscr.m_iProgress_perCent);
        strAddTrcInfo += ", Result: " + i_reqDscr.m_errResultInfo.getResultStr();

        strAddTrcInfo += ", State: " + State2Str(m_state);

        if( m_pReqInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + m_pReqInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onSigAnaThreadPendingRequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( CErrLog::GetInstance() != nullptr && i_reqDscr.m_errResultInfo.isErrorResult() )
    {
        if( !i_reqDscr.m_errResultInfo.isAddedToErrLogModel() )
        {
            CErrLog::GetInstance()->addEntry(i_reqDscr.m_errResultInfo);
            i_reqDscr.m_errResultInfo.setAddedToErrLogModel(true);
        }
    }

    if( m_pReqInProgress != nullptr )
    {
        switch( m_pReqInProgress->getRequest() )
        {
            case ERequestStartup:
            {
                if( m_state == EStateStartingSigAnaThread )
                {
                    if( i_reqDscr.m_iProgress_perCent == 100 && m_hshReqsInProgress.contains(i_reqDscr.m_iId) )
                    {
                        m_hshReqsInProgress.remove(i_reqDscr.m_iId);

                        m_pSigAna = m_pSigAnaThread->getSigAna();

                        m_state = EStateIdle;

                        m_pReqInProgress->setErrResult(ErrResultSuccess);
                        m_pReqInProgress->setProgressInPerCent(100);
                        m_pReqInProgress->update();

                        deleteRequest(m_pReqInProgress);
                        m_pReqInProgress = nullptr;
                    }
                } // if( m_state == EStateStartingSigGenThread )
                break;
            } // case ERequestStartup
        } // switch( pReqInProgress->getRequest() )
    } // if( pReqInProgress != nullptr )

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // onSigAnaThreadPendingRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onTestModule1PendingRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "Request: " + i_reqDscr.m_strRequest;
        strAddTrcInfo += ", ReqId: " + QString::number(i_reqDscr.m_iId);
        strAddTrcInfo += ", Progress: " + QString::number(i_reqDscr.m_iProgress_perCent);
        strAddTrcInfo += ", Result: " + i_reqDscr.m_errResultInfo.getResultStr();

        strAddTrcInfo += ", State: " + State2Str(m_state);

        if( m_pReqInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + m_pReqInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTestModule1PendingRequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( CErrLog::GetInstance() != nullptr && i_reqDscr.m_errResultInfo.isErrorResult() )
    {
        if( !i_reqDscr.m_errResultInfo.isAddedToErrLogModel() )
        {
            CErrLog::GetInstance()->addEntry(i_reqDscr.m_errResultInfo);
            i_reqDscr.m_errResultInfo.setAddedToErrLogModel(true);
        }
    }

    if( m_pReqInProgress != nullptr )
    {
        switch( m_pReqInProgress->getRequest() )
        {
            case CTestModule1::ERequestStartup:
            {
                if( m_state == EStateCreatingModule2 )
                {
                    if( i_reqDscr.m_iProgress_perCent == 100 && m_pReqInProgress->getId() == i_reqDscr.m_iId )
                    {
                        if( m_pTestModule1 == nullptr )
                        {
                            throw CException( __FILE__, __LINE__, EResultInternalStateMachineError );
                        }
                        if( m_pTestModule2Thread != nullptr || m_pTestModule2 != nullptr )
                        {
                            throw CException( __FILE__, __LINE__, EResultInternalStateMachineError );
                        }

                        m_pTestModule2Thread = m_pTestModule1->getTestModule2Thread();
                        m_pTestModule2 = m_pTestModule1->getTestModule2();

                        if( m_pTestModule2Thread == nullptr || m_pTestModule2 == nullptr )
                        {
                            throw CException( __FILE__, __LINE__, EResultInternalProgramError );
                        }

                        m_pReqInProgress = nullptr; // deleted by test module

                        m_state = EStateIdle;

                        if( !QObject::connect(
                            /* pObjSender   */ m_pTestModule2Thread,
                            /* szSignal     */ SIGNAL(destroyed(QObject*)),
                            /* pObjReceiver */ this,
                            /* szSlot       */ SLOT(onObjectDestroyed(QObject*)) ) )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                        }

                        // Use "direct connection" to avoid that the "onDestroyed" slot is called after
                        // the object has already been completely destroyed.
                        if( !QObject::connect(
                            /* pObjSender     */ m_pTestModule2,
                            /* szSignal       */ SIGNAL(destroyed(QObject*)),
                            /* pObjReceiver   */ this,
                            /* szSlot         */ SLOT(onObjectDestroyed(QObject*)),
                            /* connectionType */ Qt::DirectConnection ) )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                        }
                    } // if( i_reqDscr.m_iProgress_perCent == 100 )
                } // if( m_state == EStateCreatingModule2 )
                else
                {
                    QString strAddErrInfo = "RequestInProgress: " + Request2Str(m_pReqInProgress->getRequest()) + ", State: " + State2Str(m_state);
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, strAddErrInfo );
                }
                break;
            } // case CTestModule1::ERequestStartup

            case CTestModule1::ERequestShutdown:
            {
                if( m_state == EStateDeletingModule2 )
                {
                    if( i_reqDscr.m_iProgress_perCent == 100 && m_pReqInProgress->getId() == i_reqDscr.m_iId )
                    {
                        if( m_pTestModule1 == nullptr )
                        {
                            throw CException( __FILE__, __LINE__, EResultInternalStateMachineError );
                        }

                        m_pTestModule2Thread = nullptr;
                        m_pTestModule2 = nullptr;

                        m_pReqInProgress = nullptr; // deleted by test module

                        m_state = EStateIdle;

                    } // if( i_reqDscr.m_iProgress_perCent == 100 )
                } // if( m_state == EStateDeletingModule2 )
                else
                {
                    QString strAddErrInfo = "RequestInProgress: " + Request2Str(m_pReqInProgress->getRequest()) + ", State: " + State2Str(m_state);
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, strAddErrInfo );
                }
                break;
            } // case CTestModule1::ERequestShutdown

            default:
            {
                QString strAddErrInfo = "Don't expect signal for request " + Request2Str(m_pReqInProgress->getRequest());
                throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, strAddErrInfo );
                break;
            }

        } // switch( pReqInProgress->getRequest() )
    } // if( pReqInProgress != nullptr )

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // onTestModule1PendingRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onObjectDestroyed( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pObj == nullptr )
        {
            strAddTrcInfo  = "Object: nullptr";
        }
        else if( i_pObj == m_pRemCmdServer )
        {
            strAddTrcInfo  = "Object: RemCmdServer";
        }
        else if( i_pObj == m_pDb )
        {
            strAddTrcInfo  = "Object: Db";
        }
        else if( i_pObj == m_pDbThread )
        {
            strAddTrcInfo  = "Object: DbThread";
        }
        else if( i_pObj == m_pSigGen )
        {
            strAddTrcInfo  = "Object: SigGen";
        }
        else if( i_pObj == m_pSigGenThread )
        {
            strAddTrcInfo  = "Object: SigGenThread";
        }
        else if( i_pObj == m_pSigAna )
        {
            strAddTrcInfo  = "Object: SigAna";
        }
        else if( i_pObj == m_pSigAnaThread )
        {
            strAddTrcInfo  = "Object: SigAnaThread";
        }
        else if( i_pObj == m_pTestModule1 )
        {
            strAddTrcInfo  = "Object: TestModule1";
        }
        else if( i_pObj == m_pTestModule2Thread )
        {
            strAddTrcInfo  = "Object: TestModule2Thread";
        }
        else if( i_pObj == m_pTestModule2 )
        {
            strAddTrcInfo  = "Object: TestModule2";
        }

        strAddTrcInfo += ", State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onObjectDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pObj == m_pRemCmdServer )
    {
        m_pRemCmdServer = nullptr;
    }
    else if( i_pObj == m_pDb )
    {
        m_pDb = nullptr;
    }
    else if( i_pObj == m_pDbThread )
    {
        m_pDbThread = nullptr;
        m_pDb = nullptr;
    }
    else if( i_pObj == m_pSigGen )
    {
        m_pSigGen = nullptr;
    }
    else if( i_pObj == m_pSigGenThread )
    {
        m_pSigGenThread = nullptr;
        m_pSigGen = nullptr;
    }
    else if( i_pObj == m_pSigAna )
    {
        m_pSigAna = nullptr;
    }
    else if( i_pObj == m_pSigAnaThread )
    {
        m_pSigAnaThread = nullptr;
        m_pSigAna = nullptr;
    }
    else if( i_pObj == m_pTestModule1 )
    {
        m_pTestModule1 = nullptr;
    }
    else if( i_pObj == m_pTestModule2Thread )
    {
        m_pTestModule2Thread = nullptr;
        m_pTestModule2 = nullptr;
    }
    else if( i_pObj == m_pTestModule2 )
    {
        m_pTestModule2 = nullptr;
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", RequestInProgress: " + requestInProgress2Str();
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // onObjectDestroyed
