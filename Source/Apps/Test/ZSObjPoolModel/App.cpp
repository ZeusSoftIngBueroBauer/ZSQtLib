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
#include "MainWindow.h"
#include "WidgetCentral.h"

#include "ZSSysGUI/ZSSysObjPoolModel.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Apps::Test::ObjPoolModel;


//******************************************************************************
class CObjTest : public QObject
//******************************************************************************
{
public: // ctors and dtor
    CObjTest( const QString& i_strNameSpace, const QString& i_strObjName, CObjPool& i_objPool ) :
        m_strNameSpace(i_strNameSpace),
        m_strObjName(i_strObjName),
        m_iObjId(-1),
        m_pObjPool(&i_objPool)
    {
        m_iObjId = m_pObjPool->addObjNode(
            /* strNameSpace */ m_strNameSpace,
            /* strObjName   */ m_strObjName,
            /* pvObj        */ this );
    }
    ~CObjTest()
    {
        m_pObjPool->invalidateObjNode(m_iObjId);
        m_pObjPool = nullptr;
    }
protected: // instance members
    QString   m_strNameSpace;
    QString   m_strObjName;
    int       m_iObjId;
    CObjPool* m_pObjPool;

}; // class CObjTest


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
    m_bReqExecTreeGarbageCollectorEnabled(true),
    m_fReqExecTreeGarbageCollectorInterval_s(5.0),
    m_fReqExecTreeGarbageCollectorElapsed_s(60.0),
    m_pReqExecTree(nullptr),
    m_objPool(
        /* strObjPoolName         */ "TestObjPool",
        /* strNodeSeparater       */ "::",
        /* bSortTreeEntriesByName */ true ),
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

    // Error Log Model
    //----------------

    CErrLog::CreateInstance(true, m_strErrLogFileAbsFilePath);

    // Request Execution Tree
    //------------------------

    m_pReqExecTree = CRequestExecTree::CreateInstance();

    m_pReqExecTree->setGarbageCollectorEnabled(m_bReqExecTreeGarbageCollectorEnabled);
    m_pReqExecTree->setGarbageCollectorIntervalInSec(m_fReqExecTreeGarbageCollectorInterval_s);
    m_pReqExecTree->setGarbageCollectorElapsedInSec(m_fReqExecTreeGarbageCollectorElapsed_s);

    // Main Window
    //------------

    m_pMainWindow = new CMainWindow( i_strWindowTitle, &m_objPool );
    m_pMainWindow->show();

    // Object pool
    //-------------

    int idxClm;

    CWidgetCentral::GetInstance()->getTreeView()->expandAll();

    for( idxClm = 0; idxClm < CModelObjPool::EColumnCount; idxClm++ )
    {
        CWidgetCentral::GetInstance()->getTreeView()->resizeColumnToContents(idxClm);
    }

    QTimer::singleShot( 1000, this, SLOT(onCreateZSIpcClientObjectsTimeout()) );

} // ctor

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
    // Save settings of the application
    //--------------------------------------

    saveSettings();

    // Destroy objects within the object pool
    //----------------------------------------------------------

    CObjTest* pObjTest;
    int       iObjId;

    if( m_objPool.getObjCount() > 0 )
    {
        for( iObjId = static_cast<int>(m_objPool.getObjCount())-1; iObjId >= 0; iObjId-- )
        {
            pObjTest = reinterpret_cast<CObjTest*>(m_objPool.getObj(iObjId));

            try
            {
                delete pObjTest;
            }
            catch(...)
            {
            }
            pObjTest = nullptr;
        }
    }

    // Destroy objects created and controlled by the application
    //----------------------------------------------------------

    try
    {
        delete m_pMainWindow;
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
    m_pReqExecTree = nullptr;
    m_pMainWindow = nullptr;

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
    } // if( m_pSettingsFile != nullptr )

} // saveSettings

/*==============================================================================
public slots: // instance methods of system shutdown
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onLastWindowClosed()
//------------------------------------------------------------------------------
{
    QTimer::singleShot( 200, this, SLOT(quit()) );
}

/*==============================================================================
public: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onCreateZSIpcClientObjectsTimeout()
//------------------------------------------------------------------------------
{
    new CObjTest(
        /* strNameSpace */ "ZS::Ipc::CClient",
        /* strObjName   */ "Download",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::Ipc::CClientGateway",
        /* strObjName   */ "Download",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::Ipc::CClient",
        /* strObjName   */ "Upload",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::Ipc::CClientGateway",
        /* strObjName   */ "Upload",
        /* objPool      */ m_objPool );

    int idxClm;

    CWidgetCentral::GetInstance()->getTreeView()->expandAll();

    for( idxClm = 0; idxClm < CModelObjPool::EColumnCount; idxClm++ )
    {
        CWidgetCentral::GetInstance()->getTreeView()->resizeColumnToContents(idxClm);
    }

    QTimer::singleShot( 1000, this, SLOT(onCreateZSIpcServerObjectsTimeout()) );

} // onCreateZSIpcClientObjectsTimeout

//------------------------------------------------------------------------------
void CApplication::onCreateZSIpcServerObjectsTimeout()
//------------------------------------------------------------------------------
{
    new CObjTest(
        /* strNameSpace */ "ZS::Ipc::CServer",
        /* strObjName   */ "Download",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::Ipc::CServerGateway",
        /* strObjName   */ "Download",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::Ipc::CServer",
        /* strObjName   */ "Upload",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::Ipc::CServerGateway",
        /* strObjName   */ "Upload",
        /* objPool      */ m_objPool );

    int idxClm;

    CWidgetCentral::GetInstance()->getTreeView()->expandAll();

    for( idxClm = 0; idxClm < CModelObjPool::EColumnCount; idxClm++ )
    {
        CWidgetCentral::GetInstance()->getTreeView()->resizeColumnToContents(idxClm);
    }

    QTimer::singleShot( 1000, this, SLOT(onCreateZSTraceObjectsTimeout()) );

} // onCreateZSIpcServerObjectsTimeout

//------------------------------------------------------------------------------
void CApplication::onCreateZSTraceObjectsTimeout()
//------------------------------------------------------------------------------
{
    new CObjTest(
        /* strNameSpace */ "ZS::Trace::ContextTraceServer",
        /* strObjName   */ "Download",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::Trace::MessageTraceServer",
        /* strObjName   */ "Download",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::Trace::MethodTraceServer",
        /* strObjName   */ "Download",
        /* objPool      */ m_objPool );

    int idxClm;

    CWidgetCentral::GetInstance()->getTreeView()->expandAll();

    for( idxClm = 0; idxClm < CModelObjPool::EColumnCount; idxClm++ )
    {
        CWidgetCentral::GetInstance()->getTreeView()->resizeColumnToContents(idxClm);
    }

    QTimer::singleShot( 1000, this, SLOT(onCreateZSPhysValObjectsTimeout()) );

} // onCreateZSTraceObjectsTimeout

//------------------------------------------------------------------------------
void CApplication::onCreateZSPhysValObjectsTimeout()
//------------------------------------------------------------------------------
{
    new CObjTest(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Voltage",
        /* strObjName   */ "Volt",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Voltage",
        /* strObjName   */ "MilliVolt",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Voltage",
        /* strObjName   */ "MicroVolt",
        /* objPool      */ m_objPool );

    new CObjTest(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Current",
        /* strObjName   */ "Ampere",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Current",
        /* strObjName   */ "MilliAmpere",
        /* objPool      */ m_objPool );
    new CObjTest(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Current",
        /* strObjName   */ "MicroAmpere",
        /* objPool      */ m_objPool );

    int idxClm;

    CWidgetCentral::GetInstance()->getTreeView()->expandAll();

    for( idxClm = 0; idxClm < CModelObjPool::EColumnCount; idxClm++ )
    {
        CWidgetCentral::GetInstance()->getTreeView()->resizeColumnToContents(idxClm);
    }

    QTimer::singleShot( 5000, this, SLOT(onDestroyZSPhysValObjectsTimeout()) );

} // onCreateZSPhysValObjectsTimeout

//------------------------------------------------------------------------------
void CApplication::onDestroyZSPhysValObjectsTimeout()
//------------------------------------------------------------------------------
{
    CObjPoolTreeEntry* pTreeEntry;
    CObjTest*          pObjTest;

    pTreeEntry = m_objPool.getTreeEntry(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Voltage",
        /* strObjName   */ "Volt",
        /* entryType    */ EObjPoolEntryTypeObject );

    if( pTreeEntry != nullptr )
    {
        pObjTest = reinterpret_cast<CObjTest*>(pTreeEntry->getObj());

        try
        {
            delete pObjTest;
        }
        catch(...)
        {
        }
        pObjTest = nullptr;
    }

    pTreeEntry = m_objPool.getTreeEntry(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Voltage",
        /* strObjName   */ "MilliVolt",
        /* entryType    */ EObjPoolEntryTypeObject );

    if( pTreeEntry != nullptr )
    {
        pObjTest = reinterpret_cast<CObjTest*>(pTreeEntry->getObj());

        try
        {
            delete pObjTest;
        }
        catch(...)
        {
        }
        pObjTest = nullptr;
    }

    pTreeEntry = m_objPool.getTreeEntry(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Voltage",
        /* strObjName   */ "MicroVolt",
        /* entryType    */ EObjPoolEntryTypeObject );

    if( pTreeEntry != nullptr )
    {
        pObjTest = reinterpret_cast<CObjTest*>(pTreeEntry->getObj());

        try
        {
            delete pObjTest;
        }
        catch(...)
        {
        }
        pObjTest = nullptr;
    }

    pTreeEntry = m_objPool.getTreeEntry(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Current",
        /* strObjName   */ "Ampere",
        /* entryType    */ EObjPoolEntryTypeObject );

    if( pTreeEntry != nullptr )
    {
        pObjTest = reinterpret_cast<CObjTest*>(pTreeEntry->getObj());

        try
        {
            delete pObjTest;
        }
        catch(...)
        {
        }
        pObjTest = nullptr;
    }

    pTreeEntry = m_objPool.getTreeEntry(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Current",
        /* strObjName   */ "MilliAmpere",
        /* entryType    */ EObjPoolEntryTypeObject );

    if( pTreeEntry != nullptr )
    {
        pObjTest = reinterpret_cast<CObjTest*>(pTreeEntry->getObj());

        try
        {
            delete pObjTest;
        }
        catch(...)
        {
        }
        pObjTest = nullptr;
    }

    pTreeEntry = m_objPool.getTreeEntry(
        /* strNameSpace */ "ZS::PhysVal::Electricity::Current",
        /* strObjName   */ "MicroAmpere",
        /* entryType    */ EObjPoolEntryTypeObject );

    if( pTreeEntry != nullptr )
    {
        pObjTest = reinterpret_cast<CObjTest*>(pTreeEntry->getObj());

        try
        {
            delete pObjTest;
        }
        catch(...)
        {
        }
        pObjTest = nullptr;
    }

} // onDestroyZSPhysValObjectsTimeout
