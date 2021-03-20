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

#ifndef ZSTestGUI_Wdgt_h
#define ZSTestGUI_Wdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qitemdelegate.h>
#include <QtGui/qtreeview.h>
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qitemdelegate.h>
#include <QtWidgets/qtreeview.h>
#include <QtWidgets/qwidget.h>
#endif

#include "ZSTestGUI/ZSTestGUIDllMain.h"
#include "ZSTestGUI/ZSTestStepAdminObjPoolModel.h"
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
class CObjPoolTreeEntry;
}
namespace Test
{
class CTest;
class CTestStep;

namespace GUI
{
class CTestStepAdminObjPoolModel;

//******************************************************************************
class ZSTESTGUIDLL_API CDelegateModelTest : public QItemDelegate
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CDelegateModelTest( QObject* i_pObjParent = nullptr );
    virtual ~CDelegateModelTest();
public: // instance methods
    bool isCheckBoxBreakpointHit(
        const QRect&       i_rectVisual,
        const QPoint&      i_ptPos,
        const QModelIndex& i_modelIdx );
    bool isCheckBoxEnabledHit(
        const QRect&       i_rectVisual,
        const QPoint&      i_ptPos,
        const QModelIndex& i_modelIdx );
public: // overridables of base class QItemDelegate
    virtual void paint(
        QPainter*                   i_pPainter,
        const QStyleOptionViewItem& i_styleOption,
        const QModelIndex&          i_modelIdx ) const;
private: // copy ctor not implemented
    CDelegateModelTest( const CDelegateModelTest& );
private: // assignment operator not implemented
    CDelegateModelTest& operator = ( const CDelegateModelTest& );
protected: // instance members
    mutable QRect m_rectChkBoxBreakpoint;
    mutable QRect m_rectChkBoxEnabled;

}; // class CDelegateModelTest


//******************************************************************************
class ZSTESTGUIDLL_API CTreeViewModelTest : public QTreeView
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CTreeViewModelTest( CTestStepAdminObjPoolModel* i_pAdminObjPoolModel, QWidget* i_pWdgtParent = nullptr );
    virtual ~CTreeViewModelTest();
protected: // overridables of base class QTreeView
    virtual void keyPressEvent( QKeyEvent* i_pEv );
    virtual void mousePressEvent( QMouseEvent* i_pEv );
    virtual void mouseReleaseEvent( QMouseEvent* i_pEv );
protected slots:
    void onHeaderCustomContextMenuRequested( const QPoint& i_pt );
protected slots:
    void onActionEnableTestGroupsTriggered( bool i_bChecked );
    void onActionDisableTestGroupsTriggered( bool i_bChecked );
protected: // instance members
    CTestStepAdminObjPoolModel*    m_pTestStepsModel;
    QItemDelegate*                 m_pDelegate;
    QMenu*                         m_pMenuGroupContext;
    QAction*                       m_pActionTitle;
    QAction*                       m_pActionEnableTestGroups;
    QAction*                       m_pActionDisableTestGroups;
    ZS::System::CObjPoolTreeEntry* m_pTreeEntryGroupContextAction;

}; // class CTreeViewModelTest


//******************************************************************************
class ZSTESTGUIDLL_API CWdgtTest : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtTest(
        CTest*          i_pTest,
        //bool            i_bUseContentWidget = false,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    virtual ~CWdgtTest();
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
    void onModelClearing( QObject* i_pAdminObjPoolModel );
    void onTreeViewExpanded( const QModelIndex& i_iModelIdx );
    void onTreeViewSelectedNodeChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected: // instance members
    CTest*                         m_pTest;
    ZS::System::CRequest*          m_pReqInProgress;
    QVBoxLayout*                   m_pLytMain;
    QHBoxLayout*                   m_pLytHeadLine;
    QPushButton*                   m_pBtnStart;
    QPushButton*                   m_pBtnStep;
    QPushButton*                   m_pBtnPause;
    QPushButton*                   m_pBtnStop;
    QPushButton*                   m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QPushButton*                   m_pBtnTreeViewExpandAll;
    QPushButton*                   m_pBtnTreeViewCollapseAll;
    QLabel*                        m_pLblTestStepCurr;
    QLineEdit*                     m_pEdtTestStepCurr;
    CTestStepAdminObjPoolModel*    m_pTestStepsModel;
    QTreeView*                     m_pTreeViewTestSteps;
    ZS::System::CObjPoolTreeEntry* m_pTreeEntrySelected;

}; // class CWgdtTest

} // namespace GUI

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTestGUI_Wdgt_h
