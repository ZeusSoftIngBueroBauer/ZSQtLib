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
    m_hshpUnitsByPrefix(),
    m_arRefVals(),
    m_hshRefValIdxsByName(),
    m_hshRefValNamesByIdx()
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
    m_hshpUnitsByPrefix(),
    m_arRefVals(),
    m_hshRefValIdxsByName(),
    m_hshRefValNamesByIdx()
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
    //m_arRefVals.clear();
    //m_hshRefValIdxsByName.clear();
    //m_hshRefValNamesByIdx.clear();

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

/*=============================================================================
public: // overidables
=============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets a reference value with the given name to the group.

    If a reference value with the same name is already existing the existing
    reference value will be replaced by the content of the given value.

    Reference values can be used for different purposes.
    - For example as reference values for external unit conversion functions
      (conversions to units of other physical sizes).
    - Or just to store a value and retrieve them if needed by its name.

    Reference values may be indicating in the corrsponding unit widgets for
    debugging and visualization purposes.

    @param [in] i_strName
        Name of the reference value. The name must be unique within this group.
    @param [in] i_physVal
        Reference value to be set.

    @return Index of the value in the list of the reference values.
*/
int CUnitsTreeEntryGrpBase::setReferenceValue(
    const QString& i_strName, const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    int idx = m_hshRefValIdxsByName.value(i_strName, -1);
    bool bChanged = false;
    if( idx < 0 ) {
        // New Reference value
        idx = m_arRefVals.size();
        m_hshRefValIdxsByName[i_strName] = idx;
        m_hshRefValNamesByIdx[idx] = i_strName;
        m_arRefVals.append(i_physVal);
        bChanged = true;
    }
    else if( m_arRefVals[idx] != i_physVal ) {
        // Replace existing
        m_arRefVals[idx] = i_physVal;
        bChanged = true;
    }
    if( bChanged ) {
        m_pTree->onTreeEntryChanged(this);
    }
    return idx;
}

//------------------------------------------------------------------------------
int CUnitsTreeEntryGrpBase::getReferenceValuesCount() const
//------------------------------------------------------------------------------
{
    return m_arRefVals.size();
}

//------------------------------------------------------------------------------
CPhysVal CUnitsTreeEntryGrpBase::getReferenceValue( int i_idx ) const
//------------------------------------------------------------------------------
{
    return m_arRefVals[i_idx];
}

//------------------------------------------------------------------------------
QString CUnitsTreeEntryGrpBase::getReferenceValueName( int i_idx ) const
//------------------------------------------------------------------------------
{
    return m_hshRefValNamesByIdx[i_idx];
}

//------------------------------------------------------------------------------
CPhysVal CUnitsTreeEntryGrpBase::getReferenceValue( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    if( !m_hshRefValIdxsByName.contains(i_strName) ) {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strName);
    }
    int idx = m_hshRefValIdxsByName[i_strName];
    return m_arRefVals[idx];
}
