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

#ifndef ZSTestGUI_TestStepWdgt_h
#define ZSTestGUI_TestStepWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSTestGUI/ZSTestGUIDllMain.h"

class QGroupBox;
class QLabel;
class QLineEdit;
class QSplitter;
class QSpacerItem;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CIdxTreeEntry;

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
//******************************************************************************
class ZSTESTGUIDLL_API CWdgtTestStep : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtTestStep(
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    virtual ~CWdgtTestStep();
public: // instance methods
    void setTestStep( CTestStep* i_pTestStep );
    CTestStep* getTestStep();
    QStringList getResultValues( bool i_bUnmarkFailedResultValues = false ) const;
protected slots:
    void onTestStepChanged( ZS::System::CIdxTreeEntry* i_pTestStep );
protected: // auxiliary methods
    void setResultValues( const QStringList& i_strlstResultValues );
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
    QSplitter*              m_pSplitterInstructionWithExpectedAndResultValues;
    QHBoxLayout*            m_pLytGrpInstruction;
    QGroupBox*              m_pGrpInstruction;
    System::GUI::CTextEdit* m_pEdtInstruction;
    QWidget*                m_pWdgtGrpExpectedAndResultValues;
    QHBoxLayout*            m_pLytGrpExpectedAndResultValues;
    QVBoxLayout*            m_pLytGrpExpectedValues;
    QGroupBox*              m_pGrpExpectedValues;
    System::GUI::CTextEdit* m_pEdtExpectedValues;
    QVBoxLayout*            m_pLytGrpResultValues;
    QGroupBox*              m_pGrpResultValues;
    System::GUI::CTextEdit* m_pEdtResultValues;
    QSpacerItem*            m_pSpcBottomStretch;

}; // class CWdgtTestStep

} // namespace GUI

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTestGUI_TestStepWdgt_h
