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

#include "ZSTestGUI/ZSTestStepWdgt.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSSysGUI/ZSSysTextEdit.h"

#include <QtCore/qsettings.h>
#include <QtCore/qtextstream.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qsplitter.h>
#else
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qsplitter.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test;
using namespace ZS::Test::GUI;


/*******************************************************************************
class CWdgtTestStep : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTestStep::CWdgtTestStep(
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent, i_wflags),
    m_strSettingsKey("WdgtTestStep"),
    m_pTestStep(nullptr),
    m_pLytMain(nullptr),
    m_pLytLineTestStep(nullptr),
    m_pLblTestStep(nullptr),
    m_pEdtTestStep(nullptr),
    m_pWdgtOperation(nullptr),
    m_pLytLineOperation(nullptr),
    m_pLblOperation(nullptr),
    m_pEdtOperation(nullptr),
    m_pWdgtDescription(nullptr),
    m_pLytLineDescription(nullptr),
    m_pLblDescription(nullptr),
    m_pEdtDescription(nullptr),
    m_pGrpConfigValues(nullptr),
    m_pLytGrpConfigValues(nullptr),
    m_pEdtConfigValues(nullptr),
    m_pGrpInstruction(nullptr),
    m_pLytGrpInstruction(nullptr),
    m_pEdtInstruction(nullptr),
    m_pSplitterExpectedAndResultValues(nullptr),
    m_pGrpExpectedValues(nullptr),
    m_pLytGrpExpectedValues(nullptr),
    m_pEdtExpectedValues(nullptr),
    m_pGrpResultValues(nullptr),
    m_pLytGrpResultValues(nullptr),
    m_pEdtResultValues(nullptr),
    m_pSpcBottomStretch(nullptr)
{
    m_pLytMain = new QVBoxLayout;
    setLayout(m_pLytMain);

    int cxLblWidth = 100;

    m_pLytLineTestStep = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytLineTestStep);
    m_pLblTestStep = new QLabel("Test Step:");
    m_pLblTestStep->setFixedWidth(cxLblWidth);
    m_pLytLineTestStep->addWidget(m_pLblTestStep);
    m_pEdtTestStep = new QLineEdit("");
    //m_pEdtTestStep->setReadOnly(true);
    m_pEdtTestStep->setEnabled(false);
    m_pLytLineTestStep->addWidget(m_pEdtTestStep);

    m_pWdgtOperation = new QWidget();
    m_pLytLineOperation = new QHBoxLayout();
    m_pLytLineOperation->setContentsMargins(0, 0, 0, 0);
    m_pWdgtOperation->setLayout(m_pLytLineOperation);
    m_pLblOperation = new QLabel("Operation:");
    m_pLblOperation->setFixedWidth(cxLblWidth);
    m_pLytLineOperation->addWidget(m_pLblOperation);
    m_pEdtOperation = new QLineEdit("");
    m_pEdtOperation->setReadOnly(true);
    m_pLytLineOperation->addWidget(m_pEdtOperation);
    m_pLytMain->addWidget(m_pWdgtOperation);
    m_pWdgtOperation->hide();

    m_pWdgtDescription = new QWidget();
    m_pLytLineDescription = new QHBoxLayout();
    m_pLytLineDescription->setContentsMargins(0, 0, 0, 0);
    m_pWdgtDescription->setLayout(m_pLytLineDescription);
    m_pLblDescription = new QLabel("Description:");
    m_pLblDescription->setFixedWidth(cxLblWidth);
    m_pLytLineDescription->addWidget(m_pLblDescription);
    m_pEdtDescription = new QLineEdit("");
    m_pEdtDescription->setReadOnly(true);
    m_pLytLineDescription->addWidget(m_pEdtDescription);
    m_pLytMain->addWidget(m_pWdgtDescription);
    m_pWdgtDescription->hide();

    m_pGrpConfigValues = new QGroupBox("Configuration Values:");
    m_pLytGrpConfigValues = new QHBoxLayout();
    m_pGrpConfigValues->setLayout(m_pLytGrpConfigValues);
    m_pEdtConfigValues = new CTextEdit();
    m_pLytGrpConfigValues->addWidget(m_pEdtConfigValues);
    m_pLytMain->addWidget(m_pGrpConfigValues);
    m_pGrpConfigValues->hide();

    m_pGrpInstruction = new QGroupBox("Instruction:");
    m_pLytGrpInstruction = new QHBoxLayout();
    m_pGrpInstruction->setLayout(m_pLytGrpInstruction);
    m_pEdtInstruction = new CTextEdit();
    m_pEdtInstruction->setReadOnly(true);
    m_pEdtInstruction->setAcceptRichText(true);
    m_pLytGrpInstruction->addWidget(m_pEdtInstruction);
    m_pLytMain->addWidget(m_pGrpInstruction);
    m_pGrpInstruction->hide();

    m_pSplitterExpectedAndResultValues = new QSplitter(Qt::Horizontal);
    m_pSplitterExpectedAndResultValues->setContentsMargins(0, 0, 0, 0);
    m_pLytMain->addWidget(m_pSplitterExpectedAndResultValues, 1);
    m_pSplitterExpectedAndResultValues->hide();

    m_pGrpExpectedValues = new QGroupBox("Expected Values:");
    m_pLytGrpExpectedValues = new QVBoxLayout();
    m_pGrpExpectedValues->setLayout(m_pLytGrpExpectedValues);
    m_pEdtExpectedValues = new CTextEdit();
    m_pEdtExpectedValues->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pEdtExpectedValues->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pEdtExpectedValues->setWordWrapMode(QTextOption::NoWrap);
    m_pEdtExpectedValues->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
    QFont fnt = m_pEdtExpectedValues->currentFont();
    fnt.setFamily("Courier New");
    m_pEdtExpectedValues->setFont(fnt);
    // Without setMaximumHeight the text edit widget are resized to a very high value
    // if a lot of text lines are inserted. Strange ....
    m_pEdtExpectedValues->setMaximumHeight(640);
    m_pLytGrpExpectedValues->addWidget(m_pEdtExpectedValues);
    m_pSplitterExpectedAndResultValues->addWidget(m_pGrpExpectedValues);

    m_pGrpResultValues = new QGroupBox("Result Values:");
    m_pLytGrpResultValues = new QVBoxLayout();
    m_pGrpResultValues->setLayout(m_pLytGrpResultValues);
    m_pEdtResultValues = new CTextEdit();
    m_pEdtResultValues->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pEdtResultValues->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pEdtResultValues->setWordWrapMode(QTextOption::NoWrap);
    m_pEdtResultValues->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
    m_pEdtResultValues->setFont(fnt);
    // Without setMaximumHeight the text edit widget are resized to a very high value
    // if a lot of text lines are inserted. Strange ....
    m_pEdtResultValues->setMaximumHeight(640);
    m_pLytGrpResultValues->addWidget(m_pEdtResultValues);
    m_pSplitterExpectedAndResultValues->addWidget(m_pGrpResultValues);

    m_pSpcBottomStretch = new QSpacerItem(
        0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pLytMain->addSpacerItem(m_pSpcBottomStretch);

    QSettings settings;
    restoreGeometry( settings.value(m_strSettingsKey+"/Geometry").toByteArray() );
    QList<int> listSizes = m_pSplitterExpectedAndResultValues->sizes();
    for( int idx = 0; idx < listSizes.count(); idx++ ) {
        listSizes[idx] = settings.value(
            m_strSettingsKey + "/Geometry/Splitter/Wdgt" + QString::number(idx) + "Height", 50 ).toInt();
    }
    m_pSplitterExpectedAndResultValues->setSizes(listSizes);

} // ctor

//------------------------------------------------------------------------------
CWdgtTestStep::~CWdgtTestStep()
//------------------------------------------------------------------------------
{
    QSettings settings;
    settings.setValue( m_strSettingsKey+"/Geometry", saveGeometry() );
    QList<int> listSizes = m_pSplitterExpectedAndResultValues->sizes();
    for( int idx = 0; idx < listSizes.count(); idx++ ) {
        settings.setValue(m_strSettingsKey + "/Geometry/Splitter/Wdgt" + QString::number(idx) + "Height", listSizes[idx]);
    }

    //m_strSettingsKey;
    m_pTestStep = nullptr;
    m_pLytMain = nullptr;
    m_pLytLineTestStep = nullptr;
    m_pLblTestStep = nullptr;
    m_pEdtTestStep = nullptr;
    m_pLytLineOperation = nullptr;
    m_pWdgtOperation = nullptr;
    m_pLblOperation = nullptr;
    m_pEdtOperation = nullptr;
    m_pWdgtDescription = nullptr;
    m_pLytLineDescription = nullptr;
    m_pLblDescription = nullptr;
    m_pEdtDescription = nullptr;
    m_pGrpConfigValues = nullptr;
    m_pLytGrpConfigValues = nullptr;
    m_pEdtConfigValues = nullptr;
    m_pGrpInstruction = nullptr;
    m_pLytGrpInstruction = nullptr;
    m_pEdtInstruction = nullptr;
    m_pSplitterExpectedAndResultValues = nullptr;
    m_pGrpExpectedValues = nullptr;
    m_pLytGrpExpectedValues = nullptr;
    m_pEdtExpectedValues = nullptr;
    m_pGrpResultValues = nullptr;
    m_pLytGrpResultValues = nullptr;
    m_pEdtResultValues = nullptr;
    m_pSpcBottomStretch = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTestStep::setTestStep( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    if( m_pTestStep != i_pTestStep )
    {
        if( m_pTestStep != nullptr )
        {
            QObject::disconnect(
                m_pTestStep, &CTestStep::changed,
                this, &CWdgtTestStep::onTestStepChanged);
        }

        m_pTestStep = i_pTestStep;

        if( m_pTestStep == nullptr )
        {
            m_pEdtTestStep->setText("---");
            m_pWdgtOperation->hide();
            m_pWdgtDescription->hide();
            m_pGrpConfigValues->hide();
            m_pGrpInstruction->hide();
            m_pSplitterExpectedAndResultValues->hide();
        }
        else
        {
            m_pWdgtOperation->setVisible(!m_pTestStep->getOperation().isEmpty());
            m_pWdgtDescription->setVisible(!m_pTestStep->getDescription().isEmpty());
            m_pGrpConfigValues->setVisible(!m_pTestStep->getConfigValueKeys().isEmpty());
            m_pGrpInstruction->setVisible(!m_pTestStep->getInstruction().isEmpty());
            m_pSplitterExpectedAndResultValues->show();

            onTestStepChanged(m_pTestStep);

            QObject::connect(
                m_pTestStep, &CTestStep::changed,
                this, &CWdgtTestStep::onTestStepChanged);
        }
    }
} // setTestStep

//------------------------------------------------------------------------------
CTestStep* CWdgtTestStep::getTestStep()
//------------------------------------------------------------------------------
{
    return m_pTestStep;
}

//------------------------------------------------------------------------------
QStringList CWdgtTestStep::getResultValues( bool i_bUnmarkFailedResultValues ) const
//------------------------------------------------------------------------------
{
    QStringList strlstResultValues;
    QString strResultValue = m_pEdtResultValues->toPlainText();
    QTextStream txtStreamEdtResultValues(&strResultValue, QIODevice::ReadOnly);
    while (!txtStreamEdtResultValues.atEnd())
    {
        QString strTmp = txtStreamEdtResultValues.readLine();
        if( i_bUnmarkFailedResultValues )
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
    return strlstResultValues;
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTestStep::onTestStepChanged( CIdxTreeEntry* i_pTestStep )
//------------------------------------------------------------------------------
{
    if( m_pTestStep != nullptr && i_pTestStep == m_pTestStep )
    {
        m_pEdtTestStep->setText(m_pTestStep->path());
        m_pEdtOperation->setText(m_pTestStep->getOperation());
        m_pEdtDescription->setText(m_pTestStep->getDescription());

        QStringList strlstConfigValues;
        for( int iRow = 0; iRow < m_pTestStep->getConfigValueKeys().size(); ++iRow )
        {
            QString key = m_pTestStep->getConfigValueKeys().at(iRow);
            QVariant val = m_pTestStep->getConfigValue(key);
            QString str = key + ":\t" + val.toString();
            strlstConfigValues << str;
        }
        m_pEdtConfigValues->setText(strlstConfigValues.join("<br/>"));
        m_pEdtInstruction->setText(m_pTestStep->getInstruction());
        m_pEdtExpectedValues->setPlainText(m_pTestStep->getExpectedValues().join("\n"));

        if( m_pTestStep->isFinished() )
        {
            setResultValues(m_pTestStep->getResultValues());
        }
        else
        {
            setResultValues(QStringList());
        }
    }
}

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTestStep::setResultValues( const QStringList& i_strlstResultValues )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues = m_pTestStep->getExpectedValues();
    QStringList strlstResultValues = i_strlstResultValues;

    int idxVal;

    for( idxVal = 0; idxVal < strlstExpectedValues.size(); idxVal++ )
    {
        QString strExpectedValue = strlstExpectedValues[idxVal];

        if( idxVal < i_strlstResultValues.size() )
        {
            QString strResultValue = i_strlstResultValues[idxVal];

            if( strExpectedValue.compare(strResultValue, Qt::CaseSensitive) != 0 )
            {
                strlstResultValues[idxVal] = "!! " + strResultValue + " !!";
            }
        }
        else
        {
            strlstResultValues << "!!  !!";
        }
    }
    for( ; idxVal < strlstResultValues.size(); idxVal++ )
    {
        QString strResultValue = strlstResultValues[idxVal];
        strlstResultValues[idxVal] = "!! " + strResultValue + " !!";
    }

    m_pEdtResultValues->clear();
    m_pEdtResultValues->setPlainText(strlstResultValues.join("\n"));
}
