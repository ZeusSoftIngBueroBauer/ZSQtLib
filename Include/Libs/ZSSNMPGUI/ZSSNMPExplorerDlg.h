/*******************************************************************************

Copyright 2004 - 2010 by    ZeusSoft, Ing. Buero Bauer
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

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#ifndef ZSSNMPExplorerDlg_h
#define ZSSNMPExplorerDlg_h

#ifdef NO_GUI
#error Module may not be used in this configuration.
#endif

#include <QtGui/qdialog.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPExplorer.h"

class QSettings;
class QVBoxLayout;

namespace ZS
{
namespace SNMP
{
class CWdgtExplorer;

//******************************************************************************
class ZSSNMPDLL_API CDlgExplorer : public QDialog
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CDlgExplorer(
        const QString& i_strWindowTitle,
        CExplorer*     i_pExplorer,
        QSettings*     i_pSettingsFile,
        const QString& i_strSettingsKey = "",
        QWidget*       i_pWdgtParent = NULL );
    virtual ~CDlgExplorer();
signals:
    void closed();
public: // instance methods
    void show();
    void hide();
protected: // overridables of base class QWidget
    virtual void closeEvent( QCloseEvent* i_pEv );
protected: // instance members
    CExplorer*     m_pExplorer;
    QSettings*     m_pSettingsFile;
    QString        m_strSettingsKey;
    QVBoxLayout*   m_pLytMain;
    CWdgtExplorer* m_pWdgtExplorer;

}; // class CDlgExplorer

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPExplorerDlg_h
