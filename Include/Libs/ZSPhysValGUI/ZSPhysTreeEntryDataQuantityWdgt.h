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

#ifndef ZSPhysValGUI_PhysTreeEntryDataQuantityWdgt_h
#define ZSPhysValGUI_PhysTreeEntryDataQuantityWdgt_h

#include "ZSPhysValGUI/ZSPhysTreeEntryAbstractWdgt.h"

//class QCheckBox;
//class QComboBox;
//class QLabel;
//class QLineEdit;
//class QSpacerItem;
//class QSplitter;
//class QTableView;
//class QHBoxLayout;
//class QVBoxLayout;

namespace ZS
{
namespace PhysVal
{
namespace GUI
{
//******************************************************************************
class ZSPHYSVALGUIDLL_API CWdgtDataQuantity : public CWdgtAbstractTreeEntry
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CWdgtDataQuantity"; }
public: // ctors and dtor
    CWdgtDataQuantity( CIdxTreePhysSizes* i_pIdxTree, QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtDataQuantity();
public: // overridables
    virtual QString nameSpace() const { return CWdgtDataQuantity::NameSpace(); }
    virtual QString className() const { return CWdgtDataQuantity::ClassName(); }
public: // overridables
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree ) override;

}; // class CWdgtDataQuantity

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysTreeEntryDataQuantityWdgt_h
