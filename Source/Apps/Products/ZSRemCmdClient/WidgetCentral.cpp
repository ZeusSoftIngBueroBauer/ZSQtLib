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

#include <QtCore/qsettings.h>
#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qscrollarea.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "WidgetCentral.h"
#include "App.h"

#include "ZSRemCmdGUI/ZSRemCmdObjPoolWdgt.h"
#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdSubSysNode.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSysGUI/ZSSysProgressBar.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysObjPool.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTime.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::RemCmd;
using namespace ZS::RemCmd::GUI;
using namespace ZS::Apps::Products::RemCmdClient;


/*******************************************************************************
class CWidgetCentral : public QWidget
*******************************************************************************/

static const QString c_strBtnConnect    = "Connect";
static const QString c_strBtnDisconnect = "Disconnect";
static const QString c_strBtnAbort      = "Abort";

/*==============================================================================
private: // class members
==============================================================================*/

CWidgetCentral* CWidgetCentral::s_pThis = nullptr; // singleton class

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral::CWidgetCentral(
    CRemCmdClient*  i_pRemCmdClient,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pRemCmdClient(i_pRemCmdClient),
    m_pLyt(nullptr),
    m_pLytLineCnct(nullptr),
    m_pBtnConnect(nullptr),
    m_pProgressBarCnct(nullptr),
    m_pSplitter(nullptr),
    m_pSplitterRemCmd(nullptr),
    m_pGrpRemCmdObjPool(nullptr),
    m_pLytRemCmdObjPool(nullptr),
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
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("CentralWidget");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::RemCmdClient", "CWdgtCentral", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    int cxWidthClm1 =  80;
    int cxBtnWidth  = 120;

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // <Line> Remote Host
    //-------------------

    m_pLytLineCnct = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineCnct);

    m_pBtnConnect = new QPushButton();
    m_pBtnConnect->setFixedWidth(cxBtnWidth);
    m_pLytLineCnct->addWidget(m_pBtnConnect);

    if( m_pRemCmdClient->isConnected() )
    {
        m_pBtnConnect->setText(c_strBtnDisconnect);
    }
    else
    {
        m_pBtnConnect->setText(c_strBtnConnect);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnConnect,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnConnectClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pProgressBarCnct = new CProgressBar();
    m_pProgressBarCnct->setTextVisible(false);
    m_pLytLineCnct->addWidget(m_pProgressBarCnct,1);

    // <Splitter> Main
    //==================

    // Dividing remote command group and log widget

    m_pSplitter = new QSplitter(Qt::Vertical);
    m_pLyt->addWidget(m_pSplitter);

    // <GroupBox> Remote Commands
    //===========================

    m_pGrpRemCmdObjPool = new QGroupBox("Remote Commands");
    m_pLytRemCmdObjPool = new QVBoxLayout();
    m_pLytRemCmdObjPool->setContentsMargins(0,0,0,0);
    m_pGrpRemCmdObjPool->setLayout(m_pLytRemCmdObjPool);
    m_pSplitter->addWidget(m_pGrpRemCmdObjPool);

    // Dividing remote command tree view and selected command with command description
    m_pSplitterRemCmd = new QSplitter(Qt::Vertical);
    m_pLytRemCmdObjPool->addWidget(m_pSplitterRemCmd);

    m_pWdgtRemCmdObjPool = new CWdgtRemCmdObjPool();
    m_pWdgtRemCmdObjPool->setObjPool( m_pRemCmdClient->getObjPool() );
    m_pSplitterRemCmd->addWidget(m_pWdgtRemCmdObjPool);

    m_pWdgtRemCmdObjPool->onBtnObjPoolExpandAllClicked(true);
    m_pWdgtRemCmdObjPool->onBtnObjPoolResizeRowsAndColumnsToContentsClicked(true);

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtRemCmdObjPool->getTreeView(),
        /* szSignal     */ SIGNAL( expanded(const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTreeViewRemCmdObjPoolExpanded(const QModelIndex&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pWdgtRemCmdObjPool,
        /* szSignal     */ SIGNAL( remCmdObjSelected(const QString&,const QString&,const QString&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onRemCmdObjSelected(const QString&,const QString&,const QString&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Widget> Selected Command, Syntax, Description
    //-----------------------------------------------

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
    m_pLytLineRemCmd->addWidget(m_pEdtRemCmd,1);

    m_pBtnRemCmdExecute = new QPushButton("Execute");
    m_pBtnRemCmdExecute->setEnabled(false);
    m_pLytLineRemCmd->addWidget(m_pBtnRemCmdExecute);

    if( !connect(
        /* pObjSender   */ m_pBtnRemCmdExecute,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnRemCmdExecuteClicked(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Line> Selected Command Syntax
    //-------------------------------------------

    m_pLytLineRemCmdSyntax = new QHBoxLayout();
    m_pLytRemCmd->addLayout(m_pLytLineRemCmdSyntax);

    m_pLblRemCmdSyntax = new QLabel("Syntax:");
    m_pLblRemCmdSyntax->setFixedWidth(cxWidthClm1);
    m_pLytLineRemCmdSyntax->addWidget(m_pLblRemCmdSyntax);

    m_pEdtRemCmdSyntax = new QLineEdit();
    m_pLytLineRemCmdSyntax->addWidget(m_pEdtRemCmdSyntax,1);

    // <TextEdit> Description of selected Command
    //-------------------------------------------

    m_pEdtRemCmdDscr = new QTextEdit();
    m_pLytRemCmd->addWidget(m_pEdtRemCmdDscr,1);

    // <GroupBox> Command Log
    //===========================

    QFont fntEdtLog(
        /* strFamily  */ "Courier New",
        /* iPointSize */ 8,
        /* iWeight    */ -1,
        /* bItalic    */ false );

    m_pGrpLog = new QGroupBox("Log");
    m_pLytLog = new QVBoxLayout();
    m_pGrpLog->setLayout(m_pLytLog);
    m_pSplitter->addWidget(m_pGrpLog);

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

    // Geometry
    //====================

    // Restore position and size

    QSettings settings;

    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

    if( m_pSplitterRemCmd != nullptr )
    {
        QList<int> listSizes = m_pSplitterRemCmd->sizes();
        int        idx;

        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            listSizes[idx] = settings.value( objectName() + "/RemCmdSplitterHeight" + QString::number(idx), 50 ).toInt();
        }
        m_pSplitterRemCmd->setSizes(listSizes);
    }

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

    // Keeping indicated parameters up to date ..
    //===========================================

    onRemCmdClientSettingsChanged(m_pRemCmdClient);

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdClient,
        /* szSignal     */ SIGNAL(connected(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRemCmdClientConnected(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdClient,
        /* szSignal     */ SIGNAL(disconnected(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRemCmdClientDisconnected(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdClient,
        /* szSignal     */ SIGNAL(settingsChanged(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRemCmdClientSettingsChanged(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdClient,
        /* szSignal     */ SIGNAL( receivedData(QObject*,const QByteArray&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onRemCmdClientReceivedData(QObject*,const QByteArray&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdClient,
        /* szSignal     */ SIGNAL( commandSent(QObject*,const QString&,const QByteArray&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onRemCmdClientCommandSent(QObject*,const QString&,const QByteArray&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdClient,
        /* szSignal     */ SIGNAL( requestInProgressChanged(QObject*,ZS::System::SRequestDscr) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onRemCmdClientRequestInProgressChanged(QObject*,ZS::System::SRequestDscr) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    QSettings settings;

    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    if( m_pSplitterRemCmd != nullptr )
    {
        QList<int> listSizes = m_pSplitterRemCmd->sizes();
        int        idx;

        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            settings.setValue( objectName() + "/RemCmdSplitterHeight" + QString::number(idx), listSizes[idx] );
        }
    }

    if( m_pSplitter != nullptr )
    {
        QList<int> listSizes = m_pSplitter->sizes();
        int        idx;

        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            settings.setValue( objectName() + "/SplitterHeight" + QString::number(idx), listSizes[idx] );
        }
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pRemCmdClient = nullptr;
    m_pLyt = nullptr;
    m_pLytLineCnct = nullptr;
    m_pProgressBarCnct = nullptr;
    m_pBtnConnect = nullptr;
    m_pSplitter = nullptr;
    m_pSplitterRemCmd = nullptr;
    m_pGrpRemCmdObjPool = nullptr;
    m_pLytRemCmdObjPool = nullptr;
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

} // dtor

/*==============================================================================
public: // instance methods which may be called from any but not just the GUIMainThread
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::addLogItem( ETransmitDir i_transmitDir, bool i_bBold, const QString& i_strMsg )
//------------------------------------------------------------------------------
{
    CMsgReqAddLogItem* pMsg = new CMsgReqAddLogItem(
        /* pObjSender   */ this,
        /* pObjReceiver */ this,
        /* transmitDir  */ i_transmitDir,
        /* bBold        */ i_bBold,
        /* szMsg        */ i_strMsg );
    QCoreApplication::postEvent(this,pMsg);
    pMsg = nullptr;

} // addLogItem

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnConnectClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnConnectClicked",
        /* strAddInfo   */ "" );

    if( m_pBtnConnect->text() == c_strBtnConnect )
    {
        if( !m_pRemCmdClient->isBusy() )
        {
            m_pRemCmdClient->connect_();
        }
    }
    else if( m_pBtnConnect->text() == c_strBtnDisconnect )
    {
        if( !m_pRemCmdClient->isBusy() )
        {
            m_pRemCmdClient->disconnect_();
        }
    }
    else if( m_pBtnConnect->text() == c_strBtnAbort )
    {
        m_pRemCmdClient->abortRequestInProgress();
    }

} // onBtnConnectClicked

/*==============================================================================
protected slots: // connected to the signals of the tree view
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onTreeViewRemCmdObjPoolExpanded( const QModelIndex& i_iModelIdx )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTreeViewRemCmdObjPoolExpanded",
        /* strAddInfo   */ "" );

    if( i_iModelIdx.isValid() )
    {
        m_pWdgtRemCmdObjPool->getTreeView()->resizeColumnToContents(i_iModelIdx.column());
    }

} // onTreeViewRemCmdObjPoolExpanded

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onRemCmdObjSelected( const QString& i_strCmd, const QString& i_strSyntax, const QString& i_strDscr )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdObjSelected",
        /* strAddInfo   */ "" );

    m_pEdtRemCmd->setText(i_strCmd);
    m_pEdtRemCmdSyntax->setText(i_strSyntax);
    m_pEdtRemCmdDscr->setHtml(i_strDscr);

} // onTreeViewRemCmdObjPoolCurrentChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnLogClearClicked( bool /*i_bChecked*/ )
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
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnRemCmdExecuteClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnRemCmdExecuteClicked",
        /* strAddInfo   */ "" );

    if( m_pEdtRemCmd != nullptr )
    {
        QByteArray byteArrCmd = m_pEdtRemCmd->text().toLatin1();

        if( !byteArrCmd.isEmpty() )
        {
            QString    strCmd;
            bool       bIsQuery;
            QByteArray byteArrArgs;

            SErrResultInfo errResultInfo = m_pRemCmdClient->parseData( byteArrCmd, &strCmd, &bIsQuery, &byteArrArgs );

            if( errResultInfo.isErrorResult() )
            {
                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }
            else // if( !errResultInfo.isErrorResult() )
            {
                if( bIsQuery )
                {
                    strCmd += "?";
                }
                m_pRemCmdClient->sendCommand( strCmd, byteArrArgs );

            } // if( !errResultInfo.isErrorResult() )
        } // if( !byteArrCmd.isEmpty() )
    } // if( m_pEdtRemCmd != nullptr )

} // onBtnRemCmdExecuteClicked

/*==============================================================================
protected: // overridables of inherited class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::mouseDoubleClickEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    if( m_pProgressBarCnct != nullptr )
    {
        QRect  rct = m_pProgressBarCnct->geometry();
        QPoint ptMousePos = i_pEv->pos();

        if( rct.contains(ptMousePos) )
        {
            emit editRemoteSettingsTriggered();
        }
    }

    QWidget::mouseDoubleClickEvent(i_pEv);

} // mouseDoubleClickEvent

/*==============================================================================
protected: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CWidgetCentral::event( QEvent* i_pEv )
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
protected slots: // connected to the signals of the remote command client
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onRemCmdClientConnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdClientConnected",
        /* strAddInfo   */ "" );

    if( m_pBtnConnect != nullptr )
    {
        m_pBtnConnect->setText(c_strBtnDisconnect);
    }
    if( m_pBtnRemCmdExecute != nullptr )
    {
        m_pBtnRemCmdExecute->setEnabled(true);
    }
    if( m_pProgressBarCnct != nullptr )
    {
        SClientHostSettings cnctSettings = m_pRemCmdClient->getHostSettings();
        QString strText = cnctSettings.getConnectionString();
        m_pProgressBarCnct->setLabelText(strText);
        m_pProgressBarCnct->reset();
        m_pProgressBarCnct->stop();
    }

} // onRemCmdClientConnected

//------------------------------------------------------------------------------
void CWidgetCentral::onRemCmdClientDisconnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdClientDisconnected",
        /* strAddInfo   */ "" );

    if( m_pBtnConnect != nullptr )
    {
        m_pBtnConnect->setText(c_strBtnConnect);
    }
    if( m_pBtnRemCmdExecute != nullptr )
    {
        m_pBtnRemCmdExecute->setEnabled(false);
    }
    if( m_pProgressBarCnct != nullptr )
    {
        SClientHostSettings cnctSettings = m_pRemCmdClient->getHostSettings();
        QString strText = cnctSettings.getConnectionString();
        m_pProgressBarCnct->setLabelText(strText);
        m_pProgressBarCnct->reset();
        m_pProgressBarCnct->stop();
    }

} // onRemCmdClientDisconnected

//------------------------------------------------------------------------------
void CWidgetCentral::onRemCmdClientSettingsChanged( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdClientSettingsChanged",
        /* strAddInfo   */ "" );

    if( m_pProgressBarCnct != nullptr )
    {
        SClientHostSettings cnctSettings = m_pRemCmdClient->getHostSettings();
        QString strText = cnctSettings.getConnectionString();
        m_pProgressBarCnct->setLabelText(strText);
    }

} // onRemCmdClientSettingsChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onRemCmdClientReceivedData(
    QObject*          /*i_pRemCmdClient*/,
    const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdClientReceivedData",
        /* strAddInfo   */ "" );

    addLogItem( ETransmitDir::Receive, false, QString(i_byteArr) );

} // onRemCmdClientReceivedData

//------------------------------------------------------------------------------
void CWidgetCentral::onRemCmdClientCommandSent(
    QObject*          /*i_pRemCmdClient*/,
    const QString&    i_strCmd,
    const QByteArray& i_byteArrArgs )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdClientCommandSent",
        /* strAddInfo   */ "" );

    QString str = i_strCmd;

    if( !i_byteArrArgs.isEmpty() )
    {
        str += " " + i_byteArrArgs;
    }

    addLogItem( ETransmitDir::Send, true, str );

} // onRemCmdClientCommandSent

//------------------------------------------------------------------------------
void CWidgetCentral::onRemCmdClientRequestInProgressChanged(
    QObject*                 /*i_pRemCmdClient*/,
    ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddInfoTrcDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + i_reqDscr.getAddTrcInfoStr(iAddInfoTrcDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdClientRequestInProgressChanged",
        /* strAddInfo   */ strAddTrcInfo );

    SClientHostSettings cnctSettings = m_pRemCmdClient->getHostSettings();

    QString strCnctText = cnctSettings.getConnectionString();

    if( i_reqDscr.m_iProgress_perCent == 0 )
    {
        if( m_pBtnConnect != nullptr )
        {
            m_pBtnConnect->setText(c_strBtnAbort);
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
                m_pBtnConnect->setText(c_strBtnDisconnect);
            }
            else
            {
                m_pBtnConnect->setText(c_strBtnConnect);
            }
        }
        if( m_pProgressBarCnct != nullptr )
        {
            m_pProgressBarCnct->setLabelText(strCnctText);
            m_pProgressBarCnct->stop();
            m_pProgressBarCnct->reset();
        }

    } // if( i_reqDscr.m_iProgress_perCent >= 100 )

} // onRemCmdClientRequestInProgressChanged
