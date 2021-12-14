/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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
#include <QtGui/qitemdelegate.h>
#include <QtGui/qtreeview.h>
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qitemdelegate.h>
#include <QtWidgets/qtreeview.h>
#include <QtWidgets/qwidget.h>
#endif

#include "ZSTestGUI/ZSTestGUIDllMain.h"
#include "ZSTestGUI/ZSTestStepIdxTreeModel.h"
#include "ZSSys/ZSSysRequest.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSplitter;
class QTableView;
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
class CTest;
class CTestStep;

namespace GUI
{
class CModeldxTreeTestSteps;

//******************************************************************************
class ZSTESTGUIDLL_API CWdgtIdxTreeTestSteps : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtIdxTreeTestSteps(
        CTest*          i_pTest,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    virtual ~CWdgtIdxTreeTestSteps();
public: // instance methods
    QTreeView* treeView() { return m_pTreeViewTestSteps; }
protected slots:
    void onBtnStartClicked( bool i_bChecked = true );
    void onBtnStepClicked( bool i_bChecked = true );
    void onBtnPauseClicked( bool i_bChecked = true );
    void onBtnStopClicked( bool i_bChecked = true );
protected slots:
    void onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
    void onBtnTreeViewExpandAllClicked( bool i_bChecked );
    void onBtnTreeViewCollapseAllClicked( bool i_bChecked );
protected slots:
    void onTestStateChanged( int i_iStateNew, int i_iStatePrev );
    //void onTestRunModeChanged( int i_iRunModeNew, int i_iRunModePrev );
    void onCurrentTestStepChanged( ZS::Test::CTestStep* i_pTestStep );
protected slots:
    void onModelClearing( QObject* i_pAdminIdxTreeModel );
    void onTreeViewExpanded( const QModelIndex& i_iModelIdx );
    void onTreeViewSelectedNodeChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected: // instance members
    CTest*                     m_pTest;
    ZS::System::CRequest*      m_pReqInProgress;
    QVBoxLayout*               m_pLytMain;
    QHBoxLayout*               m_pLytHeadLine;
    QPushButton*               m_pBtnStart;
    QPushButton*               m_pBtnStep;
    QPushButton*               m_pBtnPause;
    QPushButton*               m_pBtnStop;
    QPushButton*               m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QPushButton*               m_pBtnTreeViewExpandAll;
    QPushButton*               m_pBtnTreeViewCollapseAll;
    QLabel*                    m_pLblTestStepCurr;
    QLineEdit*                 m_pEdtTestStepCurr;
    CModeldxTreeTestSteps*     m_pTestStepsModel;
    QTreeView*                 m_pTreeViewTestSteps;
    ZS::System::CIdxTreeEntry* m_pTreeEntrySelected;

}; // class CWgdtTest

} // namespace GUI

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTestGUI_TestStepIdxTreeWdgt_h
