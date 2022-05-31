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
#include <QtCore/qtextstream.h>

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
    m_pSplitterInstructionWithExpectedAndResultValues(nullptr),
    m_pLytGrpInstruction(nullptr),
    m_pGrpInstruction(nullptr),
    m_pEdtInstruction(nullptr),
    m_pWdgtGrpExpectedAndResultValues(nullptr),
    m_pLytGrpExpectedAndResultValues(nullptr),
    m_pLytGrpExpectedValues(nullptr),
    m_pGrpExpectedValues(nullptr),
    m_pEdtExpectedValues(nullptr),
    m_pLytGrpResultValues(nullptr),
    m_pGrpResultValues(nullptr),
    m_pEdtResultValues(nullptr),
    m_pLytGrpTestResults(nullptr),
    m_pGrpTestResults(nullptr),
    m_pBtnCompareExpectedWithResultValues(nullptr),
    m_pChkMarkFailedResultValues(nullptr),
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

    m_pSplitterInstructionWithExpectedAndResultValues = nullptr;

    if( !m_pTestStep->getInstruction().isEmpty() )
    {
        m_pSplitterInstructionWithExpectedAndResultValues = new QSplitter(Qt::Vertical);
        m_pLytMain->addWidget(m_pSplitterInstructionWithExpectedAndResultValues, 1);

        m_pGrpInstruction = new QGroupBox("Instruction:");
        m_pSplitterInstructionWithExpectedAndResultValues->addWidget(m_pGrpInstruction);
        m_pLytGrpInstruction = new QHBoxLayout();
        m_pEdtInstruction = new CTextEdit(m_pTestStep->getInstruction());
        m_pEdtInstruction->setReadOnly(true);
        m_pEdtInstruction->setAcceptRichText(true);
        m_pLytGrpInstruction->addWidget(m_pEdtInstruction);
        m_pGrpInstruction->setLayout(m_pLytGrpInstruction);
    }

    m_pWdgtGrpExpectedAndResultValues = new QWidget();
    m_pLytGrpExpectedAndResultValues = new QHBoxLayout();
    m_pLytGrpExpectedAndResultValues->setContentsMargins(0, 0, 0, 0);
    m_pWdgtGrpExpectedAndResultValues->setLayout(m_pLytGrpExpectedAndResultValues);
    // Without setMaximumHeight the text edit widget are resized to a very high value
    // if a lot of text lines are inserted. Strange ....
    m_pWdgtGrpExpectedAndResultValues->setMaximumHeight(640);

    if( m_pSplitterInstructionWithExpectedAndResultValues != nullptr )
    {
        m_pSplitterInstructionWithExpectedAndResultValues->addWidget(m_pWdgtGrpExpectedAndResultValues);
    }
    else
    {
        m_pLytMain->addWidget(m_pWdgtGrpExpectedAndResultValues, 1);
    }

    m_pGrpExpectedValues = new QGroupBox("Expected Values:");
    m_pLytGrpExpectedAndResultValues->addWidget(m_pGrpExpectedValues);
    m_pLytGrpExpectedValues = new QVBoxLayout();
    m_pEdtExpectedValues = new CTextEdit();
    //m_pEdtExpectedValues->setReadOnly(false);
    m_pEdtExpectedValues->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pEdtExpectedValues->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
    m_pEdtExpectedValues->setPlainText(m_pTestStep->getExpectedValues().join("\n"));
    m_pLytGrpExpectedValues->addWidget(m_pEdtExpectedValues);
    m_pGrpExpectedValues->setLayout(m_pLytGrpExpectedValues);

    m_pGrpExpectedValues = new QGroupBox("Result Values:");
    m_pLytGrpExpectedAndResultValues->addWidget(m_pGrpExpectedValues);
    m_pLytGrpResultValues = new QVBoxLayout();
    m_pEdtResultValues = new CTextEdit(m_pTestStep->getResultValues().join("\n"));
    //m_pEdtResultValues->setReadOnly(false);
    m_pEdtResultValues->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pEdtResultValues->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
    m_pEdtResultValues->setPlainText(m_pTestStep->getResultValues().join("\n"));
    m_pLytGrpResultValues->addWidget(m_pEdtResultValues);
    m_pGrpExpectedValues->setLayout(m_pLytGrpResultValues);

    m_pGrpTestResults = new QGroupBox("Test Result:");
    m_pLytMain->addWidget(m_pGrpTestResults);
    m_pLytGrpTestResults = new QHBoxLayout();

    m_pBtnCompareExpectedWithResultValues = new QPushButton("Compare Expected With Result Values");
    m_pLytGrpTestResults->addWidget(m_pBtnCompareExpectedWithResultValues);
    m_pLytGrpTestResults->addSpacing(20);

    m_pChkMarkFailedResultValues = new QCheckBox("Mark Failed Result Values");
    m_pLytGrpTestResults->addWidget(m_pChkMarkFailedResultValues);
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

    if( m_pSplitterInstructionWithExpectedAndResultValues != nullptr )
    {
        QList<int> listSizes;
        int        idx;

        listSizes = m_pSplitterInstructionWithExpectedAndResultValues->sizes();

        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            listSizes[idx] = settings.value(
                m_strSettingsKey + "/Geometry/Splitter/Wdgt" + QString::number(idx) + "Height", 50 ).toInt();
        }
        m_pSplitterInstructionWithExpectedAndResultValues->setSizes(listSizes);
    }

    m_pChkMarkFailedResultValues->setChecked(
        settings.value(m_strSettingsKey+"/MarkFailedResultValues", true).toBool() );

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnCompareExpectedWithResultValues,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnCompareExpectedWithResultValuesClicked(bool))) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pChkMarkFailedResultValues,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkMarkFailedResultValuesToggled(bool))) )
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

    if( m_pSplitterInstructionWithExpectedAndResultValues != nullptr )
    {
        QList<int> listSizes;
        int        idx;

        listSizes = m_pSplitterInstructionWithExpectedAndResultValues->sizes();

        for( idx = 0; idx < listSizes.count(); idx++ )
        {
            settings.setValue(m_strSettingsKey + "/Geometry/Splitter/Wdgt" + QString::number(idx) + "Height", listSizes[idx]);
        }
    }

    settings.setValue(m_strSettingsKey+"/MarkFailedResultValues", m_pChkMarkFailedResultValues->isChecked());

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
    m_pSplitterInstructionWithExpectedAndResultValues = nullptr;
    m_pLytGrpInstruction = nullptr;
    m_pGrpInstruction = nullptr;
    m_pEdtInstruction = nullptr;
    m_pWdgtGrpExpectedAndResultValues = nullptr;
    m_pLytGrpExpectedAndResultValues = nullptr;
    m_pLytGrpExpectedValues = nullptr;
    m_pGrpExpectedValues = nullptr;
    m_pEdtExpectedValues = nullptr;
    m_pLytGrpResultValues = nullptr;
    m_pGrpResultValues = nullptr;
    m_pEdtResultValues = nullptr;
    m_pLytGrpTestResults = nullptr;
    m_pGrpTestResults = nullptr;
    m_pBtnCompareExpectedWithResultValues = nullptr;
    m_pChkMarkFailedResultValues = nullptr;
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
void CDlgTestStep::onChkMarkFailedResultValuesToggled(bool /*i_bChecked*/)
//------------------------------------------------------------------------------
{
    if( !m_pChkMarkFailedResultValues->isChecked() )
    {
        QStringList strlstResultValues;
        QString strResultValue = m_pEdtResultValues->toPlainText();
        QTextStream txtStreamEdtResultValues(&strResultValue, QIODevice::ReadOnly);
        while (!txtStreamEdtResultValues.atEnd())
        {
            QString strTmp = txtStreamEdtResultValues.readLine();
            if( strTmp.startsWith("!! ") )
            {
                strTmp.remove(0, 3);
            }
            if( strTmp.endsWith(" !!") )
            {
                strTmp.remove(strTmp.size()-3, 3);
            }
            strlstResultValues << strTmp;
        }
        m_pEdtResultValues->clear();
        m_pEdtResultValues->setPlainText(strlstResultValues.join("\n"));
    }

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
    QStringList strlstExpectedValues;

    QString strExpectedValue = m_pEdtExpectedValues->toPlainText();
    QTextStream txtStreamEdtExpectedValues(&strExpectedValue, QIODevice::ReadOnly);
    while (!txtStreamEdtExpectedValues.atEnd())
    {
        strlstExpectedValues << txtStreamEdtExpectedValues.readLine();
    }

    QStringList strlstResultValues;

    QString strResultValue = m_pEdtResultValues->toPlainText();
    QTextStream txtStreamEdtResultValues(&strResultValue, QIODevice::ReadOnly);
    while (!txtStreamEdtResultValues.atEnd())
    {
        QString strTmp = txtStreamEdtResultValues.readLine();
        if( m_pChkMarkFailedResultValues->isChecked() )
        {
            if( strTmp.startsWith("!! ") )
            {
                strTmp.remove(0, 3);
            }
            if( strTmp.endsWith(" !!") )
            {
                strTmp.remove(strTmp.size()-3, 3);
            }
        }
        strlstResultValues << strTmp;
    }

    CEnumTestResult testResult = ETestResult::TestPassed;

    if( m_pChkMarkFailedResultValues->isChecked() )
    {
        int idxVal;

        for( idxVal = 0; idxVal < strlstExpectedValues.size(); idxVal++ )
        {
            strExpectedValue = strlstExpectedValues[idxVal];

            if( idxVal < strlstResultValues.size() )
            {
                strResultValue = strlstResultValues[idxVal];

                if( strExpectedValue.compare(strResultValue, Qt::CaseSensitive) != 0 )
                {
                    testResult = ETestResult::TestFailed;
                    strlstResultValues[idxVal] = "!! " + strResultValue + " !!";
                }
            }
            else
            {
                testResult = ETestResult::TestFailed;
                strlstResultValues << "!!  !!";
            }
        }
        for( ; idxVal < strlstResultValues.size(); idxVal++ )
        {
            strResultValue = strlstResultValues[idxVal];
            strlstResultValues[idxVal] = "!! " + strResultValue + " !!";
        }

        if( testResult == ETestResult::TestFailed )
        {
            m_pEdtResultValues->clear();
            m_pEdtResultValues->setPlainText(strlstResultValues.join("\n"));
        }
    }
    else
    {
        testResult = m_pTestStep->detectTestResult(strlstExpectedValues, strlstResultValues);
    }

    if( testResult == ETestResult::TestPassed )
    {
        m_pBtnTestResultPassed->setChecked(true);
    }
    else if( testResult == ETestResult::TestFailed )
    {
        m_pBtnTestResultFailed->setChecked(true);
    }
}
