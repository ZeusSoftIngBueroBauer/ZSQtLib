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

$Date: 10.01.10 19:16 $

$Revision: 4 $

$Author: Christian Bauer $

$History: ZSWidgetCentral.h $
 * 
 * *****************  Version 4  *****************
 * User: Christian Bauer Date: 10.01.10   Time: 19:16
 * Updated in $/ZSAppSNMPExplorer/Include

*******************************************************************************/

#ifndef ZSWidgetCentral_h
#define ZSWidgetCentral_h

#include <QtGui/qwidget.h>

class QSettings;
class QSplitter;
class QTabWidget;
class QTimer;
class QVBoxLayout;

namespace ZS
{
class CWdgtErrLog;

namespace Trace
{
class CWdgtServer;
}

namespace SNMP
{
class CExplorer;
class CWdgtExplorer;
class CCltStdPC;
class CWdgtCltStdPC;

//******************************************************************************
class CWidgetCentral : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWidgetCentral(
        CExplorer*     i_pExplorer,
        CCltStdPC*     i_pCltStdPC,
        QSettings*     i_pSettingsFile,
        const QString& i_strSettingsKey = "",
        QWidget*       i_pWdgtParent = NULL,
        Qt::WFlags     i_wflags = 0 );
    ~CWidgetCentral();
protected: // instance members
    QSettings*          m_pSettingsFile;
    QString             m_strSettingsKey;
    QVBoxLayout*        m_pLytMain;
    QSplitter*          m_pSplitter;
    QTabWidget*         m_pTabWdgt;
    CWdgtExplorer*      m_pWdgtExplorer;
    CWdgtCltStdPC*      m_pWdgtCltStdPC;
    Trace::CWdgtServer* m_pWdgtTrcServer;
    CWdgtErrLog*        m_pWdgtErrLog;

}; // class CWidgetCentral

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSWidgetCentral_h
