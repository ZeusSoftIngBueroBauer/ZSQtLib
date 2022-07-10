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

#ifndef ZSIpcLogGUI_SettingsWdgt_h
#define ZSIpcLogGUI_SettingsWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSIpcLogGUI/ZSIpcLogGUIDllMain.h"
#include "ZSIpcLog/ZSIpcLogClient.h"
#include "ZSIpcLog/ZSIpcLogServer.h"

class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QSpinBox;
class QFormLayout;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Log
{
class CIpcLogClient;
class CIpcLogServer;

namespace GUI
{
//******************************************************************************
class ZSIPCLOGGUIDLL_API CWdgtLogSettings : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtLogSettings( const QString& i_strObjName = "LogSettings", QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtLogSettings();
signals:
    void accepted();
    void rejected();
public: // instance methods
    void setServer( CIpcLogServer* i_pLogServer );
    void setClient( CIpcLogClient* i_pLogClient );
public: // instance methods
    ZS::System::SLogServerSettings getLogSettings() const;
    bool hasChanges() const;
    void applyChanges();
protected: // instance methods
    void enableGuiControls( bool i_bEnabled );
    void connectGuiControlsOnValueChangedSignals();
    void disconnectGuiControlsOnValueChangedSignals();
protected slots: // connected to the signals of my user controls
    void onChkLoggingEnabledToggled( bool i_bChecked );
    void onChkNewLoggersEnabledAsDefaultToggled( bool i_bChecked );
    void onCmbNewLoggersDefaultDetailLevelCurrentIndexChanged( int i_iDetailLevel );
    void onChkUseIpcServerToggled( bool i_bChecked );
    void onChkCacheDataIfNotConnectedToggled( bool i_bChecked );
    void onEdtCacheDataMaxArrLenValueChanged( int i_iMaxArrLen );
    void onChkUseLocalLogFileToggled( bool i_bChecked );
    void onEdtLocalLogFileAutoSaveIntervalValueChanged( int i_iInterval_ms );
    void onEdtLocalLogFileSubFileCountMaxValueChanged( int i_iCountMax );
    void onEdtLocalLogFileSubFileLineCountMaxValueChanged( int i_iCountMax );
    void onChkLocalLogFileCloseFileAfterEachWriteToggled( bool i_bChecked );
protected slots: // connected to the signals of the trace server
    void onIpcClientConnected( QObject* i_pClient );
    void onIpcClientDisconnected( QObject* i_pClient );
    void onLogSettingsChanged( QObject* i_pServer );
protected slots:
    void onBtnOkClicked( bool i_bChecked );
    void onBtnCancelClicked( bool i_bChecked );
    void onBtnResetClicked( bool i_bChecked );
    void onBtnApplyClicked( bool i_bChecked );
protected: // instance members
    CIpcLogServer* m_pLogServer;
    CIpcLogClient* m_pLogClient;
    QVBoxLayout*   m_pLyt;
    QFormLayout*   m_pLytSettings;
    QLabel*        m_pLblApplicationName;
    QLineEdit*     m_pEdtApplicationName;
    QLabel*        m_pLblServerName;
    QLineEdit*     m_pEdtServerName;
    QLabel*        m_pLblLoggingEnabled;
    QCheckBox*     m_pChkLoggingEnabled;
    QLabel*        m_pLblLoggersFileAbsFilePath;
    QLineEdit*     m_pEdtLoggersFileAbsFilePath;
    QLabel*        m_pLblNewLoggersEnabledAsDefault;
    QCheckBox*     m_pChkNewLoggersEnabledAsDefault;
    QLabel*        m_pLblNewLoggersDefaultDetailLevel;
    QComboBox*     m_pCmbNewLoggersDefaultDetailLevel;
    QLabel*        m_pLblUseIpcServer;
    QCheckBox*     m_pChkUseIpcServer;
    QLabel*        m_pLblCacheDataIfNotConnected;
    QCheckBox*     m_pChkCacheDataIfNotConnected;
    QLabel*        m_pLblCacheDataMaxArrLen;
    QSpinBox*      m_pEdtCacheDataMaxArrLen;
    QLabel*        m_pLblUseLocalLogFile;
    QCheckBox*     m_pChkUseLocalLogFile;
    QLabel*        m_pLblLocalLogFileAbsFilePath;
    QLineEdit*     m_pEdtLocalLogFileAbsFilePath;
    QLabel*        m_pLblLocalLogFileAutoSaveInterval;
    QSpinBox*      m_pEdtLocalLogFileAutoSaveInterval;
    QLabel*        m_pLblLocalLogFileSubFileCountMax;
    QSpinBox*      m_pEdtLocalLogFileSubFileCountMax;
    QLabel*        m_pLblLocalLogFileSubFileLineCountMax;
    QSpinBox*      m_pEdtLocalLogFileSubFileLineCountMax;
    QLabel*        m_pLblLocalLogFileCloseFileAfterEachWrite;
    QCheckBox*     m_pChkLocalLogFileCloseFileAfterEachWrite;
    // Buttons
    QHBoxLayout*   m_pLytBtns;
    QPushButton*   m_pBtnOk;
    QPushButton*   m_pBtnCancel;
    QPushButton*   m_pBtnReset;
    QPushButton*   m_pBtnApply;

}; // class CWdgtLogSettings

} // namespace GUI

} // namespace Log

} // namespace ZS

#endif // #ifndef ZSIpcLogGUI_SettingsWdgt_h
