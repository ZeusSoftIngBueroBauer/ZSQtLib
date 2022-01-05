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

#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "WdgtTestOutput.h"
#include "App.h"
#include "Test.h"
#include "TestModule1.h"
#include "TestModule2.h"

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::IpcTrace;


/*******************************************************************************
class CWdgtIdxTreeTestStepsOutput : public QWidget
*******************************************************************************/

/*==============================================================================
private: // class members
==============================================================================*/

//static const QString c_strSendMessage            = "Send Message";
//static const QString c_strCreateModule1          = "Create Module 1";
//static const QString c_strDeleteModule1          = "Delete Module 1";
//static const QString c_strWaitingForConfirmation = "Waiting for Confirmation...";
//static const QString c_strCreateModule2          = "Create Module 2";
//static const QString c_strDeleteModule2          = "Delete Module 2";

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtIdxTreeTestStepsOutput::CWdgtIdxTreeTestStepsOutput( CTest* i_pTest, QWidget* i_pWdgtParent, Qt::WindowFlags  i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pTest(i_pTest),
    m_pTimerWdgtUpdate(nullptr),
    // Test module 1
    m_rectTestModule1(),
    m_rectTitleTestModule1(),
    // Test module 2
    m_rectTestModule2Thread(),
    m_rectTitleTestModule2Thread(),
    m_rectTestModule2ThreadRun(),
    m_rectTitleTestModule2ThreadRun(),
    m_rectTitleTestModule2(),
    m_rectTestModule2()
{
    int iTestModuleCount     = 2;
    int iMarginTop           = 20;
    int iMarginBottomMin     = 20;
    int iMarginLeft          = 40;
    int iMarginRightMin      = 40;
    int cxSpace              = 100;
    int cxContextWidth       = 150;
    int cyContextHeight      = 120;
    int iContextMarginTop    = 10;
    int iContextMarginBottom = 10;
    int iContextMarginLeft   = 10;
    int iContextMarginRight  = 10;
    int cyTitleHeight        = 20;

    int cxMinimumWidth  = iMarginLeft + iMarginRightMin + iTestModuleCount*cxContextWidth + (iTestModuleCount-1)*cxSpace;
    int cyMinimumHeight = iMarginTop + iMarginBottomMin + cyContextHeight;

    setObjectName("WdgtTest");

    // All test modules
    //-----------------

    m_pTimerWdgtUpdate = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTimerWdgtUpdate,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTimerWdgtUpdateTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    m_pTimerWdgtUpdate->start(1000);

    // Test module 1
    //--------------

    m_rectTestModule1.setLeft(iMarginLeft);
    m_rectTestModule1.setTop(iMarginTop);
    m_rectTestModule1.setWidth(cxContextWidth);
    m_rectTestModule1.setHeight(cyContextHeight);

    // Title
    m_rectTitleTestModule1.setLeft(m_rectTestModule1.left());
    m_rectTitleTestModule1.setTop( m_rectTestModule1.top() + iContextMarginTop );
    m_rectTitleTestModule1.setWidth(m_rectTestModule1.width());
    m_rectTitleTestModule1.setHeight(cyTitleHeight);

    // Test module 2 thread (thread created by module 1)
    //--------------------------------------------------

    // Thread instance
    m_rectTestModule2Thread.setLeft( m_rectTestModule1.left() + iContextMarginLeft );
    m_rectTestModule2Thread.setTop( m_rectTestModule1.top() + 2*cyTitleHeight + iContextMarginTop );
    m_rectTestModule2Thread.setWidth( cxContextWidth - iContextMarginLeft - iContextMarginRight );
    m_rectTestModule2Thread.setHeight( cyContextHeight - 2*cyTitleHeight - iContextMarginTop - iContextMarginBottom );

    // Title
    m_rectTitleTestModule2Thread.setLeft(m_rectTestModule2Thread.left());
    m_rectTitleTestModule2Thread.setTop( m_rectTestModule2Thread.top() + iContextMarginTop );
    m_rectTitleTestModule2Thread.setWidth(m_rectTestModule2Thread.width());
    m_rectTitleTestModule2Thread.setHeight(cyTitleHeight);

    // Test module 2 thread context (run method)
    //------------------------------------------

    // Thread run ellipse
    m_rectTestModule2ThreadRun.setLeft( m_rectTestModule1.right() + cxSpace );
    m_rectTestModule2ThreadRun.setTop( m_rectTestModule1.top() );
    m_rectTestModule2ThreadRun.setWidth(cxContextWidth);
    m_rectTestModule2ThreadRun.setHeight(cyContextHeight);

    // Title
    m_rectTitleTestModule2ThreadRun.setLeft(m_rectTestModule2ThreadRun.left());
    m_rectTitleTestModule2ThreadRun.setTop( m_rectTestModule2ThreadRun.top() + iContextMarginTop );
    m_rectTitleTestModule2ThreadRun.setWidth(m_rectTestModule2ThreadRun.width());
    m_rectTitleTestModule2ThreadRun.setHeight(cyTitleHeight);

    // Test module 2 (created in run method of thread)
    //------------------------------------------------

    m_rectTestModule2.setLeft( m_rectTestModule2ThreadRun.left() + iContextMarginLeft );
    m_rectTestModule2.setTop( m_rectTestModule2ThreadRun.top() + 2*cyTitleHeight + iContextMarginTop );
    m_rectTestModule2.setWidth( cxContextWidth - iContextMarginLeft - iContextMarginRight );
    m_rectTestModule2.setHeight( cyContextHeight - 2*cyTitleHeight - iContextMarginTop - iContextMarginBottom );

    // Title
    m_rectTitleTestModule2.setLeft(m_rectTestModule2.left());
    m_rectTitleTestModule2.setTop( m_rectTestModule2.top() + iContextMarginTop );
    m_rectTitleTestModule2.setWidth(m_rectTestModule2.width());
    m_rectTitleTestModule2.setHeight(cyTitleHeight);

    // Complete test widget
    //---------------------

    setMinimumWidth(cxMinimumWidth);
    setMinimumHeight(cyMinimumHeight);

} // ctor

//------------------------------------------------------------------------------
CWdgtIdxTreeTestStepsOutput::~CWdgtIdxTreeTestStepsOutput()
//------------------------------------------------------------------------------
{
    m_pTest = nullptr;
    m_pTimerWdgtUpdate = nullptr;

} // dtor

/*==============================================================================
protected: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtIdxTreeTestStepsOutput::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( !bEventHandled )
    {
        bEventHandled = QWidget::event(i_pEv);
    }
    return bEventHandled;

} // event

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestStepsOutput::paintEvent( QPaintEvent* /*i_pEv*/ )
//------------------------------------------------------------------------------
{
    QPainter painter(this);
    QPen     pen;

    CTestModule1*       pTestModule1 = m_pTest->getTestModule1();
    CTestModule2Thread* pTestModule2Thread = nullptr;
    CTestModule2*       pTestModule2 = m_pTest->getTestModule2();

    if( pTestModule1 != nullptr )
    {
        pTestModule2Thread = pTestModule1->getTestModule2Thread();
    }

    // Test module 1
    //---------------

    if( pTestModule1 == nullptr )
    {
        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DotLine);
    }
    else
    {
        pen.setColor(Qt::black);
        pen.setStyle(Qt::SolidLine);
    }

    painter.setPen(pen);
    painter.drawRect(m_rectTestModule1);
    painter.drawText( m_rectTitleTestModule1, Qt::AlignHCenter|Qt::AlignVCenter, "Module 1" );

    // Test module 2 thread
    //---------------------

    if( pTestModule2Thread == nullptr )
    {
        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DotLine);
    }
    else
    {
        pen.setColor(Qt::black);
        pen.setStyle(Qt::SolidLine);
    }

    painter.setPen(pen);
    painter.drawRect(m_rectTestModule2Thread);
    painter.drawText(m_rectTitleTestModule2Thread, Qt::AlignHCenter|Qt::AlignVCenter,"Module 2 Thread");

    // Test module 2 thread run method
    //--------------------------------

    if( pTestModule2Thread == nullptr )
    {
        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DotLine);
    }
    else if( !pTestModule2Thread->isRunning() )
    {
        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DotLine);
    }
    else
    {
        pen.setColor(Qt::black);
        pen.setStyle(Qt::SolidLine);
    }

    painter.setPen(pen);
    painter.drawEllipse(m_rectTestModule2ThreadRun);
    painter.drawText(m_rectTitleTestModule2ThreadRun, Qt::AlignHCenter|Qt::AlignVCenter,"Module 2 Thread run");

    // Test module 2
    //---------------

    if( pTestModule2 == nullptr )
    {
        pen.setColor(Qt::gray);
        pen.setStyle(Qt::DotLine);
    }
    else
    {
        pen.setColor(Qt::black);
        pen.setStyle(Qt::SolidLine);
    }

    painter.setPen(pen);
    painter.drawRect(m_rectTestModule2);
    painter.drawText(m_rectTitleTestModule2, Qt::AlignHCenter|Qt::AlignVCenter,"Module 2");

} // paintEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestStepsOutput::onTimerWdgtUpdateTimeout()
//------------------------------------------------------------------------------
{
    update();
}
