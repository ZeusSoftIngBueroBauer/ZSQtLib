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

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qformlayout.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qmessagebox.h>
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
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSDataGUI/ZSDatabaseConnectionsWdgt.h"
#include "ZSDataGUI/ZSDatabaseConnectionsModel.h"
#include "ZSDataGUI/ZSDatabaseTrcMsgLogWdgt.h"
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
class CWdgtDbConnections : public QWidget
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
CWdgtDbConnections::CWdgtDbConnections( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDb(nullptr),
    m_cxSpacing(30),
    m_cxLblWidthClm1(100),
    m_cxEdtWidthClm1(160),
    m_cxLblWidthClm2(100),
    m_cxEdtWidthClm2(160),
    m_cxLblWidthClm3(80),
    m_cxEdtWidthClm3(160),
    m_pLyt(nullptr),
    // Connection Settings
    m_protTypeWidget(EProtocolTypeUndefined),
    m_hostSettingsWidget(),
    m_hostSettingsServerDefault(),
    m_pLytLineType(nullptr),
    m_pWdgtProtocolType(nullptr),
    m_pLytProtocolType(nullptr),
    m_pLblProtocolType(nullptr),
    m_pCmbProtocolType(nullptr),
    m_pWdgtSocketType(nullptr),
    m_pLytSocketType(nullptr),
    m_pLblSocketType(nullptr),
    m_pCmbSocketType(nullptr),
    m_pLblDefault(nullptr),
    m_pChkDefault(nullptr),
    m_pLblState(nullptr),
    m_pLedState(nullptr),
    m_pLytLineLocalHost(nullptr),
    m_pWdgtLocalHostName(nullptr),
    m_pLytLocalHostName(nullptr),
    m_pLblLocalHostName(nullptr),
    m_pEdtLocalHostName(nullptr),
    m_pWdgtLocalHostAddr(nullptr),
    m_pLytLocalHostAddr(nullptr),
    m_pLblLocalHostAddr(nullptr),
    m_pEdtLocalHostAddr(nullptr),
    m_bLocalHostInfoLookUpInProcess(false),
    m_pWdgtListenPort(nullptr),
    m_pLytListenPort(nullptr),
    m_pLblListenPort(nullptr),
    m_pEdtListenPort(nullptr),
    // Buffer Size (for Shm Socket Types only)
    m_pWdgtLineBufferSize(nullptr),
    m_pLytLineBufferSize(nullptr),
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
    m_pBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContents(nullptr),
    m_pWdgtSrvCltConnections(nullptr),
    m_pLytSrvCltConnections(nullptr),
    m_pModelSrvCltConnections(nullptr),
    m_pDelegateSrvCltConnections(nullptr),
    m_pTblViewSrvCltConnections(nullptr),
    // Protocol Trace
    m_pWdgtMsgLog(nullptr),
    m_pLytMsgLog(nullptr),
    m_pLblMsgLog(nullptr),
    m_bTrcMsgLogEnabled(false),
    m_uTrcMsgLogMaxItems(1000),
    m_pWdgtTrcMsgLog(nullptr),
    // Stretch at bottom of main layout
    m_idxLytItemStretch(-1),
    // Method Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName("DbCnctWdgt");

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

    // <ComboBox> Socket Type
    //-----------------------

    m_pWdgtSocketType = new QWidget();
    m_pWdgtSocketType->setFixedWidth(m_cxLblWidthClm2 + m_cxEdtWidthClm2);
    m_pLytSocketType = new QHBoxLayout();
    m_pLytSocketType->setContentsMargins(0, 0, 0, 0);
    m_pLytSocketType->setSpacing(0);
    m_pWdgtSocketType->setLayout(m_pLytSocketType);
    m_pLytLineType->addWidget(m_pWdgtSocketType);

    m_pLblSocketType = new QLabel("Socket Type:");
    m_pLblSocketType->setFixedWidth(m_cxLblWidthClm2);
    m_pLytSocketType->addWidget(m_pLblSocketType);
    m_pCmbSocketType = new QComboBox();
    m_pCmbSocketType->setFixedWidth(m_cxEdtWidthClm2);
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
    m_pLblDefault->setFixedWidth(m_cxLblWidthClm3);
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

    // <Line> Local Host
    //------------------

    m_pLytLineLocalHost = new QHBoxLayout();
    m_pLytLineLocalHost->setContentsMargins(0, 0, 0, 0);
    m_pLytLineLocalHost->setSpacing(m_cxSpacing);
    m_pLyt->addLayout(m_pLytLineLocalHost);

    // <LineEdit> Local Host Name
    //-----------------------------

    m_pWdgtLocalHostName = new QWidget();
    m_pWdgtLocalHostName->setFixedWidth(m_cxLblWidthClm1 + m_cxEdtWidthClm1);
    m_pLytLocalHostName = new QHBoxLayout();
    m_pLytLocalHostName->setContentsMargins(0, 0, 0, 0);
    m_pLytLocalHostName->setSpacing(0);
    m_pWdgtLocalHostName->setLayout(m_pLytLocalHostName);
    m_pLytLineLocalHost->addWidget(m_pWdgtLocalHostName);

    m_pLblLocalHostName = new QLabel("Local Host Name:");
    m_pLblLocalHostName->setFixedWidth(m_cxLblWidthClm1);
    m_pLytLocalHostName->addWidget(m_pLblLocalHostName);
    m_pEdtLocalHostName = new QLineEdit();
    m_pEdtLocalHostName->setFixedWidth(m_cxEdtWidthClm1);
    m_pEdtLocalHostName->setEnabled(false);
    m_pLytLocalHostName->addWidget(m_pEdtLocalHostName);
    m_pLytLocalHostName->addSpacing(m_cxSpacing);

    // <LineEdit> Local Host IP Address
    //---------------------------------

    m_pWdgtLocalHostAddr = new QWidget();
    m_pWdgtLocalHostAddr->setFixedWidth(m_cxLblWidthClm2 + m_cxEdtWidthClm2);
    m_pLytLocalHostAddr = new QHBoxLayout();
    m_pLytLocalHostAddr->setContentsMargins(0, 0, 0, 0);
    m_pLytLocalHostAddr->setSpacing(0);
    m_pWdgtLocalHostAddr->setLayout(m_pLytLocalHostAddr);
    m_pLytLineLocalHost->addWidget(m_pWdgtLocalHostAddr);

    m_pLblLocalHostAddr = new QLabel("IP Address:");
    m_pLblLocalHostAddr->setFixedWidth(m_cxLblWidthClm2);
    m_pLytLocalHostAddr->addWidget(m_pLblLocalHostAddr);
    m_pEdtLocalHostAddr = new QLineEdit();
    m_pEdtLocalHostAddr->setFixedWidth(m_cxEdtWidthClm2);
    m_pEdtLocalHostAddr->setEnabled(false);
    m_pLytLocalHostAddr->addWidget(m_pEdtLocalHostAddr);
    m_pLytLocalHostAddr->addSpacing(m_cxSpacing);

    // <LineEdit> Local Host Port
    //----------------------------

    m_pWdgtListenPort = new QWidget();
    m_pWdgtListenPort->setFixedWidth(m_cxLblWidthClm3 + m_cxEdtWidthClm3);
    m_pLytListenPort = new QHBoxLayout();
    m_pLytListenPort->setContentsMargins(0, 0, 0, 0);
    m_pLytListenPort->setSpacing(0);
    m_pWdgtListenPort->setLayout(m_pLytListenPort);
    m_pLytLineLocalHost->addWidget(m_pWdgtListenPort);

    m_pLblListenPort = new QLabel("Listen Port:");
    m_pLblListenPort->setFixedWidth(m_cxLblWidthClm3);
    m_pLytListenPort->addWidget(m_pLblListenPort);
    m_pEdtListenPort = new QLineEdit("");
    m_pEdtListenPort->setFixedWidth(m_cxEdtWidthClm3);
    m_pEdtListenPort->setEnabled(false);
    m_pEdtListenPort->setToolTip( "Port number the server is listening for incoming connections." );
    m_pEdtListenPort->setValidator( new QIntValidator(1,65535,this) );
    m_pLytListenPort->addWidget(m_pEdtListenPort);
    m_pLytListenPort->addStretch();

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtListenPort,
        /* szSignal     */ SIGNAL(textChanged(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtListenPortTextChanged(const QString&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Stretch> at EOL
    //-----------------

    m_pLytLineLocalHost->addStretch();

    // <Line> Shm BufferSize
    //----------------------

    m_pWdgtLineBufferSize = new QWidget();
    m_pLytLineBufferSize = new QHBoxLayout();
    m_pLytLineBufferSize->setContentsMargins(0, 0, 0, 0);
    m_pLytLineBufferSize->setSpacing(m_cxSpacing);
    m_pWdgtLineBufferSize->setLayout(m_pLytLineBufferSize);
    m_pLyt->addWidget(m_pWdgtLineBufferSize);

    // <LineEdit> Shm Buffer Size
    //---------------------------

    m_pWdgtBufferSize = new QWidget();
    m_pWdgtBufferSize->setFixedWidth(m_cxLblWidthClm1 + m_cxEdtWidthClm1);
    m_pLytBufferSize = new QHBoxLayout();
    m_pLytBufferSize->setContentsMargins(0, 0, 0, 0);
    m_pLytBufferSize->setSpacing(0);
    m_pWdgtBufferSize->setLayout(m_pLytBufferSize);
    m_pLytLineBufferSize->addWidget(m_pWdgtBufferSize);

    m_pLblBufferSize_bytes = new QLabel("Buffer Size / Bytes:");
    m_pLblBufferSize_bytes->setFixedWidth(m_cxLblWidthClm1);
    m_pLytBufferSize->addWidget(m_pLblBufferSize_bytes);
    m_pEdtBufferSize_bytes = new QLineEdit();
    m_pEdtBufferSize_bytes->setFixedWidth(m_cxEdtWidthClm1);
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

    m_pLytLineBufferSize->addStretch();

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
    m_pBtnReset->setEnabled(false);
    m_pBtnReset->setFixedWidth(cxBtnWidth);
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
    m_pBtnApply->setEnabled(false);
    m_pBtnApply->setFixedWidth(cxBtnWidth);
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

    m_pBtnStartup = new QPushButton(c_strBtnStartup);
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
CWdgtDbConnections::~CWdgtDbConnections()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "CDbClient.dtor",
        /* strAddInfo   */ "" );

    saveSettings();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDb = nullptr;
    m_cxSpacing = 0;
    m_cxLblWidthClm1 = 0;
    m_cxEdtWidthClm1 = 0;
    m_cxLblWidthClm2 = 0;
    m_cxEdtWidthClm2 = 0;
    m_cxLblWidthClm3 = 0;
    m_cxEdtWidthClm3 = 0;
    m_pLyt = nullptr;
    // Connection Settings
    //m_cnctType;
    //m_hostSettingsWidget;
    //m_hostSettingsServerDefault;
    m_pLytLineType = nullptr;
    m_pWdgtProtocolType = nullptr;
    m_pLytProtocolType = nullptr;
    m_pLblProtocolType = nullptr;
    m_pCmbProtocolType = nullptr;
    m_pWdgtSocketType = nullptr;
    m_pLytSocketType = nullptr;
    m_pLblSocketType = nullptr;
    m_pCmbSocketType = nullptr;
    m_pLblDefault = nullptr;
    m_pChkDefault = nullptr;
    m_pLblState = nullptr;
    m_pLedState = nullptr;
    m_pLytLineLocalHost = nullptr;
    m_pWdgtLocalHostName = nullptr;
    m_pLytLocalHostName = nullptr;
    m_pLblLocalHostName = nullptr;
    m_pEdtLocalHostName = nullptr;
    m_pWdgtLocalHostAddr = nullptr;
    m_pLytLocalHostAddr = nullptr;
    m_pLblLocalHostAddr = nullptr;
    m_pEdtLocalHostAddr = nullptr;
    m_bLocalHostInfoLookUpInProcess = false;
    m_pWdgtListenPort = nullptr;
    m_pLytListenPort = nullptr;
    m_pLblListenPort = nullptr;
    m_pEdtListenPort = nullptr;
    // Buffer Size (for Shm Socket Types only)
    m_pWdgtLineBufferSize = nullptr;
    m_pLytLineBufferSize = nullptr;
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
    m_pBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContents = nullptr;
    m_pWdgtSrvCltConnections = nullptr;
    m_pLytSrvCltConnections = nullptr;
    m_pModelSrvCltConnections = nullptr;
    m_pDelegateSrvCltConnections = nullptr;
    m_pTblViewSrvCltConnections = nullptr;
    // Protocol Trace
    m_pWdgtMsgLog = nullptr;
    m_pLytMsgLog = nullptr;
    m_pLblMsgLog = nullptr;
    m_bTrcMsgLogEnabled = false;
    m_uTrcMsgLogMaxItems = 0;
    m_pWdgtTrcMsgLog = nullptr;
    // Stretch at bottom of main layout
    m_idxLytItemStretch = 0;
    // Method Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbConnections::setDb( CDb* i_pDb )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Db: " + QString( i_pDb == nullptr ? "nullptr" : i_pDb->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDb",
        /* strAddInfo   */ strAddTrcInfo );

    // Show/hide GUI controls depending on active connection (depending on protocol type),
    // show/hide details, fill connection controls (connection parameters and watch dog settings)
    // and fill detail controls:

    if( m_pDb != i_pDb )
    {
        if( m_pDb != nullptr )
        {
            // <Signals/Slots> of connection
            //------------------------------

            QObject::disconnect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(stateChanged(ZS::Data::CDb*,ZS::Data::CDb::EState)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbStateChanged(ZS::Data::CDb*,ZS::Data::CDb::EState)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(requestInProgressChanged(ZS::Data::CDb*, ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbRequestInProgressChanged(ZS::Data::CDb*, ZS::System::SRequestDscr)) );

            QObject::disconnect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(defaultHostSettingsChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, const ZS::Ipc::SServerHostSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbDefaultHostSettingsChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, const ZS::Ipc::SServerHostSettings&)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(hostSettingsChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, const ZS::Ipc::SServerHostSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbHostSettingsChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, const ZS::Ipc::SServerHostSettings&)) );
            QObject::disconnect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(hostSettingsRowVersionChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbHostSettingsRowVersionChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)) );

            //QObject::disconnect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(connected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onDbClientConnected(QObject*,const ZS::Ipc::SSocketDscr&)) );
            //QObject::disconnect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(disconnected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onDbClientDisconnected(QObject*,const ZS::Ipc::SSocketDscr&)) );
            //QObject::disconnect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(clientRegistered(QObject*,const ZS::Data::SDbClientRegSpec&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onDbClientRegistered(QObject*,const ZS::Data::SDbClientRegSpec&)) );

            resetCnctControls(); // also resets m_hostSettingsWidget

            if( m_bShowDetails )
            {
                resetDetailControls();
            }
        } // if( m_pDb != nullptr )

        m_pDb = i_pDb;

        m_protTypeWidget            = EProtocolTypeUndefined;
        m_hostSettingsWidget        = SServerHostSettings();
        m_hostSettingsServerDefault = SServerHostSettings();

        if( m_pDb != nullptr )
        {
            // <Signals/Slots> of connection
            //------------------------------

            if( !QObject::connect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(stateChanged(ZS::Data::CDb*,ZS::Data::CDb::EState)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbStateChanged(ZS::Data::CDb*,ZS::Data::CDb::EState)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(requestInProgressChanged(ZS::Data::CDb*, ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbRequestInProgressChanged(ZS::Data::CDb*, ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            if( !QObject::connect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(defaultHostSettingsChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, const ZS::Ipc::SServerHostSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbDefaultHostSettingsChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, const ZS::Ipc::SServerHostSettings&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(hostSettingsChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, const ZS::Ipc::SServerHostSettings&)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbHostSettingsChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, const ZS::Ipc::SServerHostSettings&)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
            if( !QObject::connect(
                /* pObjSender   */ m_pDb,
                /* szSignal     */ SIGNAL(hostSettingsRowVersionChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbHostSettingsRowVersionChanged(ZS::Data::CDb*, ZS::Data::EProtocolType, ZS::Ipc::ESocketType)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            //if( !QObject::connect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(connected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onDbClientConnected(QObject*,const ZS::Ipc::SSocketDscr&)) ) )
            //{
            //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            //}
            //if( !QObject::connect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(disconnected(QObject*,const ZS::Ipc::SSocketDscr&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onDbClientDisconnected(QObject*,const ZS::Ipc::SSocketDscr&)) ) )
            //{
            //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            //}
            //if( !QObject::connect(
            //    /* pObjSender   */ m_pDb,
            //    /* szSignal     */ SIGNAL(clientRegistered(QObject*,const ZS::Data::SDbClientRegSpec&)),
            //    /* pObjReceiver */ this,
            //    /* szSlot       */ SLOT(onDbClientRegistered(QObject*,const ZS::Data::SDbClientRegSpec&)) ) )
            //{
            //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            //}

            // Read current settings
            //----------------------

            // Initially indicate first supported protocol type and
            // first socket type of this protocol type.

            QList<EProtocolType> arProtTypes = m_pDb->getProtocolTypes();
            QList<ESocketType>   arSockTypes = m_pDb->getSocketTypes(arProtTypes[0]);

            m_protTypeWidget     = arProtTypes[0];
            m_hostSettingsWidget = m_pDb->getHostSettings(m_protTypeWidget, arSockTypes[0]);

            if( m_pDb->getDefaultHostSettings(m_protTypeWidget,m_hostSettingsWidget.m_socketType) != nullptr )
            {
                m_hostSettingsServerDefault = *m_pDb->getDefaultHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);
            }

            fillCnctControls(m_hostSettingsWidget); // sets m_hostSettingsWidget to method call argument

            if( m_bShowDetails )
            {
                fillDetailControls();
            }
        } // if( m_pDb != nullptr )
    } // if( m_pDb != i_pDb )

} // setDb

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbConnections::readSettings()
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
        m_bTrcMsgLogEnabled = str2Bool( settings.value(strSettingsKey+"/TrcMsgLogEnabled",bool2Str(m_bTrcMsgLogEnabled)).toString() );
    }
    else
    {
        settings.setValue( strSettingsKey+"/TrcMsgLogEnabled", bool2Str(m_bTrcMsgLogEnabled) );
        bSyncSettings = true;
    }
    if( settings.contains(strSettingsKey+"/TrcMsgLogMaxItems") )
    {
        m_uTrcMsgLogMaxItems = settings.value(strSettingsKey+"/TrcMsgLogMaxItems",m_uTrcMsgLogMaxItems).toUInt();
    }
    else
    {
        settings.setValue( strSettingsKey+"/TrcMsgLogMaxItems", m_uTrcMsgLogMaxItems );
        bSyncSettings = true;
    }

    if( bSyncSettings )
    {
        settings.sync();
    }

} // readSettings

//------------------------------------------------------------------------------
void CWdgtDbConnections::saveSettings()
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
        ZS::Ipc::GUI::STrcMsgLogSettings trcMsgLogSettings = m_pWdgtTrcMsgLog->getSettings();

        m_bTrcMsgLogEnabled  = trcMsgLogSettings.m_bEnabled;
        m_uTrcMsgLogMaxItems = trcMsgLogSettings.m_uMaxItems;
    }

    settings.setValue( strSettingsKey+"/ShowDetails", bool2Str(m_bShowDetails) );
    settings.setValue( strSettingsKey+"/TrcMsgLogEnabled", bool2Str(m_bTrcMsgLogEnabled) );
    settings.setValue( strSettingsKey+"/TrcMsgLogMaxItems", m_uTrcMsgLogMaxItems );

} // saveSettings

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbConnections::onCmbProtocolTypeCurrentIndexChanged( int i_iIdx )
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

    bool bCanceled = false;

    EProtocolType protTypeNew = static_cast<EProtocolType>(m_pCmbProtocolType->itemData(i_iIdx).toInt());

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    // If there are changes for the previous protocol type ..
    if( m_hostSettingsWidget != hostSettingsServer )
    {
        QMessageBox msgBox;

        msgBox.setWindowTitle( windowTitle() );
        msgBox.setText("Settings have been changed for protocol type " + protocolType2Str(m_protTypeWidget) + ".");
        msgBox.setInformativeText("Do you want to accept your changes?");
        msgBox.setStandardButtons( QMessageBox::Yes|QMessageBox::Discard|QMessageBox::Cancel );
        msgBox.setDefaultButton(QMessageBox::Yes);

        int iResult = msgBox.exec();

        switch( iResult )
        {
            case QMessageBox::Yes:
            {
                applySettings();
                break;
            }
            case QMessageBox::Discard:
            {
                break;
            }
            case QMessageBox::Cancel:
            {
                bCanceled = true; // hostSettingsNew remains the same and will be newly adjusted together with the previous socket type.
                break;
            }
            default:
            {
                break;
            }
        }
    } // if( m_hostSettingsWidget != hostSettingsServer )

    if( bCanceled )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pCmbProtocolType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbProtocolTypeCurrentIndexChanged(int)) );

        int idxCmb = m_pCmbProtocolType->findData(m_protTypeWidget);
        m_pCmbProtocolType->setCurrentIndex(idxCmb);

        if( !QObject::connect(
            /* pObjSender   */ m_pCmbProtocolType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbProtocolTypeCurrentIndexChanged(int)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( bCanceled )
    else // if( !bCanceled )
    {
        m_protTypeWidget = protTypeNew;

        ESocketType socketTypeWidget = m_hostSettingsWidget.m_socketType;

        if( !m_pDb->isSocketTypeSupported(m_hostSettingsWidget.m_socketType, m_protTypeWidget) )
        {
            QList<ESocketType> arSockTypes = m_pDb->getSocketTypes(m_protTypeWidget);
            socketTypeWidget = arSockTypes[0];
        }

        resetCnctControls(); // also resets m_hostSettingsWidget

        m_hostSettingsWidget = m_pDb->getHostSettings(m_protTypeWidget, socketTypeWidget);

        fillCnctControls(m_hostSettingsWidget); // sets m_hostSettingsWidget to method call argument

        if( m_bShowDetails )
        {
            resetDetailControls();
            fillDetailControls();
        }
    } // if( !bCanceled )

} // onCmbProtocolTypeCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtDbConnections::onCmbSocketTypeCurrentIndexChanged( int i_iIdx )
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

    bool bCanceled = false;

    ESocketType sockTypeNew = static_cast<ESocketType>(m_pCmbSocketType->itemData(i_iIdx).toInt());

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    // If there are changes for the previous socket type ..
    if( m_hostSettingsWidget != hostSettingsServer )
    {
        QMessageBox msgBox;

        msgBox.setWindowTitle( windowTitle() );
        msgBox.setText("Settings have been changed for socket type " + socketType2Str(m_hostSettingsWidget.m_socketType) + ".");
        msgBox.setInformativeText("Do you want to accept your changes?");
        msgBox.setStandardButtons( QMessageBox::Yes|QMessageBox::Discard|QMessageBox::Cancel );
        msgBox.setDefaultButton(QMessageBox::Yes);

        int iResult = msgBox.exec();

        switch( iResult )
        {
            case QMessageBox::Yes:
            {
                applySettings();
                break;
            }
            case QMessageBox::Discard:
            {
                break;
            }
            case QMessageBox::Cancel:
            {
                bCanceled = true; // hostSettingsNew remains the same and will be newly adjusted together with the previous socket type.
                break;
            }
            default:
            {
                break;
            }
        }
    } // if( m_hostSettingsWidget != hostSettingsServer )

    if( bCanceled )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pCmbSocketType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) );

        int idxCmb = m_pCmbSocketType->findData(m_hostSettingsWidget.m_socketType);
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
    else // if( !bCanceled )
    {
        if( !m_pDb->isProtocolTypeSupported(m_protTypeWidget, sockTypeNew) )
        {
            QList<EProtocolType> arProtTypes = m_pDb->getProtocolTypes(sockTypeNew);
            m_protTypeWidget = arProtTypes[0];
        }

        resetCnctControls(); // also resets m_hostSettingsWidget

        m_hostSettingsWidget = m_pDb->getHostSettings(m_protTypeWidget, sockTypeNew);

        fillCnctControls(m_hostSettingsWidget); // sets m_hostSettingsWidget to method call argument

        if( m_bShowDetails )
        {
            resetDetailControls();
            fillDetailControls();
        }
    } // if( !bCanceled )

} // onCmbSocketTypeCurrentIndexChanged

//------------------------------------------------------------------------------
void CWdgtDbConnections::onChkDefaultToggled( bool i_bChecked )
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
        if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
        {
            resetCnctControls(); // also resets m_hostSettingsWidget

            fillCnctControls(m_hostSettingsServerDefault); // sets m_hostSettingsWidget to method call argument

            SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

            if( m_hostSettingsWidget == hostSettingsServer )
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
        } // if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
    } // if( i_bChecked )

} // onChkDefaultToggled

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbConnections::onEdtListenPortTextChanged( const QString& i_strText )
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
        /* strMethod    */ "onEdtListenPortTextChanged",
        /* strAddInfo   */ strAddTrcInfo );

    bool    bConverted;
    quint32 uVal = i_strText.toUInt(&bConverted);

    if( bConverted )
    {
        m_hostSettingsWidget.m_uLocalPort = uVal;
    }

    if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )
    {
        // If there are valid default settings ...
        if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pChkDefault,
                /* szSignal     */ SIGNAL(toggled(bool)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onChkDefaultToggled(bool)) );

            if( m_hostSettingsWidget == m_hostSettingsServerDefault )
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
        } // if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
    } // if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    if( m_hostSettingsWidget == hostSettingsServer )
    {
        m_pBtnReset->setEnabled(false);
        m_pBtnApply->setEnabled(false);
    }
    else
    {
        m_pBtnReset->setEnabled(true);
        m_pBtnApply->setEnabled(true);
    }

} // onEdtListenPortTextChanged

//------------------------------------------------------------------------------
void CWdgtDbConnections::onEdtBufferSizeTextChanged( const QString& i_strText )
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

    if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )
    {
        // If there are valid default settings ...
        if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pChkDefault,
                /* szSignal     */ SIGNAL(toggled(bool)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onChkDefaultToggled(bool)) );

            if( m_hostSettingsWidget == m_hostSettingsServerDefault )
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
        } // if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
    } // if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    if( m_hostSettingsWidget == hostSettingsServer )
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

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbConnections::onChkDatabaseNameUsedToggled( bool i_bChecked )
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

    if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )
    {
        // If there are valid default settings ...
        if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pChkDefault,
                /* szSignal     */ SIGNAL(toggled(bool)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onChkDefaultToggled(bool)) );

            if( m_hostSettingsWidget == m_hostSettingsServerDefault )
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
        } // if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
    } // if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    if( m_hostSettingsWidget == hostSettingsServer )
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
void CWdgtDbConnections::onEdtDatabaseNameTextChanged( const QString& i_strText )
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

    if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )
    {
        // If there are valid default settings ...
        if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pChkDefault,
                /* szSignal     */ SIGNAL(toggled(bool)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onChkDefaultToggled(bool)) );

            if( m_hostSettingsWidget == m_hostSettingsServerDefault )
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
        } // if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
    } // if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    if( m_hostSettingsWidget == hostSettingsServer )
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
void CWdgtDbConnections::onChkUserNameUsedToggled( bool i_bChecked )
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

    if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )
    {
        // If there are valid default settings ...
        if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pChkDefault,
                /* szSignal     */ SIGNAL(toggled(bool)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onChkDefaultToggled(bool)) );

            if( m_hostSettingsWidget == m_hostSettingsServerDefault )
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
        } // if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
    } // if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    if( m_hostSettingsWidget == hostSettingsServer )
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
void CWdgtDbConnections::onEdtUserNameTextChanged( const QString& i_strText )
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

    if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )
    {
        // If there are valid default settings ...
        if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pChkDefault,
                /* szSignal     */ SIGNAL(toggled(bool)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onChkDefaultToggled(bool)) );

            if( m_hostSettingsWidget == m_hostSettingsServerDefault )
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
        } // if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
    } // if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    if( m_hostSettingsWidget == hostSettingsServer )
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
void CWdgtDbConnections::onChkUserPasswordUsedToggled( bool i_bChecked )
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

    if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )
    {
        // If there are valid default settings ...
        if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pChkDefault,
                /* szSignal     */ SIGNAL(toggled(bool)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onChkDefaultToggled(bool)) );

            if( m_hostSettingsWidget == m_hostSettingsServerDefault )
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
        } // if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
    } // if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    if( m_hostSettingsWidget == hostSettingsServer )
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
void CWdgtDbConnections::onEdtUserPasswordTextChanged( const QString& i_strText )
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

    if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )
    {
        // If there are valid default settings ...
        if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pChkDefault,
                /* szSignal     */ SIGNAL(toggled(bool)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onChkDefaultToggled(bool)) );

            if( m_hostSettingsWidget == m_hostSettingsServerDefault )
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
        } // if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
    } // if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    if( m_hostSettingsWidget == hostSettingsServer )
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
void CWdgtDbConnections::onBtnOkClicked( bool /*i_bChecked*/ )
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

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    if( m_hostSettingsWidget != hostSettingsServer )
    {
        applySettings();
    }

    emit accepted();

} // onBtnOkClicked

//------------------------------------------------------------------------------
void CWdgtDbConnections::onBtnCancelClicked( bool /*i_bChecked*/ )
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

    if( m_pDb != nullptr )
    {
        SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

        resetCnctControls(); // also resets m_hostSettingsWidget

        fillCnctControls(hostSettingsServer); // sets m_hostSettingsWidget to method call argument

        if( m_bShowDetails )
        {
            fillDetailControls();
        }
    } // if( m_pDb != nullptr )

    emit rejected();

} // onBtnCancelClicked

//------------------------------------------------------------------------------
void CWdgtDbConnections::onBtnResetClicked( bool /*i_bChecked*/ )
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

    if( m_pDb != nullptr )
    {
        SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

        resetCnctControls(); // also resets m_hostSettingsWidget

        fillCnctControls(hostSettingsServer); // sets m_hostSettingsWidget to method call argument

        if( m_bShowDetails )
        {
            fillDetailControls();
        }
    } // if( m_pDb != nullptr )

} // onBtnResetClicked

//------------------------------------------------------------------------------
void CWdgtDbConnections::onBtnApplyClicked( bool /*i_bChecked*/ )
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

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    if( m_hostSettingsWidget != hostSettingsServer )
    {
        applySettings();
    }

} // onBtnApplyClicked

//------------------------------------------------------------------------------
void CWdgtDbConnections::onBtnStartupClicked( bool /*i_bChecked*/ )
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

    if( m_pDb != nullptr )
    {
        if( m_pBtnStartup->text() == c_strBtnStartup )
        {
            applySettings();

            m_pDb->startup();
        }
        else if( m_pBtnStartup->text() == c_strBtnShutdown )
        {
            m_pDb->shutdown();
        }
        else if( m_pBtnStartup->text() == c_strBtnAbort )
        {
            m_pDb->abortRequestInProgress();
        }
    } // if( m_pDb != nullptr )

} // onBtnStartupClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbConnections::onBtnDetailsClicked( bool /*i_bChecked*/ )
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
void CWdgtDbConnections::onBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContentsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContentsClicked",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pTblViewSrvCltConnections != nullptr )
    {
        m_pTblViewSrvCltConnections->resizeColumnsToContents();
        m_pTblViewSrvCltConnections->resizeRowsToContents();
    }

} // onBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContentsClicked

/*==============================================================================
protected slots: // connected to the signals of the database
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbConnections::onDbStateChanged(
    CDb*        i_pDb,
    CDb::EState i_state )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Db: " + QString( i_pDb == nullptr ? "nullptr" : i_pDb->objectName() );
        strAddTrcInfo += ", State: " + CDb::State2Str(i_state);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pDb != nullptr && i_pDb == m_pDb )
    {
        SServerHostSettings hostSettingsWidget = m_hostSettingsWidget;

        resetCnctControls(); // also resets m_hostSettingsWidget

        fillCnctControls(hostSettingsWidget); // sets m_hostSettingsWidget to method call argument

        if( m_bShowDetails )
        {
            fillDetailControls();
        }
    }

} // onDbStateChanged

//------------------------------------------------------------------------------
void CWdgtDbConnections::onDbRequestInProgressChanged(
    CDb*                     i_pDb,
    ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Db: " + QString( i_pDb == nullptr ? "nullptr" : i_pDb->objectName() );
        strAddTrcInfo += ", Req {" + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbRequestInProgressChanged",
        /* strAddInfo   */ strAddTrcInfo );

    QString strBtnStartup;
    QString strReqInProgress;

    switch( i_reqDscr.m_request )
    {
        case CDb::ERequestStartup:
        {
            if( i_reqDscr.m_iProgress_perCent < 100 )
            {
                strBtnStartup    = c_strBtnAbort;
                strReqInProgress = "Starting Up ...";
            }
            else if( i_reqDscr.m_iProgress_perCent >= 100 )
            {
                if( m_pDb->isListening(m_protTypeWidget, m_hostSettingsWidget.m_socketType) )
                {
                    strBtnStartup = c_strBtnShutdown;
                }
                else
                {
                    strBtnStartup = c_strBtnStartup;
                }
            }
            break;
        }
        case CDb::ERequestShutdown:
        {
            if( i_reqDscr.m_iProgress_perCent < 100 )
            {
                strBtnStartup    = c_strBtnAbort;
                strReqInProgress = "Shutting Down ...";
            }
            else if( i_reqDscr.m_iProgress_perCent >= 100 )
            {
                strBtnStartup = c_strBtnStartup;
            }
            break;
        }
        case CDb::ERequestUpdateHostSettings:
        {
            if( i_reqDscr.m_iProgress_perCent < 100 )
            {
                strBtnStartup    = c_strBtnAbort;
                strReqInProgress = "Changing Host Settings ...";
            }
            else if( i_reqDscr.m_iProgress_perCent >= 100 )
            {
                if( m_pDb->isListening(m_protTypeWidget, m_hostSettingsWidget.m_socketType) )
                {
                    strBtnStartup = c_strBtnShutdown;
                }
                else
                {
                    strBtnStartup = c_strBtnStartup;
                }
            }
            break;
        }
        case CDb::ERequestNone:
        default:
        {
            if( m_pDb->isListening(m_protTypeWidget, m_hostSettingsWidget.m_socketType) )
            {
                strBtnStartup = c_strBtnShutdown;
            }
            else
            {
                strBtnStartup = c_strBtnStartup;
            }
            break;
        }
    } // switch( i_reqDscr.m_request )

    if( m_pBtnStartup != nullptr )
    {
        m_pBtnStartup->setText(strBtnStartup);
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
            m_pBarRequestInProgress->setDurationInMs(10000);
            m_pBarRequestInProgress->setIncrementInMs(200);
            m_pBarRequestInProgress->start();
        }
        m_pBarRequestInProgress->setLabelText(strReqInProgress);
    }

} // onDbRequestInProgressChanged

/*==============================================================================
protected slots: // connected to the signals of the database
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbConnections::onDbDefaultHostSettingsChanged(
    CDb*                       i_pDb,
    EProtocolType              i_protocolType,
    const SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Db: " + QString( i_pDb == nullptr ? "nullptr" : i_pDb->objectName() );
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", HostSettings {" + i_hostSettings.getConnectionString(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbDefaultHostSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_protocolType == m_protTypeWidget && i_hostSettings.m_socketType == m_hostSettingsWidget.m_socketType )
    {
        m_hostSettingsServerDefault = i_hostSettings;

        if( m_hostSettingsWidget == m_hostSettingsServerDefault )
        {
            m_pChkDefault->setChecked(true);
            m_pChkDefault->setEnabled(false);
        }
        else
        {
            m_pChkDefault->setChecked(false);
            m_pChkDefault->setEnabled(true);
        }
    } // if( i_protocolType == m_protTypeWidget && i_hostSettings.m_socketType == m_hostSettingsWidget.m_socketType )

} // onDbDefaultHostSettingsChanged

//------------------------------------------------------------------------------
void CWdgtDbConnections::onDbHostSettingsChanged(
    CDb*                       i_pDb,
    EProtocolType              i_protocolType,
    const SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Db: " + QString( i_pDb == nullptr ? "nullptr" : i_pDb->objectName() );
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", HostSettings {" + i_hostSettings.getConnectionString(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbHostSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_protocolType == m_protTypeWidget && i_hostSettings.m_socketType == m_hostSettingsWidget.m_socketType )
    {
        if( m_hostSettingsWidget == m_hostSettingsServerDefault )
        {
            m_pChkDefault->setChecked(true);
            m_pChkDefault->setEnabled(false);
        }
        else
        {
            m_pChkDefault->setChecked(false);
            m_pChkDefault->setEnabled(true);
        }

        if( m_hostSettingsWidget == i_hostSettings )
        {
            m_pBtnReset->setEnabled(false);
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnReset->setEnabled(true);
            m_pBtnApply->setEnabled(true);
        }
    } // if( i_protocolType == m_protTypeWidget && i_hostSettings.m_socketType == m_hostSettingsWidget.m_socketType )

} // onDbHostSettingsChanged

//------------------------------------------------------------------------------
void CWdgtDbConnections::onDbHostSettingsRowVersionChanged(
    CDb*          i_pDb,
    EProtocolType i_protocolType,
    ESocketType   i_socketType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Db: " + QString( i_pDb == nullptr ? "nullptr" : i_pDb->objectName() );
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", SockType: " + socketType2Str(i_socketType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDbHostSettingsRowVersionChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDbHostSettingsRowVersionChanged

/*==============================================================================
protected slots: // connected to the signals of the database
==============================================================================*/

////------------------------------------------------------------------------------
//void CWdgtDbConnections::onDbClientConnected(
//    QObject*           i_pDb,
//    const SSocketDscr& i_socketDscr )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        if( i_pDb == nullptr )
//        {
//            strAddTrcInfo = "Db: nullptr";
//        }
//        else
//        {
//            strAddTrcInfo = "Db: " + i_pDb->objectName();
//        }
//
//        //strAddTrcInfo += ", Protocol: " + protocolType2Str(i_socketDscr.m_protTypeWidget);
//        strAddTrcInfo += ", ConnectionType: " + socketType2Str(i_socketDscr.m_socketType);
//        strAddTrcInfo += ", RemoteHost: " + i_socketDscr.m_strRemoteHostName;
//        strAddTrcInfo += ", RemotePort: " + QString::number(i_socketDscr.m_uRemotePort);
//        strAddTrcInfo += ", LocalHost: " + i_socketDscr.m_strLocalHostName;
//        strAddTrcInfo += ", LocalPort: " + QString::number(i_socketDscr.m_uLocalPort);
//        strAddTrcInfo += ", SocketId: " + QString::number(i_socketDscr.m_iSocketId);
//
//        //if( i_socketDscr.m_bDatabaseNameUsed )
//        //{
//        //    strAddTrcInfo += ", DbName: " + i_socketDscr.m_strDatabaseName;
//        //}
//        //if( i_socketDscr.m_bUserNameUsed )
//        //{
//        //    strAddTrcInfo += ", UserName: " + i_socketDscr.m_strUserName;
//        //}
//        //if( i_socketDscr.m_bPasswordUsed )
//        //{
//        //    strAddTrcInfo += ", Password: " + i_socketDscr.m_strPassword;
//        //}
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onDbClientConnected",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( m_pDb != nullptr )
//    {
//        if( m_pEdtActiveConnections != nullptr )
//        {
//            int iCnctsCount = m_pDb->getClientConnectionsCount(m_protTypeWidget, m_hostSettingsWidget.m_socketType);
//            m_pEdtActiveConnections->setText( QString::number(iCnctsCount) );
//        }
//    } // if( m_pDb != nullptr )
//
//} // onDbClientConnected
//
////------------------------------------------------------------------------------
//void CWdgtDbConnections::onDbClientDisconnected(
//    QObject*           i_pDb,
//    const SSocketDscr& i_socketDscr )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        if( i_pDb == nullptr )
//        {
//            strAddTrcInfo = "Db: nullptr";
//        }
//        else
//        {
//            strAddTrcInfo = "Db: " + i_pDb->objectName();
//        }
//
//        //strAddTrcInfo += ", Protocol: " + protocolType2Str(i_socketDscr.m_protTypeWidget);
//        strAddTrcInfo += ", ConnectionType: " + socketType2Str(i_socketDscr.m_socketType);
//        strAddTrcInfo += ", RemoteHost: " + i_socketDscr.m_strRemoteHostName;
//        strAddTrcInfo += ", RemotePort: " + QString::number(i_socketDscr.m_uRemotePort);
//        strAddTrcInfo += ", LocalHost: " + i_socketDscr.m_strLocalHostName;
//        strAddTrcInfo += ", LocalPort: " + QString::number(i_socketDscr.m_uLocalPort);
//        strAddTrcInfo += ", SocketId: " + QString::number(i_socketDscr.m_iSocketId);
//
//        //if( i_socketDscr.m_bDatabaseNameUsed )
//        //{
//        //    strAddTrcInfo += ", DbName: " + i_socketDscr.m_strDatabaseName;
//        //}
//        //if( i_socketDscr.m_bUserNameUsed )
//        //{
//        //    strAddTrcInfo += ", UserName: " + i_socketDscr.m_strUserName;
//        //}
//        //if( i_socketDscr.m_bPasswordUsed )
//        //{
//        //    strAddTrcInfo += ", Password: " + i_socketDscr.m_strPassword;
//        //}
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onDbClientDisconnected",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( m_pDb != nullptr )
//    {
//        if( m_pEdtActiveConnections != nullptr )
//        {
//            int iCnctsCount = m_pDb->getClientConnectionsCount(m_protTypeWidget, m_hostSettingsWidget.m_socketType);
//            m_pEdtActiveConnections->setText( QString::number(iCnctsCount) );
//        }
//    } // if( m_pDb != nullptr )
//
//} // onDbClientDisconnected
//
////------------------------------------------------------------------------------
//void CWdgtDbConnections::onDbClientRegistered(
//    QObject*                i_pDb,
//    const SDbClientRegSpec& i_regSpec )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        if( i_pDb == nullptr )
//        {
//            strAddTrcInfo = "Db: nullptr";
//        }
//        else
//        {
//            strAddTrcInfo = "Db: " + i_pDb->objectName();
//        }
//
//        if( iAddTrcInfoDetailLevel >= 1 )
//        {
//            QStringList strlstSpecs;
//            QString     strSpec;
//            SDSNodeSpec dsNodeSpec;
//            int         idxSpec;
//
//            for( idxSpec = 0; idxSpec < i_regSpec.m_arDSNodeSpecs.size(); idxSpec++ )
//            {
//                dsNodeSpec = i_regSpec.m_arDSNodeSpecs[idxSpec];
//
//                strSpec = "CONNECT " + dsNodeSpec.getAddTrcInfoStr( iAddTrcInfoDetailLevel );
//
//                strlstSpecs += strSpec;
//            }
//
//            strAddTrcInfo += ", " + strlstSpecs.join("; ");
//        }
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onDbClientRegistered",
//        /* strAddInfo   */ strAddTrcInfo );
//
//} // onDbClientRegistered

/*==============================================================================
protected slots: // host info lookup
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbConnections::onLocalHostInfoLookedUp( const QHostInfo& i_old_hostInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onLocalHostInfoLookedUp",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pEdtLocalHostAddr != nullptr )
    {
        QList<QHostAddress> arHostAddrs = i_old_hostInfo.addresses();

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

    m_bLocalHostInfoLookUpInProcess = false;

} // onLocalHostInfoLookedUp

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CWdgtDbConnections::getConnectionToolTip()
//------------------------------------------------------------------------------
{
    QString strToolTip;

    if( m_pDb != nullptr )
    {
        strToolTip = m_pDb->getConnectionString(m_protTypeWidget, m_hostSettingsWidget.m_socketType);
    }

    return strToolTip;

} // getConnectionToolTip

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbConnections::onShowDetailsChanged()
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

            m_pLytLineConnections = new QHBoxLayout();
            m_pLytDetailsStates->addLayout(m_pLytLineConnections);

            // <Label> Active Connections
            //----------------------------

            m_pLblActiveConnections = new QLabel("Active Connections:");
            m_pLytLineConnections->addWidget(m_pLblActiveConnections);
            m_pEdtActiveConnections = new QLabel("0");
            m_pLytLineConnections->addWidget(m_pEdtActiveConnections);
            m_pLytLineConnections->addSpacing(10);

            // <Button> Resize Columns and Columns to Contents
            //------------------------------------------------

            m_pBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContents = new QPushButton("<->");
            m_pBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContents->setFixedWidth(30);
            m_pLytLineConnections->addWidget(m_pBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContents);

            if( !QObject::connect(
                /* pObjSender   */ m_pBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContents,
                /* szSignal     */ SIGNAL(clicked(bool)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContentsClicked(bool)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            // <Stretch> Active Connections
            //-----------------------------

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
            m_pLytSrvCltConnections->setContentsMargins(0, 0, 0, 0); // left, top, right, bottom
            m_pWdgtSrvCltConnections->setLayout(m_pLytSrvCltConnections);
            m_pSplitterDetails->addWidget(m_pWdgtSrvCltConnections);

            m_pModelSrvCltConnections = new CModelDbConnections(this);

            if( m_pDb != nullptr )
            {
                m_pModelSrvCltConnections->setDb(m_pDb);
                m_pModelSrvCltConnections->setProtocolType(m_protTypeWidget);
                m_pModelSrvCltConnections->setSocketType(m_hostSettingsWidget.m_socketType);
                m_pModelSrvCltConnections->update();
            }

            m_pTblViewSrvCltConnections = new QTableView();
            m_pTblViewSrvCltConnections->setModel(m_pModelSrvCltConnections);
            m_pTblViewSrvCltConnections->setSelectionBehavior(QAbstractItemView::SelectItems);
            m_pTblViewSrvCltConnections->setSelectionMode(QAbstractItemView::SingleSelection);
            m_pTblViewSrvCltConnections->setEditTriggers(QAbstractItemView::NoEditTriggers);

            if( m_hostSettingsWidget.m_socketType == ESocketTypeUndefined || m_hostSettingsWidget.m_socketType == ESocketTypeInProcMsg )
            {
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnSocketId);
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnSocketState);
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnServerListenPort);
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnLocalPort);
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnRemoteHostName);
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnRemotePort);
            }
            else
            {
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnSocketId);
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnSocketState);
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnServerListenPort);
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnLocalPort);
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnRemoteHostName);
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnRemotePort);
            }

            m_pTblViewSrvCltConnections->resizeColumnsToContents();
            m_pTblViewSrvCltConnections->resizeRowsToContents();
            m_pLytSrvCltConnections->addWidget(m_pTblViewSrvCltConnections);

            m_pDelegateSrvCltConnections = new CDelegateDbConnectedNodes(m_pTblViewSrvCltConnections);
            m_pDelegateSrvCltConnections->setDb(m_pDb);
            m_pDelegateSrvCltConnections->setModel(m_pModelSrvCltConnections);
            m_pTblViewSrvCltConnections->setItemDelegate(m_pDelegateSrvCltConnections);
            m_pTblViewSrvCltConnections->setEditTriggers(QAbstractItemView::AllEditTriggers);

            m_pLytSrvCltConnections->addSpacing(10);

            // <MessageLog>
            //--------------

            m_pWdgtMsgLog = new QWidget();
            m_pLytMsgLog = new QVBoxLayout();
            m_pLytMsgLog->setContentsMargins(0, 0, 0, 0); // left, top, right, bottom
            m_pWdgtMsgLog->setLayout(m_pLytMsgLog);
            m_pSplitterDetails->addWidget(m_pWdgtMsgLog);

            pLine = new CSepLine();
            m_pLytMsgLog->addWidget(pLine);

            m_pLytMsgLog->addSpacing(5);

            m_pLblMsgLog = new QLabel( "Message Protocol:" );
            m_pLytMsgLog->addWidget(m_pLblMsgLog);

            ZS::Ipc::GUI::STrcMsgLogSettings trcMsgLogSettings(m_bTrcMsgLogEnabled, m_uTrcMsgLogMaxItems);
            m_pWdgtTrcMsgLog = new CWdgtDbTrcMsgLog(trcMsgLogSettings);
            m_pLytMsgLog->addWidget(m_pWdgtTrcMsgLog);

        } // if( m_pWdgtDetailsStates == nullptr )

        fillDetailControls();

    } // if( m_bShowDetails )

    else // if( !m_bShowDetails )
    {
        m_pBtnDetails->setText(c_strBtnDetailsShowDetails);

        if( m_pWdgtTrcMsgLog != nullptr )
        {
            ZS::Ipc::GUI::STrcMsgLogSettings trcMsgLogSettings = m_pWdgtTrcMsgLog->getSettings();

            m_bTrcMsgLogEnabled  = trcMsgLogSettings.m_bEnabled;
            m_uTrcMsgLogMaxItems = trcMsgLogSettings.m_uMaxItems;
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
        m_pSplitterDetails = nullptr;
        m_pLytLineConnections = nullptr;
        m_pLblActiveConnections = nullptr;
        m_pEdtActiveConnections = nullptr;
        m_pWdgtSrvCltConnections = nullptr;
        m_pLytSrvCltConnections = nullptr;
        m_pModelSrvCltConnections = nullptr;
        m_pTblViewSrvCltConnections = nullptr;
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
void CWdgtDbConnections::resetCnctControls()
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

    if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pChkDefault,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDefaultToggled(bool)) );

        m_pChkDefault->setChecked(false);
        m_pChkDefault->setEnabled(false);
        m_pChkDefault->show();
        m_pLblDefault->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pChkDefault,
            /* szSignal     */ SIGNAL(toggled(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onChkDefaultToggled(bool)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblDefault != nullptr && m_pChkDefault != nullptr )

    // <Led> State
    //------------

    if( m_pLedState != nullptr )
    {
        m_pLedState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
        m_pLedState->setEnabled(false);

    } // if( m_pLedState != nullptr )

    // <LineEdit> Listen Port
    //----------------------------

    if( m_pLblListenPort != nullptr && m_pEdtListenPort != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtListenPort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtListenPortTextChanged(const QString&)) );

        m_pEdtListenPort->setValidator(nullptr);
        m_pEdtListenPort->setText("");
        m_pEdtListenPort->setEnabled(false);
        m_pEdtListenPort->show();
        m_pLblListenPort->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtListenPort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtListenPortTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pLblListenPort != nullptr && m_pEdtRemotePort != nullptr )

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

    // <Button> Startup/Shutdown/Abort
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
void CWdgtDbConnections::fillCnctControls( const SServerHostSettings& i_hostSettings )
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

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, i_hostSettings.m_socketType);

    bool    bDbListening = false;
    QString strConnection;
    bool    bDatabaseNameUsed = false;
    bool    bUserNameUsed = false;
    bool    bPasswordUsed = false;
    int     idxCmb;

    if( m_pDb != nullptr )
    {
        bDbListening  = m_pDb->isListening(m_protTypeWidget, i_hostSettings.m_socketType);
        strConnection = m_pDb->getConnectionString(m_protTypeWidget, i_hostSettings.m_socketType);
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

        QList<EProtocolType> arProtTypes = m_pDb->getProtocolTypes();

        for( idxCmb = 0; idxCmb < arProtTypes.size(); idxCmb++ )
        {
            EProtocolType protType = arProtTypes[idxCmb];
            m_pCmbProtocolType->addItem( protocolType2Str(protType), protType );
        }

        idxCmb = m_pCmbProtocolType->findData(m_protTypeWidget);
        m_pCmbProtocolType->setCurrentIndex(idxCmb);

        if( m_pCmbProtocolType->count() > 1 )
        {
            m_pCmbProtocolType->setEnabled(true);
        }
        else
        {
            m_pCmbProtocolType->setEnabled(false);
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

    // <ComboBox> Socket Type
    //-----------------------

    if( m_pWdgtSocketType != nullptr && m_pCmbSocketType != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pCmbSocketType,
            /* szSignal     */ SIGNAL(currentIndexChanged(int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onCmbSocketTypeCurrentIndexChanged(int)) );

        QList<ESocketType> arSockTypes = m_pDb->getSocketTypes();

        for( idxCmb = 0; idxCmb < arSockTypes.size(); idxCmb++ )
        {
            ESocketType sockType = arSockTypes[idxCmb];
            m_pCmbSocketType->addItem( socketType2Str(sockType), sockType );
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

        // If there are no valid default settings ...
        if( m_hostSettingsServerDefault.m_socketType == ESocketTypeUndefined )
        {
            m_pChkDefault->setChecked(false);
            m_pChkDefault->setEnabled(false);
            m_pChkDefault->hide();
            m_pLblDefault->hide();
        }
        // If there are valid default settings ...
        else // if( m_hostSettingsServerDefault.m_socketType != ESocketTypeUndefined )
        {
            if( i_hostSettings == m_hostSettingsServerDefault )
            {
                m_pChkDefault->setChecked(true);
                m_pChkDefault->setEnabled(false);
            }
            else
            {
                m_pChkDefault->setChecked(false);
                m_pChkDefault->setEnabled(true);
            }
            m_pChkDefault->show();
            m_pLblDefault->show();
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
        if( bDbListening )
        {
            m_pLedState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
        }
        else // if( !bDbListening )
        {
            m_pLedState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
        }
        m_pLedState->setEnabled(true);

    } // if( m_pLblState != nullptr && m_pLedState != nullptr )

    // <LineEdit> Local Host Name
    //-----------------------------

    if( m_pEdtLocalHostName != nullptr )
    {
        m_pEdtLocalHostName->setText(i_hostSettings.m_strLocalHostName);

    } // if( m_pEdtLocalHostName != nullptr )

    // <LineEdit> Local Host IP Address
    //----------------------------------

    if( m_pWdgtLocalHostAddr != nullptr && m_pEdtLocalHostAddr != nullptr )
    {
        if( i_hostSettings.m_socketType == ESocketTypeTcp )
        {
            if( !m_bLocalHostInfoLookUpInProcess )
            {
                m_pEdtLocalHostAddr->setText("---");
                m_bLocalHostInfoLookUpInProcess = true;
                QHostInfo::lookupHost( i_hostSettings.m_strLocalHostName, this, SLOT(onLocalHostInfoLookedUp(const QHostInfo&)) );
            }
            m_pWdgtLocalHostAddr->show();
        }
        else
        {
            m_pWdgtLocalHostAddr->hide();
        }
    } // if( m_pWdgtLocalHostAddr != nullptr && m_pEdtRemoteHostAddr != nullptr )

    // <LineEdit> Listen Port
    //----------------------------

    if( m_pWdgtListenPort != nullptr && m_pEdtListenPort != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pEdtListenPort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtListenPortTextChanged(const QString&)) );

        if( i_hostSettings.m_socketType == ESocketTypeInProcMsg )
        {
            m_pEdtListenPort->setValidator(nullptr);
            m_pEdtListenPort->setText("");
            m_pEdtListenPort->setEnabled(false);
            m_pWdgtListenPort->hide();
        }
        else
        {
            m_pEdtListenPort->setValidator( new QIntValidator(1,65535,this) );
            m_pEdtListenPort->setText( QString::number(i_hostSettings.m_uLocalPort) );
            m_pEdtListenPort->setEnabled(true);
            m_pWdgtListenPort->show();
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pEdtListenPort,
            /* szSignal     */ SIGNAL(textChanged(const QString&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onEdtListenPortTextChanged(const QString&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pWdgtListenPort != nullptr && m_pEdtListenPort != nullptr )

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

    // <Widget> User Login
    //--------------------

    if( m_pWdgtLineUserLogin != nullptr )
    {
        if( m_protTypeWidget == EProtocolTypeSQL )
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
        else // if( m_protTypeWidget != EProtocolTypeSQL )
        {
            m_pWdgtLineUserLogin->hide();
        }
    } // if( m_pWdgtLineUserLogin != nullptr )

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
        if( i_hostSettings == hostSettingsServer )
        {
            m_pBtnReset->setEnabled(false);
        }
        else
        {
            m_pBtnReset->setEnabled(true);
        }
    } // if( m_pBtnReset != nullptr )

    // <Button> Apply
    //---------------

    if( m_pBtnApply != nullptr )
    {
        if( i_hostSettings == hostSettingsServer )
        {
            m_pBtnApply->setEnabled(false);
        }
        else
        {
            m_pBtnApply->setEnabled(true);
        }
    } // if( m_pBtnApply != nullptr )

    // <Button> Startup/Shutdown/Abort
    //-----------------------------------

    if( m_pBtnStartup != nullptr )
    {
        if( bDbListening )
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
void CWdgtDbConnections::resetDetailControls()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

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
void CWdgtDbConnections::fillDetailControls()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "fillDetailControls",
        /* strAddInfo   */ "" );

    bool    bDbListening = false;
    QString strDbState;
    QString strConnection;
    int     iCnctsCount = 0;

    if( m_pDb != nullptr )
    {
        bDbListening  = m_pDb->isListening(m_protTypeWidget, m_hostSettingsWidget.m_socketType);
        strDbState    = m_pDb->state2Str();
        strConnection = m_pDb->getConnectionString(m_protTypeWidget, m_hostSettingsWidget.m_socketType);
        iCnctsCount   = m_pDb->getClientCnctsCount(m_protTypeWidget, m_hostSettingsWidget.m_socketType);
    }

    if( m_pEdtDetailsConnectionSettingsApplied != nullptr )
    {
        if( m_hostSettingsWidget.m_socketType == ESocketTypeInProcMsg )
        {
            m_pLblDetailsConnectionSettingsApplied->hide();
            m_pEdtDetailsConnectionSettingsApplied->hide();
        }
        else
        {
            if( m_pBtnApply != nullptr && m_pBtnApply->isEnabled() )
            {
                m_pEdtDetailsConnectionSettingsApplied->setStyleSheet("QLineEdit { color: grey; }");
            }
            else if( bDbListening )
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
        }
    } // if( m_pEdtDetailsConnectionSettingsApplied != nullptr )

    if( m_pEdtDetailsState != nullptr )
    {
        if( bDbListening )
        {
            m_pEdtDetailsState->setStyleSheet("QLineEdit { color: green; }");
            m_pEdtDetailsState->setText(strDbState);
        }
        else // if( !bDbListening )
        {
            m_pEdtDetailsState->setStyleSheet("QLineEdit { color: red; }");
            m_pEdtDetailsState->setText(strDbState);
        }
    } // if( m_pEdtDetailsState != nullptr )

    if( m_pEdtActiveConnections != nullptr )
    {
        m_pEdtActiveConnections->setText( QString::number(iCnctsCount) );
    }

    if( m_pModelSrvCltConnections != nullptr )
    {
        bool bUpdateModel = false;

        if( m_pModelSrvCltConnections->getDb() != m_pDb )
        {
            m_pModelSrvCltConnections->setDb(m_pDb);
            bUpdateModel = true;
        }
        if( m_pModelSrvCltConnections->getProtocolType() != m_protTypeWidget )
        {
            m_pModelSrvCltConnections->setProtocolType(m_protTypeWidget);
            bUpdateModel = true;
        }
        if( m_pModelSrvCltConnections->getSocketType() != m_hostSettingsWidget.m_socketType )
        {
            m_pModelSrvCltConnections->setSocketType(m_hostSettingsWidget.m_socketType);
            bUpdateModel = true;
        }
        if( bUpdateModel )
        {
            m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnProtocolType);
            m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnSocketType);

            if( m_hostSettingsWidget.m_socketType == ESocketTypeUndefined || m_hostSettingsWidget.m_socketType == ESocketTypeInProcMsg )
            {
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnSocketId);
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnSocketState);
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnServerListenPort);
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnLocalPort);
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnRemoteHostName);
                m_pTblViewSrvCltConnections->hideColumn(CModelDbConnections::EColumnRemotePort);
            }
            else
            {
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnSocketId);
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnSocketState);
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnServerListenPort);
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnLocalPort);
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnRemoteHostName);
                m_pTblViewSrvCltConnections->showColumn(CModelDbConnections::EColumnRemotePort);
            }
            m_pModelSrvCltConnections->update();
        }
    } // if( m_pModelSrvCltConnections != nullptr )

} // fillDetailControls

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbConnections::applySettings()
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

    SServerHostSettings hostSettingsServer = m_pDb->getHostSettings(m_protTypeWidget, m_hostSettingsWidget.m_socketType);

    if( m_hostSettingsWidget != hostSettingsServer )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pDb,
            /* szSignal     */ SIGNAL(settingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDbSettingsChanged(QObject*)) );

        m_pDb->setHostSettings(m_protTypeWidget, m_hostSettingsWidget);

        if( !QObject::connect(
            /* pObjSender   */ m_pDb,
            /* szSignal     */ SIGNAL(settingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDbSettingsChanged(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        // In order for the changed settings to take affect:
        m_pDb->updateHostSettings(
            /* iTimeout_ms  */ 0,
            /* bWait        */ false,
            /* iReqIdParent */ -1 );
    }

} // applySettings
