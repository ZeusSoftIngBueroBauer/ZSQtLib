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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSRemCmdGUI/ZSRemCmdsDlg.h"
#include "ZSRemCmdGUI/ZSRemCmdObjPoolWdgt.h"
#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdObjPool.h"
#include "ZSRemCmd/ZSRemCmdClient.h"
#include "ZSRemCmd/ZSRemCmdServer.h"
#include "ZSSysGUI/ZSSysProgressBar.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::RemCmd;
using namespace ZS::RemCmd::GUI;


/*******************************************************************************
class CDlgRemCmds : public QDialog
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QHash<QString,CDlgRemCmds*> CDlgRemCmds::s_arhshInstances[ZS::Ipc::ESrvCltTypeCount]; // Key is title of dialog

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CDlgRemCmds* CDlgRemCmds::GetInstance(
    ESrvCltType    i_srvCltType,
    const QString& i_strDlgTitle )
//------------------------------------------------------------------------------
{
    return s_arhshInstances[i_srvCltType].value(i_strDlgTitle, nullptr);
}

//------------------------------------------------------------------------------
CDlgRemCmds* CDlgRemCmds::CreateInstance(
    ESrvCltType    i_srvCltType,
    const QString& i_strDlgTitle,
    QWidget*       i_pWdgtParent )
//------------------------------------------------------------------------------
{
    if( s_arhshInstances[i_srvCltType].contains(i_strDlgTitle) )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, "CDlgRemCmds::" + srvCltType2Str(i_srvCltType) + "::" + i_strDlgTitle);
    }

    CDlgRemCmds* pDlg = new CDlgRemCmds(i_srvCltType, i_strDlgTitle, i_pWdgtParent);

    s_arhshInstances[i_srvCltType][i_strDlgTitle] = pDlg;

    return pDlg;

} // CreateInstance

//------------------------------------------------------------------------------
void CDlgRemCmds::DestroyInstance(
    ESrvCltType    i_srvCltType,
    const QString& i_strDlgTitle )
//------------------------------------------------------------------------------
{
    CDlgRemCmds* pDlg = s_arhshInstances[i_srvCltType].value(i_strDlgTitle, nullptr);

    if( pDlg == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, "CDlgRemCmds::" + srvCltType2Str(i_srvCltType) + "::" + i_strDlgTitle);
    }

    s_arhshInstances[i_srvCltType].remove(i_strDlgTitle);

    delete pDlg;
    pDlg = nullptr;

} // DestroyInstance

//------------------------------------------------------------------------------
void CDlgRemCmds::DestroyInstance( CDlgRemCmds* i_pDlg )
//------------------------------------------------------------------------------
{
    CDlgRemCmds* pDlg;
    QString      strDlgTitle;
    int          iSrvCltType;

    QHash<QString,CDlgRemCmds*>::iterator itDlgs;

    for( iSrvCltType = 0; iSrvCltType < ESrvCltTypeCount; iSrvCltType++ )
    {
        for( itDlgs = s_arhshInstances[iSrvCltType].begin(); itDlgs != s_arhshInstances[iSrvCltType].end(); itDlgs++ )
        {
            strDlgTitle = itDlgs.key();
            pDlg = itDlgs.value();

            if( pDlg == i_pDlg )
            {
                s_arhshInstances[iSrvCltType].remove(strDlgTitle);
                break;
            }
        }
    } // for( iSrvCltType = 0; iSrvCltType < ESrvCltTypeCount; iSrvCltType++ )

    delete i_pDlg;
    i_pDlg = nullptr;

} // DestroyInstance

//------------------------------------------------------------------------------
void CDlgRemCmds::DestroyAllInstances( ESrvCltType i_srvCltType )
//------------------------------------------------------------------------------
{
    CDlgRemCmds* pDlg;
    QString      strDlgTitle;

    int iSrvCltTypeMin = i_srvCltType;
    int iSrvCltTypeMax = i_srvCltType;
    int iSrvCltType;

    if( i_srvCltType == ESrvCltTypeCount )
    {
        iSrvCltTypeMin = 0;
        iSrvCltTypeMax = ESrvCltTypeCount-1;
    }

    QHash<QString,CDlgRemCmds*>::iterator itDlgs;

    for( iSrvCltType = iSrvCltTypeMin; iSrvCltType <= iSrvCltTypeMax; iSrvCltType++ )
    {
        for( itDlgs = s_arhshInstances[iSrvCltType].begin(); itDlgs != s_arhshInstances[iSrvCltType].end(); itDlgs++ )
        {
            strDlgTitle = itDlgs.key();
            pDlg = itDlgs.value();

            s_arhshInstances[iSrvCltType][strDlgTitle] = nullptr;

            delete pDlg;
            pDlg = nullptr;
        }

        s_arhshInstances[iSrvCltType].clear();

    } // for( iSrvCltType = iSrvCltTypeMin; iSrvCltType <= iSrvCltTypeMax; iSrvCltType++ )

} // DestroyAllInstances

//------------------------------------------------------------------------------
void CDlgRemCmds::HideAllInstances( ZS::Ipc::ESrvCltType i_srvCltType )
//------------------------------------------------------------------------------
{
    CDlgRemCmds* pDlg;
    QString      strDlgTitle;

    int iSrvCltTypeMin = i_srvCltType;
    int iSrvCltTypeMax = i_srvCltType;
    int iSrvCltType;

    if( i_srvCltType == ESrvCltTypeCount )
    {
        iSrvCltTypeMin = 0;
        iSrvCltTypeMax = ESrvCltTypeCount-1;
    }

    QHash<QString,CDlgRemCmds*>::iterator itDlgs;

    for( iSrvCltType = iSrvCltTypeMin; iSrvCltType <= iSrvCltTypeMax; iSrvCltType++ )
    {
        for( itDlgs = s_arhshInstances[iSrvCltType].begin(); itDlgs != s_arhshInstances[iSrvCltType].end(); itDlgs++ )
        {
            strDlgTitle = itDlgs.key();
            pDlg = itDlgs.value();
            pDlg->hide();
        }
    } // for( iSrvCltType = iSrvCltTypeMin; iSrvCltType <= iSrvCltTypeMax; iSrvCltType++ )

} // HideAllInstances

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgRemCmds::CDlgRemCmds(
    ZS::Ipc::ESrvCltType i_srvCltType,
    const QString&       i_strDlgTitle,
    QWidget*             i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QDialog(i_pWdgtParent),
    m_srvCltType(i_srvCltType),
    m_pRemCmdClient(nullptr),
    m_pRemCmdServer(nullptr),
    m_pRemCmdObjPool(nullptr),
    m_pLyt(nullptr),
    m_pLytLineCnct(nullptr),
    m_pBtnConnect(nullptr),
    m_pProgressBarCnct(nullptr),
    m_pSplitter(nullptr),
    m_pGrpRemCmdObjPool(nullptr),
    m_pLytRemCmdObjPool(nullptr),
    m_pSplitterRemCmd(nullptr),
    m_pWdgtRemCmdObjPool(nullptr),
    m_pWdgtRemCmd(nullptr),
    m_pLytRemCmd(nullptr),
    m_pLytLineRemCmd(nullptr),
    m_pLblRemCmd(nullptr),
    m_pEdtRemCmd(nullptr),
    m_pBtnRemCmdExecute(nullptr),
    m_pLytLineRemCmdSyntax(nullptr),
    m_pLblRemCmdSyntax(nullptr),
    m_pEdtRemCmdSyntax(nullptr),
    m_pEdtRemCmdDscr(nullptr),
    m_pGrpLog(nullptr),
    m_pLytLog(nullptr),
    m_pEdtLog(nullptr),
    m_pLytLineLogBtns(nullptr),
    m_pBtnLogClear(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( "DlgRemCmds" );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmdGUI", "CDlgRemCmds", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( i_strDlgTitle.isEmpty() )
    {
        setWindowTitle( QCoreApplication::applicationName() + ": Remote Commands" );
    }
    else
    {
        setWindowTitle(i_strDlgTitle);
    }

    // Layout
    //==================

    int cxWidthClm1 = 80;
    int cxBtnWidth = 120;

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // <Line> Remote Host
    //-------------------

    if( m_srvCltType == ESrvCltTypeClient )
    {
        m_pLytLineCnct = new QHBoxLayout();
        m_pLyt->addLayout(m_pLytLineCnct);

        m_pBtnConnect = new QPushButton("Disconnect");
        m_pBtnConnect->setFixedWidth(cxBtnWidth);
        m_pLytLineCnct->addWidget(m_pBtnConnect);

        if( !connect(
            /* pObjSender   */ m_pBtnConnect,
            /* szSignal     */ SIGNAL(clicked(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onBtnConnectClicked(bool)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        m_pProgressBarCnct = new CProgressBar();
        m_pProgressBarCnct->setTextVisible(false);
        m_pLytLineCnct->addWidget(m_pProgressBarCnct, 1);

    } // if( m_srvCltType == ESrvCltTypeClient )

    // <Splitter> Main
    //----------------

    // Dividing remote command group and log widget

    m_pSplitter = new QSplitter(Qt::Vertical);
    m_pSplitter->setChildrenCollapsible(false);
    m_pLyt->addWidget(m_pSplitter);

    // <GroupBox> Remote Commands
    //---------------------------

    m_pGrpRemCmdObjPool = new QGroupBox("Remote Commands");
    m_pLytRemCmdObjPool = new QVBoxLayout();
    m_pLytRemCmdObjPool->setContentsMargins(0,0,0,0);
    m_pGrpRemCmdObjPool->setLayout(m_pLytRemCmdObjPool);
    m_pSplitter->addWidget(m_pGrpRemCmdObjPool);

    // Dividing remote command tree view and selected command with command description
    m_pSplitterRemCmd = new QSplitter(Qt::Vertical);
    m_pSplitterRemCmd->setChildrenCollapsible(false);
    m_pLytRemCmdObjPool->addWidget(m_pSplitterRemCmd);

    // <Widget> RemCmds
    //-----------------

    m_pWdgtRemCmdObjPool = new CWdgtRemCmdObjPool();
    m_pSplitterRemCmd->addWidget(m_pWdgtRemCmdObjPool);

    m_pWdgtRemCmdObjPool->onBtnObjPoolExpandAllClicked(true);
    m_pWdgtRemCmdObjPool->onBtnObjPoolResizeRowsAndColumnsToContentsClicked(true);

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtRemCmdObjPool,
        /* szSignal     */ SIGNAL( remCmdObjSelected(const QString&, const QString&, const QString&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onRemCmdObjSelected(const QString&, const QString&, const QString&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    QTreeView* pTreeView = m_pWdgtRemCmdObjPool->getTreeView();

    if( pTreeView != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ pTreeView,
            /* szSignal     */ SIGNAL( expanded(const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewRemCmdObjPoolExpanded(const QModelIndex&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }

    // <Widget> Remote Command (Selected Command, Syntax, Description)
    //----------------------------------------------------------------

    m_pWdgtRemCmd = new QWidget();
    m_pLytRemCmd = new QVBoxLayout();
    m_pWdgtRemCmd->setLayout(m_pLytRemCmd);
    m_pSplitterRemCmd->addWidget(m_pWdgtRemCmd);

    // <Line> Selected Command and Execute Button
    //-------------------------------------------

    m_pLytLineRemCmd = new QHBoxLayout();
    m_pLytRemCmd->addLayout(m_pLytLineRemCmd);

    m_pLblRemCmd = new QLabel("Command:");
    m_pLblRemCmd->setFixedWidth(cxWidthClm1);
    m_pLytLineRemCmd->addWidget(m_pLblRemCmd);

    m_pEdtRemCmd = new QLineEdit();
    m_pLytLineRemCmd->addWidget(m_pEdtRemCmd, 1);

    m_pBtnRemCmdExecute = new QPushButton("Execute");
    m_pBtnRemCmdExecute->setEnabled(false);
    m_pLytLineRemCmd->addWidget(m_pBtnRemCmdExecute);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnRemCmdExecute,
        /* szSignal     */ SIGNAL( clicked() ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onBtnRemCmdExecuteClicked() ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Line> Selected Command Syntax
    //-------------------------------------------

    m_pLytLineRemCmdSyntax = new QHBoxLayout();
    m_pLytRemCmd->addLayout(m_pLytLineRemCmdSyntax);

    m_pLblRemCmdSyntax = new QLabel("Syntax:");
    m_pLblRemCmdSyntax->setFixedWidth(cxWidthClm1);
    m_pLytLineRemCmdSyntax->addWidget(m_pLblRemCmdSyntax);

    m_pEdtRemCmdSyntax = new QLineEdit();
    m_pLytLineRemCmdSyntax->addWidget(m_pEdtRemCmdSyntax, 1);

    // <TextEdit> Description of selected Command
    //-------------------------------------------

    m_pEdtRemCmdDscr = new QTextEdit();
    m_pLytRemCmd->addWidget(m_pEdtRemCmdDscr, 1);

    // <GroupBox> Command Log
    //------------------------

    QFont fntEdtLog(
        /* strFamily  */ "Courier New",
        /* iPointSize */ 8,
        /* iWeight    */ -1,
        /* bItalic    */ false );

    m_pGrpLog = new QGroupBox("Log");
    m_pSplitter->addWidget(m_pGrpLog);

    m_pLytLog = new QVBoxLayout();
    m_pGrpLog->setLayout(m_pLytLog);

    m_pEdtLog = new QTextEdit();
    m_pEdtLog->setFont(fntEdtLog);
    m_pLytLog->addWidget(m_pEdtLog,1);

    m_pLytLineLogBtns = new QHBoxLayout();
    m_pLytLog->addLayout(m_pLytLineLogBtns);

    m_pBtnLogClear = new QPushButton("Clear");
    m_pBtnLogClear->setFixedWidth(cxBtnWidth);
    m_pLytLineLogBtns->addWidget(m_pBtnLogClear);

    if( !connect(
        /* pObjSender   */ m_pBtnLogClear,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnLogClearClicked(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    m_pLytLineLogBtns->addStretch();

    // Restore position and size
    //==========================

    QSettings settings;

    restoreGeometry( settings.value(windowTitle()+"/Geometry").toByteArray() );

    if( m_pSplitter != nullptr )
    {
        QList<int> listSizes = m_pSplitter->sizes();
        int        idx;

        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            listSizes[idx] = settings.value( objectName() + "/SplitterHeight" + QString::number(idx), 50 ).toInt();
        }
        m_pSplitter->setSizes(listSizes);
    }

    if( m_pSplitterRemCmd != nullptr )
    {
        QList<int> listSizes = m_pSplitterRemCmd->sizes();
        int        idx;

        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            listSizes[idx] = settings.value( objectName() + "/SplitterRemCmdsHeight" + QString::number(idx), 50 ).toInt();
        }
        m_pSplitterRemCmd->setSizes(listSizes);
    }

} // ctor

//------------------------------------------------------------------------------
CDlgRemCmds::~CDlgRemCmds()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    QSettings settings;

    settings.setValue( windowTitle()+"/Geometry", saveGeometry() );

    if( m_pSplitter != nullptr )
    {
        QList<int> listSizes = m_pSplitter->sizes();
        int        idx;

        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            settings.setValue( objectName() + "/SplitterHeight" + QString::number(idx), listSizes[idx] );
        }
    }

    if( m_pSplitterRemCmd != nullptr )
    {
        QList<int> listSizes = m_pSplitterRemCmd->sizes();
        int        idx;

        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            settings.setValue( objectName() + "/SplitterRemCmdsHeight" + QString::number(idx), listSizes[idx] );
        }
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_srvCltType = static_cast<ESrvCltType>(0);
    m_pRemCmdClient = nullptr;
    m_pRemCmdServer = nullptr;
    m_pRemCmdObjPool = nullptr;
    m_pLyt = nullptr;
    m_pLytLineCnct = nullptr;
    m_pBtnConnect = nullptr;
    m_pProgressBarCnct = nullptr;
    m_pSplitter = nullptr;
    m_pGrpRemCmdObjPool = nullptr;
    m_pLytRemCmdObjPool = nullptr;
    m_pSplitterRemCmd = nullptr;
    m_pWdgtRemCmdObjPool = nullptr;
    m_pWdgtRemCmd = nullptr;
    m_pLytRemCmd = nullptr;
    m_pLytLineRemCmd = nullptr;
    m_pLblRemCmd = nullptr;
    m_pEdtRemCmd = nullptr;
    m_pBtnRemCmdExecute = nullptr;
    m_pLytLineRemCmdSyntax = nullptr;
    m_pLblRemCmdSyntax = nullptr;
    m_pEdtRemCmdSyntax = nullptr;
    m_pEdtRemCmdDscr = nullptr;
    m_pGrpLog = nullptr;
    m_pLytLog = nullptr;
    m_pEdtLog = nullptr;
    m_pLytLineLogBtns = nullptr;
    m_pBtnLogClear = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgRemCmds::setRemCmdObjPool( CRemCmdObjPool* i_pRemCmdObjPool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "ObjPool: " + QString( i_pRemCmdObjPool == nullptr ? "nullptr" : i_pRemCmdObjPool->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setRemCmdObjPool",
        /* strAddInfo   */ strAddTrcInfo );

    // <Widget> RemCmds
    //-----------------

    m_pRemCmdObjPool = i_pRemCmdObjPool;

    m_pWdgtRemCmdObjPool->setObjPool(m_pRemCmdObjPool);

} // setRemCmdObjPool

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgRemCmds::setRemCmdClient( CRemCmdClient* i_pRemCmdClient )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Client: " + QString( i_pRemCmdClient == nullptr ? "nullptr" : i_pRemCmdClient->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setRemCmdClient",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_srvCltType != ESrvCltTypeClient )
    {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall );
    }

    // <Widget> RemCmds
    //-----------------

    if( i_pRemCmdClient == nullptr )
    {
        m_pRemCmdObjPool = nullptr;
    }
    else
    {
        m_pRemCmdObjPool = i_pRemCmdClient->getObjPool();
    }

    m_pWdgtRemCmdObjPool->setObjPool(m_pRemCmdObjPool);

    // Keeping indicated parameters up to date
    //========================================

    if( m_pRemCmdClient != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( destroyed(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltDestroyed(QObject*) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( connected(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdClientConnected(QObject*) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( disconnected(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdClientDisconnected(QObject*) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( settingsChanged(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltSettingsChanged(QObject*) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( requestInProgressChanged(QObject*, ZS::System::SRequestDscr) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltRequestInProgressChanged(QObject*, ZS::System::SRequestDscr) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( receivedData(QObject*, const QByteArray&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdClientReceivedData(QObject*, const QByteArray&) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( commandSent(QObject*, const QString&, const QByteArray&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdClientCommandSent(QObject*, const QString&, const QByteArray&) ) );

    } // if( m_pRemCmdClient != nullptr )

    m_pRemCmdClient = i_pRemCmdClient;

    if( m_pRemCmdClient != nullptr )
    {
        onRemCmdSrvCltSettingsChanged(m_pRemCmdClient);

        if( m_pRemCmdClient->isConnected() )
        {
            onRemCmdClientConnected(m_pRemCmdClient);
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( destroyed(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltDestroyed(QObject*) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( connected(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdClientConnected(QObject*) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( disconnected(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdClientDisconnected(QObject*) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( settingsChanged(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltSettingsChanged(QObject*) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( requestInProgressChanged(QObject*, ZS::System::SRequestDscr) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltRequestInProgressChanged(QObject*, ZS::System::SRequestDscr) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( receivedData(QObject*, const QByteArray&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdClientReceivedData(QObject*, const QByteArray&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdClient,
            /* szSignal     */ SIGNAL( commandSent(QObject*, const QString&, const QByteArray&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdClientCommandSent(QObject*, const QString&, const QByteArray&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pRemCmdClient != nullptr )

} // setRemCmdClient

//------------------------------------------------------------------------------
void CDlgRemCmds::setRemCmdServer( CRemCmdServer* i_pRemCmdServer )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Server: " + QString( i_pRemCmdServer == nullptr ? "nullptr" : i_pRemCmdServer->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setRemCmdServer",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_srvCltType != ESrvCltTypeServer )
    {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall );
    }

    if( i_pRemCmdServer == nullptr )
    {
        m_pRemCmdObjPool = nullptr;
    }
    else
    {
        m_pRemCmdObjPool = i_pRemCmdServer->getObjPool();
    }

    m_pWdgtRemCmdObjPool->setObjPool(m_pRemCmdObjPool);

    // Keeping indicated parameters up to date
    //========================================

    if( m_pRemCmdServer != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( destroyed(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltDestroyed(QObject*) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( connected(QObject*, const ZS::Ipc::SSocketDscr&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdServerConnected(QObject*, const ZS::Ipc::SSocketDscr&) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( disconnected(QObject*, const ZS::Ipc::SSocketDscr&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdServerDisconnected(QObject*, const ZS::Ipc::SSocketDscr&) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( settingsChanged(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltSettingsChanged(QObject*) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( requestInProgressChanged(QObject*, ZS::System::SRequestDscr) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltRequestInProgressChanged(QObject*, ZS::System::SRequestDscr) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( receivedData(QObject*, int, const QByteArray&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdServerReceivedData(QObject*, int, const QByteArray&) ) );
        QObject::disconnect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( dataSent(QObject*, int, const QByteArray&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdServerDataSent(QObject*, int, const QByteArray&) ) );

    } // if( m_pRemCmdServer != nullptr )

    m_pRemCmdServer = i_pRemCmdServer;

    if( m_pRemCmdServer == nullptr )
    {
        m_pBtnRemCmdExecute->setEnabled(false);
    }
    else // if( m_pRemCmdServer != nullptr )
    {
        m_pBtnRemCmdExecute->setEnabled(true);

        onRemCmdSrvCltSettingsChanged(m_pRemCmdServer);

        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( destroyed(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltDestroyed(QObject*) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( connected(QObject*, const ZS::Ipc::SSocketDscr&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdServerConnected(QObject*, const ZS::Ipc::SSocketDscr&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( disconnected(QObject*, const ZS::Ipc::SSocketDscr&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdServerDisconnected(QObject*, const ZS::Ipc::SSocketDscr&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( settingsChanged(QObject*) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltSettingsChanged(QObject*) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( requestInProgressChanged(QObject*, ZS::System::SRequestDscr) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdSrvCltRequestInProgressChanged(QObject*, ZS::System::SRequestDscr) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( receivedData(QObject*, int, const QByteArray&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdServerReceivedData(QObject*, int, const QByteArray&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pRemCmdServer,
            /* szSignal     */ SIGNAL( dataSent(QObject*, int, const QByteArray&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onRemCmdServerDataSent(QObject*, int, const QByteArray&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pRemCmdServer != nullptr )

} // setRemCmdServer

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgRemCmds::show()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "show",
        /* strAddInfo   */ "" );

    // Restore position and size of dialog
    //------------------------------------

    QSettings settings;

    restoreGeometry( settings.value(windowTitle()+"/Geometry").toByteArray() );

    // Call base method to show the dialog
    //------------------------------------

    QDialog::show();

} // show

//------------------------------------------------------------------------------
void CDlgRemCmds::hide()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "hide",
        /* strAddInfo   */ "" );

    // Save position and size of dialog
    //------------------------------------

    QSettings settings;

    settings.setValue( windowTitle()+"/Geometry", saveGeometry() );

    if( m_pSplitterRemCmd != nullptr )
    {
        QList<int> listSizes = m_pSplitterRemCmd->sizes();

        for( int idx = 0; idx < listSizes.count(); idx++ )
        {
            settings.setValue( objectName() + "/RemCmdSplitterHeight" + QString::number(idx), listSizes[idx] );
        }
    }

    if( m_pSplitter != nullptr )
    {
        QList<int> listSizes = m_pSplitter->sizes();

        for( int idx = 0; idx < listSizes.count(); idx++ )
        {
            settings.setValue( objectName() + "/SplitterHeight" + QString::number(idx), listSizes[idx] );
        }
    }

    // Call base method to hide the dialog
    //------------------------------------

    QDialog::hide();

} // hide

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgRemCmds::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "closeEvent",
        /* strAddInfo   */ "" );

    // Save position and size of dialog
    //------------------------------------

    QSettings settings;

    settings.setValue( windowTitle()+"/Geometry", saveGeometry() );

    if( m_pSplitterRemCmd != nullptr )
    {
        QList<int> listSizes = m_pSplitterRemCmd->sizes();

        for( int idx = 0; idx < listSizes.count(); idx++ )
        {
            settings.setValue( objectName() + "/RemCmdSplitterHeight" + QString::number(idx), listSizes[idx] );
        }
    }

    if( m_pSplitter != nullptr )
    {
        QList<int> listSizes = m_pSplitter->sizes();

        for( int idx = 0; idx < listSizes.count(); idx++ )
        {
            settings.setValue( objectName() + "/SplitterHeight" + QString::number(idx), listSizes[idx] );
        }
    }

    // Call base method to hide the dialog
    //------------------------------------

    QDialog::closeEvent(i_pEv);

} // closeEvent

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CDlgRemCmds::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    CMsgReqAddLogItem* pMsg = dynamic_cast<CMsgReqAddLogItem*>(i_pEv);

    if( pMsg != nullptr )
    {
        if( pMsg->getReceiver() != this || pMsg->getSender() != this )
        {
            return bEventHandled;
        }

        if( pMsg->isBaseMsgType() && pMsg->getMsgType() == EBaseMsgTypeReqAddLogItem )
        {
            if( m_pEdtLog != nullptr )
            {
                QString strItem;
                QString strMsg = pMsg->getMsg();

                QString strTime = pMsg->getDateTimeUtc().toLocalTime().toString("hh:mm:ss:zzz");
                QString strSysTimeFill("            ");
                QString strSysTime = QString::number(pMsg->getSystemTimeInSec(),'f',6);

                strMsg.replace("<","&lt;");
                strMsg.replace(">","&gt;");

                if( strSysTime.length() < 12 )
                {
                    strSysTime.insert(0,strSysTimeFill.data(),12-strSysTime.length());
                    strSysTime.replace(" ","&nbsp;");
                }
                strItem = CEnumTransmitDir::toString(pMsg->getTransmitDir(),EEnumEntryAliasStrSymbol) + ": " + strTime + " (" + strSysTime + "): " + strMsg;

                if( pMsg->isBold() )
                {
                    m_pEdtLog->append( "<FONT color=black><b>" + strItem + "</b></FONT>" );
                }
                else
                {
                    m_pEdtLog->append( "<FONT color=black>" + strItem + "</FONT>" );
                }

            } // switch( pMsg->getTrcMsgLogWdgt() )
        } // if( pMsg->type() == ESystemMsgTypeLog )
    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QWidget::event(i_pEv);
    }
    return bEventHandled;

} // event

/*==============================================================================
public: // instance methods which may be called from any but not just the GUIMainThread
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgRemCmds::addLogItem( ETransmitDir i_transmitDir, bool i_bBold, const QString& i_strMsg )
//------------------------------------------------------------------------------
{
    CMsgReqAddLogItem* pMsg = new CMsgReqAddLogItem(
        /* pObjSender   */ this,
        /* pObjReceiver */ this,
        /* transmitDir  */ i_transmitDir,
        /* bBold        */ i_bBold,
        /* szMsg        */ i_strMsg );
    POST_OR_DELETE_MESSAGE(pMsg);
    pMsg = nullptr;

} // addLogItem

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgRemCmds::onBtnConnectClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnConnectClicked",
        /* strAddInfo   */ "" );

    if( m_pBtnConnect->text() == "Connect" )
    {
        if( !m_pRemCmdClient->isBusy() )
        {
            m_pRemCmdClient->connect_();
        }
    }
    else if( m_pBtnConnect->text() == "Disconnect" )
    {
        if( !m_pRemCmdClient->isBusy() )
        {
            m_pRemCmdClient->disconnect_();
        }
    }
    else if( m_pBtnConnect->text() == "Abort" )
    {
        m_pRemCmdClient->abortRequestInProgress();
    }

} // onBtnConnectClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgRemCmds::onTreeViewRemCmdObjPoolExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "ModelIdx {";
        strAddTrcInfo += "Row: " + QString::number(i_modelIdx.row());
        strAddTrcInfo += ", Clm: " + QString::number(i_modelIdx.column()) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTreeViewRemCmdObjPoolExpanded",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_modelIdx.isValid() )
    {
        m_pWdgtRemCmdObjPool->getTreeView()->resizeColumnToContents(i_modelIdx.column());
    }

} // onTreeViewRemCmdObjPoolExpanded

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdObjSelected(
    const QString& i_strCmd,
    const QString& i_strSyntax,
    const QString& i_strDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Cmd: " + i_strCmd;

        if( m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
        {
            strAddTrcInfo += ", Syntax: " + i_strSyntax;
            strAddTrcInfo += ", Dscr: " + i_strDscr;
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdObjSelected",
        /* strAddInfo   */ strAddTrcInfo );

    CRemCmdObj* pRemCmdObj = nullptr;

    if( m_pRemCmdServer != nullptr )
    {
        pRemCmdObj = m_pRemCmdServer->getRemCmdObj(i_strCmd);
    }
    else if( m_pRemCmdClient != nullptr )
    {
        pRemCmdObj = m_pRemCmdClient->getRemCmdObj(i_strCmd);
    }

    QString strCmd    = i_strCmd;
    QString strSyntax = i_strSyntax;
    QString strDscr   = i_strDscr;

    if( pRemCmdObj != nullptr )
    {
        strCmd    = pRemCmdObj->getCommandStr();
        strSyntax = pRemCmdObj->getSyntax();
        strDscr   = pRemCmdObj->getDescription();

        if( pRemCmdObj->isQueryOnly() && !strCmd.endsWith("?") )
        {
            strCmd += "?";
        }
    }

    m_pEdtRemCmd->setText(strCmd);
    m_pEdtRemCmdSyntax->setText(strSyntax);
    m_pEdtRemCmdDscr->setHtml(strDscr);

} // onRemCmdObjSelected

//------------------------------------------------------------------------------
void CDlgRemCmds::onBtnRemCmdExecuteClicked()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnRemCmdExecuteClicked",
        /* strAddInfo   */ "" );

    if( m_pEdtRemCmd != nullptr )
    {
        QString    strTxt = m_pEdtRemCmd->text().trimmed();
        QByteArray byteArrCmd;

        #if QT_VERSION < 0x050000
        byteArrCmd = strTxt.toAscii();
        #else
        byteArrCmd = strTxt.toLatin1();
        #endif

        if( !strTxt.isEmpty() )
        {
            QString    strCmd;
            bool       bIsQuery;
            QByteArray byteArrArgs;

            SErrResultInfo errResultInfo = ErrResultInfoSuccess("onBtnRemCmdExecuteClicked");

            if( m_pRemCmdServer != nullptr )
            {
                errResultInfo = m_pRemCmdServer->parseData( byteArrCmd, &strCmd, &bIsQuery, &byteArrArgs );
            }
            else if( m_pRemCmdClient != nullptr )
            {
                errResultInfo = m_pRemCmdClient->parseData( byteArrCmd, &strCmd, &bIsQuery, &byteArrArgs );
            }

            if( errResultInfo.isErrorResult() )
            {
                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }
            else // if( !errResultInfo.isErrorResult() )
            {
                if( m_pRemCmdServer != nullptr )
                {
                    // As this simulates for test purposes receiving command from client.
                    addLogItem( ETransmitDir::Receive, false, strTxt );

                    errResultInfo = m_pRemCmdServer->executeCommand( -1, strCmd, bIsQuery, byteArrArgs );
                }
                else if( m_pRemCmdClient != nullptr )
                {
                    m_pRemCmdClient->sendCommand( strCmd + QString(bIsQuery ? "?" : ""), byteArrArgs );
                }
            } // if( !errResultInfo.isErrorResult() )
        } // if( !strTxt.isEmpty() )
    } // if( m_pEdtRemCmd != nullptr )

} // onBtnRemCmdExecuteClicked

//------------------------------------------------------------------------------
void CDlgRemCmds::onBtnLogClearClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnLogClearClicked",
        /* strAddInfo   */ "" );

    if( m_pEdtLog != nullptr )
    {
        m_pEdtLog->clear();
    }

} // onBtnLogClearClicked

/*==============================================================================
protected slots: // connected to the signals of the remote command server or client
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdSrvCltDestroyed( QObject* i_pRemCmdSrvClt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RemCmdSrvClt: " + QString( i_pRemCmdSrvClt == nullptr ? "nullptr" : i_pRemCmdSrvClt->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdSrvCltDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pRemCmdServer == i_pRemCmdSrvClt )
    {
        m_pRemCmdServer = nullptr;
    }
    else if( m_pRemCmdClient == i_pRemCmdSrvClt )
    {
        m_pRemCmdClient = nullptr;
    }

} // onRemCmdSrvCltDestroyed

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdSrvCltSettingsChanged( QObject* i_pRemCmdSrvClt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RemCmdSrvClt: " + QString( i_pRemCmdSrvClt == nullptr ? "nullptr" : i_pRemCmdSrvClt->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdSrvCltSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pRemCmdClient != nullptr )
    {
        QString strCnct = m_pRemCmdClient->getHostSettings().getConnectionString();

        if( m_pProgressBarCnct != nullptr )
        {
            m_pProgressBarCnct->setLabelText(strCnct);
            m_pProgressBarCnct->reset();
            m_pProgressBarCnct->stop();
        }
    }

} // onRemCmdSrvCltSettingsChanged

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdSrvCltRequestInProgressChanged( QObject* i_pRemCmdSrvClt, ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "RemCmdSrvClt: " + QString( i_pRemCmdSrvClt == nullptr ? "nullptr" : i_pRemCmdSrvClt->objectName() );
        strAddTrcInfo += ", ReqDscr {" + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdSrvCltRequestInProgressChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pRemCmdClient != nullptr )
    {
        SClientHostSettings cnctSettings = m_pRemCmdClient->getHostSettings();

        QString strCnctText = cnctSettings.getConnectionString();

        if( i_reqDscr.m_iProgress_perCent == 0 )
        {
            if( m_pBtnConnect != nullptr )
            {
                m_pBtnConnect->setText("Abort");
            }

            if( i_reqDscr.m_request == CRemCmdClient::ERequestChangeSettings )
            {
                strCnctText += " Connecting ...";
            }
            else if( i_reqDscr.m_request == CRemCmdClient::ERequestConnect )
            {
                strCnctText += " Connecting ...";
            }
            else if( i_reqDscr.m_request == CRemCmdClient::ERequestDisconnect )
            {
                strCnctText += " Disconnecting ...";
            }

            if( m_pProgressBarCnct != nullptr )
            {
                m_pProgressBarCnct->setLabelText(strCnctText);
                m_pProgressBarCnct->reset();
                m_pProgressBarCnct->setDurationInMs(cnctSettings.m_iConnectTimeout_ms);
                m_pProgressBarCnct->setIncrementInMs(200);
                m_pProgressBarCnct->start();
            }
        } // if( i_reqDscr.m_iProgress_perCent == 0 )

        else if( i_reqDscr.m_iProgress_perCent >= 100 )
        {
            if( m_pBtnConnect != nullptr )
            {
                if( m_pRemCmdClient->isConnected() )
                {
                    m_pBtnConnect->setText("Disconnect");
                }
                else
                {
                    m_pBtnConnect->setText("Connect");
                }
            }
            if( m_pProgressBarCnct != nullptr )
            {
                m_pProgressBarCnct->setLabelText(strCnctText);
                m_pProgressBarCnct->stop();
                m_pProgressBarCnct->reset();
            }
        } // if( i_reqDscr.m_iProgress_perCent >= 100 )
    } // if( m_pRemCmdClient != nullptr )

} // onRemCmdSrvCltRequestInProgressChanged

/*==============================================================================
protected slots: // connected to the signals of the remote command server
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdServerConnected( QObject* i_pRemCmdServer, const ZS::Ipc::SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "RemCmdServer: " + QString( i_pRemCmdServer == nullptr ? "nullptr" : i_pRemCmdServer->objectName() );
        strAddTrcInfo += ", SocketDscr {" + i_socketDscr.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdServerConnected",
        /* strAddInfo   */ strAddTrcInfo );

} // onRemCmdServerConnected

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdServerDisconnected( QObject* i_pRemCmdServer, const ZS::Ipc::SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "RemCmdServer: " + QString( i_pRemCmdServer == nullptr ? "nullptr" : i_pRemCmdServer->objectName() );
        strAddTrcInfo += ", SocketDscr {" + i_socketDscr.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdServerDisconnected",
        /* strAddInfo   */ strAddTrcInfo );

} // onRemCmdServerDisconnected

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdServerReceivedData(
    QObject*          i_pRemCmdServer,
    int               i_iSocketId,
    const QByteArray& i_byteArrData )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "RemCmdServer: " + QString( i_pRemCmdServer == nullptr ? "nullptr" : i_pRemCmdServer->objectName() );
        strAddTrcInfo += ", SocketId: " + QString::number(i_iSocketId);
        strAddTrcInfo += ", Data: " + i_byteArrData;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdServerReceivedData",
        /* strAddInfo   */ strAddTrcInfo );

    addLogItem( ETransmitDir::Receive, false, i_byteArrData );

} // onRemCmdServerReceivedData

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdServerDataSent(
    QObject*          i_pRemCmdServer,
    int               i_iSocketId,
    const QByteArray& i_byteArrData )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "RemCmdServer: " + QString( i_pRemCmdServer == nullptr ? "nullptr" : i_pRemCmdServer->objectName() );
        strAddTrcInfo += ", SocketId: " + QString::number(i_iSocketId);
        strAddTrcInfo += ", Data: " + i_byteArrData;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdServerDataSent",
        /* strAddInfo   */ strAddTrcInfo );

    addLogItem( ETransmitDir::Send, true, i_byteArrData );

} // onRemCmdServerDataSent

/*==============================================================================
protected slots: // connected to the signals of the remote command client
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdClientConnected( QObject* i_pRemCmdClient )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RemCmdClient: " + QString( i_pRemCmdClient == nullptr ? "nullptr" : i_pRemCmdClient->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdClientConnected",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pBtnConnect != nullptr )
    {
        m_pBtnConnect->setText("Disconnect");
    }
    if( m_pBtnRemCmdExecute != nullptr )
    {
        m_pBtnRemCmdExecute->setEnabled(true);
    }
    if( m_pProgressBarCnct != nullptr )
    {
        if( m_pRemCmdClient != nullptr )
        {
            QString strCnct = m_pRemCmdClient->getHostSettings().getConnectionString();

            m_pProgressBarCnct->setLabelText(strCnct);
            m_pProgressBarCnct->reset();
            m_pProgressBarCnct->stop();
        }
    }

} // onRemCmdClientConnected

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdClientDisconnected( QObject* i_pRemCmdClient )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RemCmdClient: " + QString( i_pRemCmdClient == nullptr ? "nullptr" : i_pRemCmdClient->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdClientDisconnected",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pBtnConnect != nullptr )
    {
        m_pBtnConnect->setText("Connect");
    }
    if( m_pBtnRemCmdExecute != nullptr )
    {
        m_pBtnRemCmdExecute->setEnabled(false);
    }
    if( m_pProgressBarCnct != nullptr )
    {
        if( m_pRemCmdClient != nullptr )
        {
            QString strCnct = m_pRemCmdClient->getHostSettings().getConnectionString();

            m_pProgressBarCnct->setLabelText(strCnct);
            m_pProgressBarCnct->reset();
            m_pProgressBarCnct->stop();
        }
    }

} // onRemCmdClientDisconnected

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdClientReceivedData(
    QObject*          i_pRemCmdClient,
    const QByteArray& i_byteArrData )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "RemCmdClient: " + QString( i_pRemCmdClient == nullptr ? "nullptr" : i_pRemCmdClient->objectName() );
        strAddTrcInfo += ", Data: " + i_byteArrData;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdClientReceivedData",
        /* strAddInfo   */ strAddTrcInfo );

    addLogItem( ETransmitDir::Receive, false, i_byteArrData );

} // onRemCmdClientReceivedData

//------------------------------------------------------------------------------
void CDlgRemCmds::onRemCmdClientCommandSent(
    QObject*          i_pRemCmdClient,
    const QString&    i_strCmd,
    const QByteArray& i_byteArrArgs )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "RemCmdClient: " + QString( i_pRemCmdClient == nullptr ? "nullptr" : i_pRemCmdClient->objectName() );
        strAddTrcInfo += ", Cmd: " + i_strCmd;
        strAddTrcInfo += ", Args: " + i_byteArrArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdClientCommandSent",
        /* strAddInfo   */ strAddTrcInfo );

    addLogItem( ETransmitDir::Send, true, i_strCmd + " " + i_byteArrArgs );

} // onRemCmdClientCommandSent
