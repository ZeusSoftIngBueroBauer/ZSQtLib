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

#ifndef ZSPhysValGUI_PhysSizesIdxTreeWdgt_h
#define ZSPhysValGUI_PhysSizesIdxTreeWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSSys/ZSSysCommon.h"

class QLineEdit;
class QModelIndex;
class QPushButton;
class QSplitter;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace PhysVal
{
class CIdxTreePhysSizes;

namespace GUI
{
class CWdgtIdxTreeViewPhysSizes;
class CWdgtIdxTreePhysSizesTableViewBranchContent;
class CTreeViewIdxTreePhysSizes;
class CTableViewIdxTreePhysSizesBranchContent;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtIdxTreePhysSizes : public QWidget
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
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtIdxTreePhysSizes"; }
public: // ctors and dtor
    CWdgtIdxTreePhysSizes(
        CIdxTreePhysSizes* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtIdxTreePhysSizes();
public: // instance methods
    CTreeViewIdxTreePhysSizes* treeView();
    CTableViewIdxTreePhysSizesBranchContent* tableView();
public: // instance methods
    void setIdxTree(CIdxTreePhysSizes* i_pIdxTree);
    CIdxTreePhysSizes* idxTree() { return m_pIdxTree; }
public: // instance methods
    void setViewMode( EViewMode i_viewMode );
    EViewMode viewMode() const { return m_viewMode; }
protected slots:
    void onBtnViewModeClicked( bool i_bChecked );
protected slots:
    void onTreeViewCurrentRowChanged( const QModelIndex& i_modelIdxCurr, const QModelIndex& i_modelIdxPrev );
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CIdxTreePhysSizes* m_pIdxTree;
    QSize m_szBtns;
    EViewMode m_viewMode;
    QVBoxLayout* m_pLytMain;
    QHBoxLayout* m_pLytHeadLine;
    QPushButton* m_pBtnViewMode;
    QLineEdit* m_pEdtBranch;
    QSplitter* m_pSplitter;
    CWdgtIdxTreeViewPhysSizes* m_pWdgtTreeView;
    CWdgtIdxTreePhysSizesTableViewBranchContent* m_pWdgtTableViewBranchContent;

}; // class CWdgtIdxTree

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysSizesIdxTreeWdgt_h
