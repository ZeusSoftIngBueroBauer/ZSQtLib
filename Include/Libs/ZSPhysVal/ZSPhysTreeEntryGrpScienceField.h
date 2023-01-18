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

#ifndef ZSPhysVal_PhysTreeEntryGrpScienceField_h
#define ZSPhysVal_PhysTreeEntryGrpScienceField_h

#include "ZSPhysVal/ZSPhysTreeEntryGrpBase.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreeUnits;

//******************************************************************************
class ZSPHYSVALDLL_API CUnitsTreeEntryGrpScienceField : public CUnitsTreeEntryGrpBase
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnitsTreeEntryGrpScienceField"; }
public: // ctors and dtor
    CUnitsTreeEntryGrpScienceField(CIdxTreeUnits* i_pIdxTree, EPhysScienceField i_scienceField);
    CUnitsTreeEntryGrpScienceField(ZS::System::CIdxTreeEntry* i_pParentBranch, EPhysScienceField i_scienceField);
    virtual ~CUnitsTreeEntryGrpScienceField();
public: // overridables of base class CUnitsTreeEntryGrpBase
    virtual CUnitsTreeEntryUnitBase* findUnit( const QString& i_strSymbolOrName ) const override;
    virtual CUnitsTreeEntryUnitBase* findUnitByName( const QString& i_strName ) const override;
    virtual CUnitsTreeEntryUnitBase* findUnitBySymbol( const QString& i_strSymbol ) const override;
    virtual CUnitsTreeEntryUnitBase* findUnitByFactorPrefix( const QString& i_strPrefix ) const override;

}; // class CUnitsTreeEntryGrpScienceField

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysTreeEntryGrpScienceField_h
