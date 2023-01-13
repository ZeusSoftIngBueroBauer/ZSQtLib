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

#include "ZSPhysVal/ZSPhysTreeEntryGrpScienceField.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitsTreeEntryGrpScienceField : public ZS::System::CIdxTreeEntry
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of a physical science field.

    The constructor will add itself to an index tree of physical sizes.

    @param i_pIdxTree [in]
        Pointer to index tree the science field should be added to.
    @param i_scienceField [in]
        e.g. Kinematics, Electricity, Geometry, ...
*/
CUnitsTreeEntryGrpScienceField::CUnitsTreeEntryGrpScienceField(
    CIdxTreeUnits* i_pIdxTree, EPhysScienceField i_scienceField ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpBase(
        i_pIdxTree, EUnitClassType::PhysSize,
        CEnumPhysScienceField(i_scienceField).toString())
{
}

//------------------------------------------------------------------------------
/*! @brief Creates an instance of a physical science field.

    The constructor will add itself to the index tree as a child of
    the given parent branch.

    @param i_pParentBranch [in]
        Pointer to parent branch the science field should be
        added to as a child.
    @param i_scienceField [in]
        e.g. Kinematics, Electricity, Geometry, ...
*/
CUnitsTreeEntryGrpScienceField::CUnitsTreeEntryGrpScienceField(
    CIdxTreeEntry* i_pParentBranch, EPhysScienceField i_scienceField ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpBase(
        i_pParentBranch, EUnitClassType::PhysSize,
        CEnumPhysScienceField(i_scienceField).toString())
{
}

//------------------------------------------------------------------------------
/*! @brief Destroys the science field instance.

    The instance will be removed from the index tree of physical sizes.

    If the constructor created an index tree with the default name
    the index tree will be released by the destructor.
*/
CUnitsTreeEntryGrpScienceField::~CUnitsTreeEntryGrpScienceField()
//------------------------------------------------------------------------------
{
}

/*=============================================================================
public: // overridables of base class CUnitsTreeEntryGrpBase
=============================================================================*/

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase* CUnitsTreeEntryGrpScienceField::findUnit( const QString& i_strSymbolOrName ) const
//------------------------------------------------------------------------------
{
    CUnitsTreeEntryUnitBase* pUnit = findUnitBySymbol(i_strSymbolOrName);
    if( pUnit == nullptr ) {
        pUnit = findUnitByName(i_strSymbolOrName);
    }
    if( pUnit == nullptr ) {
        pUnit = findUnitByFactorPrefix(i_strSymbolOrName);
    }
    return pUnit;
}

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase* CUnitsTreeEntryGrpScienceField::findUnitByName( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    CUnitsTreeEntryUnitBase* pUnit = nullptr;
    for( const CIdxTreeEntry* pTreeEntryChild : m_arpTreeEntries ) {
        const CUnitsTreeEntryGrpPhysUnits* pPhysSize =
            dynamic_cast<const CUnitsTreeEntryGrpPhysUnits*>(pTreeEntryChild);
        if( pPhysSize != nullptr ) {
            pUnit = pPhysSize->findUnitByName(i_strName);
            if( pUnit != nullptr) {
                break;
            }
        }
    }
    return pUnit;
}

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase* CUnitsTreeEntryGrpScienceField::findUnitBySymbol( const QString& i_strSymbol ) const
//------------------------------------------------------------------------------
{
    CUnitsTreeEntryUnitBase* pUnit = nullptr;
    for( const CIdxTreeEntry* pTreeEntryChild : m_arpTreeEntries ) {
        const CUnitsTreeEntryGrpPhysUnits* pPhysSize =
            dynamic_cast<const CUnitsTreeEntryGrpPhysUnits*>(pTreeEntryChild);
        if( pPhysSize != nullptr ) {
            pUnit = pPhysSize->findUnitBySymbol(i_strSymbol);
            if( pUnit != nullptr) {
                break;
            }
        }
    }
    return pUnit;
}

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase* CUnitsTreeEntryGrpScienceField::findUnitByFactorPrefix( const QString& i_strPrefix ) const
//------------------------------------------------------------------------------
{
    CUnitsTreeEntryUnitBase* pUnit = nullptr;
    for( const CIdxTreeEntry* pTreeEntryChild : m_arpTreeEntries ) {
        const CUnitsTreeEntryGrpPhysUnits* pPhysSize =
            dynamic_cast<const CUnitsTreeEntryGrpPhysUnits*>(pTreeEntryChild);
        if( pPhysSize != nullptr ) {
            pUnit = pPhysSize->findUnitByFactorPrefix(i_strPrefix);
            if( pUnit != nullptr) {
                break;
            }
        }
    }
    return pUnit;
}
