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
class QGroupBox;
class QLabel;
class QSpinBox;
class QTextEdit;
class QFormLayout;
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
    CWdgtTrcSettings( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtTrcSettings();
public: // instance methods
    void setServer( CIpcTrcServer* i_pTrcServer );
    void setClient( CIpcTrcClient* i_pTrcClient );
protected slots: // connected to the signals of my user controls
    void onChkTracingEnabledToggled( bool i_bChecked );
    void onChkUseLocalTrcFileToggled( bool i_bChecked );
    void onChkLocalTrcFileCloseFileAfterEachWriteToggled( bool i_bChecked );
    void onChkCacheDataIfNotConnectedToggled( bool i_bChecked );
    void onEdtCacheDataMaxArrLenValueChanged( int i_iMaxArrLen );
protected slots: // connected to the signals of the trace server
    void onTraceSettingsChanged( QObject* i_pServer );
protected: // instance methods
    STrcServerSettings getTraceSettings() const;
protected: // instance methods
    void fillControls( const STrcServerSettings& i_trcSettings );
protected: // instance members
    CIpcTrcServer* m_pTrcServer;
    CIpcTrcClient* m_pTrcClient;
    QVBoxLayout*   m_pLyt;
    QFormLayout*   m_pLytTop;
    QLabel*        m_pLblTracingEnabled;
    QCheckBox*     m_pChkTracingEnabled;
    QLabel*        m_pLblAdminObjFileName;
    QLabel*        m_pEdtAdminObjFileName;
    QFormLayout*   m_pLytLocalTrcFile;
    QLabel*        m_pLblLocalTrcFileHdr;
    QLabel*        m_pLblUseLocalTrcFile;
    QCheckBox*     m_pChkUseLocalTrcFile;
    QLabel*        m_pLblLocalTrcFileCloseFileAfterEachWrite;
    QCheckBox*     m_pChkLocalTrcFileCloseFileAfterEachWrite;
    QLabel*        m_pLblLocalTrcFile;
    QLabel*        m_pEdtLocalTrcFile;
    QFormLayout*   m_pLytCacheData;
    QLabel*        m_pLblCacheDataHdr;
    QLabel*        m_pLblCacheDataIfNotConnected;
    QCheckBox*     m_pChkCacheDataIfNotConnected;
    QLabel*        m_pLblCacheDataMaxArrLen;
    QSpinBox*      m_pEdtCacheDataMaxArrLen;

}; // class CWdgtTrcSettings

} // namespace GUI

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSIpcTraceGUI_ServerWdgt_h
