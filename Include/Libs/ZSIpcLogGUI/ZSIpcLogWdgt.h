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

#ifndef ZSIpcLogGUI_Wdgt_h
#define ZSIpcLogGUI_Wdgt_h

#include <QtCore/qabstractitemmodel.h>
#include <QtGui/qtextdocument.h>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qitemdelegate.h>
#include <QtGui/qtableview.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qitemdelegate.h>
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSIpcLogGUI/ZSIpcLogGUIDllMain.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysDataRateCalculator.h"
#include "ZSSys/ZSSysRequest.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QTimer;

namespace ZS
{
namespace System
{
namespace GUI
{
class CDlgEditIntValue;
class CProgressBar;
}
}

namespace Log
{
class CIpcLogClient;

namespace GUI
{
//******************************************************************************
class ZSIPCLOGGUIDLL_API CWdgtLog : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Log::GUI"; }
    static QString ClassName() { return "CWdgtLog"; }
public: // ctors and dtor
    CWdgtLog(
        CIpcLogClient* i_pLogClient,
        int            i_iItemsCountMax = 100000,
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtLog();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    void progressBarConnectDblClicked();
    void textItemAdded( const QString& i_strText );
public: // instance methods
    QString getDefaultThreadColorsFilePath() const;
    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    void loadThreadColors( const QString& i_strAbsFilePath = "" );
    void saveThreadColors( const QString& i_strAbsFilePath = "" );
    #endif
public: // instance methods
    ZS::System::SErrResultInfo readLogFile( const QString& i_strAbsFilePath );
    ZS::System::SErrResultInfo writeLogFile( const QString& i_strAbsFilePath );
public: // instance methods
    QTextEdit* getTextEdit() { return m_pEdt; }
public: // instance methods
    void findText();
    bool find( const QString& i_strExp, QTextDocument::FindFlags i_findFlags = QTextDocument::FindFlags() );
protected: // overridables of base class QWidget
    virtual bool eventFilter( QObject* i_pObjWatched, QEvent* i_pEv );
protected slots: // connected to the signals of my user controls
    void onBtnClearClicked( bool i_bChecked );
    void onChkServerLoggingEnabledToggled( bool i_bChecked );
    void onChkServerUseIpcServerToggled( bool i_bChecked );
    void onBtnLoggerIdxTreeClicked( bool i_bChecked );
    void onBtnConnectClicked( bool i_bChecked );
protected slots: // connected to the signals of the IPC client
    void onIpcClientConnected( QObject* i_pClient );
    void onIpcClientDisconnected( QObject* i_pClient );
    void onIpcClientSettingsChanged( QObject* i_pClient );
    void onIpcClientPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots: // connected to the signals of the trace client
    void onLogSettingsChanged( QObject* i_pLogClient );
    void onLogDataReceived( QObject* i_pLogClient, const QString& i_str );
protected: // instance methods
    void addEdtItem( const QString& i_strText, const QString& i_strHtmlClrCode );
protected: // instance methods
    void normalize( QString& i_str ) const;
private: // instance members
    CIpcLogClient*                 m_pLogClient;
    ZS::System::CRequest*          m_pReqInProgress;
    QString                        m_strThreadClrFileAbsFilePath;
    int                            m_iEdtItemsCountMax;
    int                            m_iEdtItems;
    int                            m_bEdtFull;
    QTextEdit*                     m_pEdt;
    QHash<QString, QString>        m_hashThreadColors;
    QPushButton*                   m_pBtnClear;
    QLabel*                        m_pLblServerLoggingEnabled;
    QCheckBox*                     m_pChkServerLoggingEnabled;
    QLabel*                        m_pLblServerUseIpcServer;
    QCheckBox*                     m_pChkServerUseIpcServer;
    QPushButton*                   m_pBtnLoggerIdxTree;
    QPushButton*                   m_pBtnConnect;
    ZS::System::GUI::CProgressBar* m_pProgressBarCnct;

}; // class CWdgtLog

} // namespace GUI

} // namespace Log

} // namespace ZS

#endif // #ifndef ZSIpcLogGUI_Wdgt_h
