/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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
#include <QtCore/qtimer.h>
#include <QtCore/qthread.h>
#include <QtGui/qbitmap.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qmessagebox.h>
#else
#include <QtWidgets/qmessagebox.h>
#endif

#include "App.h"
#include "MainWindow.h"

#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Apps::Products::MethodTraceClient;


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
    return dynamic_cast<CApplication*>(qApp);
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
    m_bReqExecTreeGarbageCollectorEnabled(true),
    m_fReqExecTreeGarbageCollectorInterval_s(5.0),
    m_fReqExecTreeGarbageCollectorElapsed_s(60.0),
    m_pReqExecTree(nullptr),
    // Trace Client
    m_trcClientHostSettings(
        /* strRemoteHost      */ "127.0.0.1",
        /* uRemotePort        */ 24763,
        /* iConnectTimeout_ms */ 5000 ),
    m_pTrcClient(nullptr),
    // Main Window
    m_pMainWindow(nullptr)
{
    setObjectName("theApp");

    if( thread()->objectName().length() == 0 )
    {
        thread()->setObjectName("GUIMain");
    }

    QIcon iconApp;

    QPixmap pxmApp32x32(":/ZS/App/ZeusSoft_32x32.png");
    QPixmap pxmApp48x48(":/ZS/App/ZeusSoft_48x48.png");
    QPixmap pxmApp64x64(":/ZS/App/ZeusSoft_64x64.png");

    iconApp.addPixmap(pxmApp32x32);
    iconApp.addPixmap(pxmApp48x48);
    iconApp.addPixmap(pxmApp64x64);

    QApplication::setWindowIcon(iconApp);

    SClientHostSettings trcClientHostSettingsDefault = m_trcClientHostSettings;

    // Parse command arguments (first part, IniFile)
    //----------------------------------------------

    int         idxArg;
    QString     strArg;
    QString     strVal;
    int         iVal;
    bool        bConverted;
    QStringList strListArgsPar;
    QStringList strListArgsVal;
    QString     strAppName = i_strAppName;
    QString     strWindowTitle = i_strWindowTitle;
    QString     strRemoteAppName;

    // Range of IniFileScope: ["AppDir", "User", "System"]
    #ifdef __linux__
    // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
    // where the application has not write access rights. Stupid ...
    QString strIniFileScope = "User";
    #else
    QString strIniFileScope = "System"; // Default
    #endif

    parseAppArgs( i_argc, i_argv, strListArgsPar, strListArgsVal );

    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
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
        else if( strArg.compare("RemoteAppName",Qt::CaseInsensitive) == 0 )
        {
            strRemoteAppName = strVal;
        }
    }

    if( !strRemoteAppName.isEmpty() )
    {
        strAppName += "-" + strRemoteAppName;
        strWindowTitle += " / " + strRemoteAppName;
    }

    QCoreApplication::setOrganizationName(i_strOrganizationName);
    QCoreApplication::setOrganizationDomain(i_strOrganizationDomain);
    QCoreApplication::setApplicationName(strAppName);

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

    m_pSettingsFile = new QSettings(strIniFileAbsFilePath, QSettings::IniFormat);

    readSettings();

    // Parse command arguments (second part, overwriting IniFile settings)
    //--------------------------------------------------------------------

    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    for( idxArg = 0; idxArg < strListArgsPar.length() && idxArg < strListArgsVal.length(); idxArg++ )
    #else
    for( idxArg = 0; idxArg < strListArgsPar.size() && idxArg < strListArgsVal.size(); idxArg++ )
    #endif
    {
        strArg = strListArgsPar[idxArg];
        strVal = strListArgsVal[idxArg];

        if( strArg.compare("RemoteHostName",Qt::CaseInsensitive) == 0 )
        {
            m_trcClientHostSettings.m_strRemoteHostName = strVal;
        }
        else if( strArg.compare("RemotePort",Qt::CaseInsensitive) == 0 )
        {
            iVal = strVal.toInt(&bConverted);
            if( bConverted && iVal >= 1000 && iVal <= UINT16_MAX )
            {
                m_trcClientHostSettings.m_uRemotePort = static_cast<quint16>(iVal);
            }
        }
    }

    // Create error manager
    //------------------------

    CErrLog::CreateInstance();

    // Request Execution Tree
    //------------------------

    m_pReqExecTree = CRequestExecTree::CreateInstance();

    m_pReqExecTree->setGarbageCollectorEnabled(m_bReqExecTreeGarbageCollectorEnabled);
    m_pReqExecTree->setGarbageCollectorIntervalInSec(m_fReqExecTreeGarbageCollectorInterval_s);
    m_pReqExecTree->setGarbageCollectorElapsedInSec(m_fReqExecTreeGarbageCollectorElapsed_s);

    // Trace client
    //-------------

    m_pTrcClient = new CIpcTrcClient("MthTrcClient");

    m_pTrcClient->setWatchDogTimerUsed(false);
    m_pTrcClient->setHostSettings(m_trcClientHostSettings);
    m_pTrcClient->changeSettings();

    // Main Window
    //------------

    //setQuitOnLastWindowClosed(false);

    //if( !QObject::connect(
    //    /* pObjSender   */ this,
    //    /* szSignal     */ SIGNAL(lastWindowClosed()),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onLastWindowClosed()) ) )
    //{
    //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    //}

    m_pMainWindow = new CMainWindow(strWindowTitle, m_pTrcClient);
    m_pMainWindow->show();

} // ctor

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
    saveSettings();

    try
    {
        delete m_pMainWindow;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pTrcClient;
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

    CErrLog::ReleaseInstance();

    m_pSettingsFile = nullptr;
    m_bReqExecTreeGarbageCollectorEnabled = false;
    m_fReqExecTreeGarbageCollectorInterval_s = 0.0;
    m_fReqExecTreeGarbageCollectorElapsed_s = 0.0;
    m_pReqExecTree = nullptr;
    //m_trcClientHostSettings;
    m_pTrcClient = nullptr;
    m_pMainWindow = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CIpcTrcClient* CApplication::getTrcClient()
//------------------------------------------------------------------------------
{
    return m_pTrcClient;
}

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

        // Trace Client
        //-------------

        strSettingsKey = "TrcClient";
        bSyncSettings  = false;

        if( m_pSettingsFile->contains(strSettingsKey+"/SocketType") )
        {
            m_trcClientHostSettings.m_socketType = str2SocketType( m_pSettingsFile->value(strSettingsKey+"/SocketType",socketType2Str(m_trcClientHostSettings.m_socketType)).toString() );
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/SocketType", socketType2Str(m_trcClientHostSettings.m_socketType) );
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/RemoteHostName") )
        {
            m_trcClientHostSettings.m_strRemoteHostName = m_pSettingsFile->value(strSettingsKey+"/RemoteHostName",m_trcClientHostSettings.m_strRemoteHostName).toString();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/RemoteHostName", m_trcClientHostSettings.m_strRemoteHostName );
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/RemotePort") )
        {
            m_trcClientHostSettings.m_uRemotePort = m_pSettingsFile->value(strSettingsKey+"/RemotePort",m_trcClientHostSettings.m_uRemotePort).toUInt();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/RemotePort", m_trcClientHostSettings.m_uRemotePort );
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/ConnectTimeout_ms") )
        {
            m_trcClientHostSettings.m_iConnectTimeout_ms = m_pSettingsFile->value(strSettingsKey+"/ConnectTimeout_ms",m_trcClientHostSettings.m_iConnectTimeout_ms).toInt();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/ConnectTimeout_ms", m_trcClientHostSettings.m_iConnectTimeout_ms );
            bSyncSettings = true;
        }

        if( m_pSettingsFile->contains(strSettingsKey+"/BufferSize") )
        {
            m_trcClientHostSettings.m_uBufferSize = m_pSettingsFile->value(strSettingsKey+"/BufferSize",m_trcClientHostSettings.m_uBufferSize).toUInt();
        }
        else
        {
            m_pSettingsFile->setValue( strSettingsKey+"/BufferSize", m_trcClientHostSettings.m_uBufferSize );
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

        if( m_pTrcClient != nullptr )
        {
            strSettingsKey = "TrcClient";

            SClientHostSettings trcClientSettings = m_pTrcClient->getHostSettings();

            m_pSettingsFile->setValue( strSettingsKey+"/SocketType", socketType2Str(trcClientSettings.m_socketType) );
            m_pSettingsFile->setValue( strSettingsKey+"/RemoteHostName", trcClientSettings.m_strRemoteHostName );
            m_pSettingsFile->setValue( strSettingsKey+"/RemotePort", trcClientSettings.m_uRemotePort );
            m_pSettingsFile->setValue( strSettingsKey+"/ConnectTimeout_ms", trcClientSettings.m_iConnectTimeout_ms );
            m_pSettingsFile->setValue( strSettingsKey+"/BufferSize", trcClientSettings.m_uBufferSize );

        } // if( m_pTrcClient != nullptr )

        m_pSettingsFile->sync();

    } // if( m_pSettingsFile != nullptr )

} // saveSettings

///*==============================================================================
//public slots: // instance methods of system shutdown
//==============================================================================*/
//
////------------------------------------------------------------------------------
//void CApplication::shutdown()
////------------------------------------------------------------------------------
//{
//    m_pTrcClient->abortAllRequests();
//
//    // Quit the application (terminate main event loop)
//    quit();
//
//} // shutdown
//
////------------------------------------------------------------------------------
//void CApplication::onLastWindowClosed()
////------------------------------------------------------------------------------
//{
//    QTimer::singleShot( 100, this, SLOT(shutdown()) );
//}
