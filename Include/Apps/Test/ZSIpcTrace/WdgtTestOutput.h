/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_TestIpcTrace_WdgtTestOutput_h
#define ZSApps_TestIpcTrace_WdgtTestOutput_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSSys/ZSSysErrResult.h"

class QPushButton;
class QTimer;
class QVBoxLayout;

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace IpcTrace
{
class CTest;

//******************************************************************************
class CWdgtIdxTreeTestStepsOutput : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtIdxTreeTestStepsOutput( CTest* i_pTest, QWidget* i_pWdgtParent = nullptr, Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    ~CWdgtIdxTreeTestStepsOutput();
protected: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv ) override;
protected: // overridables of base class QWidget
    virtual void paintEvent( QPaintEvent* i_pEv ) override;
protected slots:
    void onTimerWdgtUpdateTimeout();
protected: // instance members
    CTest*  m_pTest;
    QTimer* m_pTimerWdgtUpdate;
    // Test module 1
    QRect   m_rectTestModule1;
    QRect   m_rectTitleTestModule1;
    // Test module 2 thread
    QRect   m_rectTestModule2Thread;
    QRect   m_rectTitleTestModule2Thread;
    // Test module 2 thread run method
    QRect   m_rectTestModule2ThreadRun;
    QRect   m_rectTitleTestModule2ThreadRun;
    // Test module 2
    QRect   m_rectTitleTestModule2;
    QRect   m_rectTestModule2;

}; // class CWgdtTestOutput

} // namespace IpcTrace

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTrace_WdgtTestOutput_h
