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

#ifndef ZSTestGUI_TestStepDlg_h
#define ZSTestGUI_TestStepDlg_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qdialog.h>
#else
#include <QtWidgets/qdialog.h>
#endif

#include "ZSTestGUI/ZSTestGUIDllMain.h"

class QAbstractButton;
class QButtonGroup;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
namespace GUI
{
class CTextEdit;
}
}
namespace Test
{
class CTestStep;

namespace GUI
{
class CWdgtIdxTreeTestSteps;

//******************************************************************************
class ZSTESTGUIDLL_API CDlgTestStep : public QDialog
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CDlgTestStep(
        CTestStep*      i_pTestStep,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint));
    ~CDlgTestStep();
public: // instance methods
    void show();
    void hide();
protected: // overridables of base class QDialog
    virtual void closeEvent( QCloseEvent* i_pEv );
protected slots:
    void onBtnGroupTestResultsButtonToggled(QAbstractButton* i_pBtn, bool i_bChecked);
    void onBtnRunClicked(bool i_bChecked);
    void onBtnStepClicked(bool i_bChecked);
    void onBtnStopClicked(bool i_bChecked);
protected: // auxiliary methods
    void onBtnTestContinueClicked(QAbstractButton* i_pBtn, bool i_bChecked);
protected: // instance members
    QString                 m_strSettingsKey;
    CTestStep*              m_pTestStep;
    QVBoxLayout*            m_pLytMain;
    QHBoxLayout*            m_pLytLineTestStep;
    QLabel*                 m_pLblTestStep;
    QLineEdit*              m_pEdtTestStep;
    QHBoxLayout*            m_pLytLineOperation;
    QLabel*                 m_pLblOperation;
    QLineEdit*              m_pEdtOperation;
    QHBoxLayout*            m_pLytLineDescription;
    QLabel*                 m_pLblDescription;
    QLineEdit*              m_pEdtDescription;
    QHBoxLayout*            m_pLytGrpConfigValues;
    QGroupBox*              m_pGrpConfigValues;
    System::GUI::CTextEdit* m_pEdtConfigValues;
    QHBoxLayout*            m_pLytGrpInstruction;
    QGroupBox*              m_pGrpInstruction;
    System::GUI::CTextEdit* m_pEdtInstruction;
    QHBoxLayout*            m_pLytGrpExpectedAndResultValues;
    QGroupBox*              m_pGrpExpectedAndResultValues;
    System::GUI::CTextEdit* m_pEdtExpectedValues;
    System::GUI::CTextEdit* m_pEdtResultValues;
    QVBoxLayout*            m_pLytGroupTestResults;
    QGroupBox*              m_pGroupTestResults;
    QButtonGroup*           m_pBtnGroupTestResults;
    QRadioButton*           m_pBtnTestResultUndefined;
    QRadioButton*           m_pBtnTestResultPassed;
    QRadioButton*           m_pBtnTestResultFailed;
    QRadioButton*           m_pBtnTestResultSkip;
    QHBoxLayout*            m_pLytLineButtons;
    QPushButton*            m_pBtnRun;
    QPushButton*            m_pBtnStep;
    QPushButton*            m_pBtnStop;

}; // class CDlgTestStep

} // namespace GUI

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTestGUI_TestStepDlg_h
