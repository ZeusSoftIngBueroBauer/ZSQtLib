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

#include "ZSTestGUI/ZSTestStepDlg.h"
#include "ZSTestGUI/ZSTestStepWdgt.h"
#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSSysGUI/ZSSysTextEdit.h"

#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#include <QtGui/qbuttongroup.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qsplitter.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qbuttongroup.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qradiobutton.h>
#include <QtWidgets/qsplitter.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test;
using namespace ZS::Test::GUI;


/*******************************************************************************
class CDlgTestStep : public QDialog
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDlgTestStep::CDlgTestStep(
    CTestStep*      i_pTestStep,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QDialog(i_pWdgtParent, i_wflags),
    m_strSettingsKey("DlgTestStep"),
    m_pTestStep(i_pTestStep),
    m_pLytMain(nullptr),
    m_pWdgtTestStep(nullptr),
    m_pLytGrpTestResults(nullptr),
    m_pGrpTestResults(nullptr),
    m_pBtnCompareExpectedWithResultValues(nullptr),
    m_pBtnGrpTestResults(nullptr),
    m_pBtnTestResultUndefined(nullptr),
    m_pBtnTestResultPassed(nullptr),
    m_pBtnTestResultFailed(nullptr),
    m_pBtnTestResultSkip(nullptr),
    m_pLytLineButtons(nullptr),
    m_pBtnRun(nullptr),
    m_pBtnStep(nullptr),
    m_pBtnPause(nullptr),
    m_pBtnStop(nullptr)
{
    m_pTestStep->getTest()->pause();

    setWindowTitle( QCoreApplication::applicationName() + ": " + i_pTestStep->path() );

    m_pLytMain = new QVBoxLayout;
    setLayout(m_pLytMain);

    int cxLblWidth = 100;

    m_pWdgtTestStep = new CWdgtTestStep();
    m_pWdgtTestStep->setTestStep(m_pTestStep);
    m_pLytMain->addWidget(m_pWdgtTestStep);

    m_pGrpTestResults = new QGroupBox("Test Result:");
    m_pLytMain->addWidget(m_pGrpTestResults);
    m_pLytGrpTestResults = new QHBoxLayout();

    m_pBtnCompareExpectedWithResultValues = new QPushButton("Compare Expected With Result Values");
    m_pLytGrpTestResults->addWidget(m_pBtnCompareExpectedWithResultValues);
    m_pLytGrpTestResults->addSpacing(20);

    m_pBtnGrpTestResults = new QButtonGroup(this);
    m_pBtnTestResultUndefined = new QRadioButton("Undefined");
    m_pBtnTestResultUndefined->setStyleSheet("QRadioButton {color: gray};");
    m_pBtnGrpTestResults->addButton(m_pBtnTestResultUndefined);
    m_pBtnTestResultUndefined->setChecked(m_pTestStep->getTestResult() == ETestResult::Undefined);
    m_pLytGrpTestResults->addWidget(m_pBtnTestResultUndefined);
    m_pBtnTestResultPassed = new QRadioButton("PASSED");
    m_pBtnTestResultPassed->setStyleSheet("QRadioButton {color: green};");
    m_pBtnGrpTestResults->addButton(m_pBtnTestResultPassed);
    m_pBtnTestResultPassed->setChecked(m_pTestStep->getTestResult() == ETestResult::TestPassed);
    m_pLytGrpTestResults->addWidget(m_pBtnTestResultPassed);
    m_pBtnTestResultFailed = new QRadioButton("FAILED");
    m_pBtnTestResultFailed->setStyleSheet("QRadioButton {color: red};");
    m_pBtnGrpTestResults->addButton(m_pBtnTestResultFailed);
    m_pBtnTestResultFailed->setChecked(m_pTestStep->getTestResult() == ETestResult::TestFailed);
    m_pLytGrpTestResults->addWidget(m_pBtnTestResultFailed);
    m_pBtnTestResultSkip = new QRadioButton("SKIP");
    m_pBtnTestResultSkip->setStyleSheet("QRadioButton {color: blue};");
    m_pBtnGrpTestResults->addButton(m_pBtnTestResultSkip);
    m_pBtnTestResultSkip->setChecked(m_pTestStep->getTestResult() == ETestResult::TestSkipped);
    m_pLytGrpTestResults->addWidget(m_pBtnTestResultSkip);
    m_pLytGrpTestResults->addStretch();
    m_pGrpTestResults->setLayout(m_pLytGrpTestResults);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnGrpTestResults,
        /* szSignal     */ SIGNAL(buttonToggled(QAbstractButton*, bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnGroupTestResultsButtonToggled(QAbstractButton*, bool))) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytLineButtons = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytLineButtons);

    QPixmap pxmStart(":/ZS/Button/ButtonCmdStart16x16.bmp");
    pxmStart.setMask(pxmStart.createHeuristicMask());
    m_pBtnRun = new QPushButton();
    m_pBtnRun->setIcon(pxmStart);
    m_pLytLineButtons->addWidget(m_pBtnRun);

    QPixmap pxmStep(":/ZS/Button/ButtonCmdStepForward16x16.bmp");
    pxmStep.setMask(pxmStep.createHeuristicMask());
    m_pBtnStep = new QPushButton();
    m_pBtnStep->setIcon(pxmStep);
    m_pLytLineButtons->addWidget(m_pBtnStep);

    QPixmap pxmPause(":/ZS/Button/ButtonCmdPause16x16.bmp");
    pxmPause.setMask(pxmPause.createHeuristicMask());
    m_pBtnPause = new QPushButton();
    m_pBtnPause->setIcon(pxmPause);
    m_pLytLineButtons->addWidget(m_pBtnPause);

    QPixmap pxmStop(":/ZS/Button/ButtonCmdStop16x16.bmp");
    pxmStop.setMask(pxmStop.createHeuristicMask());
    m_pBtnStop = new QPushButton();
    m_pBtnStop->setIcon(pxmStop);
    m_pLytLineButtons->addWidget(m_pBtnStop);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnRun,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnRunClicked(bool))) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pBtnStep,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnStepClicked(bool))) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pBtnPause,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnPauseClicked(bool))) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pBtnStop,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnStopClicked(bool))) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    onBtnGroupTestResultsButtonToggled(nullptr, true);

    QSettings settings;

    restoreGeometry( settings.value(m_strSettingsKey+"/Geometry").toByteArray() );

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnCompareExpectedWithResultValues,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCompareExpectedWithResultValuesClicked(bool))) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CDlgTestStep::~CDlgTestStep()
//------------------------------------------------------------------------------
{
    QSettings settings;
    settings.setValue( m_strSettingsKey+"/Geometry", saveGeometry() );

    //m_strSettingsKey;
    m_pTestStep = nullptr;
    m_pLytMain = nullptr;
    m_pLytGrpTestResults = nullptr;
    m_pGrpTestResults = nullptr;
    m_pBtnCompareExpectedWithResultValues = nullptr;
    m_pBtnGrpTestResults = nullptr;
    m_pBtnTestResultUndefined = nullptr;
    m_pBtnTestResultPassed = nullptr;
    m_pBtnTestResultFailed = nullptr;
    m_pBtnTestResultSkip = nullptr;
    m_pLytLineButtons = nullptr;
    m_pBtnRun = nullptr;
    m_pBtnStep = nullptr;
    m_pBtnPause = nullptr;
    m_pBtnStop = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTestStep::show()
//------------------------------------------------------------------------------
{
    QSettings settings;
    restoreGeometry( settings.value(m_strSettingsKey+"/Geometry").toByteArray() );

    QDialog::show();
}

//------------------------------------------------------------------------------
void CDlgTestStep::hide()
//------------------------------------------------------------------------------
{
    QSettings settings;
    settings.setValue( m_strSettingsKey+"/Geometry", saveGeometry() );

    QDialog::hide();
}

/*==============================================================================
protected: // overridables of base class QDialog
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTestStep::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QSettings settings;
    settings.setValue( m_strSettingsKey+"/Geometry", saveGeometry() );

    QDialog::closeEvent(i_pEv);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTestStep::onBtnCompareExpectedWithResultValuesClicked(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    compareExpectedWithResultValues();
}

//------------------------------------------------------------------------------
void CDlgTestStep::onBtnGroupTestResultsButtonToggled(QAbstractButton* /*i_pBtn*/, bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    if( m_pBtnGrpTestResults->checkedButton() == m_pBtnTestResultUndefined )
    {
        m_pBtnRun->setEnabled(false);
        m_pBtnStep->setEnabled(false);
        m_pBtnPause->setEnabled(false);
        m_pBtnStop->setEnabled(false);
    }
    else
    {
        m_pBtnRun->setEnabled(true);
        m_pBtnStep->setEnabled(true);
        m_pBtnPause->setEnabled(true);
        m_pBtnStop->setEnabled(true);
    }
}

//------------------------------------------------------------------------------
void CDlgTestStep::onBtnRunClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    onBtnTestContinueClicked(m_pBtnRun, i_bChecked);
}

//------------------------------------------------------------------------------
void CDlgTestStep::onBtnStepClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    onBtnTestContinueClicked(m_pBtnStep, i_bChecked);
}

//------------------------------------------------------------------------------
void CDlgTestStep::onBtnPauseClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    onBtnTestContinueClicked(m_pBtnPause, i_bChecked);
}

//------------------------------------------------------------------------------
void CDlgTestStep::onBtnStopClicked(bool i_bChecked)
//------------------------------------------------------------------------------
{
    onBtnTestContinueClicked(m_pBtnStop, i_bChecked);
}

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDlgTestStep::onBtnTestContinueClicked(QAbstractButton* i_pBtn, bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    if( m_pBtnTestResultPassed->isChecked() )
    {
        m_pTestStep->setTestResult(ETestResult::TestPassed);
    }
    else if( m_pBtnTestResultFailed->isChecked() )
    {
        m_pTestStep->setTestResult(ETestResult::TestFailed);
    }
    else if( m_pBtnTestResultSkip->isChecked() )
    {
        m_pTestStep->setTestResult(ETestResult::TestSkipped);
    }
    else
    {
        m_pTestStep->setTestResult(ETestResult::Undefined);
    }

    if( i_pBtn == m_pBtnRun )
    {
        m_pTestStep->getTest()->start();
    }
    else if( i_pBtn == m_pBtnStep )
    {
        m_pTestStep->getTest()->step();
    }
    else if( i_pBtn == m_pBtnPause )
    {
        m_pTestStep->getTest()->pause();
    }
    else // if( i_pBtn == m_pBtnStop )
    {
        m_pTestStep->getTest()->stop();
    }

    QDialog::accept();
}

//------------------------------------------------------------------------------
void CDlgTestStep::compareExpectedWithResultValues()
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues = m_pTestStep->getExpectedValues();
    QStringList strlstResultValues = m_pWdgtTestStep->getResultValues();

    m_pTestStep->setResultValues(strlstResultValues);

    CEnumTestResult testResult = m_pTestStep->getTestResult();

    if( testResult == ETestResult::TestPassed )
    {
        m_pBtnTestResultPassed->setChecked(true);
    }
    else if( testResult == ETestResult::TestFailed )
    {
        m_pBtnTestResultFailed->setChecked(true);
    }
}
