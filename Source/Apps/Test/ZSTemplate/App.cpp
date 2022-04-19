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
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>

#include "App.h"
#include "MainWindow.h"

#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::Template;


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
    return dynamic_cast<CApplication*>(QCoreApplication::instance());
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
    m_trcServerHostSettings(24763),
    m_trcServerSettings(),
    m_pTrcServer(nullptr),
    m_pTest(nullptr),
    m_pMainWindow(nullptr)
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

    QPixmap pxmApp16x16(":/ZS/App/ZeusSoft_16x16.bmp");
    QPixmap pxmApp32x32(":/ZS/App/ZeusSoft_32x32.bmp");
    QPixmap pxmApp48x48(":/ZS/App/ZeusSoft_48x48.bmp");
    QPixmap pxmApp64x64(":/ZS/App/ZeusSoft_64x64.bmp");

    pxmApp16x16.setMask(pxmApp16x16.createHeuristicMask());
    pxmApp32x32.setMask(pxmApp32x32.createHeuristicMask());
    pxmApp48x48.setMask(pxmApp48x48.createHeuristicMask());
    pxmApp64x64.setMask(pxmApp64x64.createHeuristicMask());

    iconApp.addPixmap(pxmApp16x16);
    iconApp.addPixmap(pxmApp32x32);
    iconApp.addPixmap(pxmApp48x48);
    iconApp.addPixmap(pxmApp64x64);

    QApplication::setWindowIcon(iconApp);

    ZS::Ipc::SServerHostSettings  trcServerHostSettingsDefault = m_trcServerHostSettings;
    ZS::Trace::STrcServerSettings trcServerSettingsDefault = m_trcServerSettings;

    // Create error manager
    //------------------------

    CErrLog::CreateInstance();

    // Create trace server
    //------------------------

    m_pTrcServer = ZS::Trace::CIpcTrcServer::CreateInstance();

    m_pTrcServer->setHostSettings(m_trcServerHostSettings);
    m_pTrcServer->setTraceSettings(m_trcServerSettings);
    m_pTrcServer->recallAdminObjs();
    m_pTrcServer->startup();

    // Test
    //----------------------------

    m_pTest = new CTest();

    // Main Window
    //------------

    m_pMainWindow = new CMainWindow(i_strWindowTitle);
    m_pMainWindow->show();

} // ctor

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
    // Save settings of the application
    //--------------------------------------

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
        ZS::Trace::CTrcServer::ReleaseInstance();
    }
    catch(...)
    {
    }

    CErrLog::ReleaseInstance();

    m_pTest = nullptr;
    m_pMainWindow = nullptr;

} // dtor

/*==============================================================================
public slots: // instance methods of system shutdown
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onLastWindowClosed()
//------------------------------------------------------------------------------
{
    quit();
}
