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
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSIpcGUI/ZSIpcClientWdgt.h"
#include "ZSIpcGUI/ZSIpcTrcMsgLogWdgt.h"
#include "ZSIpc/ZSIpcClient.h"
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
class CWdgtIpcClient : public QWidget
*******************************************************************************/

static const QString c_strBtnConnect    = "Connect";
static const QString c_strBtnDisconnect = "Disconnect";
static const QString c_strBtnAbort      = "Abort";

static const QString c_strBtnDetailsShowDetails = "Details >>";
static const QString c_strBtnDetailsHideDetails = "<< No Details";

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtIpcClient::CWdgtIpcClient( const QString& i_strObjName, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pClient(nullptr),
    m_bClientObjNameVisible(true),
    m_bProtocolTypeImageVisible(true),
    m_iLblWidth(120),
    m_pLyt(nullptr),
    m_pWdgtClientObjName(nullptr),
    m_pLytWdgtClientObjName(nullptr),
    m_pLytLineClientObjName(nullptr),
    m_pLblClientObjName(nullptr),
    m_pLblClientProtocolTypeImg(nullptr),
    m_pLblClientState(nullptr),
    m_pLedClientState(nullptr),
    // Connection Settings
    m_hostSettingsClient(),
    m_hostSettingsWidget(),
    m_pLytLineDefault(nullptr),
    m_pLytCnct(nullptr),
    m_pLblSocketType(nullptr),
    m_pCmbSocketType(nullptr),
    m_pLblRemoteHostName(nullptr),
    m_pEdtRemoteHostName(nullptr),
    m_pLblRemoteHostAddr(nullptr),
    m_pEdtRemoteHostAddr(nullptr),
    m_bRemoteHostInfoLookUpInProcess(false),
    m_pLblRemotePort(nullptr),
    m_pEdtRemotePort(nullptr),
    m_pLblConnectTimeout_ms(nullptr),
    m_pEdtConnectTimeout_ms(nullptr),
    m_pLblBufferSize_bytes(nullptr),
    m_pEdtBufferSize_bytes(nullptr),
    // Watch Dog Timer
    m_bWatchDogSettingsUsed(true),
    m_watchDogSettingsClient(),
    m_watchDogSettingsWidget(),
    m_pLblWatchDogTimer(nullptr),
    m_pLytWatchDogTimer(nullptr),
    m_pLblWatchDogTimerEnabled(nullptr),
    m_pChkWatchDogTimerEnabled(nullptr),
    m_pLblWatchDogTimerInterval_ms(nullptr),
    m_pEdtWatchDogTimerInterval_ms(nullptr),
    m_pLblWatchDogTimeout_ms(nullptr),
    m_pEdtWatchDogTimeout_ms(nullptr),
    // Buttons
    m_pLytBtns(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnCancel(nullptr),
    m_pBtnReset(nullptr),
    m_pBtnApply(nullptr),
    m_pBtnConnect(nullptr),
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
    // Active Connection
    m_pLblLocalHostName(nullptr),
    m_pEdtLocalHostName(nullptr),
    m_pLblLocalPort(nullptr),
    m_pEdtLocalPort(nullptr),
    m_pLblSocketId(nullptr),
    m_pEdtSocketId(nullptr),
    m_idxLytItemStretchAtBottom(-1),
    // Trace
    m_pWdgtMsgLog(nullptr),
    m_pLytMsgLog(nullptr),
    m_pLblMsgLog(nullptr),
    m_trcMsgLogSettings(),
    m_pWdgtTrcMsgLog(nullptr),
    m_pTrcAdminObj(nullptr)
{
    if( i_strObjName.isEmpty() )
    {
        setObjectName("IpcClient");
    }
    else
    {
        setObjectName(i_strObjName);
    }

    // The trace client may not trace itself. This will lead to endless recursions.
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

    // <Line> Client Object Name
    //--------------------------

    m_pWdgtClientObjName = new QWidget();
    m_pLytWdgtClientObjName = new QVBoxLayout();
    m_pLytWdgtClientObjName->setContentsMargins(0, 0, 0, 0);
    m_pWdgtClientObjName->setLayout(m_pLytWdgtClientObjName);
    m_pLyt->addWidget(m_pWdgtClientObjName);

    m_pLytLineClientObjName = new QHBoxLayout();
    m_pLytWdgtClientObjName->addLayout(m_pLytLineClientObjName);

    m_pLblClientObjName = new QLabel("Client Connection Settings");
    m_pLblClientObjName->setStyleSheet("font-weight: bold;");
    m_pLytLineClientObjName->addWidget(m_pLblClientObjName, 1);

    // <Image> ProtocolType
    //---------------------

    m_pLytLineClientObjName->addStretch();

    QPixmap pxmProtocolTypeImg(":/ZS/App/Zeus32x32.bmp");

    pxmProtocolTypeImg.setMask(pxmProtocolTypeImg.createHeuristicMask());

    m_pLblClientProtocolTypeImg = new QLabel();
    m_pLblClientProtocolTypeImg->setMaximumWidth(32);
    m_pLblClientProtocolTypeImg->setMaximumHeight(32);
    m_pLblClientProtocolTypeImg->setScaledContents(true);
    m_pLblClientProtocolTypeImg->setPixmap(pxmProtocolTypeImg);
    m_pLytLineClientObjName->addWidget(m_pLblClientProtocolTypeImg);

    m_pLytLineClientObjName->addSpacing(10);

    // <Led> State
    //------------

    m_pLblClientState = new QLabel("State:");
    m_pLytLineClientObjName->addWidget(m_pLblClientState);
    m_pLedClientState = new QLabel();
    m_pLedClientState->setMaximumWidth(20);
    m_pLedClientState->setMaximumHeight(20);
    m_pLedClientState->setScaledContents(true);
    m_pLedClientState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
    m_pLytLineClientObjName->addWidget(m_pLedClientState);

    // <SepLine> at bottom of Client Object Name
    //------------------------------------------

    m_pLytWdgtClientObjName->addSpacing(5);
    pLine = new CSepLine();
    m_pLytWdgtClientObjName->addWidget(pLine);
    m_pLytWdgtClientObjName->addSpacing(5);

    // <Line> Default
    //---------------

    m_pLytLineDefault = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytLineDefault);

    m_pLytLineDefault->addStretch();

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new CSepLine();
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

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

    // <LineEdit> Remote Host Name
    //-----------------------------

    m_pLblRemoteHostName = new QLabel("Remote Host:");
    m_pLblRemoteHostName->setFixedWidth(m_iLblWidth);
    m_pEdtRemoteHostName = new QLineEdit();
    m_pEdtRemoteHostName->setEnabled(false);
    m_pLytCnct->addRow( m_pLblRemoteHostName, m_pEdtRemoteHostName );

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtRemoteHostName,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtRemoteHostNameTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Remote Host IP Address
    //----------------------------------

    m_pLblRemoteHostAddr = new QLabel("IP Address:");
    m_pEdtRemoteHostAddr = new QLineEdit();
    m_pEdtRemoteHostAddr->setEnabled(false);
    m_pLytCnct->addRow( m_pLblRemoteHostAddr, m_pEdtRemoteHostAddr );

    // <LineEdit> Remote Host Port
    //----------------------------

    m_pLblRemotePort = new QLabel("Remote Port:");
    m_pLblRemotePort->setFixedWidth(m_iLblWidth);
    m_pEdtRemotePort = new QLineEdit();
    m_pEdtRemotePort->setEnabled(false);
    m_pLytCnct->addRow( m_pLblRemotePort, m_pEdtRemotePort );

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtRemotePort,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtRemotePortTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Connect Timeout
    //-----------------------------

    m_pLblConnectTimeout_ms = new QLabel("Connect Timeout / ms:");
    m_pLblConnectTimeout_ms->setFixedWidth(m_iLblWidth);
    m_pEdtConnectTimeout_ms = new QLineEdit();
    m_pEdtConnectTimeout_ms->setEnabled(false);
    m_pLytCnct->addRow( m_pLblConnectTimeout_ms, m_pEdtConnectTimeout_ms );

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtConnectTimeout_ms,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtConnectTimeoutTextChanged(const QString&)) ) )
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

    // <Line> Watch Dog Timer
    //-----------------------

    m_pLblWatchDogTimer = new QLabel( "Connection Watch Dog:" );
    m_pLyt->addWidget(m_pLblWatchDogTimer);

    m_pLyt->addSpacing(5);

    m_pLytWatchDogTimer = new QFormLayout();
    m_pLyt->addLayout(m_pLytWatchDogTimer);

    // <CheckBox> Watch Dog Timer Enabled
    //-----------------------------------

    m_pLblWatchDogTimerEnabled = new QLabel("Enabled:");
    m_pLblWatchDogTimerEnabled->setFixedWidth(m_iLblWidth);
    m_pChkWatchDogTimerEnabled = new QCheckBox();
    m_pChkWatchDogTimerEnabled->setEnabled(false);
    m_pLytWatchDogTimer->addRow( m_pLblWatchDogTimerEnabled, m_pChkWatchDogTimerEnabled );

    if( !QObject::connect(
        /* pObjSender   */ m_pChkWatchDogTimerEnabled,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkWatchDogTimerEnabledToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Watch Dog Timer Interval
    //------------------------------------

    m_pLblWatchDogTimerInterval_ms = new QLabel("Interval / ms:");
    m_pLblWatchDogTimerInterval_ms->setFixedWidth(m_iLblWidth);
    m_pEdtWatchDogTimerInterval_ms = new QLineEdit();
    m_pEdtWatchDogTimerInterval_ms->setEnabled(false);
    m_pLytWatchDogTimer->addRow( m_pLblWatchDogTimerInterval_ms, m_pEdtWatchDogTimerInterval_ms );

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtWatchDogTimerInterval_ms,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtWatchDogTimerIntervalTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Watch Dog Timeout
    //-----------------------------

    m_pLblWatchDogTimeout_ms = new QLabel("Timeout / ms:");
    m_pLblWatchDogTimeout_ms->setFixedWidth(m_iLblWidth);
    m_pEdtWatchDogTimeout_ms = new QLineEdit();
    m_pEdtWatchDogTimeout_ms->setEnabled(false);
    m_pLytWatchDogTimer->addRow( m_pLblWatchDogTimeout_ms, m_pEdtWatchDogTimeout_ms );

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtWatchDogTimeout_ms,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtWatchDogTimeoutTextChanged(const QString&)) ) )
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

    // <Button> Connect/Disconnect/Abort
    //----------------------------------

    m_pBtnConnect = new QPushButton(c_strBtnConnect);
    m_pBtnConnect->setFixedWidth(cxBtnWidth);
    m_pBtnConnect->setEnabled(false);
    m_pLytBtns->addWidget(m_pBtnConnect);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnConnect,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnConnectClicked(bool)) ) )
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
    m_pBtnDetails->setFixedWidth(120);
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
CWdgtIpcClient::~CWdgtIpcClient()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ strAddTrcInfo );

    saveSettings();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pClient = nullptr;
    m_bClientObjNameVisible = false;
    m_bProtocolTypeImageVisible = false;
    m_iLblWidth = 0;
    m_pLyt = nullptr;
    m_pWdgtClientObjName = nullptr;
    m_pLytWdgtClientObjName = nullptr;
    m_pLytLineClientObjName = nullptr;
    m_pLblClientProtocolTypeImg = nullptr;
    m_pLblClientObjName = nullptr;
    m_pLblClientState = nullptr;
    m_pLedClientState = nullptr;
    // Connection Settings
    //m_hostSettingsClient;
    //m_hostSettingsClientDefault;
    //m_hostSettingsWidget;
    m_pLytLineDefault = nullptr;
    m_pLytCnct = nullptr;
    m_pLblSocketType = nullptr;
    m_pCmbSocketType = nullptr;
    m_pLblRemoteHostName = nullptr;
    m_pEdtRemoteHostName = nullptr;
    m_pLblRemoteHostAddr = nullptr;
    m_pEdtRemoteHostAddr = nullptr;
    m_bRemoteHostInfoLookUpInProcess = false;
    m_pLblRemotePort = nullptr;
    m_pEdtRemotePort = nullptr;
    m_pLblConnectTimeout_ms = nullptr;
    m_pEdtConnectTimeout_ms = nullptr;
    m_pLblBufferSize_bytes = nullptr;
    m_pEdtBufferSize_bytes = nullptr;
    // Watch Dog Timer (STimerSettings)
    m_bWatchDogSettingsUsed = false;
    //m_watchDogSettingsClient;
    //m_watchDogSettingsClientDefault;
    //m_watchDogSettingsWidget;
    m_pLblWatchDogTimer = nullptr;
    m_pLytWatchDogTimer = nullptr;
    m_pLblWatchDogTimerEnabled = nullptr;
    m_pChkWatchDogTimerEnabled = nullptr;
    m_pLblWatchDogTimerInterval_ms = nullptr;
    m_pEdtWatchDogTimerInterval_ms = nullptr;
    m_pLblWatchDogTimeout_ms = nullptr;
    m_pEdtWatchDogTimeout_ms = nullptr;
    // Buttons
    m_pLytBtns = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnCancel = nullptr;
    m_pBtnReset = nullptr;
    m_pBtnApply = nullptr;
    m_pBtnConnect = nullptr;
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
    // Active Connection
    m_pLblLocalHostName = nullptr;
    m_pEdtLocalHostName = nullptr;
    m_pLblLocalPort = nullptr;
    m_pEdtLocalPort = nullptr;
    m_pLblSocketId = nullptr;
    m_pEdtSocketId = nullptr;
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
void CWdgtIpcClient::setClient( CClient* i_pClient )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Client: " + QString( i_pClient == nullptr ? "nullptr" : i_pClient->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setClient",
        /* strAddInfo   */ strAddTrcInfo );

    // Show/hide GUI controls depending on active connection (depending on protocol type),
    // show/hide details, fill connection controls (connection parameters and watch dog settings)
    // and fill detail controls:

    if( m_pClient != i_pClient )
    {
        if( m_pClient != nullptr )
        {
            // <Signals/Slots> of connection
            //------------------------------

            QObject::disconnect(
                /* pObjSender   */ m_pClient,
                /* szSignal     */ SIGNAL(stateChanged(QObject*,int)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onClientStateChanged(QObject*,int)) );

            QObject::disconnect(
                /* pObjSender   */ m_pClient,
                /* szSignal     */ SIGNAL(settingsChanged(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onClientSettingsChanged(QObject*)) );

            QObject::disconnect(
                /* pObjSender   */ m_pClient,
                /* szSignal     */ SIGNAL(requestInProgressChanged(QObject*, ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onClientRequestInProgressChanged(QObject*, ZS::System::SRequestDscr)) );

            resetCnctControls();

            if( m_bShowDetails )
            {
                resetDetailControls();
            }
        } // if( m_pClient != nullptr )

        m_pClient = i_pClient;

        m_hostSettingsClient = SClientHostSettings();
        m_hostSettingsWidget = SClientHostSettings();

        m_watchDogSettingsClient = STimerSettings();
        m_watchDogSettingsWidget = STimerSettings();

        if( m_pClient != nullptr )
        {
            // <Signals/Slots> of connection
            //------------------------------

            if( !QObject::connect(
                /* pObjSender   */ m_pClient,
                /* szSignal     */ SIGNAL(stateChanged(QObject*,int)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onClientStateChanged(QObject*,int)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pClient,
                /* szSignal     */ SIGNAL(settingsChanged(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onClientSettingsChanged(QObject*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pClient,
                /* szSignal     */ SIGNAL(requestInProgressChanged(QObject*, ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onClientRequestInProgressChanged(QObject*, ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            // Read current settings
            //----------------------

            m_hostSettingsClient     = m_pClient->getHostSettings();
            m_bWatchDogSettingsUsed  = m_pClient->isWatchDogTimerUsed();
            m_watchDogSettingsClient = m_pClient->getWatchDogSettings();

            fillCnctControls(m_hostSettingsClient, m_watchDogSettingsClient);

            if( m_bShowDetails )
            {
                fillDetailControls();
            }
        } // if( m_pClient != nullptr )
    } // if( m_pClient != i_pClient )

} // setClient

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcClient::setClientObjectNameVisible( bool i_bVisible )
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
        /* strMethod    */ "setClientObjectNameVisible",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_bClientObjNameVisible != i_bVisible )
    {
        m_bClientObjNameVisible = i_bVisible;

        if( m_bClientObjNameVisible )
        {
            m_pLytLineDefault->removeWidget(m_pLblClientState);
            m_pLytLineDefault->removeWidget(m_pLedClientState);

            m_pLytLineClientObjName->addWidget(m_pLblClientState);
            m_pLytLineClientObjName->addWidget(m_pLedClientState);

            m_pWdgtClientObjName->show();

            if( m_bProtocolTypeImageVisible )
            {
                m_pLytLineDefault->removeWidget(m_pLblClientProtocolTypeImg);
                int idxLblClientState = m_pLytLineClientObjName->indexOf(m_pLblClientState);
                m_pLytLineClientObjName->insertWidget(idxLblClientState, m_pLblClientProtocolTypeImg);
            }
        }
        else // if( !m_bClientObjNameVisible )
        {
            m_pLytLineClientObjName->removeWidget(m_pLblClientState);
            m_pLytLineClientObjName->removeWidget(m_pLedClientState);

            m_pWdgtClientObjName->hide();

            m_pLytLineDefault->addWidget(m_pLblClientState);
            m_pLytLineDefault->addWidget(m_pLedClientState);

            if( m_bProtocolTypeImageVisible )
            {
                m_pLytLineClientObjName->removeWidget(m_pLblClientProtocolTypeImg);
                int idxLblClientState = m_pLytLineDefault->indexOf(m_pLblClientState);
                m_pLytLineDefault->insertWidget(idxLblClientState, m_pLblClientProtocolTypeImg);
            }
        }
    } // if( m_bClientObjNameVisible != i_bVisible )

} // setClientObjectNameVisible

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcClient::setProtocolTypeImage( const QPixmap& i_pxm )
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

    if( m_pLblClientProtocolTypeImg != nullptr )
    {
        QPixmap pxmProtocolTypeImg(i_pxm);
        pxmProtocolTypeImg.setMask(pxmProtocolTypeImg.createHeuristicMask());
        m_pLblClientProtocolTypeImg->setPixmap(pxmProtocolTypeImg);
    }

} // setProtocolTypeImage

//------------------------------------------------------------------------------
void CWdgtIpcClient::setProtocolTypeImageVisible( bool i_bVisible )
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
            if( m_bClientObjNameVisible )
            {
                int idxLblClientState = m_pLytLineClientObjName->indexOf(m_pLblClientState);
                m_pLytLineClientObjName->insertWidget(idxLblClientState, m_pLblClientProtocolTypeImg);
            }
            else
            {
                int idxLblClientState = m_pLytLineDefault->indexOf(m_pLblClientState);
                m_pLytLineDefault->insertWidget(idxLblClientState, m_pLblClientProtocolTypeImg);
            }
        }
        else // if( !m_bProtocolTypeImageVisible )
        {
            if( m_bClientObjNameVisible )
            {
                m_pLytLineClientObjName->removeWidget(m_pLblClientProtocolTypeImg);
            }
            else
            {
                m_pLytLineDefault->removeWidget(m_pLblClientProtocolTypeImg);
            }
        }
    } // if( m_bProtocolTypeImageVisible != i_bVisible )

} // setProtocolTypeImageVisible

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcClient::readSettings()
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
void CWdgtIpcClient::saveSettings()
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
void CWdgtIpcClient::onCmbSocketTypeCurrentIndexChanged( int i_iIdx )
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

    fillCnctControls(m_hostSettingsWidget, m_watchDogSettingsWidget);

    CRequest* pReqInProgress = m_pClient->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget != m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
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
void CWdgtIpcClient::onEdtRemoteHostNameTextChanged( const QString& i_strText )
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
        /* strMethod    */ "onEdtRemoteHostNameTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_hostSettingsWidget.m_strRemoteHostName = i_strText;

    CRequest* pReqInProgress = m_pClient->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

} // onEdtRemoteHostNameTextChanged

//------------------------------------------------------------------------------
void CWdgtIpcClient::onEdtRemotePortTextChanged( const QString& i_strText )
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
        /* strMethod    */ "onEdtRemotePortTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    bool    bConverted;
    quint32 uVal = i_strText.toUInt(&bConverted);

    if( bConverted )
    {
        m_hostSettingsWidget.m_uRemotePort = uVal;
    }

    CRequest* pReqInProgress = m_pClient->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

} // onEdtRemotePortTextChanged

//------------------------------------------------------------------------------
void CWdgtIpcClient::onEdtConnectTimeoutTextChanged( const QString& i_strText )
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
        /* strMethod    */ "onEdtConnectTimeoutTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    bool   bConverted;
    qint32 iVal = i_strText.toInt(&bConverted);

    if( bConverted )
    {
        m_hostSettingsWidget.m_iConnectTimeout_ms = iVal;
    }

    CRequest* pReqInProgress = m_pClient->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

} // onEdtConnectTimeoutTextChanged

//------------------------------------------------------------------------------
void CWdgtIpcClient::onEdtBufferSizeTextChanged( const QString& i_strText )
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

    CRequest* pReqInProgress = m_pClient->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
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
void CWdgtIpcClient::onChkWatchDogTimerEnabledToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onChkWatchDogTimerEnabledToggled",
        /* strAddInfo   */ strAddTrcInfo );

    m_watchDogSettingsWidget.m_bEnabled = i_bChecked;

    CRequest* pReqInProgress = m_pClient->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

} // onChkWatchDogTimerEnabledToggled

//------------------------------------------------------------------------------
void CWdgtIpcClient::onEdtWatchDogTimerIntervalTextChanged( const QString& i_strText )
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
        /* strMethod    */ "onEdtWatchDogTimerIntervalTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    bool   bConverted;
    qint32 iVal = i_strText.toInt(&bConverted);

    if( bConverted )
    {
        m_watchDogSettingsWidget.m_iInterval_ms = iVal;
    }

    CRequest* pReqInProgress = m_pClient->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

} // onEdtWatchDogTimerIntervalTextChanged

//------------------------------------------------------------------------------
void CWdgtIpcClient::onEdtWatchDogTimeoutTextChanged( const QString& i_strText )
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
        /* strMethod    */ "onEdtWatchDogTimeoutTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    bool   bConverted;
    qint32 iVal = i_strText.toInt(&bConverted);

    if( bConverted )
    {
        m_watchDogSettingsWidget.m_iTimeout_ms = iVal;
    }

    CRequest* pReqInProgress = m_pClient->getRequestInProgress();

    if( m_pBtnReset != nullptr )
    {
        if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
        if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

} // onEdtWatchDogTimeoutTextChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcClient::onBtnOkClicked( bool /*i_bChecked*/ )
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

    if( m_hostSettingsClient != m_hostSettingsWidget )
    {
        applySettings();
    }

    emit accepted();

} // onBtnOkClicked

//------------------------------------------------------------------------------
void CWdgtIpcClient::onBtnCancelClicked( bool /*i_bChecked*/ )
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

    if( m_pClient != nullptr )
    {
        m_hostSettingsClient     = m_pClient->getHostSettings();
        m_watchDogSettingsClient = m_pClient->getWatchDogSettings();

        fillCnctControls(m_hostSettingsClient, m_watchDogSettingsClient);

        CRequest* pReqInProgress = m_pClient->getRequestInProgress();

        if( m_pBtnReset != nullptr )
        {
            if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
            if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
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

    } // if( m_pClient != nullptr )

    emit rejected();

} // onBtnCancelClicked

//------------------------------------------------------------------------------
void CWdgtIpcClient::onBtnResetClicked( bool /*i_bChecked*/ )
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

    if( m_pClient != nullptr )
    {
        m_hostSettingsClient     = m_pClient->getHostSettings();
        m_watchDogSettingsClient = m_pClient->getWatchDogSettings();

        fillCnctControls(m_hostSettingsClient, m_watchDogSettingsClient);

        CRequest* pReqInProgress = m_pClient->getRequestInProgress();

        if( m_pBtnReset != nullptr )
        {
            if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
            if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
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
    } // if( m_pClient != nullptr )

} // onBtnResetClicked

//------------------------------------------------------------------------------
void CWdgtIpcClient::onBtnApplyClicked( bool /*i_bChecked*/ )
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

    if( m_hostSettingsClient != m_hostSettingsWidget || m_watchDogSettingsClient != m_watchDogSettingsWidget )
    {
        applySettings();
    }

} // onBtnApplyClicked

//------------------------------------------------------------------------------
void CWdgtIpcClient::onBtnConnectClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnConnectClicked",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pClient != nullptr )
    {
        CRequest* pReqInProgress = m_pClient->getRequestInProgress();

        if( m_pBtnConnect->text() == c_strBtnConnect )
        {
            if( pReqInProgress == nullptr )
            {
                applySettings();

                m_pClient->connect_();
            }
        }
        else if( m_pBtnConnect->text() == c_strBtnDisconnect )
        {
            if( pReqInProgress == nullptr )
            {
                m_pClient->disconnect_();
            }
        }
        else if( m_pBtnConnect->text() == c_strBtnAbort )
        {
            m_pClient->abortRequestInProgress();
        }
    } // if( m_pClient != nullptr )

} // onBtnConnectClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcClient::onBtnDetailsClicked( bool /*i_bChecked*/ )
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
protected slots: // connected to the signals of the Ipc client
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcClient::onClientStateChanged( QObject* i_pClient, int i_iState )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Client: " + QString( i_pClient == nullptr ? "nullptr" : i_pClient->objectName() );
        strAddTrcInfo += ", State: " + CClient::State2Str(i_iState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onClientStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pClient != nullptr && i_pClient == m_pClient )
    {
        fillCnctControls(m_hostSettingsWidget, m_watchDogSettingsWidget);

        CRequest* pReqInProgress = m_pClient->getRequestInProgress();

        if( m_pBtnReset != nullptr )
        {
            if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
            if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
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

    } // if( i_pClient != nullptr && i_pClient == m_pClient )

} // onClientStateChanged

//------------------------------------------------------------------------------
void CWdgtIpcClient::onClientSettingsChanged( QObject* i_pClient )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Client: " + QString( i_pClient == nullptr ? "nullptr" : i_pClient->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onClientSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pClient != nullptr && i_pClient == m_pClient )
    {
        m_hostSettingsClient     = m_pClient->getHostSettings();
        m_watchDogSettingsClient = m_pClient->getWatchDogSettings();

        fillCnctControls(m_hostSettingsClient, m_watchDogSettingsClient);

        CRequest* pReqInProgress = m_pClient->getRequestInProgress();

        if( m_pBtnReset != nullptr )
        {
            if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
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
            if( pReqInProgress != nullptr || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
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
    } // if( i_pClient != nullptr && i_pClient == m_pClient )

} // onClientSettingsChanged

//------------------------------------------------------------------------------
void CWdgtIpcClient::onClientRequestInProgressChanged( QObject* i_pClient, ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "Client: " + QString( i_pClient == nullptr ? "nullptr" : i_pClient->objectName() );
        strAddTrcInfo += ", Req {" + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onClientRequestInProgressChanged",
        /* strAddInfo   */ strAddTrcInfo );

    QString strBtnConnect;
    QString strReqInProgress;

    bool bIsMainRequest   = false;
    bool bMainReqStarted  = false;
    bool bMainReqFinished = false;
    bool bIsReqInProgress = false;

    switch( i_reqDscr.m_request )
    {
        case CClient::ERequestNone:
        {
            if( m_pClient->isConnected() )
            {
                strBtnConnect = c_strBtnDisconnect;
            }
            else
            {
                strBtnConnect = c_strBtnConnect;
            }
            break;
        }
        case CClient::ERequestConnect:
        {
            bIsMainRequest = true;

            if( i_reqDscr.m_iProgress_perCent < 100 )
            {
                strBtnConnect    = c_strBtnAbort;
                strReqInProgress = "Connecting ...";
                bMainReqStarted  = (i_reqDscr.m_iProgress_perCent == 0);
                bIsReqInProgress = true;
            }
            else if( i_reqDscr.m_iProgress_perCent >= 100 )
            {
                if( m_pClient->isConnected() )
                {
                    strBtnConnect = c_strBtnDisconnect;
                }
                else
                {
                    strBtnConnect = c_strBtnConnect;
                }
                bMainReqFinished = true;
            }
            break;
        }
        case CClient::ERequestDisconnect:
        {
            bIsMainRequest = true;

            if( i_reqDscr.m_iProgress_perCent < 100 )
            {
                strBtnConnect    = c_strBtnAbort;
                strReqInProgress = "Disconnecting ...";
                bMainReqStarted  = (i_reqDscr.m_iProgress_perCent == 0);
                bIsReqInProgress = true;
            }
            else if( i_reqDscr.m_iProgress_perCent >= 100 )
            {
                strBtnConnect = c_strBtnConnect;
                bMainReqFinished = true;
            }
            break;
        }
        case CClient::ERequestChangeSettings:
        {
            bIsMainRequest = true;

            if( i_reqDscr.m_iProgress_perCent < 100 )
            {
                strBtnConnect    = c_strBtnAbort;
                strReqInProgress = "Connecting ...";
                bMainReqStarted  = (i_reqDscr.m_iProgress_perCent == 0);
                bIsReqInProgress = true;
            }
            else if( i_reqDscr.m_iProgress_perCent >= 100 )
            {
                if( m_pClient->isConnected() )
                {
                    strBtnConnect = c_strBtnDisconnect;
                }
                else
                {
                    strBtnConnect = c_strBtnConnect;
                }
                bMainReqFinished = true;
            }
            break;
        }
        case CClient::ERequestSendData:
        {
            bIsMainRequest = true;

            if( i_reqDscr.m_iProgress_perCent < 100 )
            {
                strBtnConnect    = c_strBtnAbort;
                strReqInProgress = "Sending data ...";
                bMainReqStarted  = (i_reqDscr.m_iProgress_perCent == 0);
                bIsReqInProgress = true;
            }
            else if( i_reqDscr.m_iProgress_perCent >= 100 )
            {
                if( m_pClient->isConnected() )
                {
                    strBtnConnect = c_strBtnDisconnect;
                }
                else
                {
                    strBtnConnect = c_strBtnConnect;
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
        if( bIsReqInProgress || (m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient) )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

    if( m_pBtnConnect != nullptr )
    {
        if( bIsMainRequest )
        {
            m_pBtnConnect->setText(strBtnConnect);
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

} // onClientRequestInProgressChanged

/*==============================================================================
protected slots: // host info lookup
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcClient::onRemoteHostInfoLookedUp( const QHostInfo& i_hostInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemoteHostInfoLookedUp",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pEdtRemoteHostAddr != nullptr )
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
        m_pEdtRemoteHostAddr->setText(strHostAddrs);

    } // if( m_pEdtRemoteHostAddr != nullptr )

    m_bRemoteHostInfoLookUpInProcess = false;

} // onRemoteHostInfoLookedUp

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CWdgtIpcClient::getConnectionToolTip()
//------------------------------------------------------------------------------
{
    QString strToolTip;

    if( m_pClient != nullptr )
    {
        strToolTip = m_pClient->getConnectionString();
    }

    return strToolTip;

} // getConnectionToolTip

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcClient::onShowDetailsChanged()
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
            CSepLine* pLine;

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

            // <ProgressBar> Request In Progress
            //-----------------------------------

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

            // <GroupBox> Active Connection
            //=============================

            QFormLayout* pLytDetailsActiveCnct = new QFormLayout();
            m_pLytDetailsStates->addLayout(pLytDetailsActiveCnct);

            // <LineEdit> Local Host Name
            //-----------------------------

            m_pLblLocalHostName = new QLabel("Local Host:");
            m_pLblLocalHostName->setFixedWidth(m_iLblWidth);
            m_pEdtLocalHostName = new QLineEdit();
            m_pEdtLocalHostName->setEnabled(false);
            pLytDetailsActiveCnct->addRow(m_pLblLocalHostName,m_pEdtLocalHostName);

            // <LineEdit> Local Host Port
            //----------------------------

            m_pLblLocalPort = new QLabel("Local Port:");
            m_pLblLocalPort->setFixedWidth(m_iLblWidth);
            m_pEdtLocalPort = new QLineEdit();
            m_pEdtLocalPort->setEnabled(false);
            pLytDetailsActiveCnct->addRow(m_pLblLocalPort,m_pEdtLocalPort);

            // <LineEdit> Socket Id
            //---------------------

            m_pLblSocketId = new QLabel("Socket Id:");
            m_pLblSocketId->setFixedWidth(m_iLblWidth);
            m_pEdtSocketId = new QLineEdit();
            m_pEdtSocketId->setEnabled(false);
            pLytDetailsActiveCnct->addRow(m_pLblSocketId,m_pEdtSocketId);

            // <GroupBox> Message Log
            //=============================

            m_pWdgtMsgLog = new QWidget();
            m_pLytMsgLog = new QVBoxLayout();
            m_pLytMsgLog->setContentsMargins(0,0,0,0);
            m_pWdgtMsgLog->setLayout(m_pLytMsgLog);
            m_pLytDetailsStates->addWidget(m_pWdgtMsgLog);

            pLine = new CSepLine();
            m_pLytMsgLog->addWidget(pLine);

            m_pLytMsgLog->addSpacing(5);

            m_pLblMsgLog = new QLabel( "Message Protocol:" );
            m_pLytMsgLog->addWidget(m_pLblMsgLog);

            m_pWdgtTrcMsgLog = new CWdgtTrcMsgLog(m_trcMsgLogSettings);
            m_pLytMsgLog->addWidget(m_pWdgtTrcMsgLog);
            if( m_pClient != nullptr )
            {
                m_pWdgtTrcMsgLog->setClient(m_pClient);
            }
        } // if( m_pWdgtDetailsStates == nullptr )

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
        m_pLblLocalHostName = nullptr;
        m_pEdtLocalHostName = nullptr;
        m_pLblLocalPort = nullptr;
        m_pEdtLocalPort = nullptr;
        m_pLblSocketId = nullptr;
        m_pEdtSocketId = nullptr;
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
void CWdgtIpcClient::resetCnctControls()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "resetCnctControls",
        /* strAddInfo   */ strAddTrcInfo );

    // <Label> Client Object Name
    //---------------------------

    if( m_pLblClientObjName != nullptr )
    {
        m_pLblClientObjName->setText("Client Connection Settings");
    }

    // <Led> State
    //------------

    if( m_pLedClientState != nullptr )
    {
        m_pLedClientState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
        m_pLedClientState->setEnabled(false);

    } // if( m_pLedClientState != nullptr )

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

    // <LineEdit> Remote Host Name
    //-----------------------------

    if( m_pLblRemoteHostName != nullptr && m_pEdtRemoteHostName != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtRemoteHostName,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRemoteHostNameTextChanged(const QString&)) );

        m_pEdtRemoteHostName->setText("");
        m_pEdtRemoteHostName->setEnabled(false);
        m_pEdtRemoteHostName->show();
        m_pLblRemoteHostName->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtRemoteHostName,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRemoteHostNameTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblRemoteHostName != nullptr && m_pEdtRemoteHostName != nullptr )

    // <LineEdit> Remote Host IP Address
    //----------------------------------

    if( m_pLblRemoteHostAddr != nullptr && m_pEdtRemoteHostAddr != nullptr )
    {
        m_pEdtRemoteHostAddr->setText("");
        m_pEdtRemoteHostAddr->show();
        m_pLblRemoteHostAddr->show();

    } // if( m_pLblRemoteHostAddr != nullptr && m_pEdtRemoteHostAddr != nullptr )

    // <LineEdit> Remote Host Port
    //----------------------------

    if( m_pLblRemotePort != nullptr && m_pEdtRemotePort != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtRemotePort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRemotePortTextChanged(const QString&)) );

        m_pEdtRemotePort->setValidator(nullptr);
        m_pEdtRemotePort->setText("");
        m_pEdtRemotePort->setEnabled(false);
        m_pEdtRemotePort->show();
        m_pLblRemotePort->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtRemotePort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRemotePortTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblRemotePort != nullptr && m_pEdtRemotePort != nullptr )

    // <LineEdit> Connect Timeout
    //-----------------------------

    if( m_pLblConnectTimeout_ms != nullptr && m_pEdtConnectTimeout_ms != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtConnectTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtConnectTimeoutTextChanged(const QString&)) );

        m_pEdtConnectTimeout_ms->setValidator(nullptr);
        m_pEdtConnectTimeout_ms->setText("");
        m_pEdtConnectTimeout_ms->setEnabled(false);
        m_pEdtConnectTimeout_ms->show();
        m_pLblConnectTimeout_ms->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtConnectTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtConnectTimeoutTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblConnectTimeout_ms != nullptr && m_pEdtConnectTimeout_ms != nullptr )

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

    // <CheckBox> Watch Dog Timer Enabled
    //-----------------------------------

    if( m_pLblWatchDogTimerEnabled != nullptr && m_pChkWatchDogTimerEnabled != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pChkWatchDogTimerEnabled,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkWatchDogTimerEnabledToggled(bool)) );

        m_pChkWatchDogTimerEnabled->setChecked(false);
        m_pChkWatchDogTimerEnabled->setEnabled(false);
        m_pChkWatchDogTimerEnabled->show();
        m_pLblWatchDogTimerEnabled->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pChkWatchDogTimerEnabled,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkWatchDogTimerEnabledToggled(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblWatchDogTimerEnabled != nullptr && m_pEdtBufferSize_bytes != nullptr )

    // <LineEdit> Watch Dog Timer Interval
    //------------------------------------

    if( m_pLblWatchDogTimerInterval_ms != nullptr && m_pEdtWatchDogTimerInterval_ms != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtWatchDogTimerInterval_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimerIntervalTextChanged(const QString&)) );

        m_pEdtWatchDogTimerInterval_ms->setValidator(nullptr);
        m_pEdtWatchDogTimerInterval_ms->setText("");
        m_pEdtWatchDogTimerInterval_ms->setEnabled(false);
        m_pEdtWatchDogTimerInterval_ms->show();
        m_pLblWatchDogTimerInterval_ms->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtWatchDogTimerInterval_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimerIntervalTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblWatchDogTimerInterval_ms != nullptr && m_pEdtBufferSize_bytes != nullptr )

    // <LineEdit> Watch Dog Timeout
    //-----------------------------

    if( m_pLblWatchDogTimeout_ms != nullptr && m_pEdtWatchDogTimeout_ms != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtWatchDogTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimeoutTextChanged(const QString&)) );

        m_pEdtWatchDogTimeout_ms->setValidator(nullptr);
        m_pEdtWatchDogTimeout_ms->setText("");
        m_pEdtWatchDogTimeout_ms->setEnabled(false);
        m_pEdtWatchDogTimeout_ms->show();
        m_pLblWatchDogTimeout_ms->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtWatchDogTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimeoutTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblWatchDogTimeout_ms != nullptr && m_pEdtBufferSize_bytes != nullptr )

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

    if( m_pBtnConnect != nullptr )
    {
        m_pBtnConnect->setText(c_strBtnConnect);
        m_pBtnConnect->setEnabled(false);
    }

    // Update internal cache
    //----------------------

    m_hostSettingsWidget     = SClientHostSettings();
    m_watchDogSettingsWidget = STimerSettings();

} // resetCnctControls

//------------------------------------------------------------------------------
void CWdgtIpcClient::fillCnctControls(
    const SClientHostSettings& i_hostSettings,
    const STimerSettings&      i_watchDogSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "HostSettings {" + i_hostSettings.getConnectionString(iAddTrcInfoDetailLevel) + "}";
        strAddTrcInfo += ", WatchDogSettings {";
        strAddTrcInfo += "Enabled: " + bool2Str(i_watchDogSettings.m_bEnabled);
        strAddTrcInfo += ", Interval: " + QString::number(i_watchDogSettings.m_iInterval_ms) + " ms";
        strAddTrcInfo += ", Timeout: " + QString::number(i_watchDogSettings.m_iTimeout_ms) + " ms";
        strAddTrcInfo += "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "fillCnctControls",
        /* strAddInfo   */ strAddTrcInfo );

    CRequest* pReqInProgress = nullptr;
    bool      bClientConnected = false;
    QString   strConnection;
    int       idxCmb;

    if( m_pClient != nullptr )
    {
        pReqInProgress   = m_pClient->getRequestInProgress();
        bClientConnected = m_pClient->isConnected();
        strConnection    = m_pClient->getConnectionString();
    }

    // <Label> Client Object Name
    //---------------------------

    if( m_pLblClientObjName != nullptr )
    {
        m_pLblClientObjName->setText( QString( m_pClient == nullptr ? "Client Connection Settings" : m_pClient->objectName() ) );
    }

    // <LED> State
    //------------

    if( m_pLedClientState != nullptr )
    {
        if( bClientConnected )
        {
            m_pLedClientState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
        }
        else // if( !bDbClientConnected )
        {
            m_pLedClientState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
        }
        m_pLedClientState->setEnabled(true);

    } // if( m_pLedClientState != nullptr )

    // <ComboBox> Socket Type
    //-----------------------

    if( m_pLblSocketType != nullptr && m_pCmbSocketType != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pCmbSocketType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) );

        for( idxCmb = 0; idxCmb < m_pClient->getSocketTypesCount(); idxCmb++ )
        {
            ESocketType socketType = m_pClient->getSocketType(idxCmb);
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

        if( m_pClient->isSocketTypeChangeable() )
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

    if( m_pLblRemoteHostName != nullptr && m_pEdtRemoteHostName != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtRemoteHostName,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRemoteHostNameTextChanged(const QString&)) );

        if( i_hostSettings.m_socketType == Ipc::ESocketTypeInProcMsg )
        {
            m_pEdtRemoteHostName->setText("");
            m_pEdtRemoteHostName->setEnabled(false);
            m_pEdtRemoteHostName->hide();
            m_pLblRemoteHostName->hide();
        }
        else
        {
            m_pEdtRemoteHostName->setText(i_hostSettings.m_strRemoteHostName);
            m_pEdtRemoteHostName->setEnabled(true);
            m_pEdtRemoteHostName->show();
            m_pLblRemoteHostName->show();
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtRemoteHostName,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRemoteHostNameTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblRemoteHostName != nullptr && m_pEdtRemoteHostName != nullptr )

    // <LineEdit> Remote Host IP Address
    //----------------------------------

    if( m_pLblRemoteHostAddr != nullptr && m_pEdtRemoteHostAddr != nullptr )
    {
        if( i_hostSettings.m_socketType == ESocketTypeTcp )
        {
            if( !m_bRemoteHostInfoLookUpInProcess )
            {
                m_pEdtRemoteHostAddr->setText("---");
                m_bRemoteHostInfoLookUpInProcess = true;
                QHostInfo::lookupHost( i_hostSettings.m_strRemoteHostName, this, SLOT(onRemoteHostInfoLookedUp(const QHostInfo&)) );
            }
            m_pEdtRemoteHostAddr->show();
            m_pLblRemoteHostAddr->show();
        }
        else
        {
            m_pEdtRemoteHostAddr->hide();
            m_pLblRemoteHostAddr->hide();
        }
    } // if( m_pLblRemoteHostAddr != nullptr && m_pEdtRemoteHostAddr != nullptr )

    // <LineEdit> Remote Port
    //-----------------------

    if( m_pLblRemotePort != nullptr && m_pEdtRemotePort != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtRemotePort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRemotePortTextChanged(const QString&)) );

        if( i_hostSettings.m_socketType == ESocketTypeInProcMsg )
        {
            m_pEdtRemotePort->setValidator(nullptr);
            m_pEdtRemotePort->setText("");
            m_pEdtRemotePort->setEnabled(false);
            m_pEdtRemotePort->hide();
            m_pLblRemotePort->hide();
        }
        else
        {
            m_pEdtRemotePort->setValidator( new QIntValidator(1,65535,this) );
            m_pEdtRemotePort->setText( QString::number(i_hostSettings.m_uRemotePort) );
            m_pEdtRemotePort->setEnabled(true);
            m_pEdtRemotePort->show();
            m_pLblRemotePort->show();
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtRemotePort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRemotePortTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblRemotePort != nullptr && m_pEdtRemotePort != nullptr )

    // <LineEdit> Connect Timeout
    //-----------------------------

    if( m_pLblConnectTimeout_ms != nullptr && m_pEdtConnectTimeout_ms != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtConnectTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtConnectTimeoutTextChanged(const QString&)) );

        m_pEdtConnectTimeout_ms->setValidator( new QIntValidator(0,60000,this) );
        m_pEdtConnectTimeout_ms->setText( QString::number(i_hostSettings.m_iConnectTimeout_ms) );
        m_pEdtConnectTimeout_ms->setEnabled(true);

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtConnectTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtConnectTimeoutTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblConnectTimeout_ms != nullptr && m_pEdtRemotePort != nullptr )

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

    // <CheckBox> Watch Dog Timer Enabled
    //-----------------------------------

    if( m_pLblWatchDogTimerEnabled != nullptr && m_pChkWatchDogTimerEnabled != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pChkWatchDogTimerEnabled,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkWatchDogTimerEnabledToggled(bool)) );

        if( i_hostSettings.m_socketType == ESocketTypeInProcMsg )
        {
            m_pChkWatchDogTimerEnabled->setChecked(false);
            m_pChkWatchDogTimerEnabled->setEnabled(false);
            m_pChkWatchDogTimerEnabled->hide();
            m_pLblWatchDogTimerEnabled->hide();
        }
        else
        {
            m_pChkWatchDogTimerEnabled->setChecked(i_watchDogSettings.m_bEnabled);
            m_pChkWatchDogTimerEnabled->setEnabled(true);
            m_pChkWatchDogTimerEnabled->show();
            m_pLblWatchDogTimerEnabled->show();
        }

        if( !m_bWatchDogSettingsUsed )
        {
            m_pChkWatchDogTimerEnabled->setEnabled(false);
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pChkWatchDogTimerEnabled,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkWatchDogTimerEnabledToggled(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblWatchDogTimerEnabled != nullptr && m_pChkWatchDogTimerEnabled != nullptr )

    // <LineEdit> Watch Dog Timer Interval
    //------------------------------------

    if( m_pLblWatchDogTimerInterval_ms != nullptr && m_pEdtWatchDogTimerInterval_ms != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtWatchDogTimerInterval_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimerIntervalTextChanged(const QString&)) );

        if( i_hostSettings.m_socketType == ESocketTypeInProcMsg )
        {
            m_pEdtWatchDogTimerInterval_ms->setValidator(nullptr);
            m_pEdtWatchDogTimerInterval_ms->setText("");
            m_pEdtWatchDogTimerInterval_ms->setEnabled(false);
            m_pEdtWatchDogTimerInterval_ms->hide();
            m_pLblWatchDogTimerInterval_ms->hide();
        }
        else
        {
            m_pEdtWatchDogTimerInterval_ms->setValidator( new QIntValidator(14,60000,this) );
            m_pEdtWatchDogTimerInterval_ms->setText( QString::number(i_watchDogSettings.m_iInterval_ms) );
            m_pEdtWatchDogTimerInterval_ms->setEnabled(true);
            m_pEdtWatchDogTimerInterval_ms->show();
            m_pLblWatchDogTimerInterval_ms->show();
        }

        if( !m_bWatchDogSettingsUsed )
        {
            m_pEdtWatchDogTimerInterval_ms->setEnabled(false);
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtWatchDogTimerInterval_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimerIntervalTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblWatchDogTimerInterval_ms != nullptr && m_pEdtWatchDogTimerInterval_ms != nullptr )

    // <LineEdit> Watch Dog Timeout
    //-----------------------------

    if( m_pLblWatchDogTimeout_ms != nullptr && m_pEdtWatchDogTimeout_ms != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtWatchDogTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimeoutTextChanged(const QString&)) );

        if( i_hostSettings.m_socketType == ESocketTypeInProcMsg )
        {
            m_pEdtWatchDogTimeout_ms->setValidator(nullptr);
            m_pEdtWatchDogTimeout_ms->setText("");
            m_pEdtWatchDogTimeout_ms->setEnabled(false);
            m_pEdtWatchDogTimeout_ms->hide();
            m_pLblWatchDogTimeout_ms->hide();
        }
        else
        {
            m_pEdtWatchDogTimeout_ms->setValidator( new QIntValidator(14,60000,this) );
            m_pEdtWatchDogTimeout_ms->setText( QString::number(i_watchDogSettings.m_iTimeout_ms) );
            m_pEdtWatchDogTimeout_ms->setEnabled(true);
            m_pEdtWatchDogTimeout_ms->show();
            m_pLblWatchDogTimeout_ms->show();
        }

        if( !m_bWatchDogSettingsUsed )
        {
            m_pEdtWatchDogTimeout_ms->setEnabled(false);
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtWatchDogTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimeoutTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblWatchDogTimerInterval_ms != nullptr && m_pEdtWatchDogTimeout_ms != nullptr )

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

    if( m_pBtnConnect != nullptr )
    {
        if( pReqInProgress != nullptr )
        {
            m_pBtnConnect->setText(c_strBtnAbort);
        }
        else if( bClientConnected )
        {
            m_pBtnConnect->setText(c_strBtnDisconnect);
        }
        else
        {
            m_pBtnConnect->setText(c_strBtnConnect);
        }
        m_pBtnConnect->setEnabled(true);

    } // if( m_pBtnConnect != nullptr )

    // Update internal cache
    //----------------------

    m_hostSettingsWidget     = i_hostSettings;
    m_watchDogSettingsWidget = i_watchDogSettings;

} // fillCnctControls

//------------------------------------------------------------------------------
void CWdgtIpcClient::resetDetailControls()
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

    if( m_pEdtLocalHostName != nullptr )
    {
        m_pEdtLocalHostName->setText("");
    }
    if( m_pEdtLocalPort != nullptr )
    {
        m_pEdtLocalPort->setText("");
    }
    if( m_pEdtSocketId != nullptr )
    {
        m_pEdtSocketId->setText("");
    }

} // resetDetailControls

//------------------------------------------------------------------------------
void CWdgtIpcClient::fillDetailControls()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "fillDetailControls",
        /* strAddInfo   */ strAddTrcInfo );

    bool        bClientConnected = false;
    QString     strState;
    SSocketDscr socketDscr;
    QString     strConnectionToolTip = getConnectionToolTip();
    QString     strConnection;

    if( m_pClient != nullptr )
    {
        bClientConnected = m_pClient->isConnected();
        strState = m_pClient->state2Str();
        socketDscr = m_pClient->getSocketDscr();
        strConnection = m_pClient->getConnectionString();
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
            else if( bClientConnected )
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
        if( bClientConnected )
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

    if( m_pEdtLocalHostName != nullptr )
    {
        m_pEdtLocalHostName->setText(socketDscr.m_strLocalHostName);
    }
    if( m_pEdtLocalPort != nullptr )
    {
        m_pEdtLocalPort->setText( QString::number(socketDscr.m_uLocalPort) );
    }
    if( m_pEdtSocketId != nullptr )
    {
        m_pEdtSocketId->setText( QString::number(socketDscr.m_iSocketId) );
    }

} // fillDetailControls

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIpcClient::applySettings()
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

    if( m_pClient != nullptr )
    {
        bool bHasChanges = false;

        if( m_hostSettingsWidget != m_hostSettingsClient )
        {
            bHasChanges = true;
            m_pClient->setHostSettings(m_hostSettingsWidget);
        }
        if( m_watchDogSettingsWidget != m_watchDogSettingsClient )
        {
            bHasChanges = true;
            m_pClient->setWatchDogSettings(m_watchDogSettingsWidget);
        }

        // In order for the changed settings to take affect:
        if( bHasChanges )
        {
            m_pClient->changeSettings();
        }
    } // if( m_pClient != nullptr )

} // applySettings
