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

#include "ZSPhysVal/ZSPhysUnitsPool.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSPhysVal/ZSPhysUnitsGrp.h"
#include "ZSPhysVal/ZSPhysUnitsRatio.h"
#include "ZSPhysVal/ZSPhysUnitsDataQuantity.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitsPools
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QHash<QString, CUnitsPool*> CUnitsPool::s_hshpInstances;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnitsPool* CUnitsPool::GetInstance( const QString& i_strName )
//------------------------------------------------------------------------------
{
    return s_hshpInstances.value(i_strName, nullptr);
}

//------------------------------------------------------------------------------
/*! Creates a unit pool with the given name if an instance with the given
    name is not already existing.

    If an instance with the given name is already existing the reference to
    the existing instance is returned and a reference counter is incremented.

    \param i_strName [in] Name of the unit pool (default "ZSPhysSizes")

    \return Pointer to unit pool instance.
*/
CUnitsPool* CUnitsPool::CreateInstance( const QString& i_strName )
//------------------------------------------------------------------------------
{
    CUnitsPool* pUnitsPool = s_hshpInstances.value(i_strName, nullptr);

    if( pUnitsPool == NULL )
    {
        pUnitsPool = new CUnitsPool(i_strName);
    }

    pUnitsPool->incrementRefCount();

    return pUnitsPool;

} // CreateInstance

//------------------------------------------------------------------------------
void CUnitsPool::ReleaseInstance( const QString& i_strName )
//------------------------------------------------------------------------------
{
    CUnitsPool* pUnitsPool = s_hshpInstances.value(i_strName, nullptr);

    if( pUnitsPool == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, "CUnitsPool::" + i_strName);
    }

    int iRefCount = pUnitsPool->decrementRefCount();

    if( iRefCount == 0 )
    {
        s_hshpInstances.remove(i_strName);

        try
        {
            delete pUnitsPool;
        }
        catch(...)
        {
        }
        pUnitsPool = nullptr;
    }
} // ReleaseInstance

//------------------------------------------------------------------------------
void CUnitsPool::ReleaseInstance( CUnitsPool* i_pUnitsPool )
//------------------------------------------------------------------------------
{
    QString strName = i_pUnitsPool->name();

    if( !s_hshpInstances.contains(strName) )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, "CUnitsPool::" + strName);
    }

    int iRefCount = i_pUnitsPool->decrementRefCount();

    if( iRefCount == 0 )
    {
        s_hshpInstances.remove(strName);

        delete i_pUnitsPool;
        i_pUnitsPool = nullptr;
    }
} // ReleaseInstance

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitsPool::CUnitsPool( const QString& i_strName ) :
//------------------------------------------------------------------------------
    m_strName(i_strName),
    m_cNameSeparator('.'),
    m_hshpUnitGrps(),
    m_hshpUnits(),
    //m_arpUnitGrpClassTypes[EUnitClassTypeCount]
    //m_arpUnitGrpPhysScienceFields[EPhysScienceFieldCount]
    m_iRefCount(0)
{
    memset( m_arpUnitGrpClassTypes, 0x00, sizeof(m_arpUnitGrpClassTypes) );
    memset( m_arpUnitGrpPhysScienceFields, 0x00, sizeof(m_arpUnitGrpPhysScienceFields) );

    EUnitClassType    classType;
    int               idxClassType;
    EPhysScienceField scienceField;
    int               idxScienceField;

    for( idxClassType = 0; idxClassType < EUnitClassTypeCount; idxClassType++ )
    {
        classType = static_cast<EUnitClassType>(idxClassType);

        if( classType == EUnitClassTypeRatios )
        {
            m_arpUnitGrpClassTypes[idxClassType] = new CUnitGrpRatio(this);
        }
        else if( classType == EUnitClassTypeDataQuantity )
        {
            m_arpUnitGrpClassTypes[idxClassType] = new CUnitGrpDataQuantity(this);
        }
        else if( classType == EUnitClassTypePhysScienceFields )
        {
            m_arpUnitGrpClassTypes[idxClassType] = new CUnitGrp(
                /* pUnitsPool       */ this,
                /* classType        */ classType,
                /* strName          */ unitClassType2Str(classType),
                /* strKey           */ unitClassType2Str(classType),
                /* bIsNameSpaceNode */ true,
                /* pUnitGrpParent   */ nullptr );

            for( idxScienceField = 0; idxScienceField < EPhysScienceFieldCount; idxScienceField++ )
            {
                scienceField = static_cast<EPhysScienceField>(idxScienceField);

                m_arpUnitGrpPhysScienceFields[idxScienceField] = new CUnitGrp(
                    /* pUnitsPool       */ this,
                    /* classType        */ classType,
                    /* strName          */ physScienceField2Str(scienceField),
                    /* strKey           */ physScienceField2Str(scienceField),
                    /* bIsNameSpaceNode */ true,
                    /* pUnitGrpParent   */ m_arpUnitGrpClassTypes[idxClassType] );
            }
        }
        else // if( classType == EUnitClassTypeUserDefinedQuantities )
        {
            m_arpUnitGrpClassTypes[idxClassType] = new CUnitGrp(
                /* pUnitsPool       */ this,
                /* classType        */ classType,
                /* strName          */ unitClassType2Str(classType),
                /* strKey           */ unitClassType2Str(classType),
                /* bIsNameSpaceNode */ true,
                /* pUnitGrpParent   */ nullptr );
        }
    }

} // ctor

//------------------------------------------------------------------------------
CUnitsPool::~CUnitsPool()
//------------------------------------------------------------------------------
{
    EUnitClassType    classType;
    int               idxClassType;
    EPhysScienceField scienceField;
    int               idxScienceField;

    for( idxScienceField = 0; idxScienceField < EPhysScienceFieldCount; idxScienceField++ )
    {
        scienceField = static_cast<EPhysScienceField>(idxScienceField);

        try
        {
            delete m_arpUnitGrpPhysScienceFields[scienceField];
        }
        catch(...)
        {
        }
        m_arpUnitGrpPhysScienceFields[scienceField] = nullptr;
    }

    for( idxClassType = 0; idxClassType < EUnitClassTypeCount; idxClassType++ )
    {
        classType = static_cast<EUnitClassType>(idxClassType);

        try
        {
            delete m_arpUnitGrpClassTypes[classType];
        }
        catch(...)
        {
        }
        m_arpUnitGrpClassTypes[classType] = nullptr;
    }

    //m_strName;
    //m_strName;
    //m_cNameSeparator;
    //m_hshpUnitGrps;
    //m_hshpUnits;
    //m_arpUnitGrpClassTypes[EUnitClassTypeCount];
    //m_arpUnitGrpPhysScienceFields[EPhysScienceFieldCount];
    m_iRefCount = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QChar CUnitsPool::getNameSeparator() const
//------------------------------------------------------------------------------
{
    return m_cNameSeparator;
}

//------------------------------------------------------------------------------
void CUnitsPool::setNameSeparator( QChar i_cSeparator )
//------------------------------------------------------------------------------
{
    m_cNameSeparator = i_cSeparator;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnitGrp* CUnitsPool::getUnitClassTypeGroup( EUnitClassType i_classType )
//------------------------------------------------------------------------------
{
    return m_arpUnitGrpClassTypes[i_classType];
}

//------------------------------------------------------------------------------
CUnitGrp* CUnitsPool::getPhysScienceFieldUnitGroup( EPhysScienceField i_scienceField )
//------------------------------------------------------------------------------
{
    return m_arpUnitGrpPhysScienceFields[i_scienceField];
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnitGrp* CUnitsPool::findUnitGrp( const QString& i_strUnitGrpKey )
//------------------------------------------------------------------------------
{
    CUnitGrp* pUnitGrp = nullptr;

    if( m_hshpUnitGrps.contains(i_strUnitGrpKey) )
    {
        pUnitGrp = m_hshpUnitGrps[i_strUnitGrpKey];
    }
    return pUnitGrp;

} // findUnitGrp

//------------------------------------------------------------------------------
CPhysSize* CUnitsPool::findPhysSize( const QString& i_strUnitGrpKey )
//------------------------------------------------------------------------------
{
    CPhysSize* pPhysSize = nullptr;

    if( m_hshpUnitGrps.contains(i_strUnitGrpKey) )
    {
        CUnitGrp* pUnitGrp = m_hshpUnitGrps[i_strUnitGrpKey];

        if( pUnitGrp->classType() == EUnitClassTypePhysScienceFields )
        {
            pPhysSize = dynamic_cast<CPhysSize*>(pUnitGrp);
        }
    }
    return pPhysSize;

} // findPhysSize

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnit* CUnitsPool::findUnit( const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    CUnit* pUnit = nullptr;

    if( m_hshpUnits.contains(i_strUnitKey) )
    {
        pUnit = m_hshpUnits[i_strUnitKey];
    }
    return pUnit;

} // findUnit

//------------------------------------------------------------------------------
CUnit* CUnitsPool::findUnit( const QString& i_strUnitGrpKey, const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    CUnit*  pUnit = nullptr;
    QString strUnitKey = i_strUnitGrpKey;

    if( !strUnitKey.isEmpty() )
    {
        strUnitKey += m_cNameSeparator;
    }
    strUnitKey += i_strUnitKey;

    if( m_hshpUnits.contains(strUnitKey) )
    {
        pUnit = m_hshpUnits[strUnitKey];
    }
    else // if( !m_hshpUnits.contains(strUnitKey) )
    {
        CUnitGrp* pUnitGrp = findUnitGrp(i_strUnitGrpKey);

        if( pUnitGrp != nullptr && pUnitGrp->isNameSpaceNode() )
        {
            CUnitGrp* pUnitGrpChild;
            int       idxUnitGrpChild;
            QString   strUnitGrpKeyChild;

            for( idxUnitGrpChild = 0; idxUnitGrpChild < pUnitGrp->getChildUnitGrpCount(); idxUnitGrpChild++ )
            {
                pUnitGrpChild = pUnitGrp->getChildUnitGrp(idxUnitGrpChild);

                strUnitGrpKeyChild = pUnitGrpChild->getKey();

                strUnitKey = strUnitGrpKeyChild + m_cNameSeparator + i_strUnitKey;

                if( m_hshpUnits.contains(strUnitKey) )
                {
                    pUnit = m_hshpUnits[strUnitKey];
                    break;
                }
            }
        } // if( pUnitGrp != nullptr && pUnitGrp->isNameSpaceNode() )
    } // if( !m_hshpUnits.contains(strUnitKey) )

    return pUnit;

} // findUnit

//------------------------------------------------------------------------------
CPhysUnit* CUnitsPool::findPhysUnit( const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    CPhysUnit* pPhysUnit = nullptr;

    if( m_hshpUnits.contains(i_strUnitKey) )
    {
        CUnit* pUnit = m_hshpUnits[i_strUnitKey];

        if( pUnit->classType() == EUnitClassTypePhysScienceFields )
        {
            pPhysUnit = dynamic_cast<CPhysUnit*>(pUnit);
        }
    }
    return pPhysUnit;

} // findPhysUnit

//------------------------------------------------------------------------------
CPhysUnit* CUnitsPool::findPhysUnit( const QString& i_strUnitGrpKey, const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    CPhysUnit* pPhysUnit = nullptr;
    QString    strUnitKey = i_strUnitGrpKey;

    if( !strUnitKey.isEmpty() )
    {
        strUnitKey += m_cNameSeparator;
    }
    strUnitKey += i_strUnitKey;

    if( m_hshpUnits.contains(strUnitKey) )
    {
        CUnit* pUnit = m_hshpUnits[strUnitKey];

        if( pUnit->classType() == EUnitClassTypePhysScienceFields )
        {
            pPhysUnit = dynamic_cast<CPhysUnit*>(pUnit);
        }
    }
    else // if( !m_hshpUnits.contains(strUnitKey) )
    {
        CUnitGrp* pUnitGrp = findUnitGrp(i_strUnitGrpKey);

        if( pUnitGrp != nullptr && pUnitGrp->isNameSpaceNode() )
        {
            CUnitGrp* pUnitGrpChild;
            int       idxUnitGrpChild;
            QString   strUnitGrpKeyChild;
            CUnit*    pUnit;

            for( idxUnitGrpChild = 0; idxUnitGrpChild < pUnitGrp->getChildUnitGrpCount(); idxUnitGrpChild++ )
            {
                pUnitGrpChild = pUnitGrp->getChildUnitGrp(idxUnitGrpChild);

                strUnitGrpKeyChild = pUnitGrpChild->getKey();

                strUnitKey = strUnitGrpKeyChild + m_cNameSeparator + i_strUnitKey;

                if( m_hshpUnits.contains(strUnitKey) )
                {
                    pUnit = m_hshpUnits[strUnitKey];

                    if( pUnit->classType() == EUnitClassTypePhysScienceFields )
                    {
                        pPhysUnit = dynamic_cast<CPhysUnit*>(pUnit);
                        break;
                    }
                }
            }
        } // if( pUnitGrp != nullptr && pUnitGrp == m_arpUnitGrpClassTypes[EUnitClassTypePhysScienceFields] )
    } // if( !m_hshpUnits.contains(strUnitKey) )

    return pPhysUnit;

} // findPhysUnit

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CUnitsPool::onUnitGrpCreated( CUnitGrp* i_pUnitGrp )
//------------------------------------------------------------------------------
{
    QString strKey = i_pUnitGrp->getKey();

    if( m_hshpUnitGrps.contains(strKey) )
    {
        QString strAddErrInfo = "A unit group with key " + strKey + " is already existing.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }
    m_hshpUnitGrps.insert(strKey,i_pUnitGrp);

} // onUnitGrpCreated

//------------------------------------------------------------------------------
void CUnitsPool::onUnitGrpDestroyed( const QString& i_strUnitGrpKey )
//------------------------------------------------------------------------------
{
    if( !m_hshpUnitGrps.contains(i_strUnitGrpKey) )
    {
        QString strAddErrInfo = "A unit group with key " + i_strUnitGrpKey + " is not part of the units pool.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
    }
    m_hshpUnitGrps.remove(i_strUnitGrpKey);

} // onUnitGrpDestroyed

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CUnitsPool::onUnitCreated( CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    QString strKey = i_pUnit->getKey();

    if( m_hshpUnits.contains(strKey) )
    {
        QString strAddErrInfo = "A unit with key " + strKey + " is already existing.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }
    m_hshpUnits.insert(strKey,i_pUnit);

} // onUnitCreated

//------------------------------------------------------------------------------
void CUnitsPool::onUnitDestroyed( const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    if( !m_hshpUnits.contains(i_strUnitKey) )
    {
        QString strAddErrInfo = "A unit with key " + i_strUnitKey + " is not part of the units pool.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
    }
    m_hshpUnits.remove(i_strUnitKey);

} // onUnitDestroyed

//------------------------------------------------------------------------------
/*! Returns the number of active references to this instance.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references.
*/
//------------------------------------------------------------------------------
int CUnitsPool::getRefCount() const
{
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Increments the number of active reference to this instance.

    /return Number of active references after increment.
*/
//------------------------------------------------------------------------------
int CUnitsPool::incrementRefCount()
{
    return ++m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Decrements the number of active reference to this instance.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references after decrement.
*/
//------------------------------------------------------------------------------
int CUnitsPool::decrementRefCount()
{
    if( m_iRefCount <= 0)
    {
        throw CException(__FILE__, __LINE__, EResultObjRefCounterIsZero, "CUnitsPool::" + name());
    }
    return --m_iRefCount;
}
