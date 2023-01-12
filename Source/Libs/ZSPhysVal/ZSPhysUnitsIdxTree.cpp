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

#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpScienceField.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysTreeEntryUnitRatio.h"
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
CUnitsTreeEntryGrpScienceField* CIdxTreeUnits::findPhysScienceField( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryGrpScienceField*>(findEntry(i_strUniqueName));
}

//------------------------------------------------------------------------------
CUnitsTreeEntryGrpBase* CIdxTreeUnits::findUnitGrp( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryGrpBase*>(findEntry(i_strUniqueName));
}

//------------------------------------------------------------------------------
/*! @brief
    Same as "findUnitGrp" but with implicit type cast.
*/
CUnitsTreeEntryGrpPhysUnits* CIdxTreeUnits::findPhysUnitsGroup( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryGrpPhysUnits*>(findEntry(i_strUniqueName));
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase* CIdxTreeUnits::findUnit( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryUnitBase*>(findEntry(i_strUniqueName));
}

//------------------------------------------------------------------------------
CUnitsTreeEntryUnitBase* CIdxTreeUnits::findUnit( const QString& i_strGrpPath, const QString& i_strUnitName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryUnitBase*>(findLeave(i_strGrpPath, i_strUnitName));
}

//------------------------------------------------------------------------------
/*! @brief
    Same as "findUnit" but with implicit type cast
*/
CUnitsTreeEntryPhysUnit* CIdxTreeUnits::findPhysUnit( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryPhysUnit*>(findEntry(i_strUniqueName));
}

//------------------------------------------------------------------------------
/*! @brief
    Same as "findUnit" but with implicit type cast
*/
CUnitsTreeEntryPhysUnit* CIdxTreeUnits::findPhysUnit( const QString& i_strGrpPath, const QString& i_strUnitName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryPhysUnit*>(findLeave(i_strGrpPath, i_strUnitName));
}

//------------------------------------------------------------------------------
/*! @brief
    Same as "findUnit" but with implicit type cast
*/
CUnitsTreeEntryUnitRatio* CIdxTreeUnits::findRatio( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryUnitRatio*>(findEntry(i_strUniqueName));
}

//------------------------------------------------------------------------------
/*! @brief
    Same as "findUnit" but with implicit type cast
*/
CUnitsTreeEntryUnitRatio* CIdxTreeUnits::findRatio( const QString& i_strGrpPath, const QString& i_strUnitName )
//------------------------------------------------------------------------------
{
    return dynamic_cast<CUnitsTreeEntryUnitRatio*>(findLeave(i_strGrpPath, i_strUnitName));
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the number of active references to this instance.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references.
*/
//------------------------------------------------------------------------------
int CIdxTreeUnits::getRefCount() const
{
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Increments the number of active reference to this instance.

    /return Number of active references after increment.
*/
//------------------------------------------------------------------------------
int CIdxTreeUnits::incrementRefCount()
{
    return ++m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Decrements the number of active reference to this instance.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references after decrement.
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
