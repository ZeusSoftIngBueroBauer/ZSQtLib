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

#ifndef ZSSysGUI_IdxTreeWdgt_h
#define ZSSysGUI_IdxTreeWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysCommon.h"

class QLineEdit;
class QModelIndex;
class QPushButton;
class QSpacerItem;
class QSplitter;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}
namespace System
{
namespace GUI
{
class CModelIdxTree;
class CTreeViewIdxTree;
class CTableViewIdxTreeBranchContent;

//******************************************************************************
class ZSSYSGUIDLL_API CWdgtIdxTree : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum class EViewMode {
        NavPanelOnly             = 0,
        NavPanelAndBranchContent = 1,
        Count,
        Undefined
    };
    static QString viewMode2Str( EViewMode i_eVal, int i_alias = ZS::System::EEnumEntryAliasStrName );
    static QPixmap viewMode2Pixmap( EViewMode i_eVal, const QSize& i_sz = QSize(24,24));
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CWdgtIdxTree"; }
public: // ctors and dtor
    CWdgtIdxTree(
        CModelIdxTree*  i_pModel,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags(),
        int             i_iTrcDetailLevel = ZS::Trace::ETraceDetailLevelNone );
    virtual ~CWdgtIdxTree();
signals:
    void traceMethodDetailLevelIdxTreeChanged( ZS::Trace::ETraceDetailLevel i_mthTrcDetailLevel );
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    CModelIdxTree* model() { return m_pModel; }
    CTreeViewIdxTree* treeView() { return m_pTreeView; }
    CTableViewIdxTreeBranchContent* tableView() { return m_pTableViewBranchContent; }
public: // instance methods
    void setViewMode( EViewMode i_viewMode );
    EViewMode viewMode() const { return m_viewMode; }
protected slots:
    void onBtnViewModeClicked( bool i_bChecked );
    void onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
    void onBtnTreeViewExpandAllClicked( bool i_bChecked );
    void onBtnTreeViewCollapseAllClicked( bool i_bChecked );
    void onBtnSortOrderClicked( bool i_bChecked );
protected slots:
    void onTreeViewSelectionModelCurrentRowChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected slots:
    void onTrcAdminObjChanged( QObject* i_pTrcAdminObj );
protected: // instance members
    EViewMode                       m_viewMode;
    QSize                           m_szBtns;
    QVBoxLayout*                    m_pLytMain;
    QHBoxLayout*                    m_pLytHeadLine;
    QPushButton*                    m_pBtnViewMode;
    QPushButton*                    m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QPushButton*                    m_pBtnTreeViewExpandAll;
    QPushButton*                    m_pBtnTreeViewCollapseAll;
    QPushButton*                    m_pBtnSortOrder;
    QLineEdit*                      m_pEdtBranch;
    QSpacerItem*                    m_pSpcHeadLine;
    CModelIdxTree*                  m_pModel;
    QSplitter*                      m_pSplitter;
    CTreeViewIdxTree*               m_pTreeView;
    CTableViewIdxTreeBranchContent* m_pTableViewBranchContent;
    int                             m_iTrcDetailLevel;    /*!< Trace detail level for method tracing.
                                                               Trace output may not be controlled by trace admin objects
                                                               if the index tree belongs the trace server. */
    ZS::Trace::CTrcAdminObj*        m_pTrcAdminObj;       /*!< Trace admin object to control trace outputs of the class.
                                                               The object will not be created if the index tree's belongs to the trace server. */

}; // class CWdgtIdxTree

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_IdxTreeWdgt_h
