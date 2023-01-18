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

#ifndef ZSDraw_GraphObjsTreeViewWdgt_h
#define ZSDraw_GraphObjsTreeViewWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/GraphObjs/ZSDrawGraphObjsTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"

class QModelIndex;
class QPushButton;
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
class CTreeViewIdxTreeGraphObjs;

//******************************************************************************
class ZSDRAWDLL_API CWdgtIdxTreeViewGraphObjs : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CWdgtIdxTreeView"; }
public: // ctors and dtor
    CWdgtIdxTreeViewGraphObjs(
        CDrawingScene* i_pDrawingScene,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtIdxTreeViewGraphObjs();
signals:
    void currentRowChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
public: // instance methods
    CTreeViewIdxTreeGraphObjs* treeView() { return m_pTreeView; }
//public: // instance methods
//    void setExcludeLeaves(bool i_bExcludeLeaves);
//    bool areLeavesExcluded() const;
protected slots:
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
    CDrawingScene* m_pDrawingScene;
    QSize m_szBtns;
    QVBoxLayout* m_pLytMain;
    QHBoxLayout* m_pLytHeadLine;
    QPushButton* m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QPushButton* m_pBtnTreeViewExpandAll;
    QPushButton* m_pBtnTreeViewCollapseAll;
    QPushButton* m_pBtnSortOrder;
    CTreeViewIdxTreeGraphObjs* m_pTreeView;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtIdxTreeViewGraphObjs

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjsTreeViewWdgt_h
