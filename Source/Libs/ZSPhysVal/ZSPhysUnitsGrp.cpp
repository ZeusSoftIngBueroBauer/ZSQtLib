/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSPhysVal/ZSPhysUnitsGrp.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSPhysVal/ZSPhysUnitsPool.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CUnitGrp
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitGrp::CUnitGrp(
    EUnitClassType i_classType,
    const QString& i_strName,
    const QString& i_strKey,
    bool           i_bIsNameSpaceNode,
    CUnitGrp*      i_pUnitGrpParent ) :
//------------------------------------------------------------------------------
    m_classType(i_classType),
    m_strName(i_strName),
    m_strKey(i_strKey),
    m_bIsNameSpaceNode(i_bIsNameSpaceNode),
    m_pUnitGrpParent(nullptr),
    m_vecpUnitGrpChilds(),
    m_hshpUnitGrpChilds(),
    m_vecpUnits(),
    m_hshpUnits()
{
    if( i_pUnitGrpParent != nullptr )
    {
        i_pUnitGrpParent->addChildUnitGrp(this); // sets m_pUnitGrpParent to i_pUnitGrpParent
    }

    CUnitsPool::GetInstance()->onUnitGrpCreated(this);

} // ctor

//------------------------------------------------------------------------------
CUnitGrp::~CUnitGrp()
//------------------------------------------------------------------------------
{
    CUnit* pUnit;
    int    idxUnit;

    if( m_vecpUnits.count() > 0 )
    {
        for( idxUnit = m_vecpUnits.count()-1; idxUnit >= 0; idxUnit-- )
        {
            pUnit = m_vecpUnits[idxUnit];

            removeUnit(idxUnit);

            try
            {
                delete pUnit;
            }
            catch(...)
            {
            }
            pUnit = nullptr;
        }
    }

    // Recursively remove and delete the child groups ..

    CUnitGrp* pUnitGrp;
    int       idxChild;

    if( m_vecpUnitGrpChilds.count() > 0 )
    {
        for( idxChild = m_vecpUnitGrpChilds.count()-1; idxChild >= 0; idxChild-- )
        {
            pUnitGrp = m_vecpUnitGrpChilds[idxChild];

            try
            {
                // The dtor of the unit group also deletes child unit groups or units belonging to the unit group
                // and also calls "removeChildUnitGrp" as a reentry.
                delete pUnitGrp;
            }
            catch(...)
            {
            }
            pUnitGrp = nullptr;
        }
    }

    // Inform parent that the unit group has been destroyed.
    if( m_pUnitGrpParent != nullptr )
    {
        m_pUnitGrpParent->removeChildUnitGrp(m_strName);
    }

    m_pUnitGrpParent = nullptr;

    CUnitsPool::GetInstance()->onUnitGrpDestroyed(m_strKey);

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitGrp::operator == ( const CUnitGrp& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    if( this != &i_physsizeOther )
    {
        return false;
    }
    return true;

} // operator ==

//------------------------------------------------------------------------------
bool CUnitGrp::operator != ( const CUnitGrp& i_physsizeOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physsizeOther );

} // operator ==

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CUnitGrp::classType2Str() const
//------------------------------------------------------------------------------
{
    return unitClassType2Str(m_classType);
}

//------------------------------------------------------------------------------
QString CUnitGrp::getParentGroupName( bool i_bInsertParentNames ) const
//------------------------------------------------------------------------------
{
    QString strName;

    if( m_pUnitGrpParent != nullptr )
    {
        strName = m_pUnitGrpParent->getName();
    }

    if( i_bInsertParentNames && m_pUnitGrpParent != nullptr )
    {
        CUnitGrp* pUnitGrpParent = m_pUnitGrpParent;

        while( pUnitGrpParent != nullptr )
        {
            strName.insert( 0, CUnitsPool::GetNameSeparator() );
            strName.insert( 0, pUnitGrpParent->getName() );

            pUnitGrpParent = pUnitGrpParent->m_pUnitGrpParent;
        }
    }

    return strName;

} // getParentGroupName

//------------------------------------------------------------------------------
QString CUnitGrp::getName( bool i_bInsertParentNames ) const
//------------------------------------------------------------------------------
{
    QString strName = m_strName;

    if( i_bInsertParentNames )
    {
        if( m_pUnitGrpParent != nullptr )
        {
            strName.insert( 0, CUnitsPool::GetNameSeparator() );
            strName.insert( 0, m_pUnitGrpParent->getName(true) );
        }
    }

    return strName;

} // getName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CUnitGrp::addChildUnitGrp( CUnitGrp* i_pUnitGrp )
//------------------------------------------------------------------------------
{
    if( i_pUnitGrp == nullptr )
    {
        QString strAddErrInfo;
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pUnitGroup == nullptr" );
    }
    if( i_pUnitGrp->m_pUnitGrpParent != nullptr )
    {
        QString strAddErrInfo = "Unit Group " + i_pUnitGrp->getName(true) + " already belongs to unit group " + i_pUnitGrp->m_pUnitGrpParent->getName(true);
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }
    if( !m_bIsNameSpaceNode || m_vecpUnits.size() > 0 )
    {
        QString strAddErrInfo = "Cannot add unit groups to groups which are not name space nodes.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }

    // The name got to be unique:
    CUnitGrp* pUnitGrp = findChildUnitGrp(i_pUnitGrp->getName());
    if( pUnitGrp != nullptr )
    {
        QString strAddErrInfo = "A unit group with name " + i_pUnitGrp->getName() + " already belongs to unit group " + getName(true);
        throw ZS::System::CException( __FILE__ ,__LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }

    m_vecpUnitGrpChilds.append(i_pUnitGrp);
    m_hshpUnitGrpChilds.insert( i_pUnitGrp->getName(), i_pUnitGrp );

    i_pUnitGrp->m_pUnitGrpParent = this;

} // addChildUnitGrp

//------------------------------------------------------------------------------
void CUnitGrp::removeChildUnitGrp( int i_idx )
//------------------------------------------------------------------------------
{
    if( m_vecpUnitGrpChilds.count() == 0 )
    {
        QString strAddErrInfo = getName(true) + ".removeChildUnitGrp( " + QString::number(i_idx) + " ): List of child unit groups is empty";
        throw CException( __FILE__, __LINE__, EResultListIsEmpty, strAddErrInfo );
    }

    if( i_idx < 0 || i_idx >= m_vecpUnitGrpChilds.count() )
    {
        QString strAddErrInfo = getName(true) + ".removeChildUnitGrp( " + QString::number(i_idx) + " ): Index is out of range";
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo );
    }

    CUnitGrp* pUnitGrp = m_vecpUnitGrpChilds[i_idx];

    m_vecpUnitGrpChilds.remove(i_idx);
    m_hshpUnitGrpChilds.remove(pUnitGrp->getName());

} // removeChildUnitGrp

//------------------------------------------------------------------------------
void CUnitGrp::removeChildUnitGrp( CUnitGrp* i_pUnitGrp )
//------------------------------------------------------------------------------
{
    if( i_pUnitGrp == nullptr )
    {
        QString strAddErrInfo;
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pUnitGroup == nullptr" );
    }

    int idxChild = -1;

    if( m_vecpUnitGrpChilds.count() > 0 )
    {
        CUnitGrp* pUnitGrpTmp;
        int       idxChildTmp;

        for( idxChildTmp = m_vecpUnitGrpChilds.count()-1; idxChildTmp >= 0; idxChildTmp-- )
        {
            pUnitGrpTmp = m_vecpUnitGrpChilds[idxChildTmp];

            if( i_pUnitGrp == pUnitGrpTmp )
            {
                idxChild = idxChildTmp;
                break;
            }
        }
    }

    if( idxChild < 0 )
    {
        QString strAddErrInfo = "Unit Group " + i_pUnitGrp->getName(true) + " does not belong to unit group " + getName(true);
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
    }

    removeChildUnitGrp(idxChild);

} // removeChildUnitGrp

//------------------------------------------------------------------------------
void CUnitGrp::removeChildUnitGrp( const QString& i_strName )
//------------------------------------------------------------------------------
{
    CUnitGrp* pUnitGrp = findChildUnitGrp(i_strName);

    if( pUnitGrp == nullptr )
    {
        QString strAddErrInfo = "Unit Group " + i_strName + " does not belong to unit group " + getName(true);
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
    }

    removeChildUnitGrp(pUnitGrp);

} // removeChildUnitGrp

//------------------------------------------------------------------------------
CUnitGrp* CUnitGrp::getChildUnitGrp( int i_idx ) const
//------------------------------------------------------------------------------
{
    if( m_vecpUnitGrpChilds.count() == 0 )
    {
        QString strAddErrInfo = getName(true) + ".getChildUnitGrp( " + QString::number(i_idx) + " ): List of child unit groups is empty";
        throw CException( __FILE__, __LINE__, EResultListIsEmpty, strAddErrInfo );
    }

    if( i_idx < 0 || i_idx >= m_vecpUnitGrpChilds.count() )
    {
        QString strAddErrInfo = getName(true) + ".getChildUnitGrp( " + QString::number(i_idx) + " ): Index is out of range";
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo );
    }

    return m_vecpUnitGrpChilds[i_idx];

} // getChildUnitGrp

//------------------------------------------------------------------------------
CUnitGrp* CUnitGrp::findChildUnitGrp( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    CUnitGrp* pUnitGrp = nullptr;

    if( m_hshpUnitGrpChilds.contains(i_strName) )
    {
        pUnitGrp = m_hshpUnitGrpChilds[i_strName];
    }
    return pUnitGrp;

} // findChildUnitGrp

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CUnitGrp::addUnit( CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    if( i_pUnit == nullptr )
    {
        QString strAddErrInfo;
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pUnit == nullptr" );
    }
    if( i_pUnit->m_pUnitGrp != nullptr )
    {
        QString strAddErrInfo = "Unit " + i_pUnit->getName(true) + " already belongs to unit group " + i_pUnit->m_pUnitGrp->getName(true);
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }
    if( m_bIsNameSpaceNode || m_vecpUnitGrpChilds.size() > 0 )
    {
        QString strAddErrInfo = "Cannot add units to name space nodes.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }

    // The name and symbol got to be unique:
    CUnit* pUnit = findUnitByName(i_pUnit->getName());
    if( pUnit != nullptr )
    {
        QString strAddErrInfo = "A unit with name " + i_pUnit->getName() + " already belongs to unit group " + getName(true);
        throw ZS::System::CException( __FILE__ ,__LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }
    pUnit = findUnitBySymbol(i_pUnit->getSymbol());
    if( pUnit != nullptr )
    {
        QString strAddErrInfo = "A unit with symbol " + i_pUnit->getSymbol() + " already belongs to unit group " + getName(true);
        throw ZS::System::CException( __FILE__ ,__LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }

    m_vecpUnits.append(i_pUnit);
    m_hshpUnits.insert( i_pUnit->getSymbol(), i_pUnit );

    i_pUnit->m_pUnitGrp = this;

} // addUnit

//------------------------------------------------------------------------------
void CUnitGrp::removeUnit( int i_idx )
//------------------------------------------------------------------------------
{
    if( m_vecpUnits.count() == 0 )
    {
        QString strAddErrInfo = getName(true) + ".removeUnit( " + QString::number(i_idx) + " ): List of units is empty";
        throw CException( __FILE__, __LINE__, EResultListIsEmpty, strAddErrInfo );
    }

    if( i_idx < 0 || i_idx >= m_vecpUnits.count() )
    {
        QString strAddErrInfo = getName(true) + ".removeUnit( " + QString::number(i_idx) + " ): Index is out of range";
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo );
    }

    CUnit* pUnit = m_vecpUnits[i_idx];

    m_vecpUnits.remove(i_idx);
    m_hshpUnits.remove(pUnit->getSymbol());

} // removeUnit

//------------------------------------------------------------------------------
void CUnitGrp::removeUnit( CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    if( i_pUnit == nullptr )
    {
        QString strAddErrInfo;
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pUnit == nullptr" );
    }

    int idxUnit = -1;

    if( m_vecpUnits.count() > 0 )
    {
        CUnit* pUnitTmp;
        int    idxUnitTmp;

        for( idxUnitTmp = m_vecpUnits.count()-1; idxUnitTmp >= 0; idxUnitTmp-- )
        {
            pUnitTmp = m_vecpUnits[idxUnitTmp];

            if( i_pUnit == pUnitTmp )
            {
                idxUnit = idxUnitTmp;
                break;
            }
        }
    }

    if( idxUnit < 0 )
    {
        QString strAddErrInfo = "Unit " + i_pUnit->getName(true) + " does not belong to unit group " + getName(true);
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
    }

    removeUnit(idxUnit);

} // removeUnit

//------------------------------------------------------------------------------
void CUnitGrp::removeUnit( const QString& i_strSymbolOrName )
//------------------------------------------------------------------------------
{
    CUnit* pUnit = findUnit(i_strSymbolOrName);

    if( pUnit == nullptr )
    {
        QString strAddErrInfo = "Unit " + i_strSymbolOrName + " does not belong to unit group " + getName(true);
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
    }

    removeUnit(pUnit);

} // removeUnit

//------------------------------------------------------------------------------
CUnit* CUnitGrp::getUnit( int i_idx ) const
//------------------------------------------------------------------------------
{
    if( m_vecpUnits.count() == 0 )
    {
        QString strAddErrInfo = getName(true) + ".getUnit( " + QString::number(i_idx) + " ): List of units is empty";
        throw CException( __FILE__, __LINE__, EResultListIsEmpty, strAddErrInfo );
    }

    if( i_idx < 0 || i_idx >= m_vecpUnits.count() )
    {
        QString strAddErrInfo = getName(true) + ".getUnit( " + QString::number(i_idx) + " ): Index is out of range";
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo );
    }

    return m_vecpUnits[i_idx];

} // getUnit

//------------------------------------------------------------------------------
CUnit* CUnitGrp::findUnit( const QString& i_strSymbolOrName ) const
//------------------------------------------------------------------------------
{
    CUnit* pUnit = findUnitBySymbol(i_strSymbolOrName);
    CUnit* pUnitTmp;
    int    idxUnitTmp;

    // Not found by symbol ..
    if( pUnit == nullptr )
    {
        for( idxUnitTmp = 0; idxUnitTmp < m_vecpUnits.count(); idxUnitTmp++ )
        {
            pUnitTmp = m_vecpUnits[idxUnitTmp];

            if( i_strSymbolOrName.compare(pUnitTmp->getName(),Qt::CaseInsensitive) == 0 )
            {
                pUnit = pUnitTmp;
                break;
            }
        }
    }

    // Not found by name or symbol of unit ..
    if( pUnit == nullptr )
    {
        // .. maybe we can find the prefix. We can't search for the prefix before we checked all
        // "complete" unit names as e.g. "m" means "Meter" but not "mm".
        CUnitGrp*  pVThis    = const_cast<CUnitGrp*>(this);
        CPhysSize* pPhysSize = dynamic_cast<CPhysSize*>(pVThis);

        if( pPhysSize != nullptr )
        {
            CPhysUnit* pPhysUnitTmp;

            for( idxUnitTmp = 0; idxUnitTmp < m_vecpUnits.count(); idxUnitTmp++ )
            {
                pPhysUnitTmp = dynamic_cast<CPhysUnit*>(m_vecpUnits[idxUnitTmp]);

                if( i_strSymbolOrName.compare(pPhysUnitTmp->getPrefixStr(),Qt::CaseSensitive) == 0 ) // Symbols (and prefix) Case Sensitive (e.g. mV, MV)
                {
                    pUnit = pPhysUnitTmp;
                    break;
                }
            }
        } // if( pPhysSize != nullptr )
    } // if( pUnit == nullptr )

    return pUnit;

} // findUnit

//------------------------------------------------------------------------------
CUnit* CUnitGrp::findUnitBySymbol( const QString& i_strSymbol ) const
//------------------------------------------------------------------------------
{
    CUnit* pUnit = nullptr;

    if( m_hshpUnits.contains(i_strSymbol) )
    {
        pUnit = m_hshpUnits[i_strSymbol];
    }
    return pUnit;

} // findUnitBySymbol

//------------------------------------------------------------------------------
CUnit* CUnitGrp::findUnitByName( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    CUnit* pUnit = nullptr;
    CUnit* pUnitTmp;
    int    idxUnitTmp;

    for( idxUnitTmp = 0; idxUnitTmp < m_vecpUnits.count(); idxUnitTmp++ )
    {
        pUnitTmp = m_vecpUnits[idxUnitTmp];

        if( i_strName.compare(pUnitTmp->getName(),Qt::CaseInsensitive) == 0 )
        {
            pUnit = pUnitTmp;
            break;
        }
    }
    return pUnit;

} // findUnitByName
