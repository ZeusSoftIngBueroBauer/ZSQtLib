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

#ifndef ZSDraw_GraphObjsTreeWdgt_h
#define ZSDraw_GraphObjsTreeWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDraw/ZSDrawDllMain.h"
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
namespace Draw
{
class CModelIdxTreeGraphObjs;
class CTreeViewIdxTreeGraphObjs;

//******************************************************************************
class ZSDRAWDLL_API CWdgtIdxTreeGraphObjs : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CWdgtIdxTreeGraphObjs"; }
public: // ctors and dtor
    CWdgtIdxTreeGraphObjs(
        CModelIdxTreeGraphObjs* i_pModel,
        QWidget*                i_pWdgtParent = nullptr,
        Qt::WindowFlags         i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtIdxTreeGraphObjs();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    CModelIdxTreeGraphObjs* model() { return m_pModel; }
    CTreeViewIdxTreeGraphObjs* treeView() { return m_pTreeView; }
protected slots:
    void onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
    void onBtnTreeViewExpandAllClicked( bool i_bChecked );
    void onBtnTreeViewCollapseAllClicked( bool i_bChecked );
    void onBtnSortOrderClicked( bool i_bChecked );
protected slots:
    void onTreeViewSelectionModelCurrentRowChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected: // instance members
    QSize                      m_szBtns;
    QVBoxLayout*               m_pLytMain;
    QHBoxLayout*               m_pLytHeadLine;
    QPushButton*               m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QPushButton*               m_pBtnTreeViewExpandAll;
    QPushButton*               m_pBtnTreeViewCollapseAll;
    QPushButton*               m_pBtnSortOrder;
    QSpacerItem*               m_pSpcHeadLine;
    CModelIdxTreeGraphObjs*    m_pModel;
    QSplitter*                 m_pSplitter;
    CTreeViewIdxTreeGraphObjs* m_pTreeView;
    ZS::Trace::CTrcAdminObj*   m_pTrcAdminObj;

}; // class CWdgtIdxTreeGraphObjs

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjsTreeWdgt_h
