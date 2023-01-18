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

#include "ZSPhysVal/ZSPhysTreeEntryGrpBase.h"
#include "ZSPhysVal/ZSPhysTreeEntryUnitBase.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitsTreeEntryGrpBase : CIdxTreeEntry
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a unit group.

    The constructor will add itself as a child of the root node
    to the given index tree.

    @param i_pIdxTree
    @param i_classType
        [PhysSize, Ratios, UserDefinedQuantities]
    @param i_strName
        e.g. "Time", "Power", "Voltage", "Current", ...
*/
CUnitsTreeEntryGrpBase::CUnitsTreeEntryGrpBase(
    CIdxTreeUnits* i_pIdxTree,
    EUnitClassType i_classType,
    const QString& i_strName ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Branch, i_strName),
    m_classType(EUnitClassType::Undefined),
    m_hshpUnitsBySymbol(),
    m_hshpUnitsByPrefix()
{
    i_pIdxTree->add(this);
}

//------------------------------------------------------------------------------
/*! @brief Creates a unit group.

    The constructor will add itself to an index tree of physical sizes
    as a child of the passed parent branch.

    @param i_pParentBranch
    @param i_classType
        [PhysSize, Ratios, UserDefinedQuantities]
    @param i_strName
        e.g. "Time", "Power", "Voltage", "Current", ...
*/
CUnitsTreeEntryGrpBase::CUnitsTreeEntryGrpBase(
    CIdxTreeEntry* i_pParentBranch,
    EUnitClassType i_classType,
    const QString& i_strName ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EIdxTreeEntryType::Branch, i_strName),
    m_classType(i_classType),
    m_hshpUnitsBySymbol(),
    m_hshpUnitsByPrefix()
{
    i_pParentBranch->tree()->add(this, i_pParentBranch);
}

//------------------------------------------------------------------------------
/*! @brief Destroys the unit group.

    The instance will be removed from the index tree.

    If the constructor created an index tree with the default name
    the index tree will be released by the destructor.
*/
CUnitsTreeEntryGrpBase::~CUnitsTreeEntryGrpBase()
//------------------------------------------------------------------------------
{
    m_classType = static_cast<EUnitClassType>(0);
    //m_hshpUnitsBySymbol.clear();
    //m_hshpUnitsByPrefix.clear();

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitsTreeEntryGrpBase::operator == ( const CUnitsTreeEntryGrpBase& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    if( this != &i_physsizeOther )
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
bool CUnitsTreeEntryGrpBase::operator != ( const CUnitsTreeEntryGrpBase& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physsizeOther );
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CUnitsTreeEntryGrpBase::classType2Str() const
//------------------------------------------------------------------------------
{
    return CEnumUnitClassType(m_classType).toString();
}

/*=============================================================================
public: // overridables of base class CIdxTreeEntry
=============================================================================*/

//------------------------------------------------------------------------------
int CUnitsTreeEntryGrpBase::addChild( CIdxTreeEntry* i_pChildTreeEntry )
//------------------------------------------------------------------------------
{
    int idxInParentBranch = CIdxTreeEntry::addChild(i_pChildTreeEntry);

    // The name and symbol got to be unique.
    // If the name would not have been unique the addChild method above
    // would have thrown an exception. Here we check whether the symbol is unique.
    if( i_pChildTreeEntry->isLeave() ) {
        CUnitsTreeEntryUnitBase* pUnit = dynamic_cast<CUnitsTreeEntryUnitBase*>(i_pChildTreeEntry);
        if( pUnit == nullptr ) {
            throw CException(
                __FILE__, __LINE__, EResultInternalProgramError,
                "Only units can be leaves in the index tree");
        }
        if( !pUnit->symbol().isEmpty() ) {
            if( findUnitBySymbol(pUnit->symbol()) != nullptr ) {
                throw CException(
                    __FILE__ ,__LINE__, EResultObjAlreadyInList,
                    "A unit with symbol " + pUnit->symbol() +
                    " already belongs to unit group " + keyInTree());
            }
            m_hshpUnitsBySymbol[pUnit->symbol()] = pUnit;
        }
        if( !pUnit->factorPrefix().isEmpty() ) {
            if( findUnitByFactorPrefix(pUnit->factorPrefix()) != nullptr ) {
                throw CException(
                    __FILE__ ,__LINE__, EResultObjAlreadyInList,
                    "A unit with factor prefix " + pUnit->factorPrefix() +
                    " already belongs to unit group " + keyInTree());
            }
            m_hshpUnitsByPrefix[pUnit->factorPrefix()] = pUnit;
        }
    }
    return idxInParentBranch;

} // addChild

/*=============================================================================
public: // instance methods
=============================================================================*/

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase* CUnitsTreeEntryGrpBase::findUnit( const QString& i_strSymbolOrName ) const
//------------------------------------------------------------------------------
{
    CUnitsTreeEntryUnitBase* pUnit = findUnitBySymbol(i_strSymbolOrName);
    if( pUnit == nullptr )
    {
        pUnit = findUnitByName(i_strSymbolOrName);
    }
    if( pUnit == nullptr )
    {
        pUnit = findUnitByFactorPrefix(i_strSymbolOrName);
    }
    return pUnit;
}

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase* CUnitsTreeEntryGrpBase::findUnitByName( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryUnitBase*>(find(EIdxTreeEntryType::Leave, i_strName));
}

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase* CUnitsTreeEntryGrpBase::findUnitBySymbol( const QString& i_strSymbol ) const
//------------------------------------------------------------------------------
{
    return m_hshpUnitsBySymbol.value(i_strSymbol, nullptr);
}

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase* CUnitsTreeEntryGrpBase::findUnitByFactorPrefix( const QString& i_strPrefix ) const
//------------------------------------------------------------------------------
{
    return m_hshpUnitsByPrefix.value(i_strPrefix, nullptr);
}
