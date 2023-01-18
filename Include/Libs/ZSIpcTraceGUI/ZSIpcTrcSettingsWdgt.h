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

#ifndef ZSIpcTraceGUI_ServerWdgt_h
#define ZSIpcTraceGUI_ServerWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSIpcTraceGUI/ZSIpcTrcGUIDllMain.h"
#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"

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
namespace Trace
{
class CIpcTrcClient;
class CIpcTrcServer;

namespace GUI
{
//******************************************************************************
class ZSIPCTRACEGUIDLL_API CWdgtTrcSettings : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtTrcSettings( const QString& i_strObjName = "TrcSettings", QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtTrcSettings();
signals:
    void accepted();
    void rejected();
public: // instance methods
    void setServer( CIpcTrcServer* i_pTrcServer );
    void setClient( CIpcTrcClient* i_pTrcClient );
public: // instance methods
    ZS::System::STrcServerSettings getTraceSettings() const;
    bool hasChanges() const;
    void applyChanges();
protected: // instance methods
    void enableGuiControls( bool i_bEnabled );
    void connectGuiControlsOnValueChangedSignals();
    void disconnectGuiControlsOnValueChangedSignals();
protected slots: // connected to the signals of my user controls
    void onChkTracingEnabledToggled( bool i_bChecked );
    void onChkNewTrcAdminObjsEnabledAsDefaultToggled( bool i_bChecked );
    void onCmbNewTrcAdminObjsMethodCallsDefaultDetailLevelCurrentIndexChanged( int i_iDetailLevel );
    void onCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevelCurrentIndexChanged( int i_iDetailLevel );
    void onChkUseIpcServerToggled( bool i_bChecked );
    void onChkCacheDataIfNotConnectedToggled( bool i_bChecked );
    void onEdtCacheDataMaxArrLenValueChanged( int i_iMaxArrLen );
    void onChkUseLocalTrcFileToggled( bool i_bChecked );
    void onEdtLocalTrcFileAutoSaveIntervalValueChanged( int i_iInterval_ms );
    void onEdtLocalTrcFileSubFileCountMaxValueChanged( int i_iCountMax );
    void onEdtLocalTrcFileSubFileLineCountMaxValueChanged( int i_iCountMax );
    void onChkLocalTrcFileCloseFileAfterEachWriteToggled( bool i_bChecked );
protected slots: // connected to the signals of the trace server
    void onIpcClientConnected( QObject* i_pClient );
    void onIpcClientDisconnected( QObject* i_pClient );
    void onTraceSettingsChanged( QObject* i_pServer );
protected slots:
    void onBtnOkClicked( bool i_bChecked );
    void onBtnCancelClicked( bool i_bChecked );
    void onBtnResetClicked( bool i_bChecked );
    void onBtnApplyClicked( bool i_bChecked );
protected: // instance members
    CIpcTrcServer* m_pTrcServer;
    CIpcTrcClient* m_pTrcClient;
    QVBoxLayout*   m_pLyt;
    QFormLayout*   m_pLytSettings;
    QLabel*        m_pLblApplicationName;
    QLineEdit*     m_pEdtApplicationName;
    QLabel*        m_pLblServerName;
    QLineEdit*     m_pEdtServerName;
    QLabel*        m_pLblTracingEnabled;
    QCheckBox*     m_pChkTracingEnabled;
    QLabel*        m_pLblAdminObjFileAbsFilePath;
    QLineEdit*     m_pEdtAdminObjFileAbsFilePath;
    QLabel*        m_pLblNewTrcAdminObjsEnabledAsDefault;
    QCheckBox*     m_pChkNewTrcAdminObjsEnabledAsDefault;
    QLabel*        m_pLblNewTrcAdminObjsMethodCallsDefaultDetailLevel;
    QComboBox*     m_pCmbNewTrcAdminObjsMethodCallsDefaultDetailLevel;
    QLabel*        m_pLblNewTrcAdminObjsRuntimeInfoDefaultDetailLevel;
    QComboBox*     m_pCmbNewTrcAdminObjsRuntimeInfoDefaultDetailLevel;
    QLabel*        m_pLblUseIpcServer;
    QCheckBox*     m_pChkUseIpcServer;
    QLabel*        m_pLblCacheDataIfNotConnected;
    QCheckBox*     m_pChkCacheDataIfNotConnected;
    QLabel*        m_pLblCacheDataMaxArrLen;
    QSpinBox*      m_pEdtCacheDataMaxArrLen;
    QLabel*        m_pLblUseLocalTrcFile;
    QCheckBox*     m_pChkUseLocalTrcFile;
    QLabel*        m_pLblLocalTrcFileAbsFilePath;
    QLineEdit*     m_pEdtLocalTrcFileAbsFilePath;
    QLabel*        m_pLblLocalTrcFileAutoSaveInterval;
    QSpinBox*      m_pEdtLocalTrcFileAutoSaveInterval;
    QLabel*        m_pLblLocalTrcFileSubFileCountMax;
    QSpinBox*      m_pEdtLocalTrcFileSubFileCountMax;
    QLabel*        m_pLblLocalTrcFileSubFileLineCountMax;
    QSpinBox*      m_pEdtLocalTrcFileSubFileLineCountMax;
    QLabel*        m_pLblLocalTrcFileCloseFileAfterEachWrite;
    QCheckBox*     m_pChkLocalTrcFileCloseFileAfterEachWrite;
    // Buttons
    QHBoxLayout*   m_pLytBtns;
    QPushButton*   m_pBtnOk;
    QPushButton*   m_pBtnCancel;
    QPushButton*   m_pBtnReset;
    QPushButton*   m_pBtnApply;

}; // class CWdgtTrcSettings

} // namespace GUI

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSIpcTraceGUI_ServerWdgt_h
