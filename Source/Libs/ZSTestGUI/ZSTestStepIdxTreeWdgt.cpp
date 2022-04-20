/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSTestGUI/ZSTestStepIdxTreeWdgt.h"
#include "ZSTestGUI/ZSTestStepIdxTreeModel.h"
#include "ZSTestGUI/ZSTestStepIdxTreeView.h"
#include "ZSTest/ZSTestStep.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Test;
using namespace ZS::Test::GUI;


/*******************************************************************************
class CWdgtIdxTreeTestSteps : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtIdxTreeTestSteps::CWdgtIdxTreeTestSteps(
    CTest*          i_pTest,
    //bool            i_bUseContentWidget,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pTest(i_pTest),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pBtnStart(nullptr),
    m_pBtnStep(nullptr),
    m_pBtnPause(nullptr),
    m_pBtnStop(nullptr),
    m_pBtnTreeViewResizeRowsAndColumnsToContents(nullptr),
    m_pBtnTreeViewExpandAll(nullptr),
    m_pBtnTreeViewCollapseAll(nullptr),
    m_pLblTestStepIntervalInMs(nullptr),
    m_pEdtTestStepIntervalInMs(nullptr),
    m_pLblTestStepCurr(nullptr),
    m_pEdtTestStepCurr(nullptr),
    m_pTestStepsModel(nullptr),
    m_pTreeViewTestSteps(nullptr)
{
    setObjectName( "WdgtTest" + m_pTest->objectName() );

    m_pLytMain = new QVBoxLayout;
    setLayout(m_pLytMain);

    // <Headline> with start/stop/pause buttons
    //=========================================

    QSize szBtn(24, 24);

    m_pLytHeadLine = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytHeadLine);

    // <Button> Start
    //---------------

    QPixmap pxmStart(":/ZS/Button/ButtonCmdStart16x16.bmp");
    pxmStart.setMask(pxmStart.createHeuristicMask());

    m_pBtnStart = new QPushButton();
    m_pBtnStart->setIcon(pxmStart);
    m_pBtnStart->setFixedSize(szBtn);
    m_pLytHeadLine->addWidget(m_pBtnStart);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnStart,
        /* szSignal     */ SIGNAL( clicked(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onBtnStartClicked(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Step
    //---------------

    QPixmap pxmStep(":/ZS/Button/ButtonCmdStepForward16x16.bmp");
    pxmStep.setMask(pxmStep.createHeuristicMask());

    m_pBtnStep = new QPushButton();
    m_pBtnStep->setIcon(pxmStep);
    m_pBtnStep->setFixedSize(szBtn);
    m_pLytHeadLine->addWidget(m_pBtnStep);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnStep,
        /* szSignal     */ SIGNAL( clicked(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onBtnStepClicked(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Pause
    //---------------

    QPixmap pxmPause(":/ZS/Button/ButtonCmdPause16x16.bmp");
    pxmPause.setMask(pxmPause.createHeuristicMask());

    m_pBtnPause = new QPushButton();
    m_pBtnPause->setIcon(pxmPause);
    m_pBtnPause->setFixedSize(szBtn);
    m_pBtnPause->setEnabled(false);
    m_pLytHeadLine->addWidget(m_pBtnPause);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnPause,
        /* szSignal     */ SIGNAL( clicked(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onBtnPauseClicked(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Stop
    //---------------

    QPixmap pxmStop(":/ZS/Button/ButtonCmdStop16x16.bmp");
    pxmStop.setMask(pxmStop.createHeuristicMask());

    m_pBtnStop = new QPushButton();
    m_pBtnStop->setIcon(pxmStop);
    m_pBtnStop->setFixedSize(szBtn);
    m_pBtnStop->setEnabled(false);
    m_pLytHeadLine->addWidget(m_pBtnStop);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnStop,
        /* szSignal     */ SIGNAL( clicked(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onBtnStopClicked(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(30);

    // <Button> Resize Columns To Contents
    //------------------------------------

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.bmp");
    pxmResizeToContents.setMask(pxmResizeToContents.createHeuristicMask());

    m_pBtnTreeViewResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setFixedSize(szBtn);
    m_pLytHeadLine->addWidget(m_pBtnTreeViewResizeRowsAndColumnsToContents);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnTreeViewResizeRowsAndColumnsToContents,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTreeViewResizeRowsAndColumnsToContentsClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Expand All
    //--------------------

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.bmp");
    pxmExpandAll.setMask(pxmExpandAll.createHeuristicMask());

    m_pBtnTreeViewExpandAll = new QPushButton();
    m_pBtnTreeViewExpandAll->setIcon(pxmExpandAll);
    m_pBtnTreeViewExpandAll->setFixedSize(szBtn);
    m_pLytHeadLine->addWidget(m_pBtnTreeViewExpandAll);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnTreeViewExpandAll,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTreeViewExpandAllClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Collapse All
    //----------------------

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.bmp");
    pxmCollapseAll.setMask(pxmCollapseAll.createHeuristicMask());

    m_pBtnTreeViewCollapseAll = new QPushButton();
    m_pBtnTreeViewCollapseAll->setIcon(pxmCollapseAll);
    m_pBtnTreeViewCollapseAll->setFixedSize(szBtn);
    m_pLytHeadLine->addWidget(m_pBtnTreeViewCollapseAll);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnTreeViewCollapseAll,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTreeViewCollapseAllClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Label> Test step interval
    //---------------------------

    m_pLblTestStepIntervalInMs = new QLabel("Test Step Interval/ms:");
    m_pEdtTestStepIntervalInMs = new QLineEdit( QString::number(m_pTest->getTestStepIntervalInMs()) );
    m_pEdtTestStepIntervalInMs->setValidator( new QIntValidator(0, 60 * 1000, m_pEdtTestStepIntervalInMs) );
    m_pEdtTestStepIntervalInMs->setMaximumWidth(60);
    m_pLytHeadLine->addWidget(m_pLblTestStepIntervalInMs);
    m_pLytHeadLine->addWidget(m_pEdtTestStepIntervalInMs);

    m_pLytHeadLine->addSpacing(10);

    if( !QObject::connect(
        /* pObjSender   */ m_pEdtTestStepIntervalInMs,
        /* szSignal     */ SIGNAL( editingFinished() ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onEdtTestStepIntervalEditingFinished() ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <EditField> Current Test Step
    //------------------------------------

    m_pLblTestStepCurr = new QLabel("Current Test Step:");
    m_pEdtTestStepCurr = new QLineEdit("---",this);
    m_pEdtTestStepCurr->setEnabled(false);
    m_pLytHeadLine->addWidget(m_pLblTestStepCurr);
    m_pLytHeadLine->addWidget(m_pEdtTestStepCurr);

    // <Stretch> at EOL
    //-----------------

    //m_pLytHeadLine->addStretch();

    //// <Line> Test Steps Summary
    ////==========================

    //m_pLytLineTestStepsSummary = new QHBoxLayout();
    //m_pLytMain->addLayout(m_pLytLineTestStepsSummary);

    //// Number of tests
    ////-----------------------

    //m_pLblTestStepsCount = new QLabel("Number Of Tests:");
    //m_pEdtTestStepsCount = new QLineEdit("0");
    //m_pEdtTestStepsCount->setEnabled(false);
    //m_pEdtTestStepsCount->setMaximumWidth(60);
    //m_pLytLineTestStepsSummary->addWidget(m_pLblTestStepsCount);
    //m_pLytLineTestStepsSummary->addWidget(m_pEdtTestStepsCount);

    //m_pLytLineTestStepsSummary->addSpacing(10);

    //// Number of executed tests
    ////-------------------------

    //m_pLblTestStepsExecuted = new QLabel("Executed:");
    //m_pEdtTestStepsExecuted = new QLineEdit("0");
    //m_pEdtTestStepsExecuted->setEnabled(false);
    //m_pEdtTestStepsExecuted->setMaximumWidth(60);
    //m_pLytLineTestStepsSummary->addWidget(m_pLblTestStepsExecuted);
    //m_pLytLineTestStepsSummary->addWidget(m_pEdtTestStepsExecuted);

    //m_pLytLineTestStepsSummary->addSpacing(10);

    //// Number of passed tests
    ////-----------------------

    //m_pLblTestStepsPassed = new QLabel("Passed:");
    //m_pEdtTestStepsPassed = new QLineEdit("0");
    //m_pEdtTestStepsPassed->setEnabled(false);
    //m_pEdtTestStepsPassed->setMaximumWidth(60);
    //m_pEdtTestStepsPassed->setStyleSheet("QLineEdit { color: green; }");
    //m_pLytLineTestStepsSummary->addWidget(m_pLblTestStepsPassed);
    //m_pLytLineTestStepsSummary->addWidget(m_pEdtTestStepsPassed);

    //m_pLytLineTestStepsSummary->addSpacing(10);

    //// Number of failed tests
    ////-----------------------

    //m_pLblTestStepsFailed = new QLabel("Failed:");
    //m_pEdtTestStepsFailed = new QLineEdit("0",this);
    //m_pEdtTestStepsFailed->setEnabled(false);
    //m_pEdtTestStepsFailed->setMaximumWidth(60);
    //m_pEdtTestStepsFailed->setStyleSheet("QLineEdit { color: green; }");
    //m_pLytLineTestStepsSummary->addWidget(m_pLblTestStepsFailed);
    //m_pLytLineTestStepsSummary->addWidget(m_pEdtTestStepsFailed);

    //m_pLytLineTestStepsSummary->addSpacing(10);

    //// Number of tests with ignored result
    ////------------------------------------

    //m_pLblTestStepsIgnored = new QLabel("Ignored:");
    //m_pEdtTestStepsIgnored = new QLineEdit("0",this);
    //m_pEdtTestStepsIgnored->setEnabled(false);
    //m_pEdtTestStepsIgnored->setMaximumWidth(60);
    //m_pLytLineTestStepsSummary->addWidget(m_pLblTestStepsIgnored);
    //m_pLytLineTestStepsSummary->addWidget(m_pEdtTestStepsIgnored);

    //// <Stretch> at EOL
    ////-----------------

    //m_pLytLineTestStepsSummary->addStretch();

    // <TreeView> Test Steps
    //======================

    m_pTestStepsModel = new CModeldxTreeTestSteps( m_pTest->getTestStepIdxTree() );

    m_pTreeViewTestSteps = new CTreeViewIdxTreeTestSteps(m_pTestStepsModel,this);
    m_pLytMain->addWidget(m_pTreeViewTestSteps);

    // Connect to state changed signal of test
    //----------------------------------------

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL( stateChanged(const ZS::Test::CEnumTestState&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTestStateChanged(const ZS::Test::CEnumTestState&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL( runModeChanged(const ZS::System::CEnumRunMode&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTestRunModeChanged(const ZS::System::CEnumRunMode&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL( currentTestStepChanged(ZS::Test::CTestStep*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onCurrentTestStepChanged(ZS::Test::CTestStep*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL( testStepIntervalChanged(int) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTestStepIntervalChanged(int) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Connect to the signals of the tree view
    //----------------------------------------

    if( m_pTreeViewTestSteps != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ m_pTreeViewTestSteps,
            /* szSignal     */ SIGNAL( expanded(const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewExpanded(const QModelIndex&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }

    // <TreeView> Resize
    //------------------

    if( m_pTreeViewTestSteps != nullptr )
    {
        m_pTreeViewTestSteps->resizeColumnToContents(CModeldxTreeTestSteps::EColumnName);
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtIdxTreeTestSteps::~CWdgtIdxTreeTestSteps()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pTestStepsModel;
    }
    catch(...)
    {
    }

    m_pTest = nullptr;
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pBtnStart = nullptr;
    m_pBtnStep = nullptr;
    m_pBtnPause = nullptr;
    m_pBtnStop = nullptr;
    m_pBtnTreeViewResizeRowsAndColumnsToContents = nullptr;
    m_pBtnTreeViewExpandAll = nullptr;
    m_pBtnTreeViewCollapseAll = nullptr;
    m_pLblTestStepIntervalInMs = nullptr;
    m_pEdtTestStepIntervalInMs = nullptr;
    m_pLblTestStepCurr = nullptr;
    m_pEdtTestStepCurr = nullptr;
    m_pTestStepsModel = nullptr;
    m_pTreeViewTestSteps = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onBtnStartClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pTest->start();
}

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onBtnStepClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pTest->step();
}

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onBtnPauseClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pTest->pause();
}

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onBtnStopClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pTest->stop();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeViewTestSteps != nullptr )
    {
        int idxClm;

        for( idxClm = 0; idxClm < CModeldxTreeTestSteps::EColumnCount; idxClm++ )
        {
            m_pTreeViewTestSteps->resizeColumnToContents(idxClm);
        }
    }

} // onBtnTreeViewResizeRowsAndColumnsToContentsClicked

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onBtnTreeViewExpandAllClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeViewTestSteps != nullptr )
    {
        m_pTreeViewTestSteps->expandAll();

        for( int idxClm = 0; idxClm < m_pTestStepsModel->columnCount(); idxClm++ )
        {
            m_pTreeViewTestSteps->resizeColumnToContents(idxClm);
        }
    }

} // onBtnTreeViewExpandAllClicked

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onBtnTreeViewCollapseAllClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeViewTestSteps != nullptr )
    {
        m_pTreeViewTestSteps->collapseAll();
    }

} // onBtnTreeViewCollapseAllClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onEdtTestStepIntervalEditingFinished()
//------------------------------------------------------------------------------
{
    m_pTest->setTestStepInterval( m_pEdtTestStepIntervalInMs->text().toInt() );
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onTestStateChanged( const ZS::Test::CEnumTestState& i_state )
//------------------------------------------------------------------------------
{
    if( i_state == ETestState::Idle )
    {
        m_pBtnStart->setEnabled(true);
        m_pBtnStep->setEnabled(true);
        m_pBtnPause->setEnabled(false);
        m_pBtnStop->setEnabled(false);
    }
    else if( i_state == ETestState::Running )
    {
        m_pBtnStart->setEnabled(false);
        m_pBtnStep->setEnabled(false);
        m_pBtnPause->setEnabled(true);
        m_pBtnStop->setEnabled(true);
    }
    else if( i_state == ETestState::Paused )
    {
        m_pBtnStart->setEnabled(true);
        m_pBtnStep->setEnabled(true);
        m_pBtnPause->setEnabled(false);
        m_pBtnStop->setEnabled(true);
    }

} // onTestStateChanged

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onTestRunModeChanged( const ZS::System::CEnumRunMode& /*i_runMode*/ )
//------------------------------------------------------------------------------
{
} // onTestRunModeChanged

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onCurrentTestStepChanged( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    if( i_pTestStep == nullptr )
    {
        m_pEdtTestStepCurr->setText("---");
    }
    else // if( i_pTestStep != nullptr )
    {
        m_pEdtTestStepCurr->setText( i_pTestStep->path() );
    }

} // onCurrentTestStepChanged

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onTestStepIntervalChanged( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    if( m_pEdtTestStepIntervalInMs->text() != QString::number(i_iInterval_ms) )
    {
        m_pEdtTestStepIntervalInMs->setText(QString::number(i_iInterval_ms));
    }
}

/*==============================================================================
public slots:
==============================================================================*/

////------------------------------------------------------------------------------
//void CWdgtIdxTreeTestSteps::onTestStepChanged( QObject* /*i_pAdminIdxTreeModel*/, QObject* /*i_pTestStep*/ )
////------------------------------------------------------------------------------
//{
//    m_pEdtTestStepsExecuted->setText( QString::number(m_pTest->getAdminObjIdxTree()->getTestStepsExecutedCount()) );
//    m_pEdtTestStepsPassed->setText( QString::number(m_pTest->getAdminObjIdxTree()->getTestStepsPassedCount()) );
//    m_pEdtTestStepsFailed->setText( QString::number(m_pTest->getAdminObjIdxTree()->getTestStepsFailedCount()) );
//    m_pEdtTestStepsIgnored->setText( QString::number(m_pTest->getAdminObjIdxTree()->getTestStepsIgnoredCount()) );
//
//} // onTestStepChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onTreeViewExpanded( const QModelIndex& i_iModelIdx )
//------------------------------------------------------------------------------
{
    if( i_iModelIdx.isValid() )
    {
        #if QT_VERSION >= 0x040000 && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        m_pTreeViewTestSteps->resizeColumnToContents(i_iModelIdx.column());
        #elif QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        // Cannot invoke resizeColumnToContents as this leads to an exception.
        #endif
    }

} // onTreeViewExpanded
