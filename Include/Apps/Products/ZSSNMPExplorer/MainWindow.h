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

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#ifndef ZSMainWindow_h
#define ZSMainWindow_h

#include <QtGui/qmainwindow.h>

class QSettings;

namespace ZS
{
namespace SNMP
{
class CExplorer;
class CWidgetCentral;
class CCltStdPC;

//******************************************************************************
class CMainWindow : public QMainWindow
//******************************************************************************
{
public: // class methods
    static CMainWindow* instance(); // singleton class
public: // ctors and dtor
    CMainWindow(
        const QString& i_strWindowTitle,
        CExplorer*     i_pExplorer,
        CCltStdPC*     i_pCltStdPC,
        QSettings*     i_pSettingsFile,
        const QString& i_strSettingsKey = "",
        QWidget*       i_pWdgtParent = NULL,
        Qt::WFlags     i_wflags = 0 );
    ~CMainWindow();
private: // class members
    static CMainWindow* s_pThis; // singleton class
protected: // instance members
    QSettings*      m_pSettingsFile;
    QString         m_strSettingsKey;
    CWidgetCentral* m_pWdgtCentral;
};

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSMainWindow_h
