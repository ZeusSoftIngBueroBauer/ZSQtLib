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

#ifndef ZSDraw_GraphObjsTreeWdgt_h
#define ZSDraw_GraphObjsTreeWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/TreeView/ZSDrawGraphObjsTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"

class QModelIndex;
class QPushButton;
class QSettings;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}
namespace Draw
{
class CDrawingView;
class CTreeViewGraphObjs;

//******************************************************************************
class ZSDRAWDLL_API CWdgtGraphObjsTree : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CWdgtGraphObjsTree"; }
public: // type definitions and constants
    enum class EViewMode {
        NavPanelOnly           = 0,
        NavPanelAndNodeContent = 1,
        Count,
        Undefined
    };
    static QString viewMode2Str( EViewMode i_eVal, int i_alias = ZS::System::EEnumEntryAliasStrName );
    static QPixmap viewMode2Pixmap( EViewMode i_eVal, const QSize& i_sz = QSize(24,24));
    static EViewMode str2ViewMode( const QString& i_str );
public: // ctors and dtor
    CWdgtGraphObjsTree(
        CDrawingView* i_pDrawingView,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtGraphObjsTree();
signals:
    void viewModeChanged( const QString& i_strViewMode );
    void currentRowChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
public: // instance methods
    void saveState(QSettings& i_settings) const;
    void restoreState(const QSettings& i_settings);
public: // instance methods
    EViewMode viewMode() const { return m_viewMode; }
    CTreeViewGraphObjs* treeView() { return m_pTreeView; }
//public: // instance methods
//    void setExcludeLeaves(bool i_bExcludeLeaves);
//    bool areLeavesExcluded() const;
protected slots:
    void onBtnViewModeClicked( bool i_bChecked );
    void onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
    void onBtnTreeViewExpandAllClicked( bool i_bChecked );
    void onBtnTreeViewCollapseAllClicked( bool i_bChecked );
    void onBtnSortOrderClicked( bool i_bChecked );
protected slots:
    void onTreeViewSortOrderChanged(ZS::System::GUI::EIdxTreeSortOrder i_sortOrder);
    void onTreeViewCurrentRowChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
private: // auxiliary methods (tracing)
    void emit_currentRowChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected: // instance members
    CDrawingView* m_pDrawingView;
    QSize m_szBtns;
    EViewMode m_viewMode;
    QVBoxLayout* m_pLytMain;
    QHBoxLayout* m_pLytHeadLine;
    QPushButton* m_pBtnViewMode;
    QPushButton* m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QPushButton* m_pBtnTreeViewExpandAll;
    QPushButton* m_pBtnTreeViewCollapseAll;
    QPushButton* m_pBtnSortOrder;
    CTreeViewGraphObjs* m_pTreeView;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtGraphObjsTree

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjsTreeWdgt_h
