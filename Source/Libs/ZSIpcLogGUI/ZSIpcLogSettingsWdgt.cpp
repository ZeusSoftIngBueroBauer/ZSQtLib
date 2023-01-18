/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qtimer.h>
#include <QtGui/qpainter.h>
#include <QtGui/qvalidator.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qformlayout.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSIpcLogGUI/ZSIpcLogSettingsWdgt.h"
#include "ZSIpcLog/ZSIpcLogClient.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Log;
using namespace ZS::Log::GUI;


/*******************************************************************************
class CWdgtLogSettings : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtLogSettings::CWdgtLogSettings( const QString& i_strObjName, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pLogServer(nullptr),
    m_pLogClient(nullptr),
    m_pLyt(nullptr),
    m_pLytSettings(nullptr),
    m_pLblApplicationName(nullptr),
    m_pEdtApplicationName(nullptr),
    m_pLblServerName(nullptr),
    m_pEdtServerName(nullptr),
    m_pLblLoggingEnabled(nullptr),
    m_pChkLoggingEnabled(nullptr),
    m_pLblLoggersFileAbsFilePath(nullptr),
    m_pEdtLoggersFileAbsFilePath(nullptr),
    m_pLblNewLoggersEnabledAsDefault(nullptr),
    m_pChkNewLoggersEnabledAsDefault(nullptr),
    m_pLblNewLoggersDefaultDetailLevel(nullptr),
    m_pCmbNewLoggersDefaultDetailLevel(nullptr),
    m_pLblUseIpcServer(nullptr),
    m_pChkUseIpcServer(nullptr),
    m_pLblCacheDataIfNotConnected(nullptr),
    m_pChkCacheDataIfNotConnected(nullptr),
    m_pLblCacheDataMaxArrLen(nullptr),
    m_pEdtCacheDataMaxArrLen(nullptr),
    m_pLblUseLocalLogFile(nullptr),
    m_pChkUseLocalLogFile(nullptr),
    m_pLblLocalLogFileAbsFilePath(nullptr),
    m_pEdtLocalLogFileAbsFilePath(nullptr),
    m_pLblLocalLogFileAutoSaveInterval(nullptr),
    m_pEdtLocalLogFileAutoSaveInterval(nullptr),
    m_pLblLocalLogFileSubFileCountMax(nullptr),
    m_pEdtLocalLogFileSubFileCountMax(nullptr),
    m_pLblLocalLogFileSubFileLineCountMax(nullptr),
    m_pEdtLocalLogFileSubFileLineCountMax(nullptr),
    m_pLblLocalLogFileCloseFileAfterEachWrite(nullptr),
    m_pChkLocalLogFileCloseFileAfterEachWrite(nullptr),
    m_pLytBtns(nullptr),
    m_pBtnOk(nullptr),
    m_pBtnCancel(nullptr),
    m_pBtnReset(nullptr),
    m_pBtnApply(nullptr)
{
    setObjectName(i_strObjName);

    // Main Layout
    //============

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    // Settings
    //=========

    SLogServerSettings logSettings; // Default settings

    m_pLytSettings = new QFormLayout();
    m_pLyt->addLayout(m_pLytSettings);

    QFrame* pLine;
    int     iLblWidth = 200;

    // Application and Server Name
    //============================

    // <LineEdit> Application Name
    //----------------------------

    m_pLblApplicationName = new QLabel("Application Name:");
    m_pLblApplicationName->setFixedWidth(iLblWidth);
    m_pEdtApplicationName = new QLineEdit();
    m_pEdtApplicationName->setEnabled(false);
    m_pLytSettings->addRow(m_pLblApplicationName, m_pEdtApplicationName);

    // <LineEdit> Server Name
    //-----------------------

    m_pLblServerName = new QLabel("Server Name:");
    m_pLblServerName->setFixedWidth(iLblWidth);
    m_pEdtServerName = new QLineEdit();
    m_pEdtServerName->setEnabled(false);
    m_pLytSettings->addRow(m_pLblServerName, m_pEdtServerName);

    // <CheckBox> Logging Enabled
    //---------------------------

    m_pLblLoggingEnabled = new QLabel("Logging Enabled:");
    m_pLblLoggingEnabled->setFixedWidth(iLblWidth);
    m_pChkLoggingEnabled = new QCheckBox();
    m_pChkLoggingEnabled->setChecked(logSettings.m_bEnabled);
    m_pLytSettings->addRow(m_pLblLoggingEnabled, m_pChkLoggingEnabled);

    // <Separator>
    //-------------

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLytSettings->addRow(pLine);

    // Loggers
    //========

    // <LineEdit> Loggers File
    //------------------------

    m_pLblLoggersFileAbsFilePath = new QLabel("Loggers File:");
    m_pLblLoggersFileAbsFilePath->setFixedWidth(iLblWidth);
    m_pEdtLoggersFileAbsFilePath = new QLineEdit();
    m_pEdtLoggersFileAbsFilePath->setEnabled(false);
    m_pLytSettings->addRow(m_pLblLoggersFileAbsFilePath, m_pEdtLoggersFileAbsFilePath);

    // <CheckBox> New Loggers Default Enabled
    //---------------------------------------

    m_pLblNewLoggersEnabledAsDefault = new QLabel("New Loggers Enabled as Default:");
    m_pLblNewLoggersEnabledAsDefault->setFixedWidth(iLblWidth);
    m_pChkNewLoggersEnabledAsDefault = new QCheckBox();
    m_pChkNewLoggersEnabledAsDefault->setChecked(logSettings.m_bNewLoggersEnabledAsDefault);
    m_pLytSettings->addRow(m_pLblNewLoggersEnabledAsDefault, m_pChkNewLoggersEnabledAsDefault);

    // <ComboBox> New Loggers Default Detail Level
    //--------------------------------------------

    m_pLblNewLoggersDefaultDetailLevel = new QLabel("New Loggers Default Detail Level:");
    m_pLblNewLoggersDefaultDetailLevel->setFixedWidth(iLblWidth);
    m_pCmbNewLoggersDefaultDetailLevel = new QComboBox();
    CEnumLogDetailLevel eLogDetailLevel;
    for( eLogDetailLevel = 0; eLogDetailLevel < CEnumLogDetailLevel::count(); ++eLogDetailLevel )
    {
        m_pCmbNewLoggersDefaultDetailLevel->addItem(eLogDetailLevel.toString());
    }
    m_pCmbNewLoggersDefaultDetailLevel->setCurrentIndex(static_cast<int>(logSettings.m_eNewLoggersDefaultDetailLevel));
    m_pCmbNewLoggersDefaultDetailLevel->setEnabled(true);
    m_pLytSettings->addRow(m_pLblNewLoggersDefaultDetailLevel, m_pCmbNewLoggersDefaultDetailLevel);

    // <Separator>
    //-------------

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLytSettings->addRow(pLine);

    // Ipc Server
    //===========

    // <CheckBox> Use Ipc Server
    //--------------------------

    m_pLblUseIpcServer = new QLabel("Remote Tracing:");
    m_pLblUseIpcServer->setFixedWidth(iLblWidth);
    m_pChkUseIpcServer = new QCheckBox();
    m_pChkUseIpcServer->setChecked(logSettings.m_bUseIpcServer);
    m_pLytSettings->addRow(m_pLblUseIpcServer, m_pChkUseIpcServer);

    // <CheckBox> Cache Data
    //----------------------

    m_pLblCacheDataIfNotConnected = new QLabel("Cache Data:");
    m_pLblCacheDataIfNotConnected->setFixedWidth(iLblWidth);
    m_pChkCacheDataIfNotConnected = new QCheckBox();
    m_pChkCacheDataIfNotConnected->setChecked(logSettings.m_bCacheDataIfNotConnected);
    m_pLytSettings->addRow(m_pLblCacheDataIfNotConnected, m_pChkCacheDataIfNotConnected);

    // <SpinBox> Cache Data MaxArrLen
    //--------------------------------

    m_pLblCacheDataMaxArrLen = new QLabel("MaxArrLen:");
    m_pLblCacheDataMaxArrLen->setFixedWidth(iLblWidth);
    m_pEdtCacheDataMaxArrLen = new QSpinBox();
    m_pEdtCacheDataMaxArrLen->setMinimum(1);
    m_pEdtCacheDataMaxArrLen->setMaximum(100000);
    m_pEdtCacheDataMaxArrLen->setValue(logSettings.m_iCacheDataMaxArrLen);
    m_pLytSettings->addRow( m_pLblCacheDataMaxArrLen, m_pEdtCacheDataMaxArrLen );

    // <Separator>
    //-------------

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLytSettings->addRow(pLine);

    // Local Log File
    //=================

    // <CheckBox> Use Local Log File
    //--------------------------------

    m_pLblUseLocalLogFile = new QLabel("Use Local Log File:");
    m_pLblUseLocalLogFile->setFixedWidth(iLblWidth);
    m_pChkUseLocalLogFile = new QCheckBox();
    m_pChkUseLocalLogFile->setChecked(logSettings.m_bUseLocalLogFile);
    m_pLytSettings->addRow(m_pLblUseLocalLogFile, m_pChkUseLocalLogFile);

    // <Label> Local Log File
    //--------------------------

    m_pLblLocalLogFileAbsFilePath = new QLabel("Local Log File:");
    m_pLblLocalLogFileAbsFilePath->setFixedWidth(iLblWidth);
    m_pEdtLocalLogFileAbsFilePath = new QLineEdit();
    m_pEdtLocalLogFileAbsFilePath->setEnabled(false);
    m_pLytSettings->addRow(m_pLblLocalLogFileAbsFilePath, m_pEdtLocalLogFileAbsFilePath);

    // <SpinBox> Auto Save Interval
    //-----------------------------

    m_pLblLocalLogFileAutoSaveInterval = new QLabel("Auto Save Interval:");
    m_pLblLocalLogFileAutoSaveInterval->setFixedWidth(iLblWidth);
    m_pEdtLocalLogFileAutoSaveInterval = new QSpinBox();
    m_pEdtLocalLogFileAutoSaveInterval->setMinimum(0);
    m_pEdtLocalLogFileAutoSaveInterval->setMaximum(10000);
    m_pEdtLocalLogFileAutoSaveInterval->setValue(logSettings.m_iLocalLogFileAutoSaveInterval_ms);
    m_pLytSettings->addRow(m_pLblLocalLogFileAutoSaveInterval, m_pEdtLocalLogFileAutoSaveInterval);

    // <SpinBox> Sub File Count Max
    //-----------------------------

    m_pLblLocalLogFileSubFileCountMax = new QLabel("Max File Count:");
    m_pLblLocalLogFileSubFileCountMax->setFixedWidth(iLblWidth);
    m_pEdtLocalLogFileSubFileCountMax = new QSpinBox();
    m_pEdtLocalLogFileSubFileCountMax->setMinimum(0);
    m_pEdtLocalLogFileSubFileCountMax->setMaximum(10000);
    m_pEdtLocalLogFileSubFileCountMax->setValue(logSettings.m_iLocalLogFileSubFileCountMax);
    m_pLytSettings->addRow(m_pLblLocalLogFileSubFileCountMax, m_pEdtLocalLogFileSubFileCountMax);

    // <SpinBox> Sub File Line Count Max
    //----------------------------------

    m_pLblLocalLogFileSubFileLineCountMax = new QLabel("Max Line Count:");
    m_pLblLocalLogFileSubFileLineCountMax->setFixedWidth(iLblWidth);
    m_pEdtLocalLogFileSubFileLineCountMax = new QSpinBox();
    m_pEdtLocalLogFileSubFileLineCountMax->setMinimum(0);
    m_pEdtLocalLogFileSubFileLineCountMax->setMaximum(10000);
    m_pEdtLocalLogFileSubFileLineCountMax->setValue(logSettings.m_iLocalLogFileSubFileLineCountMax);
    m_pLytSettings->addRow(m_pLblLocalLogFileSubFileLineCountMax, m_pEdtLocalLogFileSubFileLineCountMax);

    // <CheckBox> Close Trace Method File After Each Write
    //----------------------------------------------------

    m_pLblLocalLogFileCloseFileAfterEachWrite = new QLabel("Close File After Each Write:");
    m_pLblLocalLogFileCloseFileAfterEachWrite->setFixedWidth(iLblWidth);
    m_pChkLocalLogFileCloseFileAfterEachWrite = new QCheckBox();
    m_pChkLocalLogFileCloseFileAfterEachWrite->setChecked(logSettings.m_bLocalLogFileCloseFileAfterEachWrite);
    m_pLytSettings->addRow(m_pLblLocalLogFileCloseFileAfterEachWrite, m_pChkLocalLogFileCloseFileAfterEachWrite);

    // <ButtonRow>
    //============

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new CSepLine();
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

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

    QObject::connect(m_pBtnOk, &QPushButton::clicked, this, &CWdgtLogSettings::onBtnOkClicked);

    // <Button> Cancel
    //----------------

    m_pBtnCancel = new QPushButton("Cancel");
    m_pBtnCancel->setFixedWidth(cxBtnWidth);
    m_pLytBtns->addWidget(m_pBtnCancel);
    m_pLytBtns->addSpacing(cxBtnSpacing);

    QObject::connect(m_pBtnCancel, &QPushButton::clicked, this, &CWdgtLogSettings::onBtnCancelClicked);

    // <Button> Reset
    //---------------

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setFixedWidth(cxBtnWidth);
    m_pLytBtns->addWidget(m_pBtnReset);
    m_pLytBtns->addSpacing(cxBtnSpacing);

    QObject::connect(m_pBtnReset, &QPushButton::clicked, this, &CWdgtLogSettings::onBtnResetClicked);

    // <Button> Apply
    //---------------

    m_pBtnApply = new QPushButton("Apply");
    m_pBtnApply->setFixedWidth(cxBtnWidth);
    m_pLytBtns->addWidget(m_pBtnApply);

    QObject::connect(m_pBtnApply, &QPushButton::clicked, this, &CWdgtLogSettings::onBtnApplyClicked);

    // <Stretch> at right side of buttons
    //-----------------------------------

    m_pLytBtns->addStretch();

    // Stretch at bottom of main layout
    //=================================

    m_pLyt->addStretch();

    // <GUIControlStates>
    //===================

    // If neither a server nor a client is set the GUI controls are disabled.
    enableGuiControls(false);

} // ctor

//------------------------------------------------------------------------------
CWdgtLogSettings::~CWdgtLogSettings()
//------------------------------------------------------------------------------
{
    m_pLogServer = nullptr;
    m_pLogClient = nullptr;
    m_pLytSettings = nullptr;
    m_pLblApplicationName = nullptr;
    m_pEdtApplicationName = nullptr;
    m_pLblServerName = nullptr;
    m_pEdtServerName = nullptr;
    m_pLblLoggingEnabled = nullptr;
    m_pChkLoggingEnabled = nullptr;
    m_pLblLoggersFileAbsFilePath = nullptr;
    m_pEdtLoggersFileAbsFilePath = nullptr;
    m_pLblNewLoggersEnabledAsDefault = nullptr;
    m_pChkNewLoggersEnabledAsDefault = nullptr;
    m_pLblNewLoggersDefaultDetailLevel = nullptr;
    m_pCmbNewLoggersDefaultDetailLevel = nullptr;
    m_pLblUseIpcServer = nullptr;
    m_pChkUseIpcServer = nullptr;
    m_pLblCacheDataIfNotConnected = nullptr;
    m_pChkCacheDataIfNotConnected = nullptr;
    m_pLblCacheDataMaxArrLen = nullptr;
    m_pEdtCacheDataMaxArrLen = nullptr;
    m_pLblUseLocalLogFile = nullptr;
    m_pChkUseLocalLogFile = nullptr;
    m_pLblLocalLogFileAbsFilePath = nullptr;
    m_pEdtLocalLogFileAbsFilePath = nullptr;
    m_pLblLocalLogFileAutoSaveInterval = nullptr;
    m_pEdtLocalLogFileAutoSaveInterval = nullptr;
    m_pLblLocalLogFileSubFileCountMax = nullptr;
    m_pEdtLocalLogFileSubFileCountMax = nullptr;
    m_pLblLocalLogFileSubFileLineCountMax = nullptr;
    m_pEdtLocalLogFileSubFileLineCountMax = nullptr;
    m_pLblLocalLogFileCloseFileAfterEachWrite = nullptr;
    m_pChkLocalLogFileCloseFileAfterEachWrite = nullptr;
    m_pLytBtns = nullptr;
    m_pBtnOk = nullptr;
    m_pBtnCancel = nullptr;
    m_pBtnReset = nullptr;
    m_pBtnApply = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtLogSettings::setServer( CIpcLogServer* i_pLogServer )
//------------------------------------------------------------------------------
{
    if( m_pLogClient != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pLogClient,
            /* szSignal     */ SIGNAL(connected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIpcClientConnected(QObject*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pLogClient,
            /* szSignal     */ SIGNAL(disconnected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIpcClientDisconnected(QObject*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pLogClient,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onLogSettingsChanged(QObject*)) );

        m_pLogClient = nullptr;

        disconnectGuiControlsOnValueChangedSignals();
        enableGuiControls(false);
    }

    if( m_pLogServer != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pLogServer,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onLogSettingsChanged(QObject*)) );

        m_pLogServer = nullptr;

        disconnectGuiControlsOnValueChangedSignals();
        enableGuiControls(false);
    }

    m_pLogServer = i_pLogServer;

    if( m_pLogServer != nullptr )
    {
        m_pEdtApplicationName->setText(QCoreApplication::applicationName());
        m_pEdtServerName->setText(m_pLogServer->objectName());

        onLogSettingsChanged(m_pLogServer);

        QObject::connect(
            m_pLogServer, &CIpcLogServer::logSettingsChanged,
            this, &CWdgtLogSettings::onLogSettingsChanged);

        connectGuiControlsOnValueChangedSignals();
        enableGuiControls(true);
    }

} // setServer

//------------------------------------------------------------------------------
void CWdgtLogSettings::setClient( CIpcLogClient* i_pLogClient )
//------------------------------------------------------------------------------
{
    if( m_pLogServer != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pLogServer,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onLogSettingsChanged(QObject*)) );

        m_pLogServer = nullptr;

        disconnectGuiControlsOnValueChangedSignals();
        enableGuiControls(false);
    }

    if( m_pLogClient != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pLogClient,
            /* szSignal     */ SIGNAL(connected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIpcClientConnected(QObject*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pLogClient,
            /* szSignal     */ SIGNAL(disconnected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIpcClientDisconnected(QObject*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pLogClient,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onLogSettingsChanged(QObject*)) );

        m_pLogClient = nullptr;

        disconnectGuiControlsOnValueChangedSignals();
        enableGuiControls(false);
    }

    m_pLogClient = i_pLogClient;

    if( m_pLogClient != nullptr )
    {
        onLogSettingsChanged(m_pLogClient);

        QObject::connect(m_pLogClient, &CIpcLogClient::connected, this, &CWdgtLogSettings::onIpcClientConnected);
        QObject::connect(m_pLogClient, &CIpcLogClient::logSettingsChanged, this, &CWdgtLogSettings::onLogSettingsChanged);

        // GUI controls will be enabled if the trace client connects with the server.
        if( m_pLogClient->isConnected() )
        {
            connectGuiControlsOnValueChangedSignals();
            enableGuiControls(true);
        }
    }

} // setClient

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SLogServerSettings CWdgtLogSettings::getLogSettings() const
//------------------------------------------------------------------------------
{
    SLogServerSettings logSettings;

    logSettings.m_bEnabled = m_pChkLoggingEnabled->isChecked();
    logSettings.m_strLoggerFileAbsFilePath = m_pEdtLoggersFileAbsFilePath->text();
    logSettings.m_bNewLoggersEnabledAsDefault = m_pChkNewLoggersEnabledAsDefault->isChecked();
    logSettings.m_eNewLoggersDefaultDetailLevel =
        static_cast<ELogDetailLevel>(m_pCmbNewLoggersDefaultDetailLevel->currentIndex());
    logSettings.m_bUseIpcServer = m_pChkUseIpcServer->isChecked();
    logSettings.m_bCacheDataIfNotConnected = m_pChkCacheDataIfNotConnected->isChecked();
    logSettings.m_iCacheDataMaxArrLen = m_pEdtCacheDataMaxArrLen->value();
    logSettings.m_bUseLocalLogFile = m_pChkUseLocalLogFile->isChecked();
    logSettings.m_strLocalLogFileAbsFilePath = m_pEdtLocalLogFileAbsFilePath->text();
    logSettings.m_iLocalLogFileAutoSaveInterval_ms = m_pEdtLocalLogFileAutoSaveInterval->value();
    logSettings.m_iLocalLogFileSubFileCountMax = m_pEdtLocalLogFileSubFileCountMax->value();
    logSettings.m_iLocalLogFileSubFileLineCountMax = m_pEdtLocalLogFileSubFileLineCountMax->value();
    logSettings.m_bLocalLogFileCloseFileAfterEachWrite = m_pChkLocalLogFileCloseFileAfterEachWrite->isChecked();

    return logSettings;

} // getLogSettings

//------------------------------------------------------------------------------
bool CWdgtLogSettings::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    if( m_pLogServer != nullptr )
    {
        SLogServerSettings logSettingsSrv = m_pLogServer->getLogSettings();
        SLogServerSettings logSettingsWdgt = getLogSettings();
        bHasChanges = logSettingsSrv != logSettingsWdgt;
    }
    else if( m_pLogClient != nullptr )
    {
        SLogServerSettings logSettingsClt = m_pLogClient->getLogSettings();
        SLogServerSettings logSettingsWdgt = getLogSettings();
        bHasChanges = logSettingsClt != logSettingsWdgt;
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::applyChanges()
//------------------------------------------------------------------------------
{
    SLogServerSettings logSettings = getLogSettings();

    if( m_pLogServer != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pLogServer,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onLogSettingsChanged(QObject*)) );

        m_pLogServer->setLogSettings(logSettings);

        QObject::connect(m_pLogServer, &CIpcLogServer::logSettingsChanged, this, &CWdgtLogSettings::onLogSettingsChanged);
    }
    else if( m_pLogClient != nullptr )
    {
        QObject::disconnect(m_pLogClient, &CIpcLogClient::logSettingsChanged, this, &CWdgtLogSettings::onLogSettingsChanged);

        m_pLogClient->setLogSettings(logSettings);

        QObject::connect(m_pLogClient, &CIpcLogClient::logSettingsChanged, this, &CWdgtLogSettings::onLogSettingsChanged);
    }

    bool bHasChanges = hasChanges();

    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtLogSettings::enableGuiControls( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    m_pChkLoggingEnabled->setEnabled(i_bEnabled);
    m_pChkNewLoggersEnabledAsDefault->setEnabled(i_bEnabled);
    m_pCmbNewLoggersDefaultDetailLevel->setEnabled(i_bEnabled);
    m_pChkUseIpcServer->setEnabled(i_bEnabled);
    m_pChkCacheDataIfNotConnected->setEnabled(i_bEnabled);
    m_pEdtCacheDataMaxArrLen->setEnabled(i_bEnabled);
    m_pChkUseLocalLogFile->setEnabled(i_bEnabled);
    m_pEdtLocalLogFileAutoSaveInterval->setEnabled(i_bEnabled);
    m_pEdtLocalLogFileSubFileCountMax->setEnabled(i_bEnabled);
    m_pEdtLocalLogFileSubFileLineCountMax->setEnabled(i_bEnabled);
    m_pChkLocalLogFileCloseFileAfterEachWrite->setEnabled(i_bEnabled);
    //m_pBtnOk->setEnabled(i_bEnabled);
    //m_pBtnCancel->setEnabled(i_bEnabled);
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(i_bEnabled && bHasChanges);
    m_pBtnApply->setEnabled(i_bEnabled && bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::connectGuiControlsOnValueChangedSignals()
//------------------------------------------------------------------------------
{
    if( !QObject::connect(
        /* pObjSender   */ m_pChkLoggingEnabled,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkLoggingEnabledToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pChkNewLoggersEnabledAsDefault,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkNewLoggersEnabledAsDefaultToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pCmbNewLoggersDefaultDetailLevel,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbNewLoggersDefaultDetailLevelCurrentIndexChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pChkUseIpcServer,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkUseIpcServerToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pChkCacheDataIfNotConnected,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkCacheDataIfNotConnectedToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pEdtCacheDataMaxArrLen,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtCacheDataMaxArrLenValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pChkUseLocalLogFile,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkUseLocalLogFileToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pEdtLocalLogFileAutoSaveInterval,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalLogFileAutoSaveIntervalValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pEdtLocalLogFileSubFileCountMax,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalLogFileSubFileCountMaxValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pEdtLocalLogFileSubFileLineCountMax,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalLogFileSubFileLineCountMaxValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pChkLocalLogFileCloseFileAfterEachWrite,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkLocalLogFileCloseFileAfterEachWriteToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::disconnectGuiControlsOnValueChangedSignals()
//------------------------------------------------------------------------------
{
    QObject::disconnect(
        /* pObjSender   */ m_pChkLoggingEnabled,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkLoggingEnabledToggled(bool)) );
    QObject::disconnect(
        /* pObjSender   */ m_pChkNewLoggersEnabledAsDefault,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkNewLoggersEnabledAsDefaultToggled(bool)) );
    QObject::disconnect(
        /* pObjSender   */ m_pCmbNewLoggersDefaultDetailLevel,
        /* szSignal     */ SIGNAL(currentIndexChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onCmbNewLoggersDefaultDetailLevelCurrentIndexChanged(int)) );
    QObject::disconnect(
        /* pObjSender   */ m_pChkUseIpcServer,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkUseIpcServerToggled(bool)) );
    QObject::disconnect(
        /* pObjSender   */ m_pChkCacheDataIfNotConnected,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkCacheDataIfNotConnectedToggled(bool)) );
    QObject::disconnect(
        /* pObjSender   */ m_pEdtCacheDataMaxArrLen,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtCacheDataMaxArrLenValueChanged(int)) );
    QObject::disconnect(
        /* pObjSender   */ m_pChkUseLocalLogFile,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkUseLocalLogFileToggled(bool)) );
    QObject::disconnect(
        /* pObjSender   */ m_pEdtLocalLogFileAutoSaveInterval,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalLogFileAutoSaveIntervalValueChanged(int)) );
    QObject::disconnect(
        /* pObjSender   */ m_pEdtLocalLogFileSubFileCountMax,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalLogFileSubFileCountMaxValueChanged(int)) );
    QObject::disconnect(
        /* pObjSender   */ m_pEdtLocalLogFileSubFileLineCountMax,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalLogFileSubFileLineCountMaxValueChanged(int)) );
    QObject::disconnect(
        /* pObjSender   */ m_pChkLocalLogFileCloseFileAfterEachWrite,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkLocalLogFileCloseFileAfterEachWriteToggled(bool)) );
}

/*==============================================================================
protected slots: // connected to the signals of my user controls
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtLogSettings::onChkLoggingEnabledToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onChkNewLoggersEnabledAsDefaultToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onCmbNewLoggersDefaultDetailLevelCurrentIndexChanged( int /*i_iDetailLevel*/ )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onChkUseIpcServerToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onChkCacheDataIfNotConnectedToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onEdtCacheDataMaxArrLenValueChanged( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onChkUseLocalLogFileToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onEdtLocalLogFileAutoSaveIntervalValueChanged( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onEdtLocalLogFileSubFileCountMaxValueChanged( int i_iCountMax )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onEdtLocalLogFileSubFileLineCountMaxValueChanged( int i_iCountMax )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onChkLocalLogFileCloseFileAfterEachWriteToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

/*==============================================================================
protected slots: // connected to the signals of the trace server
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtLogSettings::onLogSettingsChanged( QObject* /*i_pServer*/ )
//------------------------------------------------------------------------------
{
    SLogServerSettings logSettings;

    if( m_pLogServer != nullptr )
    {
        logSettings = m_pLogServer->getLogSettings();
    }
    else // if( m_pLogClient != nullptr )
    {
        m_pEdtApplicationName->setText(m_pLogClient->getRemoteApplicationName());
        m_pEdtServerName->setText(m_pLogClient->getRemoteServerName());

        logSettings = m_pLogClient->getLogSettings();
    }

    m_pChkLoggingEnabled->setChecked(logSettings.m_bEnabled);
    m_pEdtLoggersFileAbsFilePath->setText(logSettings.m_strLoggerFileAbsFilePath);
    m_pChkNewLoggersEnabledAsDefault->setChecked(logSettings.m_bNewLoggersEnabledAsDefault);
    m_pCmbNewLoggersDefaultDetailLevel->setCurrentIndex(
        static_cast<int>(logSettings.m_eNewLoggersDefaultDetailLevel));
    m_pChkUseIpcServer->setChecked(logSettings.m_bUseIpcServer);
    m_pChkCacheDataIfNotConnected->setChecked(logSettings.m_bCacheDataIfNotConnected);
    m_pEdtCacheDataMaxArrLen->setValue(logSettings.m_iCacheDataMaxArrLen);
    m_pChkUseLocalLogFile->setChecked(logSettings.m_bUseLocalLogFile);
    m_pEdtLocalLogFileAbsFilePath->setText(logSettings.m_strLocalLogFileAbsFilePath);
    m_pEdtLocalLogFileAutoSaveInterval->setValue(logSettings.m_iLocalLogFileAutoSaveInterval_ms);
    m_pEdtLocalLogFileSubFileCountMax->setValue(logSettings.m_iLocalLogFileSubFileCountMax);
    m_pEdtLocalLogFileSubFileLineCountMax->setValue(logSettings.m_iLocalLogFileSubFileLineCountMax);
    m_pChkLocalLogFileCloseFileAfterEachWrite->setChecked(logSettings.m_bLocalLogFileCloseFileAfterEachWrite);

    bool bHasChanges = hasChanges();

    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);

} // onLogSettingsChanged

//------------------------------------------------------------------------------
void CWdgtLogSettings::onIpcClientConnected( QObject* i_pClient )
//------------------------------------------------------------------------------
{
    connectGuiControlsOnValueChangedSignals();
    enableGuiControls(true);
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onIpcClientDisconnected( QObject* i_pClient )
//------------------------------------------------------------------------------
{
    disconnectGuiControlsOnValueChangedSignals();
    enableGuiControls(false);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtLogSettings::onBtnOkClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( hasChanges() )
    {
        applyChanges();
    }
    emit accepted();
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onBtnCancelClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    emit rejected();
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onBtnResetClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    disconnectGuiControlsOnValueChangedSignals();

    if( m_pLogServer != nullptr )
    {
        onLogSettingsChanged(m_pLogServer);
    }
    else if( m_pLogClient != nullptr )
    {
        onLogSettingsChanged(m_pLogClient);
    }

    connectGuiControlsOnValueChangedSignals();
}

//------------------------------------------------------------------------------
void CWdgtLogSettings::onBtnApplyClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    applyChanges();
}
