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

#ifndef ZSPhysValGUI_PhysSizesTreeViewWdgt_h
#define ZSPhysValGUI_PhysSizesTreeViewWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSSys/ZSSysAux.h"

class QModelIndex;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace PhysVal
{
class CIdxTreeUnits;

namespace GUI
{
class CTreeViewPhysSizes;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtTreeViewPhysSizes : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtTreeViewPhysSizes"; }
public: // ctors and dtor
    CWdgtTreeViewPhysSizes(
        CIdxTreeUnits* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtTreeViewPhysSizes();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    void currentRowChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
public: // instance methods
    CTreeViewPhysSizes* treeView() { return m_pTreeView; }
public: // instance methods
    void setIdxTree(CIdxTreeUnits* i_pIdxTree);
    CIdxTreeUnits* idxTree() { return m_pIdxTree; }
protected slots:
    void onTreeViewExpanded( const QModelIndex& i_modelIdx );
    void onTreeViewCurrentRowChanged(const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected slots:
    void onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
    void onBtnTreeViewExpandAllClicked( bool i_bChecked );
    void onBtnTreeViewCollapseAllClicked( bool i_bChecked );
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CIdxTreeUnits* m_pIdxTree;
    QSize m_szBtns;
    QVBoxLayout* m_pLytMain;
    QHBoxLayout* m_pLytHeadLine;
    QPushButton* m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QPushButton* m_pBtnTreeViewExpandAll;
    QPushButton* m_pBtnTreeViewCollapseAll;
    CTreeViewPhysSizes* m_pTreeView;

}; // class CWdgtTreeViewPhysSizes

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysSizesTreeViewWdgt_h
