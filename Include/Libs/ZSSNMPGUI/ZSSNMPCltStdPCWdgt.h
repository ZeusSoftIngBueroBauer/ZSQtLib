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

$Revision: 3 $

$Author: Christian Bauer $

$History: ZSSNMPCltStdPCWdgt.h $
 * 
 * *****************  Version 3  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSSNMPCltStdPC/Include

*******************************************************************************/

#ifndef ZSSNMPCltStdPCWdgt_h
#define ZSSNMPCltStdPCWdgt_h

#ifdef NO_GUI
#error Module may not be used in this configuration.
#endif

#include <QtGui/qwidget.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPC.h"
#include "ZSSNMP/Include/ZSSNMPExplorer.h"

class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QModelIndex;
class QPushButton;
class QSplitter;
class QTableView;
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
//******************************************************************************
class ZSSNMPDLL_API CWdgtCltStdPC : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtCltStdPC(
        CCltStdPC*     i_pCltStdPC,
        QSettings*     i_pSettingsFile,
        const QString& i_strSettingsKey = "",
        QWidget*       i_pWdgtParent = NULL );
    virtual ~CWdgtCltStdPC();
protected slots: // connected to the signals of my user controls
    void onBtnUpdateStartClicked( bool i_bChecked );
    void onBtnUpdateIntervalsEditClicked( bool i_bChecked );
    void onBtnOidsEditClicked( bool i_bChecked );
protected slots: // connected to the signals of the explorer
    void onCltStdPCUpdateIntervalsChanged( QObject* i_pCltStdPC );
    void onCltStdPCOidsChanged( QObject* i_pCltStdPC );
    void onCltStdPCStateChanged( QObject* i_pCltStdPC, int i_iState, int i_iRequestInProgress );
    void onCltStdPCReceivedSystemValues( QObject* i_pCltStdPC );
    void onCltStdPCReceivedProcessorValues( QObject* i_pCltStdPC );
    void onCltStdPCReceivedStorageValues( QObject* i_pCltStdPC );
protected: // instance members
    CCltStdPC*            m_pCltStdPC;
    CExplorer*            m_pExplorer;
    QSettings*            m_pSettingsFile;
    QString               m_strSettingsKey;
    QVBoxLayout*          m_pLytMain;
    SAutoUpdateIntervals  m_autoUpdateIntervals;
    QGroupBox*            m_pGrpAutoUpdateIntervals;
    QGridLayout*          m_pLytGrpAutoUpdateIntervals;
    QPushButton*          m_pBtnAutoUpdateStart;
    bool                  m_bAutoUpdateStarted;
    QLabel*               m_pLblAutoOpenSessionInterval_ms;
    QLineEdit*            m_pEdtAutoOpenSessionInterval_ms;
    QLabel*               m_pLblAutoUpdateIntervalAllValues_ms;
    QLineEdit*            m_pEdtAutoUpdateIntervalAllValues_ms;
    QLabel*               m_pLblAutoUpdateIntervalProcessorValues_ms;
    QLineEdit*            m_pEdtAutoUpdateIntervalProcessorValues_ms;
    QLabel*               m_pLblAutoUpdateIntervalStorageValues_ms;
    QLineEdit*            m_pEdtAutoUpdateIntervalStorageValues_ms;
    QPushButton*          m_pBtnAutoUpdateIntervalsEdit;
    bool                  m_bEditingAutoUpdateIntervals;
    SOidsStdPC            m_oids;
    QGroupBox*            m_pGrpOids;
    QGridLayout*          m_pLytGrpOids;
    QPushButton*          m_pBtnOidsEdit;
    bool                  m_bEditingOids;
    QLabel*               m_pLblOidSystem;
    QLineEdit*            m_pEdtOidSystem;
    QLabel*               m_pLblOidSysDescr;
    QLineEdit*            m_pEdtOidSysDescr;
    QLabel*               m_pLblOidSysName;
    QLineEdit*            m_pEdtOidSysName;
    QLabel*               m_pLblOidHrProcessorLoad;
    QLineEdit*            m_pEdtOidHrProcessorLoad;
    QLabel*               m_pLblOidHrStorageType;
    QLineEdit*            m_pEdtOidHrStorageType;
    QLabel*               m_pLblOidHrStorageDescr;
    QLineEdit*            m_pEdtOidHrStorageDescr;
    QLabel*               m_pLblOidHrStorageAllocationUnits;
    QLineEdit*            m_pEdtOidHrStorageAllocationUnits;
    QLabel*               m_pLblOidHrStorageSize;
    QLineEdit*            m_pEdtOidHrStorageSize;
    QLabel*               m_pLblOidHrStorageUsed;
    QLineEdit*            m_pEdtOidHrStorageUsed;
    QSplitter*            m_pSplitterValueTables;
    QGroupBox*            m_pGrpSystem;
    QVBoxLayout*          m_pLytGrpSystem;
    QTableView*           m_pTableViewSystem;
    QGroupBox*            m_pGrpProcessors;
    QVBoxLayout*          m_pLytGrpProcessors;
    QTableView*           m_pTableViewProcessors;
    QGroupBox*            m_pGrpStorages;
    QVBoxLayout*          m_pLytGrpStorages;
    QTableView*           m_pTableViewStorages;
private: // instance members
    Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CWdgtCltStdPC

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPExplorerWdgt_h
