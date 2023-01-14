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

#ifndef ZSPhysVal_PhysTreeEntryGrpBase_h
#define ZSPhysVal_PhysTreeEntryGrpBase_h

#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreeUnits;
class CUnitsTreeEntryUnitBase;

//******************************************************************************
class ZSPHYSVALDLL_API CUnitsTreeEntryGrpBase : public ZS::System::CIdxTreeEntry
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CUnitsTreeEntryGrpBase"; }
public: // ctors and dtor
    CUnitsTreeEntryGrpBase(
        CIdxTreeUnits* i_pIdxTree,
        EUnitClassType i_classType,
        const QString& i_strName );
    CUnitsTreeEntryGrpBase(
        CIdxTreeEntry* i_pParentBranch,
        EUnitClassType i_classType,
        const QString& i_strName );
    virtual ~CUnitsTreeEntryGrpBase();
public: // operators
    bool operator == ( const CUnitsTreeEntryGrpBase& i_other ) const;
    bool operator != ( const CUnitsTreeEntryGrpBase& i_other ) const;
public: // instance methods
    EUnitClassType classType() const { return m_classType; }
    QString classType2Str() const;
public: // overridables of base class CIdxTreeEntry
    virtual int addChild( CIdxTreeEntry* i_pChildTreeEntry ) override;
public: // instance methods
    virtual CUnitsTreeEntryUnitBase* findUnit( const QString& i_strSymbolOrName ) const;
    virtual CUnitsTreeEntryUnitBase* findUnitByName( const QString& i_strName ) const;
    virtual CUnitsTreeEntryUnitBase* findUnitBySymbol( const QString& i_strSymbol ) const;
    virtual CUnitsTreeEntryUnitBase* findUnitByFactorPrefix( const QString& i_strPrefix ) const;
protected: // instance members
    /*!< Class type of the child nodes.
         A group may be the parent of either
         Ratio, Quantity or PhysUnit entries. */
    EUnitClassType m_classType;
    /*!< Key is symbol of unit (e.g. "mW", "GW"). */
    QHash<QString, CUnitsTreeEntryUnitBase*> m_hshpUnitsBySymbol;
    /*!< Key is factor prefix of unit (e.g. "k", "G"). */
    QHash<QString, CUnitsTreeEntryUnitBase*> m_hshpUnitsByPrefix;

}; // class CUnitsTreeEntryGrpBase

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysTreeEntryGrpBase_h
