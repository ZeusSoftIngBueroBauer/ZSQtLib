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

#ifndef ZSPhysValGUI_PhysSizesIdxTreeTableViewBranchContentWdgt_h
#define ZSPhysValGUI_PhysSizesIdxTreeTableViewBranchContentWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"

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
class CTableViewIdxTreePhysSizesBranchContent;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtIdxTreePhysSizesTableViewBranchContent : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CWdgtIdxTreePhysSizesTableViewBranchContent"; }
public: // ctors and dtor
    CWdgtIdxTreePhysSizesTableViewBranchContent(
        CIdxTreePhysSizes* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags() );
    virtual ~CWdgtIdxTreePhysSizesTableViewBranchContent();
public: // instance methods
    CTableViewIdxTreePhysSizesBranchContent* tableView() { return m_pTableViewBranchContent; }
public: // instance methods
    void setIdxTree(CIdxTreePhysSizes* i_pIdxTree);
    CIdxTreePhysSizes* idxTree() { return m_pIdxTree; }
public: // overridables
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree );
    QString getKeyInTreeOfRootEntry() const;
protected slots:
    void onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CIdxTreePhysSizes* m_pIdxTree;
    QSize m_szBtns;
    QVBoxLayout* m_pLytMain;
    QHBoxLayout* m_pLytHeadLine;
    QPushButton* m_pBtnTreeViewResizeRowsAndColumnsToContents;
    CTableViewIdxTreePhysSizesBranchContent* m_pTableViewBranchContent;

}; // class CWdgtIdxTreePhysSizesTableViewBranchContent

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysSizesIdxTreeTableViewBranchContentWdgt_h
