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

#ifndef ZSPhysValGUI_PhysSizesIdxTreeTableViewBranchContent_h
#define ZSPhysValGUI_PhysSizesIdxTreeTableViewBranchContent_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qtableview.h>
#endif

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreePhysSizes;

namespace GUI
{
class CModelIdxTreePhysSizesBranchContent;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CTableViewIdxTreePhysSizesBranchContent : public QTableView
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CTableViewPhysSizesIdxTreeBranchContent"; }
public: // ctors and dtor
    CTableViewIdxTreePhysSizesBranchContent( CIdxTreePhysSizes* i_pIdxTree, QWidget* i_pWdgtParent = nullptr );
    virtual ~CTableViewIdxTreePhysSizesBranchContent();
public: // overridables
    virtual QString nameSpace() const { return CTableViewIdxTreePhysSizesBranchContent::NameSpace(); }
    virtual QString className() const { return CTableViewIdxTreePhysSizesBranchContent::ClassName(); }
public: // instance methods
    void setIdxTree(CIdxTreePhysSizes* i_pIdxTree);
    CIdxTreePhysSizes* idxTree() { return m_pIdxTree; }
public: // overridables
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree );
    QString getKeyInTreeOfRootEntry() const;
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CIdxTreePhysSizes* m_pIdxTree;
    CModelIdxTreePhysSizesBranchContent* m_pModel;
    //QString m_strKeyInTreeOfRootEntry;
    QModelIndex m_modelIdxSelectedOnMousePressEvent;
    QModelIndex m_modelIdxSelectedOnMouseReleaseEvent;

}; // class CTableViewIdxTreePhysSizesBranchContent

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysSizesIdxTreeTableViewBranchContent_h
