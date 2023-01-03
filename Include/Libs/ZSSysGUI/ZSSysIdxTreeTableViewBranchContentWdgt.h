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

#ifndef ZSSysGUI_IdxTreeTableViewBranchContentWdgt_h
#define ZSSysGUI_IdxTreeTableViewBranchContentWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSSysGUI/ZSSysIdxTreeModel.h"
#include "ZSSys/ZSSysCommon.h"

class QModelIndex;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CIdxTree;
class CTrcAdminObj;

namespace GUI
{
class CTableViewIdxTreeBranchContent;

//******************************************************************************
class ZSSYSGUIDLL_API CWdgtIdxTreeTableViewBranchContent : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CWdgtIdxTreeTableViewBranchContent"; }
public: // ctors and dtor
    CWdgtIdxTreeTableViewBranchContent(
        CIdxTree* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtIdxTreeTableViewBranchContent();
public: // instance methods
    CTableViewIdxTreeBranchContent* tableView() { return m_pTableViewBranchContent; }
public: // instance methods
    void setIdxTree(CIdxTree* i_pIdxTree);
    CIdxTree* idxTree() { return m_pIdxTree; }
public: // overridables
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree );
    QString getKeyInTreeOfRootEntry() const;
protected slots:
    void onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
    void onBtnSortOrderClicked( bool i_bChecked );
protected slots:
    void onTableViewSortOrderChanged(EIdxTreeSortOrder i_sortOrder);
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CIdxTree* m_pIdxTree;
    QSize m_szBtns;
    QVBoxLayout* m_pLytMain;
    QHBoxLayout* m_pLytHeadLine;
    QPushButton* m_pBtnTreeViewResizeRowsAndColumnsToContents;
    QPushButton* m_pBtnSortOrder;
    CTableViewIdxTreeBranchContent* m_pTableViewBranchContent;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CWdgtIdxTreeTableViewBranchContent

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_IdxTreeTableViewBranchContentWdgt_h
