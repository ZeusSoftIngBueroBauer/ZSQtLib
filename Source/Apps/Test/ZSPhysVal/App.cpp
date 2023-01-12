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

#include "App.h"
#include "PhysSizes/Kinematics/Kinematics.h"
#include "MainWindow.h"
#include "Test.h"

#include "ZSPhysVal/ZSPhysSizesIdxTree.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSTest/ZSTestStepIdxTree.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"

#include <QtCore/qdir.h>
#include <QtCore/qthread.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


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
    m_pTest(nullptr),
    m_pMainWindow(nullptr),
    m_bAutoStartTest(false),
    m_pIdxTreePhysSizes(nullptr),
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

    QPixmap pxmApp32x32(":/ZS/App/ZeusSoft_32x32.png");
    QPixmap pxmApp48x48(":/ZS/App/ZeusSoft_48x48.png");
    QPixmap pxmApp64x64(":/ZS/App/ZeusSoft_64x64.png");

    iconApp.addPixmap(pxmApp32x32);
    iconApp.addPixmap(pxmApp48x48);
    iconApp.addPixmap(pxmApp64x64);

    QApplication::setWindowIcon(iconApp);

    // Parse command arguments (first part, IniFile)
    //----------------------------------------------

    int         idxArg;
    QString     strArg;
    QString     strVal;
    QStringList strListArgsPar;
    QStringList strListArgsVal;

    parseAppArgs( i_argc, i_argv, strListArgsPar, strListArgsVal );

    #if QT_VERSION >= 0x040501
    for( idxArg = 0; idxArg < strListArgsPar.length() && idxArg < strListArgsVal.length(); idxArg++ )
    #else
    for( idxArg = 0; idxArg < strListArgsPar.size() && idxArg < strListArgsVal.size(); idxArg++ )
    #endif
    {
        strArg = strListArgsPar[idxArg];
        strVal = strListArgsVal[idxArg];

        if( strArg == "AutoStartTest" )
        {
            m_bAutoStartTest = true;
        }
    }

    // Create error manager
    //------------------------

    CErrLog::CreateInstance();

    // Trace Server
    //-------------

    m_pTrcServer = CIpcTrcServer::CreateInstance();
    m_pTrcServer->setLocalTrcFileSubFileLineCountMax(10000);

    m_pTrcServer->setCacheTrcDataIfNotConnected(true);
    m_pTrcServer->setCacheTrcDataMaxArrLen(5000);
    m_pTrcServer->recallAdminObjs();
    m_pTrcServer->startup();

    m_pTrcAdminObj = m_pTrcServer->GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

    // Physical Sizes
    //----------------

    //m_pIdxTreePhysSizes = CIdxTreePhysSizes::CreateInstance();
    //Kinematics = new CPhysScienceFieldKinematics(m_pIdxTreePhysSizes);

    //Kinematics = std::move(kinematics);

    // Test
    //----------------

    m_pTest = new CTest();

    // Main Window
    //------------

    m_pMainWindow = new CMainWindow(i_strWindowTitle, m_pTest);
    m_pMainWindow->show();

} // ctor

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
    try {
        delete m_pMainWindow;
    }
    catch(...) {
    }
    m_pMainWindow = nullptr;

    try {
        delete m_pTest;
    }
    catch(...) {
    }
    m_pTest = nullptr;

    //try {
    //    delete Kinematics;
    //}
    //catch(...) {
    //}
    //Kinematics = nullptr;

    //CPhysScienceFieldKinematics kinematics;
    //Kinematics = std::move(kinematics);

    // !! Please note !!
    // On releasing the index tree the tree together with all its nodes would be deleted.
    // The physical size nodes are created as instance members of the Kinematics global instance.
    // The phyiscal units are instance members of the physical sizes created by the Kinematics global instance.
    // The index tree nodes are not created on the heap and cannot be deleted separately with the delete operator.
    // We therefore need to remove (delete) the Kinematics global instance before the index tree.
    // The dtor of the global Kinematics will invoke the dtor of the its instance members (physical sizes) and
    // the dtor of the global physical sizes will invoke the dtor of its instance members (physical units).
    // The dtors will remove themselves from the index tree and afterward the index tree can be deleted.
    if( m_pIdxTreePhysSizes != nullptr ) {
        CIdxTreePhysSizes::ReleaseInstance();
        m_pIdxTreePhysSizes = nullptr;
    }

    if( m_pTrcAdminObj != nullptr ) {
        CIpcTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
        m_pTrcAdminObj = nullptr;
    }

    if( m_pTrcServer != nullptr ) {
        m_pTrcServer->saveAdminObjs();
        CIpcTrcServer::ReleaseInstance();
        m_pTrcServer = nullptr;
    }

    CErrLog::ReleaseInstance();

    m_bAutoStartTest = false;

} // dtor
