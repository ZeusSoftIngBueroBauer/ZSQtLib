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

CUnitsPool* CUnitsPool::s_pInstance = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QChar CUnitsPool::GetNameSeparator()
//------------------------------------------------------------------------------
{
    if( s_pInstance == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pInstance->m_cNameSeparator;
}

//------------------------------------------------------------------------------
void CUnitsPool::SetNameSeparator( QChar i_cSeparator )
//------------------------------------------------------------------------------
{
    if( s_pInstance == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    s_pInstance->m_cNameSeparator = i_cSeparator;
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnitGrp* CUnitsPool::GetUnitClassTypeGroup( EUnitClassType i_classType )
//------------------------------------------------------------------------------
{
    if( s_pInstance == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pInstance->m_arpUnitGrpClassTypes[i_classType];
}

//------------------------------------------------------------------------------
CUnitGrp* CUnitsPool::GetPhysScienceFieldUnitGroup( EPhysScienceField i_scienceField )
//------------------------------------------------------------------------------
{
    if( s_pInstance == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pInstance->m_arpUnitGrpPhysScienceFields[i_scienceField];
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnitGrp* CUnitsPool::FindUnitGrp( const QString& i_strUnitGrpKey )
//------------------------------------------------------------------------------
{
    if( s_pInstance == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }

    CUnitGrp* pUnitGrp = nullptr;

    if( s_pInstance->m_hshpUnitGrps.contains(i_strUnitGrpKey) )
    {
        pUnitGrp = s_pInstance->m_hshpUnitGrps[i_strUnitGrpKey];
    }
    return pUnitGrp;

} // findUnitGrp

//------------------------------------------------------------------------------
CPhysSize* CUnitsPool::FindPhysSize( const QString& i_strUnitGrpKey )
//------------------------------------------------------------------------------
{
    if( s_pInstance == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }

    CPhysSize* pPhysSize = nullptr;

    if( s_pInstance->m_hshpUnitGrps.contains(i_strUnitGrpKey) )
    {
        CUnitGrp* pUnitGrp = s_pInstance->m_hshpUnitGrps[i_strUnitGrpKey];

        if( pUnitGrp->classType() == EUnitClassTypePhysScienceFields )
        {
            pPhysSize = dynamic_cast<CPhysSize*>(pUnitGrp);
        }
    }
    return pPhysSize;

} // findPhysSize

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnit* CUnitsPool::FindUnit( const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    if( s_pInstance == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }

    CUnit* pUnit = nullptr;

    if( s_pInstance->m_hshpUnits.contains(i_strUnitKey) )
    {
        pUnit = s_pInstance->m_hshpUnits[i_strUnitKey];
    }
    return pUnit;

} // findUnit

//------------------------------------------------------------------------------
CUnit* CUnitsPool::FindUnit( const QString& i_strUnitGrpKey, const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    if( s_pInstance == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }

    CUnit*  pUnit = nullptr;
    QString strUnitKey = i_strUnitGrpKey;

    if( !strUnitKey.isEmpty() )
    {
        strUnitKey += s_pInstance->m_cNameSeparator;
    }
    strUnitKey += i_strUnitKey;

    if( s_pInstance->m_hshpUnits.contains(strUnitKey) )
    {
        pUnit = s_pInstance->m_hshpUnits[strUnitKey];
    }
    else // if( !s_pInstance->m_hshpUnits.contains(strUnitKey) )
    {
        CUnitGrp* pUnitGrp = FindUnitGrp(i_strUnitGrpKey);

        if( pUnitGrp != nullptr && pUnitGrp->isNameSpaceNode() )
        {
            CUnitGrp* pUnitGrpChild;
            int       idxUnitGrpChild;
            QString   strUnitGrpKeyChild;

            for( idxUnitGrpChild = 0; idxUnitGrpChild < pUnitGrp->getChildUnitGrpCount(); idxUnitGrpChild++ )
            {
                pUnitGrpChild = pUnitGrp->getChildUnitGrp(idxUnitGrpChild);

                strUnitGrpKeyChild = pUnitGrpChild->getKey();

                strUnitKey = strUnitGrpKeyChild + s_pInstance->m_cNameSeparator + i_strUnitKey;

                if( s_pInstance->m_hshpUnits.contains(strUnitKey) )
                {
                    pUnit = s_pInstance->m_hshpUnits[strUnitKey];
                    break;
                }
            }
        } // if( pUnitGrp != nullptr && pUnitGrp->isNameSpaceNode() )
    } // if( !s_pInstance->m_hshpUnits.contains(strUnitKey) )

    return pUnit;

} // findUnit

//------------------------------------------------------------------------------
CPhysUnit* CUnitsPool::FindPhysUnit( const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    if( s_pInstance == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }

    CPhysUnit* pPhysUnit = nullptr;

    if( s_pInstance->m_hshpUnits.contains(i_strUnitKey) )
    {
        CUnit* pUnit = s_pInstance->m_hshpUnits[i_strUnitKey];

        if( pUnit->classType() == EUnitClassTypePhysScienceFields )
        {
            pPhysUnit = dynamic_cast<CPhysUnit*>(pUnit);
        }
    }
    return pPhysUnit;

} // findPhysUnit

//------------------------------------------------------------------------------
CPhysUnit* CUnitsPool::FindPhysUnit( const QString& i_strUnitGrpKey, const QString& i_strUnitKey )
//------------------------------------------------------------------------------
{
    if( s_pInstance == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }

    CPhysUnit* pPhysUnit = nullptr;
    QString    strUnitKey = i_strUnitGrpKey;

    if( !strUnitKey.isEmpty() )
    {
        strUnitKey += s_pInstance->m_cNameSeparator;
    }
    strUnitKey += i_strUnitKey;

    if( s_pInstance->m_hshpUnits.contains(strUnitKey) )
    {
        CUnit* pUnit = s_pInstance->m_hshpUnits[strUnitKey];

        if( pUnit->classType() == EUnitClassTypePhysScienceFields )
        {
            pPhysUnit = dynamic_cast<CPhysUnit*>(pUnit);
        }
    }
    else // if( !s_pInstance->m_hshpUnits.contains(strUnitKey) )
    {
        CUnitGrp* pUnitGrp = FindUnitGrp(i_strUnitGrpKey);

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

                strUnitKey = strUnitGrpKeyChild + s_pInstance->m_cNameSeparator + i_strUnitKey;

                if( s_pInstance->m_hshpUnits.contains(strUnitKey) )
                {
                    pUnit = s_pInstance->m_hshpUnits[strUnitKey];

                    if( pUnit->classType() == EUnitClassTypePhysScienceFields )
                    {
                        pPhysUnit = dynamic_cast<CPhysUnit*>(pUnit);
                        break;
                    }
                }
            }
        } // if( pUnitGrp != nullptr && pUnitGrp == m_arpUnitGrpClassTypes[EUnitClassTypePhysScienceFields] )
    } // if( !s_pInstance->m_hshpUnits.contains(strUnitKey) )

    return pPhysUnit;

} // findPhysUnit

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitsPool::CUnitsPool() :
//------------------------------------------------------------------------------
    m_cNameSeparator('.'),
    m_hshpUnitGrps(),
    m_hshpUnits()
    //m_arpUnitGrpClassTypes[EUnitClassTypeCount]
    //m_arpUnitGrpPhysScienceFields[EPhysScienceFieldCount]
{
    if( s_pInstance != nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated );
    }
    s_pInstance = this;

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
            m_arpUnitGrpClassTypes[idxClassType] = new CUnitGrpRatio();
        }
        else if( classType == EUnitClassTypeDataQuantity )
        {
            m_arpUnitGrpClassTypes[idxClassType] = new CUnitGrpDataQuantity();
        }
        else if( classType == EUnitClassTypePhysScienceFields )
        {
            m_arpUnitGrpClassTypes[idxClassType] = new CUnitGrp(
                /* classType        */ classType,
                /* strName          */ unitClassType2Str(classType),
                /* strKey           */ unitClassType2Str(classType),
                /* bIsNameSpaceNode */ true,
                /* pUnitGrpParent   */ nullptr );

            for( idxScienceField = 0; idxScienceField < EPhysScienceFieldCount; idxScienceField++ )
            {
                scienceField = static_cast<EPhysScienceField>(idxScienceField);

                m_arpUnitGrpPhysScienceFields[idxScienceField] = new CUnitGrp(
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

    s_pInstance = nullptr;

} // dtor

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

