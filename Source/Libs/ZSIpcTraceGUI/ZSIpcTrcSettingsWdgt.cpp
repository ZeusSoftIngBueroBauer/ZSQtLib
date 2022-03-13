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

#include <QtCore/qtimer.h>
#include <QtGui/qpainter.h>
#include <QtGui/qvalidator.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
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
    m_pLblCacheDataIfNotConnected(nullptr),
    m_pChkCacheDataIfNotConnected(nullptr),
    m_pLblCacheDataMaxArrLen(nullptr),
    m_pEdtCacheDataMaxArrLen(nullptr),
    m_pLblAdminObjFileAbsFilePath(nullptr),
    m_pEdtAdminObjFileAbsFilePath(nullptr),
    m_pLblNewTrcAdminObjsEnabledAsDefault(nullptr),
    m_pChkNewTrcAdminObjsEnabledAsDefault(nullptr),
    m_pLblNewTrcAdminObjsDefaultDetailLevel(nullptr),
    m_pEdtNewTrcAdminObjsDefaultDetailLevel(nullptr),
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

    // <SpinBox> New Trace Admin Objects Default Detail Level
    //-------------------------------------------------------

    m_pLblNewTrcAdminObjsDefaultDetailLevel = new QLabel("New Admin Objects Default Detail Level:");
    m_pLblNewTrcAdminObjsDefaultDetailLevel->setFixedWidth(iLblWidth);
    m_pEdtNewTrcAdminObjsDefaultDetailLevel = new QSpinBox();
    m_pEdtNewTrcAdminObjsDefaultDetailLevel->setMinimum(0);
    m_pEdtNewTrcAdminObjsDefaultDetailLevel->setMaximum(10);
    m_pEdtNewTrcAdminObjsDefaultDetailLevel->setValue(trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel);
    m_pLytSettings->addRow(m_pLblNewTrcAdminObjsDefaultDetailLevel, m_pEdtNewTrcAdminObjsDefaultDetailLevel);

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

    // <Button> Cancel
    //----------------

    m_pBtnCancel = new QPushButton("Cancel");
    m_pBtnCancel->setFixedWidth(cxBtnWidth);
    m_pLytBtns->addWidget(m_pBtnCancel);
    m_pLytBtns->addSpacing(cxBtnSpacing);

    // <Button> Reset
    //---------------

    m_pBtnReset = new QPushButton("Reset");
    m_pBtnReset->setFixedWidth(cxBtnWidth);
    m_pLytBtns->addWidget(m_pBtnReset);
    m_pLytBtns->addSpacing(cxBtnSpacing);

    // <Button> Apply
    //---------------

    m_pBtnApply = new QPushButton("Apply");
    m_pBtnApply->setFixedWidth(cxBtnWidth);
    m_pLytBtns->addWidget(m_pBtnApply);

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
    m_pLblCacheDataIfNotConnected = nullptr;
    m_pChkCacheDataIfNotConnected = nullptr;
    m_pLblCacheDataMaxArrLen = nullptr;
    m_pEdtCacheDataMaxArrLen = nullptr;
    m_pLblAdminObjFileAbsFilePath = nullptr;
    m_pEdtAdminObjFileAbsFilePath = nullptr;
    m_pLblNewTrcAdminObjsEnabledAsDefault = nullptr;
    m_pChkNewTrcAdminObjsEnabledAsDefault = nullptr;
    m_pLblNewTrcAdminObjsDefaultDetailLevel = nullptr;
    m_pEdtNewTrcAdminObjsDefaultDetailLevel = nullptr;
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
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(connected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIpcClientConnected(QObject*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(disconnected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIpcClientDisconnected(QObject*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) );

        m_pTrcClient = nullptr;

        disconnectGuiControlsOnValueChangedSignals();
        enableGuiControls(false);
    }

    if( m_pTrcServer != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pTrcServer,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) );

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

        if( !QObject::connect(
            /* pObjSender   */ m_pTrcServer,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

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
            /* pObjSender   */ m_pTrcServer,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) );

        m_pTrcServer = nullptr;

        disconnectGuiControlsOnValueChangedSignals();
        enableGuiControls(false);
    }

    if( m_pTrcClient != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(connected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIpcClientConnected(QObject*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(disconnected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIpcClientDisconnected(QObject*)) );
        QObject::disconnect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) );

        m_pTrcClient = nullptr;

        disconnectGuiControlsOnValueChangedSignals();
        enableGuiControls(false);
    }

    m_pTrcClient = i_pTrcClient;

    if( m_pTrcClient != nullptr )
    {
        onTraceSettingsChanged(m_pTrcClient);

        if( !QObject::connect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(connected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIpcClientConnected(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(disconnected(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onIpcClientDisconnected(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        // GUI controls will be enabled if the trace client connects with the server.
    }

} // setClient

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcSettings::enableGuiControls( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    m_pChkTracingEnabled->setEnabled(i_bEnabled);
    m_pChkCacheDataIfNotConnected->setEnabled(i_bEnabled);
    m_pEdtCacheDataMaxArrLen->setEnabled(i_bEnabled);
    m_pChkNewTrcAdminObjsEnabledAsDefault->setEnabled(i_bEnabled);
    m_pEdtNewTrcAdminObjsDefaultDetailLevel->setEnabled(i_bEnabled);
    m_pChkUseLocalTrcFile->setEnabled(i_bEnabled);
    m_pEdtLocalTrcFileAutoSaveInterval->setEnabled(i_bEnabled);
    m_pEdtLocalTrcFileSubFileCountMax->setEnabled(i_bEnabled);
    m_pEdtLocalTrcFileSubFileLineCountMax->setEnabled(i_bEnabled);
    m_pChkLocalTrcFileCloseFileAfterEachWrite->setEnabled(i_bEnabled);
    m_pBtnOk->setEnabled(i_bEnabled);
    m_pBtnCancel->setEnabled(i_bEnabled);
    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(i_bEnabled && bHasChanges);
    m_pBtnApply->setEnabled(i_bEnabled && bHasChanges);
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::connectGuiControlsOnValueChangedSignals()
//------------------------------------------------------------------------------
{
    if( !QObject::connect(
        /* pObjSender   */ m_pChkTracingEnabled,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkTracingEnabledToggled(bool)) ) )
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
        /* pObjSender   */ m_pChkNewTrcAdminObjsEnabledAsDefault,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkNewTrcAdminObjsEnabledAsDefaultToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pEdtNewTrcAdminObjsDefaultDetailLevel,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtNewTrcAdminObjsDefaultDetailLevelValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pChkUseLocalTrcFile,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkUseLocalTrcFileToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pEdtLocalTrcFileAutoSaveInterval,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalTrcFileAutoSaveIntervalValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pEdtLocalTrcFileSubFileCountMax,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalTrcFileSubFileCountMaxValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pEdtLocalTrcFileSubFileLineCountMax,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalTrcFileSubFileLineCountMaxValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pChkLocalTrcFileCloseFileAfterEachWrite,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkLocalTrcFileCloseFileAfterEachWriteToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pBtnOk,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnOkClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pBtnCancel,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCancelClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pBtnReset,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnResetClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pBtnApply,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnApplyClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
}

//------------------------------------------------------------------------------
void CWdgtTrcSettings::disconnectGuiControlsOnValueChangedSignals()
//------------------------------------------------------------------------------
{
    if( !QObject::disconnect(
        /* pObjSender   */ m_pChkTracingEnabled,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkTracingEnabledToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pChkCacheDataIfNotConnected,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkCacheDataIfNotConnectedToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pEdtCacheDataMaxArrLen,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtCacheDataMaxArrLenValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pChkNewTrcAdminObjsEnabledAsDefault,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkNewTrcAdminObjsEnabledAsDefaultToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pEdtNewTrcAdminObjsDefaultDetailLevel,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtNewTrcAdminObjsDefaultDetailLevelValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pChkUseLocalTrcFile,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkUseLocalTrcFileToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pEdtLocalTrcFileAutoSaveInterval,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalTrcFileAutoSaveIntervalValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pEdtLocalTrcFileSubFileCountMax,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalTrcFileSubFileCountMaxValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pEdtLocalTrcFileSubFileLineCountMax,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtLocalTrcFileSubFileLineCountMaxValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pChkLocalTrcFileCloseFileAfterEachWrite,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkLocalTrcFileCloseFileAfterEachWriteToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pBtnOk,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnOkClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pBtnCancel,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCancelClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pBtnReset,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnResetClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::disconnect(
        /* pObjSender   */ m_pBtnApply,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnApplyClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
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
void CWdgtTrcSettings::onEdtNewTrcAdminObjsDefaultDetailLevelValueChanged( int i_iDetailLevel )
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
    m_pChkCacheDataIfNotConnected->setChecked(trcSettings.m_bCacheDataIfNotConnected);
    m_pEdtCacheDataMaxArrLen->setValue(trcSettings.m_iCacheDataMaxArrLen);
    m_pEdtAdminObjFileAbsFilePath->setText(trcSettings.m_strAdminObjFileAbsFilePath);
    m_pChkNewTrcAdminObjsEnabledAsDefault->setChecked(trcSettings.m_bNewTrcAdminObjsEnabledAsDefault);
    m_pEdtNewTrcAdminObjsDefaultDetailLevel->setValue(trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel);
    m_pChkUseLocalTrcFile->setChecked(trcSettings.m_bUseLocalTrcFile);
    m_pEdtLocalTrcFileAbsFilePath->setText(trcSettings.m_strLocalTrcFileAbsFilePath);
    m_pEdtLocalTrcFileAutoSaveInterval->setValue(trcSettings.m_iLocalTrcFileAutoSaveInterval_ms);
    m_pEdtLocalTrcFileSubFileCountMax->setValue(trcSettings.m_iLocalTrcFileSubFileCountMax);
    m_pEdtLocalTrcFileSubFileLineCountMax->setValue(trcSettings.m_iLocalTrcFileSubFileLineCountMax);
    m_pChkLocalTrcFileCloseFileAfterEachWrite->setChecked(trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite);

    m_pBtnReset->setEnabled(hasChanges());
    m_pBtnApply->setEnabled(hasChanges());

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

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
STrcServerSettings CWdgtTrcSettings::getTraceSettings() const
//------------------------------------------------------------------------------
{
    STrcServerSettings trcSettings;

    trcSettings.m_bEnabled = m_pChkTracingEnabled->isChecked();
    trcSettings.m_bCacheDataIfNotConnected = m_pChkCacheDataIfNotConnected->isChecked();
    trcSettings.m_iCacheDataMaxArrLen = m_pEdtCacheDataMaxArrLen->value();
    trcSettings.m_strAdminObjFileAbsFilePath = m_pEdtAdminObjFileAbsFilePath->text();
    trcSettings.m_bNewTrcAdminObjsEnabledAsDefault = m_pChkNewTrcAdminObjsEnabledAsDefault->isChecked();
    trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel = m_pEdtNewTrcAdminObjsDefaultDetailLevel->value();
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

    STrcServerSettings trcSettingsCltSrv;

    if( m_pTrcServer != nullptr )
    {
        trcSettingsCltSrv = m_pTrcServer->getTraceSettings();
    }
    else if( m_pTrcClient != nullptr )
    {
        trcSettingsCltSrv = m_pTrcClient->getTraceSettings();
    }
    if( getTraceSettings() != trcSettingsCltSrv )
    {
        bHasChanges = true;
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
            /* pObjSender   */ m_pTrcServer,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) );

        m_pTrcServer->setTraceSettings(trcSettings);

        if( !QObject::connect(
            /* pObjSender   */ m_pTrcServer,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }
    else if( m_pTrcClient != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) );

        m_pTrcClient->setTraceSettings(trcSettings);

        if( !QObject::connect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }

    bool bHasChanges = hasChanges();
    m_pBtnReset->setEnabled(bHasChanges);
    m_pBtnApply->setEnabled(bHasChanges);
}
