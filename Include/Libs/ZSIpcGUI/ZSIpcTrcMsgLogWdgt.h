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

#ifndef ZSIpcGUI_TrcMsgLogWdgt_h
#define ZSIpcGUI_TrcMsgLogWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSIpcGUI/ZSIpcGUIDllMain.h"
#include "ZSSys/ZSSysAux.h"

class QHBoxLayout;
class QVBoxLayout;

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSettings;
class QTextEdit;
class QTimer;

namespace ZS
{
namespace Ipc
{
class CServer;
class CClient;

namespace GUI
{
//******************************************************************************
struct ZSIPCGUIDLL_API STrcMsgLogSettings
//******************************************************************************
{
public: // ctors and dtor
    STrcMsgLogSettings(
        bool         i_bMsgLogEnabled = false,
        unsigned int i_uMsgLogMaxItems = 1000 ) :
        m_bEnabled(i_bMsgLogEnabled),
        m_uMaxItems(i_uMsgLogMaxItems)
    {
    }
public: // struct members
    bool         m_bEnabled;
    unsigned int m_uMaxItems;

}; // struct STrcMsgLogSettings


//******************************************************************************
class ZSIPCGUIDLL_API CWdgtTrcMsgLog : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc::GUI"; }
    static QString ClassName() { return "CWdgtTrcMsgLog"; }
public: // ctors and dtor
    CWdgtTrcMsgLog(
        const STrcMsgLogSettings& i_settings,
        QWidget*                  i_pWdgtParent = nullptr );
    virtual ~CWdgtTrcMsgLog();
public: // instance methods
    void setClient( CClient* i_pClient );
    void setServer( CServer* i_pServer );
public: // instance methods
    void setSettings( const STrcMsgLogSettings& i_settings );
    STrcMsgLogSettings getSettings() const { return m_settings; }
public: // instance methods which may be called from any but not just the GUIMainThread
    void addItem( ZS::System::ETransmitDir i_transmitDir, bool i_bBold, const QString& i_strMsg );
public: // overridables of inherited class QObject
    bool event( QEvent* i_pEv );
protected slots:
    void onChkEnabledToggled( bool i_bChecked );
    void onEdtMaxItemCountEditingFinished();
    void onBtnClearClicked( bool i_bChecked );
protected:
    CClient*           m_pClient;
    CServer*           m_pServer;
    STrcMsgLogSettings m_settings;
    QVBoxLayout*       m_pLyt;
    QHBoxLayout*       m_pLytLineEditSettings;
    QLabel*            m_pLblEnabled;
    QCheckBox*         m_pChkEnabled;
    QLabel*            m_pLblMaxItemCount;
    QLineEdit*         m_pEdtMaxItemCount;
    QPushButton*       m_pBtnClear;
    unsigned int       m_uLogItemCount;
    bool               m_bLogIsFull;
    QTextEdit*         m_pEdtLog;

}; // class CWdgtTrcMsgLog

} // namespace GUI

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpcGUI_TrcMsgLogWdgt_h
