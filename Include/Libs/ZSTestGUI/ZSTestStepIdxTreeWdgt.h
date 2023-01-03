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

#ifndef ZSTestGUI_TestStepIdxTreeWdgt_h
#define ZSTestGUI_TestStepIdxTreeWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSTestGUI/ZSTestGUIDllMain.h"
#include "ZSTest/ZSTest.h"

class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSplitter;
class QTreeView;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CIdxTreeEntry;
}
namespace Test
{
class CTestStep;

namespace GUI
{
class CModeldxTreeTestSteps;
class CWdgtTestStep;

//******************************************************************************
class ZSTESTGUIDLL_API CWdgtIdxTreeTestSteps : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum class EViewMode {
        NavPanelOnly            = 0,
        NavPanelAndLeaveContent = 1,
        Count,
        Undefined
    };
    static QString viewMode2Str( EViewMode i_eVal, int i_alias = ZS::System::EEnumEntryAliasStrName );
    static QPixmap viewMode2Pixmap( EViewMode i_eVal, const QSize& i_sz = QSize(24,24));
public: // ctors and dtor
    CWdgtIdxTreeTestSteps(
        CTest*          i_pTest,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    virtual ~CWdgtIdxTreeTestSteps();
public: // instance methods
    QTreeView* treeView() { return m_pTreeViewTestSteps; }
public: // instance methods
    void setViewMode( EViewMode i_viewMode );
    EViewMode viewMode() const { return m_viewMode; }
protected slots:
    void onBtnStartClicked( bool i_bChecked = true );
    void onBtnStepClicked( bool i_bChecked = true );
    void onBtnPauseClicked( bool i_bChecked = true );
    void onBtnStopClicked( bool i_bChecked = true );
protected slots:
    void onBtnViewModeClicked( bool i_bChecked );
    void onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
    void onBtnTreeViewExpandAllClicked( bool i_bChecked );
    void onBtnTreeViewCollapseAllClicked( bool i_bChecked );
protected slots:
    void onEdtTestStepIntervalEditingFinished();
protected slots:
    void onTestStateChanged( const ZS::Test::CEnumTestState& i_state );
    void onTestRunModeChanged( const ZS::System::CEnumRunMode& i_runMode );
    void onCurrentTestStepChanged( ZS::Test::CTestStep* i_pTestStep );
    void onTestStepIntervalChanged( int i_iInterval_ms );
protected slots:
    void onTreeViewTestStepsExpanded( const QModelIndex& i_iModelIdx );
    void onTreeViewTestStepsSelectionModelCurrentRowChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
private: // auxiliary methods
    CTestStep* getSelectedTestStep() const;
protected: // instance members
    QString                m_strSettingsKey;
    CTest*                 m_pTest;
    EViewMode              m_viewMode;
    QSize                  m_szBtns;
    QVBoxLayout*           m_pLytMain;
    QHBoxLayout*           m_pLytHeadLine;
    QPushButton*           m_pBtnStart;
    QPushButton*           m_pBtnStep;
    QPushButton*           m_pBtnPause;
    QPushButton*           m_pBtnStop;
    QPushButton*           m_pBtnViewMode;
    QPushButton*           m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QPushButton*           m_pBtnTreeViewExpandAll;
    QPushButton*           m_pBtnTreeViewCollapseAll;
    QLabel*                m_pLblTestStepIntervalInMs;
    QLineEdit*             m_pEdtTestStepIntervalInMs;
    QLabel*                m_pLblTestStepCurr;
    QLineEdit*             m_pEdtTestStepCurr;
    CModeldxTreeTestSteps* m_pTestStepsModel;
    QSplitter*             m_pSplitter;
    QTreeView*             m_pTreeViewTestSteps;
    QGroupBox*             m_pGrpWdgtTestStep;
    QVBoxLayout*           m_pLytGrpWdgtTestStep;
    CWdgtTestStep*         m_pWdgtTestStep;

}; // class CWgdtTest

} // namespace GUI

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTestGUI_TestStepIdxTreeWdgt_h
