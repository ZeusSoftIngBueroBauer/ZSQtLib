/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestSysErrLog_WidgetCentral_h
#define ZSApps_TestSysErrLog_WidgetCentral_h

#include "ZSSys/ZSSysDllMain.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QGroupBox;
class QSplitter;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
namespace GUI
{
class CWdgtErrLog;
}
}

namespace Test
{
namespace GUI
{
class CWdgtIdxTreeTestSteps;
}
}

namespace Apps
{
namespace Test
{
namespace SysErrLog
{
//******************************************************************************
class CWidgetCentral : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static CWidgetCentral* GetInstance() { return s_pThis; } // singleton class
public: // ctors and dtor
    CWidgetCentral( QWidget* i_pWdgtParent = nullptr, Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    ~CWidgetCentral();
protected: // class members
    static CWidgetCentral* s_pThis; // singleton class
protected: // instance members
    QString                       m_strSettingsKey;
    QVBoxLayout*                  m_pLyt;
    QSplitter*                    m_pSplitter;
    QVBoxLayout*                  m_pLytGrpTest;
    QGroupBox*                    m_pGrpTest;
    ZS::Test::GUI::CWdgtIdxTreeTestSteps*     m_pWdgtTest;
    QVBoxLayout*                  m_pLytGrpErrLog;
    QGroupBox*                    m_pGrpErrLog;
    ZS::System::GUI::CWdgtErrLog* m_pWdgtErrLog;

}; // class CWidgetCentral

} // namespace SysErrLog

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestSysErrLog_WidgetCentral_h
