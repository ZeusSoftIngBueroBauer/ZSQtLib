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

#include "App.h"

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSysGUI/ZSSysErrLogModel.h"

#include <QtQml/qqmlapplicationengine.h>
#include <QtQml/qqmlcontext.h>
#include <QtQuickControls2/qquickstyle.h>
#include <QtQuick/qquickview.h>
#include <QtQuick/qquickwindow.h>

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Apps::Products::IpcServer;
using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;


/*******************************************************************************
class CApplication : public QGuiApplication
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
    const QString& i_strAppVersion,
    const QString& i_strWindowTitle ) :
//------------------------------------------------------------------------------
    QGuiApplication(i_argc,i_argv),
    m_pQmlAppEngine(nullptr),
    m_pMainWindow(nullptr),
    m_pErrLogModel(nullptr),
    m_pTrcServer(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName("theApp");

    if( thread()->objectName().length() == 0 )
    {
        thread()->setObjectName("GUIMain");
    }

    // Resources (like images) but not Qml modules are compiled into the ZSSysGUI dll.
    ZS::System::GUI::qInitResources();
    ZS::System::GUI::qInitResourcesQml();

    // Qml modules from ZSSysGUI are compiled into the applications executable.
    //Q_INIT_RESOURCE(ZSSysGUI);

    //QDirIterator qrc(":", QDirIterator::Subdirectories);
    //qDebug("qrcs BEGIN -------------------------------------");
    //while(qrc.hasNext())
    //{
    //    qDebug() << qrc.next();
    //}
    //qDebug("qrcs END ---------------------------------------");

    QIcon iconApp(":/Images/ZSAppIpcServer.ico");

    setWindowIcon(iconApp);
    setOrganizationName(i_strOrganizationName);
    setOrganizationDomain(i_strOrganizationDomain);
    setApplicationName(i_strAppName);
    setApplicationVersion(i_strAppVersion);
    setApplicationDisplayName(i_strWindowTitle);

    CErrLog::CreateInstance();

    m_pTrcServer = CIpcTrcServer::CreateInstance();
    m_pTrcServer->setCacheTrcDataIfNotConnected(true);
    m_pTrcServer->setCacheTrcDataMaxArrLen(5000);
    m_pTrcServer->recallAdminObjs();
    m_pTrcServer->startup();

    m_pTrcAdminObj = m_pTrcServer->GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    m_pErrLogModel = new CModelErrLog(CErrLog::GetInstance());

    QQuickStyle::setStyle("fusion");

    m_pQmlAppEngine = new QQmlApplicationEngine();
    // Add import path to the applications resource storage.
    m_pQmlAppEngine->addImportPath("qrc:/");

    QQmlContext* pQmlCtx = m_pQmlAppEngine->rootContext();

    pQmlCtx->setContextProperty("_trcServer", m_pTrcServer);
    pQmlCtx->setContextProperty("_errLog", CErrLog::GetInstance());
    pQmlCtx->setContextProperty("_errLogModel", m_pErrLogModel);

    //qDebug("QmlAppEngine.importPaths BEGIN ---------------------------------------");
    //QStringList strlstImportPathList = m_pQmlAppEngine->importPathList();
    //QStringList::iterator itPath = strlstImportPathList.begin();
    //while(itPath != strlstImportPathList.end())
    //{
    //    qDebug() << *itPath;
    //    ++itPath;
    //}
    //qDebug("QmlAppEngine.importPaths END -----------------------------------------");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        m_pQmlAppEngine, &QQmlApplicationEngine::objectCreated,
        this, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    m_pQmlAppEngine->load(url);

    if (m_pQmlAppEngine->rootObjects().isEmpty())
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo(
                NameSpace(), ClassName(), objectName(), "ctor",
                EResultInternalProgramError, EResultSeverityCritical,
                "Couldn't load " + url.toString());
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else
    {
        QObject* pViewObj = m_pQmlAppEngine->rootObjects().at(0);

        m_pMainWindow = dynamic_cast<QQuickWindow*>(pViewObj);

        if( m_pMainWindow == nullptr)
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                SErrResultInfo errResultInfo(
                    NameSpace(), ClassName(), objectName(), "ctor",
                    EResultInternalProgramError, EResultSeverityCritical,
                    "No MainWindow created");
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
        else
        {
            m_pMainWindow->showNormal();
        }
    }

} // ctor

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pQmlAppEngine;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pErrLogModel;
    }
    catch(...)
    {
    }
    m_pErrLogModel = nullptr;

    CIpcTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    if( m_pTrcServer != nullptr )
    {
        m_pTrcServer->saveAdminObjs();
    }

    CIpcTrcServer::ReleaseInstance();
    m_pTrcServer = nullptr;

    CErrLog::ReleaseInstance();

    m_pQmlAppEngine = nullptr;
    m_pMainWindow = nullptr;
    m_pErrLogModel = nullptr;
    m_pTrcServer = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor
