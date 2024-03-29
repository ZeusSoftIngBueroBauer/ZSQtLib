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

#ifndef ZSApps_LogClient_WidgetCentral_h
#define ZSApps_LogClient_WidgetCentral_h

#include "ZSSys/ZSSysDllMain.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QVBoxLayout;

namespace ZS
{
namespace Log
{
class CIpcLogClient;

namespace GUI
{
class CWdgtLog;
}
}
namespace Apps
{
namespace Products
{
namespace LogClient
{
//******************************************************************************
class CWidgetCentral : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWidgetCentral(
        ZS::Log::CIpcLogClient* i_pLogClient,
        QWidget*                i_pWdgtParent = nullptr,
        Qt::WindowFlags         i_wflags = Qt::WindowFlags());
    ~CWidgetCentral();
signals:
    void progressBarConnectDblClicked();
public: // instance methods
    ZS::Log::GUI::CWdgtLog* getLogWidget() { return m_pWdgtLog; }
protected slots:
    void onProgressBarConnectDblClicked();
protected: // instance members
    QVBoxLayout* m_pLyt;
    ZS::Log::GUI::CWdgtLog* m_pWdgtLog;

}; // class CWidgetCentral

} // namespace LogClient

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_LogClient_WidgetCentral_h
