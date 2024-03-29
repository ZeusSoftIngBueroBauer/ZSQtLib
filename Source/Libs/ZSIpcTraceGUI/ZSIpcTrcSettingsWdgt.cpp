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

#include "ZSIpcTraceGUI/ZSIpcTrcSettingsWdgt.h"
#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSSysGUI/ZSSysSepLine.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;


/*******************************************************************************
class CWdgtTrcSettings : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTrcSettings::CWdgtTrcSettings( const QString& i_strObjName, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pTrcServer(nullptr),
    m_pTrcClient(nullptr),
    m_pLyt(nullptr),
    m_pLytSettings(nullptr),
    m_pLblApplicationName(nullptr),
    m_pEdtApplicationName(nullptr),
    m_pLblServerName(nullptr),
    m_pEdtServerName(nullptr),
    m_pLblTracingEnabled(nullptr),
    m_pChkTracingEnabled(nullptr),
    m_pLblAdminObjFileAbsFilePath(nullptr),
    m_pEdtAdminObjFileAbsFilePath(nullptr),
    m_pLblNewTrcAdminObjsEnabledAsDefault(nullptr),
    m_pChkNewTrcAdminObjsEnabledAsDefault(nullptr),
    m_pLblNewTrcAdminObjsMethodCallsDefaultDetailLevel(nullptr),
    m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel(nullptr),
    m_pLblNewTrcAdminObjsRuntimeInfoDefaultDetailLevel(nullptr),
    m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel(nullptr),
    m_pLblUseIpcServer(nullptr),
    m_pChkUseIpcServer(nullptr),
    m_pLblCacheDataIfNotConnected(nullptr),
    m_pChkCacheDataIfNotConnected(nullptr),
    m_pLblCacheDataMaxArrLen(nullptr),
    m_pEdtCacheDataMaxArrLen(nullptr),
    m_pLblUseLocalTrcFile(nullptr),
    m_pChkUseLocalTrcFile(nullptr),
    m_pLblLocalTrcFileAbsFilePath(nullptr),
    m_pEdtLocalTrcFileAbsFilePath(nullptr),
    m_pLblLocalTrcFileAutoSaveInterval(nullptr),
    m_pEdtLocalTrcFileAutoSaveInterval(nullptr),
    m_pLblLocalTrcFileSubFileCountMax(nullptr),
    m_pEdtLocalTrcFileSubFileCountMax(nullptr),
    m_pLblLocalTrcFileSubFileLineCountMax(nullptr),
    m_pEdtLocalTrcFileSubFileLineCountMax(nullptr),
    m_pLblLocalTrcFileCloseFileAfterEachWrite(nullptr),
    m_pChkLocalTrcFileCloseFileAfterEachWrite(nullptr),
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

    STrcServerSettings trcSettings; // Default settings

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

    // <CheckBox> Tracing Enabled
    //---------------------------

    m_pLblTracingEnabled = new QLabel("Tracing Enabled:");
    m_pLblTracingEnabled->setFixedWidth(iLblWidth);
    m_pChkTracingEnabled = new QCheckBox();
    m_pChkTracingEnabled->setChecked(trcSettings.m_bEnabled);
    m_pLytSettings->addRow(m_pLblTracingEnabled, m_pChkTracingEnabled);

    // <Separator>
    //-------------

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLytSettings->addRow(pLine);

    // Admin Objects
    //==============

    // <LineEdit> Admin Objects File
    //------------------------------

    m_pLblAdminObjFileAbsFilePath = new QLabel("Admin Objects File:");
    m_pLblAdminObjFileAbsFilePath->setFixedWidth(iLblWidth);
    m_pEdtAdminObjFileAbsFilePath = new QLineEdit();
    m_pEdtAdminObjFileAbsFilePath->setEnabled(false);
    m_pLytSettings->addRow(m_pLblAdminObjFileAbsFilePath, m_pEdtAdminObjFileAbsFilePath);

    // <CheckBox> New Trace Admin Objects Default Enabled
    //---------------------------------------------------

    m_pLblNewTrcAdminObjsEnabledAsDefault = new QLabel("New Admin Objects Enabled as Default:");
    m_pLblNewTrcAdminObjsEnabledAsDefault->setFixedWidth(iLblWidth);
    m_pChkNewTrcAdminObjsEnabledAsDefault = new QCheckBox();
    m_pChkNewTrcAdminObjsEnabledAsDefault->setChecked(trcSettings.m_bNewTrcAdminObjsEnabledAsDefault);
    m_pLytSettings->addRow(m_pLblNewTrcAdminObjsEnabledAsDefault, m_pChkNewTrcAdminObjsEnabledAsDefault);

    // <SpinBox> New Trace Admin Objects Default Detail Level for Method Calls
    //------------------------------------------------------------------------

    m_pLblNewTrcAdminObjsMethodCallsDefaultDetailLevel = new QLabel("Default Detail Level for Method Calls:");
    m_pLblNewTrcAdminObjsMethodCallsDefaultDetailLevel->setFixedWidth(iLblWidth);
    m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel = new QComboBox();
    CEnumMethodTraceDetailLevel eDetailLevelMethodCalls;
    for( eDetailLevelMethodCalls = 0; eDetailLevelMethodCalls < CEnumMethodTraceDetailLevel::count(); ++eDetailLevelMethodCalls )
    {
        m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel->addItem(eDetailLevelMethodCalls.toString());
    }
    m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel->setCurrentIndex(static_cast<int>(trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel));
    m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel->setEnabled(true);
    m_pLytSettings->addRow(m_pLblNewTrcAdminObjsMethodCallsDefaultDetailLevel, m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel);

    // <SpinBox> New Trace Admin Objects Default Detail Level for Runtime Info
    //------------------------------------------------------------------------

    m_pLblNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = new QLabel("Default Detail Level for Runtime Info:");
    m_pLblNewTrcAdminObjsRuntimeInfoDefaultDetailLevel->setFixedWidth(iLblWidth);
    m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = new QComboBox();
    CEnumLogDetailLevel eDetailLevelRuntimeInfo;
    for( eDetailLevelRuntimeInfo = 0; eDetailLevelRuntimeInfo < CEnumLogDetailLevel::count(); ++eDetailLevelRuntimeInfo )
    {
        m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel->addItem(eDetailLevelRuntimeInfo.toString());
    }
    m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel->setCurrentIndex(static_cast<int>(trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel));
    m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel->setEnabled(true);
    m_pLytSettings->addRow(m_pLblNewTrcAdminObjsRuntimeInfoDefaultDetailLevel, m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel);

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
    m_pChkUseIpcServer->setChecked(trcSettings.m_bUseIpcServer);
    m_pLytSettings->addRow(m_pLblUseIpcServer, m_pChkUseIpcServer);

    // <CheckBox> Cache Data
    //----------------------

    m_pLblCacheDataIfNotConnected = new QLabel("Cache Data:");
    m_pLblCacheDataIfNotConnected->setFixedWidth(iLblWidth);
    m_pChkCacheDataIfNotConnected = new QCheckBox();
    m_pChkCacheDataIfNotConnected->setChecked(trcSettings.m_bCacheDataIfNotConnected);
    m_pLytSettings->addRow(m_pLblCacheDataIfNotConnected, m_pChkCacheDataIfNotConnected);

    // <SpinBox> Cache Data MaxArrLen
    //--------------------------------

    m_pLblCacheDataMaxArrLen = new QLabel("MaxArrLen:");
    m_pLblCacheDataMaxArrLen->setFixedWidth(iLblWidth);
    m_pEdtCacheDataMaxArrLen = new QSpinBox();
    m_pEdtCacheDataMaxArrLen->setMinimum(1);
    m_pEdtCacheDataMaxArrLen->setMaximum(100000);
    m_pEdtCacheDataMaxArrLen->setValue(trcSettings.m_iCacheDataMaxArrLen);
    m_pLytSettings->addRow( m_pLblCacheDataMaxArrLen, m_pEdtCacheDataMaxArrLen );

    // <Separator>
    //-------------

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLytSettings->addRow(pLine);

    // Local Trace File
    //=================

    // <CheckBox> Use Local Trace File
    //--------------------------------

    m_pLblUseLocalTrcFile = new QLabel("Use Local Trace File:");
    m_pLblUseLocalTrcFile->setFixedWidth(iLblWidth);
    m_pChkUseLocalTrcFile = new QCheckBox();
    m_pChkUseLocalTrcFile->setChecked(trcSettings.m_bUseLocalTrcFile);
    m_pLytSettings->addRow(m_pLblUseLocalTrcFile, m_pChkUseLocalTrcFile);

    // <Label> Trace Method File
    //--------------------------

    m_pLblLocalTrcFileAbsFilePath = new QLabel("Local Trace File:");
    m_pLblLocalTrcFileAbsFilePath->setFixedWidth(iLblWidth);
    m_pEdtLocalTrcFileAbsFilePath = new QLineEdit();
    m_pEdtLocalTrcFileAbsFilePath->setEnabled(false);
    m_pLytSettings->addRow(m_pLblLocalTrcFileAbsFilePath, m_pEdtLocalTrcFileAbsFilePath);

    // <SpinBox> Auto Save Interval
    //-----------------------------

    m_pLblLocalTrcFileAutoSaveInterval = new QLabel("Auto Save Interval:");
    m_pLblLocalTrcFileAutoSaveInterval->setFixedWidth(iLblWidth);
    m_pEdtLocalTrcFileAutoSaveInterval = new QSpinBox();
    m_pEdtLocalTrcFileAutoSaveInterval->setMinimum(0);
    m_pEdtLocalTrcFileAutoSaveInterval->setMaximum(10000);
    m_pEdtLocalTrcFileAutoSaveInterval->setValue(trcSettings.m_iLocalTrcFileAutoSaveInterval_ms);
    m_pLytSettings->addRow(m_pLblLocalTrcFileAutoSaveInterval, m_pEdtLocalTrcFileAutoSaveInterval);

    // <SpinBox> Sub File Count Max
    //-----------------------------

    m_pLblLocalTrcFileSubFileCountMax = new QLabel("Max File Count:");
    m_pLblLocalTrcFileSubFileCountMax->setFixedWidth(iLblWidth);
    m_pEdtLocalTrcFileSubFileCountMax = new QSpinBox();
    m_pEdtLocalTrcFileSubFileCountMax->setMinimum(0);
    m_pEdtLocalTrcFileSubFileCountMax->setMaximum(10000);
    m_pEdtLocalTrcFileSubFileCountMax->setValue(trcSettings.m_iLocalTrcFileSubFileCountMax);
    m_pLytSettings->addRow(m_pLblLocalTrcFileSubFileCountMax, m_pEdtLocalTrcFileSubFileCountMax);

    // <SpinBox> Sub File Line Count Max
    //----------------------------------

    m_pLblLocalTrcFileSubFileLineCountMax = new QLabel("Max Line Count:");
    m_pLblLocalTrcFileSubFileLineCountMax->setFixedWidth(iLblWidth);
    m_pEdtLocalTrcFileSubFileLineCountMax = new QSpinBox();
    m_pEdtLocalTrcFileSubFileLineCountMax->setMinimum(0);
    m_pEdtLocalTrcFileSubFileLineCountMax->setMaximum(10000);
    m_pEdtLocalTrcFileSubFileLineCountMax->setValue(trcSettings.m_iLocalTrcFileSubFileLineCountMax);
    m_pLytSettings->addRow(m_pLblLocalTrcFileSubFileLineCountMax, m_pEdtLocalTrcFileSubFileLineCountMax);

    // <CheckBox> Close Trace Method File After Each Write
    //----------------------------------------------------

    m_pLblLocalTrcFileCloseFileAfterEachWrite = new QLabel("Close File After Each Write:");
    m_pLblLocalTrcFileCloseFileAfterEachWrite->setFixedWidth(iLblWidth);
    m_pChkLocalTrcFileCloseFileAfterEachWrite = new QCheckBox();
    m_pChkLocalTrcFileCloseFileAfterEachWrite->setChecked(trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite);
    m_pLytSettings->addRow(m_pLblLocalTrcFileCloseFileAfterEachWrite, m_pChkLocalTrcFileCloseFileAfterEachWrite);

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

    QObject::connect(
        m_pBtnOk, &QPushButton::clicked,
        this, &CWdgtTrcSettings::onBtnOkClicked);

    // <Button> Cancel
    //----------------

    m_pBtnCancel = new QPushButton("Cancel");
    m_pBtnCancel->setFixedWidth(cxBtnWidth);
    m_pLytBtns->addWidget(m_pBtnCancel);
    m_pLytBtns->addSpacing(cxBtnSpacing);

    QObject::connect(
        m_pBtnCancel, &QPushButton::clicked,
        this, &CWdgtTrcSettings::onBtnCancelClicked);

    // <Button> Reset
    //---------------

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setFixedWidth(cxBtnWidth);
    m_pLytBtns->addWidget(m_pBtnReset);
    m_pLytBtns->addSpacing(cxBtnSpacing);

    QObject::connect(
        m_pBtnReset, &QPushButton::clicked,
        this, &CWdgtTrcSettings::onBtnResetClicked);

    // <Button> Apply
    //---------------

    m_pBtnApply = new QPushButton("Apply");
    m_pBtnApply->setFixedWidth(cxBtnWidth);
    m_pLytBtns->addWidget(m_pBtnApply);

    QObject::connect(
        m_pBtnApply, &QPushButton::clicked,
        this, &CWdgtTrcSettings::onBtnApplyClicked);

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
CWdgtTrcSettings::~CWdgtTrcSettings()
//------------------------------------------------------------------------------
{
    m_pTrcServer = nullptr;
    m_pTrcClient = nullptr;
    m_pLytSettings = nullptr;
    m_pLblApplicationName = nullptr;
    m_pEdtApplicationName = nullptr;
    m_pLblServerName = nullptr;
    m_pEdtServerName = nullptr;
    m_pLblTracingEnabled = nullptr;
    m_pChkTracingEnabled = nullptr;
    m_pLblAdminObjFileAbsFilePath = nullptr;
    m_pEdtAdminObjFileAbsFilePath = nullptr;
    m_pLblNewTrcAdminObjsEnabledAsDefault = nullptr;
    m_pChkNewTrcAdminObjsEnabledAsDefault = nullptr;
    m_pLblNewTrcAdminObjsMethodCallsDefaultDetailLevel = nullptr;
    m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel = nullptr;
    m_pLblNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = nullptr;
    m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = nullptr;
    m_pLblUseIpcServer = nullptr;
    m_pChkUseIpcServer = nullptr;
    m_pLblCacheDataIfNotConnected = nullptr;
    m_pChkCacheDataIfNotConnected = nullptr;
    m_pLblCacheDataMaxArrLen = nullptr;
    m_pEdtCacheDataMaxArrLen = nullptr;
    m_pLblUseLocalTrcFile = nullptr;
    m_pChkUseLocalTrcFile = nullptr;
    m_pLblLocalTrcFileAbsFilePath = nullptr;
    m_pEdtLocalTrcFileAbsFilePath = nullptr;
    m_pLblLocalTrcFileAutoSaveInterval = nullptr;
    m_pEdtLocalTrcFileAutoSaveInterval = nullptr;
    m_pLblLocalTrcFileSubFileCountMax = nullptr;
    m_pEdtLocalTrcFileSubFileCountMax = nullptr;
    m_pLblLocalTrcFileSubFileLineCountMax = nullptr;
    m_pEdtLocalTrcFileSubFileLineCountMax = nullptr;
    m_pLblLocalTrcFileCloseFileAfterEachWrite = nullptr;
    m_pChkLocalTrcFileCloseFileAfterEachWrite = nullptr;
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
void CWdgtTrcSettings::setServer( CIpcTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    if( m_pTrcClient != nullptr )
    {
        QObject::disconnect(
            m_pTrcClient, &CIpcTrcClient::connected,
            this, &CWdgtTrcSettings::onIpcClientConnected);
        QObject::disconnect(
            m_pTrcClient, &CIpcTrcClient::disconnected,
            this, &CWdgtTrcSettings::onIpcClientDisconnected);
        QObject::disconnect(
            m_pTrcClient, &CIpcTrcClient::traceSettingsChanged,
            this, &CWdgtTrcSettings::onTraceSettingsChanged);

        m_pTrcClient = nullptr;

        disconnectGuiControlsOnValueChangedSignals();
        enableGuiControls(false);
    }

    if( m_pTrcServer != nullptr )
    {
        QObject::disconnect(
            m_pTrcServer, &CIpcTrcServer::traceSettingsChanged,
            this, &CWdgtTrcSettings::onTraceSettingsChanged);

        m_pTrcServer = nullptr;

        disconnectGuiControlsOnValueChangedSignals();
        enableGuiControls(false);
    }

    m_pTrcServer = i_pTrcServer;

    if( m_pTrcServer != nullptr )
    {
        m_pEdtApplicationName->setText(QCoreApplication::applicationName());
        m_pEdtServerName->setText(m_pTrcServer->objectName());

        onTraceSettingsChanged(m_pTrcServer);

        QObject::connect(
            m_pTrcServer, &CIpcTrcServer::traceSettingsChanged,
            this, &CWdgtTrcSettings::onTraceSettingsChanged);

        connectGuiControlsOnValueChangedSignals();
        enableGuiControls(true);
    }

} // setServer

//------------------------------------------------------------------------------
void CWdgtTrcSettings::setClient( CIpcTrcClient* i_pTrcClient )
//------------------------------------------------------------------------------
{
    if( m_pTrcServer != nullptr )
    {
        QObject::disconnect(
            m_pTrcServer, &CIpcTrcServer::traceSettingsChanged,
            this, &CWdgtTrcSettings::onTraceSettingsChanged);

        m_pTrcServer = nullptr;

        disconnectGuiControlsOnValueChangedSignals();
        enableGuiControls(false);
    }

    if( m_pTrcClient != nullptr )
    {
        QObject::disconnect(
            m_pTrcClient, &CIpcTrcClient::connected,
            this, &CWdgtTrcSettings::onIpcClientConnected);
        QObject::disconnect(
            m_pTrcClient, &CIpcTrcClient::disconnected,
            this, &CWdgtTrcSettings::onIpcClientDisconnected);
        QObject::disconnect(
            m_pTrcClient, &CIpcTrcClient::traceSettingsChanged,
            this, &CWdgtTrcSettings::onTraceSettingsChanged);

        m_pTrcClient = nullptr;

        disconnectGuiControlsOnValueChangedSignals();
        enableGuiControls(false);
    }

    m_pTrcClient = i_pTrcClient;

    if( m_pTrcClient != nullptr )
    {
        onTraceSettingsChanged(m_pTrcClient);

        QObject::connect(
            m_pTrcClient, &CIpcTrcClient::connected,
            this, &CWdgtTrcSettings::onIpcClientConnected);
        QObject::connect(
            m_pTrcClient, &CIpcTrcClient::disconnected,
            this, &CWdgtTrcSettings::onIpcClientDisconnected);
        QObject::connect(
            m_pTrcClient, &CIpcTrcClient::traceSettingsChanged,
            this, &CWdgtTrcSettings::onTraceSettingsChanged);

        // GUI controls will be enabled if the trace client connects with the server.
        if( m_pTrcClient->isConnected() )
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
STrcServerSettings CWdgtTrcSettings::getTraceSettings() const
//------------------------------------------------------------------------------
{
    STrcServerSettings trcSettings;

    trcSettings.m_bEnabled = m_pChkTracingEnabled->isChecked();
    trcSettings.m_strAdminObjFileAbsFilePath = m_pEdtAdminObjFileAbsFilePath->text();
    trcSettings.m_bNewTrcAdminObjsEnabledAsDefault = m_pChkNewTrcAdminObjsEnabledAsDefault->isChecked();
    trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel =
        static_cast<EMethodTraceDetailLevel>(m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel->currentIndex());
    trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel =
        static_cast<ELogDetailLevel>(m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel->currentIndex());
    trcSettings.m_bUseIpcServer = m_pChkUseIpcServer->isChecked();
    trcSettings.m_bCacheDataIfNotConnected = m_pChkCacheDataIfNotConnected->isChecked();
    trcSettings.m_iCacheDataMaxArrLen = m_pEdtCacheDataMaxArrLen->value();
    trcSettings.m_bUseLocalTrcFile = m_pChkUseLocalTrcFile->isChecked();
    trcSettings.m_strLocalTrcFileAbsFilePath = m_pEdtLocalTrcFileAbsFilePath->text();
    trcSettings.m_iLocalTrcFileAutoSaveInterval_ms = m_pEdtLocalTrcFileAutoSaveInterval->value();
    trcSettings.m_iLocalTrcFileSubFileCountMax = m_pEdtLocalTrcFileSubFileCountMax->value();
    trcSettings.m_iLocalTrcFileSubFileLineCountMax = m_pEdtLocalTrcFileSubFileLineCountMax->value();
    trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite = m_pChkLocalTrcFileCloseFileAfterEachWrite->isChecked();

    return trcSettings;

} // getTraceSettings

//------------------------------------------------------------------------------
bool CWdgtTrcSettings::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    if( m_pTrcServer != nullptr )
    {
        STrcServerSettings trcSettingsSrv = m_pTrcServer->getTraceSettings();
        STrcServerSettings trcSettingsWdgt = getTraceSettings();
        bHasChanges = trcSettingsSrv != trcSettingsWdgt;
    }
    else if( m_pTrcClient != nullptr )
    {
        STrcServerSettings trcSettingsClt = m_pTrcClient->getTraceSettings();
        STrcServerSettings trcSettingsWdgt = getTraceSettings();
        bHasChanges = trcSettingsClt != trcSettingsWdgt;
    }
    return bHasChanges;
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::applyChanges()
//------------------------------------------------------------------------------
{
    STrcServerSettings trcSettings = getTraceSettings();

    if( m_pTrcServer != nullptr )
    {
        QObject::disconnect(
            m_pTrcServer, &CIpcTrcServer::traceSettingsChanged,
            this, &CWdgtTrcSettings::onTraceSettingsChanged);

        m_pTrcServer->setTraceSettings(trcSettings);

        QObject::connect(
            m_pTrcServer, &CIpcTrcServer::traceSettingsChanged,
            this, &CWdgtTrcSettings::onTraceSettingsChanged);
    }
    else if( m_pTrcClient != nullptr )
    {
        QObject::disconnect(
            m_pTrcClient, &CIpcTrcClient::traceSettingsChanged,
            this, &CWdgtTrcSettings::onTraceSettingsChanged);

        m_pTrcClient->setTraceSettings(trcSettings);

        QObject::connect(
            m_pTrcClient, &CIpcTrcClient::traceSettingsChanged,
            this, &CWdgtTrcSettings::onTraceSettingsChanged);
    }

    bool bHasChanges = hasChanges();

    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcSettings::enableGuiControls( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    m_pChkTracingEnabled->setEnabled(i_bEnabled);
    m_pChkNewTrcAdminObjsEnabledAsDefault->setEnabled(i_bEnabled);
    m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel->setEnabled(i_bEnabled);
    m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel->setEnabled(i_bEnabled);
    m_pChkUseIpcServer->setEnabled(i_bEnabled);
    m_pChkCacheDataIfNotConnected->setEnabled(i_bEnabled);
    m_pEdtCacheDataMaxArrLen->setEnabled(i_bEnabled);
    m_pChkUseLocalTrcFile->setEnabled(i_bEnabled);
    m_pEdtLocalTrcFileAutoSaveInterval->setEnabled(i_bEnabled);
    m_pEdtLocalTrcFileSubFileCountMax->setEnabled(i_bEnabled);
    m_pEdtLocalTrcFileSubFileLineCountMax->setEnabled(i_bEnabled);
    m_pChkLocalTrcFileCloseFileAfterEachWrite->setEnabled(i_bEnabled);
    //m_pBtnOk->setEnabled(i_bEnabled);
    //m_pBtnCancel->setEnabled(i_bEnabled);
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(i_bEnabled && bHasChanges);
    m_pBtnApply->setEnabled(i_bEnabled && bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::connectGuiControlsOnValueChangedSignals()
//------------------------------------------------------------------------------
{
    QObject::connect(
        m_pChkTracingEnabled, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkTracingEnabledToggled);
    QObject::connect(
        m_pChkNewTrcAdminObjsEnabledAsDefault, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkNewTrcAdminObjsEnabledAsDefaultToggled);
    QObject::connect(
        m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &CWdgtTrcSettings::onCmbNewTrcAdminObjsMethodCallsDefaultDetailLevelCurrentIndexChanged);
    QObject::connect(
        m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &CWdgtTrcSettings::onCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevelCurrentIndexChanged);
    QObject::connect(
        m_pChkUseIpcServer, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkUseIpcServerToggled);
    QObject::connect(
        m_pChkCacheDataIfNotConnected, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkCacheDataIfNotConnectedToggled);
    QObject::connect(
        m_pEdtCacheDataMaxArrLen, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &CWdgtTrcSettings::onEdtCacheDataMaxArrLenValueChanged);
    QObject::connect(
        m_pChkUseLocalTrcFile, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkUseLocalTrcFileToggled);
    QObject::connect(
        m_pEdtLocalTrcFileAutoSaveInterval, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &CWdgtTrcSettings::onEdtLocalTrcFileAutoSaveIntervalValueChanged);
    QObject::connect(
        m_pEdtLocalTrcFileSubFileCountMax, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &CWdgtTrcSettings::onEdtLocalTrcFileSubFileCountMaxValueChanged);
    QObject::connect(
        m_pEdtLocalTrcFileSubFileLineCountMax, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &CWdgtTrcSettings::onEdtLocalTrcFileSubFileLineCountMaxValueChanged);
    QObject::connect(
        m_pChkLocalTrcFileCloseFileAfterEachWrite, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkLocalTrcFileCloseFileAfterEachWriteToggled);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::disconnectGuiControlsOnValueChangedSignals()
//------------------------------------------------------------------------------
{
    QObject::disconnect(
        m_pChkTracingEnabled, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkTracingEnabledToggled);
    QObject::disconnect(
        m_pChkNewTrcAdminObjsEnabledAsDefault, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkNewTrcAdminObjsEnabledAsDefaultToggled);
    QObject::disconnect(
        m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &CWdgtTrcSettings::onCmbNewTrcAdminObjsMethodCallsDefaultDetailLevelCurrentIndexChanged);
    QObject::disconnect(
        m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &CWdgtTrcSettings::onCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevelCurrentIndexChanged);
    QObject::disconnect(
        m_pChkUseIpcServer, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkUseIpcServerToggled);
    QObject::disconnect(
        m_pChkCacheDataIfNotConnected, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkCacheDataIfNotConnectedToggled);
    QObject::disconnect(
        m_pEdtCacheDataMaxArrLen, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &CWdgtTrcSettings::onEdtCacheDataMaxArrLenValueChanged);
    QObject::disconnect(
        m_pChkUseLocalTrcFile, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkUseLocalTrcFileToggled);
    QObject::disconnect(
        m_pEdtLocalTrcFileAutoSaveInterval, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &CWdgtTrcSettings::onEdtLocalTrcFileAutoSaveIntervalValueChanged);
    QObject::disconnect(
        m_pEdtLocalTrcFileSubFileCountMax, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &CWdgtTrcSettings::onEdtLocalTrcFileSubFileCountMaxValueChanged);
    QObject::disconnect(
        m_pEdtLocalTrcFileSubFileLineCountMax, QOverload<int>::of(&QSpinBox::valueChanged),
        this, &CWdgtTrcSettings::onEdtLocalTrcFileSubFileLineCountMaxValueChanged);
    QObject::disconnect(
        m_pChkLocalTrcFileCloseFileAfterEachWrite, &QCheckBox::toggled,
        this, &CWdgtTrcSettings::onChkLocalTrcFileCloseFileAfterEachWriteToggled);
}

/*==============================================================================
protected slots: // connected to the signals of my user controls
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onChkTracingEnabledToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onChkNewTrcAdminObjsEnabledAsDefaultToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onCmbNewTrcAdminObjsMethodCallsDefaultDetailLevelCurrentIndexChanged( int /*i_iDetailLevel*/ )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevelCurrentIndexChanged( int /*i_iDetailLevel*/ )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onChkUseIpcServerToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onChkCacheDataIfNotConnectedToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onEdtCacheDataMaxArrLenValueChanged( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onChkUseLocalTrcFileToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onEdtLocalTrcFileAutoSaveIntervalValueChanged( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onEdtLocalTrcFileSubFileCountMaxValueChanged( int i_iCountMax )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onEdtLocalTrcFileSubFileLineCountMaxValueChanged( int i_iCountMax )
//------------------------------------------------------------------------------
{
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onChkLocalTrcFileCloseFileAfterEachWriteToggled( bool i_bChecked )
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
void CWdgtTrcSettings::onTraceSettingsChanged( QObject* /*i_pServer*/ )
//------------------------------------------------------------------------------
{
    STrcServerSettings trcSettings;

    if( m_pTrcServer != nullptr )
    {
        trcSettings = m_pTrcServer->getTraceSettings();
    }
    else // if( m_pTrcClient != nullptr )
    {
        m_pEdtApplicationName->setText(m_pTrcClient->getRemoteApplicationName());
        m_pEdtServerName->setText(m_pTrcClient->getRemoteServerName());

        trcSettings = m_pTrcClient->getTraceSettings();
    }

    m_pChkTracingEnabled->setChecked(trcSettings.m_bEnabled);
    m_pEdtAdminObjFileAbsFilePath->setText(trcSettings.m_strAdminObjFileAbsFilePath);
    m_pChkNewTrcAdminObjsEnabledAsDefault->setChecked(trcSettings.m_bNewTrcAdminObjsEnabledAsDefault);
    m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel->setCurrentIndex(
        static_cast<int>(trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel));
    m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel->setCurrentIndex(
        static_cast<int>(trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel));
    m_pChkUseIpcServer->setChecked(trcSettings.m_bUseIpcServer);
    m_pChkCacheDataIfNotConnected->setChecked(trcSettings.m_bCacheDataIfNotConnected);
    m_pEdtCacheDataMaxArrLen->setValue(trcSettings.m_iCacheDataMaxArrLen);
    m_pChkUseLocalTrcFile->setChecked(trcSettings.m_bUseLocalTrcFile);
    m_pEdtLocalTrcFileAbsFilePath->setText(trcSettings.m_strLocalTrcFileAbsFilePath);
    m_pEdtLocalTrcFileAutoSaveInterval->setValue(trcSettings.m_iLocalTrcFileAutoSaveInterval_ms);
    m_pEdtLocalTrcFileSubFileCountMax->setValue(trcSettings.m_iLocalTrcFileSubFileCountMax);
    m_pEdtLocalTrcFileSubFileLineCountMax->setValue(trcSettings.m_iLocalTrcFileSubFileLineCountMax);
    m_pChkLocalTrcFileCloseFileAfterEachWrite->setChecked(trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite);

    bool bHasChanges = hasChanges();

    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);

} // onTraceSettingsChanged

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onIpcClientConnected( QObject* i_pClient )
//------------------------------------------------------------------------------
{
    connectGuiControlsOnValueChangedSignals();
    enableGuiControls(true);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onIpcClientDisconnected( QObject* i_pClient )
//------------------------------------------------------------------------------
{
    disconnectGuiControlsOnValueChangedSignals();
    enableGuiControls(false);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onBtnOkClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( hasChanges() )
    {
        applyChanges();
    }
    emit accepted();
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onBtnCancelClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    emit rejected();
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onBtnResetClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    disconnectGuiControlsOnValueChangedSignals();

    if( m_pTrcServer != nullptr )
    {
        onTraceSettingsChanged(m_pTrcServer);
    }
    else if( m_pTrcClient != nullptr )
    {
        onTraceSettingsChanged(m_pTrcClient);
    }

    connectGuiControlsOnValueChangedSignals();
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onBtnApplyClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    applyChanges();
}
