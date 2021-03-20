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
#else
#include <QtWidgets/qmessagebox.h>
#endif

#include "App.h"
#include "MainWindow.h"

#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Apps::Test::IpcTraceDllIfQtApp;


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
    m_strErrLogFileAbsFilePath(),
    m_strTestStepsFileAbsFilePath(),
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

    QPixmap pxmApp16x16(":/ZS/App/Zeus16x16.bmp");
    QPixmap pxmApp32x32(":/ZS/App/Zeus32x32.bmp");

    pxmApp16x16.setMask(pxmApp16x16.createHeuristicMask());
    pxmApp32x32.setMask(pxmApp32x32.createHeuristicMask());

    iconApp.addPixmap(pxmApp16x16);
    iconApp.addPixmap(pxmApp32x32);

    QApplication::setWindowIcon(iconApp);

    // Parse command arguments
    //------------------------

    int         idxArg;
    QString     strArg;
    QString     strVal;
    QStringList strListArgsPar;
    QStringList strListArgsVal;

    // Range of IniFileScope: ["AppDir", "User", "System"]
    #ifdef __linux__
    // Using "System" on linux Mint ends up in directory "etc/xdg/<CompanyName>"
    // where the application has no write access rights. Stupid ...
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

    // Calculate default file paths
    //-----------------------------

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

    QString strErrLogFileBaseName = strAppNameNormalized + "-Error";
    QString strErrLogFileSuffix = "xml";

    m_strErrLogFileAbsFilePath = strAppLogDir + "/" + strErrLogFileBaseName + "." + strErrLogFileSuffix;

    QString strTestStepsFileBaseName = strAppNameNormalized + "-TestSteps";
    QString strTestStepsFileSuffix = "xml";

    m_strTestStepsFileAbsFilePath = strAppConfigDir + "/" + strTestStepsFileBaseName + "." + strTestStepsFileSuffix;

    // Create error manager
    //------------------------

    CErrLog::CreateInstance(true, m_strErrLogFileAbsFilePath);

    // Test
    //----------------------------

    m_pTest = new CTest(m_strTestStepsFileAbsFilePath);

    // Main Window
    //------------

    m_pMainWindow = new CMainWindow(i_strWindowTitle);
    m_pMainWindow->show();

} // ctor

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
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

    CErrLog::DestroyInstance();

    //m_strErrLogFileAbsFilePath;
    //m_strTestStepsFileAbsFilePath;
    m_pTest = nullptr;
    m_pMainWindow = nullptr;

} // dtor
