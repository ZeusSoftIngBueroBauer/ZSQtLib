/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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
#include "MainWindow.h"
#include "Test/Test.h"

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSysGUI/ZSSysErrLogModel.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

//#include <QtCore/qdir.h>
//#include <QtCore/qfileinfo.h>
//#include <QtCore/qsettings.h>
//#include <QtCore/qthread.h>
//#include <QtCore/qtimer.h>
//#include <QtGui/qbitmap.h>
//#include <QtGui/qevent.h>
//
//#if QT_VERSION < 0x050000
//#include <QtGui/qlabel.h>
//#include <QtGui/qmenubar.h>
//#include <QtGui/qmessagebox.h>
//#include <QtGui/qstatusbar.h>
//#else
//#include <QtWidgets/qlabel.h>
//#include <QtWidgets/qmenubar.h>
//#include <QtWidgets/qmessagebox.h>
//#include <QtWidgets/qstatusbar.h>
//#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Apps::Products::Draw;


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
    const QString& i_strAppName ) :
//------------------------------------------------------------------------------
    CGUIApp(i_argc,i_argv),
    m_pTrcServer(nullptr),
    m_pTest(nullptr),
    m_pMainWindow(nullptr),
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
    // On some desktop platforms (including Windows and Unix), the application name
    // (from QGuiApplication::applicationDisplayName) is added at the end of the
    // window title, if set. This is done by the QPA plugin, so it is shown to the user,
    // but isn't part of the windowTitle string. !! This is not desired !!
    //QGuiApplication::setApplicationDisplayName(i_strWindowTitle);

    QIcon iconApp;

    QPixmap pxmApp32x32(":/ZS/App/ZeusSoft_32x32.png");
    QPixmap pxmApp48x48(":/ZS/App/ZeusSoft_48x48.png");
    QPixmap pxmApp64x64(":/ZS/App/ZeusSoft_64x64.png");

    iconApp.addPixmap(pxmApp32x32);
    iconApp.addPixmap(pxmApp48x48);
    iconApp.addPixmap(pxmApp64x64);

    QApplication::setWindowIcon(iconApp);

    setStyleSheet(
        "QDoubleSpinBox:read-only { "
            "color: #7070F0;"
            "background-color: #F8F8F8; }"
        "QSpinBox:read-only { "
            "color: #7070F0;"
            "background-color: #F8F8F8; }"
        "QLineEdit:read-only { "
            "color: #7070F0;"
            "background-color: #F8F8F8; }");

    // Create error manager
    //------------------------

    CErrLog::CreateInstance();

    // Create trace server
    //--------------------

    m_pTrcServer = ZS::Trace::CIpcTrcServer::CreateInstance();

    m_pTrcServer->setCacheTrcDataIfNotConnected(true);
    m_pTrcServer->setCacheTrcDataMaxArrLen(1000);
    m_pTrcServer->recallAdminObjs();
    m_pTrcServer->startup();

    // Get trace admin object
    //-----------------------

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CApplication", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // Parse command arguments
    //------------------------

    QStringList strlstArgsPar;
    QStringList strlstArgsVal;

    ZS::System::parseAppArgs(i_argc, i_argv, strlstArgsPar, strlstArgsVal);

    bool bTest = false;

    #if QT_VERSION >= 0x040501
    for( int idxArg = 0; idxArg < strlstArgsPar.length() && idxArg < strlstArgsVal.length(); idxArg++ )
    #else
    for( int idxArg = 0; idxArg < strListArgsPar.size() && idxArg < strListArgsVal.size(); idxArg++ )
    #endif
    {
        QString strArg = strlstArgsPar[idxArg];
        QString strVal = strlstArgsVal[idxArg];

        if( strArg.compare("Test",Qt::CaseInsensitive) == 0 )
        {
            bTest = true;
        }
    }

    // Create Test (if desired)
    //-------------------------

    if( bTest )
    {
        m_pTest = new CTest();
    }
}

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // Save settings of the application
    //--------------------------------------

    m_pTrcServer->saveAdminObjs();

    // Destroy objects created and controlled by the application
    //----------------------------------------------------------

    try
    {
        delete m_pMainWindow;
    }
    catch(...)
    {
    }
    m_pMainWindow = nullptr;

    try
    {
        delete m_pTest;
    }
    catch(...)
    {
    }
    m_pTest = nullptr;

    // Please note that the trace server must be deleted after the application has been
    // destroyed and the main event loop has been quit. There might still be events (messages)
    // pending while the application is quit whose dtor will be called. If the message dtor
    // wants to output traces the server must still be alive. Otherwise the trace admin
    // objects referenced by the message dtor would have already been destroyed before the dtor
    // of the event (message) is called.

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcServer->saveAdminObjs();

    try
    {
        ZS::System::CTrcServer::ReleaseInstance();
    }
    catch(...)
    {
    }
    m_pTrcServer = nullptr;

    CErrLog::ReleaseInstance();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::createAndShowMainWindow(
    const QString&     i_strMainWindowTitle,
    const QString&     i_strFileName,
    const QStringList& i_strlstObjFactories)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "MainWindowTitle: " + i_strMainWindowTitle;
        strMthInArgs += ", FileName: " + i_strFileName;
        strMthInArgs += ", ObjFactories: " + i_strlstObjFactories.join(", ");
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showMainWindow",
        /* strAddInfo   */ strMthInArgs );

    setQuitOnLastWindowClosed(false);

    QObject::connect(
        this, &CApplication::lastWindowClosed,
        this, &CApplication::onLastWindowClosed);

    m_pMainWindow = new CMainWindow(
        /* strWindowTitleAppName */ i_strMainWindowTitle,
        /* pTest                 */ m_pTest,
        /* strlstObjFactories    */ i_strlstObjFactories );
    m_pMainWindow->show();

    if (m_pTest == nullptr) {
        if (i_strFileName.isEmpty()) {
            m_pMainWindow->onActionFilePageSetupTriggered(false);
        }
        else {
            m_pMainWindow->setCurrentUsedFile(i_strFileName);
        }
    }
    else {
        m_pTest->setMainWindow(m_pMainWindow);
        m_pMainWindow->onActionDebugTestTriggered(false);
    }
}

/*==============================================================================
protected slots: // instance methods of system shutdown
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onLastWindowClosed()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onLastWindowClosed",
        /* strAddInfo   */ "" );

    // Quit the application (terminate main event loop)
    quit();
}
