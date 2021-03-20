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

#if QT_VERSION < 0x050000
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qprogressbar.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qprogressbar.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "WidgetCentral.h"
#include "App.h"
#include "DatabaseThread.h"
#include "DbClient.h"
#include "DbClientThread.h"
#include "MainWindow.h"

#include "ZSSysGUI/ZSSysRequestSequencerWdgt.h"
#include "ZSSys/ZSSysRequestSequencer.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Apps::Test::AsyncRequests;
using namespace ZS::Apps::Test::AsyncRequests::Database;


/*******************************************************************************
class CWidgetCentral : public QWidget
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

CWidgetCentral* CWidgetCentral::s_pThis = nullptr; // singleton class

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWidgetCentral::CWidgetCentral( QWidget* i_pWdgtParent, Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pLyt(nullptr),
    // Application
    m_pLytApp(nullptr),
    m_pLblApp(nullptr),
    m_pBtnAppStartup(nullptr),
    m_pBtnAppShutdown(nullptr),
    m_pLblAppReqInProgress(nullptr),
    m_pBarAppReqInProgress(nullptr),
    m_pLedAppState(nullptr),
    // Database
    m_pLytDb(nullptr),
    m_pLblDb(nullptr),
    m_pLblDbUseBlockingMethodCalls(nullptr),
    m_pChkDbUseBlockingMethodCalls(nullptr),
    m_pLytDbBtnsLine(nullptr),
    m_pLblDbTimeout(nullptr),
    m_pCmbDbTimeout(nullptr),
    m_pBtnDbStartup(nullptr),
    m_pBtnDbShutdown(nullptr),
    m_pLblDbReqInProgress(nullptr),
    m_pBarDbReqInProgress(nullptr),
    m_pLedDbState(nullptr),
    m_pLytDbSingleShotsLine(nullptr),
    m_pLblDbModeSingleShot(nullptr),
    m_pChkDbModeSingleShot(nullptr),
    m_pBtnDbExecuteSingleShot(nullptr),
    // DbCltGUI
    m_pLytDbCltGUI(nullptr),
    m_pLblDbCltGUI(nullptr),
    m_pLblDbCltGUIUseBlockingMethodCalls(nullptr),
    m_pChkDbCltGUIUseBlockingMethodCalls(nullptr),
    m_pLblDbCltGUIConnectType(nullptr),
    m_pCmbDbCltGUIConnectType(nullptr),
    m_pLytDbCltGUIConnectBtnsLine(nullptr),
    m_pLblDbCltGUIConnectTimeout(nullptr),
    m_pCmbDbCltGUIConnectTimeout(nullptr),
    m_pBtnDbCltGUIConnect(nullptr),
    m_pBtnDbCltGUIDisconnect(nullptr),
    m_pLblDbCltGUIConnectReqInProgress(nullptr),
    m_pBarDbCltGUIConnectReqInProgress(nullptr),
    m_pLedDbCltGUIConnectState(nullptr),
    m_pLytDbCltGUIRegisterBtnsLine(nullptr),
    m_pLblDbCltGUIRegisterTimeout(nullptr),
    m_pCmbDbCltGUIRegisterTimeout(nullptr),
    m_pBtnDbCltGUIRegister(nullptr),
    m_pBtnDbCltGUIUnregister(nullptr),
    m_pLblDbCltGUIRegisterReqInProgress(nullptr),
    m_pBarDbCltGUIRegisterReqInProgress(nullptr),
    m_pLytDbCltGUISchemaBtnsLine(nullptr),
    m_pLblDbCltGUISchemaTimeout(nullptr),
    m_pCmbDbCltGUISchemaTimeout(nullptr),
    m_pBtnDbCltGUISelectSchema(nullptr),
    m_pBtnDbCltGUIUpdateSchema(nullptr),
    m_pLblDbCltGUISchemaReqInProgress(nullptr),
    m_pBarDbCltGUISchemaReqInProgress(nullptr),
    m_pLytDbCltGUIDataBtnsLine(nullptr),
    m_pLblDbCltGUIDataTimeout(nullptr),
    m_pCmbDbCltGUIDataTimeout(nullptr),
    m_pBtnDbCltGUISelectData(nullptr),
    m_pBtnDbCltGUIUpdateData(nullptr),
    m_pLblDbCltGUIDataReqInProgress(nullptr),
    m_pBarDbCltGUIDataReqInProgress(nullptr),
    // DbCltSigGen
    m_pLytDbCltSigGen(nullptr),
    m_pLblDbCltSigGen(nullptr),
    m_pLblDbCltSigGenUseBlockingMethodCalls(nullptr),
    m_pChkDbCltSigGenUseBlockingMethodCalls(nullptr),
    m_pLblDbCltSigGenConnectType(nullptr),
    m_pCmbDbCltSigGenConnectType(nullptr),
    m_pLytDbCltSigGenStartupBtnsLine(nullptr),
    m_pLblDbCltSigGenStartupTimeout(nullptr),
    m_pCmbDbCltSigGenStartupTimeout(nullptr),
    m_pBtnDbCltSigGenStartup(nullptr),
    m_pBtnDbCltSigGenShutdown(nullptr),
    m_pLblDbCltSigGenStartupReqInProgress(nullptr),
    m_pBarDbCltSigGenStartupReqInProgress(nullptr),
    m_pLytDbCltSigGenConnectBtnsLine(nullptr),
    m_pLblDbCltSigGenConnectTimeout(nullptr),
    m_pCmbDbCltSigGenConnectTimeout(nullptr),
    m_pBtnDbCltSigGenConnect(nullptr),
    m_pBtnDbCltSigGenDisconnect(nullptr),
    m_pLblDbCltSigGenConnectReqInProgress(nullptr),
    m_pBarDbCltSigGenConnectReqInProgress(nullptr),
    m_pLedDbCltSigGenState(nullptr),
    m_pLytDbCltSigGenRegisterBtnsLine(nullptr),
    m_pLblDbCltSigGenRegisterTimeout(nullptr),
    m_pCmbDbCltSigGenRegisterTimeout(nullptr),
    m_pBtnDbCltSigGenRegister(nullptr),
    m_pBtnDbCltSigGenUnregister(nullptr),
    m_pLblDbCltSigGenRegisterReqInProgress(nullptr),
    m_pBarDbCltSigGenRegisterReqInProgress(nullptr),
    m_pLytDbCltSigGenSchemaBtnsLine(nullptr),
    m_pLblDbCltSigGenSchemaTimeout(nullptr),
    m_pCmbDbCltSigGenSchemaTimeout(nullptr),
    m_pBtnDbCltSigGenSelectSchema(nullptr),
    m_pBtnDbCltSigGenUpdateSchema(nullptr),
    m_pLblDbCltSigGenSchemaReqInProgress(nullptr),
    m_pBarDbCltSigGenSchemaReqInProgress(nullptr),
    m_pLytDbCltSigGenDataBtnsLine(nullptr),
    m_pLblDbCltSigGenDataTimeout(nullptr),
    m_pCmbDbCltSigGenDataTimeout(nullptr),
    m_pBtnDbCltSigGenSelectData(nullptr),
    m_pBtnDbCltSigGenUpdateData(nullptr),
    m_pLblDbCltSigGenDataReqInProgress(nullptr),
    m_pBarDbCltSigGenDataReqInProgress(nullptr),
    // DbCltCapture
    m_pLytDbCltCapture(nullptr),
    m_pLblDbCltCapture(nullptr),
    m_pLblDbCltCaptureUseBlockingMethodCalls(nullptr),
    m_pChkDbCltCaptureUseBlockingMethodCalls(nullptr),
    m_pLblDbCltCaptureConnectType(nullptr),
    m_pCmbDbCltCaptureConnectType(nullptr),
    m_pLytDbCltCaptureStartupBtnsLine(nullptr),
    m_pLblDbCltCaptureStartupTimeout(nullptr),
    m_pCmbDbCltCaptureStartupTimeout(nullptr),
    m_pBtnDbCltCaptureStartup(nullptr),
    m_pBtnDbCltCaptureShutdown(nullptr),
    m_pLblDbCltCaptureStartupReqInProgress(nullptr),
    m_pBarDbCltCaptureStartupReqInProgress(nullptr),
    m_pLytDbCltCaptureConnectBtnsLine(nullptr),
    m_pLblDbCltCaptureConnectTimeout(nullptr),
    m_pCmbDbCltCaptureConnectTimeout(nullptr),
    m_pBtnDbCltCaptureConnect(nullptr),
    m_pBtnDbCltCaptureDisconnect(nullptr),
    m_pLblDbCltCaptureConnectReqInProgress(nullptr),
    m_pBarDbCltCaptureConnectReqInProgress(nullptr),
    m_pLedDbCltCaptureState(nullptr),
    m_pLytDbCltCaptureRegisterBtnsLine(nullptr),
    m_pLblDbCltCaptureRegisterTimeout(nullptr),
    m_pCmbDbCltCaptureRegisterTimeout(nullptr),
    m_pBtnDbCltCaptureRegister(nullptr),
    m_pBtnDbCltCaptureUnregister(nullptr),
    m_pLblDbCltCaptureRegisterReqInProgress(nullptr),
    m_pBarDbCltCaptureRegisterReqInProgress(nullptr),
    m_pLytDbCltCaptureSchemaBtnsLine(nullptr),
    m_pLblDbCltCaptureSchemaTimeout(nullptr),
    m_pCmbDbCltCaptureSchemaTimeout(nullptr),
    m_pBtnDbCltCaptureSelectSchema(nullptr),
    m_pBtnDbCltCaptureUpdateSchema(nullptr),
    m_pLblDbCltCaptureSchemaReqInProgress(nullptr),
    m_pBarDbCltCaptureSchemaReqInProgress(nullptr),
    m_pLytDbCltCaptureDataBtnsLine(nullptr),
    m_pLblDbCltCaptureDataTimeout(nullptr),
    m_pCmbDbCltCaptureDataTimeout(nullptr),
    m_pBtnDbCltCaptureSelectData(nullptr),
    m_pBtnDbCltCaptureUpdateData(nullptr),
    m_pLblDbCltCaptureDataReqInProgress(nullptr),
    m_pBarDbCltCaptureDataReqInProgress(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("CentralWidget");

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    QFrame* pLine;

    int idxCmb;

    int cxSpacing               =  10;
    int cxLblWidthClm1          = 100;
    int cxLblWidthClm2          =  70;
    //int cxLblWidthClm3        =  64;
    int cxBtnWidth              =  84;
    int cxProgressBarWidth      =  120;
    int cxLblWidthReqInProgress =  140;

    // <Line> Application
    //==========================================================================

    m_pLytApp = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytApp);

    // <Label> Application
    //--------------------

    m_pLblApp = new QLabel("Application:");
    m_pLblApp->setFixedWidth(cxLblWidthClm1);
    m_pLytApp->addWidget(m_pLblApp);
    m_pLytApp->addSpacing(cxSpacing);

    // <Spacing> for UseBlockingMethodCalls check boxes in other lines
    //----------------------------------------------------------------

    //int cxSpacingClm1BtnStartup =
    //    cxLblWidthClm2 +
    //    cxSpacing +
    //    13 + // check box width
    //    2*m_pLytApp->spacing();
    //    //cxLblWidthClm3 +
    //    //cxSpacing +
    //    //13 + // check box width
    //    //2*m_pLytApp->spacing() +
    //    //cxBtnWidth + // Button Execute
    //    //2*cxSpacing +
    //    //m_pLytApp->spacing();

    //m_pLytApp->addSpacing(cxSpacingClm1BtnStartup);

    // <ComboBox> Timeout
    //-------------------

    QLabel* pLblAppTimeout = new QLabel();
    pLblAppTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytApp->addWidget(pLblAppTimeout);
    QLabel* pCmbDbTimeout = new QLabel();
    pCmbDbTimeout->setFixedWidth(cxBtnWidth);
    m_pLytApp->addWidget(pCmbDbTimeout);
    m_pLytApp->addSpacing(cxSpacing);

    // <Button> Startup
    //-----------------

    m_pBtnAppStartup = new QPushButton("Startup");
    m_pBtnAppStartup->setFixedWidth(cxBtnWidth);
    m_pLytApp->addWidget(m_pBtnAppStartup);
    m_pLytApp->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnAppStartup,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnAppStartupClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Shutdown
    //------------------

    m_pBtnAppShutdown = new QPushButton("Shutdown");
    m_pBtnAppShutdown->setFixedWidth(cxBtnWidth);
    m_pLytApp->addWidget(m_pBtnAppShutdown);
    m_pLytApp->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnAppShutdown,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnAppShutdownClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarAppReqInProgress = new QProgressBar();
    m_pBarAppReqInProgress->setMaximumHeight(12);
    m_pBarAppReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarAppReqInProgress->setRange(0,100);
    m_pBarAppReqInProgress->setTextVisible(false);
    m_pBarAppReqInProgress->installEventFilter(this);
    m_pLytApp->addWidget(m_pBarAppReqInProgress);
    m_pBarAppReqInProgress->hide();

    m_pLblAppReqInProgress = new QLabel("Ready");
    m_pLblAppReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblAppReqInProgress->installEventFilter(this);
    m_pLytApp->addWidget(m_pLblAppReqInProgress);
    m_pLytApp->addStretch();

    updateAppReqInProgressStatus( "Ready", 100 );

    // <Led> State
    //----------------------------

    m_pLedAppState = new QLabel();
    m_pLedAppState->setFixedWidth(20);
    m_pLedAppState->setFixedHeight(20);
    m_pLedAppState->setScaledContents(true);
    m_pLytApp->addWidget(m_pLedAppState);

    if( CApplication::GetInstance()->state() == CApplication::EStateCreated )
    {
        m_pLedAppState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
    }
    else if( CApplication::GetInstance()->state() == CApplication::EStateRunning )
    {
        m_pLedAppState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
    }
    else
    {
        m_pLedAppState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
    }

    if( !QObject::connect(
        /* pObjSender   */ CApplication::GetInstance(),
        /* szSignal     */ SIGNAL(stateChanged(int,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAppStateChanged(int,int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

    // <Line> Database
    //==========================================================================

    m_pLytDb = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDb);

    // <Label> Database
    //--------------------

    m_pLblDb = new QLabel("Database:");
    m_pLblDb->setFixedWidth(cxLblWidthClm1);
    m_pLytDb->addWidget(m_pLblDb);
    m_pLytDb->addSpacing(cxSpacing);

    // <CheckBox> UseBlockingMethodCalls
    //----------------------------------

    m_pLblDbUseBlockingMethodCalls = new QLabel("Blocking Calls:");
    m_pLblDbUseBlockingMethodCalls->setFixedWidth(cxLblWidthClm2);
    m_pLytDb->addWidget(m_pLblDbUseBlockingMethodCalls);
    m_pChkDbUseBlockingMethodCalls = new QCheckBox();
    m_pChkDbUseBlockingMethodCalls->setChecked( CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDb) );
    m_pLytDb->addWidget(m_pChkDbUseBlockingMethodCalls);
    m_pLytDb->addStretch();

    if( !QObject::connect(
        /* pObjSender   */ m_pChkDbUseBlockingMethodCalls,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkDbUseBlockingMethodCallsStateChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Line> Buttons
    //---------------

    m_pLytDbBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbBtnsLine = new QLabel("");
    pLblDbBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbBtnsLine->addWidget(pLblDbBtnsLine);
    m_pLytDbBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbTimeout = new QLabel("Timeout:");
    m_pLblDbTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbBtnsLine->addWidget(m_pLblDbTimeout);
    m_pCmbDbTimeout = new QComboBox();
    m_pCmbDbTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbBtnsLine->addWidget(m_pCmbDbTimeout);
    m_pLytDbBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDb) );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Startup
    //-----------------

    m_pBtnDbStartup = new QPushButton("Startup");
    m_pBtnDbStartup->setFixedWidth(cxBtnWidth);
    m_pLytDbBtnsLine->addWidget(m_pBtnDbStartup);
    m_pLytDbBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbStartup,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbStartupClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Shutdown
    //------------------

    m_pBtnDbShutdown = new QPushButton("Shutdown");
    m_pBtnDbShutdown->setFixedWidth(cxBtnWidth);
    m_pLytDbBtnsLine->addWidget(m_pBtnDbShutdown);
    m_pLytDbBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbShutdown,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbShutdownClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbReqInProgress = new QProgressBar();
    m_pBarDbReqInProgress->setMaximumHeight(12);
    m_pBarDbReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbReqInProgress->setRange(0,100);
    m_pBarDbReqInProgress->setTextVisible(false);
    m_pBarDbReqInProgress->installEventFilter(this);
    m_pLytDbBtnsLine->addWidget(m_pBarDbReqInProgress);
    m_pBarDbReqInProgress->hide();

    m_pLblDbReqInProgress = new QLabel("Ready");
    m_pLblDbReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbReqInProgress->installEventFilter(this);
    m_pLytDbBtnsLine->addWidget(m_pLblDbReqInProgress);
    m_pLytDbBtnsLine->addStretch();

    updateDbReqInProgressStatus( "Ready", 100 );

    // <Led> State
    //----------------------------

    m_pLedDbState = new QLabel();
    m_pLedDbState->setFixedWidth(20);
    m_pLedDbState->setFixedHeight(20);
    m_pLedDbState->setScaledContents(true);
    m_pLytDbBtnsLine->addWidget(m_pLedDbState);

    if( CApplication::GetInstance()->getDbThread() == nullptr )
    {
        m_pLedDbState->setText("---");
    }
    else // if( CApplication::GetInstance()->getDbThread() != nullptr )
    {
        if( CApplication::GetInstance()->getDbThread()->state() == Database::EStateCreated )
        {
            m_pLedDbState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
        }
        else if( CApplication::GetInstance()->getDbThread()->state() == Database::EStateRunning )
        {
            m_pLedDbState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
        }
        else
        {
            m_pLedDbState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
        }
        if( !QObject::connect(
            /* pObjSender   */ CApplication::GetInstance()->getDbThread(),
            /* szSignal     */ SIGNAL(stateChanged(int,int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDbStateChanged(int,int)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( CApplication::GetInstance()->getDbThread() != nullptr )

    // <Line> Single Shots
    //--------------------

    m_pLytDbSingleShotsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbSingleShotsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbSingleShotsLine = new QLabel("");
    pLblDbSingleShotsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbSingleShotsLine->addWidget(pLblDbSingleShotsLine);
    m_pLytDbSingleShotsLine->addSpacing(cxSpacing);

    // <CheckBox> Single Shot Mode
    //----------------------------------

    m_pLblDbModeSingleShot = new QLabel("Single Shots:");
    m_pLblDbModeSingleShot->setFixedWidth(cxLblWidthClm2);
    m_pLytDbSingleShotsLine->addWidget(m_pLblDbModeSingleShot);
    m_pChkDbModeSingleShot = new QCheckBox();
    m_pChkDbModeSingleShot->setChecked( CApplication::GetInstance()->isDbReqModeSingleShot() );
    m_pLytDbSingleShotsLine->addWidget(m_pChkDbModeSingleShot);
    m_pLytDbSingleShotsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pChkDbModeSingleShot,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkDbModeSingleShotStateChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Execute Single Shot
    //-----------------------------

    m_pLytDbSingleShotsLine->addSpacing(cxBtnWidth-13);

    m_pBtnDbExecuteSingleShot = new QPushButton("Execute");
    m_pBtnDbExecuteSingleShot->setFixedWidth(cxBtnWidth);
    m_pLytDbSingleShotsLine->addWidget(m_pBtnDbExecuteSingleShot);
    m_pLytDbSingleShotsLine->addStretch();

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbExecuteSingleShot,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbExecuteSingleShotClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

    // <Line> DbClient GUI
    //==========================================================================

    m_pLytDbCltGUI = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltGUI);

    // <Label> DbClient
    //--------------------

    m_pLblDbCltGUI = new QLabel("DbClient GUI:");
    m_pLblDbCltGUI->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltGUI->addWidget(m_pLblDbCltGUI);
    m_pLytDbCltGUI->addSpacing(cxSpacing);

    // <CheckBox> UseBlockingMethodCalls
    //----------------------------------

    m_pLblDbCltGUIUseBlockingMethodCalls = new QLabel("Blocking Calls:");
    m_pLblDbCltGUIUseBlockingMethodCalls->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltGUI->addWidget(m_pLblDbCltGUIUseBlockingMethodCalls);
    m_pChkDbCltGUIUseBlockingMethodCalls = new QCheckBox();
    m_pChkDbCltGUIUseBlockingMethodCalls->setChecked( CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltGUI) );
    m_pLytDbCltGUI->addWidget(m_pChkDbCltGUIUseBlockingMethodCalls);
    m_pLytDbCltGUI->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pChkDbCltGUIUseBlockingMethodCalls,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkDbCltGUIUseBlockingMethodCallsStateChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <ComboBox> Connection Type
    //---------------------------

    m_pLblDbCltGUIConnectType = new QLabel("Cnct. Type:");
    m_pLblDbCltGUIConnectType->setFixedWidth(cxBtnWidth-12-m_pLytDbCltGUI->spacing());
    m_pLytDbCltGUI->addWidget(m_pLblDbCltGUIConnectType);
    m_pCmbDbCltGUIConnectType = new QComboBox();
    m_pCmbDbCltGUIConnectType->setFixedWidth(2*cxBtnWidth+cxSpacing+m_pLytDbCltGUI->spacing()-2);
    m_pLytDbCltGUI->addWidget(m_pCmbDbCltGUIConnectType);
    m_pLytDbCltGUI->addStretch();

    for( idxCmb = 0; idxCmb < Ipc::ESocketTypeCount; idxCmb++ )
    {
        m_pCmbDbCltGUIConnectType->addItem( Ipc::socketType2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltGUIConnectType->setCurrentIndex( CApplication::GetInstance()->getSocketType(c_strObjNameDbCltGUI) );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltGUIConnectType,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltGUIConnectTypeCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Line> Connect/Disconnect Buttons
    //----------------------------------

    m_pLytDbCltGUIConnectBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltGUIConnectBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltGUIConnectBtnsLine = new QLabel("");
    pLblDbCltGUIConnectBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltGUIConnectBtnsLine->addWidget(pLblDbCltGUIConnectBtnsLine);
    m_pLytDbCltGUIConnectBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltGUIConnectTimeout = new QLabel("Timeout:");
    m_pLblDbCltGUIConnectTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltGUIConnectBtnsLine->addWidget(m_pLblDbCltGUIConnectTimeout);
    m_pCmbDbCltGUIConnectTimeout = new QComboBox();
    m_pCmbDbCltGUIConnectTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUIConnectBtnsLine->addWidget(m_pCmbDbCltGUIConnectTimeout);
    m_pLytDbCltGUIConnectBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltGUIConnectTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltGUIConnectTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Connect") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltGUIConnectTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltGUIConnectTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Connect
    //-----------------

    m_pBtnDbCltGUIConnect = new QPushButton("Connect");
    m_pBtnDbCltGUIConnect->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUIConnectBtnsLine->addWidget(m_pBtnDbCltGUIConnect);
    m_pLytDbCltGUIConnectBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltGUIConnect,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltGUIConnectClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Disconnect
    //--------------------

    m_pBtnDbCltGUIDisconnect = new QPushButton("Disconnect");
    m_pBtnDbCltGUIDisconnect->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUIConnectBtnsLine->addWidget(m_pBtnDbCltGUIDisconnect);
    m_pLytDbCltGUIConnectBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltGUIDisconnect,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltGUIDisconnectClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltGUIConnectReqInProgress = new QProgressBar();
    m_pBarDbCltGUIConnectReqInProgress->setMaximumHeight(12);
    m_pBarDbCltGUIConnectReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltGUIConnectReqInProgress->setRange(0,100);
    m_pBarDbCltGUIConnectReqInProgress->setTextVisible(false);
    m_pBarDbCltGUIConnectReqInProgress->installEventFilter(this);
    m_pLytDbCltGUIConnectBtnsLine->addWidget(m_pBarDbCltGUIConnectReqInProgress);
    m_pBarDbCltGUIConnectReqInProgress->hide();

    m_pLblDbCltGUIConnectReqInProgress = new QLabel("Ready");
    m_pLblDbCltGUIConnectReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltGUIConnectReqInProgress->installEventFilter(this);
    m_pLytDbCltGUIConnectBtnsLine->addWidget(m_pLblDbCltGUIConnectReqInProgress);
    m_pLytDbCltGUIConnectBtnsLine->addStretch();

    updateDbCltGUIConnectReqInProgressStatus( "Ready", 100 );

    // <Led> State
    //----------------------------

    m_pLedDbCltGUIConnectState = new QLabel();
    m_pLedDbCltGUIConnectState->setFixedWidth(20);
    m_pLedDbCltGUIConnectState->setFixedHeight(20);
    m_pLedDbCltGUIConnectState->setScaledContents(true);
    m_pLytDbCltGUIConnectBtnsLine->addWidget(m_pLedDbCltGUIConnectState);

    if( CApplication::GetInstance()->getDbClientGUI() == nullptr )
    {
        m_pLedDbCltGUIConnectState->setText("---");
    }
    else // if( CApplication::GetInstance()->getDbClientGUIT() != nullptr )
    {
        if( CApplication::GetInstance()->getDbClientGUI()->state() == Database::EStateDisconnected )
        {
            m_pLedDbCltGUIConnectState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
        }
        else if( CApplication::GetInstance()->getDbClientGUI()->state() == Database::EStateConnected )
        {
            m_pLedDbCltGUIConnectState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
        }
        else
        {
            m_pLedDbCltGUIConnectState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
        }
        if( !QObject::connect(
            /* pObjSender   */ CApplication::GetInstance()->getDbClientGUI(),
            /* szSignal     */ SIGNAL(stateChanged(int,int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDbCltGUIStateChanged(int,int)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( CApplication::GetInstance()->getDbClientThread() != nullptr )

    // <Line> Register/Unregister Buttons
    //----------------------------------

    m_pLytDbCltGUIRegisterBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltGUIRegisterBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltGUIRegisterBtnsLine = new QLabel("");
    pLblDbCltGUIRegisterBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltGUIRegisterBtnsLine->addWidget(pLblDbCltGUIRegisterBtnsLine);
    m_pLytDbCltGUIRegisterBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltGUIRegisterTimeout = new QLabel("Timeout:");
    m_pLblDbCltGUIRegisterTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltGUIRegisterBtnsLine->addWidget(m_pLblDbCltGUIRegisterTimeout);
    m_pCmbDbCltGUIRegisterTimeout = new QComboBox();
    m_pCmbDbCltGUIRegisterTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUIRegisterBtnsLine->addWidget(m_pCmbDbCltGUIRegisterTimeout);
    m_pLytDbCltGUIRegisterBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltGUIRegisterTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltGUIRegisterTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Register") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltGUIRegisterTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltGUIRegisterTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Register
    //-----------------

    m_pBtnDbCltGUIRegister = new QPushButton("Register");
    m_pBtnDbCltGUIRegister->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUIRegisterBtnsLine->addWidget(m_pBtnDbCltGUIRegister);
    m_pLytDbCltGUIRegisterBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltGUIRegister,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltGUIRegisterClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Unregister
    //--------------------

    m_pBtnDbCltGUIUnregister = new QPushButton("Unregister");
    m_pBtnDbCltGUIUnregister->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUIRegisterBtnsLine->addWidget(m_pBtnDbCltGUIUnregister);
    m_pLytDbCltGUIRegisterBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltGUIUnregister,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltGUIUnregisterClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltGUIRegisterReqInProgress = new QProgressBar();
    m_pBarDbCltGUIRegisterReqInProgress->setMaximumHeight(12);
    m_pBarDbCltGUIRegisterReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltGUIRegisterReqInProgress->setRange(0,100);
    m_pBarDbCltGUIRegisterReqInProgress->setTextVisible(false);
    m_pBarDbCltGUIRegisterReqInProgress->installEventFilter(this);
    m_pLytDbCltGUIRegisterBtnsLine->addWidget(m_pBarDbCltGUIRegisterReqInProgress);
    m_pBarDbCltGUIRegisterReqInProgress->hide();

    m_pLblDbCltGUIRegisterReqInProgress = new QLabel("Ready");
    m_pLblDbCltGUIRegisterReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltGUIRegisterReqInProgress->installEventFilter(this);
    m_pLytDbCltGUIRegisterBtnsLine->addWidget(m_pLblDbCltGUIRegisterReqInProgress);
    m_pLytDbCltGUIRegisterBtnsLine->addStretch();

    updateDbCltGUIRegisterReqInProgressStatus( "Ready", 100 );

    // <Line> Schema Buttons
    //-----------------------

    m_pLytDbCltGUISchemaBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltGUISchemaBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltGUISchemaBtnsLine = new QLabel("");
    pLblDbCltGUISchemaBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltGUISchemaBtnsLine->addWidget(pLblDbCltGUISchemaBtnsLine);
    m_pLytDbCltGUISchemaBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltGUISchemaTimeout = new QLabel("Timeout:");
    m_pLblDbCltGUISchemaTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltGUISchemaBtnsLine->addWidget(m_pLblDbCltGUISchemaTimeout);
    m_pCmbDbCltGUISchemaTimeout = new QComboBox();
    m_pCmbDbCltGUISchemaTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUISchemaBtnsLine->addWidget(m_pCmbDbCltGUISchemaTimeout);
    m_pLytDbCltGUISchemaBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltGUISchemaTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltGUISchemaTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Schema") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltGUISchemaTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltGUISchemaTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> SelectSchema
    //----------------------

    m_pBtnDbCltGUISelectSchema = new QPushButton("Select Schema");
    m_pBtnDbCltGUISelectSchema->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUISchemaBtnsLine->addWidget(m_pBtnDbCltGUISelectSchema);
    m_pLytDbCltGUISchemaBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltGUISelectSchema,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltGUISelectSchemaClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> UpdateSchema
    //---------------------

    m_pBtnDbCltGUIUpdateSchema = new QPushButton("Update Schema");
    m_pBtnDbCltGUIUpdateSchema->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUISchemaBtnsLine->addWidget(m_pBtnDbCltGUIUpdateSchema);
    m_pLytDbCltGUISchemaBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltGUIUpdateSchema,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltGUIUpdateSchemaClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltGUISchemaReqInProgress = new QProgressBar();
    m_pBarDbCltGUISchemaReqInProgress->setMaximumHeight(12);
    m_pBarDbCltGUISchemaReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltGUISchemaReqInProgress->setRange(0,100);
    m_pBarDbCltGUISchemaReqInProgress->setTextVisible(false);
    m_pBarDbCltGUISchemaReqInProgress->installEventFilter(this);
    m_pLytDbCltGUISchemaBtnsLine->addWidget(m_pBarDbCltGUISchemaReqInProgress);
    m_pBarDbCltGUISchemaReqInProgress->hide();

    m_pLblDbCltGUISchemaReqInProgress = new QLabel("Ready");
    m_pLblDbCltGUISchemaReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltGUISchemaReqInProgress->installEventFilter(this);
    m_pLytDbCltGUISchemaBtnsLine->addWidget(m_pLblDbCltGUISchemaReqInProgress);
    m_pLytDbCltGUISchemaBtnsLine->addStretch();

    updateDbCltGUISchemaReqInProgressStatus( "Ready", 100 );

    // <Line> Data
    //------------

    m_pLytDbCltGUIDataBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltGUIDataBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltGUIDataBtnsLine = new QLabel("");
    pLblDbCltGUIDataBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltGUIDataBtnsLine->addWidget(pLblDbCltGUIDataBtnsLine);
    m_pLytDbCltGUIDataBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltGUIDataTimeout = new QLabel("Timeout:");
    m_pLblDbCltGUIDataTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltGUIDataBtnsLine->addWidget(m_pLblDbCltGUIDataTimeout);
    m_pCmbDbCltGUIDataTimeout = new QComboBox();
    m_pCmbDbCltGUIDataTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUIDataBtnsLine->addWidget(m_pCmbDbCltGUIDataTimeout);
    m_pLytDbCltGUIDataBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltGUIDataTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltGUIDataTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Data") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltGUIDataTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltGUIDataTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> SelectData
    //---------------------

    m_pBtnDbCltGUISelectData = new QPushButton("Select Data");
    m_pBtnDbCltGUISelectData->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUIDataBtnsLine->addWidget(m_pBtnDbCltGUISelectData);
    m_pLytDbCltGUIDataBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltGUISelectData,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltGUISelectDataClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> UpdateData
    //--------------------

    m_pBtnDbCltGUIUpdateData = new QPushButton("Update Data");
    m_pBtnDbCltGUIUpdateData->setFixedWidth(cxBtnWidth);
    m_pLytDbCltGUIDataBtnsLine->addWidget(m_pBtnDbCltGUIUpdateData);
    m_pLytDbCltGUIDataBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltGUIUpdateData,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltGUIUpdateDataClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltGUIDataReqInProgress = new QProgressBar();
    m_pBarDbCltGUIDataReqInProgress->setMaximumHeight(12);
    m_pBarDbCltGUIDataReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltGUIDataReqInProgress->setRange(0,100);
    m_pBarDbCltGUIDataReqInProgress->setTextVisible(false);
    m_pBarDbCltGUIDataReqInProgress->installEventFilter(this);
    m_pLytDbCltGUIDataBtnsLine->addWidget(m_pBarDbCltGUIDataReqInProgress);
    m_pBarDbCltGUIDataReqInProgress->hide();

    m_pLblDbCltGUIDataReqInProgress = new QLabel("Ready");
    m_pLblDbCltGUIDataReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltGUIDataReqInProgress->installEventFilter(this);
    m_pLytDbCltGUIDataBtnsLine->addWidget(m_pLblDbCltGUIDataReqInProgress);
    m_pLytDbCltGUIDataBtnsLine->addStretch();

    updateDbCltGUIDataReqInProgressStatus( "Ready", 100 );

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

    // <Line> DbClient SigGen
    //==========================================================================

    m_pLytDbCltSigGen = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltSigGen);

    // <Label> DbClient
    //--------------------

    m_pLblDbCltSigGen = new QLabel("DbClient SigGen:");
    m_pLblDbCltSigGen->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltSigGen->addWidget(m_pLblDbCltSigGen);
    m_pLytDbCltSigGen->addSpacing(cxSpacing);

    // <CheckBox> UseBlockingMethodCalls
    //----------------------------------

    m_pLblDbCltSigGenUseBlockingMethodCalls = new QLabel("Blocking Calls:");
    m_pLblDbCltSigGenUseBlockingMethodCalls->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltSigGen->addWidget(m_pLblDbCltSigGenUseBlockingMethodCalls);
    m_pChkDbCltSigGenUseBlockingMethodCalls = new QCheckBox();
    m_pChkDbCltSigGenUseBlockingMethodCalls->setChecked( CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltSigGen) );
    m_pLytDbCltSigGen->addWidget(m_pChkDbCltSigGenUseBlockingMethodCalls);
    m_pLytDbCltSigGen->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pChkDbCltSigGenUseBlockingMethodCalls,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkDbCltSigGenUseBlockingMethodCallsStateChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <ComboBox> Connection Type
    //---------------------------

    m_pLblDbCltSigGenConnectType = new QLabel("Cnct. Type:");
    m_pLblDbCltSigGenConnectType->setFixedWidth(cxBtnWidth-12-m_pLytDbCltGUI->spacing());
    m_pLytDbCltSigGen->addWidget(m_pLblDbCltSigGenConnectType);
    m_pCmbDbCltSigGenConnectType = new QComboBox();
    m_pCmbDbCltSigGenConnectType->setFixedWidth(2*cxBtnWidth+cxSpacing+m_pLytDbCltGUI->spacing()-2);
    m_pLytDbCltSigGen->addWidget(m_pCmbDbCltSigGenConnectType);
    m_pLytDbCltSigGen->addStretch();

    for( idxCmb = 0; idxCmb < Ipc::ESocketTypeCount; idxCmb++ )
    {
        m_pCmbDbCltSigGenConnectType->addItem( Ipc::socketType2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltSigGenConnectType->setCurrentIndex( CApplication::GetInstance()->getSocketType(c_strObjNameDbCltSigGen) );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltSigGenConnectType,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltSigGenConnectTypeCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Line> Startup/Shutdown Buttons
    //----------------------------------

    m_pLytDbCltSigGenStartupBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltSigGenStartupBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltSigGenStartupBtnsLine = new QLabel("");
    pLblDbCltSigGenStartupBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltSigGenStartupBtnsLine->addWidget(pLblDbCltSigGenStartupBtnsLine);
    m_pLytDbCltSigGenStartupBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltSigGenStartupTimeout = new QLabel("Timeout:");
    m_pLblDbCltSigGenStartupTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltSigGenStartupBtnsLine->addWidget(m_pLblDbCltSigGenStartupTimeout);
    m_pCmbDbCltSigGenStartupTimeout = new QComboBox();
    m_pCmbDbCltSigGenStartupTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenStartupBtnsLine->addWidget(m_pCmbDbCltSigGenStartupTimeout);
    m_pLytDbCltSigGenStartupBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltSigGenStartupTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltSigGenStartupTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Startup") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltSigGenStartupTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltSigGenStartupTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Startup
    //-----------------

    m_pBtnDbCltSigGenStartup = new QPushButton("Startup");
    m_pBtnDbCltSigGenStartup->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenStartupBtnsLine->addWidget(m_pBtnDbCltSigGenStartup);
    m_pLytDbCltSigGenStartupBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltSigGenStartup,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltSigGenStartupClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Shutdown
    //--------------------

    m_pBtnDbCltSigGenShutdown = new QPushButton("Shutdown");
    m_pBtnDbCltSigGenShutdown->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenStartupBtnsLine->addWidget(m_pBtnDbCltSigGenShutdown);
    m_pLytDbCltSigGenStartupBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltSigGenShutdown,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltSigGenShutdownClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltSigGenStartupReqInProgress = new QProgressBar();
    m_pBarDbCltSigGenStartupReqInProgress->setMaximumHeight(12);
    m_pBarDbCltSigGenStartupReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltSigGenStartupReqInProgress->setRange(0,100);
    m_pBarDbCltSigGenStartupReqInProgress->setTextVisible(false);
    m_pBarDbCltSigGenStartupReqInProgress->installEventFilter(this);
    m_pLytDbCltSigGenStartupBtnsLine->addWidget(m_pBarDbCltSigGenStartupReqInProgress);
    m_pBarDbCltSigGenStartupReqInProgress->hide();

    m_pLblDbCltSigGenStartupReqInProgress = new QLabel("Ready");
    m_pLblDbCltSigGenStartupReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltSigGenStartupReqInProgress->installEventFilter(this);
    m_pLytDbCltSigGenStartupBtnsLine->addWidget(m_pLblDbCltSigGenStartupReqInProgress);
    m_pLytDbCltSigGenStartupBtnsLine->addStretch();

    updateDbCltSigGenStartupReqInProgressStatus( "Ready", 100 );

    // <Line> Connect/Disconnect Buttons
    //----------------------------------

    m_pLytDbCltSigGenConnectBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltSigGenConnectBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltSigGenConnectBtnsLine = new QLabel("");
    pLblDbCltSigGenConnectBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltSigGenConnectBtnsLine->addWidget(pLblDbCltSigGenConnectBtnsLine);
    m_pLytDbCltSigGenConnectBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltSigGenConnectTimeout = new QLabel("Timeout:");
    m_pLblDbCltSigGenConnectTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltSigGenConnectBtnsLine->addWidget(m_pLblDbCltSigGenConnectTimeout);
    m_pCmbDbCltSigGenConnectTimeout = new QComboBox();
    m_pCmbDbCltSigGenConnectTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenConnectBtnsLine->addWidget(m_pCmbDbCltSigGenConnectTimeout);
    m_pLytDbCltSigGenConnectBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltSigGenConnectTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltSigGenConnectTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Connect") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltSigGenConnectTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltSigGenConnectTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Connect
    //-----------------

    m_pBtnDbCltSigGenConnect = new QPushButton("Connect");
    m_pBtnDbCltSigGenConnect->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenConnectBtnsLine->addWidget(m_pBtnDbCltSigGenConnect);
    m_pLytDbCltSigGenConnectBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltSigGenConnect,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltSigGenConnectClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Disconnect
    //--------------------

    m_pBtnDbCltSigGenDisconnect = new QPushButton("Disconnect");
    m_pBtnDbCltSigGenDisconnect->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenConnectBtnsLine->addWidget(m_pBtnDbCltSigGenDisconnect);
    m_pLytDbCltSigGenConnectBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltSigGenDisconnect,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltSigGenDisconnectClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltSigGenConnectReqInProgress = new QProgressBar();
    m_pBarDbCltSigGenConnectReqInProgress->setMaximumHeight(12);
    m_pBarDbCltSigGenConnectReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltSigGenConnectReqInProgress->setRange(0,100);
    m_pBarDbCltSigGenConnectReqInProgress->setTextVisible(false);
    m_pBarDbCltSigGenConnectReqInProgress->installEventFilter(this);
    m_pLytDbCltSigGenConnectBtnsLine->addWidget(m_pBarDbCltSigGenConnectReqInProgress);
    m_pBarDbCltSigGenConnectReqInProgress->hide();

    m_pLblDbCltSigGenConnectReqInProgress = new QLabel("Ready");
    m_pLblDbCltSigGenConnectReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltSigGenConnectReqInProgress->installEventFilter(this);
    m_pLytDbCltSigGenConnectBtnsLine->addWidget(m_pLblDbCltSigGenConnectReqInProgress);
    m_pLytDbCltSigGenConnectBtnsLine->addStretch();

    updateDbCltSigGenConnectReqInProgressStatus( "Ready", 100 );

    // <Line> Register/Unregister Buttons
    //----------------------------------

    m_pLytDbCltSigGenRegisterBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltSigGenRegisterBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltSigGenRegisterBtnsLine = new QLabel("");
    pLblDbCltSigGenRegisterBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltSigGenRegisterBtnsLine->addWidget(pLblDbCltSigGenRegisterBtnsLine);
    m_pLytDbCltSigGenRegisterBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltSigGenRegisterTimeout = new QLabel("Timeout:");
    m_pLblDbCltSigGenRegisterTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltSigGenRegisterBtnsLine->addWidget(m_pLblDbCltSigGenRegisterTimeout);
    m_pCmbDbCltSigGenRegisterTimeout = new QComboBox();
    m_pCmbDbCltSigGenRegisterTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenRegisterBtnsLine->addWidget(m_pCmbDbCltSigGenRegisterTimeout);
    m_pLytDbCltSigGenRegisterBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltSigGenRegisterTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltSigGenRegisterTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Register") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltSigGenRegisterTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltSigGenRegisterTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Register
    //-----------------

    m_pBtnDbCltSigGenRegister = new QPushButton("Register");
    m_pBtnDbCltSigGenRegister->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenRegisterBtnsLine->addWidget(m_pBtnDbCltSigGenRegister);
    m_pLytDbCltSigGenRegisterBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltSigGenRegister,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltSigGenRegisterClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Unregister
    //--------------------

    m_pBtnDbCltSigGenUnregister = new QPushButton("Unregister");
    m_pBtnDbCltSigGenUnregister->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenRegisterBtnsLine->addWidget(m_pBtnDbCltSigGenUnregister);
    m_pLytDbCltSigGenRegisterBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltSigGenUnregister,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltSigGenUnregisterClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltSigGenRegisterReqInProgress = new QProgressBar();
    m_pBarDbCltSigGenRegisterReqInProgress->setMaximumHeight(12);
    m_pBarDbCltSigGenRegisterReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltSigGenRegisterReqInProgress->setRange(0,100);
    m_pBarDbCltSigGenRegisterReqInProgress->setTextVisible(false);
    m_pBarDbCltSigGenRegisterReqInProgress->installEventFilter(this);
    m_pLytDbCltSigGenRegisterBtnsLine->addWidget(m_pBarDbCltSigGenRegisterReqInProgress);
    m_pBarDbCltSigGenRegisterReqInProgress->hide();

    m_pLblDbCltSigGenRegisterReqInProgress = new QLabel("Ready");
    m_pLblDbCltSigGenRegisterReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltSigGenRegisterReqInProgress->installEventFilter(this);
    m_pLytDbCltSigGenRegisterBtnsLine->addWidget(m_pLblDbCltSigGenRegisterReqInProgress);
    m_pLytDbCltSigGenRegisterBtnsLine->addStretch();

    updateDbCltSigGenRegisterReqInProgressStatus( "Ready", 100 );

    // <Led> State
    //----------------------------

    m_pLedDbCltSigGenState = new QLabel();
    m_pLedDbCltSigGenState->setFixedWidth(20);
    m_pLedDbCltSigGenState->setFixedHeight(20);
    m_pLedDbCltSigGenState->setScaledContents(true);
    m_pLytDbCltSigGenConnectBtnsLine->addWidget(m_pLedDbCltSigGenState);

    if( !QObject::connect(
        /* pObjSender   */ CApplication::GetInstance()->getDbClientThreadSigGen(),
        /* szSignal     */ SIGNAL(stateChanged(int,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbCltSigGenStateChanged(int,int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    onDbCltSigGenStateChanged( CApplication::GetInstance()->getDbClientThreadSigGen()->state(), CApplication::GetInstance()->getDbClientThreadSigGen()->state() );

    // <Line> Schema Buttons
    //-----------------------

    m_pLytDbCltSigGenSchemaBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltSigGenSchemaBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltSigGenSchemaBtnsLine = new QLabel("");
    pLblDbCltSigGenSchemaBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltSigGenSchemaBtnsLine->addWidget(pLblDbCltSigGenSchemaBtnsLine);
    m_pLytDbCltSigGenSchemaBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltSigGenSchemaTimeout = new QLabel("Timeout:");
    m_pLblDbCltSigGenSchemaTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltSigGenSchemaBtnsLine->addWidget(m_pLblDbCltSigGenSchemaTimeout);
    m_pCmbDbCltSigGenSchemaTimeout = new QComboBox();
    m_pCmbDbCltSigGenSchemaTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenSchemaBtnsLine->addWidget(m_pCmbDbCltSigGenSchemaTimeout);
    m_pLytDbCltSigGenSchemaBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltSigGenSchemaTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltSigGenSchemaTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Schema") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltSigGenSchemaTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltSigGenSchemaTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> SelectSchema
    //----------------------

    m_pBtnDbCltSigGenSelectSchema = new QPushButton("Select Schema");
    m_pBtnDbCltSigGenSelectSchema->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenSchemaBtnsLine->addWidget(m_pBtnDbCltSigGenSelectSchema);
    m_pLytDbCltSigGenSchemaBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltSigGenSelectSchema,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltSigGenSelectSchemaClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> UpdateSchema
    //---------------------

    m_pBtnDbCltSigGenUpdateSchema = new QPushButton("Update Schema");
    m_pBtnDbCltSigGenUpdateSchema->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenSchemaBtnsLine->addWidget(m_pBtnDbCltSigGenUpdateSchema);
    m_pLytDbCltSigGenSchemaBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltSigGenUpdateSchema,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltSigGenUpdateSchemaClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltSigGenSchemaReqInProgress = new QProgressBar();
    m_pBarDbCltSigGenSchemaReqInProgress->setMaximumHeight(12);
    m_pBarDbCltSigGenSchemaReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltSigGenSchemaReqInProgress->setRange(0,100);
    m_pBarDbCltSigGenSchemaReqInProgress->setTextVisible(false);
    m_pBarDbCltSigGenSchemaReqInProgress->installEventFilter(this);
    m_pLytDbCltSigGenSchemaBtnsLine->addWidget(m_pBarDbCltSigGenSchemaReqInProgress);
    m_pBarDbCltSigGenSchemaReqInProgress->hide();

    m_pLblDbCltSigGenSchemaReqInProgress = new QLabel("Ready");
    m_pLblDbCltSigGenSchemaReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltSigGenSchemaReqInProgress->installEventFilter(this);
    m_pLytDbCltSigGenSchemaBtnsLine->addWidget(m_pLblDbCltSigGenSchemaReqInProgress);
    m_pLytDbCltSigGenSchemaBtnsLine->addStretch();

    updateDbCltSigGenSchemaReqInProgressStatus( "Ready", 100 );

    // <Line> Data
    //------------

    m_pLytDbCltSigGenDataBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltSigGenDataBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltSigGenDataBtnsLine = new QLabel("");
    pLblDbCltSigGenDataBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltSigGenDataBtnsLine->addWidget(pLblDbCltSigGenDataBtnsLine);
    m_pLytDbCltSigGenDataBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltSigGenDataTimeout = new QLabel("Timeout:");
    m_pLblDbCltSigGenDataTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltSigGenDataBtnsLine->addWidget(m_pLblDbCltSigGenDataTimeout);
    m_pCmbDbCltSigGenDataTimeout = new QComboBox();
    m_pCmbDbCltSigGenDataTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenDataBtnsLine->addWidget(m_pCmbDbCltSigGenDataTimeout);
    m_pLytDbCltSigGenDataBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltSigGenDataTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltSigGenDataTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Data") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltSigGenDataTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltSigGenDataTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> SelectData
    //---------------------

    m_pBtnDbCltSigGenSelectData = new QPushButton("Select Data");
    m_pBtnDbCltSigGenSelectData->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenDataBtnsLine->addWidget(m_pBtnDbCltSigGenSelectData);
    m_pLytDbCltSigGenDataBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltSigGenSelectData,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltSigGenSelectDataClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> UpdateData
    //--------------------

    m_pBtnDbCltSigGenUpdateData = new QPushButton("Update Data");
    m_pBtnDbCltSigGenUpdateData->setFixedWidth(cxBtnWidth);
    m_pLytDbCltSigGenDataBtnsLine->addWidget(m_pBtnDbCltSigGenUpdateData);
    m_pLytDbCltSigGenDataBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltSigGenUpdateData,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltSigGenUpdateDataClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltSigGenDataReqInProgress = new QProgressBar();
    m_pBarDbCltSigGenDataReqInProgress->setMaximumHeight(12);
    m_pBarDbCltSigGenDataReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltSigGenDataReqInProgress->setRange(0,100);
    m_pBarDbCltSigGenDataReqInProgress->setTextVisible(false);
    m_pBarDbCltSigGenDataReqInProgress->installEventFilter(this);
    m_pLytDbCltSigGenDataBtnsLine->addWidget(m_pBarDbCltSigGenDataReqInProgress);
    m_pBarDbCltSigGenDataReqInProgress->hide();

    m_pLblDbCltSigGenDataReqInProgress = new QLabel("Ready");
    m_pLblDbCltSigGenDataReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltSigGenDataReqInProgress->installEventFilter(this);
    m_pLytDbCltSigGenDataBtnsLine->addWidget(m_pLblDbCltSigGenDataReqInProgress);
    m_pLytDbCltSigGenDataBtnsLine->addStretch();

    updateDbCltSigGenDataReqInProgressStatus( "Ready", 100 );

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

    // <Line> DbClient Capture
    //==========================================================================

    m_pLytDbCltCapture = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltCapture);

    // <Label> DbClient
    //--------------------

    m_pLblDbCltCapture = new QLabel("DbClient Capture:");
    m_pLblDbCltCapture->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltCapture->addWidget(m_pLblDbCltCapture);
    m_pLytDbCltCapture->addSpacing(cxSpacing);

    // <CheckBox> UseBlockingMethodCalls
    //----------------------------------

    m_pLblDbCltCaptureUseBlockingMethodCalls = new QLabel("Blocking Calls:");
    m_pLblDbCltCaptureUseBlockingMethodCalls->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltCapture->addWidget(m_pLblDbCltCaptureUseBlockingMethodCalls);
    m_pChkDbCltCaptureUseBlockingMethodCalls = new QCheckBox();
    m_pChkDbCltCaptureUseBlockingMethodCalls->setChecked( CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltCapture) );
    m_pLytDbCltCapture->addWidget(m_pChkDbCltCaptureUseBlockingMethodCalls);
    m_pLytDbCltCapture->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pChkDbCltCaptureUseBlockingMethodCalls,
        /* szSignal     */ SIGNAL(stateChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkDbCltCaptureUseBlockingMethodCallsStateChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <ComboBox> Connection Type
    //---------------------------

    m_pLblDbCltCaptureConnectType = new QLabel("Cnct. Type:");
    m_pLblDbCltCaptureConnectType->setFixedWidth(cxBtnWidth-12-m_pLytDbCltGUI->spacing());
    m_pLytDbCltCapture->addWidget(m_pLblDbCltCaptureConnectType);
    m_pCmbDbCltCaptureConnectType = new QComboBox();
    m_pCmbDbCltCaptureConnectType->setFixedWidth(2*cxBtnWidth+cxSpacing+m_pLytDbCltGUI->spacing()-2);
    m_pLytDbCltCapture->addWidget(m_pCmbDbCltCaptureConnectType);
    m_pLytDbCltCapture->addStretch();

    for( idxCmb = 0; idxCmb < Ipc::ESocketTypeCount; idxCmb++ )
    {
        m_pCmbDbCltCaptureConnectType->addItem( Ipc::socketType2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltCaptureConnectType->setCurrentIndex( CApplication::GetInstance()->getSocketType(c_strObjNameDbCltCapture) );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltCaptureConnectType,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltCaptureConnectTypeCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Line> Startup/Shutdown Buttons
    //----------------------------------

    m_pLytDbCltCaptureStartupBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltCaptureStartupBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltCaptureStartupBtnsLine = new QLabel("");
    pLblDbCltCaptureStartupBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltCaptureStartupBtnsLine->addWidget(pLblDbCltCaptureStartupBtnsLine);
    m_pLytDbCltCaptureStartupBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltCaptureStartupTimeout = new QLabel("Timeout:");
    m_pLblDbCltCaptureStartupTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltCaptureStartupBtnsLine->addWidget(m_pLblDbCltCaptureStartupTimeout);
    m_pCmbDbCltCaptureStartupTimeout = new QComboBox();
    m_pCmbDbCltCaptureStartupTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureStartupBtnsLine->addWidget(m_pCmbDbCltCaptureStartupTimeout);
    m_pLytDbCltCaptureStartupBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltCaptureStartupTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltCaptureStartupTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Startup") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltCaptureStartupTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltCaptureStartupTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Startup
    //-----------------

    m_pBtnDbCltCaptureStartup = new QPushButton("Startup");
    m_pBtnDbCltCaptureStartup->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureStartupBtnsLine->addWidget(m_pBtnDbCltCaptureStartup);
    m_pLytDbCltCaptureStartupBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltCaptureStartup,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltCaptureStartupClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Shutdown
    //--------------------

    m_pBtnDbCltCaptureShutdown = new QPushButton("Shutdown");
    m_pBtnDbCltCaptureShutdown->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureStartupBtnsLine->addWidget(m_pBtnDbCltCaptureShutdown);
    m_pLytDbCltCaptureStartupBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltCaptureShutdown,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltCaptureShutdownClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltCaptureStartupReqInProgress = new QProgressBar();
    m_pBarDbCltCaptureStartupReqInProgress->setMaximumHeight(12);
    m_pBarDbCltCaptureStartupReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltCaptureStartupReqInProgress->setRange(0,100);
    m_pBarDbCltCaptureStartupReqInProgress->setTextVisible(false);
    m_pBarDbCltCaptureStartupReqInProgress->installEventFilter(this);
    m_pLytDbCltCaptureStartupBtnsLine->addWidget(m_pBarDbCltCaptureStartupReqInProgress);
    m_pBarDbCltCaptureStartupReqInProgress->hide();

    m_pLblDbCltCaptureStartupReqInProgress = new QLabel("Ready");
    m_pLblDbCltCaptureStartupReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltCaptureStartupReqInProgress->installEventFilter(this);
    m_pLytDbCltCaptureStartupBtnsLine->addWidget(m_pLblDbCltCaptureStartupReqInProgress);
    m_pLytDbCltCaptureStartupBtnsLine->addStretch();

    updateDbCltCaptureStartupReqInProgressStatus( "Ready", 100 );

    // <Line> Connect/Disconnect Buttons
    //----------------------------------

    m_pLytDbCltCaptureConnectBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltCaptureConnectBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltCaptureBtnsLine = new QLabel("");
    pLblDbCltCaptureBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltCaptureConnectBtnsLine->addWidget(pLblDbCltCaptureBtnsLine);
    m_pLytDbCltCaptureConnectBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltCaptureConnectTimeout = new QLabel("Timeout:");
    m_pLblDbCltCaptureConnectTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltCaptureConnectBtnsLine->addWidget(m_pLblDbCltCaptureConnectTimeout);
    m_pCmbDbCltCaptureConnectTimeout = new QComboBox();
    m_pCmbDbCltCaptureConnectTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureConnectBtnsLine->addWidget(m_pCmbDbCltCaptureConnectTimeout);
    m_pLytDbCltCaptureConnectBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltCaptureConnectTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltCaptureConnectTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Connect") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltCaptureConnectTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltCaptureConnectTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }


    // <Button> Connect
    //-----------------

    m_pBtnDbCltCaptureConnect = new QPushButton("Connect");
    m_pBtnDbCltCaptureConnect->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureConnectBtnsLine->addWidget(m_pBtnDbCltCaptureConnect);
    m_pLytDbCltCaptureConnectBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltCaptureConnect,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltCaptureConnectClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Disconnect
    //--------------------

    m_pBtnDbCltCaptureDisconnect = new QPushButton("Disconnect");
    m_pBtnDbCltCaptureDisconnect->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureConnectBtnsLine->addWidget(m_pBtnDbCltCaptureDisconnect);
    m_pLytDbCltCaptureConnectBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltCaptureDisconnect,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltCaptureDisconnectClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltCaptureConnectReqInProgress = new QProgressBar();
    m_pBarDbCltCaptureConnectReqInProgress->setMaximumHeight(12);
    m_pBarDbCltCaptureConnectReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltCaptureConnectReqInProgress->setRange(0,100);
    m_pBarDbCltCaptureConnectReqInProgress->setTextVisible(false);
    m_pBarDbCltCaptureConnectReqInProgress->installEventFilter(this);
    m_pLytDbCltCaptureConnectBtnsLine->addWidget(m_pBarDbCltCaptureConnectReqInProgress);
    m_pBarDbCltCaptureConnectReqInProgress->hide();

    m_pLblDbCltCaptureConnectReqInProgress = new QLabel("Ready");
    m_pLblDbCltCaptureConnectReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltCaptureConnectReqInProgress->installEventFilter(this);
    m_pLytDbCltCaptureConnectBtnsLine->addWidget(m_pLblDbCltCaptureConnectReqInProgress);
    m_pLytDbCltCaptureConnectBtnsLine->addStretch();

    updateDbCltCaptureConnectReqInProgressStatus( "Ready", 100 );

    // <Led> State
    //----------------------------

    m_pLedDbCltCaptureState = new QLabel();
    m_pLedDbCltCaptureState->setFixedWidth(20);
    m_pLedDbCltCaptureState->setFixedHeight(20);
    m_pLedDbCltCaptureState->setScaledContents(true);
    m_pLytDbCltCaptureConnectBtnsLine->addWidget(m_pLedDbCltCaptureState);

    if( !QObject::connect(
        /* pObjSender   */ CApplication::GetInstance()->getDbClientThreadCapture(),
        /* szSignal     */ SIGNAL(stateChanged(int,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbCltCaptureStateChanged(int,int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    onDbCltCaptureStateChanged( CApplication::GetInstance()->getDbClientThreadCapture()->state(), CApplication::GetInstance()->getDbClientThreadCapture()->state() );

    // <Line> Register/Unregister Buttons
    //----------------------------------

    m_pLytDbCltCaptureRegisterBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltCaptureRegisterBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltCaptureRegisterBtnsLine = new QLabel("");
    pLblDbCltCaptureRegisterBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltCaptureRegisterBtnsLine->addWidget(pLblDbCltCaptureRegisterBtnsLine);
    m_pLytDbCltCaptureRegisterBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltCaptureRegisterTimeout = new QLabel("Timeout:");
    m_pLblDbCltCaptureRegisterTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltCaptureRegisterBtnsLine->addWidget(m_pLblDbCltCaptureRegisterTimeout);
    m_pCmbDbCltCaptureRegisterTimeout = new QComboBox();
    m_pCmbDbCltCaptureRegisterTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureRegisterBtnsLine->addWidget(m_pCmbDbCltCaptureRegisterTimeout);
    m_pLytDbCltCaptureRegisterBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltCaptureRegisterTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltCaptureRegisterTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Register") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltCaptureRegisterTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltCaptureRegisterTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Register
    //-----------------

    m_pBtnDbCltCaptureRegister = new QPushButton("Register");
    m_pBtnDbCltCaptureRegister->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureRegisterBtnsLine->addWidget(m_pBtnDbCltCaptureRegister);
    m_pLytDbCltCaptureRegisterBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltCaptureRegister,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltCaptureRegisterClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Unregister
    //--------------------

    m_pBtnDbCltCaptureUnregister = new QPushButton("Unregister");
    m_pBtnDbCltCaptureUnregister->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureRegisterBtnsLine->addWidget(m_pBtnDbCltCaptureUnregister);
    m_pLytDbCltCaptureRegisterBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltCaptureUnregister,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltCaptureUnregisterClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltCaptureRegisterReqInProgress = new QProgressBar();
    m_pBarDbCltCaptureRegisterReqInProgress->setMaximumHeight(12);
    m_pBarDbCltCaptureRegisterReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltCaptureRegisterReqInProgress->setRange(0,100);
    m_pBarDbCltCaptureRegisterReqInProgress->setTextVisible(false);
    m_pBarDbCltCaptureRegisterReqInProgress->installEventFilter(this);
    m_pLytDbCltCaptureRegisterBtnsLine->addWidget(m_pBarDbCltCaptureRegisterReqInProgress);
    m_pBarDbCltCaptureRegisterReqInProgress->hide();

    m_pLblDbCltCaptureRegisterReqInProgress = new QLabel("Ready");
    m_pLblDbCltCaptureRegisterReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltCaptureRegisterReqInProgress->installEventFilter(this);
    m_pLytDbCltCaptureRegisterBtnsLine->addWidget(m_pLblDbCltCaptureRegisterReqInProgress);
    m_pLytDbCltCaptureRegisterBtnsLine->addStretch();

    updateDbCltCaptureRegisterReqInProgressStatus( "Ready", 100 );

    // <Line> Schema Buttons
    //-----------------------

    m_pLytDbCltCaptureSchemaBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltCaptureSchemaBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltCaptureSchemaBtnsLine = new QLabel("");
    pLblDbCltCaptureSchemaBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltCaptureSchemaBtnsLine->addWidget(pLblDbCltCaptureSchemaBtnsLine);
    m_pLytDbCltCaptureSchemaBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltCaptureSchemaTimeout = new QLabel("Timeout:");
    m_pLblDbCltCaptureSchemaTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltCaptureSchemaBtnsLine->addWidget(m_pLblDbCltCaptureSchemaTimeout);
    m_pCmbDbCltCaptureSchemaTimeout = new QComboBox();
    m_pCmbDbCltCaptureSchemaTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureSchemaBtnsLine->addWidget(m_pCmbDbCltCaptureSchemaTimeout);
    m_pLytDbCltCaptureSchemaBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltCaptureSchemaTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltCaptureSchemaTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Schema") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltCaptureSchemaTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltCaptureSchemaTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> SelectSchema
    //----------------------

    m_pBtnDbCltCaptureSelectSchema = new QPushButton("Select Schema");
    m_pBtnDbCltCaptureSelectSchema->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureSchemaBtnsLine->addWidget(m_pBtnDbCltCaptureSelectSchema);
    m_pLytDbCltCaptureSchemaBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltCaptureSelectSchema,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltCaptureSelectSchemaClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> UpdateSchema
    //---------------------

    m_pBtnDbCltCaptureUpdateSchema = new QPushButton("Update Schema");
    m_pBtnDbCltCaptureUpdateSchema->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureSchemaBtnsLine->addWidget(m_pBtnDbCltCaptureUpdateSchema);
    m_pLytDbCltCaptureSchemaBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltCaptureUpdateSchema,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltCaptureUpdateSchemaClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltCaptureSchemaReqInProgress = new QProgressBar();
    m_pBarDbCltCaptureSchemaReqInProgress->setMaximumHeight(12);
    m_pBarDbCltCaptureSchemaReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltCaptureSchemaReqInProgress->setRange(0,100);
    m_pBarDbCltCaptureSchemaReqInProgress->setTextVisible(false);
    m_pBarDbCltCaptureSchemaReqInProgress->installEventFilter(this);
    m_pLytDbCltCaptureSchemaBtnsLine->addWidget(m_pBarDbCltCaptureSchemaReqInProgress);
    m_pBarDbCltCaptureSchemaReqInProgress->hide();

    m_pLblDbCltCaptureSchemaReqInProgress = new QLabel("Ready");
    m_pLblDbCltCaptureSchemaReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltCaptureSchemaReqInProgress->installEventFilter(this);
    m_pLytDbCltCaptureSchemaBtnsLine->addWidget(m_pLblDbCltCaptureSchemaReqInProgress);
    m_pLytDbCltCaptureSchemaBtnsLine->addStretch();

    updateDbCltCaptureSchemaReqInProgressStatus( "Ready", 100 );

    // <Line> Data
    //------------

    m_pLytDbCltCaptureDataBtnsLine = new QHBoxLayout();
    m_pLyt->addLayout(m_pLytDbCltCaptureDataBtnsLine);

    // <Label> Space Column 1
    //-----------------------

    QLabel* pLblDbCltCaptureDataBtnsLine = new QLabel("");
    pLblDbCltCaptureDataBtnsLine->setFixedWidth(cxLblWidthClm1);
    m_pLytDbCltCaptureDataBtnsLine->addWidget(pLblDbCltCaptureDataBtnsLine);
    m_pLytDbCltCaptureDataBtnsLine->addSpacing(cxSpacing);

    // <ComboBox> Timeout
    //-------------------

    m_pLblDbCltCaptureDataTimeout = new QLabel("Timeout:");
    m_pLblDbCltCaptureDataTimeout->setFixedWidth(cxLblWidthClm2);
    m_pLytDbCltCaptureDataBtnsLine->addWidget(m_pLblDbCltCaptureDataTimeout);
    m_pCmbDbCltCaptureDataTimeout = new QComboBox();
    m_pCmbDbCltCaptureDataTimeout->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureDataBtnsLine->addWidget(m_pCmbDbCltCaptureDataTimeout);
    m_pLytDbCltCaptureDataBtnsLine->addSpacing(cxSpacing);

    for( idxCmb = 0; idxCmb < ETimeoutCount; idxCmb++ )
    {
        m_pCmbDbCltCaptureDataTimeout->addItem( timeout2Str(idxCmb), idxCmb );
    }
    m_pCmbDbCltCaptureDataTimeout->setCurrentIndex( CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Data") );

    if( !QObject::connect(
        /* pObjSender   */ m_pCmbDbCltCaptureDataTimeout,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbDbCltCaptureDataTimeoutCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> SelectData
    //---------------------

    m_pBtnDbCltCaptureSelectData = new QPushButton("Select Data");
    m_pBtnDbCltCaptureSelectData->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureDataBtnsLine->addWidget(m_pBtnDbCltCaptureSelectData);
    m_pLytDbCltCaptureDataBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltCaptureSelectData,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltCaptureSelectDataClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> UpdateData
    //--------------------

    m_pBtnDbCltCaptureUpdateData = new QPushButton("Update Data");
    m_pBtnDbCltCaptureUpdateData->setFixedWidth(cxBtnWidth);
    m_pLytDbCltCaptureDataBtnsLine->addWidget(m_pBtnDbCltCaptureUpdateData);
    m_pLytDbCltCaptureDataBtnsLine->addSpacing(cxSpacing);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnDbCltCaptureUpdateData,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnDbCltCaptureUpdateDataClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarDbCltCaptureDataReqInProgress = new QProgressBar();
    m_pBarDbCltCaptureDataReqInProgress->setMaximumHeight(12);
    m_pBarDbCltCaptureDataReqInProgress->setFixedWidth(cxProgressBarWidth);
    m_pBarDbCltCaptureDataReqInProgress->setRange(0,100);
    m_pBarDbCltCaptureDataReqInProgress->setTextVisible(false);
    m_pBarDbCltCaptureDataReqInProgress->installEventFilter(this);
    m_pLytDbCltCaptureDataBtnsLine->addWidget(m_pBarDbCltCaptureDataReqInProgress);
    m_pBarDbCltCaptureDataReqInProgress->hide();

    m_pLblDbCltCaptureDataReqInProgress = new QLabel("Ready");
    m_pLblDbCltCaptureDataReqInProgress->setFixedWidth(cxLblWidthReqInProgress);
    m_pLblDbCltCaptureDataReqInProgress->installEventFilter(this);
    m_pLytDbCltCaptureDataBtnsLine->addWidget(m_pLblDbCltCaptureDataReqInProgress);
    m_pLytDbCltCaptureDataBtnsLine->addStretch();

    updateDbCltCaptureDataReqInProgressStatus( "Ready", 100 );

    // <Stretch> at bottom of main layout
    //-----------------------------------

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWidgetCentral::~CWidgetCentral()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;
    // Application
    m_pLytApp = nullptr;
    m_pLblApp = nullptr;
    m_pBtnAppStartup = nullptr;
    m_pBtnAppShutdown = nullptr;
    m_pLblAppReqInProgress = nullptr;
    m_pBarAppReqInProgress = nullptr;
    m_pLedAppState = nullptr;
    // Database
    m_pLytDb = nullptr;
    m_pLblDb = nullptr;
    m_pLblDbUseBlockingMethodCalls = nullptr;
    m_pChkDbUseBlockingMethodCalls = nullptr;
    m_pLytDbBtnsLine = nullptr;
    m_pLblDbTimeout = nullptr;
    m_pCmbDbTimeout = nullptr;
    m_pBtnDbStartup = nullptr;
    m_pBtnDbShutdown = nullptr;
    m_pLblDbReqInProgress = nullptr;
    m_pBarDbReqInProgress = nullptr;
    m_pLedDbState = nullptr;
    m_pLytDbSingleShotsLine = nullptr;
    m_pLblDbModeSingleShot = nullptr;
    m_pChkDbModeSingleShot = nullptr;
    m_pBtnDbExecuteSingleShot = nullptr;
    // DbCltGUI
    m_pLytDbCltGUI = nullptr;
    m_pLblDbCltGUI = nullptr;
    m_pLblDbCltGUIUseBlockingMethodCalls = nullptr;
    m_pChkDbCltGUIUseBlockingMethodCalls = nullptr;
    m_pLblDbCltGUIConnectType = nullptr;
    m_pCmbDbCltGUIConnectType = nullptr;
    m_pLytDbCltGUIConnectBtnsLine = nullptr;
    m_pLblDbCltGUIConnectTimeout = nullptr;
    m_pCmbDbCltGUIConnectTimeout = nullptr;
    m_pBtnDbCltGUIConnect = nullptr;
    m_pBtnDbCltGUIDisconnect = nullptr;
    m_pLblDbCltGUIConnectReqInProgress = nullptr;
    m_pBarDbCltGUIConnectReqInProgress = nullptr;
    m_pLedDbCltGUIConnectState = nullptr;
    m_pLytDbCltGUIRegisterBtnsLine = nullptr;
    m_pLblDbCltGUIRegisterTimeout = nullptr;
    m_pCmbDbCltGUIRegisterTimeout = nullptr;
    m_pBtnDbCltGUIRegister = nullptr;
    m_pBtnDbCltGUIUnregister = nullptr;
    m_pLblDbCltGUIRegisterReqInProgress = nullptr;
    m_pBarDbCltGUIRegisterReqInProgress = nullptr;
    m_pLytDbCltGUISchemaBtnsLine = nullptr;
    m_pLblDbCltGUISchemaTimeout = nullptr;
    m_pCmbDbCltGUISchemaTimeout = nullptr;
    m_pBtnDbCltGUISelectSchema = nullptr;
    m_pBtnDbCltGUIUpdateSchema = nullptr;
    m_pLblDbCltGUISchemaReqInProgress = nullptr;
    m_pBarDbCltGUISchemaReqInProgress = nullptr;
    m_pLytDbCltGUIDataBtnsLine = nullptr;
    m_pLblDbCltGUIDataTimeout = nullptr;
    m_pCmbDbCltGUIDataTimeout = nullptr;
    m_pBtnDbCltGUISelectData = nullptr;
    m_pBtnDbCltGUIUpdateData = nullptr;
    m_pLblDbCltGUIDataReqInProgress = nullptr;
    m_pBarDbCltGUIDataReqInProgress = nullptr;
    // DbCltSigGen
    m_pLytDbCltSigGen = nullptr;
    m_pLblDbCltSigGen = nullptr;
    m_pLblDbCltSigGenUseBlockingMethodCalls = nullptr;
    m_pChkDbCltSigGenUseBlockingMethodCalls = nullptr;
    m_pLblDbCltSigGenConnectType = nullptr;
    m_pCmbDbCltSigGenConnectType = nullptr;
    m_pLytDbCltSigGenStartupBtnsLine = nullptr;
    m_pLblDbCltSigGenStartupTimeout = nullptr;
    m_pCmbDbCltSigGenStartupTimeout = nullptr;
    m_pBtnDbCltSigGenStartup = nullptr;
    m_pBtnDbCltSigGenShutdown = nullptr;
    m_pLblDbCltSigGenStartupReqInProgress = nullptr;
    m_pBarDbCltSigGenStartupReqInProgress = nullptr;
    m_pLytDbCltSigGenConnectBtnsLine = nullptr;
    m_pLblDbCltSigGenConnectTimeout = nullptr;
    m_pCmbDbCltSigGenConnectTimeout = nullptr;
    m_pBtnDbCltSigGenConnect = nullptr;
    m_pBtnDbCltSigGenDisconnect = nullptr;
    m_pLblDbCltSigGenConnectReqInProgress = nullptr;
    m_pBarDbCltSigGenConnectReqInProgress = nullptr;
    m_pLedDbCltSigGenState = nullptr;
    m_pLytDbCltSigGenRegisterBtnsLine = nullptr;
    m_pLblDbCltSigGenRegisterTimeout = nullptr;
    m_pCmbDbCltSigGenRegisterTimeout = nullptr;
    m_pBtnDbCltSigGenRegister = nullptr;
    m_pBtnDbCltSigGenUnregister = nullptr;
    m_pLblDbCltSigGenRegisterReqInProgress = nullptr;
    m_pBarDbCltSigGenRegisterReqInProgress = nullptr;
    m_pLytDbCltSigGenSchemaBtnsLine = nullptr;
    m_pLblDbCltSigGenSchemaTimeout = nullptr;
    m_pCmbDbCltSigGenSchemaTimeout = nullptr;
    m_pBtnDbCltSigGenSelectSchema = nullptr;
    m_pBtnDbCltSigGenUpdateSchema = nullptr;
    m_pLblDbCltSigGenSchemaReqInProgress = nullptr;
    m_pBarDbCltSigGenSchemaReqInProgress = nullptr;
    m_pLytDbCltSigGenDataBtnsLine = nullptr;
    m_pLblDbCltSigGenDataTimeout = nullptr;
    m_pCmbDbCltSigGenDataTimeout = nullptr;
    m_pBtnDbCltSigGenSelectData = nullptr;
    m_pBtnDbCltSigGenUpdateData = nullptr;
    m_pLblDbCltSigGenDataReqInProgress = nullptr;
    m_pBarDbCltSigGenDataReqInProgress = nullptr;
    // DbCltCapture
    m_pLytDbCltCapture = nullptr;
    m_pLblDbCltCapture = nullptr;
    m_pLblDbCltCaptureUseBlockingMethodCalls = nullptr;
    m_pChkDbCltCaptureUseBlockingMethodCalls = nullptr;
    m_pLblDbCltCaptureConnectType = nullptr;
    m_pCmbDbCltCaptureConnectType = nullptr;
    m_pLytDbCltCaptureStartupBtnsLine = nullptr;
    m_pLblDbCltCaptureStartupTimeout = nullptr;
    m_pCmbDbCltCaptureStartupTimeout = nullptr;
    m_pBtnDbCltCaptureStartup = nullptr;
    m_pBtnDbCltCaptureShutdown = nullptr;
    m_pLblDbCltCaptureStartupReqInProgress = nullptr;
    m_pBarDbCltCaptureStartupReqInProgress = nullptr;
    m_pLytDbCltCaptureConnectBtnsLine = nullptr;
    m_pLblDbCltCaptureConnectTimeout = nullptr;
    m_pCmbDbCltCaptureConnectTimeout = nullptr;
    m_pBtnDbCltCaptureConnect = nullptr;
    m_pBtnDbCltCaptureDisconnect = nullptr;
    m_pLblDbCltCaptureConnectReqInProgress = nullptr;
    m_pBarDbCltCaptureConnectReqInProgress = nullptr;
    m_pLedDbCltCaptureState = nullptr;
    m_pLytDbCltCaptureRegisterBtnsLine = nullptr;
    m_pLblDbCltCaptureRegisterTimeout = nullptr;
    m_pCmbDbCltCaptureRegisterTimeout = nullptr;
    m_pBtnDbCltCaptureRegister = nullptr;
    m_pBtnDbCltCaptureUnregister = nullptr;
    m_pLblDbCltCaptureRegisterReqInProgress = nullptr;
    m_pBarDbCltCaptureRegisterReqInProgress = nullptr;
    m_pLytDbCltCaptureSchemaBtnsLine = nullptr;
    m_pLblDbCltCaptureSchemaTimeout = nullptr;
    m_pCmbDbCltCaptureSchemaTimeout = nullptr;
    m_pBtnDbCltCaptureSelectSchema = nullptr;
    m_pBtnDbCltCaptureUpdateSchema = nullptr;
    m_pLblDbCltCaptureSchemaReqInProgress = nullptr;
    m_pBarDbCltCaptureSchemaReqInProgress = nullptr;
    m_pLytDbCltCaptureDataBtnsLine = nullptr;
    m_pLblDbCltCaptureDataTimeout = nullptr;
    m_pCmbDbCltCaptureDataTimeout = nullptr;
    m_pBtnDbCltCaptureSelectData = nullptr;
    m_pBtnDbCltCaptureUpdateData = nullptr;
    m_pLblDbCltCaptureDataReqInProgress = nullptr;
    m_pBarDbCltCaptureDataReqInProgress = nullptr;

    s_pThis = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onAppStateChanged( int i_iStateCurr, int /*i_iStatePrev*/ )
//------------------------------------------------------------------------------
{
    if( i_iStateCurr == CApplication::EStateCreated )
    {
        m_pLedAppState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
    }
    else if( i_iStateCurr == CApplication::EStateRunning )
    {
        m_pLedAppState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
    }
    else
    {
        m_pLedAppState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
    }

} // onAppStateChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onAppRequestChanged( const ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strReqInProgress;
    int     iProgress_perCent = i_reqDscr.m_iProgress_perCent;

    if( i_reqDscr.m_strCreatorObjName.isEmpty() )
    {
        strReqInProgress = i_reqDscr.m_strRequest;
    }
    else
    {
        strReqInProgress = i_reqDscr.m_strCreatorObjName + "." + i_reqDscr.m_strRequest;
    }

    updateAppReqInProgressStatus( strReqInProgress, iProgress_perCent );

} // onAppRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onDbStateChanged( int /*i_iStateCurr*/, int /*i_iStatePrev*/ )
//------------------------------------------------------------------------------
{
    if( CApplication::GetInstance()->getDbThread()->state() == Database::EStateCreated )
    {
        m_pLedDbState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
    }
    else if( CApplication::GetInstance()->getDbThread()->state() == Database::EStateRunning )
    {
        m_pLedDbState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
    }
    else
    {
        m_pLedDbState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
    }
    if( !QObject::connect(
        /* pObjSender   */ CApplication::GetInstance()->getDbThread(),
        /* szSignal     */ SIGNAL(stateChanged(int,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDbStateChanged(int,int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // onDbStateChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onDbRequestChanged( const ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strReqInProgress;
    int     iProgress_perCent = i_reqDscr.m_iProgress_perCent;

    if( i_reqDscr.m_strCreatorObjName.isEmpty() )
    {
        strReqInProgress = i_reqDscr.m_strRequest;
    }
    else
    {
        strReqInProgress = i_reqDscr.m_strCreatorObjName + "." + i_reqDscr.m_strRequest;
    }

    updateDbReqInProgressStatus( strReqInProgress, iProgress_perCent );

} // onDbRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onDbCltGUIStateChanged( int /*i_iStateCurr*/, int /*i_iStatePrev*/ )
//------------------------------------------------------------------------------
{
    if( CApplication::GetInstance()->getDbClientGUI()->state() == Database::EStateRegistered )
    {
        m_pLedDbCltGUIConnectState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleBlue.png") );
    }
    else if( CApplication::GetInstance()->getDbClientGUI()->state() == Database::EStateConnected )
    {
        m_pLedDbCltGUIConnectState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
    }
    else
    {
        m_pLedDbCltGUIConnectState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
    }

} // onDbCltGUIStateChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onDbCltGUIRequestChanged( const ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strReqInProgress;
    int     iProgress_perCent = i_reqDscr.m_iProgress_perCent;

    if( i_reqDscr.m_strCreatorObjName.isEmpty() )
    {
        strReqInProgress = i_reqDscr.m_strRequest;
    }
    else
    {
        strReqInProgress = i_reqDscr.m_strCreatorObjName + "." + i_reqDscr.m_strRequest;
    }

    switch( i_reqDscr.m_request )
    {
        case Database::ERequestConnect:
        case Database::ERequestDisconnect:
        {
            updateDbCltGUIConnectReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        case Database::ERequestRegister:
        case Database::ERequestUnregister:
        {
            updateDbCltGUIRegisterReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        case Database::ERequestSelectSchema:
        case Database::ERequestUpdateSchema:
        {
            updateDbCltGUISchemaReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        case Database::ERequestSelectData:
        case Database::ERequestUpdateData:
        {
            updateDbCltGUIDataReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        default:
        {
            break;
        }
    } // switch( i_reqDscr.m_request )

} // onDbCltGUIRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onDbCltSigGenStateChanged( int /*i_iStateCurr*/, int /*i_iStatePrev*/ )
//------------------------------------------------------------------------------
{
    if( CApplication::GetInstance()->getDbClientThreadSigGen()->state() == Database::EStateCreated )
    {
        m_pLedDbCltSigGenState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
    }
    else if( CApplication::GetInstance()->getDbClientThreadSigGen()->state() == Database::EStateDisconnected )
    {
        m_pLedDbCltSigGenState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
    }
    else if( CApplication::GetInstance()->getDbClientThreadSigGen()->state() == Database::EStateConnected )
    {
        m_pLedDbCltSigGenState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
    }
    else if( CApplication::GetInstance()->getDbClientThreadSigGen()->state() == Database::EStateRegistered )
    {
        m_pLedDbCltSigGenState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleBlue.png") );
    }
    else
    {
        m_pLedDbCltSigGenState->setPixmap( QPixmap() );
    }

} // onDbCltSigGenStateChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onDbCltSigGenRequestChanged( const ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strReqInProgress;
    int     iProgress_perCent = i_reqDscr.m_iProgress_perCent;

    if( i_reqDscr.m_strCreatorObjName.isEmpty() )
    {
        strReqInProgress = i_reqDscr.m_strRequest;
    }
    else
    {
        strReqInProgress = i_reqDscr.m_strCreatorObjName + "." + i_reqDscr.m_strRequest;
    }

    switch( i_reqDscr.m_request )
    {
        case Database::ERequestStartup:
        case Database::ERequestShutdown:
        {
            updateDbCltSigGenStartupReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        case Database::ERequestConnect:
        case Database::ERequestDisconnect:
        {
            updateDbCltSigGenConnectReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        case Database::ERequestRegister:
        case Database::ERequestUnregister:
        {
            updateDbCltSigGenRegisterReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        case Database::ERequestSelectSchema:
        case Database::ERequestUpdateSchema:
        {
            updateDbCltSigGenSchemaReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        case Database::ERequestSelectData:
        case Database::ERequestUpdateData:
        {
            updateDbCltSigGenDataReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        default:
        {
            break;
        }
    } // switch( i_reqDscr.m_request )

} // onDbCltSigGenRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onDbCltCaptureStateChanged( int /*i_iStateCurr*/, int /*i_iStatePrev*/ )
//------------------------------------------------------------------------------
{
    if( CApplication::GetInstance()->getDbClientThreadCapture()->state() == Database::EStateCreated )
    {
        m_pLedDbCltCaptureState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGrey.png") );
    }
    else if( CApplication::GetInstance()->getDbClientThreadCapture()->state() == Database::EStateDisconnected )
    {
        m_pLedDbCltCaptureState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleRed.png") );
    }
    else if( CApplication::GetInstance()->getDbClientThreadCapture()->state() == Database::EStateConnected )
    {
        m_pLedDbCltCaptureState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleGreen.png") );
    }
    else if( CApplication::GetInstance()->getDbClientThreadCapture()->state() == Database::EStateRegistered )
    {
        m_pLedDbCltCaptureState->setPixmap( QPixmap(":/ZS/CnctState/LedCircleBlue.png") );
    }
    else
    {
        m_pLedDbCltCaptureState->setPixmap( QPixmap() );
    }

} // onDbCltCaptureStateChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onDbCltCaptureRequestChanged( const ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strReqInProgress;
    int     iProgress_perCent = i_reqDscr.m_iProgress_perCent;

    if( i_reqDscr.m_strCreatorObjName.isEmpty() )
    {
        strReqInProgress = i_reqDscr.m_strRequest;
    }
    else
    {
        strReqInProgress = i_reqDscr.m_strCreatorObjName + "." + i_reqDscr.m_strRequest;
    }

    switch( i_reqDscr.m_request )
    {
        case Database::ERequestStartup:
        case Database::ERequestShutdown:
        {
            updateDbCltCaptureStartupReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        case Database::ERequestConnect:
        case Database::ERequestDisconnect:
        {
            updateDbCltCaptureConnectReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        case Database::ERequestRegister:
        case Database::ERequestUnregister:
        {
            updateDbCltCaptureRegisterReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        case Database::ERequestSelectSchema:
        case Database::ERequestUpdateSchema:
        {
            updateDbCltCaptureSchemaReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        case Database::ERequestSelectData:
        case Database::ERequestUpdateData:
        {
            updateDbCltCaptureDataReqInProgressStatus( strReqInProgress, iProgress_perCent );
            break;
        }
        default:
        {
            break;
        }
    } // switch( i_reqDscr.m_request )

} // onDbCltCaptureRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnAppStartupClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = CApplication::GetInstance()->startup();

    if( isAsynchronousRequest(pReq) )
    {
        if( !QObject::connect(
            /* pObjSender   */ pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onAppRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        onAppRequestChanged( pReq->getDescriptor() );
    }

} // onBtnAppStartupClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnAppShutdownClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = CApplication::GetInstance()->shutdown();

    if( isAsynchronousRequest(pReq) )
    {
        if( !QObject::connect(
            /* pObjSender   */ pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onAppRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        onAppRequestChanged( pReq->getDescriptor() );
    }

} // onBtnAppShutdownClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onChkDbUseBlockingMethodCallsStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    CApplication::GetInstance()->setUseBlockingMethodCalls( i_iCheckState == Qt::Checked, c_strObjNameDb );
}

//------------------------------------------------------------------------------
void CWidgetCentral::onChkDbModeSingleShotStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    CApplication::GetInstance()->setDbReqModeSingleShot( i_iCheckState == Qt::Checked );
}

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbExecuteSingleShotClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CApplication::GetInstance()->executeDbReqSingleShot();
}

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDb );
    }

} // onCmbDbTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbStartupClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbThread* pDbThread = CApplication::GetInstance()->getDbThread();

    if( pDbThread != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDb);
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDb);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbThread->startup( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbThread != nullptr )

} // onBtnDbStartupClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbShutdownClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbThread* pDbThread = CApplication::GetInstance()->getDbThread();

    if( pDbThread != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDb);
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDb);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbThread->shutdown( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbThread != nullptr )

} // onBtnDbShutdownClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onChkDbCltGUIUseBlockingMethodCallsStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    CApplication::GetInstance()->setUseBlockingMethodCalls( i_iCheckState == Qt::Checked, c_strObjNameDbCltGUI );
}

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltGUIConnectTypeCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setSocketType( static_cast<Ipc::ESocketType>(i_iIdx), c_strObjNameDbCltGUI );
    }

} // onCmbDbCltGUIConnectTypeCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltGUIConnectTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltGUI, "Connect" );
    }

} // onCmbDbCltGUIConnectTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltGUIConnectClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClient* pDbClt = CApplication::GetInstance()->getDbClientGUI();

    if( pDbClt != nullptr )
    {
        ETimeout        timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Connect");
        int             iTimeout_ms = getTimeoutInMs(timeout);
        qint64          iReqIdParent = -1;
        bool            bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltGUI);
        Ipc::ESocketType socketType = CApplication::GetInstance()->getSocketType(c_strObjNameDbCltGUI);

        pDbClt->setSocketType(socketType);

        CRequest* pReq = pDbClt->connect_(iTimeout_ms, bWait, iReqIdParent);

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltGUIRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltGUIRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltGUIConnectClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltGUIDisconnectClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClient* pDbClt = CApplication::GetInstance()->getDbClientGUI();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Connect");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltGUI);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->disconnect_( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltGUIRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltGUIRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltGUIDisconnectClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltGUIRegisterTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltGUI, "Register" );
    }

} // onCmbDbCltGUIRegisterTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltGUIRegisterClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClient* pDbClt = CApplication::GetInstance()->getDbClientGUI();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Register");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        qint64   iReqIdParent = -1;
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltGUI);

        CRequest* pReq = pDbClt->register_( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltGUIRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltGUIRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltGUIRegisterClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltGUIUnregisterClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClient* pDbClt = CApplication::GetInstance()->getDbClientGUI();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Register");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltGUI);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->unregister( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltGUIRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltGUIRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltGUIUnregisterClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltGUISchemaTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltGUI, "Schema" );
    }

} // onCmbDbCltGUISchemaTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltGUISelectSchemaClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClient* pDbClt = CApplication::GetInstance()->getDbClientGUI();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Schema");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltGUI);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->selectSchema( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltGUIRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltGUIRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltGUISelectSchemaClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltGUIUpdateSchemaClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClient* pDbClt = CApplication::GetInstance()->getDbClientGUI();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Schema");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltGUI);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->updateSchema( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltGUIRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltGUIRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltGUIUpdateSchemaClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltGUIDataTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltGUI, "Data" );
    }

} // onCmbDbCltGUIDataTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltGUISelectDataClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClient* pDbClt = CApplication::GetInstance()->getDbClientGUI();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Data");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltGUI);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->selectData( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltGUIRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltGUIRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltGUISelectDataClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltGUIUpdateDataClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClient* pDbClt = CApplication::GetInstance()->getDbClientGUI();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltGUI,"Data");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltGUI);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->updateData( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltGUIRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltGUIRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltGUIUpdateDataClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onChkDbCltSigGenUseBlockingMethodCallsStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    CApplication::GetInstance()->setUseBlockingMethodCalls( i_iCheckState == Qt::Checked, c_strObjNameDbCltSigGen );
}

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltSigGenConnectTypeCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setSocketType( static_cast<Ipc::ESocketType>(i_iIdx), c_strObjNameDbCltSigGen );
    }

} // onCmbDbCltSigGenConnectTypeCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltSigGenStartupTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltSigGen, "Startup" );
    }

} // onCmbDbCltSigGenStartupTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltSigGenStartupClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadSigGen();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Startup");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltSigGen);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->startup( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltSigGenRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltSigGenRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltSigGenStartupClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltSigGenShutdownClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadSigGen();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Startup");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltSigGen);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->shutdown( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltSigGenRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltSigGenRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltSigGenShutdownClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltSigGenConnectTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltSigGen, "Connect" );
    }

} // onCmbDbCltSigGenConnectTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltSigGenConnectClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadSigGen();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Connect");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltSigGen);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->connect_(iTimeout_ms, bWait, iReqIdParent);

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltSigGenRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltSigGenRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltSigGenConnectClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltSigGenDisconnectClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadSigGen();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Connect");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltSigGen);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->disconnect_( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltSigGenRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltSigGenRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltSigGenDisconnectClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltSigGenRegisterTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltSigGen, "Register" );
    }

} // onCmbDbCltSigGenRegisterTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltSigGenRegisterClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadSigGen();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Register");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        qint64   iReqIdParent = -1;
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltSigGen);

        CRequest* pReq = pDbClt->register_( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltSigGenRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltSigGenRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltSigGenRegisterClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltSigGenUnregisterClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadSigGen();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Register");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltSigGen);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->unregister( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltSigGenRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltSigGenRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltSigGenUnregisterClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltSigGenSchemaTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltSigGen, "Schema" );
    }

} // onCmbDbCltSigGenSchemaTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltSigGenSelectSchemaClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbCltThread = CApplication::GetInstance()->getDbClientThreadSigGen();

    if( pDbCltThread != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Schema");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltSigGen);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbCltThread->selectSchema( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltSigGenRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltSigGenRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbCltThread != nullptr )

} // onBtnDbCltSigGenSelectSchemaClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltSigGenUpdateSchemaClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbCltThread = CApplication::GetInstance()->getDbClientThreadSigGen();

    if( pDbCltThread != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Schema");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltSigGen);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbCltThread->updateSchema( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltSigGenRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltSigGenRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbCltThread != nullptr )

} // onBtnDbCltSigGenUpdateSchemaClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltSigGenDataTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltSigGen, "Data" );
    }

} // onCmbDbCltSigGenDataTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltSigGenSelectDataClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbCltThread = CApplication::GetInstance()->getDbClientThreadSigGen();

    if( pDbCltThread != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Data");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltSigGen);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbCltThread->selectData( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltSigGenRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltSigGenRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbCltThread != nullptr )

} // onBtnDbCltSigGenSelectDataClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltSigGenUpdateDataClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbCltThread = CApplication::GetInstance()->getDbClientThreadSigGen();

    if( pDbCltThread != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltSigGen,"Data");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltSigGen);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbCltThread->updateData( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltSigGenRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltSigGenRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbCltThread != nullptr )

} // onBtnDbCltSigGenUpdateDataClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::onChkDbCltCaptureUseBlockingMethodCallsStateChanged( int i_iCheckState )
//------------------------------------------------------------------------------
{
    CApplication::GetInstance()->setUseBlockingMethodCalls( i_iCheckState == Qt::Checked, c_strObjNameDbCltCapture );
}

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltCaptureConnectTypeCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setSocketType( static_cast<Ipc::ESocketType>(i_iIdx), c_strObjNameDbCltCapture );
    }

} // onCmbDbCltCaptureConnectTypeCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltCaptureStartupTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltCapture, "Startup" );
    }

} // onCmbDbCltCaptureStartupTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltCaptureStartupClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadCapture();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Startup");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltCapture);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->startup( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltCaptureRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltCaptureRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltCaptureStartupClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltCaptureShutdownClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadCapture();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Startup");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltCapture);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->shutdown( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltCaptureRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltCaptureRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltSigGenShutdownClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltCaptureConnectTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltCapture, "Connect" );
    }

} // onCmbDbCltCaptureConnectTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltCaptureConnectClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadCapture();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Connect");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltCapture);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->connect_(iTimeout_ms, bWait, iReqIdParent);

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltCaptureRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltCaptureRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltCaptureConnectClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltCaptureDisconnectClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadCapture();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Connect");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltCapture);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->disconnect_( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltCaptureRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltCaptureRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltCaptureDisconnectClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltCaptureRegisterTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltCapture, "Register" );
    }

} // onCmbDbCltCaptureRegisterTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltCaptureRegisterClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadCapture();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Register");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        qint64   iReqIdParent = -1;
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltCapture);

        CRequest* pReq = pDbClt->register_( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltCaptureRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltCaptureRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltCaptureRegisterClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltCaptureUnregisterClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbClt = CApplication::GetInstance()->getDbClientThreadCapture();

    if( pDbClt != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Register");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltCapture);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbClt->unregister( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltCaptureRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltCaptureRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbClt != nullptr )

} // onBtnDbCltCaptureUnregisterClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltCaptureSchemaTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltCapture, "Schema" );
    }

} // onCmbDbCltCaptureSchemaTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltCaptureSelectSchemaClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbCltThread = CApplication::GetInstance()->getDbClientThreadCapture();

    if( pDbCltThread != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Schema");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltCapture);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbCltThread->selectSchema( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltCaptureRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltCaptureRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbCltThread != nullptr )

} // onBtnDbCltCaptureSelectSchemaClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltCaptureUpdateSchemaClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbCltThread = CApplication::GetInstance()->getDbClientThreadCapture();

    if( pDbCltThread != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Schema");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltCapture);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbCltThread->updateSchema( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltCaptureRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltCaptureRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbCltThread != nullptr )

} // onBtnDbCltCaptureUpdateSchemaClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onCmbDbCltCaptureDataTimeoutCurrentIndexChanged( int i_iIdx )
//------------------------------------------------------------------------------
{
    if( i_iIdx >= 0 && i_iIdx < ETimeoutCount )
    {
        CApplication::GetInstance()->setTimeout( static_cast<ETimeout>(i_iIdx), c_strObjNameDbCltCapture, "Data" );
    }

} // onCmbDbCltCaptureDataTimeoutCurrentIndexChanged

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltCaptureSelectDataClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbCltThread = CApplication::GetInstance()->getDbClientThreadCapture();

    if( pDbCltThread != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Data");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltCapture);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbCltThread->selectData( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltCaptureRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltCaptureRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbCltThread != nullptr )

} // onBtnDbCltCaptureSelectDataClicked

//------------------------------------------------------------------------------
void CWidgetCentral::onBtnDbCltCaptureUpdateDataClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CDbClientThread* pDbCltThread = CApplication::GetInstance()->getDbClientThreadCapture();

    if( pDbCltThread != nullptr )
    {
        ETimeout timeout = CApplication::GetInstance()->getTimeout(c_strObjNameDbCltCapture,"Data");
        int      iTimeout_ms = getTimeoutInMs(timeout);
        bool     bWait = CApplication::GetInstance()->useBlockingMethodCalls(c_strObjNameDbCltCapture);
        qint64   iReqIdParent = -1;

        CRequest* pReq = pDbCltThread->updateData( iTimeout_ms, bWait, iReqIdParent );

        if( isAsynchronousRequest(pReq) )
        {
            if( !QObject::connect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onDbCltCaptureRequestChanged(ZS::System::SRequestDscr)) ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }

            onDbCltCaptureRequestChanged( pReq->getDescriptor() );
        }
    } // if( pDbCltThread != nullptr )

} // onBtnDbCltCaptureUpdateDataClicked

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::updateAppReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblAppReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblAppReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblAppReqInProgress->setText("");
        }
    }

    if( m_pBarAppReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarAppReqInProgress->isVisible() )
            {
                m_pBarAppReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarAppReqInProgress->isVisible() )
            {
                m_pBarAppReqInProgress->hide();
            }
        }
        m_pBarAppReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateAppReqInProgressStatus

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbReqInProgress->setText("");
        }
    }

    if( m_pBarDbReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbReqInProgress->isVisible() )
            {
                m_pBarDbReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbReqInProgress->isVisible() )
            {
                m_pBarDbReqInProgress->hide();
            }
        }
        m_pBarDbReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbReqInProgressStatus

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltGUIConnectReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltGUIConnectReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltGUIConnectReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltGUIConnectReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltGUIConnectReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltGUIConnectReqInProgress->isVisible() )
            {
                m_pBarDbCltGUIConnectReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltGUIConnectReqInProgress->isVisible() )
            {
                m_pBarDbCltGUIConnectReqInProgress->hide();
            }
        }
        m_pBarDbCltGUIConnectReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltGUIConnectReqInProgressStatus

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltGUIRegisterReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltGUIRegisterReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltGUIRegisterReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltGUIRegisterReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltGUIRegisterReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltGUIRegisterReqInProgress->isVisible() )
            {
                m_pBarDbCltGUIRegisterReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltGUIRegisterReqInProgress->isVisible() )
            {
                m_pBarDbCltGUIRegisterReqInProgress->hide();
            }
        }
        m_pBarDbCltGUIRegisterReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltGUIRegisterReqInProgressStatus

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltGUISchemaReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltGUISchemaReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltGUISchemaReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltGUISchemaReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltGUISchemaReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltGUISchemaReqInProgress->isVisible() )
            {
                m_pBarDbCltGUISchemaReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltGUISchemaReqInProgress->isVisible() )
            {
                m_pBarDbCltGUISchemaReqInProgress->hide();
            }
        }
        m_pBarDbCltGUISchemaReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltGUISchemaReqInProgressStatus

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltGUIDataReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltGUIDataReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltGUIDataReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltGUIDataReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltGUIDataReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltGUIDataReqInProgress->isVisible() )
            {
                m_pBarDbCltGUIDataReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltGUIDataReqInProgress->isVisible() )
            {
                m_pBarDbCltGUIDataReqInProgress->hide();
            }
        }
        m_pBarDbCltGUIDataReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltGUIDataReqInProgressStatus

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltSigGenStartupReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltSigGenStartupReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltSigGenStartupReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltSigGenStartupReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltSigGenStartupReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltSigGenStartupReqInProgress->isVisible() )
            {
                m_pBarDbCltSigGenStartupReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltSigGenStartupReqInProgress->isVisible() )
            {
                m_pBarDbCltSigGenStartupReqInProgress->hide();
            }
        }
        m_pBarDbCltSigGenStartupReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltSigGenStartupReqInProgressStatus

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltSigGenConnectReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltSigGenConnectReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltSigGenConnectReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltSigGenConnectReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltSigGenConnectReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltSigGenConnectReqInProgress->isVisible() )
            {
                m_pBarDbCltSigGenConnectReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltSigGenConnectReqInProgress->isVisible() )
            {
                m_pBarDbCltSigGenConnectReqInProgress->hide();
            }
        }
        m_pBarDbCltSigGenConnectReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltSigGenConnectReqInProgressStatus

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltSigGenRegisterReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltSigGenRegisterReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltSigGenRegisterReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltSigGenRegisterReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltSigGenRegisterReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltSigGenRegisterReqInProgress->isVisible() )
            {
                m_pBarDbCltSigGenRegisterReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltSigGenRegisterReqInProgress->isVisible() )
            {
                m_pBarDbCltSigGenRegisterReqInProgress->hide();
            }
        }
        m_pBarDbCltSigGenRegisterReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltSigGenRegisterReqInProgressStatus

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltSigGenSchemaReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltSigGenSchemaReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltSigGenSchemaReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltSigGenSchemaReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltSigGenSchemaReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltSigGenSchemaReqInProgress->isVisible() )
            {
                m_pBarDbCltSigGenSchemaReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltSigGenSchemaReqInProgress->isVisible() )
            {
                m_pBarDbCltSigGenSchemaReqInProgress->hide();
            }
        }
        m_pBarDbCltSigGenSchemaReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltSigGenSchemaReqInProgressStatus

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltSigGenDataReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltSigGenDataReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltSigGenDataReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltSigGenDataReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltSigGenDataReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltSigGenDataReqInProgress->isVisible() )
            {
                m_pBarDbCltSigGenDataReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltSigGenDataReqInProgress->isVisible() )
            {
                m_pBarDbCltSigGenDataReqInProgress->hide();
            }
        }
        m_pBarDbCltSigGenDataReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltSigGenDataReqInProgressStatus

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltCaptureStartupReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltCaptureStartupReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltCaptureStartupReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltCaptureStartupReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltCaptureStartupReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltCaptureStartupReqInProgress->isVisible() )
            {
                m_pBarDbCltCaptureStartupReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltCaptureStartupReqInProgress->isVisible() )
            {
                m_pBarDbCltCaptureStartupReqInProgress->hide();
            }
        }
        m_pBarDbCltCaptureStartupReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltCaptureStartupReqInProgressStatus

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltCaptureConnectReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltCaptureConnectReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltCaptureConnectReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltCaptureConnectReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltCaptureConnectReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltCaptureConnectReqInProgress->isVisible() )
            {
                m_pBarDbCltCaptureConnectReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltCaptureConnectReqInProgress->isVisible() )
            {
                m_pBarDbCltCaptureConnectReqInProgress->hide();
            }
        }
        m_pBarDbCltCaptureConnectReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltCaptureConnectReqInProgressStatus

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltCaptureRegisterReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltCaptureRegisterReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltCaptureRegisterReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltCaptureRegisterReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltCaptureRegisterReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltCaptureRegisterReqInProgress->isVisible() )
            {
                m_pBarDbCltCaptureRegisterReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltCaptureRegisterReqInProgress->isVisible() )
            {
                m_pBarDbCltCaptureRegisterReqInProgress->hide();
            }
        }
        m_pBarDbCltCaptureRegisterReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltCaptureRegisterReqInProgressStatus

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltCaptureSchemaReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltCaptureSchemaReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltCaptureSchemaReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltCaptureSchemaReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltCaptureSchemaReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltCaptureSchemaReqInProgress->isVisible() )
            {
                m_pBarDbCltCaptureSchemaReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltCaptureSchemaReqInProgress->isVisible() )
            {
                m_pBarDbCltCaptureSchemaReqInProgress->hide();
            }
        }
        m_pBarDbCltCaptureSchemaReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltCaptureSchemaReqInProgressStatus

//------------------------------------------------------------------------------
void CWidgetCentral::updateDbCltCaptureDataReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblDbCltCaptureDataReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            m_pLblDbCltCaptureDataReqInProgress->setText(i_strRequest);
        }
        else
        {
            m_pLblDbCltCaptureDataReqInProgress->setText("");
        }
    }

    if( m_pBarDbCltCaptureDataReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarDbCltCaptureDataReqInProgress->isVisible() )
            {
                m_pBarDbCltCaptureDataReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarDbCltCaptureDataReqInProgress->isVisible() )
            {
                m_pBarDbCltCaptureDataReqInProgress->hide();
            }
        }
        m_pBarDbCltCaptureDataReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateDbCltCaptureDataReqInProgressStatus

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CWidgetCentral::eventFilter( QObject* i_pObjWatched, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bHandled = false;

    if( i_pObjWatched == m_pLblAppReqInProgress || i_pObjWatched == m_pBarAppReqInProgress
     || i_pObjWatched == m_pLblDbReqInProgress || i_pObjWatched == m_pBarDbReqInProgress
     || i_pObjWatched == m_pLblDbCltGUIConnectReqInProgress || i_pObjWatched == m_pBarDbCltGUIConnectReqInProgress
     || i_pObjWatched == m_pLblDbCltGUIRegisterReqInProgress || i_pObjWatched == m_pBarDbCltGUIRegisterReqInProgress
     || i_pObjWatched == m_pLblDbCltGUISchemaReqInProgress || i_pObjWatched == m_pBarDbCltGUISchemaReqInProgress
     || i_pObjWatched == m_pLblDbCltGUIDataReqInProgress || i_pObjWatched == m_pBarDbCltGUIDataReqInProgress
     || i_pObjWatched == m_pLblDbCltSigGenStartupReqInProgress || i_pObjWatched == m_pBarDbCltSigGenStartupReqInProgress
     || i_pObjWatched == m_pLblDbCltSigGenConnectReqInProgress || i_pObjWatched == m_pBarDbCltSigGenConnectReqInProgress
     || i_pObjWatched == m_pLblDbCltSigGenRegisterReqInProgress || i_pObjWatched == m_pBarDbCltSigGenRegisterReqInProgress
     || i_pObjWatched == m_pLblDbCltSigGenSchemaReqInProgress || i_pObjWatched == m_pBarDbCltSigGenSchemaReqInProgress
     || i_pObjWatched == m_pLblDbCltSigGenDataReqInProgress || i_pObjWatched == m_pBarDbCltSigGenDataReqInProgress
     || i_pObjWatched == m_pLblDbCltCaptureStartupReqInProgress || i_pObjWatched == m_pBarDbCltCaptureStartupReqInProgress
     || i_pObjWatched == m_pLblDbCltCaptureConnectReqInProgress || i_pObjWatched == m_pBarDbCltCaptureConnectReqInProgress
     || i_pObjWatched == m_pLblDbCltCaptureRegisterReqInProgress || i_pObjWatched == m_pBarDbCltCaptureRegisterReqInProgress
     || i_pObjWatched == m_pLblDbCltCaptureSchemaReqInProgress || i_pObjWatched == m_pBarDbCltCaptureSchemaReqInProgress
     || i_pObjWatched == m_pLblDbCltCaptureDataReqInProgress || i_pObjWatched == m_pBarDbCltCaptureDataReqInProgress )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            CMainWindow::GetInstance()->onActDebugRequestExecTreeTriggered();
            bHandled = true;
        }
    }
    else
    {
        // pass the event on to the parent class
        bHandled = QWidget::eventFilter(i_pObjWatched,i_pEv);
    }
    return bHandled;

} // eventFilter

