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
#include "ZSSys/ZSSysDataRateCalculator.h"
#include "ZSSys/ZSSysRequest.h"

class QCheckBox;
class QComboBox;
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

namespace Trace
{
class CIpcTrcClient;

namespace GUI
{
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
        int            i_iItemsCountMax = 100000,
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CWdgtTrcMthList();
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
    ZS::System::SErrResultInfo readTraceMethodFile( const QString& i_strAbsFilePath );
    ZS::System::SErrResultInfo writeTraceMethodFile( const QString& i_strAbsFilePath );
public: // instance methods
    QTextEdit* getTextEdit() { return m_pEdt; }
public: // instance methods
    bool findText( const QString& i_strExp, QTextDocument::FindFlags i_findFlags = QTextDocument::FindFlags() );
public: // instance methods
    bool getShowTimeInfo() const;
    void setShowTimeInfo( bool i_bShow );
protected: // overridables of base class QWidget
    virtual bool eventFilter( QObject* i_pObjWatched, QEvent* i_pEv ) override;
protected slots: // connected to the signals of my user controls
    void onBtnClearClicked( bool i_bChecked );
    void onChkServerTracingEnabledToggled( bool i_bChecked );
    void onChkServerUseIpcServerToggled( bool i_bChecked );
    void onBtnTrcAdminObjIdxTreeClicked( bool i_bChecked );
    void onBtnConnectClicked( bool i_bChecked );
    void onBtnFindTextNextClicked( bool i_bChecked );
    void onBtnFindTextPrevClicked( bool i_bChecked );
protected slots:
    void onDlgEditTimeSpanTooMuchDataApplied();
    void onDlgEditTimeSpanTooMuchDataAccepted();
    void onDlgEditTimeSpanTooMuchDataRejected();
protected slots: // connected to the signals of the IPC client
    void onIpcClientConnected( QObject* i_pClient );
    void onIpcClientDisconnected( QObject* i_pClient );
    void onIpcClientSettingsChanged( QObject* i_pClient );
    void onIpcClientPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots: // connected to the signals of the trace client
    void onTraceSettingsChanged( QObject* i_pTrcClient );
    void onTraceDataReceived( QObject* i_pTrcClient, const QString& i_str );
    void onTmrDataRateRefreshTimeout();
protected: // instance methods
    void addEdtItem( const QString& i_strText, const QString& i_strHtmlClrCode );
protected: // instance methods
    void normalize( QString& i_str ) const;
    void showAndCheckDataRates();
    void showEditMaxDataRateDialog();
private: // instance members
    CIpcTrcClient*                     m_pTrcClient;
    ZS::System::CDataRateCalculator    m_dataRateCalculatorBytes;
    ZS::System::CDataRateCalculator    m_dataRateCalculatorLines;
    QVector<double>                    m_arfDataRateDiffsProcTime_s;
    QVector<int>                       m_ariDataRateDiffs_linesPerSec;
    ZS::System::CRequest*              m_pReqInProgress;
    QString                            m_strThreadClrFileAbsFilePath;
    bool                               m_bShowTimeInfo;
    int                                m_iEdtItemsCountMax;
    int                                m_iEdtItems;
    int                                m_bEdtFull;
    QTextEdit*                         m_pEdt;
    QHash<QString, STrcMthThreadEntry> m_hashThreads;
    QPushButton*                       m_pBtnClear;
    QLabel*                            m_pLblServerTracingEnabled;
    QCheckBox*                         m_pChkServerTracingEnabled;
    QLabel*                            m_pLblServerUseIpcServer;
    QCheckBox*                         m_pChkServerUseIpcServer;
    QPushButton*                       m_pBtnTrcAdminObjIdxTree;
    QPushButton*                       m_pBtnConnect;
    ZS::System::GUI::CProgressBar*     m_pProgressBarCnct;
    QLabel*                            m_pLblFindText;
    QComboBox*                         m_pCmbFindText;
    QPushButton*                       m_pBtnFindTextNext;
    QPushButton*                       m_pBtnFindTextPrev;
    QTimer*                            m_pTmrDataRateRefresh;
    int                                m_iTimeSpanTooMuchData_s;
    QLabel*                            m_pLblTimeSpanTooMuchData;
    QLineEdit*                         m_pEdtTimeSpanTooMuchData;
    ZS::System::GUI::CDlgEditIntValue* m_pDlgEditTimeSpanTooMuchData;
    QLabel*                            m_pLblCurrentDataRatesClient;
    QLineEdit*                         m_pEdtCurrentDataRatesClient;
    QLabel*                            m_pLblCurrentDataRatesServer;
    QLineEdit*                         m_pEdtCurrentDataRatesServer;

}; // class CWdgtTrcMthList

} // namespace GUI

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSIpcTraceGUI_MthWdgt_h
