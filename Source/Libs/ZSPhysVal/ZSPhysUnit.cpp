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

#include "ZSPhysVal/ZSPhysUnit.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysTreeEntryUnitQuantity.h"
#include "ZSPhysVal/ZSPhysTreeEntryUnitRatio.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
Exported methods
*******************************************************************************/

//------------------------------------------------------------------------------
bool ZS::PhysVal::areOfSameUnitGroup( const CUnit& i_unit1, const CUnit& i_unit2 )
//------------------------------------------------------------------------------
{
    bool bOk = false;
    if( !i_unit1.isValidGroup() && !i_unit2.isValidGroup() ) {
        bOk = true;
    } else if( i_unit1.isValidGroup() && i_unit2.isValidGroup() ) {
        bOk = (i_unit1.groupPath() == i_unit2.groupPath());
    }
    return bOk;
}


/*******************************************************************************
class CUnit
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnit::CUnit() :
//------------------------------------------------------------------------------
    m_pTreeEntry(nullptr),
    m_classType(EUnitClassType::Undefined),
    m_strKeyInTree(),
    m_strUnitName(),
    m_strGrpPath(),
    m_strSymbol()
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnit* i_pUnit) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pUnit == nullptr ? nullptr : i_pUnit->m_pTreeEntry),
    m_classType(i_pUnit == nullptr ? EUnitClassType::Undefined : i_pUnit->m_classType),
    m_strKeyInTree(i_pUnit == nullptr ? "" : i_pUnit->m_strKeyInTree),
    m_strUnitName(i_pUnit == nullptr ? "" : i_pUnit->m_strUnitName),
    m_strGrpPath(i_pUnit == nullptr ? "" : i_pUnit->m_strGrpPath),
    m_strSymbol(i_pUnit == nullptr ? "" : i_pUnit->m_strSymbol)
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnit* i_pUnit) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pUnit == nullptr ? nullptr : i_pUnit->m_pTreeEntry),
    m_classType(i_pUnit == nullptr ? EUnitClassType::Undefined : i_pUnit->m_classType),
    m_strKeyInTree(i_pUnit == nullptr ? "" : i_pUnit->m_strKeyInTree),
    m_strUnitName(i_pUnit == nullptr ? "" : i_pUnit->m_strUnitName),
    m_strGrpPath(i_pUnit == nullptr ? "" : i_pUnit->m_strGrpPath),
    m_strSymbol(i_pUnit == nullptr ? "" : i_pUnit->m_strSymbol)
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_unit.m_pTreeEntry),
    m_classType(i_unit.m_classType),
    m_strKeyInTree(i_unit.m_strKeyInTree),
    m_strUnitName(i_unit.m_strUnitName),
    m_strGrpPath(i_unit.m_strGrpPath),
    m_strSymbol(i_unit.m_strSymbol)
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_unit.m_pTreeEntry),
    m_classType(i_unit.m_classType),
    m_strKeyInTree(i_unit.m_strKeyInTree),
    m_strUnitName(i_unit.m_strUnitName),
    m_strGrpPath(i_unit.m_strGrpPath),
    m_strSymbol(i_unit.m_strSymbol)
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnitsTreeEntryGrpBase* i_pGrpEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pGrpEntry),
    m_classType(i_pGrpEntry == nullptr ? EUnitClassType::Undefined : i_pGrpEntry->classType()),
    m_strKeyInTree(i_pGrpEntry == nullptr ? "" : i_pGrpEntry->keyInTree()),
    m_strUnitName(),
    m_strGrpPath(i_pGrpEntry == nullptr ? "" : i_pGrpEntry->path()),
    m_strSymbol()
{
    if( i_pGrpEntry != nullptr && i_pGrpEntry->classType() == EUnitClassType::PhysSize ) {
        CUnitsTreeEntryGrpPhysUnits* pPhysSize =
            dynamic_cast<CUnitsTreeEntryGrpPhysUnits*>(i_pGrpEntry);
        if( pPhysSize != nullptr && pPhysSize->getSIUnit() != nullptr ) {
            m_pTreeEntry = pPhysSize->getSIUnit();
            m_strKeyInTree = m_pTreeEntry->keyInTree();
            m_strUnitName = pPhysSize->getSIUnit()->name();
            m_strSymbol = pPhysSize->getSIUnit()->symbol();
        }
    }
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnitsTreeEntryGrpBase* i_pGrpEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitsTreeEntryGrpBase*>(i_pGrpEntry)),
    m_classType(i_pGrpEntry == nullptr ? EUnitClassType::Undefined : i_pGrpEntry->classType()),
    m_strKeyInTree(i_pGrpEntry == nullptr ? "" : i_pGrpEntry->keyInTree()),
    m_strUnitName(),
    m_strGrpPath(i_pGrpEntry == nullptr ? "" : i_pGrpEntry->path()),
    m_strSymbol()
{
    if( i_pGrpEntry != nullptr && i_pGrpEntry->classType() == EUnitClassType::PhysSize ) {
        const CUnitsTreeEntryGrpPhysUnits* pPhysSize =
            dynamic_cast<const CUnitsTreeEntryGrpPhysUnits*>(i_pGrpEntry);
        if( pPhysSize != nullptr && pPhysSize->getSIUnit() != nullptr ) {
            m_pTreeEntry = pPhysSize->getSIUnit();
            m_strKeyInTree = m_pTreeEntry->keyInTree();
            m_strUnitName = pPhysSize->getSIUnit()->name();
            m_strSymbol = pPhysSize->getSIUnit()->symbol();
        }
    }
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnitsTreeEntryGrpBase& i_grpEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(&i_grpEntry),
    m_classType(i_grpEntry.classType()),
    m_strKeyInTree(i_grpEntry.keyInTree()),
    m_strUnitName(),
    m_strGrpPath(i_grpEntry.path()),
    m_strSymbol()
{
    if( i_grpEntry.classType() == EUnitClassType::PhysSize ) {
        const CUnitsTreeEntryGrpPhysUnits* pPhysSize =
            dynamic_cast<CUnitsTreeEntryGrpPhysUnits*>(&i_grpEntry);
        if( pPhysSize != nullptr && pPhysSize->getSIUnit() != nullptr ) {
            m_pTreeEntry = pPhysSize->getSIUnit();
            m_strKeyInTree = m_pTreeEntry->keyInTree();
            m_strUnitName = pPhysSize->getSIUnit()->name();
            m_strSymbol = pPhysSize->getSIUnit()->symbol();
        }
    }
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnitsTreeEntryGrpBase& i_grpEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitsTreeEntryGrpBase*>(&i_grpEntry)),
    m_classType(i_grpEntry.classType()),
    m_strKeyInTree(i_grpEntry.keyInTree()),
    m_strUnitName(),
    m_strGrpPath(i_grpEntry.path()),
    m_strSymbol()
{
    if( i_grpEntry.classType() == EUnitClassType::PhysSize ) {
        const CUnitsTreeEntryGrpPhysUnits* pPhysSize =
            dynamic_cast<const CUnitsTreeEntryGrpPhysUnits*>(&i_grpEntry);
        if( pPhysSize != nullptr && pPhysSize->getSIUnit() != nullptr ) {
            m_pTreeEntry = pPhysSize->getSIUnit();
            m_strKeyInTree = m_pTreeEntry->keyInTree();
            m_strUnitName = pPhysSize->getSIUnit()->name();
            m_strSymbol = pPhysSize->getSIUnit()->symbol();
        }
    }
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnitsTreeEntryGrpPhysUnits* i_pGrpEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pGrpEntry),
    m_classType(i_pGrpEntry == nullptr ? EUnitClassType::Undefined : i_pGrpEntry->classType()),
    m_strKeyInTree(i_pGrpEntry == nullptr ? "" : i_pGrpEntry->keyInTree()),
    m_strUnitName(),
    m_strGrpPath(i_pGrpEntry == nullptr ? "" : i_pGrpEntry->path()),
    m_strSymbol()
{
    if( i_pGrpEntry != nullptr && i_pGrpEntry->getSIUnit() != nullptr ) {
        m_pTreeEntry = i_pGrpEntry->getSIUnit();
        m_strKeyInTree = m_pTreeEntry->keyInTree();
        m_strUnitName = i_pGrpEntry->getSIUnit()->name();
        m_strSymbol = i_pGrpEntry->getSIUnit()->symbol();
    }
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnitsTreeEntryGrpPhysUnits* i_pGrpEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitsTreeEntryGrpPhysUnits*>(i_pGrpEntry)),
    m_classType(i_pGrpEntry == nullptr ? EUnitClassType::Undefined : i_pGrpEntry->classType()),
    m_strKeyInTree(i_pGrpEntry == nullptr ? "" : i_pGrpEntry->keyInTree()),
    m_strUnitName(),
    m_strGrpPath(i_pGrpEntry == nullptr ? "" : i_pGrpEntry->path()),
    m_strSymbol()
{
    if( i_pGrpEntry != nullptr && i_pGrpEntry->getSIUnit() != nullptr ) {
        m_pTreeEntry = i_pGrpEntry->getSIUnit();
        m_strKeyInTree = m_pTreeEntry->keyInTree();
        m_strUnitName = i_pGrpEntry->getSIUnit()->name();
        m_strSymbol = i_pGrpEntry->getSIUnit()->symbol();
    }
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnitsTreeEntryGrpPhysUnits& i_grpEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(&i_grpEntry),
    m_classType(i_grpEntry.classType()),
    m_strKeyInTree(i_grpEntry.keyInTree()),
    m_strUnitName(),
    m_strGrpPath(i_grpEntry.path()),
    m_strSymbol()
{
    if( i_grpEntry.getSIUnit() != nullptr ) {
        m_pTreeEntry = i_grpEntry.getSIUnit();
        m_strKeyInTree = m_pTreeEntry->keyInTree();
        m_strUnitName = i_grpEntry.getSIUnit()->name();
        m_strSymbol = i_grpEntry.getSIUnit()->symbol();
    }
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnitsTreeEntryGrpPhysUnits& i_grpEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitsTreeEntryGrpPhysUnits*>(&i_grpEntry)),
    m_classType(i_grpEntry.classType()),
    m_strKeyInTree(i_grpEntry.keyInTree()),
    m_strUnitName(),
    m_strGrpPath(i_grpEntry.path()),
    m_strSymbol()
{
    if( i_grpEntry.getSIUnit() != nullptr ) {
        m_pTreeEntry = i_grpEntry.getSIUnit();
        m_strKeyInTree = m_pTreeEntry->keyInTree();
        m_strUnitName = i_grpEntry.getSIUnit()->name();
        m_strSymbol = i_grpEntry.getSIUnit()->symbol();
    }
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnitsTreeEntryUnitBase* i_pUnitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pUnitEntry),
    m_classType(i_pUnitEntry == nullptr ? EUnitClassType::Undefined : i_pUnitEntry->classType()),
    m_strKeyInTree(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->keyInTree()),
    m_strUnitName(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->name()),
    m_strGrpPath(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->parentBranchPath()),
    m_strSymbol(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnitsTreeEntryUnitRatio* i_pUnitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pUnitEntry),
    m_classType(i_pUnitEntry == nullptr ? EUnitClassType::Undefined : i_pUnitEntry->classType()),
    m_strKeyInTree(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->keyInTree()),
    m_strUnitName(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->name()),
    m_strGrpPath(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->parentBranchPath()),
    m_strSymbol(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnitsTreeEntryUnitQuantity* i_pUnitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pUnitEntry),
    m_classType(i_pUnitEntry == nullptr ? EUnitClassType::Undefined : i_pUnitEntry->classType()),
    m_strKeyInTree(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->keyInTree()),
    m_strUnitName(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->name()),
    m_strGrpPath(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->parentBranchPath()),
    m_strSymbol(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnitsTreeEntryPhysUnit* i_pUnitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(i_pUnitEntry),
    m_classType(i_pUnitEntry == nullptr ? EUnitClassType::Undefined : i_pUnitEntry->classType()),
    m_strKeyInTree(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->keyInTree()),
    m_strUnitName(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->name()),
    m_strGrpPath(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->parentBranchPath()),
    m_strSymbol(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnitsTreeEntryUnitRatio* i_pUnitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitsTreeEntryUnitRatio*>(i_pUnitEntry)),
    m_classType(i_pUnitEntry == nullptr ? EUnitClassType::Undefined : i_pUnitEntry->classType()),
    m_strKeyInTree(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->keyInTree()),
    m_strUnitName(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->name()),
    m_strGrpPath(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->parentBranchPath()),
    m_strSymbol(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnitsTreeEntryUnitQuantity* i_pUnitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitsTreeEntryUnitQuantity*>(i_pUnitEntry)),
    m_classType(i_pUnitEntry == nullptr ? EUnitClassType::Undefined : i_pUnitEntry->classType()),
    m_strKeyInTree(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->keyInTree()),
    m_strUnitName(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->name()),
    m_strGrpPath(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->parentBranchPath()),
    m_strSymbol(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnitsTreeEntryPhysUnit* i_pUnitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitsTreeEntryPhysUnit*>(i_pUnitEntry)),
    m_classType(i_pUnitEntry == nullptr ? EUnitClassType::Undefined : i_pUnitEntry->classType()),
    m_strKeyInTree(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->keyInTree()),
    m_strUnitName(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->name()),
    m_strGrpPath(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->parentBranchPath()),
    m_strSymbol(i_pUnitEntry == nullptr ? "" : i_pUnitEntry->symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnitsTreeEntryUnitRatio& i_unitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(&i_unitEntry),
    m_classType(i_unitEntry.classType()),
    m_strKeyInTree(i_unitEntry.keyInTree()),
    m_strUnitName(i_unitEntry.name()),
    m_strGrpPath(i_unitEntry.parentBranchPath()),
    m_strSymbol(i_unitEntry.symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnitsTreeEntryUnitQuantity& i_unitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(&i_unitEntry),
    m_classType(i_unitEntry.classType()),
    m_strKeyInTree(i_unitEntry.keyInTree()),
    m_strUnitName(i_unitEntry.name()),
    m_strGrpPath(i_unitEntry.parentBranchPath()),
    m_strSymbol(i_unitEntry.symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(CUnitsTreeEntryPhysUnit& i_unitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(&i_unitEntry),
    m_classType(i_unitEntry.classType()),
    m_strKeyInTree(i_unitEntry.keyInTree()),
    m_strUnitName(i_unitEntry.name()),
    m_strGrpPath(i_unitEntry.parentBranchPath()),
    m_strSymbol(i_unitEntry.symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnitsTreeEntryUnitRatio& i_unitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitsTreeEntryUnitRatio*>(&i_unitEntry)),
    m_classType(i_unitEntry.classType()),
    m_strKeyInTree(i_unitEntry.keyInTree()),
    m_strUnitName(i_unitEntry.name()),
    m_strGrpPath(i_unitEntry.parentBranchPath()),
    m_strSymbol(i_unitEntry.symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnitsTreeEntryUnitQuantity& i_unitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitsTreeEntryUnitQuantity*>(&i_unitEntry)),
    m_classType(i_unitEntry.classType()),
    m_strKeyInTree(i_unitEntry.keyInTree()),
    m_strUnitName(i_unitEntry.name()),
    m_strGrpPath(i_unitEntry.parentBranchPath()),
    m_strSymbol(i_unitEntry.symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(const CUnitsTreeEntryPhysUnit& i_unitEntry) :
//------------------------------------------------------------------------------
    m_pTreeEntry(const_cast<CUnitsTreeEntryPhysUnit*>(&i_unitEntry)),
    m_classType(i_unitEntry.classType()),
    m_strKeyInTree(i_unitEntry.keyInTree()),
    m_strUnitName(i_unitEntry.name()),
    m_strGrpPath(i_unitEntry.parentBranchPath()),
    m_strSymbol(i_unitEntry.symbol())
{
}

//------------------------------------------------------------------------------
CUnit::CUnit(const QString& i_strUniqueName) :
//------------------------------------------------------------------------------
    m_pTreeEntry(),
    m_classType(),
    m_strKeyInTree(),
    m_strUnitName(),
    m_strGrpPath(),
    m_strSymbol()
{
    m_pTreeEntry = CIdxTreeUnits::GetInstance()->findEntry(i_strUniqueName);

    if( m_pTreeEntry != nullptr ) {
        if( m_pTreeEntry->isRoot() || m_pTreeEntry->isBranch() ) {
            CUnitsTreeEntryGrpBase* pGrpEntry =
                dynamic_cast<CUnitsTreeEntryGrpBase*>(m_pTreeEntry);
            if( pGrpEntry != nullptr ) {
                m_classType = pGrpEntry->classType();
                m_strKeyInTree = pGrpEntry->keyInTree();
                m_strGrpPath = pGrpEntry->path();
                if( pGrpEntry->classType() == EUnitClassType::PhysSize ) {
                    CUnitsTreeEntryGrpPhysUnits* pPhysSize =
                        dynamic_cast<CUnitsTreeEntryGrpPhysUnits*>(pGrpEntry);
                    if( pPhysSize != nullptr && pPhysSize->getSIUnit() != nullptr ) {
                        m_pTreeEntry = pPhysSize->getSIUnit();
                        m_strKeyInTree = m_pTreeEntry->keyInTree();
                        m_strUnitName = pPhysSize->getSIUnit()->name();
                        m_strSymbol = pPhysSize->getSIUnit()->symbol();
                    }
                }
            }
        }
        else if( m_pTreeEntry->isLeave() ) {
            CUnitsTreeEntryUnitBase* pUnitEntry
                = dynamic_cast<CUnitsTreeEntryUnitBase*>(m_pTreeEntry);
            if( pUnitEntry != nullptr ) {
                m_classType = pUnitEntry->classType();
                m_strKeyInTree = pUnitEntry->keyInTree();
                m_strUnitName = pUnitEntry->name();
                m_strGrpPath = pUnitEntry->parentBranchPath();
                m_strSymbol = pUnitEntry->symbol();
            }
        }
    }
} // ctor

//------------------------------------------------------------------------------
CUnit::CUnit(const QString& i_strPath, const QString& i_strNameOrSymbol) :
//------------------------------------------------------------------------------
    m_pTreeEntry(),
    m_classType(),
    m_strKeyInTree(),
    m_strUnitName(),
    m_strGrpPath(),
    m_strSymbol()
{
    m_pTreeEntry = CIdxTreeUnits::GetInstance()->findUnit(i_strPath, i_strNameOrSymbol);

    if( m_pTreeEntry != nullptr ) {
        if( m_pTreeEntry->isLeave() ) {
            CUnitsTreeEntryUnitBase* pUnitEntry
                = dynamic_cast<CUnitsTreeEntryUnitBase*>(m_pTreeEntry);
            if( pUnitEntry != nullptr ) {
                m_classType = pUnitEntry->classType();
                m_strKeyInTree = pUnitEntry->keyInTree();
                m_strUnitName = pUnitEntry->name();
                m_strGrpPath = pUnitEntry->parentBranchPath();
                m_strSymbol = pUnitEntry->symbol();
            }
        }
    }
} // ctor

//------------------------------------------------------------------------------
CUnit::~CUnit()
//------------------------------------------------------------------------------
{
    m_pTreeEntry = nullptr;
    //m_strKeyInTree;
    //m_strUnitName;
    //m_strGrpPath;
    //m_strSymbol;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnit::operator == ( const CUnit& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = false;
    if( m_pTreeEntry != nullptr && i_other.m_pTreeEntry != nullptr) {
        bEqual = (m_pTreeEntry == i_other.m_pTreeEntry);
    }
    else if( m_pTreeEntry == nullptr && i_other.m_pTreeEntry == nullptr) {
        bEqual = (m_strKeyInTree.compare(i_other.m_strKeyInTree) == 0);
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CUnit::operator != ( const CUnit& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CUnit::setGroupPath(const QString& i_strPath)
//------------------------------------------------------------------------------
{
    if( m_strGrpPath.compare(i_strPath) != 0 )
    {
        m_strGrpPath = i_strPath;

        CIdxTreeEntry* pTreeEntry = nullptr;

        if( m_strUnitName.isEmpty() && m_strSymbol.isEmpty() )
        {
            pTreeEntry = CIdxTreeUnits::GetInstance()->findUnitGrp(m_strGrpPath);
            if( pTreeEntry->isRoot() || pTreeEntry->isBranch() ) {
                CUnitsTreeEntryGrpBase* pGrpEntry = dynamic_cast<CUnitsTreeEntryGrpBase*>(pTreeEntry);
                if( pGrpEntry != nullptr ) {
                    m_classType = pGrpEntry->classType();
                    m_strKeyInTree = pGrpEntry->keyInTree();
                    m_strGrpPath = pGrpEntry->path();
                    if( pGrpEntry->classType() == EUnitClassType::PhysSize ) {
                        CUnitsTreeEntryGrpPhysUnits* pPhysSize =
                            dynamic_cast<CUnitsTreeEntryGrpPhysUnits*>(pGrpEntry);
                        if( pPhysSize != nullptr && pPhysSize->getSIUnit() != nullptr ) {
                            m_strUnitName = pPhysSize->getSIUnit()->name();
                            m_strSymbol = pPhysSize->getSIUnit()->symbol();
                        }
                    }
                }
            }
        }
        else if( !m_strUnitName.isEmpty() )
        {
            pTreeEntry = CIdxTreeUnits::GetInstance()->findUnit(m_strGrpPath, m_strUnitName);
            if( pTreeEntry != nullptr ) {
                if( pTreeEntry->isLeave() ) {
                    CUnitsTreeEntryUnitBase* pUnitEntry = dynamic_cast<CUnitsTreeEntryUnitBase*>(pTreeEntry);
                    if( pUnitEntry != nullptr ) {
                        m_classType = pUnitEntry->classType();
                        m_strKeyInTree = pUnitEntry->keyInTree();
                        m_strSymbol = pUnitEntry->symbol();
                        // As the given unit name could have been the prefix string
                        // we set the unit name again.
                        m_strUnitName = pUnitEntry->name();
                    }
                }
            }
        }
        else if( !m_strSymbol.isEmpty() )
        {
            pTreeEntry = CIdxTreeUnits::GetInstance()->findUnit(m_strGrpPath, m_strSymbol);
            if( pTreeEntry != nullptr ) {
                if( pTreeEntry->isLeave() ) {
                    CUnitsTreeEntryUnitBase* pUnitEntry = dynamic_cast<CUnitsTreeEntryUnitBase*>(pTreeEntry);
                    if( pUnitEntry != nullptr ) {
                        m_classType = pUnitEntry->classType();
                        m_strKeyInTree = pUnitEntry->keyInTree();
                        m_strUnitName = pUnitEntry->name();
                        // As the given symbol could have been the prefix string
                        // we set the symbol again.
                        m_strSymbol = pUnitEntry->symbol();
                    }
                }
            }
        }
        m_pTreeEntry = pTreeEntry;
    }
} // setGroupPath

//------------------------------------------------------------------------------
QString CUnit::groupPath() const
//------------------------------------------------------------------------------
{
    return m_strGrpPath;
}

//------------------------------------------------------------------------------
void CUnit::setUnitName(const QString& i_strName)
//------------------------------------------------------------------------------
{
    if( m_strUnitName.compare(i_strName) != 0 )
    {
        m_strUnitName = i_strName;

        CIdxTreeEntry* pTreeEntry = nullptr;

        if( !m_strUnitName.isEmpty() && !m_strGrpPath.isEmpty() ) {
            pTreeEntry = CIdxTreeUnits::GetInstance()->findUnit(m_strGrpPath, m_strUnitName);
            if( pTreeEntry != nullptr && pTreeEntry->isLeave() ) {
                CUnitsTreeEntryUnitBase* pUnitEntry = dynamic_cast<CUnitsTreeEntryUnitBase*>(pTreeEntry);
                if( pUnitEntry != nullptr ) {
                    if( m_classType != pUnitEntry->classType() ) {
                        SErrResultInfo errResultInfo(
                            NameSpace(), ClassName(), "", "setUnitName",
                             EResultInternalProgramError, EResultSeverityCritical,
                            "Current unit class type " + classType2Str() +
                            " is different from newly calculated class type " +
                            pUnitEntry->classType2Str());
                        throw CException(errResultInfo);
                    }
                    if( m_strGrpPath.compare(pUnitEntry->parentBranchPath()) != 0 ) {
                        SErrResultInfo errResultInfo(
                            NameSpace(), ClassName(), "", "setUnitName",
                            EResultInternalProgramError, EResultSeverityCritical,
                            "Current group " + m_strGrpPath + " is different from "
                            "newly calculated group " + pUnitEntry->parentBranchPath());
                        throw CException(errResultInfo);
                    }
                    m_strKeyInTree = pUnitEntry->keyInTree();
                    m_strSymbol = pUnitEntry->symbol();
                    // As the given unit name could have been the symbol or
                    // prefix string we set the unit name again.
                    m_strUnitName = pUnitEntry->name();
                }
            }
        }
        m_pTreeEntry = pTreeEntry;
    }
} // setUnitName

//------------------------------------------------------------------------------
QString CUnit::unitName() const
//------------------------------------------------------------------------------
{
    return m_strUnitName;
}

//------------------------------------------------------------------------------
void CUnit::setSymbol(const QString& i_strSymbol)
//------------------------------------------------------------------------------
{
    if( m_strSymbol.compare(i_strSymbol) != 0 )
    {
        m_strSymbol = i_strSymbol;

        CIdxTreeEntry* pTreeEntry = nullptr;

        if( !m_strSymbol.isEmpty() && !m_strGrpPath.isEmpty() ) {
            pTreeEntry = CIdxTreeUnits::GetInstance()->findUnit(m_strGrpPath, m_strSymbol);
            if( pTreeEntry != nullptr && pTreeEntry->isLeave() ) {
                CUnitsTreeEntryUnitBase* pUnitEntry = dynamic_cast<CUnitsTreeEntryUnitBase*>(pTreeEntry);
                if( pUnitEntry != nullptr ) {
                    if( m_classType != pUnitEntry->classType() ) {
                        SErrResultInfo errResultInfo(
                            NameSpace(), ClassName(), "", "setUnitName",
                            EResultInternalProgramError, EResultSeverityCritical,
                            "Current unit class type " + classType2Str() +
                            " is different from newly calculated class type " +
                            pUnitEntry->classType2Str());
                        throw CException(errResultInfo);
                    }
                    if( m_strGrpPath.compare(pUnitEntry->parentBranchPath()) != 0 ) {
                        SErrResultInfo errResultInfo(
                            NameSpace(), ClassName(), "", "setUnitName",
                            EResultInternalProgramError, EResultSeverityCritical,
                            "Current group " + m_strGrpPath + " is different from "
                            "newly calculated group " + pUnitEntry->parentBranchPath());
                        throw CException(errResultInfo);
                    }
                    m_strKeyInTree = pUnitEntry->keyInTree();
                    m_strUnitName = pUnitEntry->name();
                }
            }
        }
    }
} // setSymbol

//------------------------------------------------------------------------------
QString CUnit::symbol() const
//------------------------------------------------------------------------------
{
    return m_strSymbol;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CUnit::nodeSeparator() const
//------------------------------------------------------------------------------
{
    return QString(m_pTreeEntry == nullptr ? "" : m_pTreeEntry->nodeSeparator());
}

//------------------------------------------------------------------------------
QString CUnit::keyInTree() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTree;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnit::isValidGroup() const
//------------------------------------------------------------------------------
{
    bool bValid = false;
    if( m_pTreeEntry != nullptr) {
        CUnitsTreeEntryGrpBase* pGrpEntry = dynamic_cast<CUnitsTreeEntryGrpBase*>(m_pTreeEntry);
        bValid = (pGrpEntry != nullptr);
        if( !bValid ) {
            CUnitsTreeEntryUnitBase* pUnitEntry = dynamic_cast<CUnitsTreeEntryUnitBase*>(m_pTreeEntry);
            bValid = (pUnitEntry != nullptr);
        }
    }
    return bValid;
}

//------------------------------------------------------------------------------
bool CUnit::isValid() const
//------------------------------------------------------------------------------
{
    bool bValid = false;
    if( m_pTreeEntry != nullptr) {
        CUnitsTreeEntryUnitBase* pUnitEntry = dynamic_cast<CUnitsTreeEntryUnitBase*>(m_pTreeEntry);
        bValid = (pUnitEntry != nullptr);
    }
    return bValid;
}

//------------------------------------------------------------------------------
EUnitClassType CUnit::classType() const
//------------------------------------------------------------------------------
{
    return m_classType;
}

//------------------------------------------------------------------------------
QString CUnit::classType2Str() const
//------------------------------------------------------------------------------
{
    return CEnumUnitClassType(m_classType).toString();
}

//------------------------------------------------------------------------------
bool CUnit::isRatio() const
//------------------------------------------------------------------------------
{
    return m_classType == EUnitClassType::Ratio;
}

//------------------------------------------------------------------------------
bool CUnit::isQuantity() const
//------------------------------------------------------------------------------
{
    return m_classType == EUnitClassType::Quantity;
}

//------------------------------------------------------------------------------
bool CUnit::isRatioOrQuantity() const
//------------------------------------------------------------------------------
{
    return (isRatio() || isQuantity());
}

//------------------------------------------------------------------------------
bool CUnit::isPhysSize() const
//------------------------------------------------------------------------------
{
    return m_classType == EUnitClassType::PhysSize;
}

/*==============================================================================
public: // overridables (converting values)
==============================================================================*/

//------------------------------------------------------------------------------
double CUnit::factor() const
//------------------------------------------------------------------------------
{
    double fFactor = 0.0;
    if( m_pTreeEntry != nullptr ) {
        CUnitsTreeEntryUnitRatio* pUnitEntry =
            dynamic_cast<CUnitsTreeEntryUnitRatio*>(m_pTreeEntry);
        if( pUnitEntry != nullptr ) {
            fFactor = pUnitEntry->factor();
        }
    }
    return fFactor;
}

//------------------------------------------------------------------------------
bool CUnit::isLogarithmic() const
//------------------------------------------------------------------------------
{
    bool bIs = false;
    if( m_pTreeEntry != nullptr ) {
        CUnitsTreeEntryUnitBase* pUnitEntry =
            dynamic_cast<CUnitsTreeEntryUnitBase*>(m_pTreeEntry);
        if( pUnitEntry != nullptr ) {
            bIs = pUnitEntry->isLogarithmic();
        }
    }
    return bIs;
}

//------------------------------------------------------------------------------
double CUnit::logarithmicFactor() const
//------------------------------------------------------------------------------
{
    double fFactor = 0.0;
    if( m_pTreeEntry != nullptr ) {
        CUnitsTreeEntryUnitBase* pUnitEntry =
            dynamic_cast<CUnitsTreeEntryUnitBase*>(m_pTreeEntry);
        if( pUnitEntry != nullptr ) {
            fFactor = pUnitEntry->logarithmicFactor();
        }
    }
    return fFactor;
}

//------------------------------------------------------------------------------
bool CUnit::isConvertible( const CUnit& i_unitDst, double i_fVal ) const
//------------------------------------------------------------------------------
{
    bool bIs = false;
    if( m_pTreeEntry != nullptr ) {
        CUnitsTreeEntryUnitBase* pUnitEntry =
            dynamic_cast<CUnitsTreeEntryUnitBase*>(m_pTreeEntry);
        if( pUnitEntry != nullptr ) {
            CUnitsTreeEntryUnitBase* pUnitEntryDst =
                dynamic_cast<CUnitsTreeEntryUnitBase*>(i_unitDst.m_pTreeEntry);
            if( pUnitEntryDst != nullptr ) {
                bIs = pUnitEntry->isConvertible(pUnitEntryDst, i_fVal);
            }
        }
    }
    return bIs;
}

//------------------------------------------------------------------------------
double CUnit::convertValue( double i_fVal, const CUnit& i_unitDst ) const
//------------------------------------------------------------------------------
{
    double fValResult = 0.0;
    if( m_pTreeEntry != nullptr ) {
        CUnitsTreeEntryUnitBase* pUnitEntry =
            dynamic_cast<CUnitsTreeEntryUnitBase*>(m_pTreeEntry);
        if( pUnitEntry != nullptr ) {
            CUnitsTreeEntryUnitBase* pUnitEntryDst =
                dynamic_cast<CUnitsTreeEntryUnitBase*>(i_unitDst.m_pTreeEntry);
            if( pUnitEntryDst != nullptr ) {
                fValResult = pUnitEntry->convertValue(i_fVal, pUnitEntryDst);
            }
        }
    }
    return fValResult;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnit CUnit::nextLowerUnit() const
//------------------------------------------------------------------------------
{
    CUnit unitNext;
    if( m_pTreeEntry != nullptr ) {
        CUnitsTreeEntryUnitBase* pUnitEntry =
            dynamic_cast<CUnitsTreeEntryUnitBase*>(m_pTreeEntry);
        if( pUnitEntry != nullptr ) {
            unitNext = pUnitEntry->nextLowerUnit();
        }
    }
    return unitNext;
}

//------------------------------------------------------------------------------
CUnit CUnit::nextHigherUnit() const
//------------------------------------------------------------------------------
{
    CUnit unitNext;
    if( m_pTreeEntry != nullptr ) {
        CUnitsTreeEntryUnitBase* pUnitEntry =
            dynamic_cast<CUnitsTreeEntryUnitBase*>(m_pTreeEntry);
        if( pUnitEntry != nullptr ) {
            unitNext = pUnitEntry->nextHigherUnit();
        }
    }
    return unitNext;
}

//------------------------------------------------------------------------------
CUnit CUnit::findBestUnit(
    double  i_fVal,
    double* o_pfValue,
    int     i_iDigitsLeadingMax ) const
//------------------------------------------------------------------------------
{
    CUnit unitBest;
    if( m_pTreeEntry != nullptr ) {
        CUnitsTreeEntryUnitBase* pUnitEntry =
            dynamic_cast<CUnitsTreeEntryUnitBase*>(m_pTreeEntry);
        if( pUnitEntry != nullptr ) {
            unitBest = pUnitEntry->findBestUnit(i_fVal, o_pfValue, i_iDigitsLeadingMax);
        }
    }
    return unitBest;
}
