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

#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpScienceField.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysTreeEntryUnitRatio.h"
#include "ZSPhysVal/ZSPhysTreeEntryUnitQuantity.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CIdxTreeUnits : public ZS::System::CIdxTree
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

CIdxTreeUnits* CIdxTreeUnits::s_pTheInst = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an index tree instance if if not already existing.

    If an index tree is already existing the reference counter of
    the existing index tree is incremented and this index tree is returned.

    @note After a CreateInstance call a ReleaseInstance call MUST follow if no
          the index tree is no longer needed to avoid memory leaks.

    @return Pointer to index tree instance.
*/
CIdxTreeUnits* CIdxTreeUnits::CreateInstance()
//------------------------------------------------------------------------------
{
    CIdxTreeUnits* pIdxTree = CIdxTreeUnits::GetInstance();
    if( pIdxTree == nullptr ) {
        pIdxTree = new CIdxTreeUnits();
    }
    else {
        pIdxTree->incrementRefCount();
    }
    return pIdxTree;
}

//------------------------------------------------------------------------------
/*! @brief Returns the addess of the index tree with the given name.

    This method does neither create an instance of the class nor increments the
    reference counter. If no instance has been created yet the method returns nullptr.

    If you just need to access the already existing instance and you can be sure
    that the instance is already existing this method should be preferred to the
    createInstance call as this method does not affect the reference counter and
    there is no need to call ReleaseInstance later on.

    @note After a GetInstance call a ReleaseInstance MUST NOT be called.

    @param i_strObjName [in] Name of the index tree.
        Default: "PhysSizes"

    @return Pointer to index tree instance.
*/
CIdxTreeUnits* CIdxTreeUnits::GetInstance()
//------------------------------------------------------------------------------
{
    return s_pTheInst;
}

//------------------------------------------------------------------------------
/*! @brief Releases the index tree instance with the given name.

    Before invoking this method a reference to the instance must have been
    retrieved with a CreateInstance call.

    This method decrements the reference counter of the instance.
    If the reference counter reaches 0 the instance will be destroyed.

    @param i_strObjName [in] Name of the index tree.
        Default: "PhysSizes"

    @return Current increment counter.
*/
int CIdxTreeUnits::ReleaseInstance()
//------------------------------------------------------------------------------
{
    CIdxTreeUnits* pIdxTree = GetInstance();

    if( pIdxTree == nullptr )
    {
        throw CException(
            __FILE__, __LINE__, EResultSingletonClassNotInstantiated,
            NameSpace() + "::" + ClassName() + "::ReleaseInstance");
    }

    int iRefCount = pIdxTree->getRefCount();

    if( iRefCount <= 0 )
    {
        throw CException(
            __FILE__, __LINE__, EResultInternalProgramError,
            NameSpace() + "::" + ClassName() + "::ReleaseInstance: "
            "Reference Counter is already less or equal to 0 (=" + QString::number(iRefCount) + ")");
    }

    iRefCount = pIdxTree->decrementRefCount();

    if( iRefCount == 0 )
    {
        try
        {
            delete pIdxTree;
        }
        catch(...)
        {
        }
        pIdxTree = nullptr;
    }
    return iRefCount;

} // ReleaseInstance

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates a unit pool with the given name if an instance with the given
    name is not already existing.

    If an instance with the given name is already existing the reference to
    the existing instance is returned and a reference counter is incremented.

    @param i_pRootTreeEntry [in] Reference to the root entry of the index tree.
           Usually the tree creates the node itself. However, if the root node needs
           properties that go beyond the default implementation, a user-defined node
           can be created and passed here. The root node becomes the property of the
           tree and is destroyed along with the tree.

    @return Pointer to unit pool instance.
*/
CIdxTreeUnits::CIdxTreeUnits( CIdxTreeEntry* i_pRootTreeEntry ) :
//------------------------------------------------------------------------------
    CIdxTree(
        /* strIdxTreeName   */ "Units",
        /* pRootTreeEntry   */ i_pRootTreeEntry,
        /* strNodeSeparator */ ".",
        /* bCreateMutex     */ false,
        /* pObjParent       */ nullptr ),
    m_iRefCount(0)
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ "" );

    if( s_pTheInst != nullptr )
    {
        throw CException(
            __FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated,
            NameSpace() + "::" + ClassName() + "::ctor");
    }
    s_pTheInst = this;

    m_iRefCount = 1;

} // ctor

//------------------------------------------------------------------------------
CIdxTreeUnits::~CIdxTreeUnits()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    if( m_iRefCount <= 0 )
    {
        if( CErrLog::GetInstance() != nullptr ) {
            SErrResultInfo errResultInfo(
                NameSpace(), ClassName(), objectName(), "dtor",
                EResultInvalidMethodCall, EResultSeverityError,
                "Destructor of singleton class called but reference counter "
                "is less or equal to 0 (=" + QString::number(m_iRefCount) + ").");
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( m_iRefCount > 1 )
    {
        if( CErrLog::GetInstance() != nullptr ) {
            SErrResultInfo errResultInfo(
                NameSpace(), ClassName(), objectName(), "dtor",
                EResultInvalidMethodCall, EResultSeverityError,
                "Destructor of singleton class called but reference counter "
                "is greater than 1 (=" + QString::number(m_iRefCount) + ").");
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    s_pTheInst = nullptr;

    m_iRefCount = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Tries to find the unit group entry by the given unique name.

    Units are added as childs to group nodes in the index tree.
    in case of phyiscal units the parent node is a physical size node
    (group with physical units only).
    In case of ratio and quantity unit the parent is a unit group just containing
    either ratio or quantity units.

    @param i_strUniqueName Unique name of the group.
        The unique name may be either the key of the unit group starting
        - (optional) with the type of the entry ("B" for branches)
        - followed by the path to the units group entry.
        Examples:
            "L:Electricity.Power"

    @return Pointer to units group entry or nullptr, if the node has not been found.
*/
CUnitsTreeEntryGrpBase* CIdxTreeUnits::findUnitGrp( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryGrpBase*>(findBranch(i_strUniqueName));
}

//------------------------------------------------------------------------------
/*! @brief Same as "findUnitGrp" but with implicit type cast.

    A physical science field node don't contain units but groups with physical
    unit and servers as a parent to group the physical science group nodes.
*/
CUnitsTreeEntryGrpScienceField* CIdxTreeUnits::findPhysScienceField( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryGrpScienceField*>(findBranch(i_strUniqueName));
}

//------------------------------------------------------------------------------
/*! @brief Same as "findUnitGrp" but with implicit type cast.
*/
CUnitsTreeEntryGrpPhysUnits* CIdxTreeUnits::findPhysUnitsGroup( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryGrpPhysUnits*>(findBranch(i_strUniqueName));
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Tries to find the unit tree entry by the given unique unit name.

    @param i_strUniqueName Unique name of the unit.
        The unique name may be either the key of the unit starting
        - (optional) with the type of the entry ("L" for Leaves)
        - followed by the path to the units group entry (the path to the parent node)
        - and terminated by the name of the unit which may be the name of the unit,
          the units symbol or just the factor prefix (e.g. "k").
        Examples:
            "L:Electricity.Power.KiloWatt"
            "Electricity.Power.kW"
            "Electricity.Power.k"

    @return Pointer to unit entry or nullptr, if the node has not been found.
*/
CUnitsTreeEntryUnitBase* CIdxTreeUnits::findUnit( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryUnitBase*>(findLeave(i_strUniqueName));
}

//------------------------------------------------------------------------------
/*! @brief Tries to find the unit tree entry.

    The method tries to find the entry using the given group path and unit name.
    The unit name may be either the name of the unit, the symbol or - in case of
    physical units - the factor prefix.

    @param i_strGrpPath Path of the parent node of the unit.
        For physical units this is the path to the physical size
        (e.g. "Kinematics.Electricity.Power").
    @param i_strSymbolOrName Name of the unit to be searched for.
        The name may be either the name of the unit (e.g. "KiloWatt"), the
        symbol of the unit (e.g. "kW") or just the factor prefix (e.g. "k").

    @return Pointer to unit entry or nullptr, if the node has not been found.
*/
CUnitsTreeEntryUnitBase* CIdxTreeUnits::findUnit(
    const QString& i_strGrpPath, const QString& i_strSymbolOrName )
//------------------------------------------------------------------------------
{
    CUnitsTreeEntryUnitBase* pUnit = nullptr;
    CUnitsTreeEntryGrpBase* pGrp = dynamic_cast<CUnitsTreeEntryGrpBase*>(findBranch(i_strGrpPath));
    if( pGrp != nullptr ) {
        pUnit = pGrp->findUnit(i_strSymbolOrName);
    }
    else {
        pUnit = findUnit(i_strSymbolOrName);
    }
    return pUnit;
}

//------------------------------------------------------------------------------
/*! @brief Same as "findUnit" but with implicit type cast.
*/
CUnitsTreeEntryPhysUnit* CIdxTreeUnits::findPhysUnit( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryPhysUnit*>(findUnit(i_strUniqueName));
}

//------------------------------------------------------------------------------
/*! @brief Same as "findUnit" but with implicit type cast.
*/
CUnitsTreeEntryPhysUnit* CIdxTreeUnits::findPhysUnit(
    const QString& i_strGrpPath, const QString& i_strSymbolOrName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryPhysUnit*>(findUnit(i_strGrpPath, i_strSymbolOrName));
}

//------------------------------------------------------------------------------
/*! @brief Same as "findUnit" but with implicit type cast.
*/
CUnitsTreeEntryUnitRatio* CIdxTreeUnits::findUnitRatio( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryUnitRatio*>(findUnit(i_strUniqueName));
}

//------------------------------------------------------------------------------
/*! @brief Same as "findUnit" but with implicit type cast.
*/
CUnitsTreeEntryUnitRatio* CIdxTreeUnits::findUnitRatio(
    const QString& i_strGrpPath, const QString& i_strSymbolOrName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryUnitRatio*>(findUnit(i_strGrpPath, i_strSymbolOrName));
}

//------------------------------------------------------------------------------
/*! @brief Same as "findUnit" but with implicit type cast.
*/
CUnitsTreeEntryUnitQuantity* CIdxTreeUnits::findUnitQuantity( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryUnitQuantity*>(findUnit(i_strUniqueName));
}

//------------------------------------------------------------------------------
/*! @brief Same as "findUnit" but with implicit type cast.
*/
CUnitsTreeEntryUnitQuantity* CIdxTreeUnits::findUnitQuantity(
    const QString& i_strGrpPath, const QString& i_strSymbolOrName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryUnitQuantity*>(findUnit(i_strGrpPath, i_strSymbolOrName));
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the number of active references to this instance.
    If the count reaches 0 the instance has to be deleted.

    @return Number of active references.
*/
//------------------------------------------------------------------------------
int CIdxTreeUnits::getRefCount() const
{
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Increments the number of active reference to this instance.

    @return Number of active references after increment.
*/
//------------------------------------------------------------------------------
int CIdxTreeUnits::incrementRefCount()
{
    return ++m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Decrements the number of active reference to this instance.
    If the count reaches 0 the instance has to be deleted.

    @return Number of active references after decrement.
*/
//------------------------------------------------------------------------------
int CIdxTreeUnits::decrementRefCount()
{
    if( m_iRefCount <= 0)
    {
        throw CException(__FILE__, __LINE__, EResultObjRefCounterIsZero, ClassName() + "::" + objectName());
    }
    return --m_iRefCount;
}
