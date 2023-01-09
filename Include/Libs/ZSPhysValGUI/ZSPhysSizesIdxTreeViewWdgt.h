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

#ifndef ZSPhysValGUI_PhysSizesIdxTreeViewWdgt_h
#define ZSPhysValGUI_PhysSizesIdxTreeViewWdgt_h

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
class CIdxTreePhysSizes;

namespace GUI
{
class CModelIdxTreePhysSizes;
class CTreeViewIdxTreePhysSizes;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtIdxTreeViewPhysSizes : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtIdxTreeViewPhysSizes"; }
public: // ctors and dtor
    CWdgtIdxTreeViewPhysSizes(
        CIdxTreePhysSizes* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtIdxTreeViewPhysSizes();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    void currentRowChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
public: // instance methods
    CTreeViewIdxTreePhysSizes* treeView() { return m_pTreeView; }
public: // instance methods
    void setIdxTree(CIdxTreePhysSizes* i_pIdxTree);
    CIdxTreePhysSizes* idxTree() { return m_pIdxTree; }
protected slots:
    void onTreeViewExpanded( const QModelIndex& i_modelIdx );
protected slots:
    void onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
    void onBtnTreeViewExpandAllClicked( bool i_bChecked );
    void onBtnTreeViewCollapseAllClicked( bool i_bChecked );
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CIdxTreePhysSizes* m_pIdxTree;
    QSize m_szBtns;
    QVBoxLayout* m_pLytMain;
    QHBoxLayout* m_pLytHeadLine;
    QPushButton* m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QPushButton* m_pBtnTreeViewExpandAll;
    QPushButton* m_pBtnTreeViewCollapseAll;
    CTreeViewIdxTreePhysSizes* m_pTreeView;

}; // class CWdgtIdxTreeViewPhysSizes

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysSizesIdxTreeViewWdgt_h
