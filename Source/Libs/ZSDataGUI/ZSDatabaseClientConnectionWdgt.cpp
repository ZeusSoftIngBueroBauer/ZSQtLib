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

#include <QtCore/qsettings.h>
#include <QtCore/qtimer.h>
#include <QtNetwork/qhostinfo.h>
#include <QtSql/qsqldatabase.h>
#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qformlayout.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtableview.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSDataGUI/ZSDatabaseClientConnectionWdgt.h"
#include "ZSDataGUI/ZSDatabaseConnectionsModel.h"
#include "ZSSysGUI/ZSSysProgressBar.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Data;
using namespace ZS::Data::GUI;


/*******************************************************************************
class CWdgtDbClientConnection : public QWidget
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
CWdgtDbClientConnection::CWdgtDbClientConnection( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDbClient(nullptr),
    m_strlstSQLDrivers(),
    m_cxSpacing(30),
    m_cxLblWidthClm1(120),
    m_cxEdtWidthClm1(160),
    m_cxLblWidthClm2(120),
    m_cxEdtWidthClm2(160),
    m_cxLblWidthClm3(80),
    m_cxEdtWidthClm3(160),
    m_pLyt(nullptr),
    m_idxLytItemStretch(-1),
    // Connection Settings
    m_protocolTypeClient(EProtocolTypeUndefined),
    m_protocolTypeClientDefault(EProtocolTypeUndefined),
    m_protocolTypeWidget(EProtocolTypeUndefined),
    m_hostSettingsClient(),
    m_hostSettingsClientDefault(),
    m_hostSettingsWidget(),
    m_pLytLineType(nullptr),
    m_pWdgtProtocolType(nullptr),
    m_pLytProtocolType(nullptr),
    m_pLblProtocolType(nullptr),
    m_pCmbProtocolType(nullptr),
    m_pWdgtSQLDrivers(nullptr),
    m_pLytSQLDrivers(nullptr),
    m_pLblSQLDrivers(nullptr),
    m_pCmbSQLDrivers(nullptr),
    m_pWdgtSocketType(nullptr),
    m_pLytSocketType(nullptr),
    m_pLblSocketType(nullptr),
    m_pCmbSocketType(nullptr),
    m_pLblDefault(nullptr),
    m_pChkDefault(nullptr),
    m_pLblState(nullptr),
    m_pLedState(nullptr),
    m_pLytLineRemoteHost(nullptr),
    m_pWdgtRemoteHostName(nullptr),
    m_pLytRemoteHostName(nullptr),
    m_pLblRemoteHostName(nullptr),
    m_pEdtRemoteHostName(nullptr),
    m_pWdgtRemoteHostAddr(nullptr),
    m_pLytRemoteHostAddr(nullptr),
    m_pLblRemoteHostAddr(nullptr),
    m_pEdtRemoteHostAddr(nullptr),
    m_bRemoteHostInfoLookUpInProcess(false),
    m_pWdgtRemotePort(nullptr),
    m_pLytRemotePort(nullptr),
    m_pLblRemotePort(nullptr),
    m_pEdtRemotePort(nullptr),
    // Connect Timeout and Shm Buffer Size
    m_pWdgtLineConnectTimeoutBufferSize(nullptr),
    m_pLytLineConnectTimeoutBufferSize(nullptr),
    m_pWdgtConnectTimeout(nullptr),
    m_pLytConnectTimeout(nullptr),
    m_pLblConnectTimeout_ms(nullptr),
    m_pEdtConnectTimeout_ms(nullptr),
    m_pWdgtBufferSize(nullptr),
    m_pLytBufferSize(nullptr),
    m_pLblBufferSize_bytes(nullptr),
    m_pEdtBufferSize_bytes(nullptr),
    // User Login
    m_pWdgtLineUserLogin(nullptr),
    m_pLytLineUserLogin(nullptr),
    m_pWdgtDatabaseName(nullptr),
    m_pLytDatabaseName(nullptr),
    m_pLblDatabaseName(nullptr),
    m_pWdgtEdtDatabaseName(nullptr),
    m_pLytEdtDatabaseName(nullptr),
    m_pEdtDatabaseName(nullptr),
    m_pChkDatabaseNameUsed(nullptr),
    m_pWdgtUserName(nullptr),
    m_pLytUserName(nullptr),
    m_pLblUserName(nullptr),
    m_pWdgtEdtUserName(nullptr),
    m_pLytEdtUserName(nullptr),
    m_pEdtUserName(nullptr),
    m_pChkUserNameUsed(nullptr),
    m_pWdgtUserPassword(nullptr),
    m_pLytUserPassword(nullptr),
    m_pLblUserPassword(nullptr),
    m_pWdgtEdtUserPassword(nullptr),
    m_pLytEdtUserPassword(nullptr),
    m_pEdtUserPassword(nullptr),
    m_pChkUserPasswordUsed(nullptr),
    // Watch Dog Timer
    m_watchDogSettingsClient(),
    m_watchDogSettingsClientDefault(),
    m_watchDogSettingsWidget(),
    m_pSepLineWatchDogTimer(nullptr),
    m_pWdgtLineWatchDogTimer(nullptr),
    m_pLytLineWatchDogTimer(nullptr),
    m_pWdgtWatchDogTimerEnabled(nullptr),
    m_pLytWatchDogTimerEnabled(nullptr),
    m_pLblWatchDogTimerEnabled(nullptr),
    m_pChkWatchDogTimerEnabled(nullptr),
    m_pWdgtWatchDogTimerInterval(nullptr),
    m_pLytWatchDogTimerInterval(nullptr),
    m_pLblWatchDogTimerInterval_ms(nullptr),
    m_pEdtWatchDogTimerInterval_ms(nullptr),
    m_pWdgtWatchDogTimeout(nullptr),
    m_pLytWatchDogTimeout(nullptr),
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
    m_pLytLineConnection(nullptr),
    m_pLblActiveConnection(nullptr),
    m_pBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContents(nullptr),
    m_pWdgtSrvCltConnection(nullptr),
    m_pLytSrvCltConnection(nullptr),
    m_pModelSrvCltConnection(nullptr),
    m_pTblViewSrvCltConnection(nullptr),
    // Trace
    m_pWdgtMsgLog(nullptr),
    m_pLytMsgLog(nullptr),
    m_pLblMsgLog(nullptr),
    m_trcMsgLogSettings(),
    m_pWdgtTrcMsgLog(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName("DbClientCnctWdgt");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

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

    // <Line> Type
    //------------

    m_pLytLineType = new QHBoxLayout();
    m_pLytLineType->setSpacing(m_cxSpacing);
    m_pLytLineType->setContentsMargins(0, 0, 0, 0);
    m_pLyt->addLayout(m_pLytLineType);

    // <ComboBox> Data Protocol
    //-------------------------

    m_pWdgtProtocolType = new QWidget();
    m_pWdgtProtocolType->setFixedWidth(m_cxLblWidthClm1 + m_cxEdtWidthClm1);
    m_pLytProtocolType = new QHBoxLayout();
    m_pLytProtocolType->setContentsMargins(0, 0, 0, 0);
    m_pLytProtocolType->setSpacing(0);
    m_pWdgtProtocolType->setLayout(m_pLytProtocolType);
    m_pLytLineType->addWidget(m_pWdgtProtocolType);

    m_pLblProtocolType = new QLabel("Data Protocol:");
    m_pLblProtocolType->setFixedWidth(m_cxLblWidthClm1);
    m_pLytProtocolType->addWidget(m_pLblProtocolType);
    m_pCmbProtocolType = new QComboBox();
    m_pCmbProtocolType->setFixedWidth(m_cxEdtWidthClm1);
    m_pCmbProtocolType->setEnabled(false);
    m_pLytProtocolType->addWidget(m_pCmbProtocolType);
    //m_pLytProtocolType->addSpacing(m_cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbProtocolType,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbProtocolTypeCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <ComboBox> SQLDriver
    //-----------------------

    m_pWdgtSQLDrivers = new QWidget();
    m_pWdgtSQLDrivers->setFixedWidth(m_cxLblWidthClm2 + m_cxEdtWidthClm2);
    m_pLytSQLDrivers = new QHBoxLayout();
    m_pLytSQLDrivers->setContentsMargins(0, 0, 0, 0);
    m_pLytSQLDrivers->setSpacing(0);
    m_pWdgtSQLDrivers->setLayout(m_pLytSQLDrivers);
    m_pLytLineType->addWidget(m_pWdgtSQLDrivers);

    m_pLblSQLDrivers = new QLabel("SQL Drivers:");
    m_pLblSQLDrivers->setFixedWidth(m_cxLblWidthClm2);
    m_pLytSQLDrivers->addWidget(m_pLblSQLDrivers);
    m_pCmbSQLDrivers = new QComboBox();
    m_pCmbSQLDrivers->setFixedWidth(m_cxEdtWidthClm2);
    m_pCmbSQLDrivers->setEnabled(false);
    m_pLytSQLDrivers->addWidget(m_pCmbSQLDrivers);
    //m_pLytSQLDrivers->addSpacing(m_cxSpacing);

    if( m_strlstSQLDrivers.isEmpty() )
    {
        m_pCmbSQLDrivers->setToolTip("No SQL drivers found. File \"qt.conf\" and directory \"plugins/sqldrivers\" must exist and qsql-driver dlls must fit to used Qt-version.");
    }
    else
    {
        m_pCmbSQLDrivers->addItems(m_strlstSQLDrivers);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbSQLDrivers,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbSQLDriversCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <ComboBox> Socket Type
    //-----------------------

    m_pWdgtSocketType = new QWidget();
    m_pWdgtSocketType->setFixedWidth(m_cxLblWidthClm3 + m_cxEdtWidthClm3);
    m_pLytSocketType = new QHBoxLayout();
    m_pLytSocketType->setContentsMargins(0, 0, 0, 0);
    m_pLytSocketType->setSpacing(0);
    m_pWdgtSocketType->setLayout(m_pLytSocketType);
    m_pLytLineType->addWidget(m_pWdgtSocketType);

    m_pLblSocketType = new QLabel("Socket Type:");
    m_pLblSocketType->setFixedWidth(m_cxLblWidthClm3);
    m_pLytSocketType->addWidget(m_pLblSocketType);
    m_pCmbSocketType = new QComboBox();
    m_pCmbSocketType->setFixedWidth(m_cxEdtWidthClm3);
    m_pCmbSocketType->setEnabled(false);
    m_pLytSocketType->addWidget(m_pCmbSocketType);
    //m_pLytSocketType->addSpacing(m_cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbSocketType,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <CheckBox> Default Settings
    //----------------------------

    m_pLblDefault = new QLabel("Default:");
    //m_pLblDefault->setFixedWidth(m_cxLblWidthClm4);
    m_pLytLineType->addWidget(m_pLblDefault);
    m_pChkDefault = new QCheckBox();
    m_pChkDefault->setEnabled(false);
    m_pLytLineType->addWidget(m_pChkDefault);
    m_pLytLineType->addSpacing(m_cxSpacing);
    m_pLytLineType->addStretch();

    if( !QObject::connect(
        /* pObjSender   */ m_pChkDefault,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkDefaultToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Led> State
    //----------------------------

    m_pLblState = new QLabel("State:");
    m_pLytLineType->addWidget(m_pLblState);
    m_pLedState = new QLabel();
    m_pLedState->setMaximumWidth(20);
    m_pLedState->setMaximumHeight(20);
    m_pLedState->setScaledContents(true);
    m_pLedState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
    m_pLytLineType->addWidget(m_pLedState);

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new CSepLine();
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

    // <Line> Remote Host
    //-------------------

    m_pLytLineRemoteHost = new QHBoxLayout();
    m_pLytLineRemoteHost->setContentsMargins(0, 0, 0, 0);
    m_pLytLineRemoteHost->setSpacing(m_cxSpacing);
    m_pLyt->addLayout(m_pLytLineRemoteHost);

    // <LineEdit> Host Name
    //---------------------

    m_pWdgtRemoteHostName = new QWidget();
    m_pWdgtRemoteHostName->setFixedWidth(m_cxLblWidthClm1 + m_cxEdtWidthClm1);
    m_pLytRemoteHostName = new QHBoxLayout();
    m_pLytRemoteHostName->setContentsMargins(0, 0, 0, 0);
    m_pLytRemoteHostName->setSpacing(0);
    m_pWdgtRemoteHostName->setLayout(m_pLytRemoteHostName);
    m_pLytLineRemoteHost->addWidget(m_pWdgtRemoteHostName);

    m_pLblRemoteHostName = new QLabel("Remote Host:");
    m_pLblRemoteHostName->setFixedWidth(m_cxLblWidthClm1);
    m_pLytRemoteHostName->addWidget(m_pLblRemoteHostName);
    m_pEdtRemoteHostName = new QLineEdit();
    m_pEdtRemoteHostName->setFixedWidth(m_cxEdtWidthClm1);
    m_pEdtRemoteHostName->setEnabled(false);
    m_pLytRemoteHostName->addWidget(m_pEdtRemoteHostName);
    //m_pLytRemoteHostName->addSpacing(m_cxSpacing);

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

    m_pWdgtRemoteHostAddr = new QWidget();
    m_pWdgtRemoteHostAddr->setFixedWidth(m_cxLblWidthClm2 + m_cxEdtWidthClm2);
    m_pLytRemoteHostAddr = new QHBoxLayout();
    m_pLytRemoteHostAddr->setContentsMargins(0, 0, 0, 0);
    m_pLytRemoteHostAddr->setSpacing(0);
    m_pWdgtRemoteHostAddr->setLayout(m_pLytRemoteHostAddr);
    m_pLytLineRemoteHost->addWidget(m_pWdgtRemoteHostAddr);

    m_pLblRemoteHostAddr = new QLabel("IP Address:");
    m_pLblRemoteHostAddr->setFixedWidth(m_cxLblWidthClm2);
    m_pLytRemoteHostAddr->addWidget(m_pLblRemoteHostAddr);
    m_pEdtRemoteHostAddr = new QLineEdit();
    m_pEdtRemoteHostAddr->setFixedWidth(m_cxEdtWidthClm2);
    m_pEdtRemoteHostAddr->setEnabled(false);
    m_pLytRemoteHostAddr->addWidget(m_pEdtRemoteHostAddr);
    //m_pLytRemoteHostAddr->addSpacing(m_cxSpacing);

    // <LineEdit> Remote Host Port
    //----------------------------

    QString strPortToolTip;

    if( m_strlstSQLDrivers.contains("QOCI") && m_strlstSQLDrivers.contains("QPSQL") )
    {
        strPortToolTip  = "Port to access the database via TCP/IP.";
        strPortToolTip += "\n - For Oracle databases (QOCI) this is usually 1521.";
        strPortToolTip += "\n - For PostgreSQL databases (QPSQL) this is usually 5432.";
    }
    else if( m_strlstSQLDrivers.contains("QOCI") )
    {
        strPortToolTip  = "Port to access the database via TCP/IP.";
        strPortToolTip += "\n - For Oracle databases (QOCI) this is usually 1521.";
    }
    else if( m_strlstSQLDrivers.contains("QPSQL") )
    {
        strPortToolTip  = "Port to access the database via TCP/IP.";
        strPortToolTip += "\n - For PostgreSQL databases (QPSQL) this is usually 5432.";
    }

    m_pWdgtRemotePort = new QWidget();
    m_pWdgtRemotePort->setFixedWidth(m_cxLblWidthClm3 + m_cxEdtWidthClm3);
    m_pLytRemotePort = new QHBoxLayout();
    m_pLytRemotePort->setContentsMargins(0, 0, 0, 0);
    m_pLytRemotePort->setSpacing(0);
    m_pWdgtRemotePort->setLayout(m_pLytRemotePort);
    m_pLytLineRemoteHost->addWidget(m_pWdgtRemotePort);

    m_pLblRemotePort = new QLabel("Remote Port:");
    m_pLblRemotePort->setFixedWidth(m_cxLblWidthClm3);
    m_pLytRemotePort->addWidget(m_pLblRemotePort);
    m_pEdtRemotePort = new QLineEdit("");
    m_pEdtRemotePort->setFixedWidth(m_cxEdtWidthClm3);
    m_pEdtRemotePort->setEnabled(false);
    m_pEdtRemotePort->setToolTip(strPortToolTip);
    m_pEdtRemotePort->setValidator( new QIntValidator(1,65535,this) );
    m_pLytRemotePort->addWidget(m_pEdtRemotePort);

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtRemotePort,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtRemotePortTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Stretch> at EOL
    //-----------------

    m_pLytLineRemoteHost->addStretch();

    // <Line> Connect Timeout and Shm Buffer Size
    //-------------------------------------------

    m_pWdgtLineConnectTimeoutBufferSize = new QWidget();
    m_pLytLineConnectTimeoutBufferSize = new QHBoxLayout();
    m_pLytLineConnectTimeoutBufferSize->setContentsMargins(0, 0, 0, 0);
    m_pLytLineConnectTimeoutBufferSize->setSpacing(m_cxSpacing);
    m_pWdgtLineConnectTimeoutBufferSize->setLayout(m_pLytLineConnectTimeoutBufferSize);
    m_pLyt->addWidget(m_pWdgtLineConnectTimeoutBufferSize);

    // <LineEdit> Connect Timeout
    //-----------------------------

    m_pWdgtConnectTimeout = new QWidget();
    m_pWdgtConnectTimeout->setFixedWidth(m_cxLblWidthClm1 + m_cxEdtWidthClm1);
    m_pLytConnectTimeout = new QHBoxLayout();
    m_pLytConnectTimeout->setContentsMargins(0, 0, 0, 0);
    m_pLytConnectTimeout->setSpacing(0);
    m_pWdgtConnectTimeout->setLayout(m_pLytConnectTimeout);
    m_pLytLineConnectTimeoutBufferSize->addWidget(m_pWdgtConnectTimeout); // , 0, Qt::AlignLeft);

    m_pLblConnectTimeout_ms = new QLabel("Connect Timeout / ms:");
    m_pLblConnectTimeout_ms->setFixedWidth(m_cxLblWidthClm1);
    m_pLytConnectTimeout->addWidget(m_pLblConnectTimeout_ms);
    m_pEdtConnectTimeout_ms = new QLineEdit();
    m_pEdtConnectTimeout_ms->setFixedWidth(m_cxEdtWidthClm1);
    m_pEdtConnectTimeout_ms->setEnabled(false);
    m_pLytConnectTimeout->addWidget(m_pEdtConnectTimeout_ms);
    //m_pLytConnectTimeout->addSpacing(m_cxSpacing);

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

    m_pWdgtBufferSize = new QWidget();
    m_pWdgtBufferSize->setFixedWidth(m_cxLblWidthClm2 + m_cxEdtWidthClm2);
    m_pLytBufferSize = new QHBoxLayout();
    m_pLytBufferSize->setContentsMargins(0, 0, 0, 0);
    m_pLytBufferSize->setSpacing(0);
    m_pWdgtBufferSize->setLayout(m_pLytBufferSize);
    m_pLytLineConnectTimeoutBufferSize->addWidget(m_pWdgtBufferSize);

    m_pLblBufferSize_bytes = new QLabel("Buffer Size / Bytes:");
    m_pLblBufferSize_bytes->setFixedWidth(m_cxLblWidthClm2);
    m_pLytBufferSize->addWidget(m_pLblBufferSize_bytes);
    m_pEdtBufferSize_bytes = new QLineEdit();
    m_pEdtBufferSize_bytes->setFixedWidth(m_cxEdtWidthClm2);
    m_pEdtBufferSize_bytes->setEnabled(false);
    m_pLytBufferSize->addWidget(m_pEdtBufferSize_bytes);

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtBufferSize_bytes,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtBufferSizeTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Stretch> at EOL
    //-----------------

    m_pLytLineConnectTimeoutBufferSize->addStretch();

    // <Line> Database Name and User Login
    //------------------------------------

    m_pWdgtLineUserLogin = new QWidget();
    m_pLyt->addWidget(m_pWdgtLineUserLogin);

    m_pLytLineUserLogin = new QHBoxLayout();
    m_pLytLineUserLogin->setContentsMargins(0, 0, 0, 0);
    m_pLytLineUserLogin->setSpacing(m_cxSpacing);
    m_pWdgtLineUserLogin->setLayout(m_pLytLineUserLogin);

    // <LineEdit> Database Name
    //-------------------------

    m_pWdgtDatabaseName = new QWidget();
    m_pWdgtDatabaseName->setFixedWidth(m_cxLblWidthClm1 + m_cxEdtWidthClm1);
    m_pLytDatabaseName = new QHBoxLayout();
    m_pLytDatabaseName->setContentsMargins(0, 0, 0, 0);
    m_pLytDatabaseName->setSpacing(0);
    m_pWdgtDatabaseName->setLayout(m_pLytDatabaseName);
    m_pLytLineUserLogin->addWidget(m_pWdgtDatabaseName);

    m_pLblDatabaseName = new QLabel("Database Name:");
    m_pLblDatabaseName->setFixedWidth(m_cxLblWidthClm1);
    m_pLytDatabaseName->addWidget(m_pLblDatabaseName);
    m_pWdgtEdtDatabaseName = new QWidget();
    m_pLytEdtDatabaseName = new QHBoxLayout();
    m_pLytEdtDatabaseName->setContentsMargins(0, 0, 0, 0);
    m_pWdgtEdtDatabaseName->setLayout(m_pLytEdtDatabaseName);
    m_pWdgtEdtDatabaseName->setFixedWidth(m_cxEdtWidthClm1);
    m_pLytDatabaseName->addWidget(m_pWdgtEdtDatabaseName);
    m_pEdtDatabaseName = new QLineEdit();
    m_pEdtDatabaseName->setEnabled(false);
    m_pLytEdtDatabaseName->addWidget(m_pEdtDatabaseName,1);
    m_pChkDatabaseNameUsed = new QCheckBox("Used");
    m_pChkDatabaseNameUsed->setEnabled(false);
    m_pLytEdtDatabaseName->addWidget(m_pChkDatabaseNameUsed);
    //m_pLytDatabaseName->addSpacing(m_cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pChkDatabaseNameUsed,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkDatabaseNameUsedToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtDatabaseName,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtDatabaseNameTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> User
    //----------------

    m_pWdgtUserName = new QWidget();
    m_pWdgtUserName->setFixedWidth(m_cxLblWidthClm2 + m_cxEdtWidthClm2);
    m_pLytUserName = new QHBoxLayout();
    m_pLytUserName->setContentsMargins(0, 0, 0, 0);
    m_pLytUserName->setSpacing(0);
    m_pWdgtUserName->setLayout(m_pLytUserName);
    m_pLytLineUserLogin->addWidget(m_pWdgtUserName);

    m_pLblUserName = new QLabel("User:");
    m_pLblUserName->setFixedWidth(m_cxLblWidthClm2);
    m_pLytUserName->addWidget(m_pLblUserName);
    m_pWdgtEdtUserName = new QWidget();
    m_pLytEdtUserName = new QHBoxLayout();
    m_pLytEdtUserName->setContentsMargins(0, 0, 0, 0);
    m_pWdgtEdtUserName->setLayout(m_pLytEdtUserName);
    m_pWdgtEdtUserName->setFixedWidth(m_cxEdtWidthClm2);
    m_pLytUserName->addWidget(m_pWdgtEdtUserName);
    m_pEdtUserName = new QLineEdit();
    m_pEdtUserName->setEnabled(false);
    m_pLytEdtUserName->addWidget(m_pEdtUserName,1);
    m_pChkUserNameUsed = new QCheckBox("Used");
    m_pChkUserNameUsed->setEnabled(false);
    m_pLytEdtUserName->addWidget(m_pChkUserNameUsed);
    //m_pLytUserName->addSpacing(m_cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pChkUserNameUsed,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkUserNameUsedToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtUserName,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtUserNameTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Password
    //--------------------

    m_pWdgtUserPassword = new QWidget();
    m_pWdgtUserPassword->setFixedWidth(m_cxLblWidthClm3 + m_cxEdtWidthClm3);
    m_pLytUserPassword = new QHBoxLayout();
    m_pLytUserPassword->setContentsMargins(0, 0, 0, 0);
    m_pLytUserPassword->setSpacing(0);
    m_pWdgtUserPassword->setLayout(m_pLytUserPassword);
    m_pLytLineUserLogin->addWidget(m_pWdgtUserPassword);

    m_pLblUserPassword = new QLabel("Password:");
    m_pLblUserPassword->setFixedWidth(m_cxLblWidthClm3);
    m_pLytUserPassword->addWidget(m_pLblUserPassword);
    m_pWdgtEdtUserPassword = new QWidget();
    m_pLytEdtUserPassword = new QHBoxLayout();
    m_pLytEdtUserPassword->setContentsMargins(0, 0, 0, 0);
    m_pWdgtEdtUserPassword->setLayout(m_pLytEdtUserPassword);
    m_pWdgtEdtUserPassword->setFixedWidth(m_cxEdtWidthClm3);
    m_pLytUserPassword->addWidget(m_pWdgtEdtUserPassword);
    m_pEdtUserPassword = new QLineEdit();
    m_pEdtUserPassword->setEnabled(false);
    m_pLytEdtUserPassword->addWidget(m_pEdtUserPassword,1);
    m_pChkUserPasswordUsed = new QCheckBox("Used");
    m_pChkUserPasswordUsed->setEnabled(false);
    m_pLytEdtUserPassword->addWidget(m_pChkUserPasswordUsed);

    if( !QObject::connect(
        /* pObjSender   */ m_pChkUserPasswordUsed,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkUserPasswordUsedToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtUserPassword,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtUserPasswordTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Stretch> at EOL
    //-----------------

    m_pLytLineUserLogin->addStretch();

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    m_pSepLineWatchDogTimer = new CSepLine();
    m_pLyt->addWidget(m_pSepLineWatchDogTimer);

    m_pLyt->addSpacing(5);

    // <Line> Watch Dog Timer
    //-----------------------

    m_pWdgtLineWatchDogTimer = new QWidget();
    m_pLyt->addWidget(m_pWdgtLineWatchDogTimer);
    m_pLytLineWatchDogTimer = new QHBoxLayout();
    m_pLytLineWatchDogTimer->setContentsMargins(0, 0, 0, 0);
    m_pLytLineWatchDogTimer->setSpacing(m_cxSpacing);
    m_pWdgtLineWatchDogTimer->setLayout(m_pLytLineWatchDogTimer);

    // <CheckBox> Watch Dog Timer Enabled
    //-----------------------------------

    m_pWdgtWatchDogTimerEnabled = new QWidget();
    m_pWdgtWatchDogTimerEnabled->setFixedWidth(m_cxLblWidthClm1 + m_cxEdtWidthClm1);
    m_pLytWatchDogTimerEnabled = new QHBoxLayout();
    m_pLytWatchDogTimerEnabled->setContentsMargins(0, 0, 0, 0);
    m_pLytWatchDogTimerEnabled->setSpacing(0);
    m_pWdgtWatchDogTimerEnabled->setLayout(m_pLytWatchDogTimerEnabled);
    m_pLytLineWatchDogTimer->addWidget(m_pWdgtWatchDogTimerEnabled);

    m_pLblWatchDogTimerEnabled = new QLabel( "Watch Dog:" );
    m_pLblWatchDogTimerEnabled->setFixedWidth(m_cxLblWidthClm1);
    m_pLytWatchDogTimerEnabled->addWidget(m_pLblWatchDogTimerEnabled);
    m_pChkWatchDogTimerEnabled = new QCheckBox("Used");
    m_pChkWatchDogTimerEnabled->setFixedWidth(m_cxEdtWidthClm1);
    m_pChkWatchDogTimerEnabled->setEnabled(false);
    m_pLytWatchDogTimerEnabled->addWidget(m_pChkWatchDogTimerEnabled, 0, Qt::AlignRight);
    //m_pLytWatchDogTimerEnabled->addSpacing(m_cxSpacing);

    // <LineEdit> Watch Dog Timer Interval
    //------------------------------------

    m_pWdgtWatchDogTimerInterval = new QWidget();
    m_pWdgtWatchDogTimerInterval->setFixedWidth(m_cxLblWidthClm2 + m_cxEdtWidthClm2);
    m_pLytWatchDogTimerInterval = new QHBoxLayout();
    m_pLytWatchDogTimerInterval->setContentsMargins(0, 0, 0, 0);
    m_pLytWatchDogTimerInterval->setSpacing(0);
    m_pWdgtWatchDogTimerInterval->setLayout(m_pLytWatchDogTimerInterval);
    m_pLytLineWatchDogTimer->addWidget(m_pWdgtWatchDogTimerInterval);

    m_pLblWatchDogTimerInterval_ms = new QLabel("Interval / ms:");
    m_pLblWatchDogTimerInterval_ms->setFixedWidth(m_cxLblWidthClm2);
    m_pLytWatchDogTimerInterval->addWidget(m_pLblWatchDogTimerInterval_ms);
    m_pEdtWatchDogTimerInterval_ms = new QLineEdit();
    m_pEdtWatchDogTimerInterval_ms->setFixedWidth(m_cxEdtWidthClm2);
    m_pEdtWatchDogTimerInterval_ms->setEnabled(false);
    m_pEdtWatchDogTimerInterval_ms->setValidator( new QIntValidator(1,60000,this) );
    m_pLytWatchDogTimerInterval->addWidget(m_pEdtWatchDogTimerInterval_ms);
    //m_pLytWatchDogTimerInterval->addSpacing(m_cxSpacing);

    // <LineEdit> Watch Dog Timeout
    //-----------------------------

    m_pWdgtWatchDogTimeout = new QWidget();
    m_pWdgtWatchDogTimeout->setFixedWidth(m_cxLblWidthClm3 + m_cxEdtWidthClm3);
    m_pLytWatchDogTimeout = new QHBoxLayout();
    m_pLytWatchDogTimeout->setContentsMargins(0, 0, 0, 0);
    m_pLytWatchDogTimeout->setSpacing(0);
    m_pWdgtWatchDogTimeout->setLayout(m_pLytWatchDogTimeout);
    m_pLytLineWatchDogTimer->addWidget(m_pWdgtWatchDogTimeout);

    m_pLblWatchDogTimeout_ms = new QLabel("Timeout / ms:");
    m_pLblWatchDogTimeout_ms->setFixedWidth(m_cxLblWidthClm3);
    m_pLytWatchDogTimeout->addWidget(m_pLblWatchDogTimeout_ms);
    m_pEdtWatchDogTimeout_ms = new QLineEdit();
    m_pEdtWatchDogTimeout_ms->setFixedWidth(m_cxEdtWidthClm3);
    m_pEdtWatchDogTimeout_ms->setEnabled(false);
    m_pEdtWatchDogTimeout_ms->setValidator( new QIntValidator(1,60000,this) );
    m_pLytWatchDogTimeout->addWidget(m_pEdtWatchDogTimeout_ms);

    // <Stretch> at EOL
    //-----------------

    m_pLytLineWatchDogTimer->addStretch();

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
    m_pLytDetails->setContentsMargins(0, 0, 0, 0); // left, top, right, bottom
    m_pWdgtDetails->setLayout(m_pLytDetails);
    m_pLyt->addWidget(m_pWdgtDetails);

    // <Button> Show Details
    //----------------------

    m_pBtnDetails = new QPushButton(c_strBtnDetailsShowDetails);
    m_pBtnDetails->setFixedWidth(m_cxLblWidthClm1);
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

    m_idxLytItemStretch = m_pLyt->count();
    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtDbClientConnection::~CWdgtDbClientConnection()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "CDbClient.dtor",
        /* strAddInfo   */ "" );

    saveSettings();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDbClient = nullptr;
    //m_strlstSQLDrivers;
    m_cxSpacing = 0;
    m_cxLblWidthClm1 = 0;
    m_cxEdtWidthClm1 = 0;
    m_cxLblWidthClm2 = 0;
    m_cxEdtWidthClm2 = 0;
    m_cxLblWidthClm3 = 0;
    m_cxEdtWidthClm3 = 0;
    m_pLyt = nullptr;
    m_idxLytItemStretch = 0;
    // Connection Settings
    m_protocolTypeClient = static_cast<EProtocolType>(0);
    m_protocolTypeClientDefault = static_cast<EProtocolType>(0);
    m_protocolTypeWidget = static_cast<EProtocolType>(0);
    //m_hostSettingsClient;
    //m_hostSettingsClientDefault;
    //m_hostSettingsWidget;
    m_pLytLineType = nullptr;
    m_pWdgtProtocolType = nullptr;
    m_pLytProtocolType = nullptr;
    m_pLblProtocolType = nullptr;
    m_pCmbProtocolType = nullptr;
    m_pWdgtSQLDrivers = nullptr;
    m_pLytSQLDrivers = nullptr;
    m_pLblSQLDrivers = nullptr;
    m_pCmbSQLDrivers = nullptr;
    m_pWdgtSocketType = nullptr;
    m_pLytSocketType = nullptr;
    m_pLblSocketType = nullptr;
    m_pCmbSocketType = nullptr;
    m_pLblDefault = nullptr;
    m_pChkDefault = nullptr;
    m_pLblState = nullptr;
    m_pLedState = nullptr;
    m_pLytLineRemoteHost = nullptr;
    m_pWdgtRemoteHostName = nullptr;
    m_pLytRemoteHostName = nullptr;
    m_pLblRemoteHostName = nullptr;
    m_pEdtRemoteHostName = nullptr;
    m_pWdgtRemoteHostAddr = nullptr;
    m_pLytRemoteHostAddr = nullptr;
    m_pLblRemoteHostAddr = nullptr;
    m_pEdtRemoteHostAddr = nullptr;
    m_bRemoteHostInfoLookUpInProcess = false;
    m_pWdgtRemotePort = nullptr;
    m_pLytRemotePort = nullptr;
    m_pLblRemotePort = nullptr;
    m_pEdtRemotePort = nullptr;
    // Connect Timeout and Shm Buffer Size
    m_pWdgtLineConnectTimeoutBufferSize = nullptr;
    m_pLytLineConnectTimeoutBufferSize = nullptr;
    m_pWdgtConnectTimeout = nullptr;
    m_pLytConnectTimeout = nullptr;
    m_pLblConnectTimeout_ms = nullptr;
    m_pEdtConnectTimeout_ms = nullptr;
    m_pWdgtBufferSize = nullptr;
    m_pLytBufferSize = nullptr;
    m_pLblBufferSize_bytes = nullptr;
    m_pEdtBufferSize_bytes = nullptr;
    // User Login
    m_pWdgtLineUserLogin = nullptr;
    m_pLytLineUserLogin = nullptr;
    m_pWdgtDatabaseName = nullptr;
    m_pLytDatabaseName = nullptr;
    m_pLblDatabaseName = nullptr;
    m_pWdgtEdtDatabaseName = nullptr;
    m_pLytEdtDatabaseName = nullptr;
    m_pEdtDatabaseName = nullptr;
    m_pChkDatabaseNameUsed = nullptr;
    m_pWdgtUserName = nullptr;
    m_pLytUserName = nullptr;
    m_pLblUserName = nullptr;
    m_pWdgtEdtUserName = nullptr;
    m_pLytEdtUserName = nullptr;
    m_pEdtUserName = nullptr;
    m_pChkUserNameUsed = nullptr;
    m_pWdgtUserPassword = nullptr;
    m_pLytUserPassword = nullptr;
    m_pLblUserPassword = nullptr;
    m_pWdgtEdtUserPassword = nullptr;
    m_pLytEdtUserPassword = nullptr;
    m_pEdtUserPassword = nullptr;
    m_pChkUserPasswordUsed = nullptr;
    // Watch Dog Timer
    //m_watchDogSettingsClient;
    //m_watchDogSettingsClientDefault;
    //m_watchDogSettingsWidget;
    m_pSepLineWatchDogTimer = nullptr;
    m_pWdgtLineWatchDogTimer = nullptr;
    m_pLytLineWatchDogTimer = nullptr;
    m_pWdgtWatchDogTimerEnabled = nullptr;
    m_pLytWatchDogTimerEnabled = nullptr;
    m_pLblWatchDogTimerEnabled = nullptr;
    m_pChkWatchDogTimerEnabled = nullptr;
    m_pWdgtWatchDogTimerInterval = nullptr;
    m_pLytWatchDogTimerInterval = nullptr;
    m_pLblWatchDogTimerInterval_ms = nullptr;
    m_pEdtWatchDogTimerInterval_ms = nullptr;
    m_pWdgtWatchDogTimeout = nullptr;
    m_pLytWatchDogTimeout = nullptr;
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
    m_pLytLineConnection = nullptr;
    m_pLblActiveConnection = nullptr;
    m_pBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContents = nullptr;
    m_pWdgtSrvCltConnection = nullptr;
    m_pLytSrvCltConnection = nullptr;
    m_pModelSrvCltConnection = nullptr;
    m_pTblViewSrvCltConnection = nullptr;
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
void CWdgtDbClientConnection::setDbClient( CDbClient* i_pDbClient )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DbClient: " + QString( i_pDbClient == nullptr ? "nullptr" : i_pDbClient->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDbClient",
        /* strAddInfo   */ strAddTrcInfo );

    // Show/hide GUI controls depending on active connection (depending on protocol type),
    // show/hide details, fill connection controls (connection parameters and watch dog settings)
    // and fill detail controls:

    if( m_pDbClient != i_pDbClient )
    {
        if( m_pDbClient != nullptr )
        {
            // <Signals/Slots> of connection
            //------------------------------

            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(stateChanged(ZS::Data::CDbClient*, ZS::Data::CDbClient::EState)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientStateChanged(ZS::Data::CDbClient*, ZS::Data::CDbClient::EState)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(requestInProgressChanged(ZS::Data::CDbClient*, ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientRequestInProgressChanged(ZS::Data::CDbClient*, ZS::System::SRequestDscr)) );

            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(connected(ZS::Data::CDbClient*, const ZS::Ipc::SSocketDscr&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientConnected(ZS::Data::CDbClient*, const ZS::Ipc::SSocketDscr&)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(disconnected(ZS::Data::CDbClient*, const ZS::Ipc::SSocketDscr&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDisconnected(ZS::Data::CDbClient*, const ZS::Ipc::SSocketDscr&)) );

            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(defaultProtocolTypeChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDefaultProtocolTypeChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(protocolTypeChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientProtocolTypeChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(protocolTypeRowVersionChanged(ZS::Data::CDbClient*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientProtocolTypeRowVersionChanged(ZS::Data::CDbClient*)) );

            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(defaultSocketTypeChanged(ZS::Data::CDbClient*, ZS::Ipc::ESocketType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDefaultSocketTypeChanged(ZS::Data::CDbClient*, ZS::Ipc::ESocketType)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(socketTypeChanged(ZS::Data::CDbClient*, ZS::Ipc::ESocketType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientSocketTypeChanged(ZS::Data::CDbClient*, ZS::Ipc::ESocketType)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(socketTypeRowVersionChanged(ZS::Data::CDbClient*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientSocketTypeRowVersionChanged(ZS::Data::CDbClient*)) );

            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(defaultHostSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, const ZS::Ipc::SClientHostSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDefaultHostSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, const ZS::Ipc::SClientHostSettings&)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(hostSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, const ZS::Ipc::SClientHostSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientHostSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, const ZS::Ipc::SClientHostSettings&)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(hostSettingsRowVersionChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientHostSettingsRowVersionChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)) );

            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(defaultWatchDogSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType, const ZS::Ipc::STimerSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDefaultWatchDogSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType, const ZS::Ipc::STimerSettings&)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(watchDogSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType, const ZS::Ipc::STimerSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientWatchDogSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType, const ZS::Ipc::STimerSettings&)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(watchDogSettingsRowVersionChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientWatchDogSettingsRowVersionChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)) );

            resetCnctControls();

            if( m_bShowDetails )
            {
                resetDetailControls();
            }
        } // if( m_pDbClient != nullptr )

        m_pDbClient = i_pDbClient;

        m_protocolTypeClient        = EProtocolTypeUndefined;
        m_protocolTypeClientDefault = EProtocolTypeUndefined;
        m_protocolTypeWidget        = EProtocolTypeUndefined;

        m_hostSettingsClient        = SClientHostSettings();
        m_hostSettingsClientDefault = SClientHostSettings();
        m_hostSettingsWidget        = SClientHostSettings();

        m_watchDogSettingsClient        = STimerSettings();
        m_watchDogSettingsClientDefault = STimerSettings();
        m_watchDogSettingsWidget        = STimerSettings();

        if( m_pDbClient != nullptr )
        {
            // <Signals/Slots> of connection
            //------------------------------

            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(stateChanged(ZS::Data::CDbClient*, ZS::Data::CDbClient::EState)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientStateChanged(ZS::Data::CDbClient*, ZS::Data::CDbClient::EState)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(requestInProgressChanged(ZS::Data::CDbClient*, ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientRequestInProgressChanged(ZS::Data::CDbClient*, ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(connected(ZS::Data::CDbClient*, const ZS::Ipc::SSocketDscr&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientConnected(ZS::Data::CDbClient*, const ZS::Ipc::SSocketDscr&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(disconnected(ZS::Data::CDbClient*, const ZS::Ipc::SSocketDscr&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDisconnected(ZS::Data::CDbClient*, const ZS::Ipc::SSocketDscr&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(defaultProtocolTypeChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDefaultProtocolTypeChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(protocolTypeChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientProtocolTypeChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(protocolTypeRowVersionChanged(ZS::Data::CDbClient*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientProtocolTypeRowVersionChanged(ZS::Data::CDbClient*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(defaultSocketTypeChanged(ZS::Data::CDbClient*, ZS::Ipc::ESocketType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDefaultSocketTypeChanged(ZS::Data::CDbClient*, ZS::Ipc::ESocketType)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(socketTypeChanged(ZS::Data::CDbClient*, ZS::Ipc::ESocketType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientSocketTypeChanged(ZS::Data::CDbClient*, ZS::Ipc::ESocketType)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(socketTypeRowVersionChanged(ZS::Data::CDbClient*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientSocketTypeRowVersionChanged(ZS::Data::CDbClient*)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(defaultHostSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, const ZS::Ipc::SClientHostSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDefaultHostSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, const ZS::Ipc::SClientHostSettings&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(hostSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, const ZS::Ipc::SClientHostSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientHostSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, const ZS::Ipc::SClientHostSettings&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(hostSettingsRowVersionChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientHostSettingsRowVersionChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(defaultWatchDogSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType, const ZS::Ipc::STimerSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientDefaultWatchDogSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType, const ZS::Ipc::STimerSettings&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(watchDogSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType, const ZS::Ipc::STimerSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientWatchDogSettingsChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType, const ZS::Ipc::STimerSettings&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDbClient,
                /* szSignal     */ SIGNAL(watchDogSettingsRowVersionChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbClientWatchDogSettingsRowVersionChanged(ZS::Data::CDbClient*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            // Read current settings
            //----------------------

            m_protocolTypeClient     = m_pDbClient->getProtocolType();
            m_hostSettingsClient = m_pDbClient->getHostSettings();
            m_watchDogSettingsClient = m_pDbClient->getWatchDogSettings();

            if( m_pDbClient->getDefaultProtocolType() != EProtocolTypeUndefined )
            {
                m_protocolTypeClientDefault = m_pDbClient->getDefaultProtocolType();
            }
            if( m_pDbClient->getDefaultHostSettings() != nullptr )
            {
                m_hostSettingsClientDefault = *m_pDbClient->getDefaultHostSettings();
            }
            if( m_pDbClient->getDefaultWatchDogSettings() != nullptr )
            {
                m_watchDogSettingsClientDefault = *m_pDbClient->getDefaultWatchDogSettings();
            }

            fillCnctControls(m_protocolTypeClient, m_hostSettingsClient, m_watchDogSettingsClient);

            if( m_bShowDetails )
            {
                fillDetailControls();
            }
        } // if( m_pDbClient != nullptr )
    } // if( m_pDbClient != i_pDbClient )

} // setDbClient

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::readSettings()
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
void CWdgtDbClientConnection::saveSettings()
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
void CWdgtDbClientConnection::onCmbProtocolTypeCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = QString::number(i_iIdx);
        strAddTrcInfo += " (" + protocolType2Str(i_iIdx) + ")";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onCmbProtocolTypeCurrentIndexChanged",
        /* strAddInfo   */ strAddTrcInfo );

    m_protocolTypeWidget = static_cast<EProtocolType>(m_pCmbProtocolType->itemData(i_iIdx).toInt());

    if( m_pDbClient != nullptr )
    {
        if( !m_pDbClient->isSocketTypeSupported(m_hostSettingsWidget.m_socketType, m_protocolTypeWidget) )
        {
            m_hostSettingsWidget.m_socketType = m_pDbClient->getSocketType();
        }
    }

    #ifdef _WINDOWS
    #pragma message(__TODO__"Change m_hostSettingsWidget depending on socket type and fillCnctControls")
    #endif

    fillCnctControls(m_protocolTypeWidget, m_hostSettingsWidget, m_watchDogSettingsWidget);

    if( m_protocolTypeWidget == m_protocolTypeClientDefault && m_hostSettingsWidget == m_hostSettingsClientDefault && m_watchDogSettingsWidget == m_watchDogSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_protocolTypeWidget == m_protocolTypeClient && m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

    if( m_bShowDetails )
    {
        fillDetailControls();
    }

} // onCmbProtocolTypeCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onCmbSocketTypeCurrentIndexChanged( int i_iIdx )
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

    if( m_pDbClient != nullptr )
    {
        if( !m_pDbClient->isProtocolTypeSupported(m_protocolTypeWidget, m_hostSettingsWidget.m_socketType) )
        {
            m_protocolTypeWidget = m_pDbClient->getProtocolType();
        }
    }

    #ifdef _WINDOWS
    #pragma message(__TODO__"Change m_hostSettingsWidget depending on socket type and fillCnctControls")
    #endif

    fillCnctControls(m_protocolTypeWidget, m_hostSettingsWidget, m_watchDogSettingsWidget);

    if( m_protocolTypeWidget == m_protocolTypeClientDefault && m_hostSettingsWidget == m_hostSettingsClientDefault && m_watchDogSettingsWidget == m_watchDogSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_protocolTypeWidget == m_protocolTypeClient && m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

    if( m_bShowDetails )
    {
        fillDetailControls();
    }

} // onCmbSocketTypeCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onChkDefaultToggled( bool i_bChecked )
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
        /* strMethod    */ "onChkDefaultToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_bChecked )
    {
        if( m_pDbClient != nullptr )
        {
            EProtocolType        protocolTypeDefault      = m_pDbClient->getDefaultProtocolType();
            SClientHostSettings* pHostSettingsDefault     = m_pDbClient->getDefaultHostSettings();
            STimerSettings*      pWatchDogSettingsDefault = m_pDbClient->getDefaultWatchDogSettings();

            EProtocolType       protocolType     = m_protocolTypeClient;
            SClientHostSettings hostSettings     = m_hostSettingsClient;
            STimerSettings      watchDogSettings = m_watchDogSettingsClient;

            if( protocolTypeDefault != EProtocolTypeUndefined )
            {
                protocolType = protocolTypeDefault;
            }
            if( pHostSettingsDefault != nullptr )
            {
                hostSettings = *pHostSettingsDefault;
            }
            if( pWatchDogSettingsDefault != nullptr )
            {
                watchDogSettings = *pWatchDogSettingsDefault;
            }

            fillCnctControls(protocolType, hostSettings, watchDogSettings);

            if( m_protocolTypeWidget == m_protocolTypeClient && m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
            {
                m_pBtnReset->setEnabled(false);
                m_pBtnApply->setEnabled(false);
            }
            else
            {
                m_pBtnReset->setEnabled(true);
                m_pBtnApply->setEnabled(true);
            }

            if( m_bShowDetails )
            {
                fillDetailControls();
            }
        } // if( m_pDbClient != nullptr )
    } // if( i_bChecked )

} // onChkDefaultToggled

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onEdtRemoteHostNameTextChanged( const QString& i_strText )
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

    if( m_hostSettingsWidget == m_hostSettingsClientDefault && m_watchDogSettingsWidget == m_watchDogSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onEdtRemoteHostNameTextChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onEdtRemotePortTextChanged( const QString& i_strText )
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

    if( m_hostSettingsWidget == m_hostSettingsClientDefault && m_watchDogSettingsWidget == m_watchDogSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onEdtRemotePortTextChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onEdtConnectTimeoutTextChanged( const QString& i_strText )
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

    if( m_hostSettingsWidget == m_hostSettingsClientDefault && m_watchDogSettingsWidget == m_watchDogSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onEdtConnectTimeoutTextChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onEdtBufferSizeTextChanged( const QString& i_strText )
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

    if( m_hostSettingsWidget == m_hostSettingsClientDefault && m_watchDogSettingsWidget == m_watchDogSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onEdtBufferSizeTextChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onCmbSQLDriversCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString::number(i_iIdx);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onCmbSQLDriversCurrentIndexChanged",
        /* strAddInfo   */ strAddTrcInfo );


} // onCmbSQLDriversCurrentIndexChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onChkDatabaseNameUsedToggled( bool i_bChecked )
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
        /* strMethod    */ "onChkDatabaseNameUsedToggled",
        /* strAddInfo   */ strAddTrcInfo );

    //m_dbSettingsWidget.m_bDatabaseNameUsed = i_bChecked;

    //if( m_pEdtDatabaseName != nullptr )
    //{
    //    if( m_dbSettingsWidget.m_bDatabaseNameUsed )
    //    {
    //        m_pEdtDatabaseName->setEnabled(true);
    //    }
    //    else
    //    {
    //        m_pEdtDatabaseName->setEnabled(false);
    //    }
    //}

    if( m_hostSettingsWidget == m_hostSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onChkDatabaseNameUsedToggled

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onEdtDatabaseNameTextChanged( const QString& i_strText )
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
        /* strMethod    */ "onEdtDatabaseNameTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    //m_dbSettingsWidget.m_strDatabaseName = i_strText;

    if( m_hostSettingsWidget == m_hostSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onEdtDatabaseNameTextChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onChkUserNameUsedToggled( bool i_bChecked )
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
        /* strMethod    */ "onChkUserNameUsedToggled",
        /* strAddInfo   */ strAddTrcInfo );

    //m_dbSettingsWidget.m_bUserNameUsed = i_bChecked;

    //if( m_pEdtUserName != nullptr )
    //{
    //    if( m_dbSettingsWidget.m_bUserNameUsed )
    //    {
    //        m_pEdtUserName->setEnabled(true);
    //    }
    //    else
    //    {
    //        m_pEdtUserName->setEnabled(false);
    //    }
    //}

    if( m_hostSettingsWidget == m_hostSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onChkUserNameUsedToggled

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onEdtUserNameTextChanged( const QString& i_strText )
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
        /* strMethod    */ "onEdtUserNameTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    //m_dbSettingsWidget.m_strUserName = i_strText;

    if( m_hostSettingsWidget == m_hostSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onEdtUserNameTextChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onChkUserPasswordUsedToggled( bool i_bChecked )
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
        /* strMethod    */ "onChkUserPasswordUsedToggled",
        /* strAddInfo   */ strAddTrcInfo );

    //m_dbSettingsWidget.m_bUsePasswordUsed = i_bChecked;

    //if( m_pEdtUserPassword != nullptr )
    //{
    //    if( m_dbSettingsWidget.m_bUsePasswordUsed )
    //    {
    //        m_pEdtUserPassword->setEnabled(true);
    //    }
    //    else
    //    {
    //        m_pEdtUserPassword->setEnabled(false);
    //    }
    //}

    if( m_hostSettingsWidget == m_hostSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onChkUserPasswordUsedToggled

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onEdtUserPasswordTextChanged( const QString& i_strText )
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
        /* strMethod    */ "onEdtUserPasswordTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    //m_dbSettingsWidget.m_strUserPassword = i_strText;

    if( m_hostSettingsWidget == m_hostSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onEdtUserPasswordTextChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onChkWatchDogTimerEnabledToggled( bool i_bChecked )
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

    if( m_hostSettingsWidget == m_hostSettingsClientDefault && m_watchDogSettingsWidget == m_watchDogSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onChkWatchDogTimerEnabledToggled

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onEdtWatchDogTimerIntervalTextChanged( const QString& i_strText )
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

    if( m_hostSettingsWidget == m_hostSettingsClientDefault && m_watchDogSettingsWidget == m_watchDogSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onEdtWatchDogTimerIntervalTextChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onEdtWatchDogTimeoutTextChanged( const QString& i_strText )
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

    if( m_hostSettingsWidget == m_hostSettingsClientDefault && m_watchDogSettingsWidget == m_watchDogSettingsClientDefault )
    {
        m_pChkDefault->setChecked(true);
        m_pChkDefault->setEnabled(false);
    }
    else
    {
        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);
    }

    if( m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onEdtWatchDogTimeoutTextChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onBtnOkClicked( bool /*i_bChecked*/ )
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
void CWdgtDbClientConnection::onBtnCancelClicked( bool /*i_bChecked*/ )
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

    if( m_pDbClient != nullptr )
    {
        m_protocolTypeClient     = m_pDbClient->getProtocolType();
        m_hostSettingsClient     = m_pDbClient->getHostSettings();
        m_watchDogSettingsClient = m_pDbClient->getWatchDogSettings();

        fillCnctControls(m_protocolTypeClient, m_hostSettingsClient, m_watchDogSettingsClient);

        if( m_bShowDetails )
        {
            fillDetailControls();
        }
    } // if( m_pDbClient != nullptr )

    emit rejected();

} // onBtnCancelClicked

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onBtnResetClicked( bool /*i_bChecked*/ )
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

    if( m_pDbClient != nullptr )
    {
        m_protocolTypeClient     = m_pDbClient->getProtocolType();
        m_hostSettingsClient     = m_pDbClient->getHostSettings();
        m_watchDogSettingsClient = m_pDbClient->getWatchDogSettings();

        fillCnctControls(m_protocolTypeClient, m_hostSettingsClient, m_watchDogSettingsClient);

        if( m_bShowDetails )
        {
            fillDetailControls();
        }
    } // if( m_pDbClient != nullptr )

} // onBtnResetClicked

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onBtnApplyClicked( bool /*i_bChecked*/ )
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

    if( m_protocolTypeClient != m_protocolTypeWidget
     || m_hostSettingsClient != m_hostSettingsWidget
     || m_watchDogSettingsClient != m_watchDogSettingsWidget )
    {
        applySettings();
    }

} // onBtnApplyClicked

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onBtnConnectClicked( bool /*i_bChecked*/ )
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

    if( m_pDbClient != nullptr )
    {
        if( m_pBtnConnect->text() == c_strBtnConnect )
        {
            applySettings();

            //m_pDbClient->connect_();
        }
        else if( m_pBtnConnect->text() == c_strBtnDisconnect )
        {
            //m_pDbClient->disconnect_();
        }
        else if( m_pBtnConnect->text() == c_strBtnAbort )
        {
            m_pDbClient->abortRequestInProgress();
        }
    } // if( m_pDbClient != nullptr )

} // onBtnConnectClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onBtnDetailsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnDetailsClicked",
        /* strAddInfo   */ "" );

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
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContentsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContentsClicked",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pTblViewSrvCltConnection != nullptr )
    {
        m_pTblViewSrvCltConnection->resizeColumnsToContents();
        m_pTblViewSrvCltConnection->resizeRowsToContents();
    }

} // onBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContentsClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientStateChanged(
    CDbClient*        i_pDbClt,
    CDbClient::EState i_state )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", State: " + CDbClient::State2Str(i_state);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pDbClt != nullptr && i_pDbClt == m_pDbClient )
    {
        fillCnctControls(m_protocolTypeWidget, m_hostSettingsWidget, m_watchDogSettingsWidget);

        if( m_bShowDetails )
        {
            fillDetailControls();
        }
    }

} // onDbClientStateChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientRequestInProgressChanged(
    CDbClient*   i_pDbClt,
    SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", Req {" + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientRequestInProgressChanged",
        /* strAddInfo   */ strAddTrcInfo );

    QString strBtnConnect;
    QString strReqInProgress;

    switch( i_reqDscr.m_request )
    {
        //case CDbClient::ERequestConnect:
        //{
        //    if( i_reqDscr.m_iProgress_perCent < 100 )
        //    {
        //        strBtnConnect    = c_strBtnAbort;
        //        strReqInProgress = "Connecting ...";
        //    }
        //    else if( i_reqDscr.m_iProgress_perCent >= 100 )
        //    {
        //        if( m_pDbClient->isConnected() )
        //        {
        //            strBtnConnect = c_strBtnDisconnect;
        //        }
        //        else
        //        {
        //            strBtnConnect = c_strBtnConnect;
        //        }
        //    }
        //    break;
        //}
        //case CDbClient::ERequestDisconnect:
        //{
        //    if( i_reqDscr.m_iProgress_perCent < 100 )
        //    {
        //        strBtnConnect    = c_strBtnAbort;
        //        strReqInProgress = "Disconnecting ...";
        //    }
        //    else if( i_reqDscr.m_iProgress_perCent >= 100 )
        //    {
        //        strBtnConnect = c_strBtnConnect;
        //    }
        //    break;
        //}
        //case CDbClient::ERequestChangeSettings:
        //{
        //    if( i_reqDscr.m_iProgress_perCent < 100 )
        //    {
        //        strBtnConnect    = c_strBtnAbort;
        //        strReqInProgress = "Connecting ...";
        //    }
        //    else if( i_reqDscr.m_iProgress_perCent >= 100 )
        //    {
        //        if( m_pDbClient->isConnected() )
        //        {
        //            strBtnConnect = c_strBtnDisconnect;
        //        }
        //        else
        //        {
        //            strBtnConnect = c_strBtnConnect;
        //        }
        //    }
        //    break;
        //}
        //case CDbClient::ERequestRegister:
        //case CDbClient::ERequestUnregister:
        //case CDbClient::ERequestSelectSchema:
        //case CDbClient::ERequestUpdateSchema:
        //case CDbClient::ERequestSelectData:
        //case CDbClient::ERequestUpdateData:
        //case CDbClient::ERequestConnectAndRegister:
        //case CDbClient::ERequestDisconnectAndUnregister:
        //{
        //    if( i_reqDscr.m_iProgress_perCent < 100 )
        //    {
        //        strBtnConnect    = c_strBtnAbort;
        //        strReqInProgress = "Sending data ...";
        //    }
        //    else if( i_reqDscr.m_iProgress_perCent >= 100 )
        //    {
        //        if( m_pDbClient->isConnected() )
        //        {
        //            strBtnConnect = c_strBtnDisconnect;
        //        }
        //        else
        //        {
        //            strBtnConnect = c_strBtnConnect;
        //        }
        //    }
        //    break;
        //}
        //case CDbClient::ERequestNone:
        //default:
        //{
        //    if( m_pDbClient->isConnected() )
        //    {
        //        strBtnConnect = c_strBtnDisconnect;
        //    }
        //    else
        //    {
        //        strBtnConnect = c_strBtnConnect;
        //    }
        //    break;
        //}
    } // switch( i_reqDscr.m_request )

    if( m_pBtnConnect != nullptr )
    {
        m_pBtnConnect->setText(strBtnConnect);
    }

    if( m_pBarRequestInProgress != nullptr )
    {
        if( strReqInProgress.isEmpty() )
        {
            m_pBarRequestInProgress->stop();
            m_pBarRequestInProgress->reset();
        }
        else if( i_reqDscr.m_iProgress_perCent == 0 )
        {
            m_pBarRequestInProgress->reset();
            m_pBarRequestInProgress->setDurationInMs(m_hostSettingsClient.m_iConnectTimeout_ms);
            m_pBarRequestInProgress->setIncrementInMs(200);
            m_pBarRequestInProgress->start();
        }
        m_pBarRequestInProgress->setLabelText(strReqInProgress);
    }

} // onDbClientRequestInProgressChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientConnected(
    CDbClient*         i_pDbClt,
    const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", SocketDscr {" + i_socketDscr.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientConnected",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientConnected

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientDisconnected(
    CDbClient*         i_pDbClt,
    const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", SocketDscr {" + i_socketDscr.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientDisconnected",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientDisconnected

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientDefaultProtocolTypeChanged(
    CDbClient*    i_pDbClt,
    EProtocolType i_protocolType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientDefaultProtocolTypeChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientDefaultProtocolTypeChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientProtocolTypeChanged(
    CDbClient*    i_pDbClt,
    EProtocolType i_protocolType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientProtocolTypeChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientProtocolTypeChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientProtocolTypeRowVersionChanged( CDbClient* i_pDbClt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientProtocolTypeRowVersionChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientProtocolTypeRowVersionChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientDefaultSocketTypeChanged(
    CDbClient*  i_pDbClt,
    ESocketType i_socketType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", SockType: " + socketType2Str(i_socketType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientDefaultSocketTypeChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientDefaultSocketTypeChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientSocketTypeChanged(
    CDbClient*  i_pDbClt,
    ESocketType i_socketType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", SockType: " + socketType2Str(i_socketType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientSocketTypeChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientSocketTypeChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientSocketTypeRowVersionChanged( CDbClient* i_pDbClt )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientSocketTypeRowVersionChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientSocketTypeRowVersionChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientDefaultHostSettingsChanged(
    CDbClient*                 i_pDbClt,
    EProtocolType              i_protocolType,
    const SClientHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", HostSettings {" + i_hostSettings.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientDefaultHostSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientDefaultHostSettingsChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientHostSettingsChanged(
    CDbClient*                 i_pDbClt,
    EProtocolType              i_protocolType,
    const SClientHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", HostSettings {" + i_hostSettings.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientHostSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pDbClt != nullptr && i_pDbClt == m_pDbClient )
    {
        m_protocolTypeClient     = m_pDbClient->getProtocolType();
        m_hostSettingsClient     = m_pDbClient->getHostSettings();
        m_watchDogSettingsClient = m_pDbClient->getWatchDogSettings();

        if( m_protocolTypeWidget == m_protocolTypeClientDefault && m_hostSettingsWidget == m_hostSettingsClientDefault && m_watchDogSettingsWidget == m_watchDogSettingsClientDefault )
        {
            m_pChkDefault->setChecked(true);
            m_pChkDefault->setEnabled(false);
        }
        else
        {
            m_pChkDefault->setChecked(false);
            m_pChkDefault->setEnabled(true);
        }

        if( m_protocolTypeWidget == m_protocolTypeClient && m_hostSettingsWidget == m_hostSettingsClient && m_watchDogSettingsWidget == m_watchDogSettingsClient )
        {
            m_pBtnReset->setEnabled(false);
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnReset->setEnabled(true);
            m_pBtnApply->setEnabled(true);
        }
    } // if( i_pDbClt != nullptr && i_pDbClt == m_pDbClient )

} // onDbClientHostSettingsChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientHostSettingsRowVersionChanged(
    CDbClient*    i_pDbClt,
    EProtocolType i_protocolType,
    ESocketType   i_socketType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", SockType: " + socketType2Str(i_socketType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientHostSettingsRowVersionChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientHostSettingsRowVersionChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientDefaultWatchDogSettingsChanged(
    CDbClient*            i_pDbClt,
    EProtocolType         i_protocolType,
    ESocketType           i_socketType,
    const STimerSettings& i_watchDogSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", SockType: " + socketType2Str(i_socketType);
        strAddTrcInfo += ", WatchDogSettings {Enabled: " + bool2Str(i_watchDogSettings.m_bEnabled) ;
        strAddTrcInfo += ", Interval: "+ QString::number(i_watchDogSettings.m_iInterval_ms);
        strAddTrcInfo += ", Timeout: " + QString::number(i_watchDogSettings.m_iTimeout_ms) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientDefaultWatchDogSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientDefaultWatchDogSettingsChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientWatchDogSettingsChanged(
    CDbClient*            i_pDbClt,
    EProtocolType         i_protocolType,
    ESocketType           i_socketType,
    const STimerSettings& i_watchDogSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", SockType: " + socketType2Str(i_socketType);
        strAddTrcInfo += ", WatchDogSettings {Enabled: " + bool2Str(i_watchDogSettings.m_bEnabled) ;
        strAddTrcInfo += ", Interval: "+ QString::number(i_watchDogSettings.m_iInterval_ms);
        strAddTrcInfo += ", Timeout: " + QString::number(i_watchDogSettings.m_iTimeout_ms) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientWatchDogSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientWatchDogSettingsChanged

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onDbClientWatchDogSettingsRowVersionChanged(
    CDbClient*    i_pDbClt,
    EProtocolType i_protocolType,
    ESocketType   i_socketType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "DbClient: " + QString( i_pDbClt == nullptr ? "nullptr" : i_pDbClt->objectName() );
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", SockType: " + socketType2Str(i_socketType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbClientWatchDogSettingsRowVersionChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbClientWatchDogSettingsRowVersionChanged

/*==============================================================================
protected slots: // host info lookup
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onRemoteHostInfoLookedUp( const QHostInfo& i_hostInfo )
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
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
QString CWdgtDbClientConnection::getConnectionToolTip()
//------------------------------------------------------------------------------
{
    QString strToolTip;

    if( m_pDbClient != nullptr )
    {
        strToolTip = m_pDbClient->getConnectionString();

        //if( m_pDbClient->getLastConnectErrResultInfo().isErrorResult() )
        //{
        //    if( !strToolTip.isEmpty() )
        //    {
        //        strToolTip += "\n";
        //    }
        //    strToolTip += m_pDbClient->getLastConnectErrResultInfo().getResultStr();
        //    strToolTip += ": " + m_pDbClient->getLastConnectErrResultInfo().getAddErrInfoDscr();
        //}
    }

    return strToolTip;

} // getConnectionToolTip

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::onShowDetailsChanged()
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

            QLayoutItem* pLytItemStretch = m_pLyt->itemAt(m_idxLytItemStretch);

            if( pLytItemStretch != nullptr )
            {
                m_pLyt->removeItem(pLytItemStretch);
            }

            m_pBtnDetails->setText(c_strBtnDetailsHideDetails);

            // <Widget> Details
            //----------------------

            m_pWdgtDetailsStates = new QWidget();
            m_pLytDetailsStates = new QVBoxLayout();
            m_pLytDetailsStates->setContentsMargins(0, 0, 0, 0); // left, top, right, bottom
            m_pWdgtDetailsStates->setLayout(m_pLytDetailsStates);
            m_pLytDetails->addWidget(m_pWdgtDetailsStates);

            // <Separator>
            //-------------

            m_pLytDetailsStates->addSpacing(5);

            pLine = new CSepLine();
            m_pLytDetailsStates->addWidget(pLine);

            m_pLytDetailsStates->addSpacing(5);

            // <Label> Applied Connection Settings
            //------------------------------------

            QHBoxLayout* pLytDetailsConnectionSettingsApplied = new QHBoxLayout();
            m_pLytDetailsStates->addLayout(pLytDetailsConnectionSettingsApplied);
            m_pLblDetailsConnectionSettingsApplied = new QLabel("Connection String:");
            m_pLblDetailsConnectionSettingsApplied->setFixedWidth(m_cxLblWidthClm1);
            pLytDetailsConnectionSettingsApplied->addWidget(m_pLblDetailsConnectionSettingsApplied);
            m_pEdtDetailsConnectionSettingsApplied = new QLineEdit("");
            m_pEdtDetailsConnectionSettingsApplied->setEnabled(false);
            //m_pEdtDetailsConnectionSettingsApplied->setReadOnly(true);
            pLytDetailsConnectionSettingsApplied->addWidget(m_pEdtDetailsConnectionSettingsApplied);

            // <Label> State
            //----------------------

            QHBoxLayout* pLytDetailsState = new QHBoxLayout();
            m_pLytDetailsStates->addLayout(pLytDetailsState);
            m_pLblDetailsState = new QLabel("Current State:");
            m_pLblDetailsState->setFixedWidth(m_cxLblWidthClm1);
            pLytDetailsState->addWidget(m_pLblDetailsState);
            m_pEdtDetailsState = new QLineEdit("");
            m_pEdtDetailsState->setEnabled(false);
            pLytDetailsState->addWidget(m_pEdtDetailsState);

            // <Label> Request In Progress
            //----------------------------

            QHBoxLayout* pLytDetailsRequestInProgress = new QHBoxLayout();
            m_pLytDetailsStates->addLayout(pLytDetailsRequestInProgress);
            m_pLblRequestInProgress = new QLabel("Request in Progress:");
            m_pLblRequestInProgress->setFixedWidth(m_cxLblWidthClm1);
            pLytDetailsRequestInProgress->addWidget(m_pLblRequestInProgress);
            m_pBarRequestInProgress = new CProgressBar();
            m_pBarRequestInProgress->setTextVisible(false);
            m_pBarRequestInProgress->setEnabled(false);
            pLytDetailsRequestInProgress->addWidget(m_pBarRequestInProgress);

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

            m_pLytLineConnection = new QHBoxLayout();
            m_pLytDetailsStates->addLayout(m_pLytLineConnection);

            // <Label> Active Connections
            //----------------------------

            m_pLblActiveConnection = new QLabel("Active Connection:");
            m_pLytLineConnection->addWidget(m_pLblActiveConnection);

            // <Button> Resize Columns and Columns to Contents
            //------------------------------------------------

            m_pBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContents = new QPushButton("<->");
            m_pBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContents->setFixedWidth(30);
            m_pLytLineConnection->addWidget(m_pBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContents);

            if( !QObject::connect(
                /* pObjSender   */ m_pBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContents,
                /* szSignal     */ SIGNAL(clicked(bool)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContentsClicked(bool)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            // <Stretch> Active Connections
            //-----------------------------

            m_pLytLineConnection->addStretch();

            // Table to indicate current connection
            //----------------------------------------

            m_pWdgtSrvCltConnection = new QWidget();
            m_pLytSrvCltConnection = new QVBoxLayout();
            m_pLytSrvCltConnection->setContentsMargins(0, 0, 0, 0); // left, top, right, bottom
            m_pWdgtSrvCltConnection->setLayout(m_pLytSrvCltConnection);
            m_pLytDetailsStates->addWidget(m_pWdgtSrvCltConnection);

            m_pModelSrvCltConnection = new CModelDbConnections(this);

            if( m_pDbClient != nullptr )
            {
                m_pModelSrvCltConnection->setDbClient(m_pDbClient);
                m_pModelSrvCltConnection->setProtocolType(m_pDbClient->getProtocolType());
                m_pModelSrvCltConnection->setSocketType(m_pDbClient->getSocketType());
                m_pModelSrvCltConnection->update();
            }

            m_pTblViewSrvCltConnection = new QTableView();
            m_pTblViewSrvCltConnection->setModel(m_pModelSrvCltConnection);
            m_pTblViewSrvCltConnection->setSelectionBehavior(QAbstractItemView::SelectItems);
            m_pTblViewSrvCltConnection->setSelectionMode(QAbstractItemView::SingleSelection);
            m_pTblViewSrvCltConnection->setEditTriggers(QAbstractItemView::NoEditTriggers);

            if( m_pDbClient != nullptr && m_pDbClient->getSocketType() == ESocketTypeInProcMsg )
            {
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnSocketId);
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnSocketState);
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnServerListenPort);
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnLocalPort);
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnRemoteHostName);
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnRemotePort);
            }
            else
            {
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnSocketId);
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnSocketState);
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnServerListenPort);
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnLocalPort);
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnRemoteHostName);
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnRemotePort);
            }
            m_pTblViewSrvCltConnection->resizeColumnsToContents();
            m_pTblViewSrvCltConnection->resizeRowsToContents();
            m_pLytSrvCltConnection->addWidget(m_pTblViewSrvCltConnection);

            m_pLytSrvCltConnection->addSpacing(10);

            // <MessageLog>
            //--------------

            m_pWdgtMsgLog = new QWidget();
            m_pLytMsgLog = new QVBoxLayout();
            m_pLytMsgLog->setContentsMargins(0, 0, 0, 0); // left, top, right, bottom
            m_pWdgtMsgLog->setLayout(m_pLytMsgLog);
            m_pLytDetailsStates->addWidget(m_pWdgtMsgLog);

            m_pLblMsgLog = new QLabel( "Message Protocol:" );
            m_pLytMsgLog->addWidget(m_pLblMsgLog);

            m_pWdgtTrcMsgLog = new CWdgtDbClientTrcMsgLog(m_trcMsgLogSettings);
            m_pLytDetailsStates->addWidget(m_pWdgtTrcMsgLog);

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

        // All children of the details states widget have also been deleted:
        m_pLytDetailsStates = nullptr;
        m_pLblDetailsConnectionSettingsApplied = nullptr;
        m_pEdtDetailsConnectionSettingsApplied = nullptr;
        m_pLblDetailsState = nullptr;
        m_pEdtDetailsState = nullptr;
        m_pLblRequestInProgress = nullptr;
        m_pBarRequestInProgress = nullptr;
        m_pLytLineConnection = nullptr;
        m_pLblActiveConnection = nullptr;
        m_pWdgtSrvCltConnection = nullptr;
        m_pLytSrvCltConnection = nullptr;
        m_pModelSrvCltConnection = nullptr;
        m_pTblViewSrvCltConnection = nullptr;
        m_pWdgtMsgLog = nullptr;
        m_pLytMsgLog = nullptr;
        m_pLblMsgLog = nullptr;
        m_pWdgtTrcMsgLog = nullptr;

        m_idxLytItemStretch = m_pLyt->count();
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
void CWdgtDbClientConnection::resetCnctControls()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "resetCnctControls",
        /* strAddInfo   */ strAddTrcInfo );

    // <ComboBox> Data Protocol
    //-------------------------

    if( m_pLblProtocolType != nullptr && m_pCmbProtocolType != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pCmbProtocolType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbProtocolTypeCurrentIndexChanged(int)) );

        m_pCmbProtocolType->clear();
        m_pCmbProtocolType->setEnabled(false);
        m_pCmbProtocolType->show();
        m_pLblProtocolType->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pCmbProtocolType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbProtocolTypeCurrentIndexChanged(int)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblProtocolType != nullptr && m_pCmbProtocolType != nullptr )

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

    // <CheckBox> Default
    //-------------------

    if( m_pChkDefault != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pChkDefault,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDefaultToggled(bool)) );

        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(true);

        if( !QObject::connect(
            /* pObjSender   */ m_pChkDefault,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDefaultToggled(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pChkDefault != nullptr )

    // <Led> State
    //------------

    if( m_pLedState != nullptr )
    {
        m_pLedState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
        m_pLedState->setEnabled(false);

    } // if( m_pLedState != nullptr )

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

    // <CheckBox><LineEdit> Database Name
    //-----------------------------------

    if( m_pLblDatabaseName != nullptr && m_pEdtDatabaseName != nullptr && m_pChkDatabaseNameUsed != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtDatabaseName,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtDatabaseNameTextChanged(const QString&)) );
        QObject::disconnect(
            /* pObjSender   */ m_pChkDatabaseNameUsed,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDatabaseNameUsedToggled(bool)) );

        m_pEdtDatabaseName->setText("");
        m_pEdtDatabaseName->setEnabled(false);
        m_pEdtDatabaseName->show();
        m_pChkDatabaseNameUsed->setChecked(false);
        m_pChkDatabaseNameUsed->setEnabled(false);
        m_pChkDatabaseNameUsed->show();
        m_pLblDatabaseName->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtDatabaseName,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtDatabaseNameTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pChkDatabaseNameUsed,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDatabaseNameUsedToggled(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblDatabaseName != nullptr && m_pEdtDatabaseName != nullptr && m_pChkDatabaseNameUsed != nullptr )

    // <CheckBox><LineEdit> User Name
    //-----------------------------------

    if( m_pLblUserName != nullptr && m_pEdtUserName != nullptr && m_pChkUserNameUsed != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtUserName,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtUserNameTextChanged(const QString&)) );
        QObject::disconnect(
            /* pObjSender   */ m_pChkUserNameUsed,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkUserNameUsedToggled(bool)) );

        m_pEdtUserName->setText("");
        m_pEdtUserName->setEnabled(false);
        m_pEdtUserName->show();
        m_pChkUserNameUsed->setChecked(false);
        m_pChkUserNameUsed->setEnabled(false);
        m_pChkUserNameUsed->show();
        m_pLblUserName->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtUserName,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtUserNameTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pChkUserNameUsed,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkUserNameUsedToggled(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblUserName != nullptr && m_pEdtUserName != nullptr && m_pChkUserNameUsed != nullptr )

    // <CheckBox><LineEdit> User Name
    //-----------------------------------

    if( m_pLblUserPassword != nullptr && m_pEdtUserPassword != nullptr && m_pChkUserPasswordUsed != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtUserPassword,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtUserPasswordTextChanged(const QString&)) );
        QObject::disconnect(
            /* pObjSender   */ m_pChkUserPasswordUsed,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkUserPasswordUsedToggled(bool)) );

        m_pEdtUserPassword->setText("");
        m_pEdtUserPassword->setEnabled(false);
        m_pEdtUserPassword->show();
        m_pChkUserPasswordUsed->setChecked(false);
        m_pChkUserPasswordUsed->setEnabled(false);
        m_pChkUserPasswordUsed->show();
        m_pLblUserPassword->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtUserPassword,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtUserPasswordTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pChkUserPasswordUsed,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkUserPasswordUsedToggled(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblUserPassword != nullptr && m_pEdtUserPassword != nullptr && m_pChkUserPasswordUsed != nullptr )

    // <CheckBox> Watch Dog Timer Enabled
    //-----------------------------------

    if( m_pWdgtWatchDogTimerEnabled != nullptr && m_pChkWatchDogTimerEnabled != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pChkWatchDogTimerEnabled,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkWatchDogTimerEnabledToggled(bool)) );

        m_pChkWatchDogTimerEnabled->setChecked(false);
        m_pChkWatchDogTimerEnabled->setEnabled(false);
        m_pWdgtWatchDogTimerEnabled->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pChkWatchDogTimerEnabled,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkWatchDogTimerEnabledToggled(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pWdgtWatchDogTimerEnabled != nullptr && m_pChkWatchDogTimerEnabled != nullptr )

    // <LineEdit> Watch Dog Timer Interval
    //------------------------------------

    if( m_pWdgtWatchDogTimerInterval != nullptr && m_pEdtWatchDogTimerInterval_ms != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtWatchDogTimerInterval_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimerIntervalTextChanged(const QString&)) );

        m_pEdtWatchDogTimerInterval_ms->setValidator(nullptr);
        m_pEdtWatchDogTimerInterval_ms->setText("");
        m_pEdtWatchDogTimerInterval_ms->setEnabled(false);
        m_pWdgtWatchDogTimerInterval->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtWatchDogTimerInterval_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimerIntervalTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pWdgtWatchDogTimerInterval != nullptr && m_pEdtWatchDogTimerInterval_ms != nullptr )

    // <LineEdit> Watch Dog Timeout
    //-----------------------------

    if( m_pWdgtWatchDogTimeout != nullptr && m_pEdtWatchDogTimeout_ms != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtWatchDogTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimeoutTextChanged(const QString&)) );

        m_pEdtWatchDogTimeout_ms->setValidator(nullptr);
        m_pEdtWatchDogTimeout_ms->setText("");
        m_pEdtWatchDogTimeout_ms->setEnabled(false);
        m_pWdgtWatchDogTimeout->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtWatchDogTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtWatchDogTimeoutTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pWdgtWatchDogTimeout != nullptr && m_pEdtWatchDogTimeout_ms != nullptr )

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

    m_protocolTypeWidget     = EProtocolTypeUndefined;
    m_hostSettingsWidget     = SClientHostSettings();
    m_watchDogSettingsWidget = STimerSettings();

} // resetCnctControls

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::fillCnctControls(
    EProtocolType              i_protocolType,
    const SClientHostSettings& i_old_hostSettings,
    const STimerSettings&      i_watchDogSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ProtocolType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", HostSettings {" + i_old_hostSettings.getConnectionString(iAddTrcInfoDetailLevel) + "}";
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

    bool    bDbClientConnected = false;
    QString strConnection;
    bool    bDatabaseNameUsed = false;
    bool    bUserNameUsed = false;
    bool    bPasswordUsed = false;
    int     idxCmb;

    if( m_pDbClient != nullptr )
    {
        bDbClientConnected  = m_pDbClient->isConnected();
        strConnection       = m_pDbClient->getConnectionString();
    }

    // <ComboBox> Data Protocol
    //-------------------------

    if( m_pWdgtProtocolType != nullptr && m_pCmbProtocolType != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pCmbProtocolType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbProtocolTypeCurrentIndexChanged(int)) );

        //for( idxCmb = 0; idxCmb < m_pDbClient->getProtocolTypesCount(); idxCmb++ )
        //{
        //    EProtocolType protocolType = m_pDbClient->getProtocolType(idxCmb);
        //    m_pCmbProtocolType->addItem( protocolType2Str(protocolType), protocolType );
        //}

        idxCmb = m_pCmbProtocolType->findData(i_protocolType);
        m_pCmbProtocolType->setCurrentIndex(idxCmb);

        if( m_pCmbProtocolType->count() > 1 )
        {
            m_pCmbProtocolType->setEnabled(true);
        }
        else
        {
            m_pCmbProtocolType->setEnabled(false);
        }

        if( m_pDbClient->isProtocolTypeChangeable() )
        {
            m_pWdgtProtocolType->show();
        }
        else
        {
            m_pWdgtProtocolType->hide();
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pCmbProtocolType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbProtocolTypeCurrentIndexChanged(int)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pWdgtProtocolType != nullptr && m_pCmbProtocolType != nullptr )

    // <ComboBox> SQL Drivers
    //-----------------------

    if( m_pWdgtSQLDrivers != nullptr && m_pCmbSQLDrivers != nullptr )
    {
        if( i_protocolType == EProtocolTypeSQL )
        {
            m_pCmbSQLDrivers->setEnabled(true);
            m_pWdgtSQLDrivers->show();
        }
        else
        {
            m_pCmbSQLDrivers->setEnabled(false);
            m_pWdgtSQLDrivers->hide();
        }
    } // if( m_pWdgtSQLDrivers != nullptr && m_pCmbSQLDrivers != nullptr )

    // <ComboBox> Socket Type
    //-----------------------

    if( m_pWdgtSocketType != nullptr && m_pCmbSocketType != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pCmbSocketType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) );

        //for( idxCmb = 0; idxCmb < m_pDbClient->getSocketTypesCount(i_protocolType); idxCmb++ )
        //{
        //    ESocketType socketType = m_pDbClient->getSocketType(idxCmb, i_protocolType);
        //    m_pCmbSocketType->addItem( socketType2Str(socketType), socketType );
        //}

        idxCmb = m_pCmbSocketType->findData(i_old_hostSettings.m_socketType);
        m_pCmbSocketType->setCurrentIndex(idxCmb);

        if( m_pCmbSocketType->count() > 1 )
        {
            m_pCmbSocketType->setEnabled(true);
        }
        else
        {
            m_pCmbSocketType->setEnabled(false);
        }

        if( m_pDbClient->isSocketTypeChangeable(i_protocolType) )
        {
            m_pWdgtSocketType->show();
        }
        else
        {
            m_pWdgtSocketType->hide();
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pCmbSocketType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pWdgtSocketType != nullptr && m_pCmbSocketType != nullptr )

    // <CheckBox> Default
    //-------------------

    if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pChkDefault,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDefaultToggled(bool)) );

        if( i_protocolType == m_protocolTypeClientDefault
         && i_old_hostSettings == m_hostSettingsClientDefault
         && i_watchDogSettings == m_watchDogSettingsClientDefault )
        {
            m_pChkDefault->setChecked(true);
            m_pChkDefault->setEnabled(false);
        }
        else
        {
            m_pChkDefault->setChecked(false);
            m_pChkDefault->setEnabled(true);
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pChkDefault,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDefaultToggled(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )

    // <LED> State
    //------------

    if( m_pLblState != nullptr && m_pLedState != nullptr )
    {
        if( bDbClientConnected )
        {
            m_pLedState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
        }
        else // if( !bDbClientConnected )
        {
            m_pLedState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
        }
        m_pLedState->setEnabled(true);

    } // if( m_pLblState != nullptr && m_pLedState != nullptr )

    // <LineEdit> Remote Host Name
    //-----------------------------

    if( m_pEdtRemoteHostName != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtRemoteHostName,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRemoteHostNameTextChanged(const QString&)) );

        m_pEdtRemoteHostName->setText(i_old_hostSettings.m_strRemoteHostName);

        if( i_old_hostSettings.m_socketType == Ipc::ESocketTypeInProcMsg )
        {
            m_pEdtRemoteHostName->setEnabled(false);
        }
        else
        {
            m_pEdtRemoteHostName->setEnabled(true);
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

    if( m_pWdgtRemoteHostAddr != nullptr && m_pEdtRemoteHostAddr != nullptr )
    {
        if( i_old_hostSettings.m_socketType == ESocketTypeTcp )
        {
            if( !m_bRemoteHostInfoLookUpInProcess )
            {
                m_pEdtRemoteHostAddr->setText("---");
                m_bRemoteHostInfoLookUpInProcess = true;
                QHostInfo::lookupHost( i_old_hostSettings.m_strRemoteHostName, this, SLOT(onRemoteHostInfoLookedUp(const QHostInfo&)) );
            }
            m_pWdgtRemoteHostAddr->show();
        }
        else
        {
            m_pWdgtRemoteHostAddr->hide();
        }
    } // if( m_pWdgtRemoteHostAddr != nullptr && m_pEdtRemoteHostAddr != nullptr )

    // <LineEdit> Remote Host Port
    //----------------------------

    if( m_pWdgtRemotePort != nullptr && m_pEdtRemotePort != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtRemotePort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRemotePortTextChanged(const QString&)) );

        if( i_old_hostSettings.m_socketType == ESocketTypeInProcMsg )
        {
            m_pEdtRemotePort->setValidator(nullptr);
            m_pEdtRemotePort->setText("");
            m_pEdtRemotePort->setEnabled(false);
            m_pWdgtRemotePort->hide();
        }
        else
        {
            m_pEdtRemotePort->setValidator( new QIntValidator(1,65535,this) );
            m_pEdtRemotePort->setText( QString::number(i_old_hostSettings.m_uRemotePort) );
            m_pEdtRemotePort->setEnabled(true);
            m_pWdgtRemotePort->show();
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtRemotePort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtRemotePortTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pWdgtRemotePort != nullptr && m_pEdtRemotePort != nullptr )

    // <LineEdit> Connect Timeout
    //-----------------------------

    if( m_pEdtConnectTimeout_ms != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtConnectTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtConnectTimeoutTextChanged(const QString&)) );

        m_pEdtConnectTimeout_ms->setValidator( new QIntValidator(0,60000,this) );
        m_pEdtConnectTimeout_ms->setText( QString::number(i_old_hostSettings.m_iConnectTimeout_ms) );
        m_pEdtConnectTimeout_ms->setEnabled(true);

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtConnectTimeout_ms,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtConnectTimeoutTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pEdtConnectTimeout_ms != nullptr )

    // <LineEdit> Buffer Size (for shared memory socket connections)
    //--------------------------------------------------------------

    if( m_pWdgtBufferSize != nullptr && m_pEdtBufferSize_bytes != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtBufferSize_bytes,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtBufferSizeTextChanged(const QString&)) );

        if( i_old_hostSettings.m_socketType == ESocketTypeShm )
        {
            m_pEdtBufferSize_bytes->setValidator( new QIntValidator(1024,65536,this) );
            m_pEdtBufferSize_bytes->setText( QString::number(i_old_hostSettings.m_uBufferSize) );
            m_pEdtBufferSize_bytes->setEnabled(true);
            m_pWdgtBufferSize->show();
        }
        else
        {
            m_pEdtBufferSize_bytes->setValidator(nullptr);
            m_pEdtBufferSize_bytes->setText("");
            m_pEdtBufferSize_bytes->setEnabled(false);
            m_pWdgtBufferSize->hide();
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtBufferSize_bytes,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtBufferSizeTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pWdgtBufferSize != nullptr && m_pEdtBufferSize_bytes != nullptr )

    // <Widget> User Login
    //--------------------

    if( m_pWdgtLineUserLogin != nullptr )
    {
        if( i_protocolType == EProtocolTypeSQL )
        {
            // <CheckBox><LineEdit> Database Name
            //-----------------------------------

            if( m_pEdtDatabaseName != nullptr && m_pChkDatabaseNameUsed != nullptr )
            {
                QObject::disconnect(
                    /* pObjSender   */ m_pChkDatabaseNameUsed,
                    /* szSignal     */ SIGNAL(toggled(bool)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onChkDatabaseNameUsedToggled(bool)) );

                m_pChkDatabaseNameUsed->setChecked(bDatabaseNameUsed);
                m_pChkDatabaseNameUsed->setEnabled(true);

                if( bDatabaseNameUsed )
                {
                    m_pEdtDatabaseName->setEnabled(true);
                }
                else
                {
                    m_pEdtDatabaseName->setEnabled(false);
                }

                if( !QObject::connect(
                    /* pObjSender   */ m_pChkDatabaseNameUsed,
                    /* szSignal     */ SIGNAL(toggled(bool)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onChkDatabaseNameUsedToggled(bool)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            } // if( m_pEdtDatabaseName != nullptr && m_pChkDatabaseNameUsed != nullptr )

            // <CheckBox><LineEdit> User Name
            //-----------------------------------

            if( m_pEdtUserName != nullptr && m_pChkUserNameUsed != nullptr )
            {
                QObject::disconnect(
                    /* pObjSender   */ m_pChkUserNameUsed,
                    /* szSignal     */ SIGNAL(toggled(bool)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onChkUserNameUsedToggled(bool)) );

                m_pChkUserNameUsed->setChecked(bUserNameUsed);
                m_pChkUserNameUsed->setEnabled(true);

                if( bUserNameUsed )
                {
                    m_pEdtUserName->setEnabled(true);
                }
                else
                {
                    m_pEdtUserName->setEnabled(false);
                }

                if( !QObject::connect(
                    /* pObjSender   */ m_pChkUserNameUsed,
                    /* szSignal     */ SIGNAL(toggled(bool)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onChkUserNameUsedToggled(bool)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            } // if(m_pEdtUserName != nullptr && m_pChkUserNameUsed != nullptr )

            // <CheckBox><LineEdit> User Password
            //-----------------------------------

            if( m_pEdtUserPassword != nullptr && m_pChkUserPasswordUsed != nullptr )
            {
                QObject::disconnect(
                    /* pObjSender   */ m_pChkUserPasswordUsed,
                    /* szSignal     */ SIGNAL(toggled(bool)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onChkUserPasswordUsedToggled(bool)) );

                m_pChkUserPasswordUsed->setChecked(bPasswordUsed);
                m_pChkUserPasswordUsed->setEnabled(true);

                if( bPasswordUsed )
                {
                    m_pEdtUserPassword->setEnabled(true);
                }
                else
                {
                    m_pEdtUserPassword->setEnabled(false);
                }

                if( !QObject::connect(
                    /* pObjSender   */ m_pChkUserPasswordUsed,
                    /* szSignal     */ SIGNAL(toggled(bool)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onChkUserPasswordUsedToggled(bool)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            } // if( m_pEdtUserPassword != nullptr && m_pChkUserPasswordUsed != nullptr )

            m_pWdgtLineUserLogin->show();
        }
        else // if( m_protocolType != EProtocolTypeSQL )
        {
            m_pWdgtLineUserLogin->hide();
        }
    } // if( m_pWdgtLineUserLogin != nullptr )

    // <Widget> Watch Dog Timer
    //-------------------------

    if( m_pWdgtLineWatchDogTimer != nullptr )
    {
        if( i_old_hostSettings.m_socketType != ESocketTypeInProcMsg )
        {
            // <CheckBox> Watch Dog Timer Enabled
            //-----------------------------------

            if( m_pChkWatchDogTimerEnabled != nullptr )
            {
                QObject::disconnect(
                    /* pObjSender   */ m_pChkWatchDogTimerEnabled,
                    /* szSignal     */ SIGNAL(toggled(bool)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onChkWatchDogTimerEnabledToggled(bool)) );

                if( i_old_hostSettings.m_socketType == ESocketTypeInProcMsg )
                {
                    m_pChkWatchDogTimerEnabled->setChecked(false);
                    m_pChkWatchDogTimerEnabled->setEnabled(false);
                }
                else
                {
                    m_pChkWatchDogTimerEnabled->setChecked(i_watchDogSettings.m_bEnabled);
                    m_pChkWatchDogTimerEnabled->setEnabled(true);
                }

                if( !QObject::connect(
                    /* pObjSender   */ m_pChkWatchDogTimerEnabled,
                    /* szSignal     */ SIGNAL(toggled(bool)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onChkWatchDogTimerEnabledToggled(bool)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            } // if( m_pLblWatchDogTimer != nullptr && m_pChkWatchDogTimerEnabled != nullptr )

            // <LineEdit> Watch Dog Timer Interval
            //------------------------------------

            if( m_pEdtWatchDogTimerInterval_ms != nullptr )
            {
                QObject::disconnect(
                    /* pObjSender   */ m_pEdtWatchDogTimerInterval_ms,
                    /* szSignal     */ SIGNAL(textChanged(const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onEdtWatchDogTimerIntervalTextChanged(const QString&)) );

                if( i_old_hostSettings.m_socketType == ESocketTypeInProcMsg )
                {
                    m_pEdtWatchDogTimerInterval_ms->setValidator(nullptr);
                    m_pEdtWatchDogTimerInterval_ms->setText("");
                    m_pEdtWatchDogTimerInterval_ms->setEnabled(false);
                }
                else
                {
                    m_pEdtWatchDogTimerInterval_ms->setValidator( new QIntValidator(14,60000,this) );
                    m_pEdtWatchDogTimerInterval_ms->setText( QString::number(i_watchDogSettings.m_iInterval_ms) );
                    m_pEdtWatchDogTimerInterval_ms->setEnabled(true);
                }

                if( !QObject::connect(
                    /* pObjSender   */ m_pEdtWatchDogTimerInterval_ms,
                    /* szSignal     */ SIGNAL(textChanged(const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onEdtWatchDogTimerIntervalTextChanged(const QString&)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            } // if( m_pEdtWatchDogTimerInterval_ms != nullptr )

            // <LineEdit> Watch Dog Timeout
            //-----------------------------

            if( m_pEdtWatchDogTimeout_ms != nullptr )
            {
                QObject::disconnect(
                    /* pObjSender   */ m_pEdtWatchDogTimeout_ms,
                    /* szSignal     */ SIGNAL(textChanged(const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onEdtWatchDogTimeoutTextChanged(const QString&)) );

                if( i_old_hostSettings.m_socketType == ESocketTypeInProcMsg )
                {
                    m_pEdtWatchDogTimeout_ms->setValidator(nullptr);
                    m_pEdtWatchDogTimeout_ms->setText("");
                    m_pEdtWatchDogTimeout_ms->setEnabled(false);
                }
                else
                {
                    m_pEdtWatchDogTimeout_ms->setValidator( new QIntValidator(14,60000,this) );
                    m_pEdtWatchDogTimeout_ms->setText( QString::number(i_watchDogSettings.m_iTimeout_ms) );
                    m_pEdtWatchDogTimeout_ms->setEnabled(true);
                }

                if( !QObject::connect(
                    /* pObjSender   */ m_pEdtWatchDogTimeout_ms,
                    /* szSignal     */ SIGNAL(textChanged(const QString&)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onEdtWatchDogTimeoutTextChanged(const QString&)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            } // if( m_pEdtWatchDogTimeout_ms != nullptr )

            m_pWdgtLineWatchDogTimer->show();
        }
        else // if( i_old_hostSettings.m_socketType == ESocketTypeInProcMsg )
        {
            m_pWdgtLineWatchDogTimer->hide();
        }
    } // if( m_pWdgtLineWatchDogTimer != nullptr )

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
        if( bDbClientConnected )
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

    m_protocolTypeWidget     = i_protocolType;
    m_hostSettingsWidget     = i_old_hostSettings;
    m_watchDogSettingsWidget = i_watchDogSettings;

} // fillCnctControls

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::resetDetailControls()
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

} // resetDetailControls

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::fillDetailControls()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "fillDetailControls",
        /* strAddInfo   */ "" );

    bool    bDbCltConnected = false;
    QString strDbCltState;
    QString strRequestInProgress;
    QString strConnection;

    if( m_pDbClient != nullptr )
    {
        bDbCltConnected = m_pDbClient->isConnected();
        strDbCltState = m_pDbClient->state2Str();
        strRequestInProgress = m_pDbClient->requestInProgress2Str(false);
        strConnection = m_pDbClient->getConnectionString();
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
            else if( bDbCltConnected )
            {
                m_pEdtDetailsConnectionSettingsApplied->setStyleSheet("QLineEdit { color: green; }");
            }
            else
            {
                m_pEdtDetailsConnectionSettingsApplied->setStyleSheet("QLineEdit { color: red; }");
            }
            m_pEdtDetailsConnectionSettingsApplied->setText(strConnection);
            m_pEdtDetailsConnectionSettingsApplied->setToolTip(strConnection);
            m_pLblDetailsConnectionSettingsApplied->show();
            m_pEdtDetailsConnectionSettingsApplied->show();

        } // if( m_hostSettingsWidget.m_socketType != ESocketTypeInProcMsg )
    } // if( m_pEdtDetailsConnectionSettingsApplied != nullptr )

    if( m_pEdtDetailsState != nullptr )
    {
        if( bDbCltConnected )
        {
            m_pEdtDetailsState->setStyleSheet("QLineEdit { color: green; }");
            m_pEdtDetailsState->setText(strDbCltState);
        }
        else // if( !bDbCltConnected )
        {
            m_pEdtDetailsState->setStyleSheet("QLineEdit { color: red; }");
            m_pEdtDetailsState->setText(strDbCltState);
        }
    } // if( m_pEdtDetailsState != nullptr )

    if( m_pModelSrvCltConnection != nullptr )
    {
        bool bUpdateModel = false;

        if( m_pModelSrvCltConnection->getDbClient() != m_pDbClient )
        {
            m_pModelSrvCltConnection->setDbClient(m_pDbClient);
            bUpdateModel = true;
        }
        if( m_pModelSrvCltConnection->getProtocolType() != m_protocolTypeWidget )
        {
            m_pModelSrvCltConnection->setProtocolType(m_protocolTypeWidget);
            bUpdateModel = true;
        }
        if( m_pModelSrvCltConnection->getSocketType() != m_hostSettingsWidget.m_socketType )
        {
            m_pModelSrvCltConnection->setSocketType(m_hostSettingsWidget.m_socketType);
            bUpdateModel = true;
        }
        if( bUpdateModel )
        {
            m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnProtocolType);
            m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnSocketType);

            if( m_hostSettingsWidget.m_socketType == ESocketTypeUndefined || m_hostSettingsWidget.m_socketType == ESocketTypeInProcMsg )
            {
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnSocketId);
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnSocketState);
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnServerListenPort);
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnLocalPort);
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnRemoteHostName);
                m_pTblViewSrvCltConnection->hideColumn(CModelDbConnections::EColumnRemotePort);
            }
            else
            {
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnSocketId);
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnSocketState);
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnServerListenPort);
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnLocalPort);
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnRemoteHostName);
                m_pTblViewSrvCltConnection->showColumn(CModelDbConnections::EColumnRemotePort);
            }
            m_pModelSrvCltConnection->update();
        }
    } // if( m_pModelSrvCltConnection != nullptr )

} // fillDetailControls

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientConnection::applySettings( )
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

    if( m_pDbClient != nullptr )
    {
        bool bHasChanges = false;

        if( m_protocolTypeWidget != m_protocolTypeClient )
        {
            bHasChanges = true;
            m_pDbClient->setProtocolType(m_protocolTypeWidget);
        }
        if( m_hostSettingsWidget != m_hostSettingsClient )
        {
            bHasChanges = true;
            m_pDbClient->setHostSettings(m_hostSettingsWidget);
        }
        if( m_watchDogSettingsWidget != m_watchDogSettingsClient )
        {
            bHasChanges = true;
            m_pDbClient->setWatchDogSettings(m_watchDogSettingsWidget);
        }

        // In order for the changed settings to take affect:
        if( bHasChanges )
        {
            m_pDbClient->updateHostSettings();
        }
    } // if( m_pDbClient != nullptr )

} // applySettings
