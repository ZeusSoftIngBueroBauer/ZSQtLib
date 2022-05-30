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

#include <QtCore/qsettings.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#include <QtGui/qbuttongroup.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qradiobutton.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qbuttongroup.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qradiobutton.h>
#endif

#include "ZSTestGUI/ZSTestStepDlg.h"
#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSSysGUI/ZSSysTextEdit.h"

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
    m_pLytLineTestStep(nullptr),
    m_pLblTestStep(nullptr),
    m_pEdtTestStep(nullptr),
    m_pLytLineOperation(nullptr),
    m_pLblOperation(nullptr),
    m_pEdtOperation(nullptr),
    m_pLytLineDescription(nullptr),
    m_pLblDescription(nullptr),
    m_pEdtDescription(nullptr),
    m_pLytGrpConfigValues(nullptr),
    m_pGrpConfigValues(nullptr),
    m_pEdtConfigValues(nullptr),
    m_pLytGrpInstruction(nullptr),
    m_pGrpInstruction(nullptr),
    m_pEdtInstruction(nullptr),
    m_pLytGrpExpectedAndResultValues(nullptr),
    m_pGrpExpectedAndResultValues(nullptr),
    m_pEdtExpectedValues(nullptr),
    m_pEdtResultValues(nullptr),
    m_pLytGroupTestResults(nullptr),
    m_pGroupTestResults(nullptr),
    m_pBtnGroupTestResults(nullptr),
    m_pBtnTestResultUndefined(nullptr),
    m_pBtnTestResultPassed(nullptr),
    m_pBtnTestResultFailed(nullptr),
    m_pBtnTestResultSkip(nullptr),
    m_pLytLineButtons(nullptr),
    m_pBtnRun(nullptr),
    m_pBtnStep(nullptr),
    m_pBtnStop(nullptr)
{
    m_pTestStep->getTest()->pause();

    setWindowTitle( QCoreApplication::applicationName() + ": " + i_pTestStep->path() );

    m_pLytMain = new QVBoxLayout;
    setLayout(m_pLytMain);

    int cxLblWidth = 100;

    m_pLytLineTestStep = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytLineTestStep);
    m_pLblTestStep = new QLabel("TestStep:");
    m_pLblTestStep->setFixedWidth(cxLblWidth);
    m_pLytLineTestStep->addWidget(m_pLblTestStep);
    m_pEdtTestStep = new QLineEdit(m_pTestStep->path());
    m_pEdtTestStep->setReadOnly(true);
    m_pLytLineTestStep->addWidget(m_pEdtTestStep);

    if( !m_pTestStep->getOperation().isEmpty() )
    {
        m_pLytLineOperation = new QHBoxLayout();
        m_pLytMain->addLayout(m_pLytLineOperation);
        m_pLblOperation = new QLabel("Operation:");
        m_pLblOperation->setFixedWidth(cxLblWidth);
        m_pLytLineOperation->addWidget(m_pLblOperation);
        m_pEdtOperation = new QLineEdit(m_pTestStep->getOperation());
        m_pEdtOperation->setReadOnly(true);
        m_pLytLineOperation->addWidget(m_pEdtOperation);
    }

    if( !m_pTestStep->getDescription().isEmpty() )
    {
        m_pLytLineDescription = new QHBoxLayout();
        m_pLytMain->addLayout(m_pLytLineDescription);
        m_pLblDescription = new QLabel("Description:");
        m_pLblDescription->setFixedWidth(cxLblWidth);
        m_pLytLineDescription->addWidget(m_pLblDescription);
        m_pEdtDescription = new QLineEdit(m_pTestStep->getDescription());
        m_pEdtDescription->setReadOnly(true);
        m_pLytLineDescription->addWidget(m_pEdtDescription);
    }

    if( !m_pTestStep->getConfigValueKeys().isEmpty() )
    {
        m_pGrpConfigValues = new QGroupBox("Configuration Values:");
        m_pLytMain->addWidget(m_pGrpConfigValues);
        m_pLytGrpConfigValues = new QHBoxLayout();
        m_pEdtConfigValues = new CTextEdit();
        m_pLytGrpConfigValues->addWidget(m_pEdtConfigValues);
        QStringList strlstConfigValues;
        for( int iRow = 0; iRow < m_pTestStep->getConfigValueKeys().size(); ++iRow )
        {
            QString key = m_pTestStep->getConfigValueKeys().at(iRow);
            QVariant val = m_pTestStep->getConfigValue(key);
            QString str = key + ":\t" + val.toString();
            strlstConfigValues << str;
        }
        m_pEdtConfigValues->setText(strlstConfigValues.join("<br/>"));
        m_pGrpConfigValues->setLayout(m_pLytGrpConfigValues);
    }

    if( !m_pTestStep->getInstruction().isEmpty() )
    {
        m_pGrpInstruction = new QGroupBox("Instruction:");
        m_pLytMain->addWidget(m_pGrpInstruction);
        m_pLytGrpInstruction = new QHBoxLayout();
        m_pEdtInstruction = new CTextEdit(m_pTestStep->getInstruction());
        m_pEdtInstruction->setReadOnly(true);
        m_pEdtInstruction->setAcceptRichText(true);
        m_pLytGrpInstruction->addWidget(m_pEdtInstruction);
        m_pGrpInstruction->setLayout(m_pLytGrpInstruction);
    }

    if( !m_pTestStep->getExpectedValues().isEmpty() || !m_pTestStep->getResultValues().isEmpty() )
    {
        if( !m_pTestStep->getExpectedValues().isEmpty() && !m_pTestStep->getResultValues().isEmpty() )
        {
            m_pGrpExpectedAndResultValues = new QGroupBox("Expected and Result Values:");
        }
        else if( !m_pTestStep->getExpectedValues().isEmpty() )
        {
            m_pGrpExpectedAndResultValues = new QGroupBox("Expected Values:");
        }
        else // if( !m_pTestStep->getResultValues().isEmpty() )
        {
            m_pGrpExpectedAndResultValues = new QGroupBox("Result Values:");
        }
        m_pLytMain->addWidget(m_pGrpExpectedAndResultValues);
        m_pLytGrpExpectedAndResultValues = new QHBoxLayout();

        if( !m_pTestStep->getExpectedValues().isEmpty() )
        {
            m_pEdtExpectedValues = new CTextEdit();
            m_pEdtExpectedValues->setReadOnly(true);
            m_pEdtExpectedValues->setPlainText(m_pTestStep->getExpectedValues().join("\n"));
            m_pLytGrpExpectedAndResultValues->addWidget(m_pEdtExpectedValues);
        }
        if( !m_pTestStep->getResultValues().isEmpty() )
        {
            m_pEdtResultValues = new CTextEdit(m_pTestStep->getResultValues().join("\n"));
            m_pEdtResultValues->setReadOnly(true);
            m_pEdtResultValues->setPlainText(m_pTestStep->getResultValues().join("\n"));
            m_pLytGrpExpectedAndResultValues->addWidget(m_pEdtResultValues);
        }
        m_pGrpExpectedAndResultValues->setLayout(m_pLytGrpExpectedAndResultValues);
    }

    m_pGroupTestResults = new QGroupBox("Test Result:");
    m_pLytMain->addWidget(m_pGroupTestResults);
    m_pLytGroupTestResults = new QVBoxLayout();
    m_pBtnGroupTestResults = new QButtonGroup(this);
    m_pBtnTestResultUndefined = new QRadioButton("Undefined");
    m_pBtnTestResultUndefined->setStyleSheet("QRadioButton {color: gray};");
    m_pBtnGroupTestResults->addButton(m_pBtnTestResultUndefined);
    m_pBtnTestResultUndefined->setChecked(m_pTestStep->getTestResult() == ETestResult::Undefined);
    m_pLytGroupTestResults->addWidget(m_pBtnTestResultUndefined);
    m_pBtnTestResultPassed = new QRadioButton("PASSED");
    m_pBtnTestResultPassed->setStyleSheet("QRadioButton {color: green};");
    m_pBtnGroupTestResults->addButton(m_pBtnTestResultPassed);
    m_pBtnTestResultPassed->setChecked(m_pTestStep->getTestResult() == ETestResult::TestPassed);
    m_pLytGroupTestResults->addWidget(m_pBtnTestResultPassed);
    m_pBtnTestResultFailed = new QRadioButton("FAILED");
    m_pBtnTestResultFailed->setStyleSheet("QRadioButton {color: red};");
    m_pBtnGroupTestResults->addButton(m_pBtnTestResultFailed);
    m_pBtnTestResultFailed->setChecked(m_pTestStep->getTestResult() == ETestResult::TestFailed);
    m_pLytGroupTestResults->addWidget(m_pBtnTestResultFailed);
    m_pBtnTestResultSkip = new QRadioButton("SKIP");
    m_pBtnTestResultSkip->setStyleSheet("QRadioButton {color: blue};");
    m_pBtnGroupTestResults->addButton(m_pBtnTestResultSkip);
    m_pBtnTestResultSkip->setChecked(m_pTestStep->getTestResult() == ETestResult::TestSkipped);
    m_pLytGroupTestResults->addWidget(m_pBtnTestResultSkip);
    m_pGroupTestResults->setLayout(m_pLytGroupTestResults);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnGroupTestResults,
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
    m_pLytLineTestStep = nullptr;
    m_pLblTestStep = nullptr;
    m_pEdtTestStep = nullptr;
    m_pLytLineOperation = nullptr;
    m_pLblOperation = nullptr;
    m_pEdtOperation = nullptr;
    m_pLytLineDescription = nullptr;
    m_pLblDescription = nullptr;
    m_pEdtDescription = nullptr;
    m_pLytGrpConfigValues = nullptr;
    m_pGrpConfigValues = nullptr;
    m_pEdtConfigValues = nullptr;
    m_pLytGrpInstruction = nullptr;
    m_pGrpInstruction = nullptr;
    m_pEdtInstruction = nullptr;
    m_pLytGrpExpectedAndResultValues = nullptr;
    m_pGrpExpectedAndResultValues = nullptr;
    m_pEdtExpectedValues = nullptr;
    m_pEdtResultValues = nullptr;
    m_pLytGroupTestResults = nullptr;
    m_pGroupTestResults = nullptr;
    m_pBtnGroupTestResults = nullptr;
    m_pBtnTestResultUndefined = nullptr;
    m_pBtnTestResultPassed = nullptr;
    m_pBtnTestResultFailed = nullptr;
    m_pBtnTestResultSkip = nullptr;
    m_pLytLineButtons = nullptr;
    m_pBtnRun = nullptr;
    m_pBtnStep = nullptr;
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
void CDlgTestStep::onBtnGroupTestResultsButtonToggled(QAbstractButton* /*i_pBtn*/, bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    if( m_pBtnGroupTestResults->checkedButton() == m_pBtnTestResultUndefined )
    {
        m_pBtnRun->setEnabled(false);
        m_pBtnStep->setEnabled(false);
        m_pBtnStop->setEnabled(false);
    }
    else
    {
        m_pBtnRun->setEnabled(true);
        m_pBtnStep->setEnabled(true);
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
    else // if( i_pBtn == m_pBtnStop )
    {
        m_pTestStep->getTest()->stop();
    }

    QDialog::accept();
}
