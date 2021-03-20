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

#ifndef ZSApps_PCResourceMonitor_WidgetCentral_h
#define ZSApps_PCResourceMonitor_WidgetCentral_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRequestQueue.h"

class QGroupBox;
class QLabel;
class QLineEdit;
class QModelIndex;
class QPushButton;
class QScrollArea;
class QSettings;
class QSplitter;
class QTabWidget;
class QTextEdit;
class QTreeView;
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

namespace Apps
{
namespace Products
{
namespace PCResourceMonitor
{
class CPCResourceMonitor;
class CWdgtPCSettings;
class CWdgtPCSystem;
class CWdgtPCProcessors;
class CWdgtPCMemory;
class CWdgtPCDisks;

//******************************************************************************
class CWidgetCentral : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWidgetCentral(
        CPCResourceMonitor* i_pPCResourceMonitor,
        QWidget*            i_pWdgtParent = nullptr );
    ~CWidgetCentral();
private: // class members
    static CWidgetCentral*    s_pThis; // singleton class
protected: // instance members
    CPCResourceMonitor*       m_pPCResourceMonitor;
    QVBoxLayout*              m_pLyt;
    QTabWidget*               m_pTabWdgt;
    CWdgtPCSettings*          m_pWdgtPCSettings;
    CWdgtPCSystem*            m_pWdgtPCSystem;
    CWdgtPCProcessors*        m_pWdgtPCProcessors;
    CWdgtPCMemory*            m_pWdgtPCMemory;
    CWdgtPCDisks*             m_pWdgtPCDisks;
    ZS::Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CWidgetCentral

} // namespace PCResourceMonitor

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_PCResourceMonitor_WidgetCentral_h
