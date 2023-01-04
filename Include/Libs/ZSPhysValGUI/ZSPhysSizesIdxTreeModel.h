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

#ifndef ZSPhysValGUI_PhysSizesIdxTreeModel_h
#define ZSPhysValGUI_PhysSizesIdxTreeModel_h

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreePhysSizes;

namespace GUI
{
//******************************************************************************
class ZSPHYSVALGUIDLL_API CModelIdxTreePhysSizes : public ZS::System::GUI::CModelIdxTree
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CModelIdxTreePhysSizes"; }
public: // type definitions and constants
    enum EColumn {
        EColumn0  = CModelIdxTree::EColumnCount + 0,
        EColumnCount
    };
public: // ctors and dtor
    CModelIdxTreePhysSizes(
        CIdxTreePhysSizes* i_pIdxTree = nullptr,
        QObject* i_pObjParent = nullptr );
    virtual ~CModelIdxTreePhysSizes();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    CIdxTreePhysSizes* idxTree();
public: // instance methods
    //virtual void reset();
public: // overridables of base class QAbstractItemModel
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const override;

}; // class CModelIdxTreePhysSizes

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysSizesIdxTreeModel_h
