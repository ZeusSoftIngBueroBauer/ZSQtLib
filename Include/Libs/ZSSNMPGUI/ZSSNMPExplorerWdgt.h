/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

--------------------------------------------------------------------------------

$Date: 22.01.10 22:11 $

$Revision: 6 $

$Author: Christian Bauer $

$History: ZSSNMPExplorerWdgt.h $
 * 
 * *****************  Version 6  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSSNMP/Include

*******************************************************************************/

#ifndef ZSSNMPExplorerWdgt_h
#define ZSSNMPExplorerWdgt_h

#ifdef NO_GUI
#error Module may not be used in this configuration.
#endif

#include <QtGui/qwidget.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPExplorer.h"

class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QModelIndex;
class QPushButton;
class QSplitter;
class QTextEdit;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;

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
class CTrcAdminObj;
}

namespace SNMP
{
class CInstanceNode;
class COidNode;
class CWdgtInstanceNodes;
class CWdgtOidNodes;

//******************************************************************************
class ZSSNMPDLL_API CWdgtExplorerSession : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum {
        EStateNotEditing = 0,
        EStateEditing    = 1,
        EStateCount
    }   EState;
    static QString state2Str( int i_iState );
public: // ctors and dtor
    CWdgtExplorerSession(
        CExplorer*     i_pExplorer,
        QSettings*     i_pSettingsFile,
        const QString& i_strSettingsKey = "",
        QWidget*       i_pWdgtParent = NULL );
    virtual ~CWdgtExplorerSession();
signals:
    void stateChanged( QObject* i_pWdgt, int i_iState );
protected slots: // connected to the signals of my user controls
    void onBtnHostEditClicked( bool i_bChecked );
    void onBtnSessionStartClicked( bool i_bChecked );
protected slots: // connected to the signals of the explorer
    void onExplorerHostSettingsChanged( QObject* i_pExplorer );
    void onExplorerStateChanged( QObject* i_pExplorer, int i_iState, int i_iRequestInProgress );
    void onExplorerRequestExecuted( QObject*, int i_iRequestId, const ZS::SErrResultInfo& i_errResultInfo );
protected: // instance members
    CExplorer*            m_pExplorer;
    QSettings*            m_pSettingsFile;
    QString               m_strSettingsKey;
    SHostSettings         m_hostSettings;
    EState                m_state;
    SRequestResult        m_requestResult;
    QGridLayout*          m_pLyt;
    QLabel*               m_pLblHostName;
    QLineEdit*            m_pEdtHostName;
    QLabel*               m_pLblHostPort;
    QLineEdit*            m_pEdtHostPort;
    QLabel*               m_pLblHostCommunityStr;
    QLineEdit*            m_pEdtHostCommunityStr;
    QPushButton*          m_pBtnHostEdit;
    bool                  m_bEditingHost;
    QLabel*               m_pLblSessionState;
    QLineEdit*            m_pEdtSessionState;
    QLabel*               m_pLblRequestInProgress;
    QLineEdit*            m_pEdtRequestInProgress;
    QLabel*               m_pLblProgressBar;
    CProgressBar*         m_pProgressBar;
    QPushButton*          m_pBtnSessionStart;
private: // instance members
    Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CWdgtExplorerSession

//******************************************************************************
class ZSSNMPDLL_API CWdgtExplorer : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtExplorer(
        CExplorer*     i_pExplorer,
        QSettings*     i_pSettingsFile,
        const QString& i_strSettingsKey = "",
        QWidget*       i_pWdgtParent = NULL );
    virtual ~CWdgtExplorer();
protected slots: // connected to the signals of my user controls
    void onWdgtSessionStateChanged( QObject* i_pWdgt, int i_iState );
    void onBtnOidNumericCopy2InputClicked( bool i_bChecked );
    void onBtnOidTextualCopy2InputClicked( bool i_bChecked );
    void onBtnOidModuleCopy2InputClicked( bool i_bChecked );
    void onBtnCommandGetClicked( bool i_bChecked );
    void onBtnCommandGetNextClicked( bool i_bChecked );
    void onBtnCommandGetBulkClicked( bool i_bChecked );
    void onBtnCommandWalkClicked( bool i_bChecked );
    void onBtnCommandAbortClicked( bool i_bChecked );
    void onChkLogEnableStateChanged( int i_iState );
protected slots: // connected to the signals of the explorer
    void onExplorerStateChanged( QObject* i_pExplorer, int i_iState, int i_iRequestInProgress );
    void onExplorerRequestExecuted( QObject*, int i_iRequestId, const ZS::SErrResultInfo& i_errResultInfo );
    void onExplorerSendingMessage( QObject* i_pExplorer, ZS::CMsg* i_pMsg );
    void onExplorerReceivedMessage( QObject* i_pExplorer, ZS::CMsg* i_pMsg );
protected slots: // connected to the signals of the tree view
    void onNodeSelected( COidNode* i_pOidNode );
    void onNodeSelected( CInstanceNode* i_pInstanceNode );
protected: // instance members
    CExplorer*            m_pExplorer;
    QSettings*            m_pSettingsFile;
    QString               m_strSettingsKey;
    SRequestResult        m_requestResult;
    QVBoxLayout*          m_pLytMain;
    QGroupBox*            m_pGrpSession;
    QVBoxLayout*          m_pLytGrpSession;
    CWdgtExplorerSession* m_pWdgtSession;
    QGroupBox*            m_pGrpOid;
    QVBoxLayout*          m_pLytGrpOid;
    QHBoxLayout*          m_pLytOidInput;
    QLabel*               m_pLblOidInput;
    QLineEdit*            m_pEdtOidInput;
    QHBoxLayout*          m_pLytOidNumericLine;
    QHBoxLayout*          m_pLytOidNumeric;
    QLabel*               m_pLblOidNumeric;
    QLineEdit*            m_pEdtOidNumeric;
    QPushButton*          m_pBtnOidNumericCopy2Input;
    QHBoxLayout*          m_pLytOidTextualLine;
    QHBoxLayout*          m_pLytOidTextual;
    QLabel*               m_pLblOidTextual;
    QLineEdit*            m_pEdtOidTextual;
    QPushButton*          m_pBtnOidTextualCopy2Input;
    QHBoxLayout*          m_pLytOidModuleLine;
    QHBoxLayout*          m_pLytOidModule;
    QLabel*               m_pLblOidModule;
    QLineEdit*            m_pEdtOidModule;
    QPushButton*          m_pBtnOidModuleCopy2Input;
    QGroupBox*            m_pGrpCommands;
    QHBoxLayout*          m_pLytGrpCommands;
    QPushButton*          m_pBtnGet;
    QPushButton*          m_pBtnGetNext;
    QPushButton*          m_pBtnGetBulk;
    QPushButton*          m_pBtnWalk;
    QPushButton*          m_pBtnAbort;
    QSplitter*            m_pSplitterOidNodesAndLog;
    QSplitter*            m_pSplitterOidNodes;
    CWdgtOidNodes*        m_pWdgtOidNodes;
    CWdgtInstanceNodes*   m_pWdgtInstanceNodes;
    QGroupBox*            m_pGrpLog;
    QVBoxLayout*          m_pLytGrpLog;
    QWidget*              m_pWdgtLogHeadline;
    QHBoxLayout*          m_pLytLogHeadline;
    QLabel*               m_pLblLogEnable;
    QCheckBox*            m_pChkLogEnable;
    QLabel*               m_pLblLogEnableHint;
    QTextEdit*            m_pEdtLog;
private: // instance members
    Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CWdgtExplorer

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPExplorerWdgt_h
