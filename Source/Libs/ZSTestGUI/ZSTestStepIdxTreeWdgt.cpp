/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSTestGUI/ZSTestStepIdxTreeWdgt.h"
#include "ZSTestGUI/ZSTestStepIdxTreeModel.h"
#include "ZSTestGUI/ZSTestStepIdxTreeView.h"
#include "ZSTestGUI/ZSTestStepWdgt.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSTest/ZSTestStep.h"

#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#else
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test;
using namespace ZS::Test::GUI;


/*******************************************************************************
class CWdgtIdxTreeTestSteps : public QWidget
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* enum class EViewMode
==============================================================================*/

static const SEnumEntry s_arEnumStrWdgtIdxTreeViewModes[] = {                                      // IdxName,                  Symbol
    /* 0 */ SEnumEntry( static_cast<int>(CWdgtIdxTreeTestSteps::EViewMode::NavPanelOnly),            "NavPanelOnly",            "NP"   ),
    /* 1 */ SEnumEntry( static_cast<int>(CWdgtIdxTreeTestSteps::EViewMode::NavPanelAndLeaveContent), "NavPanelAndLeaveContent", "NPLC" )
};

//------------------------------------------------------------------------------
QString CWdgtIdxTreeTestSteps::viewMode2Str( EViewMode i_eVal, int i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrWdgtIdxTreeViewModes, _ZSArrLen(s_arEnumStrWdgtIdxTreeViewModes), static_cast<int>(i_eVal), i_alias);
}

//------------------------------------------------------------------------------
QPixmap CWdgtIdxTreeTestSteps::viewMode2Pixmap( EViewMode i_eVal, const QSize& i_sz )
//------------------------------------------------------------------------------
{
    QString str = viewMode2Str(i_eVal);
    QPixmap pxm = QPixmap( ":/ZS/TreeView/TreeViewViewMode" + str + ".png" );
    pxm = pxm.scaled(i_sz);
    return pxm;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtIdxTreeTestSteps::CWdgtIdxTreeTestSteps(
    CTest*          i_pTest,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_strSettingsKey("WdgtIdxTreeTestSteps"),
    m_pTest(i_pTest),
    m_viewMode(EViewMode::NavPanelOnly),
    m_szBtns(24, 24),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pBtnStart(nullptr),
    m_pBtnStep(nullptr),
    m_pBtnPause(nullptr),
    m_pBtnStop(nullptr),
    m_pBtnViewMode(nullptr),
    m_pBtnTreeViewResizeRowsAndColumnsToContents(nullptr),
    m_pBtnTreeViewExpandAll(nullptr),
    m_pBtnTreeViewCollapseAll(nullptr),
    m_pLblTestStepIntervalInMs(nullptr),
    m_pEdtTestStepIntervalInMs(nullptr),
    m_pLblTestStepCurr(nullptr),
    m_pEdtTestStepCurr(nullptr),
    m_pTestStepsModel(nullptr),
    m_pSplitter(nullptr),
    m_pTreeViewTestSteps(nullptr),
    m_pGrpWdgtTestStep(nullptr),
    m_pLytGrpWdgtTestStep(nullptr),
    m_pWdgtTestStep(nullptr)
{
    setObjectName( "WdgtTest" + m_pTest->objectName() );

    m_pLytMain = new QVBoxLayout;
    setLayout(m_pLytMain);

    // <Headline> with start/stop/pause buttons
    //=========================================

    m_pLytHeadLine = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytHeadLine);

    // <Button> Start
    //---------------

    QPixmap pxmStart(":/ZS/Button/ButtonCmdStart16x16.png");
    m_pBtnStart = new QPushButton();
    m_pBtnStart->setIcon(pxmStart);
    m_pBtnStart->setFixedSize(m_szBtns);
    m_pLytHeadLine->addWidget(m_pBtnStart);

    QObject::connect(
        m_pBtnStart, &QPushButton::clicked,
        this, &CWdgtIdxTreeTestSteps::onBtnStartClicked);

    m_pLytHeadLine->addSpacing(10);

    // <Button> Step
    //---------------

    QPixmap pxmStep(":/ZS/Button/ButtonCmdStepForward16x16.png");
    m_pBtnStep = new QPushButton();
    m_pBtnStep->setIcon(pxmStep);
    m_pBtnStep->setFixedSize(m_szBtns);
    m_pLytHeadLine->addWidget(m_pBtnStep);

    QObject::connect(
        m_pBtnStep, &QPushButton::clicked,
        this, &CWdgtIdxTreeTestSteps::onBtnStepClicked);

    m_pLytHeadLine->addSpacing(10);

    // <Button> Pause
    //---------------

    QPixmap pxmPause(":/ZS/Button/ButtonCmdPause16x16.png");
    m_pBtnPause = new QPushButton();
    m_pBtnPause->setIcon(pxmPause);
    m_pBtnPause->setFixedSize(m_szBtns);
    m_pBtnPause->setEnabled(false);
    m_pLytHeadLine->addWidget(m_pBtnPause);

    QObject::connect(
        m_pBtnPause, &QPushButton::clicked,
        this, &CWdgtIdxTreeTestSteps::onBtnPauseClicked);

    m_pLytHeadLine->addSpacing(10);

    // <Button> Stop
    //---------------

    QPixmap pxmStop(":/ZS/Button/ButtonCmdStop16x16.png");
    m_pBtnStop = new QPushButton();
    m_pBtnStop->setIcon(pxmStop);
    m_pBtnStop->setFixedSize(m_szBtns);
    m_pBtnStop->setEnabled(false);
    m_pLytHeadLine->addWidget(m_pBtnStop);

    QObject::connect(
        m_pBtnStop, &QPushButton::clicked,
        this, &CWdgtIdxTreeTestSteps::onBtnStopClicked);

    m_pLytHeadLine->addSpacing(30);

    // <Button> View Mode
    //-------------------

    QPixmap pxmViewMode = viewMode2Pixmap(m_viewMode, m_szBtns);

    m_pBtnViewMode = new QPushButton();
    m_pBtnViewMode->setFixedSize(m_szBtns);
    m_pBtnViewMode->setIcon(pxmViewMode);
    m_pBtnViewMode->setToolTip("Press to toggle view mode between NavPanelOnly and NavPanelAndLeaveContent");
    m_pLytHeadLine->addWidget(m_pBtnViewMode);
    m_pLytHeadLine->addSpacing(10);

    QObject::connect(
        m_pBtnViewMode, &QPushButton::clicked,
        this, &CWdgtIdxTreeTestSteps::onBtnViewModeClicked);

    // <Button> Resize Columns To Contents
    //------------------------------------

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");

    m_pBtnTreeViewResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setFixedSize(m_szBtns);
    m_pLytHeadLine->addWidget(m_pBtnTreeViewResizeRowsAndColumnsToContents);

    QObject::connect(
        m_pBtnTreeViewResizeRowsAndColumnsToContents, &QPushButton::clicked,
        this, &CWdgtIdxTreeTestSteps::onBtnTreeViewResizeRowsAndColumnsToContentsClicked);

    m_pLytHeadLine->addSpacing(10);

    // <Button> Expand All
    //--------------------

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.png");

    m_pBtnTreeViewExpandAll = new QPushButton();
    m_pBtnTreeViewExpandAll->setIcon(pxmExpandAll);
    m_pBtnTreeViewExpandAll->setFixedSize(m_szBtns);
    m_pLytHeadLine->addWidget(m_pBtnTreeViewExpandAll);

    QObject::connect(
        m_pBtnTreeViewExpandAll, &QPushButton::clicked,
        this, &CWdgtIdxTreeTestSteps::onBtnTreeViewExpandAllClicked);

    m_pLytHeadLine->addSpacing(10);

    // <Button> Collapse All
    //----------------------

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.png");

    m_pBtnTreeViewCollapseAll = new QPushButton();
    m_pBtnTreeViewCollapseAll->setIcon(pxmCollapseAll);
    m_pBtnTreeViewCollapseAll->setFixedSize(m_szBtns);
    m_pLytHeadLine->addWidget(m_pBtnTreeViewCollapseAll);

    QObject::connect(
        m_pBtnTreeViewCollapseAll, &QPushButton::clicked,
        this, &CWdgtIdxTreeTestSteps::onBtnTreeViewCollapseAllClicked);

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

    QObject::connect(
        m_pEdtTestStepIntervalInMs, &QLineEdit::editingFinished,
        this, &CWdgtIdxTreeTestSteps::onEdtTestStepIntervalEditingFinished);

    // <EditField> Current Test Step
    //------------------------------------

    m_pLblTestStepCurr = new QLabel("Current Test Step:");
    m_pLblTestStepCurr->setStyleSheet("font-weight: bold");
    m_pEdtTestStepCurr = new QLineEdit("---",this);
    m_pEdtTestStepCurr->setStyleSheet("font-weight: bold");
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

    m_pLytMain->addSpacing(5);

    m_pTestStepsModel = new CModeldxTreeTestSteps( m_pTest->getTestStepIdxTree() );

    m_pTreeViewTestSteps = new CTreeViewIdxTreeTestSteps(m_pTestStepsModel, this);
    m_pLytMain->addWidget(m_pTreeViewTestSteps);

    QObject::connect(
        m_pTreeViewTestSteps->selectionModel(), &QItemSelectionModel::currentRowChanged,
        this, &CWdgtIdxTreeTestSteps::onTreeViewTestStepsSelectionModelCurrentRowChanged);
    if( m_pTreeViewTestSteps != nullptr )
    {
        QObject::connect(
            m_pTreeViewTestSteps, &QTreeView::expanded,
            this, &CWdgtIdxTreeTestSteps::onTreeViewTestStepsExpanded);
    }

    // Connect to state changed signal of test
    //----------------------------------------

    QObject::connect(
        m_pTest, &CTest::stateChanged,
        this, &CWdgtIdxTreeTestSteps::onTestStateChanged);
    QObject::connect(
        m_pTest, &CTest::runModeChanged,
        this, &CWdgtIdxTreeTestSteps::onTestRunModeChanged);
    QObject::connect(
        m_pTest, &CTest::currentTestStepChanged,
        this, &CWdgtIdxTreeTestSteps::onCurrentTestStepChanged);
    QObject::connect(
        m_pTest, &CTest::testStepIntervalChanged,
        this, &CWdgtIdxTreeTestSteps::onTestStepIntervalChanged);

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

    //m_strSettingsKey;
    m_pTest = nullptr;
    m_viewMode = static_cast<EViewMode>(0);
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pBtnStart = nullptr;
    m_pBtnStep = nullptr;
    m_pBtnPause = nullptr;
    m_pBtnStop = nullptr;
    m_pBtnViewMode = nullptr;
    m_pBtnTreeViewResizeRowsAndColumnsToContents = nullptr;
    m_pBtnTreeViewExpandAll = nullptr;
    m_pBtnTreeViewCollapseAll = nullptr;
    m_pLblTestStepIntervalInMs = nullptr;
    m_pEdtTestStepIntervalInMs = nullptr;
    m_pLblTestStepCurr = nullptr;
    m_pEdtTestStepCurr = nullptr;
    m_pTestStepsModel = nullptr;
    m_pSplitter = nullptr;
    m_pTreeViewTestSteps = nullptr;
    m_pGrpWdgtTestStep = nullptr;
    m_pLytGrpWdgtTestStep = nullptr;
    m_pWdgtTestStep = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::setViewMode( EViewMode i_viewMode )
//------------------------------------------------------------------------------
{
    if( i_viewMode != m_viewMode )
    {
        m_viewMode = i_viewMode;

        if( m_viewMode == EViewMode::NavPanelOnly )
        {
            QSettings settings;
            settings.setValue( m_strSettingsKey+"/Geometry", saveGeometry() );
            if( m_pSplitter != nullptr ) {
                QList<int> listSizes = m_pSplitter->sizes();
                for( int idx = 0; idx < listSizes.count(); idx++ ) {
                    settings.setValue(m_strSettingsKey + "/Geometry/Splitter/Wdgt" + QString::number(idx) + "Height", listSizes[idx]);
                }
            }

            m_pTreeViewTestSteps->setColumnHidden(
                CModeldxTreeTestSteps::EColumnExpectedValues, false);
            m_pTreeViewTestSteps->setColumnHidden(
                CModeldxTreeTestSteps::EColumnResultValues, false);

            if( m_pSplitter != nullptr )
            {
                // Reparent tree view. This will remove the tree view
                // from the splitter but does not destroy the tree view.
                m_pLytMain->addWidget(m_pTreeViewTestSteps);

                delete m_pSplitter;
                m_pSplitter = nullptr;
                m_pGrpWdgtTestStep = nullptr;
                m_pLytGrpWdgtTestStep = nullptr;
                m_pWdgtTestStep = nullptr;
            }
        } // if( m_viewMode == EViewMode::NavPanelOnly )

        else if( m_viewMode == EViewMode::NavPanelAndLeaveContent )
        {
            m_pTreeViewTestSteps->setColumnHidden(
                CModeldxTreeTestSteps::EColumnExpectedValues, true);
            m_pTreeViewTestSteps->setColumnHidden(
                CModeldxTreeTestSteps::EColumnResultValues, true);

            if( m_pSplitter == nullptr )
            {
                m_pSplitter = new QSplitter(Qt::Horizontal);
                m_pLytMain->addWidget(m_pSplitter, 1);

                m_pSplitter->addWidget(m_pTreeViewTestSteps);

                if( m_pGrpWdgtTestStep == nullptr )
                {
                    m_pGrpWdgtTestStep = new QGroupBox("Selected Test Step Details");
                    m_pLytGrpWdgtTestStep = new QVBoxLayout();
                    m_pGrpWdgtTestStep->setLayout(m_pLytGrpWdgtTestStep);
                    m_pWdgtTestStep = new CWdgtTestStep();
                    m_pLytGrpWdgtTestStep->addWidget(m_pWdgtTestStep);

                    CTestStep* pTestStep = getSelectedTestStep();
                    m_pWdgtTestStep->setTestStep(pTestStep);
                }
                m_pSplitter->addWidget(m_pGrpWdgtTestStep);
            }

            QSettings settings;
            restoreGeometry( settings.value(m_strSettingsKey+"/Geometry").toByteArray() );
            if( m_pSplitter != nullptr ) {
                QList<int> listSizes = m_pSplitter->sizes();
                for( int idx = 0; idx < listSizes.count(); idx++ ) {
                    listSizes[idx] = settings.value(
                        m_strSettingsKey + "/Geometry/Splitter/Wdgt" + QString::number(idx) + "Height", 50 ).toInt();
                }
                m_pSplitter->setSizes(listSizes);
            }
        } // if( m_viewMode == EViewMode::NavPanelAndLeaveContent )

        QPixmap pxmViewMode = viewMode2Pixmap(m_viewMode, m_szBtns);

        m_pBtnViewMode->setIcon(pxmViewMode);

    } // if( i_viewMode != m_viewMode )

} // setViewMode

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
void CWdgtIdxTreeTestSteps::onBtnViewModeClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    EViewMode viewModeNew = m_viewMode;

    if( m_viewMode == EViewMode::NavPanelOnly )
    {
        viewModeNew = EViewMode::NavPanelAndLeaveContent;
    }
    else
    {
        viewModeNew = EViewMode::NavPanelOnly;
    }
    setViewMode(viewModeNew);
}

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
}

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
}

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onBtnTreeViewCollapseAllClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeViewTestSteps != nullptr )
    {
        m_pTreeViewTestSteps->collapseAll();
    }
}

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
}

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onTestRunModeChanged( const ZS::System::CEnumRunMode& /*i_runMode*/ )
//------------------------------------------------------------------------------
{
}

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
}

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
void CWdgtIdxTreeTestSteps::onTreeViewTestStepsExpanded( const QModelIndex& i_iModelIdx )
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
}

//------------------------------------------------------------------------------
void CWdgtIdxTreeTestSteps::onTreeViewTestStepsSelectionModelCurrentRowChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& i_modelIdxPrev )
//------------------------------------------------------------------------------
{
    if( m_pWdgtTestStep != nullptr )
    {
        CTestStep* pTestStep = getSelectedTestStep();
        m_pWdgtTestStep->setTestStep(pTestStep);
    }
}

/*==============================================================================
private: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
CTestStep* CWdgtIdxTreeTestSteps::getSelectedTestStep() const
//------------------------------------------------------------------------------
{
    CTestStep* pTestStep = nullptr;

    QModelIndex modelIdx = m_pTreeViewTestSteps->selectionModel()->currentIndex();
    if( modelIdx.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry =
            static_cast<CModelIdxTreeEntry*>(modelIdx.internalPointer());
        if( pModelTreeEntry != nullptr )
        {
            CIdxTreeEntry* pIdxTreeEntry =
                dynamic_cast<CIdxTreeEntry*>(pModelTreeEntry->getIdxTreeEntry());
            if( pIdxTreeEntry != nullptr )
            {
                CAbstractTestStepIdxTreeEntry* pTestStepEntry =
                    dynamic_cast<CAbstractTestStepIdxTreeEntry*>(pIdxTreeEntry);
                if( pTestStepEntry != nullptr && pTestStepEntry->isLeave() )
                {
                    pTestStep = dynamic_cast<CTestStep*>(pTestStepEntry);
                }
            }
        }
    }
    return pTestStep;
}
