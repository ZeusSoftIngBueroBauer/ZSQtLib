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

#include <QtCore/qtimer.h>
#include <QtGui/qpainter.h>
#include <QtGui/qvalidator.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qformlayout.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSIpcTraceGUI/ZSIpcTrcSettingsWdgt.h"
#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;


/*******************************************************************************
class CWdgtTrcSettings : public QWidget
*******************************************************************************/

//static const QString c_strBtnServerStartup          = "Startup";
//static const QString c_strBtnServerStartupShutdown  = "Shutdown";
//static const QString c_strBtnServerStartupAbort     = "Abort";
//
//static const QString c_strBtnHostSettingsEdit       = "Edit Settings";
//static const QString c_strBtnHostSettingsEditAccept = "Accept ...";

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTrcSettings::CWdgtTrcSettings( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pTrcServer(nullptr),
    m_pTrcClient(nullptr),
    m_pLyt(nullptr),
    m_pLytTop(nullptr),
    m_pLblTracingEnabled(nullptr),
    m_pChkTracingEnabled(nullptr),
    m_pLblAdminObjFileName(nullptr),
    m_pEdtAdminObjFileName(nullptr),
    m_pLytLocalTrcFile(nullptr),
    m_pLblLocalTrcFileHdr(nullptr),
    m_pLblUseLocalTrcFile(nullptr),
    m_pChkUseLocalTrcFile(nullptr),
    m_pLblLocalTrcFileCloseFileAfterEachWrite(nullptr),
    m_pChkLocalTrcFileCloseFileAfterEachWrite(nullptr),
    m_pLblLocalTrcFile(nullptr),
    m_pEdtLocalTrcFile(nullptr),
    m_pLytCacheData(nullptr),
    m_pLblCacheDataHdr(nullptr),
    m_pLblCacheDataIfNotConnected(nullptr),
    m_pChkCacheDataIfNotConnected(nullptr),
    m_pLblCacheDataMaxArrLen(nullptr),
    m_pEdtCacheDataMaxArrLen(nullptr)
{
    setObjectName("WdgtTrcSettings");

    m_pLyt = new QVBoxLayout();
    setLayout(m_pLyt);

    QFrame* pLine;
    int     iLblWidth = 160;

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

    // <FormLayout> at top of widget
    //------------------------------

    m_pLytTop = new QFormLayout();
    m_pLyt->addLayout(m_pLytTop);

    // <CheckBox> Tracing Enabled
    //---------------------------

    m_pLblTracingEnabled = new QLabel("Tracing Enabled:");
    m_pLblTracingEnabled->setFixedWidth(iLblWidth);
    m_pChkTracingEnabled = new QCheckBox();
    m_pLytTop->addRow( m_pLblTracingEnabled, m_pChkTracingEnabled );

    if( !QObject::connect(
        /* pObjSender   */ m_pChkTracingEnabled,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkTracingEnabledToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> Admin Objects File
    //---------------------------

    m_pLblAdminObjFileName = new QLabel("Admin Objects File:");
    m_pLblAdminObjFileName->setFixedWidth(iLblWidth);
    m_pEdtAdminObjFileName = new QLabel();
    m_pLytTop->addRow( m_pLblAdminObjFileName, m_pEdtAdminObjFileName );

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

    // Local Trace File
    //--------------------------------

    m_pLblLocalTrcFileHdr = new QLabel( "Local Trace (Log) File:" );
    m_pLyt->addWidget(m_pLblLocalTrcFileHdr);

    m_pLyt->addSpacing(5);

    m_pLytLocalTrcFile = new QFormLayout();
    m_pLyt->addLayout(m_pLytLocalTrcFile);

    // <CheckBox> Use Local Trace File
    //--------------------------------

    m_pLblUseLocalTrcFile = new QLabel("Use Local Trace File:");
    m_pLblUseLocalTrcFile->setFixedWidth(iLblWidth);
    m_pChkUseLocalTrcFile = new QCheckBox();
    m_pLytLocalTrcFile->addRow( m_pLblUseLocalTrcFile, m_pChkUseLocalTrcFile );

    if( !QObject::connect(
        /* pObjSender   */ m_pChkUseLocalTrcFile,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkUseLocalTrcFileToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <CheckBox> Close Trace Method File After Each Write
    //----------------------------------------------------

    m_pLblLocalTrcFileCloseFileAfterEachWrite = new QLabel("Close File After Each Write:");
    m_pLblLocalTrcFileCloseFileAfterEachWrite->setFixedWidth(iLblWidth);
    m_pChkLocalTrcFileCloseFileAfterEachWrite = new QCheckBox();
    m_pLytLocalTrcFile->addRow( m_pLblLocalTrcFileCloseFileAfterEachWrite, m_pChkLocalTrcFileCloseFileAfterEachWrite );

    if( !QObject::connect(
        /* pObjSender   */ m_pChkLocalTrcFileCloseFileAfterEachWrite,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkLocalTrcFileCloseFileAfterEachWriteToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Label> Trace Method File
    //--------------------------

    m_pLblLocalTrcFile = new QLabel("Local Trace File:");
    m_pLblLocalTrcFile->setFixedWidth(iLblWidth);
    m_pEdtLocalTrcFile = new QLabel();
    m_pLytLocalTrcFile->addRow( m_pLblLocalTrcFile, m_pEdtLocalTrcFile );

    // <Separator>
    //-------------

    m_pLyt->addSpacing(5);

    pLine = new QFrame();
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    m_pLyt->addWidget(pLine);

    m_pLyt->addSpacing(5);

    // Cache Data
    //--------------------------------

    m_pLblCacheDataHdr = new QLabel( "Cache Data (if no Trace Client connected):" );
    m_pLyt->addWidget(m_pLblCacheDataHdr);

    m_pLyt->addSpacing(5);

    m_pLytCacheData = new QFormLayout();
    m_pLyt->addLayout(m_pLytCacheData);

    // <CheckBox> Cache Data
    //----------------------

    m_pLblCacheDataIfNotConnected = new QLabel("Cache Data:");
    m_pLblCacheDataIfNotConnected->setFixedWidth(iLblWidth);
    m_pChkCacheDataIfNotConnected = new QCheckBox();
    m_pLytCacheData->addRow( m_pLblCacheDataIfNotConnected, m_pChkCacheDataIfNotConnected );

    if( !QObject::connect(
        /* pObjSender   */ m_pChkCacheDataIfNotConnected,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkCacheDataIfNotConnectedToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <SpinBox> Cache Data MaxArrLen
    //--------------------------------

    m_pLblCacheDataMaxArrLen = new QLabel("MaxArrLen:");
    m_pLblCacheDataMaxArrLen->setFixedWidth(iLblWidth);
    m_pEdtCacheDataMaxArrLen = new QSpinBox();
    m_pEdtCacheDataMaxArrLen->setMinimum(1);
    m_pEdtCacheDataMaxArrLen->setMaximum(100000);
    m_pLytCacheData->addRow( m_pLblCacheDataMaxArrLen, m_pEdtCacheDataMaxArrLen );

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtCacheDataMaxArrLen,
        /* szSignal     */ SIGNAL(valueChanged(int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onEdtCacheDataMaxArrLenValueChanged(int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Stretch> at bottom of main layout
    //===================================

    m_pLyt->addStretch();

} // ctor

//------------------------------------------------------------------------------
CWdgtTrcSettings::~CWdgtTrcSettings()
//------------------------------------------------------------------------------
{
    m_pTrcServer = nullptr;
    m_pTrcClient = nullptr;
    m_pLyt = nullptr;
    m_pLytTop = nullptr;
    m_pLblTracingEnabled = nullptr;
    m_pChkTracingEnabled = nullptr;
    m_pLblAdminObjFileName = nullptr;
    m_pEdtAdminObjFileName = nullptr;
    m_pLytLocalTrcFile = nullptr;
    m_pLblLocalTrcFileHdr = nullptr;
    m_pLblUseLocalTrcFile = nullptr;
    m_pChkUseLocalTrcFile = nullptr;
    m_pLblLocalTrcFileCloseFileAfterEachWrite = nullptr;
    m_pChkLocalTrcFileCloseFileAfterEachWrite = nullptr;
    m_pLblLocalTrcFile = nullptr;
    m_pEdtLocalTrcFile = nullptr;
    m_pLytCacheData = nullptr;
    m_pLblCacheDataHdr = nullptr;
    m_pLblCacheDataIfNotConnected = nullptr;
    m_pChkCacheDataIfNotConnected = nullptr;
    m_pLblCacheDataMaxArrLen = nullptr;
    m_pEdtCacheDataMaxArrLen = nullptr;

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
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) );
    }

    if( m_pTrcServer != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pTrcServer,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) );
    }

    m_pTrcClient = nullptr;

    m_pTrcServer = i_pTrcServer;

    if( m_pTrcServer != nullptr )
    {
        m_pChkTracingEnabled->setChecked(m_pTrcServer->isEnabled());
        m_pEdtAdminObjFileName->setText(m_pTrcServer->getAdminObjFileAbsoluteFilePath());
        m_pChkUseLocalTrcFile->setChecked(m_pTrcServer->isLocalTrcFileUsed());
        m_pChkLocalTrcFileCloseFileAfterEachWrite->setChecked(m_pTrcServer->getLocalTrcFileCloseFileAfterEachWrite());
        m_pEdtLocalTrcFile->setText(m_pTrcServer->getLocalTrcFileAbsoluteFilePath());
        m_pChkCacheDataIfNotConnected->setChecked(m_pTrcServer->getCacheTrcDataIfNotConnected());
        m_pEdtCacheDataMaxArrLen->setValue(m_pTrcServer->getCacheTrcDataMaxArrLen());

        if( !QObject::connect(
            /* pObjSender   */ m_pTrcServer,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }

} // setServer

//------------------------------------------------------------------------------
void CWdgtTrcSettings::setClient( CIpcTrcClient* i_pTrcClient )
//------------------------------------------------------------------------------
{
    if( m_pTrcClient != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) );
    }

    if( m_pTrcServer != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pTrcServer,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) );
    }

    m_pTrcServer = nullptr;

    m_pTrcClient = i_pTrcClient;

    if( m_pTrcClient != nullptr )
    {
        m_pChkTracingEnabled->setChecked(m_pTrcClient->isEnabled());
        m_pEdtAdminObjFileName->setText(m_pTrcClient->getAdminObjFileAbsoluteFilePath());
        m_pChkUseLocalTrcFile->setChecked(m_pTrcClient->isLocalTrcFileUsed());
        m_pChkLocalTrcFileCloseFileAfterEachWrite->setChecked(m_pTrcClient->getLocalTrcFileCloseFileAfterEachWrite());
        m_pEdtLocalTrcFile->setText(m_pTrcClient->getLocalTrcFileAbsoluteFilePath());
        m_pChkCacheDataIfNotConnected->setChecked(m_pTrcClient->getCacheTrcDataIfNotConnected());
        m_pEdtCacheDataMaxArrLen->setValue(m_pTrcClient->getCacheTrcDataMaxArrLen());

        if( !QObject::connect(
            /* pObjSender   */ m_pTrcClient,
            /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }

} // setClient

/*==============================================================================
protected slots: // connected to the signals of my user controls
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onChkTracingEnabledToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_pTrcServer != nullptr )
    {
        m_pTrcServer->setEnabled(i_bChecked);
    }
    else // if( m_pTrcClient != nullptr )
    {
        m_pTrcClient->setEnabled(i_bChecked);
    }

} // onChkTracingEnabledToggled

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onChkCacheDataIfNotConnectedToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_pTrcServer != nullptr )
    {
        m_pTrcServer->setCacheTrcDataIfNotConnected(i_bChecked);
    }
    else // if( m_pTrcClient != nullptr )
    {
        m_pTrcClient->setCacheTrcDataIfNotConnected(i_bChecked);
    }

} // onChkCacheDataIfNotConnectedToggled

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onEdtCacheDataMaxArrLenValueChanged( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    if( i_iMaxArrLen > 0 )
    {
        if( m_pTrcServer != nullptr )
        {
            m_pTrcServer->setCacheTrcDataMaxArrLen(i_iMaxArrLen);
        }
        else // if( m_pTrcClient != nullptr )
        {
            m_pTrcClient->setCacheTrcDataMaxArrLen(i_iMaxArrLen);
        }
    }

} // onEdtCacheDataMaxArrLenValueChanged

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onChkUseLocalTrcFileToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_pTrcServer != nullptr )
    {
        m_pTrcServer->setUseLocalTrcFile(i_bChecked);
    }
    else // if( m_pTrcClient != nullptr )
    {
        m_pTrcClient->setUseLocalTrcFile(i_bChecked);
    }

} // onChkUseLocalTrcFileToggled

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onChkLocalTrcFileCloseFileAfterEachWriteToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( m_pTrcServer != nullptr )
    {
        m_pTrcServer->setLocalTrcFileCloseFileAfterEachWrite(i_bChecked);
    }
    else // if( m_pTrcClient != nullptr )
    {
        m_pTrcClient->setLocalTrcFileCloseFileAfterEachWrite(i_bChecked);
    }

} // onChkLocalTrcFileCloseFileAfterEachWriteToggled

/*==============================================================================
protected slots: // connected to the signals of the trace server
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcSettings::onTraceSettingsChanged( QObject* /*i_pServer*/ )
//------------------------------------------------------------------------------
{
    if( m_pChkUseLocalTrcFile != nullptr )
    {
        if( m_pTrcServer != nullptr )
        {
            m_pChkUseLocalTrcFile->setChecked( m_pTrcServer->isLocalTrcFileUsed() );
        }
        else // if( m_pTrcClient != nullptr )
        {
            m_pChkUseLocalTrcFile->setChecked( m_pTrcClient->isLocalTrcFileUsed() );
        }
    }

    if( m_pChkLocalTrcFileCloseFileAfterEachWrite != nullptr )
    {
        if( m_pTrcServer != nullptr )
        {
            m_pChkLocalTrcFileCloseFileAfterEachWrite->setChecked( m_pTrcServer->getLocalTrcFileCloseFileAfterEachWrite() );
        }
        else // if( m_pTrcClient != nullptr )
        {
            m_pChkLocalTrcFileCloseFileAfterEachWrite->setChecked( m_pTrcClient->getLocalTrcFileCloseFileAfterEachWrite() );
        }
    }

    if( m_pEdtLocalTrcFile != nullptr )
    {
        if( m_pTrcServer != nullptr )
        {
            m_pEdtLocalTrcFile->setText( m_pTrcServer->getLocalTrcFileAbsoluteFilePath() );
        }
        else // if( m_pTrcClient != nullptr )
        {
            m_pEdtLocalTrcFile->setText( m_pTrcClient->getLocalTrcFileAbsoluteFilePath() );
        }
    }

    if( m_pChkTracingEnabled != nullptr )
    {
        if( m_pTrcServer != nullptr )
        {
            m_pChkTracingEnabled->setChecked( m_pTrcServer->isEnabled() );
        }
        else // if( m_pTrcClient != nullptr )
        {
            m_pChkTracingEnabled->setChecked( m_pTrcClient->isEnabled() );
        }
    }

    if( m_pChkCacheDataIfNotConnected != nullptr )
    {
        if( m_pTrcServer != nullptr )
        {
            m_pChkCacheDataIfNotConnected->setChecked( m_pTrcServer->getCacheTrcDataIfNotConnected() );
        }
        else // if( m_pTrcClient != nullptr )
        {
            m_pChkCacheDataIfNotConnected->setChecked( m_pTrcClient->getCacheTrcDataIfNotConnected() );
        }
    }

    if( m_pEdtCacheDataMaxArrLen != nullptr )
    {
        if( m_pTrcServer != nullptr )
        {
            m_pEdtCacheDataMaxArrLen->setValue( m_pTrcServer->getCacheTrcDataMaxArrLen() );
        }
        else // if( m_pTrcClient != nullptr )
        {
            m_pEdtCacheDataMaxArrLen->setValue( m_pTrcClient->getCacheTrcDataMaxArrLen() );
        }
    }

    if( m_pEdtAdminObjFileName != nullptr )
    {
        if( m_pTrcServer != nullptr )
        {
            m_pEdtAdminObjFileName->setText( m_pTrcServer->getAdminObjFileAbsoluteFilePath() );
        }
        else // if( m_pTrcClient != nullptr )
        {
            m_pEdtAdminObjFileName->setText( m_pTrcClient->getAdminObjFileAbsoluteFilePath() );
        }
    }

} // onTraceSettingsChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
STrcServerSettings CWdgtTrcSettings::getTraceSettings() const
//------------------------------------------------------------------------------
{
    STrcServerSettings trcSettings;

    if( m_pChkUseLocalTrcFile != nullptr )
    {
        trcSettings.m_bUseLocalTrcFile = m_pChkUseLocalTrcFile->isChecked();
    }
    if( m_pChkLocalTrcFileCloseFileAfterEachWrite != nullptr )
    {
        trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite = m_pChkLocalTrcFileCloseFileAfterEachWrite->isChecked();
    }
    if( m_pEdtLocalTrcFile != nullptr )
    {
        trcSettings.m_strLocalTrcFileAbsFilePath = m_pEdtLocalTrcFile->text();
    }
    if( m_pChkTracingEnabled != nullptr )
    {
        trcSettings.m_bEnabled = m_pChkTracingEnabled->isChecked();
    }
    if( m_pChkCacheDataIfNotConnected != nullptr )
    {
        trcSettings.m_bCacheDataIfNotConnected = m_pChkCacheDataIfNotConnected->isChecked();
    }
    if( m_pEdtCacheDataMaxArrLen != nullptr )
    {
        trcSettings.m_iCacheDataMaxArrLen = m_pEdtCacheDataMaxArrLen->value();
    }
    if( m_pEdtAdminObjFileName != nullptr )
    {
        trcSettings.m_strAdminObjFileAbsFilePath = m_pEdtAdminObjFileName->text();
    }

    return trcSettings;

} // getTraceSettings

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcSettings::fillControls( const STrcServerSettings& i_trcSettings )
//------------------------------------------------------------------------------
{
    if( m_pChkTracingEnabled != nullptr && i_trcSettings.m_bEnabled != m_pChkTracingEnabled->isChecked() )
    {
        m_pChkTracingEnabled->setChecked(i_trcSettings.m_bEnabled);
    }
    if( m_pEdtAdminObjFileName != nullptr && i_trcSettings.m_strAdminObjFileAbsFilePath != m_pEdtAdminObjFileName->text() )
    {
        m_pEdtAdminObjFileName->setText(i_trcSettings.m_strAdminObjFileAbsFilePath);
    }

    if( m_pChkUseLocalTrcFile != nullptr && i_trcSettings.m_bUseLocalTrcFile != m_pChkUseLocalTrcFile->isChecked() )
    {
        m_pChkUseLocalTrcFile->setChecked(i_trcSettings.m_bUseLocalTrcFile);
    }
    if( m_pChkLocalTrcFileCloseFileAfterEachWrite != nullptr && i_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite != m_pChkLocalTrcFileCloseFileAfterEachWrite->isChecked() )
    {
        m_pChkLocalTrcFileCloseFileAfterEachWrite->setChecked(i_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite);
    }
    if( m_pEdtLocalTrcFile != nullptr && i_trcSettings.m_strLocalTrcFileAbsFilePath != m_pEdtLocalTrcFile->text() )
    {
        m_pEdtLocalTrcFile->setText(i_trcSettings.m_strLocalTrcFileAbsFilePath);
    }

    if( m_pChkCacheDataIfNotConnected != nullptr && i_trcSettings.m_bCacheDataIfNotConnected != m_pChkCacheDataIfNotConnected->isChecked() )
    {
        m_pChkCacheDataIfNotConnected->setChecked(i_trcSettings.m_bCacheDataIfNotConnected);
    }
    if( m_pEdtCacheDataMaxArrLen != nullptr && i_trcSettings.m_iCacheDataMaxArrLen != m_pEdtCacheDataMaxArrLen->text().toInt() )
    {
        m_pEdtCacheDataMaxArrLen->setValue(i_trcSettings.m_iCacheDataMaxArrLen);
    }

} // fillControls
