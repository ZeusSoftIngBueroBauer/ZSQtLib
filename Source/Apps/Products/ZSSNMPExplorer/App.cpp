/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

--------------------------------------------------------------------------------

$Date: 10.01.10 19:16 $

$Revision: 6 $

$Author: Christian Bauer $

$History: ZSApp.cpp $
 * 
 * *****************  Version 6  *****************
 * User: Christian Bauer Date: 10.01.10   Time: 19:16
 * Updated in $/ZSAppSNMPExplorer/Source

*******************************************************************************/

#include <QtCore/qdir.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtimer.h>
#include <QtCore/qthread.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qlabel.h>
#include <QtGui/qmainwindow.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qstatusbar.h>
#include <QtGui/qtoolbutton.h>
#include "Include/ZSApp.h"
#include "Include/ZSMainWindow.h"
#include "ZSSNMP/Include/ZSSNMPMsg.h"
#include "ZSTrace/Include/ZSTrcAdminObj.h"
#include "ZSTrace/Include/ZSTrcMethod.h"
#include "ZSTrace/Include/ZSTrcServer.h"
#include "ZSSys/Include/ZSSysErrLogModel.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMsg.h"
#include "ZSSys/Include/ZSSysTime.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;
using namespace ZS::Trace;


/*******************************************************************************
class CApplication : public QApplication
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::SEnumStr s_arEnumStrStates[CApplication::EStateCount] =
//------------------------------------------------------------------------------
{
    { CApplication::EStateIdle,     "Idle",     "Idle"     },
    { CApplication::EStateStartup,  "Startup",  "Startup"  },
    { CApplication::EStateShutdown, "Shutdown", "Shutdown" },
    { CApplication::EStateRunning,  "Running",  "Running"  }
};

//------------------------------------------------------------------------------
QString CApplication::state2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::SEnumStr::val2Str(s_arEnumStrStates,EStateCount,i_iState);
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CApplication* CApplication::instance()
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
    const QString& i_strAppName ) :
//------------------------------------------------------------------------------
    QApplication(i_argc,i_argv),
    m_pTrcServer(NULL),
    m_bTrcServerCreatedByMe(false),
    m_pTrcAdminObj(NULL),
    m_pSettingsFile(NULL),
    m_bSettingsFileCreatedByMe(false),
    m_pModelErrLog(NULL),
    m_bModelErrLogCreatedByMe(false),
    m_pExplorer(NULL),
    m_pCltStdPC(NULL),
    m_pMainWindow(NULL),
    m_pStatusBar(NULL)
{
    setObjectName("theApp");

    QString strCurrentThread;

    if( QThread::currentThread() != NULL )
    {
        strCurrentThread = QThread::currentThread()->objectName();

        if( strCurrentThread.length() == 0 )
        {
            strCurrentThread = "GUIMain";
        }
        QThread::currentThread()->setObjectName(strCurrentThread);
    }

    QCoreApplication::setOrganizationName(i_strOrganizationName);
    QCoreApplication::setOrganizationDomain(i_strOrganizationDomain);
    QCoreApplication::setApplicationName(i_strAppName);

    QString strAppDirPath = QCoreApplication::applicationDirPath();
    QString strAppName = QCoreApplication::applicationName();

    QIcon iconApp;

    QPixmap pxmApp32x32(":/ZSApp/Images/ZSAppSNMPExplorerIcon32x32.bmp");
    QBitmap bmpApp32x32Mask(":/ZSApp/Images/ZSAppSNMPExplorerIcon32x32Mask.bmp");
    QPixmap pxmApp16x16(":/ZSApp/Images/ZSAppSNMPExplorerIcon16x16.bmp");
    QBitmap bmpApp16x16Mask(":/ZSApp/Images/ZSAppSNMPExplorerIcon16x16Mask.bmp");
    pxmApp32x32.setMask(bmpApp32x32Mask);
    pxmApp16x16.setMask(bmpApp16x16Mask);

    iconApp.addPixmap(pxmApp32x32);
    iconApp.addPixmap(pxmApp32x32);
    QApplication::setWindowIcon(iconApp);

    // Parse command arguments
    //------------------------

    int         idxArg;
    QString     strArg;
    QString     strVal;
    QStringList strListArgsPar;
    QStringList strListArgsVal;
    QString     strSettingsFilePath;

    parseAppArgs( i_argc, i_argv, strListArgsPar, strListArgsVal );

    #if QT_VERSION >= 0x040501
    for( idxArg = 0; idxArg < strListArgsPar.length() && idxArg < strListArgsVal.length(); idxArg++ )
    #else
    for( idxArg = 0; idxArg < strListArgsPar.size() && idxArg < strListArgsVal.size(); idxArg++ )
    #endif
    {
        strArg = strListArgsPar[idxArg];
        strVal = strListArgsVal[idxArg];

        if( strArg.compare("IniFile",Qt::CaseInsensitive) == 0 )
        {
            strSettingsFilePath = strVal;
        }
    }

    // Create settings file (if not yet created)
    //------------------------------------------

    if( m_pSettingsFile == NULL )
    {
        if( strSettingsFilePath.isEmpty() )
        {
            strSettingsFilePath = strAppDirPath + "/" + strAppName + ".ini";
        }

        m_pSettingsFile = new QSettings( strSettingsFilePath, QSettings::IniFormat );
        m_bSettingsFileCreatedByMe = true;
    }

    // Get reference to err log model
    //--------------------------------

    m_pModelErrLog = CModelErrLog::getInstance(m_pSettingsFile);

    if( m_pModelErrLog == NULL )
    {
        m_pModelErrLog = CModelErrLog::createInstance(m_pSettingsFile);
        m_bModelErrLogCreatedByMe = true;
    }

    // Create trace server (if not yet created)
    //-----------------------------------------

    m_pTrcServer = CTrcServer::getTrcServer(ETraceTypeMth);

    if( m_pTrcServer == NULL )
    {
        m_pTrcServer = new CTrcServer(
            /* traceType                */ ZS::Trace::ETraceTypeMth,
            /* szObjName                */ "TrcMthServer",
            /* pSettingsFile            */ m_pSettingsFile,
            /* strSettingsKey           */ "TrcMthServer",
            /* iReadSettingsInterval_ms */ 0 );
        m_pTrcServer->recallAdminObjs();
        m_bTrcServerCreatedByMe = true;
    }

    // Create trace admin objects
    //---------------------------

    m_pTrcAdminObj = CTrcServer::getTrcAdminObj(
        /* traceType         */ ZS::Trace::ETraceTypeMth,
        /* strClassNameSpace */ "ZS::SNMP",
        /* strClassName      */ "CApplication",
        /* strObjNameSpace   */ "",
        /* strObjName        */ "theApp" );

    M_TRACE_INST_METHOD(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 0,
        /* szMethod     */ "ctor",
        /* szAddInfo    */ "" );

    // SNMP Explorer
    //--------------

    m_pExplorer = CExplorer::createInstance(
        /* szObjName             */ "SNMPExplorer",
        /* szObjNameSession      */ "SNMPSession",
        /* pSettingsFile         */ m_pSettingsFile,
        /* strSettingsKey        */ "SNMPSession",
        /* iReadSettingsInterval */ 0 );

    // SNMP Client Standard PC
    //------------------------

    m_pCltStdPC = new CCltStdPC(
        /* szObjName              */ "SNMPCltStdPC",
        /* szObjNameExplorer      */ "",                // empty string as pExplorer != NULL
        /* szObjNameSession       */ "",                // empty string as pExplorer != NULL
        /* pSettingsFile          */ m_pSettingsFile,
        /* strSettingsKey         */ "SNMPCltStdPC",
        /* strSettingsKeyExplorer */ "",                // empty string as pExplorer != NULL
        /* iReadSettingsInterval  */ 0 );

    // Main Window
    //------------

    setQuitOnLastWindowClosed(false);
    if( !connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL(lastWindowClosed()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onLastWindowClosed()) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    m_pMainWindow = new CMainWindow(
        /* strWindowTitle */ i_strAppName,
        /* pExplorer      */ m_pExplorer,
        /* pCltStdPC      */ m_pCltStdPC,
        /* pSettingsFile  */ m_pSettingsFile,
        /* strSettingsKey */ "MainWindow" );
    m_pMainWindow->show();

    // Status Bar
    //-----------

    m_pStatusBar = new QStatusBar;
    m_pMainWindow->setStatusBar(m_pStatusBar);

    // System startup
    //---------------

    startup();

} // ctor

//------------------------------------------------------------------------------
CApplication::~CApplication()
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 0,
        /* szMethod     */ "dtor",
        /* szAddInfo    */ "" );

    m_pStatusBar = NULL;

    try
    {
        delete m_pCltStdPC;
    }
    catch(...)
    {
    }
    m_pCltStdPC = NULL;

    try
    {
        delete m_pExplorer;
    }
    catch(...)
    {
    }
    m_pExplorer = NULL;

    try
    {
        delete m_pMainWindow;
    }
    catch(...)
    {
    }
    m_pMainWindow = NULL;

    if( m_bModelErrLogCreatedByMe )
    {
        try
        {
            delete m_pModelErrLog;
        }
        catch(...)
        {
        }
    }
    m_pModelErrLog = NULL;

    if( m_bSettingsFileCreatedByMe )
    {
        try
        {
            delete m_pSettingsFile;
        }
        catch(...)
        {
        }
    }
    m_pSettingsFile = NULL;

    CTrcServer::releaseTrcAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = NULL;

    m_pTrcServer->saveAdminObjs();

    if( m_bTrcServerCreatedByMe )
    {
        try
        {
            delete m_pTrcServer;
        }
        catch(...)
        {
        }
    }
    m_pTrcServer = NULL;

} // dtor

/*==============================================================================
public: // instance methods to trigger system startup and shutdown
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::startup()
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 0,
        /* szMethod     */ "startup",
        /* szAddInfo    */ "" );

    //m_pCltStdPC->start();
    //m_pCltStdPC->updateAllValues();

} // startup

//------------------------------------------------------------------------------
void CApplication::shutdown()
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 0,
        /* szMethod     */ "shutdown",
        /* szAddInfo    */ "" );

    m_pCltStdPC->stop();
    m_pExplorer->shutdown();

    // Quit the application (terminate main event loop)
    quit();

} // shutdown

/*==============================================================================
public: // overridables of base class QCoreApplication
==============================================================================*/

//------------------------------------------------------------------------------
bool CApplication::notify( QObject* i_pObjReceiver, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool         bResult = false;
    bool         bDeleteMsg = false;
    ZS::CMsgErr* pMsgErr = NULL;
    ZS::CMsgErr* pMsgExc = NULL;

    if( i_pEv->type() != QEvent::Timer )
    {
        pMsgErr = dynamic_cast<CMsgErr*>(i_pEv);
    }

    if( pMsgErr != NULL )
    {
        if( pMsgErr->wasException() )
        {
            // The error (exception) message will be deleted by Qt.
            pMsgExc = pMsgErr;
        }
        else if( m_pModelErrLog != NULL )
        {
            m_pModelErrLog->addEntry(
                /* dateTime    */ QDateTime::currentDateTime().toUTC(),
                /* severity    */ pMsgErr->getSeverity(),
                /* iErrNr      */ pMsgErr->getResult(),
                /* strErrNr    */ pMsgErr->getResultStr(),
                /* strSource   */ pMsgErr->getErrSource(),
                /* strAddInfo  */ pMsgErr->getAddErrInfoDscr(),
                /* strProposal */ "---" );
        }
        pMsgErr = NULL;
    }

    if( pMsgErr == NULL )
    {
        QString strExcType;

        try
        {
            #ifdef NO_GUI
            bResult = QCoreApplication::notify(i_pObjReceiver,i_pEv);
            #else
            bResult = QApplication::notify(i_pObjReceiver,i_pEv);
            #endif
        }
        catch( ZS::CException* pExc )
        {
            // This type of exception has already been logged on throwing (creating) the exception.
            pMsgExc = new ZS::CMsgErr(
                /* pObjSender    */ this,
                /* iSenderId     */ -1,
                /* pObjReceiver  */ QCoreApplication::instance(),
                /* iReceiverId   */ -1,
                /* exc           */ *pExc,
                /* strExcType    */ "ZS::CException" );
        }
        catch( ZS::CException& exc )
        {
            // This type of exception has already been logged on throwing (creating) the exception.
            pMsgExc = new ZS::CMsgErr(
                /* pObjSender    */ this,
                /* iSenderId     */ -1,
                /* pObjReceiver  */ QCoreApplication::instance(),
                /* iReceiverId   */ -1,
                /* exc           */ exc,
                /* strExcType    */ "ZS::CException" );
        }
        catch(...)
        {
            // An ZS::CException instance will be temporarily created.
            // This exception instance is used to create an exception error message.
            // The ctor of this exception adds an entry of this event to the error log model.
            ZS::CException exc(
                /* strFile    */ "",
                /* iLineNr    */ -1,
                /* result     */ ZS::EResultUnknownException,
                /* strAddInfo */ "" );
            pMsgExc = new ZS::CMsgErr(
                /* pObjSender    */ this,
                /* iSenderId     */ -1,
                /* pObjReceiver  */ QCoreApplication::instance(),
                /* iReceiverId   */ -1,
                /* exc           */ exc,
                /* strExcType    */ "Unknown exception" );
        }

        if( pMsgExc != NULL )
        {
            // Dialogs may only be opened from within the GUI main thread.
            if( QThread::currentThread() != thread() )
            {
                #ifdef NO_GUI
                QCoreApplication::postEvent(this,pMsgExc);
                #else
                QApplication::postEvent(this,pMsgExc);
                #endif
                pMsgExc = NULL;
            }
            else
            {
                // The error dialog may be opened immediately, the exception
                // message don't need to be posted and therefore hast to be
                // destroyed within the notify method.
                bDeleteMsg = true;
            }
        }
    }

    if( pMsgExc != NULL )
    {
        // Exceptions have already been logged either on throwing (creating) a "known" exception
        // of type ZS::CException or if an unknown exception has been caught above.

        #ifdef NO_GUI

        if( bDeleteMsg )
        {
            delete pMsgExc;
            pMsgExc = NULL;
        }

        // Quit the application (terminate main event loop)
        quit();

        #else

        QDateTime         dateTime = QDateTime::currentDateTime().toUTC();
        SErrResultInfo    errResultInfo = pMsgExc->getErrResultInfo();
        QMessageBox::Icon icon = QMessageBox::NoIcon;

        QString strBtnAbort;
        QString strBtnIgnore;

        switch( pMsgExc->getSeverity() )
        {
            case EResultSeveritySuccess:
            {
                icon = QMessageBox::Information;
                break;
            }
            case EResultSeverityInfo:
            {
                icon = QMessageBox::Information;
                break;
            }
            case EResultSeverityWarning:
            {
                icon = QMessageBox::Warning;
                strBtnAbort  = "[Abort] \t to terminate the application";
                strBtnIgnore = "[Ignore]\t to continue execution but with unpredictable behavior of the program.";
                break;
            }
            case EResultSeverityError:
            {
                icon = QMessageBox::Warning;
                strBtnAbort  = "[Abort] \t to terminate the application";
                strBtnIgnore = "[Ignore]\t to continue execution but with unpredictable behavior of the program.";
                break;
            }
            case EResultSeverityCritical:
            {
                icon = QMessageBox::Critical;
                strBtnAbort  = "[Abort] \t to terminate the application";
                strBtnIgnore = "[Ignore]\t to continue execution but with unpredictable behavior of the program.";
                break;
            }
            default:
            {
                break;
            }
        }

        QString strErrMsg;

        if( !errResultInfo.getErrMsgHeadLine().isEmpty() )
        {
            strErrMsg = errResultInfo.getErrMsgHeadLine() + "\n\n";
        }

        strErrMsg += "Severity: \t" + errResultInfo.getSeverityStr() + "\n";
        strErrMsg += "Error:    \t" + errResultInfo.getResultStr() + "\n";

        if( !errResultInfo.getAddErrInfoDscr().isEmpty() )
        {
            strErrMsg += "AddInfo: \t" + errResultInfo.getAddErrInfoDscr() + "\n";
        }
        strErrMsg += "\n";

        strErrMsg += "Exception was thrown from:\n\n";
        strErrMsg += "File:   \t" + pMsgExc->getExcFileName() + "\n";
        strErrMsg += "Line:   \t" + QString::number(pMsgExc->getExcLineNr()) + "\n";
        strErrMsg += "Thread: \t" + pMsgExc->getExcThreadName() + "\n\n";

        QString strDateTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        QString strSysTime = QString::number(ZS::System::Time::getProcTimeInMicroSec(),'f',6);

        strErrMsg.insert( 0, strDateTime + "\n" + strSysTime + "\n\n" );

        if( !strBtnAbort.isEmpty() )
        {
            strErrMsg += strBtnAbort + "\n";
        }
        if( !strBtnIgnore.isEmpty() )
        {
            strErrMsg += strBtnIgnore + "\n";
        }

        QMessageBox msgBox(
            /* icon     */ icon,
            /* strTitle */ QCoreApplication::applicationName(),
            /* strText  */ strErrMsg );
        msgBox.setModal(true);

        switch( pMsgExc->getSeverity() )
        {
            case EResultSeveritySuccess:
            {
                msgBox.addButton("Ok",QMessageBox::AcceptRole);
                #if QT_VERSION >= 0x040501
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setEscapeButton(QMessageBox::Ok);
                #endif
                break;
            }
            case EResultSeverityInfo:
            {
                msgBox.addButton("Ok",QMessageBox::AcceptRole);
                #if QT_VERSION >= 0x040501
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setEscapeButton(QMessageBox::Ok);
                #endif
                break;
            }
            case EResultSeverityWarning:
            {
                msgBox.addButton("Ignore",QMessageBox::RejectRole);
                msgBox.addButton("Abort",QMessageBox::AcceptRole);
                #if QT_VERSION >= 0x040501
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setEscapeButton(QMessageBox::Abort);
                #endif
                break;
            }
            case EResultSeverityError:
            {
                msgBox.addButton("Ignore",QMessageBox::RejectRole);
                msgBox.addButton("Abort",QMessageBox::AcceptRole);
                #if QT_VERSION >= 0x040501
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setEscapeButton(QMessageBox::Abort);
                #endif
                break;
            }
            case EResultSeverityCritical:
            {
                msgBox.addButton("Ignore",QMessageBox::RejectRole);
                msgBox.addButton("Abort",QMessageBox::AcceptRole);
                #if QT_VERSION >= 0x040501
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setEscapeButton(QMessageBox::Abort);
                #endif
                break;
            }
            default:
            {
                msgBox.addButton("Ok",QMessageBox::AcceptRole);
                break;
            }
        }

        int iDlgCode = msgBox.exec();

        if( bDeleteMsg )
        {
            delete pMsgExc;
            pMsgExc = NULL;
        }
        if( iDlgCode == QDialog::Accepted )
        {
            // Quit the application (terminate main event loop)
            quit();
        }
        #endif // #ifdef NO_GUI
    }
    return bResult;

} // notify

/*==============================================================================
protected slots: // instance methods of system startup
==============================================================================*/

//------------------------------------------------------------------------------
void CApplication::onLastWindowClosed()
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 0,
        /* szMethod     */ "onLastWindowClosed",
        /* szAddInfo    */ "" );

    shutdown();

} // onLastWindowClosed
