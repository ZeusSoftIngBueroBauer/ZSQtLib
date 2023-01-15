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

#ifndef ZSApps_TestDiagram_WidgetCentral_h
#define ZSApps_TestDiagram_WidgetCentral_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class QGroupBox;
class QSplitter;
class QVBoxLayout;

namespace ZS
{
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
namespace Diagram
{
class CWdgtTestOutput;

//******************************************************************************
class CWidgetCentral : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static CWidgetCentral* GetInstance(); // singleton class
public: // ctors and dtor
    CWidgetCentral( QWidget* i_pWdgtParent = nullptr, Qt::WindowFlags i_wflags = Qt::WindowFlags());
    ~CWidgetCentral();
public: // instance methods
    CWdgtTestOutput* getTestOutputWidget() { return m_pWdgtTestOutput; }
private: // class members
    static CWidgetCentral*   s_pThis; // singleton class
protected: // instance members
    QVBoxLayout*                          m_pLyt;
    QSplitter*                            m_pSplitter;
    ZS::Test::GUI::CWdgtIdxTreeTestSteps* m_pWdgtTest;
    QGroupBox*                            m_pGrpTestOutput;
    QVBoxLayout*                          m_pLytGrpTestOutput;
    CWdgtTestOutput*                      m_pWdgtTestOutput;

}; // class CWidgetCentral

} // namespace Diagram

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestDiagram_WidgetCentral_h
