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
    QWidget*                m_pWdgtOperation;
    QHBoxLayout*            m_pLytLineOperation;
    QLabel*                 m_pLblOperation;
    QLineEdit*              m_pEdtOperation;
    QWidget*                m_pWdgtDescription;
    QHBoxLayout*            m_pLytLineDescription;
    QLabel*                 m_pLblDescription;
    QLineEdit*              m_pEdtDescription;
    QGroupBox*              m_pGrpConfigValues;
    QHBoxLayout*            m_pLytGrpConfigValues;
    System::GUI::CTextEdit* m_pEdtConfigValues;
    QGroupBox*              m_pGrpInstruction;
    QHBoxLayout*            m_pLytGrpInstruction;
    System::GUI::CTextEdit* m_pEdtInstruction;
    QSplitter*              m_pSplitterExpectedAndResultValues;
    QGroupBox*              m_pGrpExpectedValues;
    QVBoxLayout*            m_pLytGrpExpectedValues;
    System::GUI::CTextEdit* m_pEdtExpectedValues;
    QGroupBox*              m_pGrpResultValues;
    QVBoxLayout*            m_pLytGrpResultValues;
    System::GUI::CTextEdit* m_pEdtResultValues;
    QSpacerItem*            m_pSpcBottomStretch;

}; // class CWdgtTestStep

} // namespace GUI

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTestGUI_TestStepWdgt_h
