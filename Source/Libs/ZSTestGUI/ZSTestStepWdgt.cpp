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
    m_pSpcBottomStretch(nullptr)
{
    m_pLytMain = new QVBoxLayout;
    setLayout(m_pLytMain);

} // ctor

//------------------------------------------------------------------------------
CWdgtTestStep::~CWdgtTestStep()
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
        QSettings settings;
        settings.setValue( m_strSettingsKey+"/Geometry", saveGeometry() );
        if( m_pSplitterInstructionWithExpectedAndResultValues != nullptr ) {
            QList<int> listSizes = m_pSplitterInstructionWithExpectedAndResultValues->sizes();
            for( int idx = 0; idx < listSizes.count(); idx++ ) {
                settings.setValue(m_strSettingsKey + "/Geometry/Splitter/Wdgt" + QString::number(idx) + "Height", listSizes[idx]);
            }
        }

        if( m_pTestStep != nullptr )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pTestStep,
                /* szSignal     */ SIGNAL(changed(ZS::System::CIdxTreeEntry*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onTestStepChanged(ZS::System::CIdxTreeEntry*)));
        }

        int cxLblWidth = 100;

        if( i_pTestStep == nullptr )
        {
            delete m_pLblTestStep;
            m_pLblTestStep = nullptr;
            delete m_pEdtTestStep;
            m_pEdtTestStep = nullptr;
            delete m_pLytLineTestStep;
            m_pLytLineTestStep = nullptr;
        }
        if( i_pTestStep == nullptr || i_pTestStep->getOperation().isEmpty() )
        {
            delete m_pLblOperation;
            m_pLblOperation = nullptr;
            delete m_pEdtOperation;
            m_pEdtOperation = nullptr;
            delete m_pLytLineOperation;
            m_pLytLineOperation = nullptr;
        }
        if( i_pTestStep == nullptr || i_pTestStep->getDescription().isEmpty() )
        {
            delete m_pLblDescription;
            m_pLblDescription = nullptr;
            delete m_pEdtDescription;
            m_pEdtDescription = nullptr;
            delete m_pLytLineDescription;
            m_pLytLineDescription = nullptr;
        }
        if( i_pTestStep == nullptr || i_pTestStep->getConfigValueKeys().isEmpty() )
        {
            delete m_pGrpConfigValues;
            m_pGrpConfigValues = nullptr;
            m_pEdtConfigValues = nullptr;
            m_pLytGrpConfigValues = nullptr;
        }
        if( i_pTestStep == nullptr || i_pTestStep->getInstruction().isEmpty() )
        {
            delete m_pGrpInstruction;
            m_pGrpInstruction = nullptr;
            m_pEdtInstruction = nullptr;
            m_pLytGrpInstruction = nullptr;

            if( m_pSplitterInstructionWithExpectedAndResultValues != nullptr )
            {
                delete m_pSplitterInstructionWithExpectedAndResultValues;
                m_pSplitterInstructionWithExpectedAndResultValues = nullptr;
                m_pWdgtGrpExpectedAndResultValues = nullptr;
                m_pLytGrpExpectedAndResultValues = nullptr;
                m_pGrpExpectedValues = nullptr;
                m_pLytGrpExpectedValues = nullptr;
                m_pEdtExpectedValues = nullptr;
                m_pGrpResultValues = nullptr;
                m_pLytGrpResultValues = nullptr;
                m_pEdtResultValues = nullptr;
            }
        }
        if( i_pTestStep == nullptr )
        {
            delete m_pWdgtGrpExpectedAndResultValues;
            m_pWdgtGrpExpectedAndResultValues = nullptr;
            m_pLytGrpExpectedAndResultValues = nullptr;
            m_pGrpExpectedValues = nullptr;
            m_pLytGrpExpectedValues = nullptr;
            m_pEdtExpectedValues = nullptr;
            m_pGrpResultValues = nullptr;
            m_pLytGrpResultValues = nullptr;
            m_pEdtResultValues = nullptr;
        }

        // Remove spacer item at bottom always as it has to be added at the end.
        if( m_pSpcBottomStretch != nullptr )
        {
            m_pLytMain->removeItem(m_pSpcBottomStretch);
            delete m_pSpcBottomStretch;
            m_pSpcBottomStretch = nullptr;
        }

        if( i_pTestStep != nullptr )
        {
            if( m_pLytLineTestStep == nullptr )
            {
                m_pLytLineTestStep = new QHBoxLayout();
                m_pLytMain->addLayout(m_pLytLineTestStep);
                m_pLblTestStep = new QLabel("Test Step:");
                m_pLblTestStep->setFixedWidth(cxLblWidth);
                m_pLytLineTestStep->addWidget(m_pLblTestStep);
                m_pEdtTestStep = new QLineEdit("");
                m_pEdtTestStep->setReadOnly(true);
                m_pLytLineTestStep->addWidget(m_pEdtTestStep);
            }
            if( !i_pTestStep->getOperation().isEmpty() && m_pLytLineOperation == nullptr )
            {
                m_pLytLineOperation = new QHBoxLayout();
                m_pLytMain->addLayout(m_pLytLineOperation);
                m_pLblOperation = new QLabel("Operation:");
                m_pLblOperation->setFixedWidth(cxLblWidth);
                m_pLytLineOperation->addWidget(m_pLblOperation);
                m_pEdtOperation = new QLineEdit("");
                m_pEdtOperation->setReadOnly(true);
                m_pLytLineOperation->addWidget(m_pEdtOperation);
            }
            if( !i_pTestStep->getDescription().isEmpty() && m_pLytLineDescription == nullptr )
            {
                m_pLytLineDescription = new QHBoxLayout();
                m_pLytMain->addLayout(m_pLytLineDescription);
                m_pLblDescription = new QLabel("Description:");
                m_pLblDescription->setFixedWidth(cxLblWidth);
                m_pLytLineDescription->addWidget(m_pLblDescription);
                m_pEdtDescription = new QLineEdit("");
                m_pEdtDescription->setReadOnly(true);
                m_pLytLineDescription->addWidget(m_pEdtDescription);
            }
            if( !i_pTestStep->getConfigValueKeys().isEmpty() && m_pGrpConfigValues == nullptr )
            {
                m_pGrpConfigValues = new QGroupBox("Configuration Values:");
                m_pLytMain->addWidget(m_pGrpConfigValues);
                m_pLytGrpConfigValues = new QHBoxLayout();
                m_pEdtConfigValues = new CTextEdit();
                m_pLytGrpConfigValues->addWidget(m_pEdtConfigValues);
                m_pGrpConfigValues->setLayout(m_pLytGrpConfigValues);
            }
            if( !i_pTestStep->getInstruction().isEmpty() && m_pSplitterInstructionWithExpectedAndResultValues == nullptr )
            {
                m_pSplitterInstructionWithExpectedAndResultValues = new QSplitter(Qt::Vertical);
                m_pLytMain->addWidget(m_pSplitterInstructionWithExpectedAndResultValues, 1);

                m_pGrpInstruction = new QGroupBox("Instruction:");
                m_pSplitterInstructionWithExpectedAndResultValues->addWidget(m_pGrpInstruction);
                m_pLytGrpInstruction = new QHBoxLayout();
                m_pEdtInstruction = new CTextEdit();
                m_pEdtInstruction->setReadOnly(true);
                m_pEdtInstruction->setAcceptRichText(true);
                m_pLytGrpInstruction->addWidget(m_pEdtInstruction);
                m_pGrpInstruction->setLayout(m_pLytGrpInstruction);
            }

            if( m_pWdgtGrpExpectedAndResultValues == nullptr )
            {
                m_pWdgtGrpExpectedAndResultValues = new QWidget();
                m_pLytGrpExpectedAndResultValues = new QHBoxLayout();
                m_pLytGrpExpectedAndResultValues->setContentsMargins(0, 0, 0, 0);
                m_pWdgtGrpExpectedAndResultValues->setLayout(m_pLytGrpExpectedAndResultValues);
                // Without setMaximumHeight the text edit widget are resized to a very high value
                // if a lot of text lines are inserted. Strange ....
                m_pWdgtGrpExpectedAndResultValues->setMaximumHeight(640);

                m_pGrpExpectedValues = new QGroupBox("Expected Values:");
                m_pLytGrpExpectedAndResultValues->addWidget(m_pGrpExpectedValues);
                m_pLytGrpExpectedValues = new QVBoxLayout();
                m_pEdtExpectedValues = new CTextEdit();
                //m_pEdtExpectedValues->setReadOnly(false);
                m_pEdtExpectedValues->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                m_pEdtExpectedValues->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
                m_pLytGrpExpectedValues->addWidget(m_pEdtExpectedValues);
                m_pGrpExpectedValues->setLayout(m_pLytGrpExpectedValues);

                m_pGrpResultValues = new QGroupBox("Result Values:");
                m_pLytGrpExpectedAndResultValues->addWidget(m_pGrpResultValues);
                m_pLytGrpResultValues = new QVBoxLayout();
                m_pEdtResultValues = new CTextEdit();
                //m_pEdtResultValues->setReadOnly(false);
                m_pEdtResultValues->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                m_pEdtResultValues->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
                m_pLytGrpResultValues->addWidget(m_pEdtResultValues);
                m_pGrpResultValues->setLayout(m_pLytGrpResultValues);

                if( m_pSplitterInstructionWithExpectedAndResultValues != nullptr )
                {
                    m_pSplitterInstructionWithExpectedAndResultValues->addWidget(m_pWdgtGrpExpectedAndResultValues);
                }
                else
                {
                    m_pLytMain->addWidget(m_pWdgtGrpExpectedAndResultValues, 1);
                }
            }
            else
            {
                if( m_pSplitterInstructionWithExpectedAndResultValues != nullptr )
                {
                    m_pSplitterInstructionWithExpectedAndResultValues->addWidget(m_pWdgtGrpExpectedAndResultValues);
                }
                else
                {
                    m_pLytMain->addWidget(m_pWdgtGrpExpectedAndResultValues, 1);
                }
            }

            if( m_pSpcBottomStretch == nullptr )
            {
                m_pSpcBottomStretch = new QSpacerItem(
                    0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
                m_pLytMain->addSpacerItem(m_pSpcBottomStretch);
            }
        } // if( i_pTestStep != nullptr )

        m_pTestStep = i_pTestStep;

        if( m_pTestStep != nullptr )
        {
            onTestStepChanged(m_pTestStep);

            if( !QObject::connect(
                /* pObjSender   */ m_pTestStep,
                /* szSignal     */ SIGNAL(changed(ZS::System::CIdxTreeEntry*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onTestStepChanged(ZS::System::CIdxTreeEntry*)) ) )
            {
                throw ZS::System::CException(__FILE__, __LINE__, EResultSignalSlotConnectionFailed);
            }

            QSettings settings;
            restoreGeometry( settings.value(m_strSettingsKey+"/Geometry").toByteArray() );
            if( m_pSplitterInstructionWithExpectedAndResultValues != nullptr ) {
                QList<int> listSizes = m_pSplitterInstructionWithExpectedAndResultValues->sizes();
                for( int idx = 0; idx < listSizes.count(); idx++ ) {
                    listSizes[idx] = settings.value(
                        m_strSettingsKey + "/Geometry/Splitter/Wdgt" + QString::number(idx) + "Height", 50 ).toInt();
                }
                m_pSplitterInstructionWithExpectedAndResultValues->setSizes(listSizes);
            }
        } // if( m_pTestStep != nullptr )
    } // if( m_pTestStep != i_pTestStep )
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
        if( m_pEdtTestStep != nullptr )
        {
            m_pEdtTestStep->setText(m_pTestStep->path());
        }
        if( m_pEdtOperation != nullptr )
        {
            m_pEdtOperation->setText(m_pTestStep->getOperation());
        }
        if( m_pEdtDescription != nullptr )
        {
            m_pEdtDescription->setText(m_pTestStep->getDescription());
        }
        if( m_pEdtConfigValues != nullptr )
        {
            QStringList strlstConfigValues;
            for( int iRow = 0; iRow < m_pTestStep->getConfigValueKeys().size(); ++iRow )
            {
                QString key = m_pTestStep->getConfigValueKeys().at(iRow);
                QVariant val = m_pTestStep->getConfigValue(key);
                QString str = key + ":\t" + val.toString();
                strlstConfigValues << str;
            }
            m_pEdtConfigValues->setText(strlstConfigValues.join("<br/>"));
        }
        if( m_pEdtInstruction != nullptr )
        {
            m_pEdtInstruction->setText(m_pTestStep->getInstruction());
        }
        if( m_pEdtExpectedValues != nullptr )
        {
            m_pEdtExpectedValues->setPlainText(m_pTestStep->getExpectedValues().join("\n"));
        }
        if( m_pEdtResultValues != nullptr )
        {
            if( m_pTestStep->isFinished() )
            {
                setResultValues(m_pTestStep->getResultValues());
            }
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
