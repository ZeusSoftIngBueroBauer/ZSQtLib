/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qdatetime.h>
#include <QtCore/qsettings.h>
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>
#include <QtNetwork/qhostinfo.h>
#include <QtGui/qvalidator.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qformlayout.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtableview.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSIpcGUI/ZSIpcServerWdgt.h"
#include "ZSIpcGUI/ZSIpcTrcMsgLogWdgt.h"
#include "ZSIpcGUI/ZSIpcModelSrvCltConnection.h"
#include "ZSIpc/ZSIpcServer.h"
#include "ZSSysGUI/ZSSysProgressBar.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Ipc::GUI;
using namespace ZS::Trace;


/*******************************************************************************
class CWdgtIpcServer : public QWidget
*******************************************************************************/

static const QString c_strBtnStartup  = "Startup";
static const QString c_strBtnShutdown = "Shutdown";
static const QString c_strBtnAbort    = "Abort";

static const QString c_strBtnDetailsShowDetails = "Details >>";
static const QString c_strBtnDetailsHideDetails = "<< No Details";

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtIpcServer::CWdgtIpcServer( const QString& i_strServerName, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pServer(nullptr),
    m_bProtocolTypeImageVisible(true),
    m_iLblWidth(140),
    m_pLyt(nullptr),
    m_pWdgtServerObjName(nullptr),
    m_pLytWdgtServerObjName(nullptr),
    m_pLytLineServerObjName(nullptr),
    m_pLblServerObjName(nullptr),
    m_pLblServerProtocolTypeImg(nullptr),
    m_pLblServerState(nullptr),
    m_pLedServerState(nullptr),
    // Connection Settings (SServerHostSettings)
    m_hostSettingsServer(),
    m_hostSettingsWidget(),
    m_pLytCnct(nullptr),
    m_pLblSocketType(nullptr),
    m_pCmbSocketType(nullptr),
    m_pLblLocalHostName(nullptr),
    m_pEdtLocalHostName(nullptr),
    m_pLblLocalHostAddr(nullptr),
    m_pEdtLocalHostAddr(nullptr),
    m_bHostInfoLookUpInProcess(false),
    m_pLblLocalPort(nullptr),
    m_pEdtLocalPort(nullptr),
    m_pLblMaxPendingConnections(nullptr),
    m_pEdtMaxPendingConnections(nullptr),
    m_pLblBufferSize_bytes(nullptr),
    m_pEdtBufferSize_bytes(nullptr),
    // Buttons
    m_pLytBtns(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnCancel(nullptr),
    m_pBtnReset(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnStartup(nullptr),
    // Details
    m_bShowDetails(false),
    m_pWdgtDetails(nullptr),
    m_pLytDetails(nullptr),
    m_pBtnDetails(nullptr),
    m_pWdgtDetailsStates(nullptr),
    m_pLytDetailsStates(nullptr),
    m_pLblDetailsConnectionSettingsApplied(nullptr),
    m_pEdtDetailsConnectionSettingsApplied(nullptr),
    m_pLblDetailsState(nullptr),
    m_pEdtDetailsState(nullptr),
    m_pLblRequestInProgress(nullptr),
    m_pBarRequestInProgress(nullptr),
    // Splitter for following tables:
    m_pSplitterDetails(nullptr),
    // Active Connections
    m_pLytLineConnections(nullptr),
    m_pLblActiveConnections(nullptr),
    m_pEdtActiveConnections(nullptr),
    m_pLblArrLenConnections(nullptr),
    m_pEdtArrLenConnections(nullptr),
    m_pWdgtSrvCltConnections(nullptr),
    m_pLytSrvCltConnections(nullptr),
    m_pModelSrvCltConnection(nullptr),
    m_pTblViewSrvCltConnection(nullptr),
    m_idxLytItemStretchAtBottom(-1),
    // Trace
    m_pWdgtMsgLog(nullptr),
    m_pLytMsgLog(nullptr),
    m_pLblMsgLog(nullptr),
    m_trcMsgLogSettings(),
    m_pWdgtTrcMsgLog(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strServerName);

    // The trace server may not trace itself. This will lead to endless recursions.
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // Restore settings
    //-----------------

    readSettings();

    // Layout
    //--------

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    CSepLine* pLine;

    // <Line> Server Object Name
    //--------------------------

    m_pWdgtServerObjName = new QWidget();
    m_pLytWdgtServerObjName = new QVBoxLayout();
    m_pLytWdgtServerObjName->setContentsMargins(0, 0, 0, 0);
    m_pWdgtServerObjName->setLayout(m_pLytWdgtServerObjName);
    m_pLyt->addWidget(m_pWdgtServerObjName);

    m_pLytLineServerObjName = new QHBoxLayout();
    m_pLytWdgtServerObjName->addLayout(m_pLytLineServerObjName);

    m_pLblServerObjName = new QLabel("Server Connection Settings");
    m_pLblServerObjName->setStyleSheet("font-weight: bold;");
    m_pLytLineServerObjName->addWidget(m_pLblServerObjName, 1);

    // <Image> ProtocolType
    //---------------------

    m_pLytLineServerObjName->addStretch();

    QPixmap pxmProtocolTypeImg(":/ZS/App/Zeus32x32.bmp");

    pxmProtocolTypeImg.setMask(pxmProtocolTypeImg.createHeuristicMask());

    m_pLblServerProtocolTypeImg = new QLabel();
    m_pLblServerProtocolTypeImg->setMaximumWidth(32);
    m_pLblServerProtocolTypeImg->setMaximumHeight(32);
    m_pLblServerProtocolTypeImg->setScaledContents(true);
    m_pLblServerProtocolTypeImg->setPixmap(pxmProtocolTypeImg);
    m_pLytLineServerObjName->addWidget(m_pLblServerProtocolTypeImg);

    m_pLytLineServerObjName->addSpacing(10);

    // <Led> State
    //------------

    m_pLblServerState = new QLabel("State:");
    m_pLytLineServerObjName->addWidget(m_pLblServerState);
    m_pLedServerState = new QLabel();
    m_pLedServerState->setMaximumWidth(20);
    m_pLedServerState->setMaximumHeight(20);
    m_pLedServerState->setScaledContents(true);
    m_pLedServerState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
    m_pLytLineServerObjName->addWidget(m_pLedServerState);

    // <SepLine> at bottom of Server Object Name
    //------------------------------------------

    m_pLytWdgtServerObjName->addSpacing(5);
    pLine = new CSepLine();
    m_pLytWdgtServerObjName->addWidget(pLine);
    m_pLytWdgtServerObjName->addSpacing(5);

    // <FormLayout> Connection Parameters
    //-----------------------------------

    m_pLytCnct = new QFormLayout();
    m_pLyt->addLayout(m_pLytCnct);

    // <ComboBox> SocketType
    //-----------------------------

    m_pLblSocketType = new QLabel("Socket Type:");
    m_pLblSocketType->setFixedWidth(m_iLblWidth);
    m_pCmbSocketType = new QComboBox();
    m_pCmbSocketType->setEnabled(false);
    m_pLytCnct->addRow( m_pLblSocketType, m_pCmbSocketType );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbSocketType,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Local Host Name
    //-----------------------------

    m_pLblLocalHostName = new QLabel("Local Host:");
    m_pLblLocalHostName->setFixedWidth(m_iLblWidth);
    m_pEdtLocalHostName = new QLineEdit();
    m_pEdtLocalHostName->setEnabled(false);
    m_pLytCnct->addRow( m_pLblLocalHostName, m_pEdtLocalHostName );

    // <LineEdit> Local Host IP Address
    //----------------------------------

    m_pLblLocalHostAddr = new QLabel("IP Address:");
    m_pLblLocalHostAddr->setFixedWidth(m_iLblWidth);
    m_pEdtLocalHostAddr = new QLineEdit();
    m_pEdtLocalHostAddr->setEnabled(false);
    m_pLytCnct->addRow( m_pLblLocalHostAddr, m_pEdtLocalHostAddr );

    // <LineEdit> Local Host Port
    //----------------------------

    m_pLblLocalPort = new QLabel("Local Port:");
    m_pLblLocalPort->setFixedWidth(m_iLblWidth);
    m_pEdtLocalPort = new QLineEdit();
    m_pEdtLocalPort->setEnabled(false);
    m_pEdtLocalPort->setValidator( new QIntValidator(1,65535,this) );
    m_pLytCnct->addRow( m_pLblLocalPort, m_pEdtLocalPort );

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtLocalPort,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalPortTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Max Pending Connections
    //-----------------------------------

    m_pLblMaxPendingConnections = new QLabel("Max. Pending Connections:");
    m_pLblMaxPendingConnections->setFixedWidth(m_iLblWidth);
    m_pEdtMaxPendingConnections = new QLineEdit();
    m_pEdtMaxPendingConnections->setEnabled(false);
    m_pEdtMaxPendingConnections->setValidator( new QIntValidator(1,65535,this) );
    m_pLytCnct->addRow( m_pLblMaxPendingConnections, m_pEdtMaxPendingConnections );

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtMaxPendingConnections,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtMaxPendingConnectionsTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Buffer Size (for shared memory socket connections)
    //--------------------------------------------------------------

    m_pLblBufferSize_bytes = new QLabel("Buffer Size / Bytes:");
    m_pLblBufferSize_bytes->setFixedWidth(m_iLblWidth);
    m_pEdtBufferSize_bytes = new QLineEdit();
    m_pEdtBufferSize_bytes->setEnabled(false);
    m_pLytCnct->addRow( m_pLblBufferSize_bytes, m_pEdtBufferSize_bytes );

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtBufferSize_bytes,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtBufferSizeTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new CSepLine();
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

    // <ButtonRow>
    //============

    int cxBtnSpacing = 5;
    int cxBtnWidth = 70;

    m_pLytBtns = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytBtns);

    // <Button> Ok
    //------------

    m_pBtnOk = new QPushButton("Ok");
    m_pBtnOk->setFixedWidth(cxBtnWidth);
    m_pLytBtns->addWidget(m_pBtnOk);
    m_pLytBtns->addSpacing(cxBtnSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnOk,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnOkClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Cancel
    //----------------

    m_pBtnCancel = new QPushButton("Cancel");
    m_pBtnCancel->setFixedWidth(cxBtnWidth);
    m_pLytBtns->addWidget(m_pBtnCancel);
    m_pLytBtns->addSpacing(cxBtnSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnCancel,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCancelClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Reset
    //---------------

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setFixedWidth(cxBtnWidth);
    m_pBtnReset->setEnabled(false);
    m_pLytBtns->addWidget(m_pBtnReset);
    m_pLytBtns->addSpacing(cxBtnSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnReset,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnResetClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Apply
    //---------------

    m_pBtnApply = new QPushButton("Apply");
    m_pBtnApply->setFixedWidth(cxBtnWidth);
    m_pBtnApply->setEnabled(false);
    m_pLytBtns->addWidget(m_pBtnApply);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnApply,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnApplyClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Startup/Shutdown/Abort
    //----------------------------------

    m_pBtnStartup = new QPushButton();
    m_pBtnStartup->setFixedWidth(cxBtnWidth);
    m_pBtnStartup->setEnabled(false);
    m_pLytBtns->addWidget(m_pBtnStartup);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnStartup,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnStartupClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Stretch> at right side of buttons
    //-----------------------------------

    m_pLytBtns->addStretch();

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new CSepLine();
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

    // <GroupBox> Details
    //======================

    // <Widget> Details
    //----------------------

    m_pWdgtDetails = new QWidget();
    m_pLytDetails = new QVBoxLayout();
    m_pLytDetails->setContentsMargins(0,0,0,0);
    m_pWdgtDetails->setLayout(m_pLytDetails);
    m_pLyt->addWidget(m_pWdgtDetails);

    // <Button> Show Details
    //----------------------

    m_pBtnDetails = new QPushButton(c_strBtnDetailsShowDetails);
    m_pBtnDetails->setFixedWidth(m_iLblWidth);
    m_pLytDetails->addWidget(m_pBtnDetails);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDetails,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDetailsClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Widget> Details
    //-----------------

    if( m_bShowDetails )
    {
        onShowDetailsChanged();
    }

    // Stretch at bottom of main layout
    //=================================

    m_idxLytItemStretchAtBottom = m_pLyt->count();
    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtIpcServer::~CWdgtIpcServer()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ strAddTrcInfo );

    saveSettings();

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pServer = nullptr;
    m_bProtocolTypeImageVisible = false;
    m_iLblWidth = 0;
    m_pLyt = nullptr;
    m_pWdgtServerObjName = nullptr;
    m_pLytWdgtServerObjName = nullptr;
    m_pLytLineServerObjName = nullptr;
    m_pLblServerProtocolTypeImg = nullptr;
    m_pLblServerObjName = nullptr;
    m_pLblServerState = nullptr;
    m_pLedServerState = nullptr;
    // Connection Settings
    //m_hostSettingsServer;
    //m_hostSettingsWidget;
    m_pLytCnct = nullptr;
    m_pLblSocketType = nullptr;
    m_pCmbSocketType = nullptr;
    m_pLblLocalHostName = nullptr;
    m_pEdtLocalHostName = nullptr;
    m_pLblLocalHostAddr = nullptr;
    m_pEdtLocalHostAddr = nullptr;
    m_bHostInfoLookUpInProcess = false;
    m_pLblLocalPort = nullptr;
    m_pEdtLocalPort = nullptr;
    m_pLblMaxPendingConnections = nullptr;
    m_pEdtMaxPendingConnections = nullptr;
    m_pLblBufferSize_bytes = nullptr;
    m_pEdtBufferSize_bytes = nullptr;
    // Buttons
    m_pLytBtns = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnCancel = nullptr;
    m_pBtnReset = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnStartup = nullptr;
    // Details
    m_bShowDetails = false;
    m_pWdgtDetails = nullptr;
    m_pLytDetails = nullptr;
    m_pBtnDetails = nullptr;
    m_pWdgtDetailsStates = nullptr;
    m_pLytDetailsStates = nullptr;
    m_pLblDetailsConnectionSettingsApplied = nullptr;
    m_pEdtDetailsConnectionSettingsApplied = nullptr;
    m_pLblDetailsState = nullptr;
    m_pEdtDetailsState = nullptr;
    m_pLblRequestInProgress = nullptr;
    m_pBarRequestInProgress = nullptr;
    // Splitter for following tables
    m_pSplitterDetails = nullptr;
    // Active Connections
    m_pLytLineConnections = nullptr;
    m_pLblActiveConnections = nullptr;
    m_pEdtActiveConnections = nullptr;
    m_pLblArrLenConnections = nullptr;
    m_pEdtArrLenConnections = nullptr;
    m_pWdgtSrvCltConnections = nullptr;
    m_pLytSrvCltConnections = nullptr;
    m_pModelSrvCltConnection = nullptr;
    m_pTblViewSrvCltConnection = nullptr;
    m_idxLytItemStretchAtBottom = 0;
    // Trace
    m_pWdgtMsgLog = nullptr;
    m_pLytMsgLog = nullptr;
    m_pLblMsgLog = nullptr;
    //m_trcMsgLogSettings;
    m_pWdgtTrcMsgLog = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::setServer( CServer* i_pServer )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Server: " + QString( i_pServer == nullptr ? "nullptr" : i_pServer->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setServer",
        /* strAddInfo   */ strAddTrcInfo );

    // Show/hide GUI controls depending on active connection (depending on protocol type),
    // show/hide details, fill connection controls (connection parameters and watch dog settings)
    // and fill detail controls:

    if( m_pServer != i_pServer )
    {
        if( m_pServer != nullptr )
        {
            // <Signals/Slots> of connection
            //------------------------------

            QObject::disconnect(
                /* pObjSender   */ m_pServer,
                /* szSignal     */ SIGNAL(connected(QObject*,const ZS::Ipc::SSocketDscr&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onIpcServerConnected(QObject*,const ZS::Ipc::SSocketDscr&)) );

            QObject::disconnect(
                /* pObjSender   */ m_pServer,
                /* szSignal     */ SIGNAL(disconnected(QObject*,const ZS::Ipc::SSocketDscr&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onIpcServerDisconnected(QObject*,const ZS::Ipc::SSocketDscr&)) );

            QObject::disconnect(
                /* pObjSender   */ m_pServer,
                /* szSignal     */ SIGNAL(settingsChanged(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onIpcServerSettingsChanged(QObject*)) );

            QObject::disconnect(
                /* pObjSender   */ m_pServer,
                /* szSignal     */ SIGNAL(stateChanged(QObject*,int)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onIpcServerStateChanged(QObject*,int)) );

            QObject::disconnect(
                /* pObjSender   */ m_pServer,
                /* szSignal     */ SIGNAL(requestInProgressChanged(QObject*,ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onIpcServerRequestInProgressChanged(QObject*,ZS::System::SRequestDscr)) );

            resetCnctControls();

            if( m_bShowDetails )
            {
                resetDetailControls();
            }
        } // if( m_pServer != nullptr )

        m_pServer = i_pServer;

        m_hostSettingsServer = SServerHostSettings();
        m_hostSettingsWidget = SServerHostSettings();

        if( m_pServer != nullptr )
        {
            // <Signals/Slots> of connection
            //------------------------------

            if( !QObject::connect(
                /* pObjSender   */ m_pServer,
                /* szSignal     */ SIGNAL(connected(QObject*,const ZS::Ipc::SSocketDscr&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onIpcServerConnected(QObject*,const ZS::Ipc::SSocketDscr&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pServer,
                /* szSignal     */ SIGNAL(disconnected(QObject*,const ZS::Ipc::SSocketDscr&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onIpcServerDisconnected(QObject*,const ZS::Ipc::SSocketDscr&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pServer,
                /* szSignal     */ SIGNAL(settingsChanged(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onIpcServerSettingsChanged(QObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pServer,
                /* szSignal     */ SIGNAL(stateChanged(QObject*,int)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onIpcServerStateChanged(QObject*,int)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pServer,
                /* szSignal     */ SIGNAL(requestInProgressChanged(QObject*,ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onIpcServerRequestInProgressChanged(QObject*,ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            // Read current settings
            //----------------------

            m_hostSettingsServer = m_pServer->getHostSettings();

            fillCnctControls(m_hostSettingsServer);

            if( m_bShowDetails )
            {
                if( m_pModelSrvCltConnection != nullptr )
                {
                    m_pModelSrvCltConnection->setServer(m_pServer);
                }

                if( m_pWdgtTrcMsgLog != nullptr )
                {
                    m_pWdgtTrcMsgLog->setServer(m_pServer);
                }

                fillDetailControls();
            }
        } // if( m_pServer != nullptr )

    } // if( m_pServer != i_pServer )

} // setServer

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::setProtocolTypeImage( const QPixmap& i_pxm )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setProtocolTypeImage",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pLblServerProtocolTypeImg != nullptr )
    {
        QPixmap pxmProtocolTypeImg(i_pxm);
        pxmProtocolTypeImg.setMask(pxmProtocolTypeImg.createHeuristicMask());
        m_pLblServerProtocolTypeImg->setPixmap(pxmProtocolTypeImg);
    }

} // setProtocolTypeImage

//------------------------------------------------------------------------------
void CWdgtIpcServer::setProtocolTypeImageVisible( bool i_bVisible )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = bool2Str(i_bVisible);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setProtocolTypeImageVisible",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_bProtocolTypeImageVisible != i_bVisible )
    {
        m_bProtocolTypeImageVisible = i_bVisible;

        if( m_bProtocolTypeImageVisible )
        {
            int idxLblServerState = m_pLytLineServerObjName->indexOf(m_pLblServerState);
            m_pLytLineServerObjName->insertWidget(idxLblServerState, m_pLblServerProtocolTypeImg);
        }
        else
        {
            m_pLytLineServerObjName->removeWidget(m_pLblServerProtocolTypeImg);
        }
    } // if( m_bProtocolTypeImageVisible != i_bVisible )

} // setProtocolTypeImageVisible

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::readSettings()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "readSettings",
        /* strAddInfo   */ strAddTrcInfo );

    QSettings settings;

    QString strSettingsKey = objectName();
    bool    bSyncSettings  = false;

    if( settings.contains(strSettingsKey+"/ShowDetails") )
    {
        m_bShowDetails = str2Bool( settings.value(strSettingsKey+"/ShowDetails",bool2Str(m_bShowDetails)).toString() );
    }
    else
    {
        settings.setValue( strSettingsKey+"/ShowDetails", bool2Str(m_bShowDetails) );
        bSyncSettings = true;
    }
    if( settings.contains(strSettingsKey+"/TrcMsgLogEnabled") )
    {
        m_trcMsgLogSettings.m_bEnabled = str2Bool( settings.value(strSettingsKey+"/TrcMsgLogEnabled",bool2Str(m_trcMsgLogSettings.m_bEnabled)).toString() );
    }
    else
    {
        settings.setValue( strSettingsKey+"/TrcMsgLogEnabled", bool2Str(m_trcMsgLogSettings.m_bEnabled) );
        bSyncSettings = true;
    }
    if( settings.contains(strSettingsKey+"/TrcMsgLogMaxItems") )
    {
        m_trcMsgLogSettings.m_uMaxItems = settings.value(strSettingsKey+"/TrcMsgLogMaxItems",m_trcMsgLogSettings.m_uMaxItems).toUInt();
    }
    else
    {
        settings.setValue( strSettingsKey+"/TrcMsgLogMaxItems", m_trcMsgLogSettings.m_uMaxItems );
        bSyncSettings = true;
    }

    if( bSyncSettings )
    {
        settings.sync();
    }

} // readSettings

//------------------------------------------------------------------------------
void CWdgtIpcServer::saveSettings()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "saveSettings",
        /* strAddInfo   */ strAddTrcInfo );

    QSettings settings;

    QString strSettingsKey = objectName();

    if( m_pWdgtTrcMsgLog != nullptr )
    {
        m_trcMsgLogSettings = m_pWdgtTrcMsgLog->getSettings();
    }

    settings.setValue( strSettingsKey+"/ShowDetails", bool2Str(m_bShowDetails) );
    settings.setValue( strSettingsKey+"/TrcMsgLogEnabled", bool2Str(m_trcMsgLogSettings.m_bEnabled) );
    settings.setValue( strSettingsKey+"/TrcMsgLogMaxItems", m_trcMsgLogSettings.m_uMaxItems );

} // saveSettings

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::onCmbSocketTypeCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = QString::number(i_iIdx);
        strAddTrcInfo += " (" + socketType2Str(i_iIdx) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onCmbSocketTypeCurrentIndexChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_hostSettingsWidget.m_socketType = static_cast<ESocketType>(m_pCmbSocketType->itemData(i_iIdx).toInt());

    #ifdef _WINDOWS
    #pragma message(__TODO__"Change m_hostSettingsWidget depending on socket type and fillCnctControls")
    #endif

    fillCnctControls(m_hostSettingsWidget);

    CRequest* pReqInProgress = m_pServer->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget == m_hostSettingsServer )
        {
            m_pBtnReset->setEnabled(false);
        }
        else
        {
            m_pBtnReset->setEnabled(true);
        }
    } // if( m_pBtnReset != nullptr )

    if( m_pBtnApply != nullptr )
    {
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsServer) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

    if( m_bShowDetails )
    {
        fillDetailControls();
    }

} // onCmbSocketTypeCurrentIndexChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::onEdtLocalPortTextChanged( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strText;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtLocalPortTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    bool    bConverted;
    quint32 uVal = i_strText.toUInt(&bConverted);

    if( bConverted )
    {
        m_hostSettingsWidget.m_uLocalPort = uVal;
    }

    CRequest* pReqInProgress = m_pServer->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget == m_hostSettingsServer )
        {
            m_pBtnReset->setEnabled(false);
        }
        else
        {
            m_pBtnReset->setEnabled(true);
        }
    } // if( m_pBtnReset != nullptr )

    if( m_pBtnApply != nullptr )
    {
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsServer) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

} // onEdtLocalPortTextChanged

//------------------------------------------------------------------------------
void CWdgtIpcServer::onEdtMaxPendingConnectionsTextChanged( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strText;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtMaxPendingConnectionsTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    bool    bConverted;
    quint32 uVal = i_strText.toUInt(&bConverted);

    if( bConverted )
    {
        m_hostSettingsWidget.m_uMaxPendingConnections = uVal;
    }

    CRequest* pReqInProgress = m_pServer->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget == m_hostSettingsServer )
        {
            m_pBtnReset->setEnabled(false);
        }
        else
        {
            m_pBtnReset->setEnabled(true);
        }
    } // if( m_pBtnReset != nullptr )

    if( m_pBtnApply != nullptr )
    {
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsServer) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

} // onEdtMaxPendingConnectionsTextChanged

//------------------------------------------------------------------------------
void CWdgtIpcServer::onEdtBufferSizeTextChanged( const QString& i_strText )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strText;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtBufferSizeTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    bool    bConverted;
    quint32 uVal = i_strText.toUInt(&bConverted);

    if( bConverted )
    {
        m_hostSettingsWidget.m_uBufferSize = uVal;
    }

    CRequest* pReqInProgress = m_pServer->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget == m_hostSettingsServer )
        {
            m_pBtnReset->setEnabled(false);
        }
        else
        {
            m_pBtnReset->setEnabled(true);
        }
    } // if( m_pBtnReset != nullptr )

    if( m_pBtnApply != nullptr )
    {
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsServer) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

} // onEdtBufferSizeTextChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::onBtnOkClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnOkClicked",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_hostSettingsServer != m_hostSettingsWidget )
    {
        applySettings();
    }

    emit accepted();

} // onBtnOkClicked

//------------------------------------------------------------------------------
void CWdgtIpcServer::onBtnCancelClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnCancelClicked",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pServer != nullptr )
    {
        m_hostSettingsServer = m_pServer->getHostSettings();

        fillCnctControls(m_hostSettingsServer);

        CRequest* pReqInProgress = m_pServer->getRequestInProgress();

        if( m_pBtnReset != nullptr )
        {
            if( m_hostSettingsWidget == m_hostSettingsServer )
            {
                m_pBtnReset->setEnabled(false);
            }
            else
            {
                m_pBtnReset->setEnabled(true);
            }
        } // if( m_pBtnReset != nullptr )

        if( m_pBtnApply != nullptr )
        {
            if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsServer) )
            {
                m_pBtnApply->setEnabled(false);
            }
            else
            {
                m_pBtnApply->setEnabled(true);
            }
        } // if( m_pBtnApply != nullptr )

        if( m_bShowDetails )
        {
            fillDetailControls();
        }

    } // if( m_pServer != nullptr )

    emit rejected();

} // onBtnCancelClicked

//------------------------------------------------------------------------------
void CWdgtIpcServer::onBtnResetClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnResetClicked",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pServer != nullptr )
    {
        m_hostSettingsServer = m_pServer->getHostSettings();

        fillCnctControls(m_hostSettingsServer);

        CRequest* pReqInProgress = m_pServer->getRequestInProgress();

        if( m_pBtnReset != nullptr )
        {
            if( m_hostSettingsWidget == m_hostSettingsServer )
            {
                m_pBtnReset->setEnabled(false);
            }
            else
            {
                m_pBtnReset->setEnabled(true);
            }
        } // if( m_pBtnReset != nullptr )

        if( m_pBtnApply != nullptr )
        {
            if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsServer) )
            {
                m_pBtnApply->setEnabled(false);
            }
            else
            {
                m_pBtnApply->setEnabled(true);
            }
        } // if( m_pBtnApply != nullptr )

        if( m_bShowDetails )
        {
            fillDetailControls();
        }
    } // if( m_pServer != nullptr )

} // onBtnResetClicked

//------------------------------------------------------------------------------
void CWdgtIpcServer::onBtnApplyClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnApplyClicked",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_hostSettingsServer != m_hostSettingsWidget )
    {
        applySettings();
    }

} // onBtnApplyClicked

//------------------------------------------------------------------------------
void CWdgtIpcServer::onBtnStartupClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnStartupClicked",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pServer != nullptr )
    {
        CRequest* pReqInProgress = m_pServer->getRequestInProgress();

        if( m_pBtnStartup->text() == c_strBtnStartup )
        {
            if( pReqInProgress == nullptr )
            {
                applySettings();

                m_pServer->startup();
            }
        }
        else if( m_pBtnStartup->text() == c_strBtnShutdown )
        {
            if( pReqInProgress == nullptr )
            {
                m_pServer->shutdown();
            }
        }
        else if( m_pBtnStartup->text() == c_strBtnAbort )
        {
            m_pServer->abortRequestInProgress();
        }
    } // if( m_pServer != nullptr )

} // onBtnStartupClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::onBtnDetailsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnDetailsClicked",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pBtnDetails->text() == c_strBtnDetailsShowDetails )
    {
        m_bShowDetails = true;
    }
    else
    {
        m_bShowDetails = false;
    }

    onShowDetailsChanged();

} // onBtnDetailsClicked

/*==============================================================================
protected slots: // connected to the signals of the Ipc Server
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::onIpcServerConnected(
    QObject*           i_pServer,
    const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "Server: " + QString( i_pServer == nullptr ? "nullptr" : i_pServer->objectName() );
        strAddTrcInfo += ", SocketDscr: " + i_socketDscr.getConnectionString(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onIpcServerConnected",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pServer != nullptr && i_pServer == m_pServer )
    {
        fillCnctControls(m_hostSettingsWidget);

        CRequest* pReqInProgress = m_pServer->getRequestInProgress();

        if( m_pBtnReset != nullptr )
        {
            if( m_hostSettingsWidget == m_hostSettingsServer )
            {
                m_pBtnReset->setEnabled(false);
            }
            else
            {
                m_pBtnReset->setEnabled(true);
            }
        } // if( m_pBtnReset != nullptr )

        if( m_pBtnApply != nullptr )
        {
            if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsServer) )
            {
                m_pBtnApply->setEnabled(false);
            }
            else
            {
                m_pBtnApply->setEnabled(true);
            }
        } // if( m_pBtnApply != nullptr )

        if( m_bShowDetails )
        {
            fillDetailControls();
        }
    }

} // onIpcServerConnected

//------------------------------------------------------------------------------
void CWdgtIpcServer::onIpcServerDisconnected(
    QObject*           i_pServer,
    const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "Server: " + QString( i_pServer == nullptr ? "nullptr" : i_pServer->objectName() );
        strAddTrcInfo += ", SocketDscr: " + i_socketDscr.getConnectionString(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onIpcServerDisconnected",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pServer != nullptr && i_pServer == m_pServer )
    {
        fillCnctControls(m_hostSettingsWidget);

        CRequest* pReqInProgress = m_pServer->getRequestInProgress();

        if( m_pBtnReset != nullptr )
        {
            if( m_hostSettingsWidget == m_hostSettingsServer )
            {
                m_pBtnReset->setEnabled(false);
            }
            else
            {
                m_pBtnReset->setEnabled(true);
            }
        } // if( m_pBtnReset != nullptr )

        if( m_pBtnApply != nullptr )
        {
            if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsServer) )
            {
                m_pBtnApply->setEnabled(false);
            }
            else
            {
                m_pBtnApply->setEnabled(true);
            }
        } // if( m_pBtnApply != nullptr )

        if( m_bShowDetails )
        {
            fillDetailControls();
        }
    }

} // onIpcServerDisconnected

//------------------------------------------------------------------------------
void CWdgtIpcServer::onIpcServerStateChanged( QObject* i_pServer, int i_iState )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Server: " + QString( i_pServer == nullptr ? "nullptr" : i_pServer->objectName() );
        strAddTrcInfo += ", State: " + CServer::State2Str(i_iState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onIpcServerStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pServer != nullptr && i_pServer == m_pServer )
    {
        fillCnctControls(m_hostSettingsWidget);

        CRequest* pReqInProgress = m_pServer->getRequestInProgress();

        if( m_pBtnReset != nullptr )
        {
            if( m_hostSettingsWidget == m_hostSettingsServer )
            {
                m_pBtnReset->setEnabled(false);
            }
            else
            {
                m_pBtnReset->setEnabled(true);
            }
        } // if( m_pBtnReset != nullptr )

        if( m_pBtnApply != nullptr )
        {
            if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsServer) )
            {
                m_pBtnApply->setEnabled(false);
            }
            else
            {
                m_pBtnApply->setEnabled(true);
            }
        } // if( m_pBtnApply != nullptr )

        if( m_bShowDetails )
        {
            fillDetailControls();
        }
    }

} // onIpcServerStateChanged

//------------------------------------------------------------------------------
void CWdgtIpcServer::onIpcServerSettingsChanged( QObject* i_pServer )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Server: " + QString( i_pServer == nullptr ? "nullptr" : i_pServer->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onIpcServerSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pServer != nullptr && i_pServer == m_pServer )
    {
        m_hostSettingsServer = m_pServer->getHostSettings();

        fillCnctControls(m_hostSettingsServer);

        CRequest* pReqInProgress = m_pServer->getRequestInProgress();

        if( m_pBtnReset != nullptr )
        {
            if( m_hostSettingsWidget == m_hostSettingsServer )
            {
                m_pBtnReset->setEnabled(false);
            }
            else
            {
                m_pBtnReset->setEnabled(true);
            }
        } // if( m_pBtnReset != nullptr )

        if( m_pBtnApply != nullptr )
        {
            if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsServer) )
            {
                m_pBtnApply->setEnabled(false);
            }
            else
            {
                m_pBtnApply->setEnabled(true);
            }
        } // if( m_pBtnApply != nullptr )

        if( m_bShowDetails )
        {
            fillDetailControls();
        }
    } // if( i_pServer != nullptr && i_pServer == m_pServer )

} // onIpcServerSettingsChanged

//------------------------------------------------------------------------------
void CWdgtIpcServer::onIpcServerRequestInProgressChanged( QObject* /*i_pServer*/, ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Req {" + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onIpcServerRequestInProgressChanged",
        /* strAddInfo   */ strAddTrcInfo );

    QString strBtnStartup;
    QString strReqInProgress;

    bool bIsMainRequest   = false;
    bool bMainReqStarted  = false;
    bool bMainReqFinished = false;
    bool bIsReqInProgress = false;

    switch( i_reqDscr.m_request )
    {
        case CServer::ERequestNone:
        {
            if( m_pServer->isConnected() )
            {
                strBtnStartup = c_strBtnShutdown;
            }
            else
            {
                strBtnStartup = c_strBtnStartup;
            }
            break;
        }
        case CServer::ERequestStartup:
        {
            bIsMainRequest = true;

            if( i_reqDscr.m_iProgress_perCent < 100 )
            {
                strBtnStartup    = c_strBtnAbort;
                strReqInProgress = "Starting up ...";
                bMainReqStarted  = (i_reqDscr.m_iProgress_perCent == 0);
                bIsReqInProgress = true;
            }
            else if( i_reqDscr.m_iProgress_perCent >= 100 )
            {
                if( m_pServer->isConnected() )
                {
                    strBtnStartup = c_strBtnShutdown;
                }
                else
                {
                    strBtnStartup = c_strBtnStartup;
                }
                bMainReqFinished = true;
            }
            break;
        }
        case CServer::ERequestShutdown:
        {
            bIsMainRequest = true;

            if( i_reqDscr.m_iProgress_perCent < 100 )
            {
                strBtnStartup    = c_strBtnAbort;
                strReqInProgress = "Shutting down ...";
                bMainReqStarted  = (i_reqDscr.m_iProgress_perCent == 0);
                bIsReqInProgress = true;
            }
            else if( i_reqDscr.m_iProgress_perCent >= 100 )
            {
                strBtnStartup    = c_strBtnStartup;
                bMainReqFinished = true;
            }
            break;
        }
        case CServer::ERequestChangeSettings:
        {
            bIsMainRequest = true;

            if( i_reqDscr.m_iProgress_perCent < 100 )
            {
                strBtnStartup    = c_strBtnAbort;
                strReqInProgress = "Changing settings ...";
                bMainReqStarted  = (i_reqDscr.m_iProgress_perCent == 0);
                bIsReqInProgress = true;
            }
            else if( i_reqDscr.m_iProgress_perCent >= 100 )
            {
                if( m_pServer->isListening() )
                {
                    strBtnStartup = c_strBtnShutdown;
                }
                else
                {
                    strBtnStartup = c_strBtnStartup;
                }
                bMainReqFinished = true;
            }
            break;
        }
        case CServer::ERequestSendData:
        {
            bIsMainRequest = true;

            if( i_reqDscr.m_iProgress_perCent < 100 )
            {
                strBtnStartup    = c_strBtnAbort;
                strReqInProgress = "Sending data ...";
                bMainReqStarted  = (i_reqDscr.m_iProgress_perCent == 0);
                bIsReqInProgress = true;
            }
            else if( i_reqDscr.m_iProgress_perCent >= 100 )
            {
                if( m_pServer->isConnected() )
                {
                    strBtnStartup = c_strBtnShutdown;
                }
                else
                {
                    strBtnStartup = c_strBtnStartup;
                }
                bMainReqFinished = true;
            }
            break;
        }
        default: // Sub requests like ERequestStartThread
        {
            bIsReqInProgress = true;
            break;
        }
    } // switch( i_reqDscr.m_request )

    if( m_pBtnApply != nullptr )
    {
        if( bIsReqInProgress || (m_hostSettingsWidget == m_hostSettingsServer) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

    if( m_pBtnStartup != nullptr )
    {
        if( bIsMainRequest )
        {
            m_pBtnStartup->setText(strBtnStartup);
        }
    }

    if( m_pBarRequestInProgress != nullptr )
    {
        // New main request started ...
        if( bMainReqStarted )
        {
            m_pBarRequestInProgress->reset();
            m_pBarRequestInProgress->setDurationInMs(30000);
            m_pBarRequestInProgress->setIncrementInMs(200);
            m_pBarRequestInProgress->start();
            m_pBarRequestInProgress->setLabelText(strReqInProgress);
        }
        else if( bMainReqFinished )
        {
            m_pBarRequestInProgress->stop();
            m_pBarRequestInProgress->reset();
            m_pBarRequestInProgress->setLabelText("");
        }
        // Update of main request progress ...
        else if( bIsMainRequest )
        {
            int iDuration_ms = m_pBarRequestInProgress->getDurationInMs();
            int iElapsed_ms  = (i_reqDscr.m_iProgress_perCent * iDuration_ms) / 100;
            m_pBarRequestInProgress->setTimeElapsedInMs(iElapsed_ms);
        }
        // Update of sub request progress ...
        else
        {
            // ... continuous increment of progress bar.
        }
    } // if( m_pBarRequestInProgress != nullptr )

} // onIpcServerRequestInProgressChanged

/*==============================================================================
protected slots: // host info lookup
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::onHostInfoLookedUp( const QHostInfo& i_hostInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onHostInfoLookedUp",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pEdtLocalHostAddr != nullptr )
    {
        QList<QHostAddress> arHostAddrs = i_hostInfo.addresses();

        QString strHostAddrs;
        int     idxAddr;

        for( idxAddr = 0; idxAddr < arHostAddrs.size(); idxAddr++ )
        {
            if( arHostAddrs[idxAddr].protocol() == QAbstractSocket::IPv4Protocol )
            {
                if( !strHostAddrs.isEmpty() )
                {
                    strHostAddrs += "; ";
                }
                strHostAddrs += arHostAddrs[idxAddr].toString();
            }
        }
        m_pEdtLocalHostAddr->setText(strHostAddrs);

    } // if( m_pEdtLocalHostAddr != nullptr )

    m_bHostInfoLookUpInProcess = false;

} // onHostInfoLookedUp

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CWdgtIpcServer::getConnectionToolTip()
//------------------------------------------------------------------------------
{
    QString strToolTip;

    if( m_pServer != nullptr )
    {
        strToolTip = m_pServer->getHostSettings().getConnectionString();
    }

    return strToolTip;

} // getConnectionToolTip

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::onShowDetailsChanged()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onShowDetailsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_bShowDetails )
    {
        if( m_pWdgtDetailsStates == nullptr )
        {
            QFrame* pLine;

            QLayoutItem* pLytItemStretch = m_pLyt->itemAt(m_idxLytItemStretchAtBottom);

            if( pLytItemStretch != nullptr )
            {
                m_pLyt->removeItem(pLytItemStretch);
            }

            m_pBtnDetails->setText(c_strBtnDetailsHideDetails);

            // <Widget> Details
            //----------------------

            m_pWdgtDetailsStates = new QWidget();
            m_pLytDetailsStates = new QVBoxLayout();
            m_pLytDetailsStates->setContentsMargins(0,0,0,0);
            m_pWdgtDetailsStates->setLayout(m_pLytDetailsStates);
            m_pLytDetails->addWidget(m_pWdgtDetailsStates);

            // <Separator>
            //-------------

            m_pLytDetailsStates->addSpacing(5);

            pLine = new CSepLine();
            m_pLytDetailsStates->addWidget(pLine);

            m_pLytDetailsStates->addSpacing(5);

            QFormLayout* pLytDetailsCnctStates = new QFormLayout();
            m_pLytDetailsStates->addLayout(pLytDetailsCnctStates);

            // <Label> Applied Connection Settings
            //------------------------------------

            m_pLblDetailsConnectionSettingsApplied = new QLabel("Connection String:");
            m_pLblDetailsConnectionSettingsApplied->setFixedWidth(m_iLblWidth);
            m_pEdtDetailsConnectionSettingsApplied = new QLineEdit("");
            m_pEdtDetailsConnectionSettingsApplied->setEnabled(false);
            pLytDetailsCnctStates->addRow(m_pLblDetailsConnectionSettingsApplied,m_pEdtDetailsConnectionSettingsApplied);

            // <Label> State
            //----------------------

            m_pLblDetailsState = new QLabel("Current State:");
            m_pLblDetailsState->setFixedWidth(m_iLblWidth);
            m_pEdtDetailsState = new QLineEdit("");
            m_pEdtDetailsState->setEnabled(false);
            pLytDetailsCnctStates->addRow(m_pLblDetailsState,m_pEdtDetailsState);

            // <Label> Request In Progress
            //----------------------------

            m_pLblRequestInProgress = new QLabel("Request in Progress:");
            m_pLblRequestInProgress->setFixedWidth(m_iLblWidth);
            m_pBarRequestInProgress = new CProgressBar();
            m_pBarRequestInProgress->setTextVisible(false);
            m_pBarRequestInProgress->setEnabled(false);
            pLytDetailsCnctStates->addRow(m_pLblRequestInProgress,m_pBarRequestInProgress);

            // <Separator>
            //-------------

            m_pLytDetailsStates->addSpacing(5);

            pLine = new CSepLine();
            m_pLytDetailsStates->addWidget(pLine);

            m_pLytDetailsStates->addSpacing(5);

            // <GroupBox> Connections
            //=======================

            // Line with number of active connection and current array length
            //---------------------------------------------------------------

            m_pLytLineConnections = new QHBoxLayout();
            m_pLytDetailsStates->addLayout(m_pLytLineConnections);

            // <Label> Active Connections
            //----------------------------

            m_pLblActiveConnections = new QLabel("Active Connections:");
            m_pLytLineConnections->addWidget(m_pLblActiveConnections);
            m_pEdtActiveConnections = new QLabel("0");
            m_pLytLineConnections->addWidget(m_pEdtActiveConnections);
            m_pLytLineConnections->addSpacing(10);

            // <Label> Array Length
            //---------------------

            m_pLblArrLenConnections = new QLabel("Current Array Length:");
            m_pLytLineConnections->addWidget(m_pLblArrLenConnections);
            m_pEdtArrLenConnections = new QLabel("0");
            m_pLytLineConnections->addWidget(m_pEdtArrLenConnections);
            m_pLytLineConnections->addStretch();

            // <Splitter>
            //=======================

            m_pSplitterDetails = new QSplitter(Qt::Vertical);
            m_pSplitterDetails->setChildrenCollapsible(false);
            m_pSplitterDetails->setHandleWidth(1);
            m_pLytDetailsStates->addWidget(m_pSplitterDetails);

            // Table to indicate current connections
            //----------------------------------------

            m_pWdgtSrvCltConnections = new QWidget();
            m_pLytSrvCltConnections = new QVBoxLayout();
            m_pLytSrvCltConnections->setContentsMargins(0,0,0,0);
            m_pWdgtSrvCltConnections->setLayout(m_pLytSrvCltConnections);
            m_pSplitterDetails->addWidget(m_pWdgtSrvCltConnections);

            m_pModelSrvCltConnection = new CModelSrvCltConnection(ESrvCltTypeServer, this);
            m_pModelSrvCltConnection->setServer(m_pServer);
            m_pTblViewSrvCltConnection = new QTableView();
            m_pTblViewSrvCltConnection->setModel(m_pModelSrvCltConnection);
            m_pTblViewSrvCltConnection->setSelectionBehavior(QAbstractItemView::SelectItems);
            m_pTblViewSrvCltConnection->setSelectionMode(QAbstractItemView::SingleSelection);
            m_pTblViewSrvCltConnection->setEditTriggers(QAbstractItemView::NoEditTriggers);
            m_pTblViewSrvCltConnection->resizeColumnsToContents();
            m_pTblViewSrvCltConnection->resizeRowsToContents();
            m_pLytSrvCltConnections->addWidget(m_pTblViewSrvCltConnection);

            m_pLytSrvCltConnections->addSpacing(10);

            // <MessageLog>
            //--------------

            m_pWdgtMsgLog = new QWidget();
            m_pLytMsgLog = new QVBoxLayout();
            m_pLytMsgLog->setContentsMargins(0,0,0,0);
            m_pWdgtMsgLog->setLayout(m_pLytMsgLog);
            m_pSplitterDetails->addWidget(m_pWdgtMsgLog);

            pLine = new CSepLine();
            m_pLytMsgLog->addWidget(pLine);

            m_pLytMsgLog->addSpacing(5);

            m_pLblMsgLog = new QLabel( "Message Protocol:" );
            m_pLytMsgLog->addWidget(m_pLblMsgLog);

            m_pWdgtTrcMsgLog = new CWdgtTrcMsgLog(m_trcMsgLogSettings);
            m_pLytMsgLog->addWidget(m_pWdgtTrcMsgLog);
            if( m_pServer != nullptr )
            {
                m_pWdgtTrcMsgLog->setServer(m_pServer);
            }
        } // if( m_pWdgtDetailsStates == nullptr )

        // Fill detail controls:
        fillDetailControls();

    } // if( m_bShowDetails )

    else // if( !m_bShowDetails )
    {
        m_pBtnDetails->setText(c_strBtnDetailsShowDetails);

        if( m_pWdgtTrcMsgLog != nullptr )
        {
            m_trcMsgLogSettings = m_pWdgtTrcMsgLog->getSettings();
        }

        delete m_pWdgtDetailsStates;
        m_pWdgtDetailsStates = nullptr;

        // All childrens of the details states widget have also been deleted:
        m_pLytDetailsStates = nullptr;
        m_pLblDetailsConnectionSettingsApplied = nullptr;
        m_pEdtDetailsConnectionSettingsApplied = nullptr;
        m_pLblDetailsState = nullptr;
        m_pEdtDetailsState = nullptr;
        m_pLblRequestInProgress = nullptr;
        m_pBarRequestInProgress = nullptr;
        m_pSplitterDetails = nullptr;
        m_pLytLineConnections = nullptr;
        m_pLblActiveConnections = nullptr;
        m_pEdtActiveConnections = nullptr;
        m_pLblArrLenConnections = nullptr;
        m_pEdtArrLenConnections = nullptr;
        m_pWdgtSrvCltConnections = nullptr;
        m_pLytSrvCltConnections = nullptr;
        m_pModelSrvCltConnection = nullptr;
        m_pTblViewSrvCltConnection = nullptr;
        m_pWdgtMsgLog = nullptr;
        m_pLytMsgLog = nullptr;
        m_pLblMsgLog = nullptr;
        m_pWdgtTrcMsgLog = nullptr;

        m_idxLytItemStretchAtBottom = m_pLyt->count();
        m_pLyt->addStretch();

    } // if( !m_bShowDetails )

    // <Size>
    //-------

    // If the widget is part of a dialog the size of the
    // dialog need to be adjusted.

    emit detailsVisibilityChanged(m_bShowDetails);

} // onShowDetailsChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::resetCnctControls()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "resetCnctControls",
        /* strAddInfo   */ strAddTrcInfo );

    // <Label> Server Object Name
    //---------------------------

    if( m_pLblServerObjName != nullptr )
    {
        m_pLblServerObjName->setText("Server Connection Settings");
    }

    // <Led> State
    //------------

    if( m_pLedServerState != nullptr )
    {
        m_pLedServerState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
        m_pLedServerState->setEnabled(false);

    } // if( m_pLedServerState != nullptr )

    // <ComboBox> Socket Type
    //-----------------------

    if( m_pLblSocketType != nullptr && m_pCmbSocketType != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pCmbSocketType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) );

        m_pCmbSocketType->clear();
        m_pCmbSocketType->setEnabled(false);
        m_pCmbSocketType->show();
        m_pLblSocketType->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pCmbSocketType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblSocketType != nullptr && m_pCmbSocketType != nullptr )

    // <LineEdit> Remote Host IP Address
    //----------------------------------

    if( m_pLblLocalHostAddr != nullptr && m_pEdtLocalHostAddr != nullptr )
    {
        m_pEdtLocalHostAddr->setText("");
        m_pEdtLocalHostAddr->show();
        m_pLblLocalHostAddr->show();

    } // if( m_pLblLocalHostAddr != nullptr && m_pEdtLocalHostAddr != nullptr )

    // <LineEdit> Remote Host Port
    //----------------------------

    if( m_pLblLocalPort != nullptr && m_pEdtLocalPort != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtLocalPort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtLocalPortTextChanged(const QString&)) );

        m_pEdtLocalPort->setValidator(nullptr);
        m_pEdtLocalPort->setText("");
        m_pEdtLocalPort->setEnabled(false);
        m_pEdtLocalPort->show();
        m_pLblLocalPort->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtLocalPort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtLocalPortTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblLocalPort != nullptr && m_pEdtLocalPort != nullptr )

    // <LineEdit> Max Pending Connections
    //-----------------------------------

    if( m_pLblMaxPendingConnections != nullptr && m_pEdtMaxPendingConnections != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtMaxPendingConnections,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtMaxPendingConnectionsTextChanged(const QString&)) );

        m_pEdtMaxPendingConnections->setValidator(nullptr);
        m_pEdtMaxPendingConnections->setText("");
        m_pEdtMaxPendingConnections->setEnabled(false);
        m_pEdtMaxPendingConnections->show();
        m_pLblMaxPendingConnections->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtMaxPendingConnections,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtMaxPendingConnectionsTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblMaxPendingConnections != nullptr && m_pEdtMaxPendingConnections != nullptr )

    // <LineEdit> Buffer Size (for shared memory socket connections)
    //--------------------------------------------------------------

    if( m_pLblBufferSize_bytes != nullptr && m_pEdtBufferSize_bytes != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtBufferSize_bytes,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtBufferSizeTextChanged(const QString&)) );

        m_pEdtBufferSize_bytes->setValidator(nullptr);
        m_pEdtBufferSize_bytes->setText("");
        m_pEdtBufferSize_bytes->setEnabled(false);
        m_pEdtBufferSize_bytes->show();
        m_pLblBufferSize_bytes->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtBufferSize_bytes,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtBufferSizeTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblBufferSize_bytes != nullptr && m_pEdtBufferSize_bytes != nullptr )

    // <Button> Ok
    //------------

    if( m_pBtnOk != nullptr )
    {
        m_pBtnOk->setEnabled(false);

    } // if( m_pBtnOk != nullptr )

    // <Button> Cancel
    //----------------

    if( m_pBtnCancel != nullptr )
    {
        m_pBtnCancel->setEnabled(false);

    } // if( m_pBtnCancel != nullptr )

    // <Button> Reset
    //---------------

    if( m_pBtnReset != nullptr )
    {
        m_pBtnReset->setEnabled(false);

    } // if( m_pBtnReset != nullptr )

    // <Button> Apply
    //---------------

    if( m_pBtnApply != nullptr )
    {
        m_pBtnApply->setEnabled(false);

    } // if( m_pBtnApply != nullptr )

    // <Button> Connect/Disconnect/Abort
    //-----------------------------------

    if( m_pBtnStartup != nullptr )
    {
        m_pBtnStartup->setText(c_strBtnStartup);
        m_pBtnStartup->setEnabled(false);
    }

    // Update internal cache
    //----------------------

    m_hostSettingsWidget = SServerHostSettings();

} // resetCnctControls

//------------------------------------------------------------------------------
void CWdgtIpcServer::fillCnctControls( const SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "HostSettings {" + i_hostSettings.getConnectionString(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "fillCnctControls",
        /* strAddInfo   */ strAddTrcInfo );

    CRequest* pReqInProgress = nullptr;
    bool      bServerListening = false;
    QString   strConnection;

    if( m_pServer != nullptr )
    {
        pReqInProgress   = m_pServer->getRequestInProgress();
        bServerListening = m_pServer->isListening();
        strConnection    = m_pServer->getHostSettings().getConnectionString();
    }

    // <Label> Server Object Name
    //---------------------------

    if( m_pLblServerObjName != nullptr )
    {
        m_pLblServerObjName->setText( QString( m_pServer == nullptr ? "Server Connection Settings" : m_pServer->objectName() ) );
    }

    // <LED> State
    //------------

    if( m_pLedServerState != nullptr )
    {
        if( bServerListening )
        {
            m_pLedServerState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
        }
        else // if( !bDbServerListening )
        {
            m_pLedServerState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
        }
        m_pLedServerState->setEnabled(true);

    } // if( m_pLedServerState != nullptr )

    // <ComboBox> Socket Type
    //-----------------------

    if( m_pLblSocketType != nullptr && m_pCmbSocketType != nullptr )
    {
        int         idxCmb     = m_pCmbSocketType->currentIndex();
        ESocketType socketType = static_cast<ESocketType>(m_pCmbSocketType->itemData(idxCmb).toInt());

        if( socketType != i_hostSettings.m_socketType )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pCmbSocketType,
                /* szSignal     */ SIGNAL(currentIndexChanged(int)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) );

            idxCmb = m_pCmbSocketType->findData(i_hostSettings.m_socketType);
            m_pCmbSocketType->setCurrentIndex(idxCmb);

            if( !QObject::connect(
                /* pObjSender   */ m_pCmbSocketType,
                /* szSignal     */ SIGNAL(currentIndexChanged(int)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
        QObject::disconnect(
            /* pObjSender   */ m_pCmbSocketType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) );

        for( idxCmb = 0; idxCmb < m_pServer->getSocketTypesCount(); idxCmb++ )
        {
            ESocketType socketType = m_pServer->getSocketType(idxCmb);
            m_pCmbSocketType->addItem( socketType2Str(socketType), socketType );
        }

        idxCmb = m_pCmbSocketType->findData(i_hostSettings.m_socketType);
        m_pCmbSocketType->setCurrentIndex(idxCmb);

        if( m_pCmbSocketType->count() > 1 )
        {
            m_pCmbSocketType->setEnabled(true);
        }
        else
        {
            m_pCmbSocketType->setEnabled(false);
        }

        if( m_pServer->isSocketTypeChangeable() )
        {
            m_pCmbSocketType->show();
            m_pLblSocketType->show();
        }
        else
        {
            m_pCmbSocketType->hide();
            m_pLblSocketType->hide();
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pCmbSocketType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblSocketType != nullptr && m_pCmbSocketType != nullptr )

    // <LineEdit> Remote Host Name
    //-----------------------------

    if( m_pLblLocalHostName != nullptr && m_pEdtLocalHostName != nullptr )
    {
        if( i_hostSettings.m_socketType == Ipc::ESocketTypeInProcMsg )
        {
            m_pEdtLocalHostName->setText("");
            m_pEdtLocalHostName->hide();
            m_pLblLocalHostName->hide();
        }
        else
        {
            m_pEdtLocalHostName->setText(i_hostSettings.m_strLocalHostName);
            m_pEdtLocalHostName->show();
            m_pLblLocalHostName->show();
        }
    } // if( m_pLblLocalHostName != nullptr && m_pEdtLocalHostName != nullptr )

    // <LineEdit> Remote Host IP Address
    //----------------------------------

    if( m_pLblLocalHostAddr != nullptr && m_pEdtLocalHostAddr != nullptr )
    {
        if( i_hostSettings.m_socketType == ESocketTypeTcp )
        {
            if( !m_bHostInfoLookUpInProcess )
            {
                m_pEdtLocalHostAddr->setText("---");
                m_bHostInfoLookUpInProcess = true;
                QHostInfo::lookupHost( i_hostSettings.m_strLocalHostName, this, SLOT(onHostInfoLookedUp(const QHostInfo&)) );
            }
            m_pEdtLocalHostAddr->show();
            m_pLblLocalHostAddr->show();
        }
        else
        {
            m_pEdtLocalHostAddr->hide();
            m_pLblLocalHostAddr->hide();
        }
    } // if( m_pLblLocalHostAddr != nullptr && m_pEdtLocalHostAddr != nullptr )

    // <LineEdit> Local Port
    //----------------------

    if( m_pLblLocalPort != nullptr && m_pEdtLocalPort != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtLocalPort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtLocalPortTextChanged(const QString&)) );

        if( i_hostSettings.m_socketType == ESocketTypeInProcMsg )
        {
            m_pEdtLocalPort->setValidator(nullptr);
            m_pEdtLocalPort->setText("");
            m_pEdtLocalPort->setEnabled(false);
            m_pEdtLocalPort->hide();
            m_pLblLocalPort->hide();
        }
        else
        {
            m_pEdtLocalPort->setValidator( new QIntValidator(1,65535,this) );
            m_pEdtLocalPort->setText( QString::number(i_hostSettings.m_uLocalPort) );
            m_pEdtLocalPort->setEnabled(true);
            m_pEdtLocalPort->show();
            m_pLblLocalPort->show();
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtLocalPort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtLocalPortTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblLocalPort != nullptr && m_pEdtLocalPort != nullptr )

    // <LineEdit> Max Pending Connections
    //-----------------------------------

    if( m_pLblMaxPendingConnections != nullptr && m_pEdtMaxPendingConnections != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtMaxPendingConnections,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtMaxPendingConnectionsTextChanged(const QString&)) );

        m_pEdtMaxPendingConnections->setValidator( new QIntValidator(0,60000,this) );
        m_pEdtMaxPendingConnections->setText( QString::number(i_hostSettings.m_uMaxPendingConnections) );
        m_pEdtMaxPendingConnections->setEnabled(true);

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtMaxPendingConnections,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtMaxPendingConnectionsTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblMaxPendingConnections != nullptr && m_pEdtMaxPendingConnections != nullptr )

    // <LineEdit> Buffer Size (for shared memory socket connections)
    //--------------------------------------------------------------

    if( m_pLblBufferSize_bytes != nullptr && m_pEdtBufferSize_bytes != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtBufferSize_bytes,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtBufferSizeTextChanged(const QString&)) );

        if( i_hostSettings.m_socketType == ESocketTypeShm )
        {
            m_pEdtBufferSize_bytes->setValidator( new QIntValidator(1024,65536,this) );
            m_pEdtBufferSize_bytes->setText( QString::number(i_hostSettings.m_uBufferSize) );
            m_pEdtBufferSize_bytes->setEnabled(true);
            m_pEdtBufferSize_bytes->show();
            m_pLblBufferSize_bytes->show();
        }
        else
        {
            m_pEdtBufferSize_bytes->setValidator(nullptr);
            m_pEdtBufferSize_bytes->setText("");
            m_pEdtBufferSize_bytes->setEnabled(false);
            m_pEdtBufferSize_bytes->hide();
            m_pLblBufferSize_bytes->hide();
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtBufferSize_bytes,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtBufferSizeTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblBufferSize_bytes != nullptr && m_pEdtBufferSize_bytes != nullptr )

    // <Button> Ok
    //------------

    if( m_pBtnOk != nullptr )
    {
        m_pBtnOk->setEnabled(true);

    } // if( m_pBtnOk != nullptr )

    // <Button> Cancel
    //----------------

    if( m_pBtnCancel != nullptr )
    {
        m_pBtnCancel->setEnabled(true);

    } // if( m_pBtnCancel != nullptr )

    // <Button> Reset
    //---------------

    if( m_pBtnReset != nullptr )
    {
        m_pBtnReset->setEnabled(false);

    } // if( m_pBtnReset != nullptr )

    // <Button> Apply
    //---------------

    if( m_pBtnApply != nullptr )
    {
        m_pBtnApply->setEnabled(false);

    } // if( m_pBtnApply != nullptr )

    // <Button> Connect/Disconnect/Abort
    //-----------------------------------

    if( m_pBtnStartup != nullptr )
    {
        if( pReqInProgress != nullptr )
        {
            m_pBtnStartup->setText(c_strBtnAbort);
        }
        else if( bServerListening )
        {
            m_pBtnStartup->setText(c_strBtnShutdown);
        }
        else
        {
            m_pBtnStartup->setText(c_strBtnStartup);
        }
        m_pBtnStartup->setEnabled(true);

    } // if( m_pBtnStartup != nullptr )

    // Update internal cache
    //----------------------

    m_hostSettingsWidget = i_hostSettings;

} // fillCnctControls

//------------------------------------------------------------------------------
void CWdgtIpcServer::resetDetailControls()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "resetDetailControls",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pEdtDetailsConnectionSettingsApplied != nullptr )
    {
        m_pEdtDetailsConnectionSettingsApplied->setStyleSheet("");
        m_pEdtDetailsConnectionSettingsApplied->setText("");
        m_pEdtDetailsConnectionSettingsApplied->setToolTip("");

    } // if( m_pEdtDetailsConnectionSettingsApplied != nullptr )

    if( m_pEdtDetailsState != nullptr )
    {
        m_pEdtDetailsState->setStyleSheet("");
        m_pEdtDetailsState->setText("");

    } // if( m_pEdtDetailsState != nullptr )

    if( m_pBarRequestInProgress != nullptr )
    {
         m_pBarRequestInProgress->setLabelText("");
         m_pBarRequestInProgress->stop();
         m_pBarRequestInProgress->reset();

    } // if( m_pBarRequestInProgress != nullptr )

    if( m_pModelSrvCltConnection != nullptr )
    {
        m_pModelSrvCltConnection->setServer(nullptr);
    }

} // resetDetailControls

//------------------------------------------------------------------------------
void CWdgtIpcServer::fillDetailControls()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "fillDetailControls",
        /* strAddInfo   */ strAddTrcInfo );

    bool    bServerListening = false;
    QString strState;
    QString strConnectionToolTip = getConnectionToolTip();
    QString strConnection;

    if( m_pServer != nullptr )
    {
        bServerListening = m_pServer->isListening();
        strState = m_pServer->state2Str();
        strConnection = m_pServer->getHostSettings().getConnectionString();
    }

    if( m_pEdtDetailsConnectionSettingsApplied != nullptr )
    {
        if( m_hostSettingsWidget.m_socketType == ESocketTypeInProcMsg )
        {
            m_pLblDetailsConnectionSettingsApplied->hide();
            m_pEdtDetailsConnectionSettingsApplied->hide();
        }
        else // if( m_hostSettingsWidget.m_socketType != ESocketTypeInProcMsg )
        {
            if( m_pBtnApply != nullptr && m_pBtnApply->isEnabled() )
            {
                m_pEdtDetailsConnectionSettingsApplied->setStyleSheet("QLineEdit { color: grey; }");
            }
            else if( bServerListening )
            {
                m_pEdtDetailsConnectionSettingsApplied->setStyleSheet("QLineEdit { color: green; }");
            }
            else
            {
                m_pEdtDetailsConnectionSettingsApplied->setStyleSheet("QLineEdit { color: red; }");
            }
            m_pEdtDetailsConnectionSettingsApplied->setText(strConnection);
            m_pEdtDetailsConnectionSettingsApplied->setToolTip(strConnection);

        } // if( m_hostSettingsWidget.m_socketType != ESocketTypeInProcMsg )
    } // if( m_pEdtDetailsConnectionSettingsApplied != nullptr )

    if( m_pEdtDetailsState != nullptr )
    {
        if( bServerListening )
        {
            m_pEdtDetailsState->setStyleSheet("QLineEdit { color: green; }");
            m_pEdtDetailsState->setText(strState);
        }
        else
        {
            m_pEdtDetailsState->setStyleSheet("QLineEdit { color: red; }");
            m_pEdtDetailsState->setText(strState);
        }
    } // if( m_pEdtDetailsState != nullptr )

} // fillDetailControls

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcServer::applySettings()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "applySettings",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pServer != nullptr )
    {
        bool bHasChanges = false;

        if( m_hostSettingsWidget != m_hostSettingsServer )
        {
            bHasChanges = true;
            m_pServer->setHostSettings(m_hostSettingsWidget);
        }

        // In order for the changed settings to take affect:
        if( bHasChanges )
        {
            m_pServer->changeSettings();
        }
    } // if( m_pServer != nullptr )

} // applySettings
