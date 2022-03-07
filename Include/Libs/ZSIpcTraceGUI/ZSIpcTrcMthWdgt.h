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

#ifndef ZSIpcTraceGUI_MthWdgt_h
#define ZSIpcTraceGUI_MthWdgt_h

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

#include "ZSIpcTraceGUI/ZSIpcTrcGUIDllMain.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRequest.h"

class QCheckBox;
class QLabel;
class QListWidget;
class QPushButton;
class QTextEdit;

namespace ZS
{
namespace System
{
namespace GUI
{
class CProgressBar;
}
}

namespace Trace
{
class CIpcTrcClient;

namespace GUI
{
//class CModelTrcMthThreadTree;

//******************************************************************************
struct ZSIPCTRACEGUIDLL_API STrcMthThreadEntry
//******************************************************************************
{
    STrcMthThreadEntry( int i_iCallDepth = 0, const QString& i_strHtmlClrCode = "#000000" ) :
        m_iCallDepth(i_iCallDepth),
        m_strHtmlClrCode(i_strHtmlClrCode)
    {
    }
    int     m_iCallDepth;
    QString m_strHtmlClrCode;
};

//******************************************************************************
class ZSIPCTRACEGUIDLL_API CWdgtTrcMthList : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Trace::GUI"; }
    static QString ClassName() { return "CWdgtTrcMthList"; }
public: // ctors and dtor
    CWdgtTrcMthList(
        CIpcTrcClient* i_pTrcClient,
        int            i_iItemsCountMax = 0,
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtTrcMthList();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    void progressBarConnectDblClicked();
    void textItemAdded( const QString& i_strText );
public: // instance methods
    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    void loadThreadColors( const QString& i_strAbsFilePath = "" );
    void saveThreadColors( const QString& i_strAbsFilePath = "" );
    #endif
public: // instance methods
    QTextEdit* getTextEdit() { return m_pEdt; }
protected: // overridables of base class QObject
    virtual bool eventFilter( QObject* i_pObjWatched, QEvent* i_pEv );
public: // instance methods
    void findText();
    bool find( const QString& i_strExp, QTextDocument::FindFlags i_findFlags = QTextDocument::FindFlags() );
protected slots: // connected to the signals of my user controls
    void onBtnClearClicked( bool i_bChecked );
    void onChkServerTracingEnabledToggled( bool i_bChecked );
    void onBtnConnectClicked( bool i_bChecked );
protected slots: // connected to the signals of the IPC client
    void onIpcClientConnected( QObject* i_pClient );
    void onIpcClientDisconnected( QObject* i_pClient );
    void onIpcClientSettingsChanged( QObject* i_pClient );
    void onIpcClientPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots: // connected to the signals of the trace client
    void onTraceSettingsChanged( QObject* i_pTrcClient );
    void onTraceDataReceived( QObject* i_pTrcClient, const QString& i_str );
protected: // instance methods
    void addEdtItem( const QString& i_strText, const QString& i_strHtmlClrCode );
protected: // instance methods
    void normalize( QString& i_str ) const;
private: // instance members
    CIpcTrcClient*                    m_pTrcClient;
    ZS::System::CRequest*             m_pReqInProgress;
    QString                           m_strThreadClrFileAbsFilePath;
    int                               m_iEdtItemsCountMax;
    int                               m_iEdtItems;
    int                               m_bEdtFull;
    QTextEdit*                        m_pEdt;
    QHash<QString,STrcMthThreadEntry> m_hashThreads;
    QPushButton*                      m_pBtnClear;
    QLabel*                           m_pLblServerTracingEnabled;
    QCheckBox*                        m_pChkServerTracingEnabled;
    QPushButton*                      m_pBtnConnect;
    ZS::System::GUI::CProgressBar*    m_pProgressBarCnct;

}; // class CWdgtTrcMthList

} // namespace GUI

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSIpcTraceGUI_MthWdgt_h
